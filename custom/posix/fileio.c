/******************************************************************************
WHISPER - An advanced database system
Copyright (C) 2008  Iulian Popa

Address: Str Olimp nr. 6
         Pantelimon Ilfov,
         Romania
Phone:   +40721939650
e-mail:  popaiulian@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE		/* for old GNU libc header files */
#endif

#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include "whisper.h"
#include "whisper_fileio.h"

#define POSIX_FAIL_RET        (~0)	/* -1 */


WH_FILE_HND
whc_fopen (const D_CHAR * fname, D_UINT mode)
{
  int open_mode = O_LARGEFILE;
  const int mode_t = (S_IRUSR | S_IWUSR | S_IRGRP);
  WH_FILE_HND result = -1;

  if (mode & WHC_FILECREATE_NEW)
    open_mode |= O_CREAT | O_EXCL;
  else if (mode & WHC_FILECREATE)
    open_mode |= O_CREAT;
  else
    open_mode |= 0; /* WHC_FILEOPEN_EXISTING */

  open_mode |= (mode & WHC_FILEDIRECT) ? O_DIRECT : 0;
  open_mode |= (mode & WHC_FILESYNC) ? O_SYNC : 0;

  if ((mode & WHC_FILERDWR) == WHC_FILERDWR)
    open_mode |= O_RDWR;
  else
    {
      open_mode |= (mode & WHC_FILEREAD) ? O_RDONLY : 0;
      open_mode |= (mode & WHC_FILEWRITE) ? O_WRONLY : 0;
    }

  result =  open (fname, open_mode, mode_t);
  return (result < 0) ? 0 : result;
}

WH_FILE_HND
whc_fdup (WH_FILE_HND f_hnd)
{
  WH_FILE_HND result = dup (f_hnd);
  return (result < 0) ? 0 : result;
}

D_BOOL
whc_fseek (WH_FILE_HND f_hnd, D_INT64 where, D_INT whence)
{
  switch (whence)
    {
    case WHC_SEEK_BEGIN:
      whence = SEEK_SET;
      break;
    case WHC_SEEK_END:
      whence = SEEK_END;
      break;
    case WHC_SEEK_CURR:
      whence = SEEK_CUR;
      break;
    default:
      assert (0);
    }
  return (lseek64 (f_hnd, where, whence) != POSIX_FAIL_RET);
}

D_BOOL
whc_fread (WH_FILE_HND f_hnd, D_UINT8 * buffer, D_UINT size)
{
  D_BOOL result = TRUE;
  D_UINT actual_count = 0;

  while (actual_count < size)
    {
      D_UINT count = read (f_hnd, buffer + actual_count,
                           size - actual_count);
      if (count == POSIX_FAIL_RET)
        {
          /* the errno is already set for this */
          result = FALSE;
          break;
        }
      else if (count == 0)
        {
          /* Reading past the end of file is not considered an
           * error in POSIX, but we do! */
          errno = ENODATA;
          result = FALSE;
          break;
        }
      actual_count += count;
    }

  assert ((result == TRUE) || (actual_count < size));
  assert ((result == FALSE) || (size == actual_count));
  return result;
}

D_BOOL
whc_fwrite (WH_FILE_HND f_hnd, const D_UINT8 * buffer, D_UINT size)
{
  D_BOOL result = TRUE;
  D_UINT actual_count = 0;

  while (actual_count < size)
    {
      D_UINT count = write (f_hnd, buffer + actual_count,
                            size - actual_count);
      if (count == POSIX_FAIL_RET)
        {
          /* the errno is already set for this */
          result = FALSE;
          break;
        }
      actual_count += count;
    }

  assert ((result == TRUE) || (actual_count < size));
  assert ((result == FALSE) || (size == actual_count));
  return result;
}

D_BOOL
whc_ftell (WH_FILE_HND f_hnd, D_UINT64 * output)
{
  *output = lseek64 (f_hnd, 0, SEEK_CUR);

  return (*output != POSIX_FAIL_RET);
}

D_BOOL
whc_fsync (WH_FILE_HND f_hnd)
{
  return (fsync (f_hnd) != POSIX_FAIL_RET);
}

D_BOOL
whc_ftellsize (WH_FILE_HND f_hnd, D_UINT64 * output)
{
  struct stat64 buf;

  if (fstat64 (f_hnd, &buf) == POSIX_FAIL_RET)
    return FALSE;

  *output = buf.st_size;
  return TRUE;
}

D_BOOL
whc_fsetsize (WH_FILE_HND f_hnd, D_UINT64 size)
{
  if (ftruncate (f_hnd, size) != 0)
    return FALSE;
  return TRUE;
}

D_BOOL
whc_fclose (WH_FILE_HND f_hnd)
{
  return (close (f_hnd) != POSIX_FAIL_RET);
}

D_UINT32
whc_fgetlasterror ()
{
  /* POSIX.1c enforce this to be thread safe */
  return errno;
}

D_BOOL
whc_ferrtostrs (D_UINT64 error_code, D_CHAR * str, D_UINT str_size)
{
  return (strerror_r ((int) error_code, str, str_size) != NULL);
}

D_BOOL
whc_fremove (const D_CHAR * fname)
{
  if (unlink (fname) == 0)
    return TRUE;
  /* else */
  return FALSE;
}

const D_CHAR* whc_get_directory_delimiter ()
{
  return "/";
}