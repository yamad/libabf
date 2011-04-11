//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  StringBuffer.CPP
// PURPOSE: Cache of strings stored efficiently and quick to access.
// AUTHOR:  BHI  Nov 1999
//
#include "wincpp.hpp"
#include "StringBuffer.hpp"

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CStringBufferBlock::CStringBufferBlock()
{
   MEMBERASSERT();
   Clear();
}

//===============================================================================================
// FUNCTION: strcat
// PURPOSE:  Add a new string into the cache.
//
BOOL CStringBufferBlock::strcat(LPCSTR psz)
{
   MEMBERASSERT();
   LPSZASSERT(psz);

   ASSERT(m_szData[m_uNext] == '\0');
   LPSZASSERT(m_szData);

   UINT l = strlen(psz);
   if (sizeof(m_szData)-m_uNext <= l)
      return FALSE;

   ::strcpy(m_szData+m_uNext, psz);
   m_uNext += l;
   ASSERT(m_szData[m_uNext] == '\0');
   return TRUE;
}

//===============================================================================================
// FUNCTION: strncat
// PURPOSE:  Add a new string into the cache.
//
BOOL CStringBufferBlock::strncat(LPCSTR psz, UINT uLen)
{
   MEMBERASSERT();
   LPSZASSERT(psz);

   ASSERT(m_szData[m_uNext] == '\0');
   LPSZASSERT(m_szData);

   UINT l = min(uLen, strlen(psz));
   if (sizeof(m_szData)-m_uNext <= l)
      return FALSE;

   ::strncpy(m_szData+m_uNext, psz, l);
   m_uNext += l;
   m_szData[m_uNext] = '\0';
   return TRUE;
}

//===============================================================================================
// FUNCTION: vsprintf
// PURPOSE:  Add a new string into the cache.
//
BOOL CStringBufferBlock::vsprintf(LPCSTR pszFmt, va_list args)
{
   MEMBERASSERT();
   LPSZASSERT(pszFmt);

   ASSERT(m_szData[m_uNext] == '\0');
   LPSZASSERT(m_szData);

   int nMaxLen = sizeof(m_szData)-m_uNext-1;
   int nRval = _vsnprintf(m_szData+m_uNext, nMaxLen, pszFmt, args);
   if (nRval < 0 || nRval >= nMaxLen-1)
   {
      m_szData[m_uNext] = '\0';
      return FALSE;
   }
   m_uNext += nRval;
   ASSERT(m_szData[m_uNext] == '\0');
   return TRUE;
}

//===============================================================================================
// FUNCTION: Get
// PURPOSE:  Get the string pointer that corresponds to the tag.
//
LPCSTR CStringBufferBlock::Get() const
{
   MEMBERASSERT();
   ASSERT(m_szData[m_uNext] == '\0');
   LPSZASSERT(m_szData);
   return m_szData;
}

//===============================================================================================
// FUNCTION: Clear
// PURPOSE:  Clear the cache.
//
void CStringBufferBlock::Clear()
{
   m_uNext     = 0;
   m_szData[0] = '\0';
}

//###############################################################################################
//###############################################################################################
//###############################################################################################
//###############################################################################################

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CStringBuffer::CStringBuffer()
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CStringBuffer::~CStringBuffer()
{
   MEMBERASSERT();
   Clear();
}

//===============================================================================================
// FUNCTION: Clear
// PURPOSE:  Clear the cache.
//
void CStringBuffer::Clear()
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
BOOL CStringBuffer::strcat(LPCSTR psz)
{
   MEMBERASSERT();
   if (!psz)
      return TRUE;
   LPSZASSERT(psz);

   // check the last block to see if there is room.
   UINT uBlocks = m_Cache.size();
   if (uBlocks && m_Cache[uBlocks-1]->strcat(psz))
      return TRUE;

   // If no room, create a new object and add the string to it.
   UINT uLen = ::strlen(psz);
   while (uLen)
   {
      CStringBufferBlock *pBlock = new CStringBufferBlock;
      UINT l = min(uLen, CStringBufferBlock::GetSize()-1);
      if (!pBlock || !pBlock->strncat(psz, l))
      {
         delete pBlock;
         return FALSE;
      }
      // Add it into the end of the list
      m_Cache.push_back(pBlock);
      uLen -= l;
      psz  += l;
   }

   return TRUE;
}

//===============================================================================================
// FUNCTION: Add
// PURPOSE:  Add a new string into the cache.
//
BOOL CStringBuffer::sprintf(LPCSTR pszFmt, ...)
{
   MEMBERASSERT();
   LPSZASSERT(pszFmt);

   va_list args;                  
   va_start(args, pszFmt);
   LPSZASSERT(pszFmt);

   // check the last block to see if there is room.
   UINT uBlocks = m_Cache.size();
   if (uBlocks && m_Cache[uBlocks-1]->vsprintf(pszFmt, args))
      return TRUE;

   // If no room, create a new object and add the string to it.
   CStringBufferBlock *pBlock = new CStringBufferBlock;
   if (!pBlock || !pBlock->vsprintf(pszFmt, args))
   {
      delete pBlock;
      return FALSE;
   }

   // Add it into the end of the list
   m_Cache.push_back(pBlock);
   return TRUE;
}

//===============================================================================================
// FUNCTION: strlen
// PURPOSE:  Returns the total number of characters held.
//
UINT CStringBuffer::strlen() const
{
   MEMBERASSERT();

   UINT uLen = 0;
   for (UINT i=0; i<m_Cache.size(); i++)
      uLen += ::strlen(m_Cache[i]->Get());
   return uLen;
}
