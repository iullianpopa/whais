/*
 * test_session.cpp
 *
 *  Created on: Mar 9, 2012
 *      Author: ipopa
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include "compiler/compiledunit.h"
#include "dbs/dbs_mgr.h"
#include "interpreter.h"
#include "custom/include/test/test_fmw.h"

static const uint_t MAX_TABLE_FIELDS = 10;

using namespace whais;

struct TableFieldDesc
{
#if 0
  TableFieldDesc()
    : field_name(nullptr),
      field_type(0),
      desc_visited(false)
  {

  }
#endif

  const char*   field_name;
  uint16_t      field_type;
  bool          desc_visited;
};

struct GlobalDescs
{
  const char*    name;
  const uint16_t raw_type;
  uint16_t       fields_count;
  bool           desc_visited;
  TableFieldDesc table_fields[MAX_TABLE_FIELDS];
};

static const char admin[]    = "administrator";
static const char test_db1[] = "t_testdb_1";

static const uint_t ADMIN_GLBS_COUNT = 5;
static const uint_t USERS_GLBS_COUNT = 7;

static GlobalDescs admin_glbs [ADMIN_GLBS_COUNT] =
    {
        {"gb1_1", T_DATE, 0, false, },
        {"gb1_2", T_UINT32, 0, false, },
        {"gb1_3", T_ARRAY_MASK | T_UINT32, 0, false, },
        {"tab1", T_TABLE_MASK, 3, false,
            {
                {"tab1_f1", T_INT8, false},
                {"tab1_f2", T_TEXT, false},
                {"tab1_f3", T_ARRAY_MASK | T_DATE, false},
            }
        },
        {"field1", T_FIELD_MASK | T_TEXT, 0, false,}
    };

static const uint8_t commonCode[] =
    "VAR gb1_1 DATE;\n"
    "VAR gb1_2 UINT32;\n"
    "VAR gb1_3 UINT32 ARRAY;\n"
    "VAR tab1 TABLE(tab1_f1 INT8, tab1_f2 TEXT, tab1_f3 DATE ARRAY);\n"
    "VAR field1 TEXT FIELD;";

static GlobalDescs user_glbs [USERS_GLBS_COUNT] =
    {
        {"us1_1", T_DATETIME, 0, false, },
        {"us1_2", T_INT64, 0, false, },
        {"us1_3", T_ARRAY_MASK | T_DATE, 0, false, },
        {"tab2", T_TABLE_MASK, 3, false,
            {
                {"tab2_f1", T_INT8, false},
                {"tab2_f2", T_DATE, false},
                {"tab2_f3", T_ARRAY_MASK | T_DATE, false},
            }
        },
        {"tab3", T_TABLE_MASK, 2, false,
            {
                {"tab3_f1", T_CHAR, false},
                {"tab3_f2", T_ARRAY_MASK | T_INT8, false},
            }
        },
        {"field2", T_FIELD_MASK | T_DATETIME, 0, false, },
        {"tab4", T_TABLE_MASK, 1, false,
            {
                {"tab4_f1", T_TEXT, false},
            }
        }
    };

static const uint8_t userCode[] =
    "VAR us1_1 DATETIME;\n"
    "VAR us1_2 INT64;\n"
    "VAR us1_3 DATE ARRAY;\n"
    "VAR tab2 TABLE(tab2_f1 INT8, tab2_f2 DATE, tab2_f3 DATE ARRAY);\n"
    "VAR tab3 TABLE(tab3_f1 CHAR, tab3_f2 INT8 ARRAY);\n"
    "VAR field2 DATETIME FIELD;\n"
    "VAR tab4 TABLE(tab4_f1 TEXT);\n";


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
}

bool
load_unit(ISession&           session,
           const uint8_t* const unitCode,
           const uint_t         unitCodeSize)
{
  bool result = true;

  try
  {
    CompiledBufferUnit unit(unitCode,
                              unitCodeSize,
                              my_postman,
                              unitCode);
    session.LoadCompiledUnit(unit);
    std::cout << "Unit loaded successfully!" << std::endl;
  }
  catch(...)
  {
      std::cout << "Error! Unable to load unit!" << std::endl;
      result = false;
  }

  return result;
}

static GlobalDescs*
find_glb_desc(const char* const glb_name,
               GlobalDescs         glb_desc[],
               const uint_t        glb_count)
{
  for (uint_t i = 0; i < glb_count; ++i)
    {
      if (strcmp(glb_desc[i].name, glb_name) == 0)
        return &glb_desc[i];
    }

  return nullptr;
}

static TableFieldDesc*
find_field_desc(const char* const   name,
                 TableFieldDesc*       fields,
                 const uint_t          fieldsCount)
{
  for (uint_t index = 0; index < fieldsCount; ++index)
    {
      if (strcmp(name, fields[index].field_name) == 0)
        {
          if (fields[index].desc_visited)
            return nullptr;
          else
            return &fields[index];
        }
    }

  return nullptr;
}

static bool
test_fields_are_ok(ISession&            session,
                    const char*         glbName,
                    const uint_t          glbId,
                    TableFieldDesc*       fields,
                    const uint_t          fieldsCount)
{
  for (uint_t index = 0;
       index < session.GlobalValueFieldsCount(glbName);
       ++index)
    {
      const char* fname = session.GlobalValueFieldName(glbName, index);
      const uint_t  type  = session.GlobalValueFieldType(glbName, index);

      if ((fname != session.GlobalValueFieldName(glbId, index))
          || (type != session.GlobalValueFieldType(glbId, index)))
        {
          return false;
        }

      TableFieldDesc* desc = find_field_desc(fname,
                                              fields,
                                              fieldsCount);
      if (desc->field_type != type)
        return false;

      desc->desc_visited = true;
    }

  for (uint_t index = 0; index < fieldsCount; ++index)
    {
      if (! fields[index].desc_visited)
        return false;
    }

  try
  {
      session.GlobalValueFieldName(glbName, fieldsCount);
      return false;
  }
  catch(DBSException&)
  {
  }
  catch(InterException&)
  {
  }

  try
  {
      session.GlobalValueFieldType(glbName, fieldsCount);
      return false;
  }
  catch(DBSException&)
  {
  }
  catch(InterException&)
  {
  }

  return true;
}

bool test_globals(ISession&     session,
                   GlobalDescs    glb_desc[],
                   const uint_t   glbs_count)
{
  if (session.GlobalValuesCount() != glbs_count)
    return false;

  for (uint_t glb_i = 0; glb_i < glbs_count; ++glb_i)
    {
      const char* glb_name = (char*)session.GlobalValueName(glb_i);
      GlobalDescs*  glb = find_glb_desc(glb_name, glb_desc, glbs_count);

      if ((glb == nullptr) || (glb->desc_visited == true))
        return false;
      else
        glb->desc_visited = true;

      if ((session.GlobalValueRawType(glb_name) != glb->raw_type) ||
          (session.GlobalValueRawType(glb_i) != glb->raw_type))
        {
          return false;
        }
      else if ((session.GlobalValueFieldsCount(glb_name) != glb->fields_count) ||
               (session.GlobalValueFieldsCount(glb_i) != glb->fields_count))
        {
          return false;
        }
      else if (glb->raw_type == T_TABLE_MASK)
        {
          if ( ! test_fields_are_ok(session,
                                     glb_name,
                                     glb_i,
                                     glb->table_fields,
                                     glb->fields_count))
            {
              return false;
            }
        }
      else if (glb->fields_count != 0)
        return false;
    }

  for (uint_t glb_i = 0; glb_i < glbs_count; ++glb_i)
    {
      if (glb_desc[glb_i].desc_visited == false)
        return false;
    }

  try
  {
      session.GlobalValueName(glbs_count);
      return false;
  }
  catch(InterException&)
  {
  }

  try
  {
      session.GlobalValueRawType(glbs_count);
      return false;
  }
  catch(InterException&)
  {
  }

  try
  {
      session.GlobalValueRawType("some_weird_name");
      return false;
  }
  catch(InterException&)
  {
  }

  return true;
}

int
main()
{
  bool success = true;
  {
    DBSInit(DBSSettings());
  }

  DBSCreateDatabase(admin);
  DBSCreateDatabase(test_db1);
  InitInterpreter();

  {
    ISession& adminSession = GetInstance(nullptr);
    ISession& userSession  = GetInstance(test_db1);

    success = true;
    success = success && load_unit(adminSession,
                                    commonCode,
                                    sizeof commonCode);
    success = success && load_unit(userSession,
                                    userCode,
                                    sizeof userCode);

    std::cout << "Testing admin globals ... ";
    success = success && test_globals(adminSession,
                                             admin_glbs,
                                             ADMIN_GLBS_COUNT);
    std::cout << (success ? "OK" : "FAIL") << std::endl;

    std::cout << "Testing user globals ... ";
    success = success && test_globals(userSession,
                                             user_glbs,
                                             USERS_GLBS_COUNT);
    std::cout << (success ? "OK" : "FAIL") << std::endl;

    ReleaseInstance(adminSession);
    ReleaseInstance(userSession);
  }

  CleanInterpreter();
  DBSRemoveDatabase(admin);
  DBSRemoveDatabase(test_db1);
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
