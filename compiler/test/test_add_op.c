#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/parser.h"
#include "../semantics/vardecl.h"
#include "../semantics/opcodes.h"
#include "../semantics/procdecl.h"

#include "custom/include/test/test_fmw.h"

extern int yyparse(struct ParserState *);

static void
init_state_for_test(struct ParserState *state, const char * buffer)
{
  state->buffer = buffer;
  state->strings = create_string_store();
  state->bufferSize = strlen(buffer);
  wh_array_init(&state->values, sizeof(struct SemValue));

  init_glbl_stmt(&state->globalStmt);
  state->pCurrentStmt = &state->globalStmt;
}

static void
free_state(struct ParserState *state)
{
  release_string_store(state->strings);
  clear_glbl_stmt(&(state->globalStmt));
  wh_array_clean(&state->values);
}

static bool_t
check_used_vals(struct ParserState *state)
{
  int vals_count = wh_array_count(&state->values);
  while (--vals_count >= 0)
    {
      struct SemValue *val = wh_array_get(&state->values, vals_count);
      if (val->val_type != VAL_REUSE)
        {
          return TRUE;                /* found value still in use */
        }

    }

  return FALSE;                        /* no value in use */
}

char proc_decl_buffer[] =
  "PROCEDURE ProcId1(v1 INT8, v2 INT8) RETURN INT32 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId2(v1 INT8, v2 INT16) RETURN INT16 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId3(v1 INT8, v2 INT32) RETURN INT32 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId4(v1 INT8, v2 INT64) RETURN INT64 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId5(v1 INT8, v2 REAL) RETURN REAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId6(v1 INT8, v2 RICHREAL) RETURN RICHREAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId7(v1 INT16, v2 INT16) RETURN INT16 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId8(v1 INT16, v2 INT32) RETURN INT32 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId9(v1 INT16, v2 INT64) RETURN INT64 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId10(v1 INT16, v2 REAL) RETURN REAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId11(v1 INT16, v2 RICHREAL) RETURN RICHREAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId12(v1 INT32, v2 INT32) RETURN INT32 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId13(v1 INT32, v2 INT64) RETURN INT64 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId14(v1 INT32, v2 REAL) RETURN REAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId15(v1 INT32, v2 RICHREAL) RETURN RICHREAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId16(v1 INT64, v2 INT64) RETURN INT64 "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId17(v1 INT64, v2 REAL) RETURN REAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId18(v1 INT64, v2 RICHREAL) RETURN RICHREAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId19(v1 REAL, v2 REAL) RETURN REAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId20(v1 REAL, v2 RICHREAL) RETURN RICHREAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId21(v1 RICHREAL, v2 RICHREAL) RETURN RICHREAL "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId22(v1 TEXT, v2 BOOL) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId23(v1 TEXT, v2 CHAR) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId24(v1 TEXT, v2 DATE) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId25(v1 TEXT, v2 DATETIME) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId26(v1 TEXT, v2 HIRESTIME) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId27(v1 TEXT, v2 INT8) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId28(v1 TEXT, v2 INT16) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId29(v1 TEXT, v2 INT32) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId30(v1 TEXT, v2 INT64) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId31(v1 TEXT, v2 REAL) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId32(v1 TEXT, v2 RICHREAL) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId33(v1 TEXT, v2 UINT8) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId34(v1 TEXT, v2 UINT16) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId35(v1 TEXT, v2 UINT32) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId36(v1 TEXT, v2 UINT64) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"
  ""
  "PROCEDURE ProcId37(v1 TEXT, v2 TEXT) RETURN TEXT "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"

  "PROCEDURE AProcId0(v1 UINT8 ARRAY, v2 INT32) RETURN UINT8 ARRAY "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"

  "PROCEDURE AProcId1(v1 DATE ARRAY, v2 DATETIME ARRAY) RETURN DATE ARRAY "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"

  "PROCEDURE AProcId2(v1 RICHREAL ARRAY, v2 INT8) RETURN RICHREAL ARRAY "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n"

  "PROCEDURE AProcId3(v1 REAL ARRAY, v2 UINT32 ARRAY) RETURN REAL ARRAY "
  "DO "
  "RETURN v1 + v2; "
  "ENDPROC\n\n";

static bool_t
check_op_symmetry()
{
  int i, j;
  for (i = T_END_OF_TYPES - 1, j = 0;
       (i >= 0) && (j < T_END_OF_TYPES); i--, j++)
    {
      if (add_op[i][j] != add_op[j][i])
        {
          return FALSE;
        }
    }

  if ((i >= 0) || (j != T_END_OF_TYPES))
    {
      /* weird error */
      return FALSE;
    }
  return TRUE;
}

