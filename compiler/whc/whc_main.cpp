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

#include <iostream>
#include <assert.h>

#include "whisper.h"

#include "compiler/compiledunit.h"

#include "../../utils/include/wfile.h"
#include "../../utils/include/le_converter.h"
#include "../../utils/include/outstream.h"

#include "msglog.h"
#include "whc_cmdline.h"
#include "wo_format.h"

using namespace std;
using namespace whisper;

uint8_t wh_header[WHC_TABLE_SIZE] = { 0, };

static void
fill_globals_table (WICompiledUnit&      rUnit,
                    struct OutputStream* pSymbolsStream,
                    struct OutputStream* pGlblsTableStream)
{
  const uint_t globals_count = rUnit.GetGlobalsCount ();

  for (uint_t glbIt = 0; glbIt < globals_count; ++glbIt)
    {
      uint32_t glbTypeIndex = rUnit.GetGlobalTypeIndex (glbIt);
      uint32_t glbNameIndex = get_size_outstream (pSymbolsStream);

      if (rUnit.IsGlobalExternal (glbIt))
        glbTypeIndex |= EXTERN_MASK;

      if ((output_uint32 (pGlblsTableStream, glbTypeIndex) == NULL) ||
          (output_uint32 (pGlblsTableStream, glbNameIndex) == NULL) ||
          (output_data (pSymbolsStream,
                       _RC (const uint8_t *,
                            rUnit.RetriveGlobalName (glbIt)),
                            rUnit.GetGlobalNameLength (glbIt)) == NULL) ||
          (output_uint8 (pSymbolsStream, 0) == NULL))
        {
          throw bad_alloc ();
        }
    }
}

static void
process_procedures_table (WICompiledUnit&    rUnit,
                          WFile&             rDestFile,
                          OutputStream*      pSymbolsStream,
                          OutputStream*      pProcTableStream)
{
  const uint_t proc_count = rUnit.GetProceduresCount ();

  for (uint_t procIt = 0; procIt < proc_count; ++procIt)
    {
      const uint16_t localsCount  = rUnit.GetProcLocalsCount (procIt);
      const uint16_t paramsCound  = rUnit.GetProcParametersCount (procIt);
      const uint32_t procOff      = rDestFile.Tell ();
      const uint32_t procCodeSize = rUnit.GetProcCodeAreaSize (procIt);
      uint32_t       procRetType  = rUnit.GetProcReturnTypeIndex (procIt);

      assert (localsCount >= paramsCound);

      for (uint_t localIt = 0; localIt < localsCount; ++localIt)
        {
          uint32_t localTypeOff = rUnit.GetProcLocalTypeIndex (procIt,
                                                               localIt);
          to_le_int32 (_RC (uint8_t *, &localTypeOff));
          rDestFile.Write (_RC (uint8_t *, &localTypeOff), sizeof localTypeOff);
        }

      if (rUnit.IsProcExternal (procIt))
        procRetType |= EXTERN_MASK;
      else
        {
          uint8_t n_sync_stmts = rUnit.GetProcSyncStatementsCount (procIt);
          rDestFile.Write (_RC (uint8_t *, &n_sync_stmts),
                           sizeof n_sync_stmts);
          rDestFile.Write (rUnit.RetriveProcCodeArea (procIt),
                           rUnit.GetProcCodeAreaSize (procIt));
        }

      if ((output_uint32 (pProcTableStream, get_size_outstream (pSymbolsStream)) == NULL) ||
          (output_uint32 (pProcTableStream, procOff) == NULL) ||
          (output_uint32 (pProcTableStream, procRetType) == NULL) ||
          (output_uint16 (pProcTableStream, localsCount) == NULL) ||
          (output_uint16 (pProcTableStream, paramsCound) == NULL) ||
          (output_uint32 (pProcTableStream, procCodeSize) == NULL))
        {
          throw bad_alloc ();
        }

      if ((output_data (pSymbolsStream,
                           _RC (const uint8_t *, rUnit.RetriveProcName (procIt)),
                           rUnit.GetProcNameSize (procIt)) == NULL) ||
           (output_uint8 (pSymbolsStream, 0) == NULL))
        {
          throw bad_alloc ();
        }
    }
}

