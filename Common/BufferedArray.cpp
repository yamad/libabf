//***********************************************************************************************
//
//    Copyright (c) 1996-1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  BufferedArray.cpp
// PURPOSE: Contains class implementation for CBufferedArray for creating and maintaining a
//          cached array of structures.
// NOTES:   The cache is a write cache, no attempt is made to cache random access reads.
// AUTHOR:  BHI  Dec 1996
//

#include "wincpp.hpp"
#include "BufferedArray.hpp"

//===============================================================================================
// PROCEDURE: CBufferedArray
// PURPOSE:   Constructor. 
//
CBufferedArray::CBufferedArray()
{
   MEMBERASSERT();

   // Initialize the internal variables.
   m_uFileCount  = 0;
   m_uCacheCount = 0;
   m_pItemCache  = NULL;
   m_uItemSize   = 0;
   m_uCacheSize  = 0;
}

//===============================================================================================
// PROCEDURE: ~CBufferedArray
// PURPOSE:   Destructor. Closes the temporary file and deletes it.
//
CBufferedArray::~CBufferedArray()
{
   MEMBERASSERT();
   Close();
}

//===============================================================================================
// PROCEDURE: Initialize
// PURPOSE:   Gets a unique filename and opens it as a temporary file.
//
BOOL CBufferedArray::Initialize(UINT uItemSize, UINT uCacheSize)
{
   MEMBERASSERT();
   Close();

   // Allocate the disk transfer buffer
   m_pItemCache = new BYTE[uItemSize * uCacheSize];
   if (!m_pItemCache)
      return FALSE;

   m_uItemSize  = uItemSize;
   m_uCacheSize = uCacheSize;

   // Get a unique temporary file name with the prefix "AXO".
   char szTempPath[_MAX_PATH], szFileName[_MAX_PATH];
   if (!GetTempPath(sizeof(szTempPath), szTempPath))
      strcpy(szTempPath, "C:\\");
   VERIFY(GetTempFileName(szTempPath, "AXO", 0, szFileName));
   
   // Create the temporary file with the delete-on-close attribute.
   return m_File.Create(szFileName, FALSE, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_DELETE_ON_CLOSE);
}

//===============================================================================================
// PROCEDURE: Close
// PURPOSE:   Closes the current file and resets all counters etc.
//
void CBufferedArray::Close()
{
   // If the temp file was created OK, close it.
   // The FILE_FLAG_DELETE_ON_CLOSE ensures that it will be deleted also.
   m_File.Close();
   m_uFileCount  = 0;
   m_uCacheCount = 0;
   delete[] m_pItemCache;
   m_pItemCache = NULL;
   m_uItemSize   = 0;
   m_uCacheSize  = 0;
}

//===============================================================================================
// PROCEDURE: Empty
// PURPOSE:   Empties the array.
//
void CBufferedArray::Empty()
{
   MEMBERASSERT();

   // if it is already empty, just return.
   if (!m_pItemCache)
      return;

#ifdef _DEBUG  // clear the buffer if debug build -- helps with debugging.
   memset(m_pItemCache, 0, m_uItemSize * m_uCacheSize);
#endif

   VERIFY(m_File.Seek(0, FILE_BEGIN));
   VERIFY(m_File.SetEndOfFile());

   m_uFileCount  = 0;
   m_uCacheCount = 0;
}

//===============================================================================================
// PROCEDURE: Flush
// PURPOSE:   Flushes the cache to disk.
//
BOOL CBufferedArray::Flush()
{
   MEMBERASSERT();
   ASSERT(m_File.IsOpen());
   ASSERT(m_uCacheCount > 0);

   // Write out the current contents of the cache.
   UINT  uBytesToWrite  = m_uCacheCount * m_uItemSize;
   DWORD dwBytesWritten = 0;
   BOOL  bOK = m_File.Write(m_pItemCache, uBytesToWrite, &dwBytesWritten);

   // If successful, update counters and return.
   if (bOK)
   {
      m_uFileCount += m_uCacheCount;
      m_uCacheCount = 0;
      return TRUE;
   }

   // If a disk full error occurs, save what was actually written.
   UINT uWritten  = dwBytesWritten/m_uItemSize;
   m_uCacheCount -= uWritten; 
   m_uFileCount  += uWritten;

   // Set the file pointer at the end of the last complete item and truncate it there.
   VERIFY(m_File.Seek(m_uFileCount*m_uItemSize, FILE_BEGIN));
   VERIFY(m_File.SetEndOfFile());

   // Shuffle the unwritten data up and make it ready for next time.
   if (uWritten)
      memmove(m_pItemCache, m_pItemCache+uWritten*m_uItemSize, m_uCacheCount*m_uItemSize);
   return FALSE;
}

//===============================================================================================
// PROCEDURE: Update
// PURPOSE:   Updates an item in the array.
//
BOOL CBufferedArray::Update(UINT uItem, const void *pItem)
{
   MEMBERASSERT();
   ASSERT(m_File.IsOpen());
   ASSERT(m_uItemSize > 0);
   ASSERT(m_uCacheSize > 0);
   ARRAYASSERT((BYTE *)pItem, m_uItemSize);

   // If the item number is out of range, return error.
   ASSERT(uItem < GetCount());
   if (uItem >= GetCount())
      return FALSE;

   if (uItem < m_uFileCount)
   {
      // The item is in the file, update it in place.
      VERIFY(m_File.Seek(uItem*m_uItemSize, FILE_BEGIN));
      VERIFY(m_File.Write(pItem, m_uItemSize));
   }
   else
      // The item is in the cache.
      memcpy(m_pItemCache+(uItem-m_uFileCount)*m_uItemSize, pItem, m_uItemSize);
   return TRUE;
}

