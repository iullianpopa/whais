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

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <signal.h>

#include "utils/enc_des.h"
#include "utils/license.h"
#include "dbs/dbs_mgr.h"
#include "client/whais_connector.h"

#include "wcmd_optglbs.h"
#include "wcmd_cmdsmgr.h"
#include "wcmd_tabcomds.h"
#include "wcmd_onlinecmds.h"
#include "wcmd_dbcheck.h"

using namespace std;
using namespace whais;

static const char sProgramName[] = "Whais Commander";
static const char sProgramDesc[] = "A database maintenance tool.";

static const char usageDescription[] =
"Usage: wcmd --create db_name [OPTIONS]\n"
"   or: wcmd --remove db_name [OPTIONS]\n"
"   or: wcmd --use db_name [OPTIONS]\n"
"   or: wcmd --use db_name [OPTIONS] [-s 'script']\n"
"   or: wcmd --help\n"
"   or: wcmd --license\n"
"An whais database directory consists of multiples files. This is used to\n"
"manage the content of a database, performing operations that ranges from\n"
"database creation to updating different fields of selected tables. If there\n"
"is no execution script provided at a command line, then it executes\n"
"interactively by reading the command lines from the standard input. For\n"
"more information about the script command lines type 'help' in interactive\n"
"mode.\n"
"\n"
"Options:\n"
"    -h, --help             Prints this text.\n"
"\n"
"    -c, --create db_name   Create a database named 'db_name'.\n"
"    -r, --remove db_name   Remove the database named 'db_name'.\n"
"    -u, --use db_name      Select 'db_name' as the target database.\n"
"\n"
"    -H, --host hostname    The host name for remote database access.\n"
"    -P, --port portnum     The port number where the to connect.\n"
"    -A, --admin            Connect to a remote database as administrator.\n"
"    -p, --pass password    The password used to authenticate.\n"
"\n"
"    -d, --dir directory    Sets the working directory.\n"
"    -o, --dbdir directory  Sets the directory where the DB content resides.\n"
"    -m, --file_size size   Specify the maximum file size. This is used to\n"
"                           cope with the maximum file size supported by the\n"
"                           file system hosting the database files. It also\n"
"                           support the size modifiers:\n"
"                            k,K : for Kilobytes.\n"
"                            m,M : for Megabytes.\n"
"                            g,G : for Gigabytes.\n"
"                           Default value for this is 2G(e.g. 2 Gigabytes),\n"
"                           and the minimum is 1M(e.g. one Megabyte).\n"
"    -t, --validate         Check the database integrity.\n"
"    -f, --auto_yes         Auto answer with 'yes' to all questions related\n"
"                           to database validation.\n"
"\n"
"    -G, --globals          Generate the declaration of external globals\n"
"                           variables for all tables found in the database.\n"
"\n"
"    --verbose level        Set the verbosity level. Level values:\n"
"                            0: No out put.\n"
"                            1: Print the status of the executed command.\n"
"                            2: Print the error messages in addition.\n"
"                            3: Print the warning messages in addition.\n"
"                            4: Print the various information in addition.\n"
"                            5: Print the debug information in addition.\n"
"                           Default values is 2.\n"
"\n"
"    -s, --script file      Execute the commands from the supplied file,\n"
"                           rather to execute interactively.\n"
"    -S, --cmd script       Execute a script provided by parameter 'script'.\n"
"\n"
"    --nologo               Do not display startup information.\n"
"    -l, --license          Prints license terms.\n"
"\n"
"Examples:\n"
"    wcmd --create some_database -m 4G\n"
" or:\n"
"    wcmd --use some_database -d /database/path -s 'list tables'\n"
"    wcmd -u remote_database -H localhost -P 1761 -U 0 -p 'Some password'\n";


static const char descExit[]    = "Exit this program.";
static const char descExtExit[] = "Exit this program.\n"
                                    "Usage:\n"
                                    "  quit";