int
main (int argc, char **argv)
{
  uint_t            buffSize = 0;
  int               retCode  = 0;
  uint_t            langVerMaj;
  uint_t            langVerMin;
  auto_ptr<uint8_t> buffer (NULL);
  OutputStream      symbolsStream;
  OutputStream      glbsTableStream;
  OutputStream      procsTableStream;

  wh_get_lang_ver (&langVerMaj, &langVerMin);
  init_outstream (OUTSTREAM_INCREMENT_SIZE, &symbolsStream);
  init_outstream (OUTSTREAM_INCREMENT_SIZE, &glbsTableStream);
  init_outstream (OUTSTREAM_INCREMENT_SIZE, &procsTableStream);

  try
  {

    WhcCmdLineParser args (argc, argv);

    WFile inputFile (args.GetSourceFile (), WHC_FILEREAD);
    buffSize = inputFile.GetSize ();

    if (buffSize >= 0xFFFFFFFE)
      throw - 1;                // we can not handle files these size anyway

    buffer.reset (new uint8_t[_SC (unsigned int, buffSize + 1)]);
    buffer.get ()[buffSize] = 0;  //ensures the null terminator

    inputFile.Seek (0, WHC_SEEK_CURR);
    inputFile.Read (buffer.get (), _SC (unsigned int, buffSize));
    assert (inputFile.Tell () == buffSize);
    inputFile.Close ();

    WBufferCompiledUnit unit (buffer.get (), buffSize, my_postman, buffer.get ());
    WFile               outputFile (args.GetOutputFile (), WHC_FILEWRITE | WHC_FILECREATE);

    outputFile.SetSize (0);
    outputFile.Seek (0, WHC_SEEK_BEGIN);

    //reserve space for header file
    outputFile.Write (wh_header, sizeof wh_header);

    process_procedures_table (unit, outputFile, &symbolsStream, &procsTableStream);
    fill_globals_table (unit, &symbolsStream, &glbsTableStream);

    *_RC(uint32_t*, wh_header + WHC_GLOBS_COUNT_OFF) = get_size_outstream (&glbsTableStream) /
                                                        WHC_GLOBAL_ENTRY_SIZE;
    assert ((get_size_outstream (&glbsTableStream) % WHC_GLOBAL_ENTRY_SIZE) == 0);

    *_RC (uint32_t*, wh_header + WHC_PROCS_COUNT_OFF) = get_size_outstream (&procsTableStream) /
                                                         WHC_PROC_ENTRY_SIZE;

    assert ((get_size_outstream (&procsTableStream) % WHC_PROC_ENTRY_SIZE) == 0);

    *_RC (uint32_t *, wh_header + WHC_TYPEINFO_START_OFF) = outputFile.Tell ();
    *_RC (uint32_t *, wh_header + WHC_TYPEINFO_SIZE_OFF)  = unit.GetTypeInformationSize ();

    outputFile.Write (unit.RetriveTypeInformation (), unit.GetTypeInformationSize ());

    *_RC (uint32_t *, wh_header + WHC_SYMTABLE_START_OFF) = outputFile.Tell ();
    *_RC (uint32_t *, wh_header + WHC_SYMTABLE_SIZE_OFF)  = get_size_outstream (&symbolsStream);

    outputFile.Write (get_buffer_outstream (&symbolsStream), get_size_outstream (&symbolsStream));


    *_RC (uint32_t *, wh_header + WHC_CONSTAREA_START_OFF) = outputFile.Tell ();
    *_RC (uint32_t *, wh_header + WHC_CONSTAREA_SIZE_OFF) = unit.GetConstAreaSize ();

    outputFile.Write (unit.RetrieveConstArea (), unit.GetConstAreaSize ());

    outputFile.Write (get_buffer_outstream (&glbsTableStream),
                      get_size_outstream (&glbsTableStream));
    outputFile.Write (get_buffer_outstream (&procsTableStream),
                      get_size_outstream (&procsTableStream));

    wh_header[WHC_SIGNATURE_OFF]     = WH_SIGNATURE[0];
    wh_header[WHC_SIGNATURE_OFF + 1] = WH_SIGNATURE[1];
    wh_header[WHC_FORMATMMAJ_OFF]    = WH_FFVER_MAJ;
    wh_header[WHC_FORMATMIN_OFF]     = WH_FFVER_MIN;
    wh_header[WHC_LANGVER_MAJ_OFF]   = langVerMaj;
    wh_header[WHC_LANGVER_MIN_OFF]   = langVerMin;

    to_le_int32 (wh_header + WHC_GLOBS_COUNT_OFF);
    to_le_int32 (wh_header + WHC_PROCS_COUNT_OFF);
    to_le_int32 (wh_header + WHC_TYPEINFO_START_OFF);
    to_le_int32 (wh_header + WHC_TYPEINFO_SIZE_OFF);
    to_le_int32 (wh_header + WHC_SYMTABLE_START_OFF);
    to_le_int32 (wh_header + WHC_SYMTABLE_SIZE_OFF);

    outputFile.Seek (0, WHC_SEEK_BEGIN);
    outputFile.Write (wh_header, sizeof wh_header);
    outputFile.Sync ();

  }
  catch (WFileException & e)
  {
    std::cerr << "File IO error: " << e.Extra ();
    if (e.Message () != NULL)
      std::cerr << ": " << e.Message () << std::endl;
    else
      std::cerr << '.' << std::endl;
    retCode = -1;
  }
  catch (WCompiledUnitException&)
  {
    retCode = -1;
  }
  catch (WhcCmdLineException& e)
  {
    std::cerr << e.Message () << std::endl;
  }
  catch (Exception & e)
  {
    std::cerr << "error : " << e.Message () << std::endl;
    std::cerr << "file: " << e.File() << " : " << e.Line() << std::endl;
    std::cerr << "Extra: " << e.Extra() << std::endl;
    retCode = -1;
  }
  catch (std::bad_alloc&)
  {
    std::cerr << "Memory allocation failed!" << std::endl;
    retCode = -1;
  }
  catch (...)
  {
    std::cerr << "Unknown exception thrown!" << std::endl;
    retCode = -1;
  }

  destroy_outstream (&symbolsStream);
  destroy_outstream (&glbsTableStream);
  destroy_outstream (&procsTableStream);

  return retCode;
}

#ifdef ENABLE_MEMORY_TRACE
uint32_t WMemoryTracker::smInitCount = 0;
const char* WMemoryTracker::smModule = "WHC";
#endif