//===============================================================================================
// PROCEDURE: Get
// PURPOSE:   Retrieves one or more ITEMs from the virtualized array.
//
BOOL CBufferedArray::Get( UINT uFirstEntry, void *pvItems, UINT uEntries )
{
   MEMBERASSERT();
   ASSERT(m_File.IsOpen());
   ASSERT(uFirstEntry+uEntries <= GetCount());
   ASSERT(uEntries > 0);
   BYTE *pItems = (BYTE *)pvItems;

   // If the block requested is not contained completely in the cache, 
   // read the file for it, reading straight into the passed buffer.
   if (GetCount() - uFirstEntry > m_uCacheSize)
   {
      // Calculate how many entries there are from the requested first entry until
      // we hit the ones currently in the cache.
      UINT uCount = GetCount() - uFirstEntry - m_uCacheSize;
      
      // Limit the count to no greater than the requested amount.
      if (uCount > uEntries)
         uCount = uEntries;
      
      // Save current file position
      LONGLONG llCurrentPos = 0;
      if (!m_File.GetCurrentPosition(&llCurrentPos))
         return FALSE;

      // Seek to the start point.
      if (!m_File.Seek(uFirstEntry * m_uItemSize, FILE_BEGIN))
         return FALSE;

      // Read the items from the file.
      if (!m_File.Read(pItems, uCount * m_uItemSize))
      {
         VERIFY(m_File.Seek(llCurrentPos, FILE_BEGIN));
         return FALSE;
      }

      // Update the pointer and the counters.
      pItems      += uCount * m_uItemSize;
      uFirstEntry += uCount;
      uEntries    -= uCount;

      // Restore the file pointer to where it was on entry.
      VERIFY(m_File.Seek(llCurrentPos, FILE_BEGIN));
      
      if (uEntries == 0)
         return TRUE;
   }
   
   // Transfer the part of the buffer that is "invalidated", i.e. about to be overwritten.
   if (uFirstEntry < m_uFileCount)
   {
      UINT uCount = m_uFileCount - uFirstEntry;
      UINT uStart = m_uCacheSize - uCount;
      // copy the items.
      uCount = min(uCount, uEntries);
      memcpy(pItems, m_pItemCache + uStart * m_uItemSize, uCount*m_uItemSize);

      // Update the pointer and the counters.
      pItems      += uCount * m_uItemSize;
      uEntries    -= uCount;
      uFirstEntry += uCount;
      if (uEntries == 0)
         return TRUE;
   }
   
   // Transfer the more recently written part of the cache.
   memcpy(pItems, m_pItemCache + (uFirstEntry-m_uFileCount) * m_uItemSize, uEntries*m_uItemSize);
   return TRUE;
}

//===============================================================================================
// PROCEDURE: Write
// PURPOSE:   Copies the complete array to another file at the files's current position.
//
BOOL CBufferedArray::Write( HANDLE hDataFile )
{
   MEMBERASSERT();
   ASSERT( hDataFile != INVALID_HANDLE_VALUE );

   // Set the return value for the number of BufferedArray entries. If none exist, return.
   if (GetCount() == 0)
      return TRUE;

   // Save the current position of the passed file. This will only fail for invalid file handles.
   CFileIO_NoClose FOut(hDataFile);
   LONGLONG llCurrentPos = 0;
   if (!FOut.Seek(0L, FILE_CURRENT, &llCurrentPos))
      return FALSE;

   // Allocate a buffer the same size as the cache to transfer the data in.
   UINT  uBufSize = m_uCacheSize * m_uItemSize;
   BYTE *pBuffer = new BYTE[uBufSize];
   if (!pBuffer)
      return FALSE;

   // Seek to the start of the temporary file.
   VERIFY(m_File.Seek(0, FILE_BEGIN));

   // The number of items in the file must be a multiple of the cache size.
   ASSERT((m_uFileCount % m_uCacheSize)==0);

   BOOL bRval = TRUE;
   for (UINT i=0; i<m_uFileCount/m_uCacheSize; i++)
   {
      VERIFY(m_File.Read(pBuffer, uBufSize));

      // Write out to the file.
      if ( !FOut.Write( pBuffer, uBufSize ) )
      {
         // If an error occurs, go back to the start of the block and truncate the file
         // ready for the next attempt after the user has freed up some disk space.
         FOut.Seek(llCurrentPos, FILE_BEGIN);
         FOut.SetEndOfFile();
         bRval = FALSE;
         break;
      }
   }

   if (bRval && m_uCacheCount)
   {
      // Write out to the file.
      if ( !FOut.Write( m_pItemCache, m_uCacheCount*m_uItemSize ) )
      {
         // If an error occurs, go back to the start of the block and truncate the file
         // ready for the next attempt after the user has freed up some disk space.
         FOut.Seek(llCurrentPos, FILE_BEGIN);
         FOut.SetEndOfFile();
         bRval = FALSE;
      }
   }
   delete[] pBuffer;

   // Seek back to end of the temporary file.
   VERIFY(m_File.Seek(0, FILE_END));
   return bRval;
}

//===============================================================================================
// PROCEDURE: GetFilename
// PURPOSE:   Get the name of the temp file.
//
LPCSTR CBufferedArray::GetFilename() const
{
   MEMBERASSERT();
   return m_File.GetFileName();
}
