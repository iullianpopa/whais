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

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "utils/tokenizer.h"
#include "utils/wunicode.h"

#include "server_protocol.h"
#include "configuration.h"

using namespace std;
using namespace whais;

static const char COMMENT_CHAR = '#';
static const char DEFAULT_LISTEN_PORT[] = "1761";
static const char CLEAR_LOG_STREAM[] = "";

static const char CIPHER_PLAIN[] = "plain";
static const char CIPHER_3K[] = "3k";
static const char CIPHER_DES[] = "des";
static const char CIPHER_3DES[] = "3des";

static const uint_t MIN_TABLE_CACHE_BLOCK_SIZE = 1024;
static const uint_t MIN_TABLE_CACHE_BLOCK_COUNT = 128;
static const uint_t MIN_VL_BLOCK_SIZE = 1024;
static const uint_t MIN_VL_BLOCK_COUNT = 128;
static const uint_t MIN_TEMP_CACHE = 128;

static const uint_t DEFAULT_MAX_CONNS = 64;
static const uint_t DEFAULT_TABLE_CACHE_BLOCK_SIZE = 4098;
static const uint_t DEFAULT_TABLE_CACHE_BLOCK_COUNT = 1024;
static const uint_t DEFAULT_VL_BLOCK_SIZE = 1024;
static const uint_t DEFAULT_VL_BLOCK_COUNT = 4098;
static const uint_t DEFAULT_TEMP_CACHE = 512;
static const uint_t DEFAULT_WAIT_TMO_MS = 60 * 1000;
static const uint_t DEFAULT_SYNC_INTERVAL_MS = 0;
static const uint_t DEFAULT_SYNC_WAKEUP_MS = 1000;
static const uint_t DEFAULT_AUTH_TMO_MS = 1000;

static const string gEntPort("listen");
static const string gEntMaxConnections("max_connections");
static const string gEntMaxFrameSize("max_frame_size");
static const string gEntEncryption("cipher");
static const string gEntTableBlkSize("table_block_cache_size");
static const string gEntTableBlkCount("table_block_cache_count");
static const string gEntVlBlkSize("vl_values_block_size");
static const string gEntVlBlkCount("vl_values_block_count");
static const string gEntTempCache("temporals_cache");
static const string gEntAuthTMO("auth_tmo_ms");
static const string gEntRequestTMO("request_tmo_ms");
static const string gEntSyncInterval("sync_interval_ms");
static const string gEntSyncWakeup("syncer_wakeup_ms");
static const string gEntLogFile("log_file");
static const string gEntDBSName("name");
static const string gEntWorkDir("directory");
static const string gEntTempDir("temp_directory");
static const string gEntShowDbg("show_debug");
static const string gEntObjectLib("load_object");
static const string gEntNativeLib("load_native");
static const string gEntRootPasswrd("admin_password");
static const string gEntUserPasswrd("user_password");
static const string gEntStackCount("max_stack_count");

static ServerSettings gMainSettings;

// Helper function to retrieve '...' or "..." text entries.
static bool get_enclosed_entry(ostream& os, const string& line, const char encChar, string& output)
{
  assert(line.at(0) == encChar);

  bool encEnded = false;

  for (auto ch : line.substr(1))
  {
    if (ch == encChar)
    {
      encEnded = true;
      break;
    }
    output.append(1, ch);
  }

  return encEnded;
}

const string&
GlobalContextDatabase()
{
  static const string dbsName("administrator");
  return dbsName;
}

const ServerSettings&
GetAdminSettings()
{
  return gMainSettings;
}

bool SeekAtConfigurationSection(ifstream& config, uint_t& outConfigLine)
{
  static const string identifier("[CONFIG]");
  static const string delimiters(" \t");

  outConfigLine = 0;
  config.clear(), config.seekg(0);

  while (config.good())
  {
    string line;
    getline(config, line);
    ++outConfigLine;

    size_t pos = 0;
    string token = NextToken(line, pos, delimiters);
    if ((token.length() > 0) && (token.at(0) == COMMENT_CHAR))
      continue;

    if (token == identifier)
      return true;
  }
  return false;
}

