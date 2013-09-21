/******************************************************************************
 PASTRA - A light database one file system and more.
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
 *****************************************************************************/

#ifndef PS_CONTAINER_H_
#define PS_CONTAINER_H_

#include <vector>
#include <string>

#include "utils/wfile.h"
#include "utils/wthread.h"
#include "dbs/dbs_values.h"

namespace whisper {
namespace pastra {

static const uint_t DEFAULT_TEMP_MEM_RESERVED = 4096; //4KB

void
append_int_to_str (std::string& dest, uint64_t number);


class DataContainerException : public Exception
{
public:
  DataContainerException (const char*   message,
                          const char*   file,
                          uint32_t      line,
                          uint32_t      extra)
    : Exception (message, file, line, extra)
  {
  }
};



class IDataContainer
{
public:
  IDataContainer ()
  {
  }

  virtual ~IDataContainer ();

  virtual void Write (uint64_t to, uint64_t size, const uint8_t* buffer) = 0;

  virtual void Read (uint64_t from, uint64_t size, uint8_t* buffer) = 0;

  virtual void Colapse (uint64_t from, uint64_t to) = 0;

  virtual uint64_t Size () const = 0;

  virtual void MarkForRemoval () = 0;
};



class FileContainer : public IDataContainer
{
public:
  FileContainer (const char*       baseFile,
                 const uint64_t    maxFileSize,
                 const uint64_t    unitsCount);

  virtual ~FileContainer ();

  virtual void Write (uint64_t to, uint64_t size, const uint8_t* buffer);

  virtual void Read (uint64_t from, uint64_t size, uint8_t* buffer);

  virtual void Colapse (uint64_t from, uint64_t to);

  virtual uint64_t Size () const;

  virtual void MarkForRemoval ();

private:
  void ExtendContainer ();

  const uint64_t       mMaxFileUnitSize;
  std::vector<File>    mFilesHandles;
  std::string          mFileNamePrefix;
  bool                 mToRemove;
};



class TemporalFileContainer : public FileContainer
{
public:
  TemporalFileContainer (const char*    baseName,
                         const uint32_t maxFileSize);
};



class TemporalContainer : public IDataContainer
{
public:
  explicit TemporalContainer (
                        const uint_t reservedMemory = DEFAULT_TEMP_MEM_RESERVED
                             );

  virtual void Write (uint64_t to, uint64_t size, const uint8_t* buffer);

  virtual void Read (uint64_t from, uint64_t size, uint8_t* buffer);

  virtual void Colapse (uint64_t from, uint64_t to);

  virtual uint64_t Size () const;

  virtual void MarkForRemoval ();

private:
  void  FillCache (uint64_t position);

  std::auto_ptr<TemporalFileContainer> mFileContainer;
  std::auto_ptr<uint8_t>               mCache;
  uint64_t                             mCacheStartPos;
  uint64_t                             mCacheEndPos;
  const uint_t                         mCacheSize;
  bool                                 mDirtyCache;

  static uint64_t      smTemporalsCount;
  static Lock smSync;
};



class WFileContainerException : public DataContainerException
{
public:
  explicit
  WFileContainerException (const char*   message,
                           const char*   file,
                           uint32_t      line,
                           uint32_t      extra)
    : DataContainerException (message, file, line, extra)
  {
  }

  virtual Exception* Clone () const;

  virtual EXCEPTION_TYPE Type () const;

  virtual const char*   Description () const;

  enum
  {
    INVALID_PARAMETERS = 1,
    CONTAINTER_INVALID,
    INVALID_ACCESS_POSITION,
    FILE_OS_IO_ERROR
  };
};

} //namespace pastra
} //namespace whisper

#endif /* PS_CONTAINER_H_ */
