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

#include <assert.h>
#include <stdio.h>

#include "utils/wsocket.h"



namespace whais {



Socket::Socket(const char* const serverHost, const char* const service)
    : mSocket(INVALID_SOCKET)
{
  const uint32_t e = whs_create_client(serverHost, service, &mSocket);
  if (e != WOP_OK)
    throw SocketException(_EXTRA(e), "Could not connect to '%s:%s'.", serverHost, service);

  assert(mSocket != INVALID_SOCKET);
}


Socket::Socket(const char* const serverHost, const uint16_t port)
    : mSocket(INVALID_SOCKET)
{
  uint32_t e;
  char service[16];

  sprintf(service, "%u", port);

  e = whs_create_client(serverHost, service, &mSocket);
  if (e != WOP_OK)
  {
    throw SocketException(_EXTRA(e),
                          "Could not connect to '%s:%u'.",
                          serverHost,
                          _SC(uint_t, port));
  }
  assert(mSocket != INVALID_SOCKET);
}


Socket::Socket(const char* const localAdress, const char* const service, const uint_t backLog)
    : mSocket(INVALID_SOCKET)
{
  uint32_t e = whs_create_server(localAdress, service, backLog, &mSocket);
  if (e != WOP_OK)
    throw SocketException(_EXTRA(e), "Could not start listen at '%s@%s'.", localAdress, service);

  assert(mSocket != INVALID_SOCKET);
}


Socket::Socket(const char* const localAdress, const uint16_t port, const uint_t backLog)
    : mSocket(INVALID_SOCKET)
{
  char   service[16];

  sprintf(service, "%u", port);

  uint32_t e = whs_create_server(localAdress, service, backLog, &mSocket);
  if (e != WOP_OK)
    {
      throw SocketException(_EXTRA(e),
                             "Could not connect to '%s:%u'.",
                             localAdress,
                             _SC(uint_t, port));
    }

  assert(mSocket != INVALID_SOCKET);
}


Socket::Socket(const WH_SOCKET sd)
    : mSocket(sd)
{
}

Socket::Socket(Socket&& src)
    : mSocket(src.mSocket)
{
  src.mSocket = INVALID_SOCKET;
}

Socket::~Socket()
{
  if (mSocket != INVALID_SOCKET)
    whs_close(mSocket);
}

Socket&
Socket::operator=(Socket&& src)
{
  if (this != &src)
  {
    if (mSocket != INVALID_SOCKET)
      whs_close(mSocket);

    mSocket = src.mSocket;
    src.mSocket = INVALID_SOCKET;
  }

  return *this;
}


Socket
Socket::Accept()
{
  WH_SOCKET client = INVALID_SOCKET;
  const uint32_t e = whs_accept(mSocket, &client);

  if (e != WOP_OK)
    throw SocketException(_EXTRA(e), "Socket(%d) failed to accept connection.", mSocket);

  assert(client != INVALID_SOCKET);

  return Socket(client);
}


uint_t
Socket::Read(uint8_t* const buffer, const uint_t maxCount)
{
  if (mSocket == INVALID_SOCKET)
    throw SocketException(_EXTRA(WOP_UNKNOW), "Invalid socket used to read.");

  uint_t result = maxCount;
  const uint32_t e = whs_read(mSocket, buffer, &result);

  if (e != WOP_OK)
    throw SocketException(_EXTRA(e), "Failed to read from socket(%d).", mSocket);

  return result;
}


void
Socket::Write(const uint8_t* const buffer, const uint_t count)
{
  if (mSocket == INVALID_SOCKET)
    throw SocketException(_EXTRA(WOP_UNKNOW), "Invalid socket used to write.");

  const uint32_t e = whs_write(mSocket, buffer, count);

  if (e != WOP_OK)
    throw SocketException(_EXTRA(e), "Failed to write on socket(%d).", mSocket);
}


void
Socket::Close()
{
  if (mSocket == INVALID_SOCKET)
    return;

  whs_close(mSocket);
  mSocket = INVALID_SOCKET;
}

} //namespace whais

