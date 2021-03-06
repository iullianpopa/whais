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

#ifndef PS_BLOCKCACHE_H_
#define PS_BLOCKCACHE_H_

#include <map>
#include <assert.h>
#include <string.h>

#include "whais.h"
#include "utils/wthread.h"


namespace whais {
namespace pastra  {


class IBlocksManager
{
public:
  virtual ~IBlocksManager() = default;

  virtual void StoreItems(uint64_t firstItem, uint_t itemsCount, const uint8_t* const from) = 0;
  virtual void RetrieveItems(uint64_t firstItem, uint_t itemsCount, uint8_t* const to) = 0;
};


class BlockEntry
{
public:
  explicit BlockEntry(uint8_t* const data)
    : mData(data),
      mReferenceCount(0),
      mFlags(0)
  {
    assert(data != nullptr);
  }

  bool IsDirty() const { return (mFlags & BLOCK_ENTRY_DIRTY) != 0; }
  bool IsInUse() const { return mReferenceCount > 0; }
  void MarkDirty() { mFlags |= BLOCK_ENTRY_DIRTY; }
  void MarkClean() { mFlags &= ~BLOCK_ENTRY_DIRTY; }
  uint8_t* Data() { return mData; }

  void RegisterUser() { wh_atomic_fetch_inc32(_RC(int32_t*, &mReferenceCount)); }
  void ReleaseUser()
  {
    assert( mReferenceCount > 0);
    wh_atomic_fetch_dec32(_RC(int32_t*, &mReferenceCount));
  }

private:
  uint8_t* const   mData;
  uint32_t         mReferenceCount;
  uint32_t         mFlags;

  static const uint32_t BLOCK_ENTRY_DIRTY = 0x00000001;
};


class StoredItem
{
public:
  StoredItem(BlockEntry& blockEntry, const uint_t itemOffset)
    : mBlockEntry(&blockEntry),
      mItemOffset(itemOffset)
  {
    mBlockEntry->RegisterUser();
  }

  StoredItem(const StoredItem& src) :
    mBlockEntry(src.mBlockEntry),
    mItemOffset(src.mItemOffset)
  {
    mBlockEntry->RegisterUser();
  }

  ~StoredItem() { mBlockEntry->ReleaseUser(); }

  StoredItem& operator= (const StoredItem& src)
  {
    if (this == &src)
      return *this;

    src.mBlockEntry->RegisterUser();
    mBlockEntry->ReleaseUser();

    _CC(BlockEntry*&, mBlockEntry) = src.mBlockEntry;
    _CC(uint_t&, mItemOffset) = src.mItemOffset;

    return *this;
  }

  uint8_t* GetDataForUpdate() const
  {
    mBlockEntry->MarkDirty();
    return mBlockEntry->Data() + mItemOffset;
  }

  const uint8_t* GetDataForRead() const { return mBlockEntry->Data() + mItemOffset; }

protected:

  BlockEntry* const   mBlockEntry;
  const uint_t        mItemOffset;
};


class BlockCache
{
public:
  BlockCache();
  ~BlockCache();

  void Init(IBlocksManager&   blocksMgr,
            const uint_t      itemSize,
            const uint_t      blockSize,
            const uint_t      maxCachedBlocks,
            const bool        nonPersitentData);

  void Flush();
  void FlushItem(const uint64_t item);
  void RefreshItem(const uint64_t item);
  StoredItem RetriveItem(const uint64_t item);

private:
  IBlocksManager  *mManager;
  uint_t           mItemSize;
  uint_t           mBlockSize;
  uint_t           mMaxCachedBlocks;
  bool             mSkipFlush;

  std::map<uint64_t, BlockEntry> mCachedBlocks;
};


} //namespace pastra
} //namespace whais


#endif /* PS_BLOCKCACHE_H_ */