bool FindNextContextSection(std::ifstream& config, uint_t& inoutConfigLine)
{
  static const string identifier("[DATABASE]");
  static const string delimiters(" \t");

  while (config.good())
  {
    string line;
    getline(config, line);
    ++inoutConfigLine;

    size_t pos = 0;
    string token = NextToken(line, pos, delimiters);
    if ((token.length() > 0) && (token.at(0) == COMMENT_CHAR))
      continue;

    if (token == identifier)
      return true;
  }

  return false;
}

bool ParseConfigurationSection(ifstream& config, uint_t& inoutConfigLine, ostream& errOut)
{
  static const string delimiters(" \t=");

  while ( !config.eof())
  {
    const streampos lastPos = config.tellg();

    string line;
    getline(config, line);

    ++inoutConfigLine;

    size_t pos = 0;
    string token = NextToken(line, pos, delimiters);

    if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      continue;

    if (token.at(0) == '[')
    {
      //Another configuration section starts from here.
      config.clear(), config.seekg(lastPos);
      break;
    }

    if (token == gEntPort)
    {
      ListenEntry entry;

      token = NextToken(line, pos, " \t=@#");
      entry.mInterface = token;

      token = NextToken(line, pos, " \t@#");
      entry.mService = token;

      if (entry.mInterface == "*")
        entry.mInterface = "";

      if (entry.mService == "")
        entry.mService = DEFAULT_LISTEN_PORT;

      gMainSettings.mListens.push_back(entry);
    }
    else if (token == gEntMaxConnections)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mMaxConnections = atoi(token.c_str());
    }
    else if (token == gEntMaxFrameSize)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mMaxFrameSize = atoi(token.c_str());
    }
    else if (token == gEntEncryption)
    {
      token = NextToken(line, pos, delimiters);
      std::transform(token.begin(), token.end(), token.begin(), wh_to_lowercase);

      if (token == CIPHER_PLAIN)
        gMainSettings.mCipher = FRAME_ENCTYPE_PLAIN;

      else if (token == CIPHER_3K)
        gMainSettings.mCipher = FRAME_ENCTYPE_3K;

      else if (token == CIPHER_DES)
        gMainSettings.mCipher = FRAME_ENCTYPE_DES;

      else if (token == CIPHER_3DES)
        gMainSettings.mCipher = FRAME_ENCTYPE_3DES;

      else
      {
        errOut << "The cipher '" << token << "' is not supported. " << "Allowed ciphers are "
            << CIPHER_PLAIN << ", " << CIPHER_DES << ", " << CIPHER_3DES << " and " << CIPHER_3K
            << ".\n";

        return false;
      }
    }
    else if (token == gEntTableBlkCount)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mTableCacheBlockCount = atoi(token.c_str());

      if (gMainSettings.mTableCacheBlockCount == 0)
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }
    }
    else if (token == gEntTableBlkSize)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mTableCacheBlockSize = atoi(token.c_str());

      if (gMainSettings.mTableCacheBlockSize == 0)
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }
    }
    else if (token == gEntVlBlkCount)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mVLBlockCount = atoi(token.c_str());

      if (gMainSettings.mVLBlockCount == 0)
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }
    }
    else if (token == gEntVlBlkSize)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mVLBlockSize = atoi(token.c_str());

      if (gMainSettings.mVLBlockSize == 0)
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }
    }
    else if (token == gEntTempCache)
    {
      token = NextToken(line, pos, delimiters);
      gMainSettings.mTempValuesCache = atoi(token.c_str());

      if (gMainSettings.mTempValuesCache == 0)
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }
    }
    else if (token == gEntAuthTMO)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      gMainSettings.mAuthTMO = atoi(token.c_str());
      if (gMainSettings.mAuthTMO <= 0)
      {
        errOut << "At line " << inoutConfigLine << "the connection timeout parameter "
            " should be an integer value bigger than 0(currently set to " << gMainSettings.mAuthTMO
            << " ).\n";

        return false;
      }
    }
    else if (token == gEntRequestTMO)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      gMainSettings.mWaitReqTmo = atoi(token.c_str());
      if (gMainSettings.mWaitReqTmo <= 0)
      {
        errOut << "At line " << inoutConfigLine << "the request waiting timeout parameter should"
            " be an integer value bigger than 0(currently set to " << gMainSettings.mWaitReqTmo
            << " ).\n";
        return false;
      }
    }
    else if (token == gEntSyncInterval)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      gMainSettings.mSyncInterval = atoi(token.c_str());
      if (gMainSettings.mSyncInterval < -1)
      {
        errOut << "At line " << inoutConfigLine << "the data sync interval timeout parameter"
            " should be a positive integer value(was set to " << gMainSettings.mSyncInterval
            << " ).\n";
        return false;
      }
    }
    else if (token == gEntSyncWakeup)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      gMainSettings.mSyncWakeup = atoi(token.c_str());
      if (gMainSettings.mSyncWakeup <= 0)
      {
        errOut << "At line " << inoutConfigLine << "the data sync interval timeout parameter"
            " should be an integer value bigger than 0(was set to " << gMainSettings.mSyncWakeup
            << " ).\n";
        return false;
      }
    }
     else if (token == gEntLogFile)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(errOut, entry, token.at(0), gMainSettings.mLogFile) == false)
        {
          errOut << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << ".\n";
          return false;
        }

      }
      else
        gMainSettings.mLogFile = token;
    }
    else if (token == gEntTempDir)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(errOut, entry, token.at(0), gMainSettings.mTempDirectory) == false)
        {
          errOut << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << ".\n";
          return false;
        }
      }
      else
        gMainSettings.mTempDirectory = token;

      NormalizeFilePath(gMainSettings.mTempDirectory, true);

    }
    else if (token == gEntWorkDir)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        errOut << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(errOut, entry, token.at(0), gMainSettings.mWorkDirectory) == false)
        {
          errOut << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << ".\n";
          return false;
        }
      }
      else
        gMainSettings.mWorkDirectory = token;

      const string &dir = NormalizeFilePath(gMainSettings.mWorkDirectory, true);

      if ((dir.length() != 0) && !whf_is_absolute(dir.c_str()))
      {
        errOut << "Error: The configured working directory cannot be a relative path('" << dir
            << "').\n";
        return false;
      }
    }
    else if (token == gEntShowDbg)
    {
      token = NextToken(line, pos, delimiters);

      if (token == "false")
        gMainSettings.mShowDebugLog = false;

      else if (token == "true")
        gMainSettings.mShowDebugLog = true;

      else
      {
        errOut << "Cannot assign '" << token << "\' to 'show_debug' at line " << inoutConfigLine
            << ". Valid value are only 'true' or 'false'.\n";
        return false;
      }
    }
    else
    {
      errOut << "At line " << inoutConfigLine << ": Don't know what to do with '" << token
          << "'.\n";
      return false;
    }
  }

  if (gMainSettings.mWorkDirectory.length() == 0)
    gMainSettings.mWorkDirectory = whf_current_dir();

  if (gMainSettings.mTempDirectory.length() > 0)
  {
    if ( !whf_is_absolute(gMainSettings.mTempDirectory.c_str()))
      gMainSettings.mTempDirectory = gMainSettings.mWorkDirectory + gMainSettings.mTempDirectory;
  }
  else
    gMainSettings.mTempDirectory = gMainSettings.mWorkDirectory;

  if ( !whf_file_exists(gMainSettings.mWorkDirectory.c_str()))
  {
    errOut << "The working directory does not exists('" << gMainSettings.mWorkDirectory << "')!\n";
    return false;
  }

  if ( !whf_file_exists(gMainSettings.mTempDirectory.c_str()))
  {
    errOut << "The directory to hold temporal values does not exists('"
        << gMainSettings.mTempDirectory << "')!\n";
    return false;
  }

  if (gMainSettings.mLogFile.length() == 0)
  {
    errOut << "The configuration main section does not have a '" << gEntLogFile << "' entry.\n";
    return false;
  }

  return true;
}

