/******************************************************************************
 PASTRA - A light database one file system and more.
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
 *****************************************************************************/

#include <assert.h>
#include <memory.h>
#include <vector>

#include "dbs/dbs_types.h"
#include "compiler/whisperc.h"
#include "utils/le_converter.h"

#include "pm_typemanager.h"
#include "pm_interpreter.h"
#include "pm_general_table.h"

using namespace std;



namespace whisper {
namespace prima {



static const uint8_t  TYPE_SPEC_END_MARK = ';';




class TypeSpec
{
public:
  TypeSpec (const uint8_t* const typeDesc)
    : mType (load_le_int16 (typeDesc)),
      mSize (load_le_int16 (typeDesc + sizeof (uint16_t))),
      mData (typeDesc + 2 * sizeof (uint16_t))
  {
  }

  uint16_t RawSize () const
  {
    return mSize + 2 * sizeof (uint16_t);
  }

  uint16_t DataSize () const
  {
    return mSize;
  }

  bool operator== (const TypeSpec& second) const
    {
      if ((mType == second.mType ) && (mSize == second.mSize))
        return (memcmp (mData, second.mData, mSize) == 0);

      return false;
    }

  uint16_t Type () const
  {
    return mType;
  }

  const uint8_t* Data () const
  {
    return mData;
  }

private:
  const uint16_t       mType;
  const uint16_t       mSize;
  const uint8_t* const mData;
};



TypeManager::TypeManager (NameSpace& space)
  : mNameSpace (space),
    mTypesDescriptions ()
{
}


uint32_t
TypeManager::AddType (const uint8_t* const typeDesc)
{
  assert (IsTypeValid (typeDesc));

  uint32_t result = FindType (typeDesc);

  if (result != INVALID_OFFSET)
    return result;

  result = mTypesDescriptions.size ();

  const TypeSpec spec (typeDesc);

  mTypesDescriptions.insert (mTypesDescriptions.end (),
                             typeDesc,
                             typeDesc + spec.RawSize ());
  return result;
}


uint32_t
TypeManager::FindType (const uint8_t* const typeDesc)
{
  assert (IsTypeValid (typeDesc));

  const TypeSpec spec (typeDesc);

  uint32_t result = 0;

  while (result < mTypesDescriptions.size ())
    {
      assert (IsTypeValid (&mTypesDescriptions[result]));

      const TypeSpec specIt (&mTypesDescriptions[result]);

      if (spec == specIt)
        return result;

      result += specIt.RawSize();

      assert (result <= mTypesDescriptions.size ());
    }

  return INVALID_OFFSET;
}


const uint8_t*
TypeManager::TypeDescription (const uint32_t offset) const
{
  assert ((offset == 0) || (offset < mTypesDescriptions.size ()));

  const uint8_t *const typeDesc = &mTypesDescriptions[offset];

  assert (IsTypeValid (typeDesc));

  return typeDesc;
}


static ITable&
create_non_persistent_table (IDBSHandler&       dbs,
                             uint8_t* const     inoutTypeDesc)
{
  assert (TypeManager::IsTypeValid (inoutTypeDesc));

  TypeSpec spec (inoutTypeDesc);

  assert (IS_TABLE (spec.Type ()));

  vector<DBSFieldDescriptor> fields;
  int                        typeOff = 0;

  while (typeOff < spec.DataSize () - 2)
    {
      DBSFieldDescriptor fd;
      uint16_t           type;

      fd.name = _RC (const char*, spec.Data () + typeOff);

      typeOff += strlen (fd.name) + 1;

      assert (typeOff < spec.DataSize () - 2);

      type     = load_le_int16 (spec.Data () + typeOff);
      typeOff += sizeof (uint16_t);

      fd.isArray = IS_ARRAY (type);
      fd.type    = _SC (DBS_FIELD_TYPE, GET_BASIC_TYPE (type));

      fields.push_back (fd);
    }

  if (fields.size () == 0)
    return GeneralTable::Instance ();

  ITable& table = dbs.CreateTempTable (fields.size (), &fields[0]);

  assert (table.FieldsCount () == fields.size ());

  typeOff = 0;
  for (uint_t fieldIndex = 0; fieldIndex < fields.size (); ++fieldIndex)
    {
      DBSFieldDescriptor field   = table.DescribeField (fieldIndex);
      uint16_t           type    = field.type;
      const uint_t       nameLen = strlen (field.name) + 1;

      if (field.isArray)
        MARK_ARRAY (type);

      memcpy (_CC (uint8_t*, spec.Data()) + typeOff, field.name, nameLen);

      typeOff += nameLen;
      store_le_int16 (type, _CC (uint8_t*, spec.Data()) + typeOff);
      typeOff += sizeof (uint16_t);
    }

  return table;
}


GlobalValue
TypeManager::CreateGlobalValue (uint8_t*    inoutTypeDesc,
                                ITable*     persitentTable)
{
  assert (TypeManager::IsTypeValid (inoutTypeDesc));

  const TypeSpec spec (inoutTypeDesc);

  if ((spec.Type () > T_UNKNOWN) && (spec.Type () < T_UNDETERMINED))
    {
      assert (persitentTable == NULL);

      switch (spec.Type ())
      {
      case T_BOOL:
        return GlobalValue (BoolOperand (DBool ()));

      case T_CHAR:
        return GlobalValue (CharOperand (DChar ()));

      case T_DATE:
        return GlobalValue (DateOperand (DDate ()));

      case T_DATETIME:
        return GlobalValue (DateTimeOperand (DDateTime ()));

      case T_HIRESTIME:
        return GlobalValue (HiresTimeOperand (DHiresTime ()));

      case T_INT8:
        return GlobalValue (Int8Operand (DInt8 ()));

      case T_INT16:
        return GlobalValue (Int16Operand (DInt16 ()));

      case T_INT32:
        return GlobalValue (Int32Operand (DInt32 ()));

      case T_INT64:
        return GlobalValue (Int64Operand (DInt64 ()));

      case T_UINT8:
        return GlobalValue (UInt8Operand (DUInt8 ()));

      case T_UINT16:
        return GlobalValue (UInt16Operand (DUInt16 ()));

      case T_UINT32:
        return GlobalValue (UInt32Operand (DUInt32 ()));

      case T_UINT64:
        return GlobalValue (UInt64Operand (DUInt64 ()));

      case T_REAL:
        return GlobalValue (RealOperand (DReal ()));

      case T_RICHREAL:
        return GlobalValue (RichRealOperand (DRichReal ()));

      case T_TEXT:
        return GlobalValue (TextOperand (DText ()));

      default:
        assert (false);
      }
    }
  else if (IS_ARRAY (spec.Type ()))
    {
      assert (persitentTable == NULL);

      switch (GET_BASIC_TYPE (spec.Type ()))
      {
      case T_BOOL:
        return GlobalValue (ArrayOperand ( DArray ((DBool*) NULL)));

      case T_CHAR:
        return GlobalValue (ArrayOperand ( DArray ((DChar*) NULL)));

      case T_DATE:
        return GlobalValue (ArrayOperand ( DArray ((DDate*) NULL)));

      case T_DATETIME:
        return GlobalValue (ArrayOperand ( DArray ((DDateTime*) NULL)));

      case T_HIRESTIME:
        return GlobalValue (ArrayOperand ( DArray ((DHiresTime*) NULL)));

      case T_INT8:
        return GlobalValue (ArrayOperand ( DArray ((DInt8*) NULL)));

      case T_INT16:
        return GlobalValue (ArrayOperand ( DArray ((DInt16*) NULL)));

      case T_INT32:
        return GlobalValue (ArrayOperand ( DArray ((DInt32*) NULL)));

      case T_INT64:
        return GlobalValue (ArrayOperand ( DArray ((DInt64*) NULL)));

      case T_UINT8:
        return GlobalValue (ArrayOperand ( DArray ((DUInt8*) NULL)));

      case T_UINT16:
        return GlobalValue (ArrayOperand ( DArray ((DUInt16*) NULL)));

      case T_UINT32:
        return GlobalValue (ArrayOperand ( DArray ((DUInt32*) NULL)));

      case T_UINT64:
        return GlobalValue (ArrayOperand ( DArray ((DUInt64*) NULL)));

      case T_REAL:
        return GlobalValue (ArrayOperand ( DArray ((DReal*) NULL)));

      case T_RICHREAL:
        return GlobalValue (ArrayOperand ( DArray ((DRichReal*) NULL)));

      case T_TEXT:
        throw InterException (NULL,
                              _EXTRA (InterException::TEXT_ARRAY_NOT_SUPP));

      case T_UNDETERMINED:
        //Just a default
        assert (false);

        return GlobalValue (ArrayOperand ( DArray ()));

      default:
        assert (false);
      }
    }
  else if (IS_FIELD (spec.Type ()))
    {
      assert (persitentTable == NULL);

      return GlobalValue (FieldOperand (GET_FIELD_TYPE (spec.Type ())));
    }
  else if (IS_TABLE (spec.Type ()))
    {
      if (persitentTable == NULL)
        {
          ITable& table = create_non_persistent_table (
                                             mNameSpace.GetDBSHandler (),
                                             inoutTypeDesc
                                                      );
          return GlobalValue (TableOperand (mNameSpace.GetDBSHandler(),
                                            table));
        }
      else
        {
          return GlobalValue (TableOperand (mNameSpace.GetDBSHandler(),
                                            *persitentTable));
        }
    }

  assert (false);

  return GlobalValue (NullOperand ());
}


StackValue
TypeManager::CreateLocalValue (uint8_t* inoutTypeDesc)
{
  assert (TypeManager::IsTypeValid (inoutTypeDesc));

  const TypeSpec spec (inoutTypeDesc);

  if ((spec.Type () > T_UNKNOWN) && (spec.Type () < T_UNDETERMINED))
    {
      switch (spec.Type ())
      {
      case T_BOOL:
        return StackValue (BoolOperand (DBool ()));

      case T_CHAR:
        return StackValue (CharOperand (DChar ()));

      case T_DATE:
        return StackValue (DateOperand (DDate ()));

      case T_DATETIME:
        return StackValue (DateTimeOperand (DDateTime ()));

      case T_HIRESTIME:
        return StackValue (HiresTimeOperand (DHiresTime ()));

      case T_INT8:
        return StackValue (Int8Operand (DInt8 ()));

      case T_INT16:
        return StackValue (Int16Operand (DInt16 ()));

      case T_INT32:
        return StackValue (Int32Operand (DInt32 ()));

      case T_INT64:
        return StackValue (Int64Operand (DInt64 ()));

      case T_REAL:
        return StackValue (RealOperand (DReal ()));

      case T_RICHREAL:
        return StackValue (RichRealOperand (DRichReal ()));

      case T_TEXT:
        return StackValue (TextOperand (DText ()));

      case T_UINT8:
        return StackValue (UInt8Operand (DUInt8 ()));

      case T_UINT16:
        return StackValue (UInt16Operand (DUInt16 ()));

      case T_UINT32:
        return StackValue (UInt32Operand (DUInt32 ()));

      case T_UINT64:
        return StackValue (UInt64Operand (DUInt64 ()));

      default:
        assert (false);
      }
    }
  else if (IS_ARRAY (spec.Type ()))
    {
      switch (GET_BASIC_TYPE (spec.Type ()))
      {
      case T_BOOL:
        return StackValue (ArrayOperand ( DArray ((DBool*) NULL)));

      case T_CHAR:
        return StackValue (ArrayOperand ( DArray ((DChar*) NULL)));

      case T_DATE:
        return StackValue (ArrayOperand ( DArray ((DDate*) NULL)));

      case T_DATETIME:
        return StackValue (ArrayOperand ( DArray ((DDateTime*) NULL)));

      case T_HIRESTIME:
        return StackValue (ArrayOperand ( DArray ((DHiresTime*) NULL)));

      case T_INT8:
        return StackValue (ArrayOperand ( DArray ((DInt8*) NULL)));

      case T_INT16:
        return StackValue (ArrayOperand ( DArray ((DInt16*) NULL)));

      case T_INT32:
        return StackValue (ArrayOperand ( DArray ((DInt32*) NULL)));

      case T_INT64:
        return StackValue (ArrayOperand ( DArray ((DInt64*) NULL)));

      case T_UINT8:
        return StackValue (ArrayOperand ( DArray ((DUInt8*) NULL)));

      case T_UINT16:
        return StackValue (ArrayOperand ( DArray ((DUInt16*) NULL)));

      case T_UINT32:
        return StackValue (ArrayOperand ( DArray ((DUInt32*) NULL)));

      case T_UINT64:
        return StackValue (ArrayOperand ( DArray ((DUInt64*) NULL)));

      case T_REAL:
        return StackValue (ArrayOperand ( DArray ((DReal*) NULL)));

      case T_RICHREAL:
        return StackValue (ArrayOperand ( DArray ((DRichReal*) NULL)));

      case T_TEXT:
        throw InterException (NULL,
                              _EXTRA (InterException::TEXT_ARRAY_NOT_SUPP));

      case T_UNDETERMINED:
        //Just a default
        return StackValue (ArrayOperand (DArray ()));

      default:
        assert (false);
      }
    }
  else if (IS_FIELD (spec.Type ()))
    {
      return StackValue (FieldOperand (GET_FIELD_TYPE (spec.Type ())));
    }
  else if (IS_TABLE (spec.Type ()))
    {
      ITable& table = create_non_persistent_table (
                                             mNameSpace.GetDBSHandler (),
                                             inoutTypeDesc
                                                  );

      return StackValue (TableOperand (mNameSpace.GetDBSHandler(), table));
    }

  assert (false);

  return StackValue (NullOperand ());
}


bool
TypeManager::IsTypeValid (const uint8_t* const typeDesc)
{
  bool result = true;

  const TypeSpec spec (typeDesc);

  if (((spec.Type () == T_UNKNOWN) || (spec.Type () > T_UNDETERMINED))
      && (IS_ARRAY (spec.Type ()) == false)
      && (IS_TABLE (spec.Type ()) == false)
      && (IS_FIELD (spec.Type ()) == false))
    {
      result = false;
    }
  else if ((spec.Data()[spec.DataSize () - 2] != TYPE_SPEC_END_MARK)
           || (spec.Data()[spec.DataSize () - 1] != 0))
    {
      result = false;
    }
  else if (IS_FIELD (spec.Type ()))
    {
      const uint16_t fieldType = GET_FIELD_TYPE (spec.Type ());

      if (spec.DataSize () != 2)
        result = false;

      else if (IS_ARRAY (fieldType))
        {
          if ((GET_BASIC_TYPE (fieldType) == T_UNKNOWN)
              || (GET_BASIC_TYPE (fieldType) > T_UNDETERMINED))
            {
              result = false;
            }
        }
      else
        {
          if (GET_BASIC_TYPE (fieldType) == T_UNKNOWN
              || GET_BASIC_TYPE (fieldType) > T_UNDETERMINED)
            {
              result = false;
            }
        }
    }
  else if (IS_ARRAY (spec.Type ()))
    {
      if ((spec.DataSize () != 2)
          || (GET_BASIC_TYPE (spec.Type ()) == T_UNKNOWN)
          || (GET_BASIC_TYPE (spec.Type ()) > T_UNDETERMINED))
        {
          result = false;
        }
    }
  else if (IS_TABLE (spec.Type ()))
    {
      uint_t index = 0;

      while ((index < (uint_t) (spec.DataSize () - 2)) && (result != FALSE))
        {
          uint_t id_len = strlen (_RC (const char*, spec.Data ()) + index);

          /* Don't check for zero here, because of strlen */
          index += id_len + 1;

          uint16_t type = load_le_int16 (spec.Data () + index);
          /* clear an eventual array mask */
          type = GET_BASIC_TYPE (type);
          if ((type == T_UNKNOWN) || (type >= T_UNDETERMINED))
            {
              result = false;
              break;
            }
          index += sizeof (uint16_t);
        }
    }

  return result;
}


uint_t
TypeManager::GetTypeLength (const uint8_t* const typeDesc)
{
  assert (IsTypeValid (typeDesc));

  const TypeSpec ts (typeDesc);

  return ts.RawSize ();
}


vector<uint8_t>
compute_table_typeinfo (ITable& table)
{
  vector<uint8_t> data;

  const FIELD_INDEX fieldsCount = table.FieldsCount ();
  for (FIELD_INDEX fieldId = 0; fieldId < fieldsCount; ++fieldId)
    {
      DBSFieldDescriptor field = table.DescribeField (fieldId);

      const uint_t nameLen = strlen (field.name) + 1;

      data.insert (data.end (), field.name, field.name + nameLen);

      uint16_t type = field.type;

      if (field.isArray)
        MARK_ARRAY (type);

      data.insert (data.end (),
                   _RC (uint8_t*, &type),
                   _RC (uint8_t*, &type) + 2);
    }

  data.push_back (TYPE_SPEC_END_MARK);
  data.push_back (0);

  vector<uint8_t> result;

  uint16_t temp = 0;
  MARK_TABLE (temp);

  result.insert (result.end (),
                 _RC (uint8_t*, &temp),
                 _RC (uint8_t*, &temp) + sizeof (temp));

  temp = data.size ();
  result.insert (result.end (),
                 _RC (uint8_t*, &temp),
                 _RC (uint8_t*, &temp) + sizeof (temp));

  result.insert (result.end (), data.begin (), data.end ());

  return result;
}


} //naemspace prima
} //naemspace whisper

