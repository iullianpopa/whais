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
#include <assert.h>
#include <signal.h>
#include <memory.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "whais.h"

#include "dbs/dbs_mgr.h"
#include "utils/logger.h"
#include "utils/license.h"

#include "../common/configuration.h"
#include "../common/loader.h"
#include "../common/server.h"


using namespace std;
using namespace whais;


static const char sProgramName[] = "Whais";
static const char sProgramDesc[] = "A database server(generic).";

static bool sDbsInited         = false;
static bool sInterpreterInited = false;

static vector<DBSDescriptors> databases;


static void
clean_frameworks(FileLogger& log)
{
  if (sInterpreterInited)
  {
    assert(sDbsInited);

    for (auto dbs = databases.rbegin() ; dbs != databases.rend(); ++dbs)
    {
      if (dbs->mSession != nullptr)
      {
        ReleaseInstance(*(dbs->mSession));
        dbs->mSession = nullptr;
      }

      ostringstream logEntry;
      logEntry << "Closing session '" << dbs->mDbsName << "'.";
      log.Log(LT_INFO, logEntry.str());
    }
  }

  if (sInterpreterInited)
    CleanInterpreter();

  if (sInterpreterInited)
  {
    assert(sDbsInited);

    for (auto dbsIterator = databases.rbegin(); dbsIterator != databases.rend(); ++dbsIterator)
    {
      if (dbsIterator->mDbs != nullptr)
        DBSReleaseDatabase( *(dbsIterator->mDbs));

      if (dbsIterator->mLogger != nullptr)
      {
        dbsIterator->mLogger->Log(LT_INFO, "Database context ended!");
        delete dbsIterator->mLogger;
      }

      ostringstream logEntry;
      logEntry << "Cleaned resources of database '";
      logEntry << dbsIterator->mDbsName << "'.";
      log.Log(LT_INFO, logEntry.str());
    }
  }
  if (sDbsInited)
    DBSShoutdown();
}


#ifndef ARCH_WINDOWS_VC

static void
sigterm_hdl(int sig, siginfo_t *siginfo, void *context)
{
  if ((sig != SIGINT) && (sig != SIGTERM))
    return ; //Ignore this!

  StopServer();
}


static bool
set_signals()
{
  struct sigaction action;

  memset( &action, 0, sizeof action);
  action.sa_flags = SA_SIGINFO;
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
  StopServer();

  return TRUE;
}

static BOOL
set_signals()
{
  return SetConsoleCtrlHandler(ServerStopHandler, TRUE);
}

#endif


