/******************************************************************************
WHISPERC - A compiler for whisper programs
Copyright (C) 2009  Iulian Popa

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
#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>

#include "wthread.h"

enum LOG_TYPE
{
  LOG_UNKNOW,
  LOG_CRITICAL,
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG
};

class Logger
{
public:
  Logger (const D_CHAR* const pFile, const bool printStart = true);

  void Log (const LOG_TYPE type, const D_CHAR* pStr);
  void Log (const LOG_TYPE type, const std::string& str);

private:
  Logger (const Logger&);
  Logger& operator= (const Logger&);

  D_UINT PrintTimeMark (LOG_TYPE type, WTICKS ticks);

  WTICKS        m_StartTick;
  WSynchronizer m_Sync;
  std::ofstream m_OutStream;
};



#endif /* LOGGER_H_ */