static bool sFinishInteraction = false;

#ifndef ARCH_WINDOWS_VC

static void
sigterm_hdl(int sig, siginfo_t *siginfo, void *context)
{
  if (sig == SIGINT)
    {
      cout << endl;
      sFinishInteraction = true;
    }
  else if (sig == SIGTERM)
    {
      cout << "\nTermination request received!\n";
      sFinishInteraction = true;
    }
}


static bool
set_signals()
{
  struct sigaction action;

  memset(&action, 0, sizeof action);
  action.sa_flags     = SA_SIGINFO;
  action.sa_sigaction = &sigterm_hdl;

 if (sigaction(SIGINT, &action, nullptr) < 0)
   return false;

 if (sigaction(SIGTERM, &action, nullptr) < 0)
   return false;

 return true;
}

#else

static BOOL WINAPI
ServerStopHandler(DWORD)
{
  sFinishInteraction = true;
  cout << endl;

  return TRUE;
}

static BOOL
set_signals()
{
  return SetConsoleCtrlHandler(ServerStopHandler, TRUE);
}

#endif


static void
PrintHelpUsage()
{
  displayBanner(cout, sProgramName, WVER_MAJ, WVER_MIN);
  cout << usageDescription;
}


static void
PrintWrongUsage(const char* const arg)
{
  displayBanner(cout, sProgramName, WVER_MAJ, WVER_MIN);

  if (arg != nullptr)
    cerr << "Cannot handle argument '" << arg << "' correctly. Use --help!\n";

  else
    cerr << "Invalid arguments. Use --help!\n";
}


static bool
ExecuteCommandLine(const string& cmdLine)
{
  assert(cmdLine.length() > 0);

  static const string spaces = " \t";

  if (sFinishInteraction)
    return false;

  size_t firstPos = cmdLine.find_first_not_of(spaces);
  size_t lastPos  = cmdLine.find_last_not_of(spaces);

  if (lastPos == 0)
    lastPos = cmdLine.length() - 1;

  if (firstPos == lastPos)
    return true;

  const string normalizeCmd = cmdLine.substr(firstPos,
                                              lastPos - firstPos + 1);

  assert(normalizeCmd != "");

  size_t          pos     = 0;
  const string    command = CmdLineNextToken(normalizeCmd, pos);
  const CmdEntry* cmd     = FindCmdEntry(command.c_str());

  if (cmd == nullptr)
    {
      cerr << "Invalid command '" << command << "'." <<  endl;
      return false;
    }

  const bool cmdResult = cmd->mCmd(normalizeCmd, cmd->mContext);

  if (cmd->mShowStatus && (GetVerbosityLevel() > VL_STATUS))
    cout << command << " : " << (cmdResult ? "OK" : "FAIL") << endl;

  return cmdResult;
}


static bool
ExecuteCommandStmt(const string& cmdStmt)
{
  size_t lastPos    = 0;
  size_t currentPos = 0;
  char   commandSep = ';';
  bool   result     = true;
  bool   armedSlash = false;

  while (result
         && (currentPos < cmdStmt.length()))
    {
      if (cmdStmt.c_str()[currentPos] != commandSep)
        {
          if ((commandSep == ';')
              && (cmdStmt.c_str()[currentPos] == '\''))
            {
              commandSep = cmdStmt.c_str()[currentPos++];
            }
          else
            {
              if ((commandSep == '\'')
                  && (cmdStmt.c_str()[currentPos] == '\\'))
                {
                  armedSlash = ! armedSlash;
                }
              else
                armedSlash = false;

              ++currentPos;
            }

          continue;
        }
      else if (commandSep == '\'')
        {
          assert(currentPos > 0);

          if ( ! armedSlash)
            commandSep = ';';

          armedSlash = false;
          ++currentPos;
          continue ;
        }

      const string command = cmdStmt.substr(lastPos, currentPos - lastPos);
      lastPos = ++currentPos;

      if (command.length() > 0)
        result = ExecuteCommandLine(command);
    }

  if (! result)
    return false;

  if (commandSep != ';')
    {
      cerr << "Invalid command line format is unexpected.\n";
      return false;
    }

  const string command = cmdStmt.substr(lastPos);

  if (command.length() > 0)
    result = ExecuteCommandLine(command);

  return result;
}


