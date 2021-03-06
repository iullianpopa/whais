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

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <assert.h>

#include "compiler/whaisc.h"
#include "utils/tokenizer.h"
#include "utils/license.h"
#include "whc_cmdline.h"

using namespace std;


static const char sProgramName[] = "Whais Compiler";
static const char sProgramDesc[] = "A tool to create procedures for data records handling.";
const static string outputFileExt(".wo");
const static string inputFileExt(".w");


namespace whais {
namespace whc {


static inline bool
areStrsEqual(const char* str1, const char* str2)
{
  return strcmp(str1, str2) == 0;
}


CmdLineParser::CmdLineParser(int argc, char** argv)
  : mArgCount(argc),
    mArgs(argv),
    mShowHelp(false),
    mPreprocessOnly(false),
    mBuildDependencies(false),
    mShowLogo(false),
    mShowLicense(false),
    mInclusionPaths(),
    mReplacementTags()
{
  AddInclusionPaths(whf_current_dir());
}


void
CmdLineParser::Parse()
{
  int index = 1;

  if (index >= mArgCount)
    throw CmdLineException(_EXTRA(0), "No arguments provided. Use '--help' for information.");

  while (index < mArgCount)
  {
    if (areStrsEqual(mArgs[index], "-h")
        || areStrsEqual(mArgs[index], "--help"))
    {
      mShowHelp = true;
      ++index;
    }
    else if (areStrsEqual(mArgs[index], "-P"))
    {
      mPreprocessOnly = true;
      ++index;
    }
    else if (areStrsEqual(mArgs[index], "-I"))
    {
      if (++index >= mArgCount || mArgs[index][0] == '-')
        throw CmdLineException(_EXTRA(0), "Missing value for parameter '-I'.");

      AddInclusionPaths(mArgs[index++]);
    }
    else if (areStrsEqual(mArgs[index], "-D"))
    {
      if (++index >= mArgCount || mArgs[index][0] == '-')
        throw CmdLineException(_EXTRA(0), "Incorrect use of parameter '-D'.");

      else if (mArgs[index][0] == '=')
        throw CmdLineException(_EXTRA(0), "Missing tag name for parameter '-D'.");

      const string param(mArgs[index]);
      const size_t separatorPos = param.find('=');

      if (separatorPos == string::npos)
        throw CmdLineException(_EXTRA(0), "Missing tag value name for parameter '-D'.");

      else
      {
        const string value = param.substr(separatorPos + 1);
        if (value.empty())
          throw CmdLineException(_EXTRA(0), "Missing tag value name for parameter '-D'.");

        mReplacementTags.push_back(ReplacementTag(param.substr(0, separatorPos),
                                                  value,
                                                  ReplacementTag::CMDLINE_OFF));
      }
      ++index;
    }
    else if (areStrsEqual(mArgs[index], "--make_deps"))
    {
      mBuildDependencies = true;
      ++index;
    }
    else if (areStrsEqual(mArgs[index], "-o"))
    {
      if (++index >= mArgCount || mArgs[index][0] == '-')
        throw CmdLineException(_EXTRA(0), "Missing value for parameter '-o'.");

      else
        mOutputFile.push_back(mArgs[index++]);
    }
    else if (areStrsEqual(mArgs[index], "-v")
             || areStrsEqual(mArgs[index], "--version"))
    {
      mShowLogo = true;
      ++index;
    }
    else if (areStrsEqual(mArgs[index], "-l")
             || areStrsEqual(mArgs[index], "--license"))
    {
      mShowLicense = true;
      ++index;
    }
    else if (mArgs[index][0] != '-' && mArgs[index][0] != '\\')
      mSourceFile.push_back(mArgs[index++]);

    else
      throw CmdLineException(_EXTRA(0), "Cannot handle argument '%s.'", mArgs[index]);
  }

  CheckArguments();
}


void
CmdLineParser::AddInclusionPaths(const char* const paths)
{
  assert(paths != nullptr);

  const char *currentPath = paths;
  const char *nextPath    = currentPath;

  while (nextPath && (*nextPath != 0))
  {
    nextPath = strpbrk(currentPath, ";");
    if (nextPath == nullptr)
    {
      string path(currentPath);
      mInclusionPaths.push_back(NormalizeFilePath(path, true));
    }
    else
    {
      const uint_t pathSize = nextPath - currentPath;
      if (pathSize > 0)
      {
        string path(currentPath, pathSize);
        mInclusionPaths.push_back(NormalizeFilePath(path, true));
      }
      currentPath = ++nextPath;
    }
  }
}


void
CmdLineParser::CheckArguments()
{
  if (mShowHelp)
  {
    DisplayUsage();
    exit(0);
  }
  else if (mShowLicense)
  {
    displayLicenseInformation(cout, sProgramName, sProgramDesc);
    exit(0);
  }
  else if (mShowLogo)
  {
    displayBanner(cout, sProgramName, WVER_MAJ, WVER_MIN);
    exit(0);
  }
  else if (mSourceFile.size() == 0)
    throw CmdLineException(_EXTRA(0), "The input file was not specified.");

  else if (mOutputFile.size() < mSourceFile.size())
  {
    for (auto i = mOutputFile.size(); i < mSourceFile.size(); ++i)
    {
      const auto& input = mSourceFile[i];
      mOutputFile.push_back(input);

      if (input.find_last_of(inputFileExt) == input.length() - inputFileExt.length() + 1)
        mOutputFile[i] += 'o';

      else
        mOutputFile[i] += outputFileExt;
    }
  }

  const char* const defaultIncDirs = getenv("WHAIS_INC");
  if (defaultIncDirs != nullptr)
    AddInclusionPaths(defaultIncDirs);

  char temp[64];
  const auto t  = wh_get_currtime();

  snprintf(temp, sizeof temp, "%d", t.year);
  mReplacementTags.push_back(ReplacementTag("_YEAR_", temp));

  snprintf(temp, sizeof temp, "%02u", t.month);
  mReplacementTags.push_back(ReplacementTag("_MONTH_", temp));

  snprintf(temp, sizeof temp, "%02u", t.day);
  mReplacementTags.push_back(ReplacementTag("_DAY_", temp));

  snprintf(temp, sizeof temp, "%02u", t.hour);
  mReplacementTags.push_back(ReplacementTag("_HOUR_", temp));

  snprintf(temp, sizeof temp, "%02u", t.min);
  mReplacementTags.push_back(ReplacementTag("_MIN_", temp));

  snprintf(temp, sizeof temp, "%02u", t.sec);
  mReplacementTags.push_back(ReplacementTag("_SEC_", temp));

  snprintf(temp, sizeof temp, "%06u", t.usec);
  mReplacementTags.push_back(ReplacementTag("_USEC_", temp));

  snprintf(temp,
           sizeof temp,
           "%d/%02u/%02u %02u:%02u:%02u.%06u",
           t.year,
           t.month,
           t.day,
           t.hour,
           t.min,
           t.sec,
           t.usec);

  mReplacementTags.push_back(ReplacementTag("_TIME_STAMP_", temp));

  //Just place holders! These are supposed to be dealt with differently!
  mReplacementTags.push_back(ReplacementTag("_FILE_", ""));
  mReplacementTags.push_back(ReplacementTag("_LINE_", ""));
  mReplacementTags.push_back(ReplacementTag("_FLINE_", ""));
  mReplacementTags.push_back(ReplacementTag("_FUNC_", ""));
  mReplacementTags.push_back(ReplacementTag("_FUNCL_", ""));
}


void
CmdLineParser::DisplayUsage() const
{
  using namespace std;

  displayBanner(cout, sProgramName, WVER_MAJ, WVER_MIN);
  cout <<
    "Usage: whc [options] input_file\n"
    "Options:\n"
    "-D 'tag=text'   Define a replace tag(e.g -D 'user_name=The Coder' ).\n"
    "-h, --help      Display this help.\n"
    "-I 'dir1;di2'   Add a list of directories paths to the list of\n"
    "                directories used to search for included files.\n"
    "                (e.g -I '/usr/local/whais/inc;C:\\whais\\inc').\n"
    "--make_deps     Generate the dependencies list of this file(in a 'make'\n"
    "                recognized way) on the standard output.\n"
    "-o file         Use 'file' as the compilation output file.\n"
    "-P              Preprocess only. Display the result on standard output.\n"
    "-v, --version   Show version information.\n"
    "-l, --license   Print the license details.\n";
}


CmdLineException::CmdLineException(const uint32_t   code,
                                   const char      *file,
                                   const uint32_t   line,
                                   const char      *fmtMsg,
                                   ... )
  : Exception(code, file, line)
{
  if (fmtMsg != nullptr)
  {
    va_list vl;

    va_start(vl, fmtMsg);
    this->Message(fmtMsg, vl);
    va_end(vl);
  }
}

Exception*
CmdLineException::Clone() const
{
  return new CmdLineException(*this);
}

EXCEPTION_TYPE
CmdLineException::Type() const
{
  return COMPILER_CMD_LINE_EXCEPTION;
}

const char*
CmdLineException::Description() const
{
  return "Invalid command line.";
}


} //namespace whc
} //namespace whais
