#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include "dbs/dbs_mgr.h"
#include "interpreter.h"
#include "custom/include/test/test_fmw.h"

#include "interpreter/prima/pm_interpreter.h"
#include "interpreter/prima/pm_processor.h"
#include "compiler//wopcodes.h"

using namespace prima;

static const char admin[] = "administrator";
static const char procName[] = "p1";

const uint8_t dummyProgram[] = ""
    "PROCEDURE p1 (a1 AS INT8, a2 AS INT8) RETURN BOOL\n"
    "DO\n"
      "LET hd1, hd2 AS HIRESTIME\n;"
      "\n"
      "hd1 = '2012/12/12 13:00:00.100';\n"
      "hd2 = '2012/12/11 13:00:00';\n"
      "RETURN hd1 < hd2;\n"
    "ENDPROC\n"
    "\n";

static const char *MSG_PREFIX[] = {
                                      "", "error ", "warning ", "error "
                                    };

static uint_t
get_line_from_buffer (const char * buffer, uint_t buff_pos)
{
  uint_t count = 0;
  int result = 1;

  if (buff_pos == WHC_IGNORE_BUFFER_POS)
    return -1;

  while (count < buff_pos)
    {
      if (buffer[count] == '\n')
        ++result;
      else if (buffer[count] == 0)
        {
          assert (0);
        }
      ++count;
    }
  return result;
}

void
my_postman (WHC_MESSENGER_ARG data,
            uint_t            buff_pos,
            uint_t            msg_id,
            uint_t            msgType,
            const char*     pMsgFormat,
            va_list           args)
{
  const char *buffer = (const char *) data;
  int buff_line = get_line_from_buffer (buffer, buff_pos);

  fprintf (stderr, MSG_PREFIX[msgType]);
  fprintf (stderr, "%d : line %d: ", msg_id, buff_line);
  vfprintf (stderr, pMsgFormat, args);
  fprintf (stderr, "\n");
};

static uint_t
w_encode_opcode (W_OPCODE opcode, uint8_t* pOutCode)
{
  if (opcode >= 0x80)
    {
      uint16_t temp = opcode;
      temp |= 0x8000;
      pOutCode [0] = (temp & 0xFF);
      pOutCode [1] = ((temp >> 8) & 0xFF);

      return 2;
    }
  pOutCode [0] = opcode;
  return 1;
}

