/******************************************************************************
  WCMD - An utility to manage whisper database files.
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

#include <assert.h>
#include <string>
#include <iostream>
#include <vector>

#include "whisper.h"

#include "client/include/whisper_connector.h"

#include "wcmd_onlinecmds.h"
#include "wcmd_cmdsmgr.h"
#include "wcmd_optglbs.h"

using namespace std;

const D_CHAR*
translate_status (CONNECTOR_STATUS cs)
{
  switch (cs)
  {
  case CS_OK:
    return "No error returned.";
  case CS_INVALID_ARGS:
    return "Invalid arguments.";
  case CS_OP_NOTPERMITED:
    return "Operation not permited.";
  case CS_DROPPED:
    return "Connection dropped by perr.";
  case CS_ENCTYPE_NOTSUPP:
    return "Could not agree on supported encrytion type.";
  case CS_UNEXPECTED_FRAME:
    return "Unexpected communication frame received.";
  case CS_COMM_OUT_OF_SYNC:
    return "Communication is out of sync.";
  case CS_LARGE_ARGS:
    return "Size of the operation arguments is not supported.";
  case CS_CONNECTION_TIMEOUT:
    return "Connection has timeout.";
  case CS_SERVER_BUSY:
    return "Server is too busy.";
  case CS_OS_INTERNAL:
    return "OS internal error encountered.";
  }

  return "Unknown error.";
}

static const D_CHAR globalShowDesc[]    = "List context database's "
                                         "global variables.";
static const D_CHAR globalShowDescExt[] =
"Show the global variables installed in the database context.\n"
"If a name is provided it limits the listing to only those variables.\n"
"Usage:\n"
"  global [variable_name] ... ";

static bool
cmdGlobalList (const string& cmdLine, ENTRY_CMD_CONTEXT context)
{

  const  VERBOSE_LEVEL level   = GetVerbosityLevel ();
  CONNECTOR_HDL conHdl = NULL;
  CONNECTOR_STATUS cs  = Connect (GetRemoteHostName ().c_str (),
                                  GetConnectionPort ().c_str (),
                                  GetWorkingDB ().c_str (),
                                  GetUserPassword ().c_str (),
                                  GetUserId (),
                                  &conHdl);
  if (cs != CS_OK)
    {
      if (level >= VL_INFO)
        cout << "Failed to connect: " << translate_status (cs) << endl;

      return false;
    }

  Close (conHdl);

  return true;
}


void
AddOnlineTableCommands ()
{

  CmdEntry entry;

  entry.m_showStatus   = true;
  entry.m_pCmdText     = "global";
  entry.m_pCmdDesc     = globalShowDesc;
  entry.m_pExtHelpDesc = globalShowDescExt;
  entry.m_cmd          = cmdGlobalList;

  RegisterCommand (entry);
}
