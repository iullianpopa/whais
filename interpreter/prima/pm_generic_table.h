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

#ifndef PM_GENERAL_TABLE_H_
#define PM_GENERAL_TABLE_H_


#include "dbs/dbs_table.h"


namespace whais {
namespace prima {


class GenericTable : public ITable
{
public:
  virtual bool IsTemporal() const override;
  virtual ITable& Spawn() const override;

  virtual FIELD_INDEX FieldsCount() override;
  virtual FIELD_INDEX RetrieveField(const char* field) override;
  virtual DBSFieldDescriptor DescribeField(const FIELD_INDEX field) override;

  virtual ROW_INDEX AllocatedRows() override;
  virtual ROW_INDEX AddRow(const bool skipThreadSafety) override;
  virtual ROW_INDEX GetReusableRow(const bool forceAdd) override;
  virtual ROW_INDEX ReusableRowsCount() override;
  virtual void MarkRowForReuse(const ROW_INDEX row) override;

  virtual void CreateIndex(const FIELD_INDEX field,
                           CREATE_INDEX_CALLBACK_FUNC* const cbFunc,
                           CreateIndexCallbackContext* const cbCotext);
  virtual void RemoveIndex(const FIELD_INDEX field) override;
  virtual bool IsIndexed(const FIELD_INDEX field) const override;

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DBool&      value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DChar&      value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DDate&      value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DDateTime&  value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DHiresTime& value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DInt8&      value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DInt16&     value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DInt32&     value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DInt64&     value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DReal&      value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DRichReal&  value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DUInt8&     value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DUInt16&    value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                    const FIELD_INDEX field,
                    const DUInt32&    value,
                    const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DUInt64&    value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DText&      value,
                   const bool        skipThreadSafety = false);

  virtual void Set(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   const DArray&     value,
                   const bool        skipThreadSafety = false);


  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DBool&            outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DChar&            outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DDate&            outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DDateTime&        outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DHiresTime&       outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DInt8&            outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DInt16&           outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DInt32&           outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DInt64&           outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DReal&            outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DRichReal&        outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DUInt8&           outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DUInt16&          outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DUInt32&          outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DUInt64&          outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DText&            outValue,
                   const bool        skipThreadSafety = false);

  virtual void Get(const ROW_INDEX   row,
                   const FIELD_INDEX field,
                   DArray&           outValue,
                   const bool        skipThreadSafety = false);

  virtual void ExchangeRows(const ROW_INDEX row1,
                            const ROW_INDEX row2,
                            const bool skipThreadSafety = false);
  virtual void Sort(const FIELD_INDEX field,
                    const ROW_INDEX from,
                    const ROW_INDEX to,
                    const bool reverse);

  virtual DArray MatchRows(const DBool&        min,
                           const DBool&        max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DChar&        min,
                           const DChar&        max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DDate&        min,
                           const DDate&        max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DDateTime&    min,
                           const DDateTime&    max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DHiresTime&   min,
                           const DHiresTime&   max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DUInt8&       min,
                           const DUInt8&       max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DUInt16&      min,
                           const DUInt16&      max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DUInt32&      min,
                           const DUInt32&      max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DUInt64&      min,
                           const DUInt64&      max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DInt8&        min,
                           const DInt8&        max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DInt16&       min,
                           const DInt16&       max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DInt32&       min,
                           const DInt32&       max,
                           const ROW_INDEX     fromRow,
                           const ROW_INDEX     toRow,
                           const FIELD_INDEX   field);

  virtual DArray MatchRows(const DInt64&         min,
                           const DInt64&         max,
                           const ROW_INDEX       fromRow,
                           const ROW_INDEX       toRow,
                           const FIELD_INDEX     field);

  virtual DArray MatchRows(const DReal&          min,
                           const DReal&          max,
                           const ROW_INDEX       fromRow,
                           const ROW_INDEX       toRow,
                           const FIELD_INDEX     field);

  virtual DArray MatchRows(const DRichReal&      min,
                           const DRichReal&      max,
                           const ROW_INDEX       fromRow,
                           const ROW_INDEX       toRow,
                           const FIELD_INDEX     field);
  virtual void Flush() override;
  virtual void LockTable() override;
  virtual void UnlockTable() override;
  virtual void ReleaseFromDbs() final override;

  static GenericTable& Instance();

private:
  GenericTable() = default;
};


} //namespace prima
} //namespace whais


#endif /* PM_GENERAL_TABLE_H_ */
