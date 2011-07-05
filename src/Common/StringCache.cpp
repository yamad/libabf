//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  StringCache.CPP
// PURPOSE: Cache of strings stored efficiently and quick to access.
// AUTHOR:  BHI  Nov 1999
//
#include "wincpp.hpp"
#include "StringCache.hpp"

#pragma warning(disable : 4201)
#include <mmsystem.h>

const UINT c_uEmptyString = 0;

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CStringCacheBlock::CStringCacheBlock()
{
   MEMBERASSERT();
   m_uNext = 0;
}

//===============================================================================================
// FUNCTION: Add
// PURPOSE:  Add a new string into the cache.
//
BOOL CStringCacheBlock::Add(LPCSTR psz, WORD &wTag)
{
   MEMBERASSERT();
   UINT l = strlen(psz)+1;
   ASSERT(l <= ELEMENTS_IN(m_szData));

   // If no room in this block, fail.
   if (sizeof(m_szData)-m_uNext < l)
      return FALSE;

   // Save the current position as the tag.
   wTag = WORD(m_uNext);

   // Copy the string into position and step on past the terminating zero.
   strcpy(m_szData+m_uNext, psz);
   m_uNext += l;
   return TRUE;
}

//===============================================================================================
// FUNCTION: Get
// PURPOSE:  Get the string pointer that corresponds to the tag.
//
LPCSTR CStringCacheBlock::Get(WORD wTag) const
{
   MEMBERASSERT();
   if (wTag < m_uNext)
      return m_szData+wTag;

   ERRORMSG1("Bad tag passed to CStringCacheBlock (%d)", wTag);
   return NULL;
}

//===============================================================================================
// FUNCTION: Clear
// PURPOSE:  Clear the block.
//
void CStringCacheBlock::Clear()
{
   m_uNext = 0;
}

//###############################################################################################
//###############################################################################################
//###############################################################################################
//###############################################################################################

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CStringCache::CStringCache()
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CStringCache::~CStringCache()
{
   MEMBERASSERT();
   Clear();
}

//===============================================================================================
// FUNCTION: Clear
// PURPOSE:  Clear the cache.
//
void CStringCache::Clear()
{
   MEMBERASSERT();
   for (UINT i=0; i<m_Cache.size(); i++)
   {
      delete m_Cache[i];
      m_Cache[i] = NULL;
   }
   m_Cache.clear();
}

//===============================================================================================
// FUNCTION: Add
// PURPOSE:  Add a new string into the cache.
//
UINT CStringCache::Add(LPCSTR psz)
{
   MEMBERASSERT();
   WORD wTag = 0;
   if (psz[0]=='\0')
      return c_uEmptyString;

   // check the last couple of blocks to see if there is room.
   if (m_Cache.size())
   {
      WORD wBlock = WORD(m_Cache.size()-1);
      if (m_Cache[wBlock]->Add(psz, wTag))
         return MAKELONG(wTag, wBlock);
      if (wBlock && m_Cache[wBlock-1]->Add(psz, wTag))
         return MAKELONG(wTag, wBlock-1);
   }

   // If no room, create a new object and add the string to it.
   CStringCacheBlock *pBlock = new CStringCacheBlock;
   if (!pBlock)
      return BAD_TAG;

   // If this is the first string, put in the empty string first.
   if (m_Cache.size()==0)
      VERIFY(pBlock->Add("", wTag));

   // Add the string to the block.
   if (!pBlock->Add(psz, wTag))
   {
      delete pBlock;
      return BAD_TAG;
   }

   // Add it into the end of the list
   m_Cache.push_back(pBlock);

   // Return the tag.
   WORD wBlock = WORD(m_Cache.size()-1);
   return MAKELONG(wTag, wBlock);
}

//===============================================================================================
// FUNCTION: Get
// PURPOSE:  Get the string pointer that corresponds to the tag.
//
LPCSTR CStringCache::Get(UINT uTag) const
{
   MEMBERASSERT();

   if (uTag==c_uEmptyString)
      return "";

   WORD wBlock = HIWORD(uTag);
   WORD wTag   = LOWORD(uTag);
   if (wBlock < m_Cache.size())
      return m_Cache[wBlock]->Get(wTag);

   ERRORMSG1("Bad tag passed to CStringCache (%d)", uTag);
   return NULL;
}

//#################################################################################################
//#################################################################################################
//###
//###  File I/O code.
//###
//#################################################################################################
//#################################################################################################

#include "FileIO.hpp"
const DWORD c_dwSIGNATURE         = MAKEFOURCC('S','C','H','S');   // String Cache Header Signature
const DWORD c_dwCURRENT_VERSION   = MAKEFOURCC(1,0,0,0);           // 1.0.0.0

struct StringCacheHeader
{
   DWORD dwSignature;
   DWORD dwVersion;
   UINT  uBlocks;
   UINT  uUnused[5];

   StringCacheHeader()
   {
      memset(this, 0, sizeof(*this));
      dwSignature = c_dwSIGNATURE;
      dwVersion   = c_dwCURRENT_VERSION;
   }
};

//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Write the cache to a file.
//
BOOL CStringCache::Write(HANDLE hFile, UINT &uOffset) const
{
   MEMBERASSERT();

   // Object wrapper for the file handle.
   CFileIO_NoClose File(hFile);

   // Get the current position.
   LONGLONG lHeaderPos = 0;
   if (!File.GetCurrentPosition(&lHeaderPos))
      return FALSE;

   // Build the header and write it to the file.
   StringCacheHeader Header;
   if (!File.Write(&Header, sizeof(Header)))
      return FALSE;
   
   // Go through the blocks, writing them to the file.
   for (UINT i=0; i<m_Cache.size(); i++)
      if (!File.Write(m_Cache[i], sizeof(CStringCacheBlock)))
         return FALSE;

   LONGLONG lSavePos = 0;
   File.GetCurrentPosition(&lSavePos);

   Header.uBlocks = m_Cache.size();
   File.Seek(lHeaderPos);
   File.Write(&Header, sizeof(Header));
   File.Seek(lSavePos);

   uOffset = UINT(lHeaderPos);
   return TRUE;
}

//===============================================================================================
// FUNCTION: Read
// PURPOSE:  Read the cache from a file.
//
BOOL CStringCache::Read(HANDLE hFile, UINT uOffset)
{
   MEMBERASSERT();
   Clear();
   
   // Object wrapper for the file handle.
   CFileIO_NoClose File(hFile);

   // Get the current position.
   if (!File.Seek(uOffset))
      return FALSE;

   // Read the header from the file.
   StringCacheHeader Header;
   if (!File.Read(&Header, sizeof(Header)))
      return FALSE;

   if ((Header.dwSignature != c_dwSIGNATURE) || (Header.dwVersion != c_dwCURRENT_VERSION))
      return FALSE;

   for (UINT i=0; i<Header.uBlocks; i++)
   {
      CStringCacheBlock *pBlock = new CStringCacheBlock;
      if (!pBlock || !File.Read(pBlock, sizeof(*pBlock)))
      {
         delete pBlock;
         Clear();
         return FALSE;
      }
      m_Cache.push_back(pBlock);
   }

   return TRUE;
}
