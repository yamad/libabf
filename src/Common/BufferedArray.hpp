//***********************************************************************************************
//
//    Copyright (c) 1996-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  BufferedArray.HPP
// PURPOSE: Contains class definition for CBufferedArray for creating and maintaining a BufferedArray array.
// AUTHOR:  BHI  Dec 1996
//

#ifndef INC_BUFFEREDARRAY_HPP
#define INC_BUFFEREDARRAY_HPP

#include "Common/FileIO.hpp"
//-----------------------------------------------------------------------------------------------
// CBufferedArray class definition

class CBufferedArray
{
private:
   // Member variables.
   UINT    m_uCacheCount;               // Count of items in the cache.
   UINT    m_uFileCount;                // Total count of items in the file.
   UINT    m_uItemSize;                 // Size of each item.
   UINT    m_uCacheSize;                // Number of items buffered in the cache.
   BYTE   *m_pItemCache;               // Buffer for caching items.
   CFileIO m_File;                      // Win32 temporary file object.

private:    // Unimplemented default member functions.
   // Declare but don't define copy constructors to prevent use of defaults.
   CBufferedArray(const CBufferedArray &CS);
   const CBufferedArray &operator=(const CBufferedArray &CS);
   
private:    // Private (implementation) member functions.
   
   // Close the file and initialize all counters.
   void Close();

   // Flush the buffer to disk.
   BOOL Flush();

public:     // Public member functions
   CBufferedArray();
   ~CBufferedArray();

   // Call this function to initialize the DTB and the temp file.
   BOOL Initialize(UINT uItemSize, UINT uBufferedItems);

   // Empty the array.
   void Empty();

   // Put an item into the array.
   BOOL Put( const void *pItem );

   // Updates an item in the array.
   BOOL Update( UINT uItem, const void *pItem );

   // Get one or more items from the array.
   BOOL Get( UINT uFirstEntry, void *pItem, UINT uEntries );

   // Get the count of items added to the array.
   UINT GetCount() const;

   // Get a pointer to the last item added to the array.
   void *GetLast();

   // Get the name of the temp file.
   LPCSTR GetFilename() const;

   // Copy the array to another file.
   BOOL Write( HANDLE hDataFile );
};

//===============================================================================================
// PROCEDURE: GetCount
// PURPOSE:   Returns the current count of ITEMs, both cached and written to file.
//
inline UINT CBufferedArray::GetCount() const
{
   MEMBERASSERT();
   return m_uFileCount + m_uCacheCount;
}

//===============================================================================================
// PROCEDURE: Put
// PURPOSE:   Put item into the array (memcpy).
//
inline BOOL CBufferedArray::Put(const void *pItem)
{
   MEMBERASSERT();
   ASSERT(m_File.IsOpen());
   ASSERT(m_uItemSize > 0);
   ASSERT(m_uCacheSize > 0);
   ARRAYASSERT((BYTE *)pItem, m_uItemSize);

   // If the cache is already full, flush it.
   if ((m_uCacheCount >= m_uCacheSize) && !Flush())
      return FALSE;

   // The item will now fit in the cache.
   memcpy(m_pItemCache+m_uCacheCount*m_uItemSize, pItem, m_uItemSize);
   ++m_uCacheCount;
   return TRUE;
}

//===============================================================================================
// PROCEDURE: GetLast
// PURPOSE:   Gets a pointer to the last item put in the array.
// NOTES:     As the buffer is flushed before writing when the cache is full, the
//            last item is always guaranteed to be present in the cache.
//
inline void *CBufferedArray::GetLast()
{
   MEMBERASSERT();
   ASSERT(m_uCacheCount > 0);
   return &m_pItemCache[(m_uCacheCount-1) * m_uItemSize];
}

#endif      // INC_BUFFEREDARRAY_HPP