static bool_t
check_procedure(struct ParserState *state, char * proc_name)
{
  struct Statement *stmt = find_proc_decl(state, proc_name, strlen(proc_name), FALSE);
  struct DeclaredVar *var = (struct DeclaredVar *)wh_array_get( &stmt->spec.proc.paramsList, 0);
  uint8_t *code = wh_ostream_data(stmt_query_instrs(stmt));
  int code_size = wh_ostream_size(stmt_query_instrs(stmt));

  const uint_t opcodeOff = 2 * (opcode_bytes(W_LDLO8) + 1);
  uint_t codeSize = 0;
  enum W_OPCODE op_expect = W_NA;

  /* check the opcode based on the return type */
  switch (var->type)
  {
  case T_INT8:
  case T_INT16:
  case T_INT32:
  case T_INT64:
    op_expect = W_ADD;
    break;
  case T_REAL:
  case T_RICHREAL:
    op_expect = W_ADDRR;
    break;
  case T_TEXT:
    op_expect = W_ADDT;
    break;
  default:
    /* we should no be here */
    return FALSE;
  }

  codeSize = opcodeOff + opcode_bytes(op_expect) + opcode_bytes(W_RET);
  if (code_size < codeSize)
    return FALSE;

  else if (decode_opcode(code + opcodeOff) != op_expect)
    return FALSE;

  return TRUE;
}

static bool_t
check_array_procedure(struct ParserState *state,
                      char * proc_name,
                      uint_t retType,
                      bool_t opBArray)
{
  struct Statement *stmt =
    find_proc_decl(state, proc_name, strlen(proc_name), FALSE);
  uint8_t *code = wh_ostream_data(stmt_query_instrs( stmt));
  const uint_t opCodeOffset = 2 * (opcode_bytes(W_LDLO8) + 1);

  if (decode_opcode(code + opCodeOffset) != W_AJOIN)
    return FALSE;

  if (opBArray ^ ((code[opCodeOffset + opcode_bytes(W_AJOIN)] & A_OPB_A_MASK) != 0))
    return FALSE;

  if ((code[opCodeOffset + opcode_bytes(W_AJOIN)] & A_SELF_MASK) != 0)
    return FALSE;

  if (GET_BASE_TYPE(code[opCodeOffset + opcode_bytes(W_AJOIN)]) != retType)
    return FALSE;

  return TRUE;
}

static bool_t
check_all_procs(struct ParserState *state)
{
  uint_t count;
  char proc_name[25];

  bool_t result = TRUE;
  for (count = 1; count <= 37; ++count)
    {
      sprintf(proc_name, "ProcId%d", count);
      if (check_procedure( state, proc_name) == FALSE)
        {
          result &=  FALSE;
        }
    }

  result = result & check_array_procedure(state, "AProcId0", T_UINT8, FALSE);
  result = result & check_array_procedure(state, "AProcId1", T_DATE, TRUE);
  result = result & check_array_procedure(state, "AProcId2", T_RICHREAL, FALSE);
  result = result & check_array_procedure(state, "AProcId3", T_REAL, TRUE);

  return result;
}

int
main()
{
  bool_t test_result = TRUE;
  struct ParserState state = { 0, };

  init_state_for_test(&state, proc_decl_buffer);

  printf("Testing parse..");
  if (yyparse( &state) != 0)
    {
      printf("FAILED\n");
      test_result = FALSE;
    }
  else
    {
      printf("PASSED\n");
    }

  if (test_result)
    {
      printf("Testing garbage vals...");
      if (check_used_vals( &state))
        {
          /* those should no be here */
          printf("FAILED\n");
          test_result = FALSE;
        }
      else
        {
          printf("PASSED\n");
        }
    }

  printf("Testing add op symmetry...");
  if (check_op_symmetry())
    {
      printf("PASSED\n");
    }
  else
    {
      printf("FAILED\n");
      test_result = FALSE;
    }

  printf("Testing add op usage ...");
  if (check_all_procs( &state))
    {
      printf("PASSED\n");
    }
  else
    {
      printf("FAILED\n");
      test_result = FALSE;
    }

  free_state(&state);
  printf("Memory peak: %u bytes \n", (uint_t)test_get_mem_peak());
  printf("Current memory usage: %u bytes...", (uint_t)test_get_mem_used());
  if (test_get_mem_used() != 0)
    {
      test_result = FALSE;
      printf("FAILED\n");
    }
  else
    {
      printf("PASSED\n");
    }

  if (test_result == FALSE)
    {
      printf("TEST RESULT: FAIL\n");
      return -1;
    }

  printf("TEST RESULT: PASS\n");
  return 0;
}
