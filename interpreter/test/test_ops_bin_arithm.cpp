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

using namespace whais;
using namespace prima;

static const char admin[] = "administrator";
static const char procName[] = "p1";

const uint8_t dummyProgram[] = ""
    "PROCEDURE p1(a1 INT8, a2 INT8) RETURN BOOL\n"
    "DO\n"
      "VAR hd1, hd2 HIRESTIME\n;"
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
get_line_from_buffer(const char * buffer, uint_t buff_pos)
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
          assert(0);
        }
      ++count;
    }
  return result;
}

void
my_postman(WH_MESSENGER_CTXT data,
            uint_t            buff_pos,
            uint_t            msg_id,
            uint_t            msgType,
            const char*     pMsgFormat,
            va_list           args)
{
  const char *buffer = (const char *) data;
  int buff_line = get_line_from_buffer(buffer, buff_pos);

  fprintf(stderr, MSG_PREFIX[msgType]);
  fprintf(stderr, "%d : line %d: ", msg_id, buff_line);
  vfprintf(stderr, pMsgFormat, args);
  fprintf(stderr, "\n");
};

static uint_t
w_encode_opcode(W_OPCODE opcode, uint8_t* pOutCode)
{
  return wh_compiler_encode_op(pOutCode, opcode);
}


template <typename DBS_T> bool
test_op_addXX(Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " addition...\n";
  const uint32_t procId = session.FindProcedure(
                                              _RC(const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  const Procedure& proc   = session.GetProcedure(procId);
  uint8_t* testCode = _CC(uint8_t*, proc.mProcMgr->Code(proc, nullptr));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode(opcode, testCode + opSize);
  w_encode_opcode(W_RET, testCode + opSize);

  stack.Push(first);
  stack.Push(second);

  session.ExecuteProcedure(procName, stack);

  if (stack.Size() != 1)
    return false;

  DBS_T result;
  stack[0].Operand().GetValue(result);

  if (first.IsNull() ||  second.IsNull())
    return result.IsNull();

  return result == DBS_T(first.mValue + second.mValue);

}

static bool
test_op_addt(Session& session,
              const char* pDesc,
              DText first,
              DText second)
{
  std::cout << "Testing " << pDesc << " addition...\n";
  const uint32_t procId = session.FindProcedure(
                                              _RC(const uint8_t*, procName),
                                              sizeof procName - 1
                                                );

  const Procedure& proc   = session.GetProcedure(procId);
  uint8_t* testCode = _CC(uint8_t*, proc.mProcMgr->Code(proc, nullptr));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode(W_ADDT, testCode + opSize);
  w_encode_opcode(W_RET, testCode + opSize);

  stack.Push(first);
  stack.Push(second);

  session.ExecuteProcedure(procName, stack);

  if (stack.Size() != 1)
    return false;

  DText result;
  stack[0].Operand().GetValue(result);

  if (result.IsNull())
    return false;

  first.Append(second);

  return result == first;
}

template <typename DBS_T> bool
test_op_subXX(Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " substraction...\n";
  const uint32_t procId = session.FindProcedure(
                                              _RC(const uint8_t*, procName),
                                              sizeof procName - 1
                                                );
  const Procedure& proc   = session.GetProcedure(procId);
  uint8_t* testCode = _CC(uint8_t*, proc.mProcMgr->Code(proc, nullptr));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode(opcode, testCode + opSize);
  w_encode_opcode(W_RET, testCode + opSize);

  stack.Push(first);
  stack.Push(second);

  session.ExecuteProcedure(procName, stack);

  if (stack.Size() != 1)
    return false;

  DBS_T result;
  stack[0].Operand().GetValue(result);

  if (first.IsNull() ||  second.IsNull())
    return result.IsNull();

  return result == DBS_T(first.mValue - second.mValue);
}

template <typename DBS_T> bool
test_op_mulXX(Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " multiplication ...\n";
  const uint32_t procId = session.FindProcedure(
                                              _RC(const uint8_t*, procName),
                                              sizeof procName - 1
                                                );
  const Procedure& proc   = session.GetProcedure(procId);
  uint8_t* testCode = _CC(uint8_t*, proc.mProcMgr->Code(proc, nullptr));

  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode(opcode, testCode + opSize);
  w_encode_opcode(W_RET, testCode + opSize);

  stack.Push(first);
  stack.Push(second);

  session.ExecuteProcedure(procName, stack);

  if (stack.Size() != 1)
    return false;

  DBS_T result;
  stack[0].Operand().GetValue(result);

  if (first.IsNull() ||  second.IsNull())
    return result.IsNull();

  return result == DBS_T(first.mValue * second.mValue);
}

template <typename DBS_T> bool
test_op_divXX(Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DBS_T first,
               const DBS_T second)
{
  std::cout << "Testing " << desc << " divide ...\n";
  const uint32_t procId = session.FindProcedure(
                                              _RC(const uint8_t*, procName),
                                              sizeof procName - 1
                                                );
  const Procedure& proc   = session.GetProcedure(procId);
  uint8_t* testCode = _CC(uint8_t*, proc.mProcMgr->Code(proc, nullptr));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode(opcode, testCode + opSize);
  w_encode_opcode(W_RET, testCode + opSize);

  stack.Push(first);
  stack.Push(second);

  session.ExecuteProcedure(procName, stack);

  if (stack.Size() != 1)
    return false;

  DBS_T result;
  stack[0].Operand().GetValue(result);

  if (first.IsNull() ||  second.IsNull())
    return result.IsNull();

  return result == DBS_T(first.mValue / second.mValue);
}

static bool
test_op_mod(Session& session,
               const char* desc,
               const W_OPCODE opcode,
               const DUInt64 first,
               const DUInt64 second)
{
  std::cout << "Testing " << desc << " modulo ...\n";
  const uint32_t procId = session.FindProcedure(
                                              _RC(const uint8_t*, procName),
                                              sizeof procName - 1
                                                );
  const Procedure& proc   = session.GetProcedure(procId);
  uint8_t* testCode = _CC(uint8_t*, proc.mProcMgr->Code(proc, nullptr));
  SessionStack stack;

  uint8_t opSize = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode);
  testCode[opSize++] = 0;
  opSize += w_encode_opcode(W_LDLO8, testCode + opSize);
  testCode[opSize++] = 1;
  opSize += w_encode_opcode(opcode, testCode + opSize);
  w_encode_opcode(W_RET, testCode + opSize);

  stack.Push(first);
  stack.Push(second);

  session.ExecuteProcedure(procName, stack);

  if (stack.Size() != 1)
    return false;

  DUInt64 result;
  stack[0].Operand().GetValue(result);

  if (first.IsNull() ||  second.IsNull())
    return result.IsNull();

  return result == DUInt64(first.mValue % second.mValue);
}


int
main()
{
  bool success = true;

  {
    DBSInit(DBSSettings());
  }

  DBSCreateDatabase(admin);
  InitInterpreter();

  {
    ISession& commonSession = GetInstance(nullptr);

    CompiledBufferUnit dummy(dummyProgram,
                               sizeof dummyProgram,
                               my_postman,
                               dummyProgram);

    commonSession.LoadCompiledUnit(dummy);

    //Addition

    success = success && test_op_addt(_SC(Session&, commonSession),
                                        "text(first null)",
                                        DText(),
                                        DText("World!"));
    success = success && test_op_addt(_SC(Session&, commonSession),
                                        "text(second null)",
                                        DText("Hello, "),
                                        DText());

    success = success && test_op_addt(_SC(Session&, commonSession),
                                        "text(no nulls)",
                                        DText("Hello, "),
                                        DText("World!"));

    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "integer",
                                        W_ADD,
                                        DUInt64(10),
                                        DUInt64(20));

    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "integer with first null",
                                        W_ADD,
                                        DUInt8(),
                                        DUInt8(20));

    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "integer with second null",
                                        W_ADD,
                                        DInt16(10),
                                        DInt16());
    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "real",
                                        W_ADDRR,
                                        DReal(10.00),
                                        DReal(20.00));
    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "real with first null",
                                        W_ADDRR,
                                        DReal(),
                                        DReal(20.00));
    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "real with second null",
                                        W_ADDRR,
                                        DReal(10.00),
                                        DReal());

    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "richreal",
                                        W_ADDRR,
                                        DRichReal(10.00),
                                        DRichReal(20.00));
    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "richreal with first null",
                                        W_ADDRR,
                                        DRichReal(),
                                        DRichReal(20.00));
    success = success && test_op_addXX(_SC(Session&, commonSession),
                                        "richreal with second null",
                                        W_ADDRR,
                                        DRichReal(10.00),
                                        DRichReal());


    //Substraction

    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "integer",
                                        W_SUB,
                                        DUInt64(10),
                                        DUInt64(20));

    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "integer with first null",
                                        W_SUB,
                                        DUInt8(),
                                        DUInt8(20));

    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "integer with second null",
                                        W_SUB,
                                        DInt16(10),
                                        DInt16());
    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "real",
                                        W_SUBRR,
                                        DReal(10.00),
                                        DReal(20.00));
    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "real with first null",
                                        W_SUBRR,
                                        DReal(),
                                        DReal(20.00));
    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "real with second null",
                                        W_SUBRR,
                                        DReal(10.00),
                                        DReal());

    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "richreal",
                                        W_SUBRR,
                                        DRichReal(10.00),
                                        DRichReal(20.00));
    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "richreal with first null",
                                        W_SUBRR,
                                        DRichReal(),
                                        DRichReal(20.00));
    success = success && test_op_subXX(_SC(Session&, commonSession),
                                        "richreal with second null",
                                        W_SUBRR,
                                        DRichReal(10.00),
                                        DRichReal());

    //Multiplication

    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "integer",
                                        W_MUL,
                                        DUInt64(10),
                                        DUInt64(20));

    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "integer with first null",
                                        W_MUL,
                                        DUInt8(),
                                        DUInt8(20));

    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "integer with second null",
                                        W_MUL,
                                        DInt16(10),
                                        DInt16());
    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "real",
                                        W_MULRR,
                                        DReal(10.00),
                                        DReal(20.00));
    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "real with first null",
                                        W_MULRR,
                                        DReal(),
                                        DReal(20.00));
    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "real with second null",
                                        W_MULRR,
                                        DReal(10.00),
                                        DReal());

    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "richreal",
                                        W_MULRR,
                                        DRichReal(10.00),
                                        DRichReal(20.00));
    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "richreal with first null",
                                        W_MULRR,
                                        DRichReal(),
                                        DRichReal(20.00));
    success = success && test_op_mulXX(_SC(Session&, commonSession),
                                        "richreal with second null",
                                        W_MULRR,
                                        DRichReal(10.00),
                                        DRichReal());

    //Divide

    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "integer",
                                        W_DIV,
                                        DUInt64(10),
                                        DUInt64(20));

    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "integer with first null",
                                        W_DIV,
                                        DUInt8(),
                                        DUInt8(20));

    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "integer with second null",
                                        W_DIV,
                                        DInt16(10),
                                        DInt16());

    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "real",
                                        W_DIVRR,
                                        DReal(10.00),
                                        DReal(20.00));
    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "real with first null",
                                        W_DIVRR,
                                        DReal(),
                                        DReal(20.00));
    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "real with second null",
                                        W_DIVRR,
                                        DReal(10.00),
                                        DReal());

    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "richreal",
                                        W_DIVRR,
                                        DRichReal(10.00),
                                        DRichReal(20.00));
    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "richreal with first null",
                                        W_DIVRR,
                                        DRichReal(),
                                        DRichReal(20.00));
    success = success && test_op_divXX(_SC(Session&, commonSession),
                                        "richreal with second null",
                                        W_DIVRR,
                                        DRichReal(10.00),
                                        DRichReal());


    //Modulo

    success = success && test_op_mod(_SC(Session&, commonSession),
                                      "integer",
                                      W_MOD,
                                      DUInt64(10),
                                      DUInt64(20));

    success = success && test_op_mod(_SC(Session&, commonSession),
                                      "integer with first null",
                                      W_MOD,
                                      DUInt64(),
                                      DUInt64(20));

    success = success && test_op_mod(_SC(Session&, commonSession),
                                      "integer with second null",
                                      W_MOD,
                                      DUInt64(10),
                                      DUInt64());

    ReleaseInstance(commonSession);
  }

  CleanInterpreter();
  DBSRemoveDatabase(admin);
  DBSShoutdown();

  if (!success)
    {
      std::cout << "TEST RESULT: FAIL" << std::endl;
      return 1;
    }

  std::cout << "TEST RESULT: PASS" << std::endl;

  return 0;
}

#ifdef ENABLE_MEMORY_TRACE
uint32_t WMemoryTracker::smInitCount = 0;
const char* WMemoryTracker::smModule = "T";
#endif
