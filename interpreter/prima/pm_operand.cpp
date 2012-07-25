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

#include "pm_operand.h"

#include "pm_typemanager.h"

using namespace std;
using namespace prima;


////////////////////////////////////////I_Operand//////////////////////////////

bool
I_Operand::IsNull () const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSBool& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSChar& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSDate& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSDateTime& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSHiresTime& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSInt8& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSInt16& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSInt32& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSInt64& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSReal& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSRichReal& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSUInt8& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSUInt16& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSUInt32& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSText& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSArray& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::GetValue (DBSUInt64& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSBool& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSChar& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSDate& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSDateTime& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSHiresTime& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSInt8& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSInt16& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSInt32& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSReal& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSRichReal& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSUInt8& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSUInt16& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSUInt32& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSUInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSText& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SetValue (const DBSArray& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfAdd (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfAdd (const DBSRichReal& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfAdd (const DBSChar& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfAdd (const DBSText& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}
void
I_Operand::SelfSub (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfSub (const DBSRichReal& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfMul (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfMul (const DBSRichReal& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfDiv (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfDiv (const DBSRichReal& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfMod (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfAnd (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfAnd (const DBSBool& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfXor (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfXor (const DBSBool& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfOr (const DBSInt64& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
I_Operand::SelfOr (const DBSBool& value)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

FIELD_INDEX
I_Operand::GetField ()
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

I_DBSTable&
I_Operand::GetTable ()
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

StackValue
I_Operand::GetValueAt (const D_UINT64 index)
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}




///////////////////////////////////////////////////////////////////////////////

template <typename T>
static void assign_null (T& output)
{
  output = T();
}

NullOperand::~NullOperand ()
{
}

bool
NullOperand::IsNull () const
{
  return true;
}

void
NullOperand::GetValue (DBSBool& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSChar& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSDate& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSDateTime& outValue) const
{
  throw InterException (NULL, _EXTRA (InterException::INVALID_OP_REQ));
}

void
NullOperand::GetValue (DBSHiresTime& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSInt8& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSInt16& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSInt32& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSInt64& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSReal& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSRichReal& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSUInt8& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSUInt16& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSUInt32& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSUInt64& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSText& outValue) const
{
  assign_null (outValue);
}

void
NullOperand::GetValue (DBSArray& outValue) const
{
  assign_null (outValue);
}

/////////////////////////////////BoolOperand///////////////////////////////////

BoolOperand::~BoolOperand ()
{
}

bool
BoolOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
BoolOperand::GetValue (DBSBool& outValue) const
{
  outValue = m_Value;
}

void
BoolOperand::SetValue (const DBSBool& value)
{
  m_Value = value;
}

void
BoolOperand::SelfAnd (const DBSBool& value)
{
  m_Value = internal_and (m_Value, value);
}

void
BoolOperand::SelfXor (const DBSBool& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
BoolOperand::SelfOr (const DBSBool& value)
{
  m_Value = internal_xor (m_Value, value);
}

////////////////////////////////CharOperand////////////////////////////////////

CharOperand::~CharOperand ()
{
}

bool
CharOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
CharOperand::GetValue (DBSChar& outValue) const
{
  outValue = m_Value;
}

void
CharOperand::GetValue (DBSText& outValue) const
{
  DBSText text;

  text.Append (m_Value);

  outValue = text;
}

void
CharOperand::SetValue (const DBSChar& value)
{
  m_Value = value;
}

/////////////////////////////////DateOperand///////////////////////////////////

DateOperand::~DateOperand ()
{
}

bool
DateOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
DateOperand::GetValue (DBSDate& outValue) const
{
  outValue = m_Value;
}

void
DateOperand::GetValue (DBSDateTime& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSDateTime temp;
      outValue = temp;
    }
  else
    {
      const DBSDateTime temp (m_Value.m_Year,
                              m_Value.m_Month,
                              m_Value.m_Day,
                              0,
                              0,
                              0);
      outValue = temp;
    }
}

void
DateOperand::GetValue (DBSHiresTime& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSHiresTime temp;
      outValue = temp;
    }
  else
    {
      const DBSHiresTime temp (m_Value.m_Year,
                              m_Value.m_Month,
                              m_Value.m_Day,
                              0,
                              0,
                              0,
                              0);
      outValue = temp;
    }
}

void
DateOperand::SetValue (const DBSDate& value)
{
  m_Value = value;
}

////////////////////////////////DateTimeOperand////////////////////////////////

DateTimeOperand::~DateTimeOperand ()
{
}

bool
DateTimeOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
DateTimeOperand::GetValue (DBSDate& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSDate temp;
      outValue = temp;
    }
  else
    {
      const DBSDate temp (m_Value.m_Year,
                          m_Value.m_Month,
                          m_Value.m_Day);
      outValue = temp;
    }
}

void
DateTimeOperand::GetValue (DBSDateTime& outValue) const
{
  outValue = m_Value;
}

void
DateTimeOperand::GetValue (DBSHiresTime& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSHiresTime temp;
      outValue = temp;
    }
  else
    {
      const DBSHiresTime temp (m_Value.m_Year,
                              m_Value.m_Month,
                              m_Value.m_Day,
                              m_Value.m_Hour,
                              m_Value.m_Minutes,
                              m_Value.m_Seconds,
                              0);
      outValue = temp;
    }
}

void
DateTimeOperand::SetValue (const DBSDateTime& value)
{
  m_Value = value;
}

//////////////////////////////HiresTimeOperand/////////////////////////////////

bool
HiresTimeOperand::IsNull () const
{
  return m_Value.IsNull ();
}

HiresTimeOperand::~HiresTimeOperand ()
{
}

void
HiresTimeOperand::GetValue (DBSDate& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSDate temp;
      outValue = temp;
    }
  else
    {
      const DBSDate temp (m_Value.m_Year,
                          m_Value.m_Month,
                          m_Value.m_Day);
      outValue = temp;
    }
}

void
HiresTimeOperand::GetValue (DBSDateTime& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSDateTime temp;
      outValue = temp;
    }
  else
    {
      const DBSDateTime temp (m_Value.m_Year,
                              m_Value.m_Month,
                              m_Value.m_Day,
                              m_Value.m_Hour,
                              m_Value.m_Minutes,
                              m_Value.m_Seconds);
      outValue = temp;
    }
}

void
HiresTimeOperand::GetValue (DBSHiresTime& outValue) const
{
  outValue = m_Value;
}

void
HiresTimeOperand::SetValue (const DBSHiresTime& value)
{
  m_Value = value;
}

/////////////////////////////////UInt8Operand//////////////////////////////////

bool
UInt8Operand::IsNull () const
{
  return m_Value.IsNull ();
}

UInt8Operand::~UInt8Operand ()
{
}

void
UInt8Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt8Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt8Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
UInt8Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
UInt8Operand::SetValue (const DBSUInt8& value)
{
  m_Value = value;
}

void
UInt8Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
UInt8Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
UInt8Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
UInt8Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt8Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt8Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
UInt8Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
UInt8Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}


////////////////////////////////UInt16Operand//////////////////////////////////

UInt16Operand::~UInt16Operand ()
{
}

bool
UInt16Operand::IsNull () const
{
  return m_Value.IsNull ();
}


void
UInt16Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt16Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt16Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
UInt16Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
UInt16Operand::SetValue (const DBSUInt16& value)
{
  m_Value = value;
}

void
UInt16Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
UInt16Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
UInt16Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
UInt16Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt16Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt16Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
UInt16Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
UInt16Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

////////////////////////////////UInt32Operand//////////////////////////////////

UInt32Operand::~UInt32Operand ()
{
}

bool
UInt32Operand::IsNull () const
{
  return m_Value.IsNull ();
}


void
UInt32Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt32Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt32Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
UInt32Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
UInt32Operand::SetValue (const DBSUInt32& value)
{
  m_Value = value;
}

void
UInt32Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
UInt32Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
UInt32Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
UInt32Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt32Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt32Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
UInt32Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
UInt32Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}


////////////////////////////////UInt64Operand//////////////////////////////////

UInt64Operand::~UInt64Operand ()
{
}

bool
UInt64Operand::IsNull () const
{
  return m_Value.IsNull ();
}


void
UInt64Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt64Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
UInt64Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
UInt64Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
UInt64Operand::SetValue (const DBSUInt64& value)
{
  m_Value = value;
}

void
UInt64Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
UInt64Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
UInt64Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
UInt64Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt64Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
UInt64Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
UInt64Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
UInt64Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

//////////////////////////////////Int8Operand//////////////////////////////////

Int8Operand::~Int8Operand ()
{
}

bool
Int8Operand::IsNull () const
{
  return m_Value.IsNull ();
}

void
Int8Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)  = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value) = _SC (D_INT8, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int8Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int8Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
Int8Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
Int8Operand::SetValue (const DBSInt8& value)
{
  m_Value = value;
}

void
Int8Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
Int8Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
Int8Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
Int8Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int8Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int8Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
Int8Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
Int8Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}


/////////////////////////////////Int16Operand//////////////////////////////////

Int16Operand::~Int16Operand ()
{
}

bool
Int16Operand::IsNull () const
{
  return m_Value.IsNull ();
}

void
Int16Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int16Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int16Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
Int16Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
Int16Operand::SetValue (const DBSInt16& value)
{
  m_Value = value;
}

void
Int16Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
Int16Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
Int16Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
Int16Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int16Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int16Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
Int16Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
Int16Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}


/////////////////////////////////Int32Operand//////////////////////////////////

Int32Operand::~Int32Operand ()
{
}

bool
Int32Operand::IsNull () const
{
  return m_Value.IsNull ();
}

void
Int32Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int32Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int32Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
Int32Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
Int32Operand::SetValue (const DBSInt32& value)
{
  m_Value = value;
}

void
Int32Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
Int32Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
Int32Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
Int32Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int32Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int32Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
Int32Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
Int32Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

/////////////////////////////////Int64Operand//////////////////////////////////

Int64Operand::~Int64Operand ()
{
}

bool
Int64Operand::IsNull () const
{
  return m_Value.IsNull ();
}

void
Int64Operand::GetValue (DBSInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT8&, outValue.m_Value)  = _SC (D_INT8, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT16&, outValue.m_Value) = _SC (D_INT16, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT32&, outValue.m_Value) = _SC (D_INT32, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)   = m_Value.m_IsNull;
  _CC (D_INT64&, outValue.m_Value) = _SC (D_INT64, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSRichReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSRichReal temp;
      outValue = temp;
    }
  else
    {
      const DBSRichReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int64Operand::GetValue (DBSReal& outValue) const
{
  if (m_Value.IsNull ())
    {
      const DBSReal temp;
      outValue = temp;
    }
  else
    {
      const DBSReal temp (m_Value.m_Value);
      outValue = temp;
    }
}

void
Int64Operand::GetValue (DBSUInt8& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT8&, outValue.m_Value)  = _SC (D_UINT8, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSUInt16& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT16&, outValue.m_Value) = _SC (D_UINT16, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSUInt32& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT32&, outValue.m_Value) = _SC (D_UINT32, m_Value.m_Value);
}

void
Int64Operand::GetValue (DBSUInt64& outValue) const
{
  _CC (bool&, outValue.m_IsNull)    = m_Value.m_IsNull;
  _CC (D_UINT64&, outValue.m_Value) = _SC (D_UINT64, m_Value.m_Value);
}

void
Int64Operand::SetValue (const DBSInt64& value)
{
  m_Value = value;
}

void
Int64Operand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
Int64Operand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
Int64Operand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
Int64Operand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int64Operand::SelfMod (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
Int64Operand::SelfAnd (const DBSInt64& value)
{
  m_Value = internal_and (m_Value, value);
}

void
Int64Operand::SelfXor (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

void
Int64Operand::SelfOr (const DBSInt64& value)
{
  m_Value = internal_xor (m_Value, value);
}

/////////////////////////////////RealOperand///////////////////////////////////

RealOperand::~RealOperand ()
{
}

bool
RealOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
RealOperand::GetValue (DBSReal& outValue) const
{
  outValue = m_Value;
}

void
RealOperand::GetValue (DBSRichReal& outValue) const
{
  _CC (bool&, outValue.m_IsNull)      = m_Value.m_IsNull;
  _CC (RICHREAL_T&, outValue.m_Value) = _SC (RICHREAL_T, m_Value.m_Value);
}

void
RealOperand::SetValue (const DBSReal& value)
{
  m_Value = value;
}

void
RealOperand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
RealOperand::SelfAdd (const DBSRichReal& value)
{
  m_Value = internal_add (m_Value, value);
}

void
RealOperand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
RealOperand::SelfSub (const DBSRichReal& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
RealOperand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
RealOperand::SelfMul (const DBSRichReal& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
RealOperand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
RealOperand::SelfDiv (const DBSRichReal& value)
{
  m_Value = internal_div (m_Value, value);
}

///////////////////////////////RichRealOperand/////////////////////////////////

RichRealOperand::~RichRealOperand ()
{
}

bool
RichRealOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
RichRealOperand::GetValue (DBSReal& outValue) const
{
  _CC (bool&, outValue.m_IsNull) = m_Value.m_IsNull;
  _CC (REAL_T&, outValue.m_Value) = _SC (REAL_T, m_Value.m_Value);
}

void
RichRealOperand::GetValue (DBSRichReal& outValue) const
{
  outValue = m_Value;
}

void
RichRealOperand::SetValue (const DBSRichReal& value)
{
  m_Value = value;
}

void
RichRealOperand::SelfAdd (const DBSInt64& value)
{
  m_Value = internal_add (m_Value, value);
}

void
RichRealOperand::SelfAdd (const DBSRichReal& value)
{
  m_Value = internal_add (m_Value, value);
}

void
RichRealOperand::SelfSub (const DBSInt64& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
RichRealOperand::SelfSub (const DBSRichReal& value)
{
  m_Value = internal_sub (m_Value, value);
}

void
RichRealOperand::SelfMul (const DBSInt64& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
RichRealOperand::SelfMul (const DBSRichReal& value)
{
  m_Value = internal_mul (m_Value, value);
}

void
RichRealOperand::SelfDiv (const DBSInt64& value)
{
  m_Value = internal_div (m_Value, value);
}

void
RichRealOperand::SelfDiv (const DBSRichReal& value)
{
  m_Value = internal_div (m_Value, value);
}

//////////////////////////////TextOperand//////////////////////////////////////

TextOperand::~TextOperand ()
{
}

bool
TextOperand::IsNull () const
{
  return m_Value.IsNull ();
}

void
TextOperand::GetValue (DBSText& outValue) const
{
  outValue = m_Value;
}

void
TextOperand::SetValue (const DBSText& value)
{
  m_Value = value;
}

void
TextOperand::SelfAdd (const DBSChar& value)
{
  m_Value.Append (value);
}

void
TextOperand::SelfAdd (const DBSText& value)
{
  m_Value.Append (value);
}

StackValue
TextOperand::GetValueAt (const D_UINT64 index)
{
  return StackValue (CharTextElOperand (m_Value, index));
}

///////////////////////////////CharTextOperand/////////////////////////////////

CharTextElOperand::~CharTextElOperand ()
{
}

bool
CharTextElOperand::IsNull () const
{
  //A text could not hold NULL characters!
  return false;
}

void
CharTextElOperand::GetValue (DBSChar& outValue) const
{
  outValue = m_Text.GetCharAtIndex (m_Index);
}

void
CharTextElOperand::GetValue (DBSText& outValue) const
{
  DBSChar ch = m_Text.GetCharAtIndex (m_Index);
  DBSText text;

  text.Append (ch);
  outValue = text;
}

void
CharTextElOperand::SetValue (const DBSChar& value)
{
  m_Text.SetCharAtIndex (value, m_Index);
}

///////////////////////GlobalOperand///////////////////////////////////////////

GlobalOperand::GlobalOperand (GlobalValue& value) :
    I_Operand (),
    m_Value (value)
{
}

GlobalOperand::~GlobalOperand ()
{
}


void
GlobalOperand::GetValue (DBSBool& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSChar& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSDate& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSDateTime& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSHiresTime& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSInt8& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSInt16& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSInt32& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSInt64& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSReal& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSRichReal& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSUInt8& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSUInt16& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSUInt32& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSUInt64& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSText& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::GetValue (DBSArray& outValue) const
{
  m_Value.GetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSBool& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSChar& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSDate& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSDateTime& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSHiresTime& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSInt8& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSInt16& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSInt32& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSInt64& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSReal& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSRichReal& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSUInt8& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSUInt16& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSUInt32& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSUInt64& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSText& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SetValue (const DBSArray& outValue)
{
  m_Value.SetValue (outValue);
}

void
GlobalOperand::SelfAdd (const DBSInt64& value)
{
  m_Value.SelfAdd (value);
}

void
GlobalOperand::SelfAdd (const DBSRichReal& value)
{
  m_Value.SelfAdd (value);
}

void
GlobalOperand::SelfAdd (const DBSChar& value)
{
  m_Value.SelfAdd (value);
}

void
GlobalOperand::SelfAdd (const DBSText& value)
{
  m_Value.SelfAdd (value);
}
void
GlobalOperand::SelfSub (const DBSInt64& value)
{
  m_Value.SelfSub (value);
}

void
GlobalOperand::SelfSub (const DBSRichReal& value)
{
  m_Value.SelfSub (value);
}

void
GlobalOperand::SelfMul (const DBSInt64& value)
{
  m_Value.SelfMul (value);
}

void
GlobalOperand::SelfMul (const DBSRichReal& value)
{
  m_Value.SelfMul (value);
}

void
GlobalOperand::SelfDiv (const DBSInt64& value)
{
  m_Value.SelfDiv (value);
}

void
GlobalOperand::SelfDiv (const DBSRichReal& value)
{
  m_Value.SelfDiv (value);
}

void
GlobalOperand::SelfMod (const DBSInt64& value)
{
  m_Value.SelfMod (value);
}

void
GlobalOperand::SelfAnd (const DBSInt64& value)
{
  m_Value.SelfAnd (value);
}

void
GlobalOperand::SelfAnd (const DBSBool& value)
{
  m_Value.SelfAnd (value);
}

void
GlobalOperand::SelfXor (const DBSInt64& value)
{
  m_Value.SelfXor (value);
}

void
GlobalOperand::SelfXor (const DBSBool& value)
{
  m_Value.SelfXor (value);
}

void
GlobalOperand::SelfOr (const DBSInt64& value)
{
  m_Value.SelfOr (value);
}

void
GlobalOperand::SelfOr (const DBSBool& value)
{
  m_Value.SelfOr (value);
}

FIELD_INDEX
GlobalOperand::GetField ()
{
  return m_Value.GetField ();
}

I_DBSTable&
GlobalOperand::GetTable ()
{
  return m_Value.GetTable ();
}

////////////////////////////////////LocalOperand///////////////////////////////

LocalOperand::LocalOperand (StackValue& localValue)
  : I_Operand (),
    m_LocalValue (localValue)
{
}

LocalOperand::~LocalOperand ()
{
}

void
LocalOperand::GetValue (DBSBool& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSChar& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSDate& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSDateTime& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSHiresTime& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSInt8& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSInt16& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSInt32& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSInt64& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSReal& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSRichReal& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSUInt8& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSUInt16& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSUInt32& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSUInt64& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSText& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::GetValue (DBSArray& outValue) const
{
  m_LocalValue.GetOperand ().GetValue (outValue);
}

void
LocalOperand::SetValue (const DBSBool& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSChar& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSDate& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSDateTime& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSHiresTime& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSInt8& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSInt16& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSInt32& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSInt64& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSReal& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSRichReal& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSUInt8& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSUInt16& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSUInt32& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSUInt64& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSText& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SetValue (const DBSArray& outValue)
{
  m_LocalValue.GetOperand ().SetValue (outValue);
}

void
LocalOperand::SelfAdd (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfAdd (value);
}

void
LocalOperand::SelfAdd (const DBSRichReal& value)
{
  m_LocalValue.GetOperand ().SelfAdd (value);
}

void
LocalOperand::SelfAdd (const DBSChar& value)
{
  m_LocalValue.GetOperand ().SelfAdd (value);
}

void
LocalOperand::SelfAdd (const DBSText& value)
{
  m_LocalValue.GetOperand ().SelfAdd (value);
}
void
LocalOperand::SelfSub (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfSub (value);
}

void
LocalOperand::SelfSub (const DBSRichReal& value)
{
  m_LocalValue.GetOperand ().SelfSub (value);
}

void
LocalOperand::SelfMul (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfMul (value);
}

void
LocalOperand::SelfMul (const DBSRichReal& value)
{
  m_LocalValue.GetOperand ().SelfMul (value);
}

void
LocalOperand::SelfDiv (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfDiv (value);
}

void
LocalOperand::SelfDiv (const DBSRichReal& value)
{
  m_LocalValue.GetOperand ().SelfDiv (value);
}

void
LocalOperand::SelfMod (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfMod (value);
}

void
LocalOperand::SelfAnd (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfAnd (value);
}

void
LocalOperand::SelfAnd (const DBSBool& value)
{
  m_LocalValue.GetOperand ().SelfAnd (value);
}

void
LocalOperand::SelfXor (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfXor (value);
}

void
LocalOperand::SelfXor (const DBSBool& value)
{
  m_LocalValue.GetOperand ().SelfXor (value);
}

void
LocalOperand::SelfOr (const DBSInt64& value)
{
  m_LocalValue.GetOperand ().SelfOr (value);
}

void
LocalOperand::SelfOr (const DBSBool& value)
{
  m_LocalValue.GetOperand ().SelfOr (value);
}

FIELD_INDEX
LocalOperand::GetField ()
{
  return m_LocalValue.GetOperand ().GetField ();
}

I_DBSTable&
LocalOperand::GetTable ()
{
  return m_LocalValue.GetOperand ().GetTable ();
}

////////////////////////////////////SessionStack///////////////////////////////

SessionStack::SessionStack ()
  : m_Stack ()
{
}

SessionStack::~SessionStack ()
{
  if (m_Stack.size () > 0)
    Pop (m_Stack.size ());
}

void
SessionStack::Push (StackValue& value)
{
  m_Stack.push_back (value);
}

void
SessionStack::Push ()
{
  NullOperand stackOp;
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSBool& value)
{
  BoolOperand stackOp (value);
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSChar& value)
{
  CharOperand stackOp (value);
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSDate& value)
{
  DateOperand stackOp (value);
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSDateTime& value)
{
  DateTimeOperand stackOp (value);
  StackValue      stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSHiresTime& value)
{
  HiresTimeOperand stackOp (value);
  StackValue       stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSInt8& value)
{
  Int8Operand stackOp (value);
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSInt16& value)
{
  Int16Operand stackOp (value);
  StackValue   stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSInt32& value)
{
  Int32Operand stackOp (value);
  StackValue   stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSInt64& value)
{
  Int64Operand stackOp (value);
  StackValue   stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSUInt8& value)
{
  UInt8Operand stackOp (value);
  StackValue   stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSUInt16& value)
{
  UInt16Operand stackOp (value);
  StackValue    stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSUInt32& value)
{
  UInt32Operand stackOp (value);
  StackValue    stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSUInt64& value)
{
  UInt64Operand stackOp (value);
  StackValue    stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSReal& value)
{
  RealOperand stackOp (value);
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSRichReal& value)
{
  RichRealOperand stackOp (value);
  StackValue      stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSText& value)
{
  TextOperand stackOp (value);
  StackValue  stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Push (DBSArray& value)
{
  ArrayOperand stackOp (value);
  StackValue   stackValue (stackOp);

  Push (stackValue);
}

void
SessionStack::Pop (const D_UINT count)
{
  if (count > m_Stack.size ())
    throw DBSException (NULL, _EXTRA (InterException::STACK_CORRUPTED));

  D_UINT topIndex = m_Stack.size () - 1;
  for (D_UINT index = 0; index < count; ++index, --topIndex)
    {
      m_Stack[topIndex].Clear ();
      m_Stack.pop_back ();
    }
}

size_t
SessionStack::Size () const
{
  return m_Stack.size ();
}

StackValue&
SessionStack::operator[] (const D_UINT index)
{
  if (index > m_Stack.size ())
    throw DBSException (NULL, _EXTRA (InterException::STACK_CORRUPTED));

  return m_Stack[index];
}