bool ParseContextSection(Logger& log,
                         ifstream& config,
                         uint_t& inoutConfigLine,
                         DBSDescriptors& output)
{
  ostringstream logEntry;
  static const string delimiters = " \t=";

  while (config.good())
  {
    const streamoff lastPos = config.tellg();

    string line;
    getline(config, line);

    ++inoutConfigLine;

    size_t pos = 0;
    string token = NextToken(line, pos, delimiters);

    if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      continue;

    if (token.at(0) == '[')
    {
      //Another configuration section starts from here.
      config.clear(), config.seekg(lastPos);
      break;
    }

    if (token == gEntDBSName)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        logEntry << "Configuration error at line " << inoutConfigLine << '.';
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(logEntry, entry, token.at(0), output.mDbsName) == false)
        {
          logEntry << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << '.';
          log.Log(LT_CRITICAL, logEntry.str());

          return false;
        }
      }
      else
        output.mDbsName = token;
    }
    else if (token == gEntRequestTMO)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        cerr << "Configuration error at line " << inoutConfigLine << '.';
        return false;
      }

      output.mWaitReqTmo = atoi(token.c_str());
      if (output.mWaitReqTmo <= 0)
      {
        cerr << "At line " << inoutConfigLine << "the request waiting timeout parameter should be"
            " an integer value bigger than 0(was set to " << gMainSettings.mWaitReqTmo << " ).\n";
        return false;
      }
    }
    else if (token == gEntSyncInterval)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        cerr << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      output.mSyncInterval = atoi(token.c_str());
      if (output.mSyncInterval < -1)
      {
        cerr << "At line " << inoutConfigLine << "the data sync interval timeout parameter should"
            " be a positive integer value (was set to " << gMainSettings.mSyncInterval << " ).\n";
        return false;
      }
    }
    else if (token == gEntWorkDir)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        cerr << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(cerr, entry, token.at(0), output.mDbsDirectory) == false)
        {
          cerr << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << ".\n";
          return false;
        }
      }
      else
        output.mDbsDirectory = token;

      const string& dir = NormalizeFilePath(output.mDbsDirectory, true);

      if ( !whf_is_absolute(dir.c_str()))
        output.mDbsDirectory = gMainSettings.mWorkDirectory + dir;
    }
    else if (token == gEntLogFile)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        cerr << "Configuration error at line " << inoutConfigLine << ".\n";
        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(cerr, entry, token.at(0), output.mDbsLogFile) == false)
        {
          cerr << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << ".\n";
          return false;
        }
      }
      else
        output.mDbsLogFile = token;

      NormalizeFilePath(output.mDbsLogFile, false);
    }
    else if (token == gEntObjectLib)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        logEntry << "Configuration error at line " << inoutConfigLine << '.';
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }

      string entryValue;
      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(logEntry, entry, token.at(0), entryValue) == false)
        {
          logEntry << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << '.';
          log.Log(LT_CRITICAL, logEntry.str());
          return false;
        }
      }
      else
        entryValue = token;

      NormalizeFilePath(entryValue, false);

      if ( !whf_is_absolute(entryValue.c_str()))
        entryValue = gMainSettings.mWorkDirectory + entryValue;

      output.mObjectLibs.push_back(entryValue);
    }
    else if (token == gEntNativeLib)
    {
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        logEntry << "Configuration error at line " << inoutConfigLine << '.';
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }

      string libEntry;
      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(logEntry, entry, token.at(0), libEntry) == false)
        {
          logEntry << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << '.';
          log.Log(LT_CRITICAL, logEntry.str());

          return false;
        }

      }
      else
        libEntry = token;

      bool fixAbsolutePath = false;
      for (size_t i = 0; i < libEntry.length(); ++i)
      {
        if ((libEntry[i] == '\\') || (libEntry[i] == '/'))
        {
          libEntry[i] = whf_dir_delim();
          fixAbsolutePath = true;
        }
      }

      if (fixAbsolutePath && !whf_is_absolute(libEntry.c_str()))
        libEntry = gMainSettings.mWorkDirectory + libEntry;

      output.mNativeLibs.push_back(libEntry);
    }
    else if (token == gEntRootPasswrd)
    {
      if ( !output.mRootPass.empty())
      {
        logEntry << "Configuration error at line " << inoutConfigLine << ". The root password for"
            " this database was already set.";
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }
      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        logEntry << "Configuration error at line " << inoutConfigLine << ".";
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(logEntry, entry, token.at(0), output.mRootPass) == false)
        {
          logEntry << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << '.';
          log.Log(LT_CRITICAL, logEntry.str());

          return false;
        }
      }
      else
        output.mRootPass = token;
    }
    else if (token == gEntUserPasswrd)
    {
      if ( !output.mUserPasswd.empty())
      {
        logEntry << "Configuration error at line " << inoutConfigLine << ". The user password for"
            " this database was already set.";
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }

      token = NextToken(line, pos, delimiters);

      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        logEntry << "Configuration error at line " << inoutConfigLine << '.';
        log.Log(LT_CRITICAL, logEntry.str());

        return false;
      }

      if ((token.at(0) == '\'') || (token.at(0) == '"'))
      {
        const string entry = line.c_str() + pos - token.length();

        if (get_enclosed_entry(logEntry, entry, token.at(0), output.mUserPasswd) == false)
        {
          logEntry << "Unmatched " << token.at(0) << " in configuration file at line "
              << inoutConfigLine << '.';
          log.Log(LT_CRITICAL, logEntry.str());

          return false;
        }
      }
      else
        output.mUserPasswd = token;
    }
    else if (token == gEntStackCount)
    {
      token = NextToken(line, pos, delimiters);
      if ((token.length() == 0) || (token.at(0) == COMMENT_CHAR))
      {
        cerr << "Configuration error at line " << inoutConfigLine << ".\n";

        return false;
      }

      output.mStackCount = atoi(token.c_str());
      if (output.mStackCount <= 0)
      {
        cerr << "At line " << inoutConfigLine << "the request waiting timeout parameter should be"
            " an integer value bigger than 0(currently set to " << gMainSettings.mWaitReqTmo
            << " ).\n";
        return false;
      }
    }
    else
    {
      logEntry << "At line " << inoutConfigLine << ": Don't know what to do " << "with '" << token
          << '.';
      log.Log(LT_CRITICAL, logEntry.str());

      return false;
    }
  }

  return true;
}