static bool
cmdExit(const string& cmdLine, ENTRY_CMD_CONTEXT context)
{
  bool* finished = _RC(bool*, context);

  *finished = true;

  return true;
}


static int
ExecuteInteractively(istream& is)
{
  CmdEntry entry;

  entry.mName         = "quit";
  entry.mDesc         = descExit;
  entry.mExtendedDesc = descExtExit;
  entry.mCmd          = cmdExit;
  entry.mContext      = &sFinishInteraction;
  entry.mShowStatus   = false;

  RegisterCommand(entry);

  string commandStmt;
  while (! sFinishInteraction)
    {
      if ((commandStmt.length() == 0) && (&is == &cin))
        cout << "> ";

      string line;
      if (getline(is, line))
        {
          if (line.length() <= 0)
            continue;

          commandStmt.append(line);
          if (&is == &cin)
            {
              if (line[line.length() - 1] == '\\')
                commandStmt.resize(commandStmt.length() - 1);

              else
                {
                  ExecuteCommandStmt(commandStmt);
                  commandStmt.resize(0);
                }
            }
          else if (line[line.length() - 1] == ';')
            {
              line.resize(line.length() - 1);
              if ( ! ExecuteCommandStmt(commandStmt))
                {
                  sFinishInteraction = true;
                  return EINVAL;
                }

              commandStmt.resize(0);
            }
        }
      else if ((&is != &cin) && (commandStmt.length() > 0))
        {
          cerr << "Error! A script statement does not end with ';'.\n";
          return EINVAL;
        }
      else
        break;
    }

  return 0;
}


static void
InitDBS()
{
  if (IsOnlineDatabase())
  {
    if (GetVerbosityLevel() >= VL_INFO)
    {
      cout << "Connecting to a remote database as ";
      cout << (GetUserId() == 0 ? "administrator" : "default user") << ".\n";
    }

    if (GetVerbosityLevel() >= VL_DEBUG)
    {
      cout << " remote host:     " << GetRemoteHostName() << endl;
      cout << " port:            " << GetConnectionPort() << endl;
      cout << " database:        " << GetWorkingDB() << endl;
      cout << " user id:         " << GetUserId() << endl;
      cout << " password:        " << GetUserPassword() << endl;
    }
  }
  else
  {
    const string& workDir = GetWorkingDirectory();
    const uint64_t maxFileSize = GetMaximumFileSize();

    if (GetVerbosityLevel() >= VL_DEBUG)
    {
      cout << "Starting the DBS framework: " << endl;
      cout << " directory: " << workDir << endl;
      cout << " file_size: " << maxFileSize << endl;
    }

    DBSSettings settings;
    settings.mWorkDir = settings.mTempDir = workDir;
    DBSInit(settings);
  }
}


static void
StopDBS()
{
  if (IsOnlineDatabase())
    return;

  DBSShoutdown();

  if (GetVerbosityLevel() >= VL_DEBUG)
    cout << "Stopping the DBS framework." << endl;
}


static void
OpenDB()
{
  const VERBOSE_LEVEL level  = GetVerbosityLevel();
  const string&       workDB = GetWorkingDB();

  if (level >= VL_DEBUG)
    cout << "Opening database: " << workDB << " ... ";

  IDBSHandler& dbsHnd = DBSRetrieveDatabase(workDB.c_str());
  SetDbsHandler(dbsHnd);

  if (level >= VL_DEBUG)
    cout << "done." << endl;
}


