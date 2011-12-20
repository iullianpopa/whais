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
#include <string.h>
#include <memory>

#include "ps_container.h"

using namespace pastra;

static void
append_int_to_str (std::string & dest, D_UINT64 number)
{
  const D_UINT integer_bits = 64;
  D_CHAR buffer[integer_bits];
  D_CHAR *conv = &buffer[integer_bits - 1];

  *conv = 0;
  do
    {
      conv--;
      *conv = _SC (D_CHAR, ('0' + (number % 10)));
      number /= 10;
    }
  while (number != 0);

  dest += conv;
}

static void
safe_memcpy (D_UINT8 * pDest, D_UINT8 * pSrc, D_UINT64 uCount)
{
  while (uCount-- > 0)
    *pDest++ = *pSrc++;
}

FileContainer::FileContainer (const D_CHAR * pFileNameBase,
                              const D_UINT32 uMaxFileSize,
                              const D_UINT32 uUnitsCount):
    m_uMaxFileUnitSize (uMaxFileSize),
    m_FilesHandles (),
    m_FileNameBase (pFileNameBase),
    mIsMarked (false)
{
  D_UINT uOpenMode;

  uOpenMode = (uUnitsCount > 0) ? WHC_FILEOPEN_EXISTING : WHC_FILECREATE_NEW;
  uOpenMode |= WHC_FILERDWR;

  for (D_UINT uCounter = 0; uCounter < uUnitsCount; ++uCounter)
    {
      std::string fileName = m_FileNameBase;
      if (uCounter != 0)
        append_int_to_str (fileName, uCounter);

      WFile container (fileName.c_str (), uOpenMode);
      m_FilesHandles.push_back (container);
    }

  assert (m_FilesHandles.size () == uUnitsCount);

  // Check for structural consistency
  for (D_UINT uCounter = 0; uCounter < uUnitsCount; ++uCounter)
    {
      WFile & rContainerUnit = m_FilesHandles[uCounter];

      if (rContainerUnit.GetSize () != uMaxFileSize)
        if ((uCounter != (uUnitsCount - 1)) || (rContainerUnit.GetSize ()
                                                > uMaxFileSize))
          throw WFileContainerException ("Inconsistent container!",
                                         _EXTRA
                                         (WFileContainerException::CONTAINTER_INVALID));
    }

}

FileContainer::~FileContainer ()
{
  if (mIsMarked)
    ColapseContent (0, GetContainerSize() );
}

void
FileContainer::StoreData (D_UINT64 uPosition,
                          D_UINT64 uLenght, const D_UINT8 * puDataSource)
{
  D_UINT64 uContainerIndex = uPosition / m_uMaxFileUnitSize;
  D_UINT64 uUnitPosition = uPosition % m_uMaxFileUnitSize;
  const D_UINT uContainersCount = m_FilesHandles.size ();

  if (uContainerIndex > uContainersCount)
    throw WFileContainerException (NULL,
                                   _EXTRA
                                   (WFileContainerException::INVALID_ACCESS_POSITION));
  else if (uContainerIndex == uContainersCount)
    {
      if (uUnitPosition != 0)
        throw WFileContainerException (NULL,
                                       _EXTRA
                                       (WFileContainerException::INVALID_ACCESS_POSITION));
      else
        ExtendContainer ();
    }

  D_UINT uWriteLenght = uLenght;

  if ((uWriteLenght + uUnitPosition) > m_uMaxFileUnitSize)
    uWriteLenght = m_uMaxFileUnitSize - uUnitPosition;

  assert (uWriteLenght <= uLenght);

  WFile & rUnitContainer = m_FilesHandles[uContainerIndex];

  if (rUnitContainer.GetSize () < uUnitPosition)
    throw WFileContainerException (NULL,
                                   _EXTRA
                                   (WFileContainerException::INVALID_ACCESS_POSITION));

  rUnitContainer.Seek (uUnitPosition, WHC_SEEK_BEGIN);
  rUnitContainer.Write (puDataSource, uWriteLenght);

  //Let write the rest
  if (uWriteLenght < uLenght)
    StoreData (uPosition + uWriteLenght, uLenght - uWriteLenght, puDataSource
               + uWriteLenght);
}