bool PrepareConfigurationSection(Logger& log)
{
  ostringstream logStream;

  if (gMainSettings.mListens.size() == 0)
  {
    ListenEntry defaultEnt
    { "", DEFAULT_LISTEN_PORT };
    gMainSettings.mListens.push_back(defaultEnt);
  }

  if (gMainSettings.mMaxConnections == UNSET_VALUE)
  {
    if (gMainSettings.mShowDebugLog)
    {
      log.Log(LT_DEBUG,
              "The number of maximum simultaneous connections per interface set by default.");
    }
    gMainSettings.mMaxConnections = DEFAULT_MAX_CONNS;
  }

  logStream << "Maximum simultaneous connections per interface set at "
      << gMainSettings.mMaxConnections << '.';
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  if (gMainSettings.mCipher == UNSET_VALUE)
  {
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The communication cipher is set by default.");
    gMainSettings.mCipher = FRAME_ENCTYPE_PLAIN;
  }

  logStream << "Communication cipher is set to '";
  switch (gMainSettings.mCipher)
  {
  case FRAME_ENCTYPE_PLAIN:
    logStream << CIPHER_PLAIN;
    break;

  case FRAME_ENCTYPE_3K:
    logStream << CIPHER_3K;
    break;

  case FRAME_ENCTYPE_DES:
    logStream << CIPHER_DES;
    break;

  case FRAME_ENCTYPE_3DES:
    logStream << CIPHER_3DES;
    break;

  default:
    assert(false);
  }
  logStream << "'.";

  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  if (gMainSettings.mMaxFrameSize == UNSET_VALUE)
  {
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The maximum communication frame size set by default.");

    gMainSettings.mMaxFrameSize = DEFAULT_FRAME_SIZE;
  }
  else if ((gMainSettings.mMaxFrameSize < MIN_FRAME_SIZE)
      || (MAX_FRAME_SIZE < gMainSettings.mMaxFrameSize))
  {
    logStream << "The maximum frame size set to a invalid value. The value should be set between "
        << MIN_FRAME_SIZE << " and " << MAX_FRAME_SIZE << " bytes.";
    log.Log(LT_ERROR, logStream.str());

    return false;
  }

  logStream << "Maximum communication frame size set to " << gMainSettings.mMaxFrameSize
      << " bytes.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  if (gMainSettings.mTableCacheBlockSize == UNSET_VALUE)
  {
    gMainSettings.mTableCacheBlockSize = DEFAULT_TABLE_CACHE_BLOCK_SIZE;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The table cache block size is set by default.");
  }

  if (gMainSettings.mTableCacheBlockSize < MIN_TABLE_CACHE_BLOCK_SIZE)
  {
    gMainSettings.mTableCacheBlockSize = MIN_TABLE_CACHE_BLOCK_SIZE;
    log.Log(LT_INFO, "The table cache block size was set to less than minimum. ");
  }

  logStream << "Table cache block size set at " << gMainSettings.mTableCacheBlockSize << " bytes.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  if (gMainSettings.mTableCacheBlockCount == UNSET_VALUE)
  {
    gMainSettings.mTableCacheBlockCount = DEFAULT_TABLE_CACHE_BLOCK_COUNT;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The table cache block count is set by default.");
  }
  if (gMainSettings.mTableCacheBlockCount < MIN_TABLE_CACHE_BLOCK_COUNT)
  {
    gMainSettings.mTableCacheBlockCount = MIN_TABLE_CACHE_BLOCK_COUNT;
    log.Log(LT_INFO, "The table cache block count was set to less than minimum. ");
  }

  logStream << "Table cache block count set at " << gMainSettings.mTableCacheBlockCount << '.';
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  //VLS
  if (gMainSettings.mVLBlockSize == UNSET_VALUE)
  {
    gMainSettings.mVLBlockSize = DEFAULT_VL_BLOCK_SIZE;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The table VL store cache block size is set by default.");
  }

  if (gMainSettings.mVLBlockSize < MIN_VL_BLOCK_SIZE)
  {
    gMainSettings.mVLBlockSize = MIN_VL_BLOCK_SIZE;
    log.Log(LT_INFO, "The table cache block size was set to less than minimum. ");
  }
  logStream << "Table VL store cache block size set at " << gMainSettings.mVLBlockSize << " bytes.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  if (gMainSettings.mVLBlockCount == UNSET_VALUE)
  {
    gMainSettings.mVLBlockCount = DEFAULT_VL_BLOCK_COUNT;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The table VL store cache block count is set by default.");
  }

  if (gMainSettings.mVLBlockCount < MIN_VL_BLOCK_COUNT)
  {
    gMainSettings.mVLBlockCount = MIN_VL_BLOCK_COUNT;
    log.Log(LT_INFO, "The table VL store he block count was set to less than minimum.");
  }

  logStream << "Table VL store cache block count set at " << gMainSettings.mVLBlockCount << '.';
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  //Temporal values
  if (gMainSettings.mTempValuesCache == UNSET_VALUE)
  {
    gMainSettings.mTempValuesCache = DEFAULT_TEMP_CACHE;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The temporal values cache is set by default");
  }
  if (gMainSettings.mTempValuesCache < MIN_TEMP_CACHE)
  {
    gMainSettings.mTempValuesCache = MIN_TEMP_CACHE;
    log.Log(LT_INFO, "The temporal values cache was set to less than minimum.");
  }
  logStream << "The temporal values cache set at " << gMainSettings.mTempValuesCache << " bytes.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  //Authentication timeout
  if (gMainSettings.mAuthTMO == UNSET_VALUE)
  {
    gMainSettings.mAuthTMO = DEFAULT_AUTH_TMO_MS;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The authentication timeout value is set by default.");
  }

  logStream << "The authentication timeout value is set at " << gMainSettings.mAuthTMO
      << " milliseconds.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  //Syncer wake up
  if (gMainSettings.mSyncWakeup == UNSET_VALUE)
  {
    gMainSettings.mSyncWakeup = DEFAULT_SYNC_WAKEUP_MS;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The data syncer wake up interval is set by default value.");
  }

  logStream << "The data syncer wake up interval is set at " << gMainSettings.mSyncWakeup
      << " milliseconds.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  //Sync data valid interval
  logStream << "The default data flush interval interval is set at " << gMainSettings.mSyncInterval
      << " milliseconds.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  //Request wait timeout
  if (gMainSettings.mWaitReqTmo == UNSET_VALUE)
  {
    gMainSettings.mWaitReqTmo = DEFAULT_WAIT_TMO_MS;
    if (gMainSettings.mShowDebugLog)
      log.Log(LT_DEBUG, "The request waiting timeout is set by default.");
  }

  logStream << "The request waiting timeout is set at " << gMainSettings.mWaitReqTmo
      << " milliseconds.";
  log.Log(LT_INFO, logStream.str());
  logStream.str(CLEAR_LOG_STREAM);

  return true;
}

