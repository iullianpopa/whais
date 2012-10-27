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
/* Not exactly POSIX, but we can leave with it. */
#define _GNU_SOURCE
#endif

#include <assert.h>
#include "whisper.h"

WTime
wh_get_currtime ()
{
  WTime result;
  SYSTEMTIME localTime;

  GetLocalTime (&localTime);

  result.year  = localTime.wYear;
  result.month = localTime.wMonth - 1;
  result.day   = localTime.wDay - 1;
  result.hour  = localTime.wHour;
  result.min   = localTime.wMinute;
  result.sec   = localTime.wSecond;

  return result;
}

WTICKS
wh_msec_ticks ()
{
  return GetTickCount64 ();
}