void
FileContainer::RetrieveData (D_UINT64 uPosition,
                             D_UINT64 uLenght,
                             D_UINT8 * puDataDestination) const
{
  D_UINT64 uContainerIndex = uPosition / m_uMaxFileUnitSize;
  D_UINT64 uUnitPosition = uPosition % m_uMaxFileUnitSize;
  const D_UINT uContainersCount = m_FilesHandles.size ();

  if ((uContainerIndex > uContainersCount) || (uPosition + uLenght
      > GetContainerSize ()))
    throw WFileContainerException (NULL,
                                   _EXTRA
                                   (WFileContainerException::INVALID_ACCESS_POSITION));

  WFile rUnitContainer = m_FilesHandles[uContainerIndex];

  D_UINT uReadLenght = uLenght;

  if (uReadLenght + uUnitPosition > rUnitContainer.GetSize ())
    uReadLenght = rUnitContainer.GetSize () - uUnitPosition;

  rUnitContainer.Seek (uUnitPosition, WHC_SEEK_BEGIN);
  rUnitContainer.Read (puDataDestination, uReadLenght);

  //Lets read the rest
  if (uReadLenght != uLenght)
    RetrieveData (uPosition + uReadLenght,
                  uLenght - uReadLenght, puDataDestination + uReadLenght);

}

void
FileContainer::ColapseContent (D_UINT64 uStartPosition, D_UINT64 uEndPosition)
{
  const D_UINT cuBufferSize = 4096;	//4KB
  const D_UINT64 uIntervalSize = uEndPosition - uStartPosition;
  const D_UINT64 uContainerSize = GetContainerSize ();

  if ((uEndPosition < uStartPosition) || (uEndPosition > uContainerSize))
    throw WFileContainerException (NULL,
                                   _EXTRA
                                   (WFileContainerException::INVALID_PARAMETERS));
  else if (uIntervalSize == 0)
    return;

  std::auto_ptr < D_UINT8 > aBuffer (new D_UINT8[cuBufferSize]);

  while (uEndPosition < uContainerSize)
    {
      D_UINT64 uStepSize = cuBufferSize;

      if (uStepSize + uEndPosition > uContainerSize)
        uStepSize = uContainerSize - uEndPosition;

      RetrieveData (uEndPosition, uStepSize, aBuffer.get ());
      StoreData (uStartPosition, uStepSize, aBuffer.get ());

      uEndPosition += uStepSize;
      uStartPosition += uStepSize;
    }

  //Let's delete the remaining junk.
  const D_UINT64 uNewSize = uContainerSize - uIntervalSize;
  D_INT uLastContainer = uNewSize / m_uMaxFileUnitSize;
  const D_INT uLastContainerSize = uNewSize % m_uMaxFileUnitSize;

  if (uNewSize == 0)
    --uLastContainer;
  else
    m_FilesHandles[uLastContainer].SetSize (uLastContainerSize);

  for (D_INT uIndex = m_FilesHandles.size () - 1; uIndex > uLastContainer;
       --uIndex)
    {
      m_FilesHandles[uIndex].Close ();

      std::string fileName = m_FileNameBase;
      if (uIndex != 0)
        append_int_to_str (fileName, uIndex);

      if (!whc_fremove (fileName.c_str ()))
        throw WFileContainerException (NULL,
                                       _EXTRA
                                       (WFileContainerException::FILE_OS_IO_ERROR));

      m_FilesHandles.pop_back ();
    }

}

D_UINT64
FileContainer::GetContainerSize () const
{
  if (m_FilesHandles.size () == 0)
    return 0;

  const WFile & rLastUnit = m_FilesHandles[m_FilesHandles.size () - 1];
  D_UINT64 uResult = (m_FilesHandles.size () - 1) * m_uMaxFileUnitSize;

  uResult += rLastUnit.GetSize ();

  return uResult;
}

void
FileContainer::MarkForRemoval()
{
  mIsMarked = true;
}

void
FileContainer::ExtendContainer ()
{
  D_UINT uCount = m_FilesHandles.size ();
  std::string fileName = m_FileNameBase;

  if (uCount != 0)
    append_int_to_str (fileName, uCount);

  WFile container (fileName.c_str (), WHC_FILECREATE_NEW | WHC_FILERDWR);
  m_FilesHandles.push_back (container);
}

////////////WTempFileContainer///////////////////////////////////////////////

FileTempContainer::FileTempContainer (const D_CHAR * pFileNameBase,
                                      const D_UINT32 uMaxFileSize):
    FileContainer (pFileNameBase, uMaxFileSize, 0)
{
  MarkForRemoval ();
}

FileTempContainer::~FileTempContainer ()
{
}