bool PrepareContextSection(Logger& log, DBSDescriptors& inoutDesc)
{
  assert(inoutDesc.mConfigLine != 0);

  ostringstream logStream;

  if (inoutDesc.mUserPasswd.empty())
  {
    logStream << "A user password for the database  section starting at line "
        << inoutDesc.mConfigLine << " was not set.";
    log.Log(LT_ERROR, logStream.str());

    return false;
  }

  if (inoutDesc.mRootPass.empty())
  {
    logStream << "A root password for the database section starting at line "
        << inoutDesc.mConfigLine << " was not set.";
    log.Log(LT_ERROR, logStream.str());

    return false;
  }

  if (inoutDesc.mDbsName.empty())
  {
    logStream << "Database section starting line " << inoutDesc.mConfigLine << " does not have a '"
        << gEntDBSName << "' entry.";
    log.Log(LT_ERROR, logStream.str());

    return false;
  }

  if (inoutDesc.mDbsDirectory.empty())
  {
    logStream << "Database section starting line " << inoutDesc.mConfigLine << " does not have a '"
        << gEntWorkDir << "' entry.";
    log.Log(LT_ERROR, logStream.str());

    return false;
  }
  else if ( !whf_file_exists(inoutDesc.mDbsDirectory.c_str()))
  {
    logStream << "Database section starting line " << inoutDesc.mConfigLine << " entry '"
        << gEntWorkDir << "' points to an inexistent directory '" << inoutDesc.mDbsDirectory
        << "'.";

    log.Log(LT_ERROR, logStream.str());

    return false;
  }

  if (inoutDesc.mDbsLogFile.length() == 0)
  {
    logStream << "Database section starting line " << inoutDesc.mConfigLine << " does not have a '"
        << gEntLogFile << "' entry.";
    log.Log(LT_ERROR, logStream.str());

    return false;
  }

  if ( !whf_is_absolute(inoutDesc.mDbsLogFile.c_str()))
    inoutDesc.mDbsLogFile = inoutDesc.mDbsDirectory + inoutDesc.mDbsLogFile;

  return true;
}