template <typename DBS_T> bool
test_op_eqXX (Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " equality ...\n";
  const uint32_t procId = session.FindProcedure (
                                              _RC (const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  uint8_t* testCode = _CC (uint8_t*, session.ProcCode (procId));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode (opcode, testCode + opSize);
  w_encode_opcode (W_RET, testCode + opSize);

  stack.Push (first);
  stack.Push (second);

  session.ExecuteProcedure (procName, stack);

  if (stack.Size () != 1)
    return false;

  DBSBool result;
  stack[0].GetOperand ().GetValue (result);

  if (result.IsNull ())
    return false;

  return result == DBSBool ((first == second));
}

template <typename DBS_T> bool
test_op_neXX (Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " inequality ...\n";
  const uint32_t procId = session.FindProcedure (
                                              _RC (const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  uint8_t* testCode = _CC (uint8_t*, session.ProcCode (procId));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode (opcode, testCode + opSize);
  w_encode_opcode (W_RET, testCode + opSize);

  stack.Push (first);
  stack.Push (second);

  session.ExecuteProcedure (procName, stack);

  if (stack.Size () != 1)
    return false;

  DBSBool result;
  stack[0].GetOperand ().GetValue (result);

  if (result.IsNull ())
    return false;

  return result == DBSBool ((first != second));
}

template <typename DBS_T> bool
test_op_ltXX (Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " less than ...\n";
  const uint32_t procId = session.FindProcedure (
                                              _RC (const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  uint8_t* testCode = _CC (uint8_t*, session.ProcCode (procId));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode (opcode, testCode + opSize);
  w_encode_opcode (W_RET, testCode + opSize);

  stack.Push (first);
  stack.Push (second);

  session.ExecuteProcedure (procName, stack);

  if (stack.Size () != 1)
    return false;

  DBSBool result;
  stack[0].GetOperand ().GetValue (result);

  if (result.IsNull ())
    return false;

  return result == DBSBool ((first < second));
}

template <typename DBS_T> bool
test_op_leXX (Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " less or equal than ...\n";
  const uint32_t procId = session.FindProcedure (
                                              _RC (const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  uint8_t* testCode = _CC (uint8_t*, session.ProcCode (procId));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode (opcode, testCode + opSize);
  w_encode_opcode (W_RET, testCode + opSize);

  stack.Push (first);
  stack.Push (second);

  session.ExecuteProcedure (procName, stack);

  if (stack.Size () != 1)
    return false;

  DBSBool result;
  stack[0].GetOperand ().GetValue (result);

  if (result.IsNull ())
    return false;

  return result == DBSBool ((first <= second));
}

template <typename DBS_T> bool
test_op_gtXX (Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " greater than ...\n";
  const uint32_t procId = session.FindProcedure (
                                              _RC (const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  uint8_t* testCode = _CC (uint8_t*, session.ProcCode (procId));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode (opcode, testCode + opSize);
  w_encode_opcode (W_RET, testCode + opSize);

  stack.Push (first);
  stack.Push (second);

  session.ExecuteProcedure (procName, stack);

  if (stack.Size () != 1)
    return false;

  DBSBool result;
  stack[0].GetOperand ().GetValue (result);

  if (result.IsNull ())
    return false;

  return result == DBSBool ((first > second));
}

template <typename DBS_T> bool
test_op_geXX (Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " greater or equal than ...\n";
  const uint32_t procId = session.FindProcedure (
                                              _RC (const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  uint8_t* testCode = _CC (uint8_t*, session.ProcCode (procId));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode (W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode (opcode, testCode + opSize);
  w_encode_opcode (W_RET, testCode + opSize);

  stack.Push (first);
  stack.Push (second);

  session.ExecuteProcedure (procName, stack);

  if (stack.Size () != 1)
    return false;

  DBSBool result;
  stack[0].GetOperand ().GetValue (result);

  if (result.IsNull ())
    return false;

  return result == DBSBool ((first >= second));
}



int
main ()
{
  bool success = true;

  {
    DBSInit (DBSSettings ());
  }

  DBSCreateDatabase (admin);
  InitInterpreter ();

  {
    I_Session& commonSession = GetInstance (NULL);

    WBufferCompiledUnit dummy (dummyProgram,
                               sizeof dummyProgram,
                               my_postman,
                               dummyProgram);

    commonSession.LoadCompiledUnit (dummy);

    //Equality
    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "integer (same values)",
                                        W_EQ,
                                        DBSInt8 (),
                                        DBSInt8 ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "integer (diff values)",
                                        W_EQ,
                                        DBSInt8 (-10),
                                        DBSInt8 (1));


    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "bool (same values)",
                                        W_EQB,
                                        DBSBool (),
                                        DBSBool ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "bool (different values)",
                                        W_EQB,
                                        DBSBool (true),
                                        DBSBool (false));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "char (same values)",
                                        W_EQC,
                                        DBSChar (),
                                        DBSChar ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "char (different values)",
                                        W_EQC,
                                        DBSChar ('A'),
                                        DBSChar ('B'));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "date (same values)",
                                        W_EQD,
                                        DBSDate (),
                                        DBSDate ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "date (different values)",
                                        W_EQD,
                                        DBSDate (-123, 1,1),
                                        DBSDate (134, 1, 1));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "datetime (same values)",
                                        W_EQDT,
                                        DBSDateTime (),
                                        DBSDateTime ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "datetime (different values)",
                                        W_EQDT,
                                        DBSDateTime (-123, 1,1, 0, 59, 59 ),
                                        DBSDateTime (-123, 1, 2, 0, 0, 0));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "hirestime (same values)",
                                        W_EQHT,
                                        DBSDateTime (),
                                        DBSDateTime ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "hirestime (different values)",
                                        W_EQHT,
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59,0 ),
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59, 1));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "real (same values)",
                                        W_EQR,
                                        DBSReal (),
                                        DBSReal ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "real (different values)",
                                        W_EQR,
                                        DBSReal (-1.0 ),
                                        DBSReal ( 1.0));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "richreal (same values)",
                                        W_EQRR,
                                        DBSReal (),
                                        DBSReal ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "richreal (different values)",
                                        W_EQRR,
                                        DBSRichReal (-1.0 ),
                                        DBSRichReal ( 1.0));

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "text (same values)",
                                        W_EQT,
                                        DBSText (),
                                        DBSText ());

    success = success && test_op_eqXX (_SC (Session&, commonSession),
                                        "text (different values)",
                                        W_EQT,
                                        DBSText ("A"),
                                        DBSText ("B"));

    //Inequality

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "integer (same values)",
                                        W_NE,
                                        DBSInt8 (),
                                        DBSInt8 ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "integer (diff values)",
                                        W_NE,
                                        DBSInt8 (-10),
                                        DBSInt8 (1));


    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "bool (same values)",
                                        W_NEB,
                                        DBSBool (),
                                        DBSBool ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "bool (different values)",
                                        W_NEB,
                                        DBSBool (true),
                                        DBSBool (false));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "char (same values)",
                                        W_NEC,
                                        DBSChar (),
                                        DBSChar ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "char (different values)",
                                        W_NEC,
                                        DBSChar ('A'),
                                        DBSChar ('B'));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "date (same values)",
                                        W_NED,
                                        DBSDate (),
                                        DBSDate ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "date (different values)",
                                        W_NED,
                                        DBSDate (-123, 1,1),
                                        DBSDate (134, 1, 1));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "datetime (same values)",
                                        W_NEDT,
                                        DBSDateTime (),
                                        DBSDateTime ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "datetime (different values)",
                                        W_NEDT,
                                        DBSDateTime (-123, 1,1, 0, 59, 59 ),
                                        DBSDateTime (-123, 1, 2, 0, 0, 0));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "hirestime (same values)",
                                        W_NEHT,
                                        DBSDateTime (),
                                        DBSDateTime ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "hirestime (different values)",
                                        W_NEHT,
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59,0 ),
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59, 1));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "real (same values)",
                                        W_NER,
                                        DBSReal (),
                                        DBSReal ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "real (different values)",
                                        W_NER,
                                        DBSReal (-1.0 ),
                                        DBSReal ( 1.0));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "richreal (same values)",
                                        W_NERR,
                                        DBSReal (),
                                        DBSReal ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "richreal (different values)",
                                        W_NERR,
                                        DBSRichReal (-1.0 ),
                                        DBSRichReal ( 1.0));

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "text (same values)",
                                        W_NET,
                                        DBSText (),
                                        DBSText ());

    success = success && test_op_neXX (_SC (Session&, commonSession),
                                        "text (different values)",
                                        W_NET,
                                        DBSText ("A"),
                                        DBSText ("B"));

    //Smaller

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "integer",
                                        W_LT,
                                        DBSInt8 (-10),
                                        DBSInt8 (1));

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "char",
                                        W_LTC,
                                        DBSChar ('A'),
                                        DBSChar ('B'));

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "date",
                                        W_LTD,
                                        DBSDate (-123, 1,1),
                                        DBSDate (134, 1, 1));

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "datetime",
                                        W_LTDT,
                                        DBSDateTime (-123, 1,1, 0, 59, 59 ),
                                        DBSDateTime (-123, 1, 2, 0, 0, 0));

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "hirestime",
                                        W_LTHT,
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59,0 ),
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59, 1));

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "real",
                                        W_LTR,
                                        DBSReal (-1.0 ),
                                        DBSReal ( 1.0));

    success = success && test_op_ltXX (_SC (Session&, commonSession),
                                        "richreal",
                                        W_LTRR,
                                        DBSRichReal (-1.0 ),
                                        DBSRichReal ( 1.0));

    //Smaller or equal


    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "integer",
                                        W_LE,
                                        DBSInt8 (-10),
                                        DBSInt8 (1));

    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "char",
                                        W_LEC,
                                        DBSChar ('A'),
                                        DBSChar ('B'));

    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "date",
                                        W_LED,
                                        DBSDate (-123, 1,1),
                                        DBSDate (134, 1, 1));

    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "datetime",
                                        W_LEDT,
                                        DBSDateTime (-123, 1,1, 0, 59, 59 ),
                                        DBSDateTime (-123, 1, 2, 0, 0, 0));

    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "hirestime",
                                        W_LEHT,
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59,0 ),
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59, 1));

    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "real",
                                        W_LER,
                                        DBSReal (-1.0 ),
                                        DBSReal ( 1.0));

    success = success && test_op_leXX (_SC (Session&, commonSession),
                                        "richreal",
                                        W_LERR,
                                        DBSRichReal (-1.0 ),
                                        DBSRichReal ( 1.0));

    //Greater


    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "integer",
                                        W_GT,
                                        DBSInt8 (-10),
                                        DBSInt8 (1));

    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "char",
                                        W_GTC,
                                        DBSChar ('A'),
                                        DBSChar ('B'));

    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "date",
                                        W_GTD,
                                        DBSDate (-123, 1,1),
                                        DBSDate (134, 1, 1));

    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "datetime",
                                        W_GTDT,
                                        DBSDateTime (-123, 1,1, 0, 59, 59 ),
                                        DBSDateTime (-123, 1, 2, 0, 0, 0));

    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "hirestime",
                                        W_GTHT,
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59,0 ),
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59, 1));

    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "real",
                                        W_GTR,
                                        DBSReal (-1.0 ),
                                        DBSReal ( 1.0));

    success = success && test_op_gtXX (_SC (Session&, commonSession),
                                        "richreal",
                                        W_GTRR,
                                        DBSRichReal (-1.0 ),
                                        DBSRichReal ( 1.0));


    //Greater or equal than

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "integer",
                                        W_GE,
                                        DBSInt8 (-10),
                                        DBSInt8 (1));

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "char",
                                        W_GEC,
                                        DBSChar ('A'),
                                        DBSChar ('B'));

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "date",
                                        W_GED,
                                        DBSDate (-123, 1,1),
                                        DBSDate (134, 1, 1));

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "datetime",
                                        W_GEDT,
                                        DBSDateTime (-123, 1,1, 0, 59, 59 ),
                                        DBSDateTime (-123, 1, 2, 0, 0, 0));

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "hirestime",
                                        W_GEHT,
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59,0 ),
                                        DBSHiresTime (-123, 1, 1, 0, 58, 59, 1));

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "real",
                                        W_GER,
                                        DBSReal (-1.0 ),
                                        DBSReal ( 1.0));

    success = success && test_op_geXX (_SC (Session&, commonSession),
                                        "richreal",
                                        W_GERR,
                                        DBSRichReal (-1.0 ),
                                        DBSRichReal ( 1.0));


    ReleaseInstance (commonSession);
  }

  CleanInterpreter ();
  DBSRemoveDatabase (admin);
  DBSShoutdown ();

  if (!success)
    {
      std::cout << "TEST RESULT: FAIL" << std::endl;
      return 1;
    }

  std::cout << "TEST RESULT: PASS" << std::endl;

  return 0;
}

#ifdef ENABLE_MEMORY_TRACE
uint32_t WMemoryTracker::sm_InitCount = 0;
const char* WMemoryTracker::sm_Module = "T";
#endif