//////////////////WTemCotainer/////////////////////////////////////////////////

TempContainer::TempContainer (const D_CHAR * pTempDirectory,
                              D_UINT uReservedMemory):
    I_DataContainer (),
    mContainer (NULL),
    mMemory (new D_UINT8[uReservedMemory]),
    mTempDirectory (pTempDirectory),
    muMemorySize (uReservedMemory),
    muValidMemory (0)
{
}

TempContainer::~TempContainer ()
{
}

void
TempContainer::StoreData (D_UINT64 uPosition,
                          D_UINT64 uLenght,
                          const D_UINT8 * puDataSource)
{
  FileTempContainer *const pContainer = mContainer.get ();

  if (pContainer != NULL)
    {
      assert (mMemory.get () == NULL);
      return pContainer->StoreData (uPosition, uLenght, puDataSource);
    }
  else
    {
      D_UINT8 *const pMemBase = mMemory.get ();
      if (uPosition > muValidMemory)
        throw WFileContainerException (NULL, _EXTRA (WFileContainerException::INVALID_ACCESS_POSITION));

      //Check if the reserved capacity is exceeded.
      if ((uPosition + uLenght) >= muMemorySize)
        {
          SwitchToFileContent ();
          //Let's try again!
          return StoreData (uPosition, uLenght, puDataSource);
        }

      memcpy (pMemBase + uPosition, puDataSource, _SC (size_t, uLenght));
      if ((uPosition + uLenght) > muValidMemory)
        muValidMemory = uPosition + uLenght;
      assert (muValidMemory < muMemorySize);
    }
}

void
TempContainer::RetrieveData (D_UINT64 uPosition,
                             D_UINT64 uLenght,
                             D_UINT8 * puDataDestination) const
{

  if (uPosition + uLenght > GetContainerSize ())
    throw WFileContainerException (NULL,
                                   _EXTRA
                                   (WFileContainerException::INVALID_ACCESS_POSITION));

  D_UINT8 *const mem_base = mMemory.get ();

  if (mem_base != NULL)
    memcpy (puDataDestination, mem_base + uPosition, _SC (size_t, uLenght));
  else
    {
      FileTempContainer *const pContainer = mContainer.get ();
      pContainer->RetrieveData (uPosition, uLenght, puDataDestination);
    }
}

void
TempContainer::ColapseContent (D_UINT64 uStartPosition, D_UINT64 uEndPosition)
{

  D_UINT8 *const mem_base = mMemory.get ();

  if (mem_base != NULL)
    {
      assert (mContainer.get () == NULL);
      if ((uEndPosition < uStartPosition) || (uEndPosition > muValidMemory))
        throw WFileContainerException (NULL,
                                       _EXTRA
                                       (WFileContainerException::INVALID_PARAMETERS));

      D_UINT uCount = uEndPosition - uStartPosition;

      safe_memcpy (mem_base + uStartPosition, mem_base + uEndPosition, muValidMemory - uEndPosition);
      muValidMemory -= uCount;
    }
  else
    {
      FileTempContainer *const pContainer = mContainer.get ();
      pContainer->ColapseContent (uStartPosition, uEndPosition);
    }
}

void
TempContainer::MarkForRemoval ()
{
  return ; //This is automatically deleted. Nothing to do here!
}

D_UINT64
TempContainer::GetContainerSize () const
{
  FileTempContainer *const
  pContainer = mContainer.get ();

  if (pContainer != NULL)
    return pContainer->GetContainerSize ();

  assert (mMemory.get () != NULL);
  return muValidMemory;
}

void
TempContainer::SwitchToFileContent ()
{
  D_UINT64 uCurrentId = 0;

  TempContainer::mSync.Enter ();
  uCurrentId = TempContainer::mTemporalsCount++;
  TempContainer::mSync.Leave ();

  std::string fileName (mTempDirectory);
  fileName += "wtemp";
  append_int_to_str (fileName, uCurrentId);
  fileName += ".tm";

  std::auto_ptr < FileTempContainer >
  apTempContainer (new FileTempContainer (fileName.c_str (), 0x80000000));

  FileTempContainer *const pContainer = apTempContainer.get ();
  pContainer->StoreData (0, muValidMemory, mMemory.get ());

  mContainer.reset (apTempContainer.release ());
  mMemory.reset (NULL);
}

D_UINT64 TempContainer::mTemporalsCount = 0;
WSynchronizer TempContainer::mSync;