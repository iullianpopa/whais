/******************************************************************************
WHAIS - An advanced database system
Copyright(C) 2014-2018  Iulian Popa

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

#include <sys/time.h>
#include <time.h>
#include <assert.h>

#include "whais.h"


WTime
wh_get_currtime()
{
  WTime          result;
  struct tm      timeUtc;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  localtime_r(&tv.tv_sec, &timeUtc);

  result.year  = timeUtc.tm_year + 1900;
  result.month = timeUtc.tm_mon  + 1;
  result.day   = timeUtc.tm_mday;
  result.hour  = timeUtc.tm_hour;
  result.min   = timeUtc.tm_min;
  result.sec   = timeUtc.tm_sec;
  result.usec  = tv.tv_usec;

  return result;
}


WTICKS
wh_msec_ticks()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return((WTICKS)tv.tv_sec * 1000ul) + (WTICKS)(tv.tv_usec / 1000);
}