int
main(int argc, char** argv)
{
  unique_ptr<ifstream> config;
  unique_ptr<FileLogger> glbLog;

  displayLicenseInformation(cout, sProgramName, sProgramDesc);
  cout << endl << endl;

  if (argc < 2)
  {
    cerr << "Main configuration file was not passed as argument!\n";
    return EINVAL;
  }
  else
  {
    config.reset(new ifstream(argv[1], ios_base::in | ios_base::binary));
    if ( !config->good())
    {
      cerr << "Could not open the configuration file '" << argv[1] << "'.\n";
      return EINVAL;
    }
  }

  if ( !whs_init())
  {
    cerr << "Could not initialize the network socket framework.\n";
    return ENOTSOCK;
  }

  try
  {
      uint_t sectionLine = 0;
      if (SeekAtConfigurationSection(*config, sectionLine) == false)
        {
          cerr << "Cannot find the CONFIG section in configuration file!\n";
          return -1;
        }

      assert(sectionLine > 0);

      if (ParseConfigurationSection(*config, sectionLine, cerr) == false)
        return -1;

      glbLog.reset(new FileLogger(GetAdminSettings().mLogFile.c_str()));

  }
  catch(ios_base::failure& e)
  {
      cerr << "Unexpected error during configuration read:\n" << e.what() << endl;
      return -1;
  }
  catch(...)
  {
    cerr << "Unknown error encountered during main configuration reading!\n";
    return -1;
  }

  try
  {
    if (! set_signals())
      throw std::runtime_error("Signals handlers could not be overwritten.");

    if ( ! PrepareConfigurationSection(*glbLog))
      return -1;

    uint_t configLine = 0;
    config->clear();
    config->seekg(0);
    while (FindNextContextSection( *config, configLine))
    {
      DBSDescriptors dbs(configLine);

      //Inherit some global settings from the server configuration area in
      //case are not set in the context configuration section.
      dbs.mWaitReqTmo = GetAdminSettings().mWaitReqTmo;
      dbs.mSyncInterval = GetAdminSettings().mSyncInterval;

      if ( !ParseContextSection( *glbLog, *config, configLine, dbs))
        return -1;

      if ( !PrepareContextSection( *glbLog, dbs))
        return -1;

      ostringstream logEntry;

      for (const auto& d : databases)
      {
        if (d.mDbsName == dbs.mDbsName)
        {
          logEntry << "Duplicate entry '" << dbs.mDbsName << "'. Ignoring the last"
              << " configuration entry.\n";
          glbLog->Log(LT_ERROR, logEntry.str());
          continue;
        }
      }

      if (dbs.mDbsName == GlobalContextDatabase())
        databases.insert(databases.begin(), dbs);

      else
        databases.push_back(dbs);
    }

    if (databases.size() == 0)
    {
      glbLog->Log(LT_CRITICAL, "No database context configured.");
      return -1;
    }
    else if (databases[0].mDbsName != GlobalContextDatabase())
    {
      ostringstream noGlbMsg;

      noGlbMsg << "No entry for global section '" << GlobalContextDatabase() << "' was found.";
      glbLog->Log(LT_CRITICAL, noGlbMsg.str());
    }

    const ServerSettings& confSettings = GetAdminSettings();

    DBSSettings dbsSettings;
    dbsSettings.mTableCacheBlkCount   = confSettings.mTableCacheBlockCount;
    dbsSettings.mTableCacheBlkSize    = confSettings.mTableCacheBlockSize;
    dbsSettings.mWorkDir              = confSettings.mWorkDirectory;
    dbsSettings.mTempDir              = confSettings.mTempDirectory;
    dbsSettings.mVLStoreCacheBlkCount = confSettings.mVLBlockCount;
    dbsSettings.mVLStoreCacheBlkSize  = confSettings.mVLBlockSize;
    dbsSettings.mVLValueCacheSize     = confSettings.mTempValuesCache;

    DBSInit(dbsSettings);
    sDbsInited = true;

    InitInterpreter(databases[0].mDbsDirectory.c_str());
    sInterpreterInited = true;

    for (auto& d : databases)
      LoadDatabase( *glbLog, d);

    cout << "All configured databases have been loaded!\n";
    StartServer(*glbLog, databases);
  }
  catch(Exception& e)
  {
    ostringstream logEntry;

    logEntry << "Unable to deal with error condition.\n";
    if (e.Description())
      logEntry << "Description:\n\t" << e.Description() << endl;

    if ( !e.Message().empty())
      logEntry << "Message:\n\t" << e.Message() << endl;

    logEntry << "Extra: " << e.Code() << " (" << e.File() << ':' << e.Line() << ").\n";
    glbLog->Log(LT_CRITICAL, logEntry.str());

    clean_frameworks( *glbLog);

    return -1;
  }
  catch(std::bad_alloc&)
  {
    glbLog->Log(LT_CRITICAL, "OUT OF MEMORY!!!");
    clean_frameworks(*glbLog);

    return -1;
  }
  catch(std::exception& e)
  {
    ostringstream logEntry;

    logEntry << "General system failure: " << e.what() << endl;
    glbLog->Log(LT_CRITICAL, logEntry.str());

    clean_frameworks(*glbLog);

    return -1;
  }
  catch(...)
  {
    assert(false);

    glbLog->Log(LT_CRITICAL, "Unknown exception!");
    clean_frameworks(*glbLog);

    return -1;
  }

  clean_frameworks(*glbLog);
  whs_clean();

  return 0;
}