static void
RemoveDB()
{
  const VERBOSE_LEVEL level  = GetVerbosityLevel();
  const string&       workDB = GetWorkingDB();

  if (level >= VL_DEBUG)
    cout << "Removing database: " << workDB << " ... ";

  DBSRemoveDatabase(workDB.c_str());

  if (level >= VL_DEBUG)
    cout << "done." << endl;
}


static void
CreateDB()
{
  const VERBOSE_LEVEL level  = GetVerbosityLevel();
  const string&       workDB = GetWorkingDB();

  if (level >= VL_INFO)
    cout << "Creating database: " << workDB << " ... ";

  DBSCreateDatabase(workDB.c_str());

  if (level >= VL_INFO)
    cout << "done." << endl;
}


int
main(const int argc, char *argv[])
{
  int           result              = 0;
  int           currentArg          = 1;
  bool          createDB            = false;
  bool          removeDB            = false;
  bool          useDB               = false;
  bool          autoYes             = false;
  bool          checkDbForErrors    = false;
  bool          showLogo            = true;
  bool          showLicense         = false;
  bool          genHeader           = false;
  const char*   scriptFile          = nullptr;
  const char*   cmdScript           = nullptr;
  string        dbDirectory;

  if ( !whs_init())
  {
    cerr << "Couldn't not initialize the network socket framework.\n";
    return ENOTSOCK;
  }

  if (argc == currentArg)
  {
    PrintWrongUsage(nullptr);
    return EINVAL;
  }

  InitCmdManager();

  while (currentArg < argc)
  {
    if ((strcmp(argv[currentArg], "--help") == 0)
        || (strcmp(argv[currentArg], "-h") == 0))
    {
      PrintHelpUsage();
      return 0;
    }
    else if ((strcmp(argv[currentArg], "-c") == 0)
             || (strcmp(argv[currentArg], "--create") == 0))
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);

        return EINVAL;
      }
      SetWorkingDB(argv[currentArg++]);
      createDB = true;
    }
    else if ((strcmp(argv[currentArg], "-r") == 0)
             || (strcmp(argv[currentArg], "--remove") == 0))
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }
      SetWorkingDB(argv[currentArg++]);
      removeDB = true;
    }
    else if ((strcmp(argv[currentArg], "-u") == 0)
             || (strcmp(argv[currentArg], "--use") == 0))
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);

        return EINVAL;
      }
      SetWorkingDB(argv[currentArg++]);
      useDB = true;
    }
    else if ((strcmp(argv[currentArg], "-H") == 0)
             || (strcmp(argv[currentArg], "--host") == 0))
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }
      SetRemoteHostName(argv[currentArg++]);
    }
    else if ((strcmp(argv[currentArg], "-P") == 0)
             || (strcmp(argv[currentArg], "--port") == 0))
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }
      SetConnectionPort(argv[currentArg++]);
    }
    else if ((strcmp(argv[currentArg], "-A") == 0)
             || (strcmp(argv[currentArg], "--admin") == 0))
    {
      ++currentArg;
      SetUserId(0);
    }
    else if ((strcmp(argv[currentArg], "-p") == 0)
             || (strcmp(argv[currentArg], "--pass") == 0))
    {
      ++currentArg;
      if (currentArg == argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);

        return EINVAL;
      }
      SetUserPassword(argv[currentArg++]);
    }
    else if ((strcmp(argv[currentArg], "-d") == 0)
             || (strcmp(argv[currentArg], "--dir") == 0))
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }

      SetWorkingDirectory(argv[currentArg++]);
    }
    else if ((strcmp(argv[currentArg], "-G") == 0)
             || (strcmp(argv[currentArg], "--globals") == 0))
    {
      genHeader = true;
      showLogo = false;
      ++currentArg;
    }
    else if (strcmp(argv[currentArg], "--verbose") == 0)
    {
      ++currentArg;
      if (currentArg >= argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }

      const int verbosityLevel = atoi(argv[currentArg++]);
      if (verbosityLevel > 5)
      {
        cerr << "The value of the verbosity level is invalid.\n";
        return EINVAL;
      }

      SetVerbosityLevel(verbosityLevel);
    }
    else if ((strcmp(argv[currentArg], "-s") == 0)
             || (strcmp(argv[currentArg], "--script") == 0))
    {
      ++currentArg;
      if (currentArg == argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }
      scriptFile = argv[currentArg++];
      showLogo = false;
    }
    else if ((strcmp(argv[currentArg], "-S") == 0)
             || (strcmp(argv[currentArg], "--cmd") == 0))
    {
      ++currentArg;
      if (currentArg == argc)
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }
      cmdScript = argv[currentArg++];
      showLogo = false;
    }
    else if ((strcmp(argv[currentArg], "-m") == 0)
             || (strcmp(argv[currentArg], "--file_size") == 0))
    {
      ++currentArg;

      if ((currentArg == argc) || ( !SetMaximumFileSize(argv[currentArg])))
      {
        PrintWrongUsage(argv[currentArg - 1]);
        return EINVAL;
      }
      else
        ++currentArg;
    }
    else if ((strcmp(argv[currentArg], "-f") == 0)
             || (strcmp(argv[currentArg], "--auto_yes") == 0))
    {
      ++currentArg;
      autoYes = true;
    }
    else if ((strcmp(argv[currentArg], "-t") == 0)
             || (strcmp(argv[currentArg], "--validate") == 0))
    {
      ++currentArg;

      checkDbForErrors = true;
    }
    else if (strcmp(argv[currentArg], "--nologo") == 0)
    {
      ++currentArg, showLogo = false;
    }
    else if ((strcmp(argv[currentArg], "-l") == 0)
             || (strcmp(argv[currentArg], "--license") == 0))
    {
      ++currentArg, showLicense = true;
    }
    else
    {
      cerr << "Unknown parameter '" << argv[currentArg] << "'.\n";
      return EINVAL;
    }
  }

  if (showLicense)
  {
    displayLicenseInformation(cout, sProgramName, sProgramDesc);
    return 0;
  }
  else if (showLogo)
    displayBanner(cout, "Whais Commander", WVER_MAJ, WVER_MIN);

  if ( ! (useDB || createDB || removeDB))
  {
    cerr << "A database needs to be selected.\n";

    return ECANCELED;
  }

  if (useDB)
  {
    if (removeDB)
    {
      cerr << "Could not use and remove a database in the same time.\n";
      return EINVAL;
    }
    else if (createDB)
    {
      cerr << "Could not use and create a database in the same time.\n";
      return EINVAL;
    }
  }
  else if (removeDB && createDB)
  {
    cerr << "Could not remove and create a database in the same time.\n";
    return EINVAL;
  }

  if (IsOnlineDatabase() || genHeader)
  {
    if (removeDB || createDB)
    {
      if (genHeader)
      {
        cerr << "Cannot generate header file for a database that will be "
             << (createDB ? "created" : "removed.") << endl;
      }
      else
        cerr << "Cannot remove nor create a database on a remote host.\n";

      return EINVAL;
    }

    assert(useDB);
  }

  if (checkDbForErrors)
  {
    if ( !useDB || IsOnlineDatabase())
    {
      cerr << "Only an existent local database may be validated.\n";
      return EINVAL;
    }
  }

  try
  {
    InitDBS();
  }
  catch(const Exception& e)
  {
    printException(cerr, e);
    return e.Code();
  }
  catch(...)
  {
    cerr << "Fatal error ... An unknown exception was encountered.\n";

    return EINVAL;
  }

  try
  {
      if (! IsOnlineDatabase() && showLogo)
        cout << DescribeDbsEngineVersion() << endl << endl;

    if (checkDbForErrors)
    {
      assert(useDB && !IsOnlineDatabase());

      checkDbForErrors = false;

      cout << "Checking database ...\n";
      result = check_database_for_errors(autoYes, true);

      if (result < 0)
      {
        cerr << "\nThe selected database is not in a valid state."
                " Aborting!\n";
      }
      else
      {
        cout << "\nThe database validation went well. Please restart the "
                "program with out '-t' or '--validate'.\n";
      }
      result = EAGAIN;
    }

      if ((result == 0) &&  ! removeDB)
        {
          if (createDB)
            CreateDB();

          if (IsOnlineDatabase())
            {
              if (GetUserPassword().size() == 0)
                {
                  string password;

                  cout << "Password"
                      << ((GetUserId() == 0) ? "(administrator): " : ": ");

                  wh_disable_echo();
                  getline(cin, password);
                  wh_enable_echo();

                  cout << endl;

                  SetUserPassword(password.c_str());
                }

              try
                {
                  WH_CONNECTION conHdl = nullptr;
                  uint32_t cs = WConnect(GetRemoteHostName().c_str(),
                                          GetConnectionPort().c_str(),
                                          GetWorkingDB().c_str(),
                                          GetUserPassword().c_str(),
                                          GetUserId(), DEFAULT_FRAME_SIZE,
                                          &conHdl);
                  if (cs != WCS_OK)
                    throw 0;

                  cs = WPingServer(conHdl);
                  WClose(conHdl);
                  if (cs != WCS_OK)
                    throw 0;
                }
              catch(...)
                {
                  /*
                   * This message might or might not appear. Though
                   * the program terminates, if it gets on this path then
                   * error message is displayed from time to time(this issue
                   * was spotted when using Linux).
                   *
                   * Probably a tool chain/OS/Bash bug!
                   */

                  cerr << "ERROR: Failed to connect at database '"
                       << GetWorkingDB() << "' from '"
                       << GetRemoteHostName() << '@'
                       << GetConnectionPort() << "'!\n";

                  return EINVAL;
                }

              AddOnlineTableCommands();
            }
          else
            {
              OpenDB();
              AddOfflineTableCommands();
            }

      set_signals();
      if (genHeader)
      {
        PrintExternalDeclarations(cout);
        result = 0;
      }
      else if (scriptFile != nullptr)
      {
        ifstream script(scriptFile, ios_base::in | ios_base::binary);
        result = ExecuteInteractively(script);
      }
      else if (cmdScript != nullptr)
      {
        istringstream script(cmdScript);
        result = ExecuteInteractively(script);
      }
      else
        result = ExecuteInteractively(cin);

      if ( !IsOnlineDatabase())
        DBSReleaseDatabase(GetDBSHandler());
    }
    else if (result == 0)
      RemoveDB();
  }
  catch(const Exception& e)
  {
    if ((e.Type() == DBS_EXCEPTION) && (e.Code() == DBSException::DATABASE_IN_USE))
    {
      cerr << "The selected database was not closed properly last time it ";
      cerr << "was used.\n";
      checkDbForErrors = repair_database_erros();
    }
    else
    {
      printException(cerr, e);
      result = e.Code();
    }
  }
  catch(...)
  {
    cerr << "Fatal error ... Unknown exception was thrown.\n";
    result = 1;
  }

  try
  {
    if (checkDbForErrors)
    {
      if (check_database_for_errors(autoYes, false) < 0)
      {
        cout << "Failed to validate the database.\n";
        result = EINVAL;
      }
      else
      {
        cout << "The database has been successfully repaired. "
                "You need to restart the program to use it.\n";
        result = EAGAIN;
      }
    }
    StopDBS();
  }
  catch(const Exception& e)
  {
    printException(cerr, e);
    result = (result != 0) ? result : e.Code();
  }
  catch(...)
  {
    cerr << "Fatal error ... Unknown exception was thrown.\n";
    result = (result != 0)  ? result : 1;
  }

  whs_clean();

  return result;
}

#ifdef ENABLE_MEMORY_TRACE
uint32_t WMemoryTracker::smInitCount = 0;
const char* WMemoryTracker::smModule = "WCMD";
#endif

