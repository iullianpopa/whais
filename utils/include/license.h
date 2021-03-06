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

#ifndef LICENSE_H_
#define LICENSE_H_


#include "whais.h"


#define xstringify(x) stringify(x)
#define stringify(x) #x

#ifndef __cplusplus


void
showBanner(const char* programName, uint_t verMajor, uint_t verMinor);

void
showLicenseInformation(const char* programName, const char* programDescription);


#else


#include <iostream>

void
displayBanner(std::ostream& os,
              const char* const programName,
              const uint_t verMajor,
              const uint_t verMinor);

void
displayLicenseInformation(std::ostream& os,
                          const char* const programName,
                          const char* const programDescription);

#endif /* __cplusplus */
#endif /* LICENSE_H_ */
