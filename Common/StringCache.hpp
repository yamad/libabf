//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  StringCache.HPP
// PURPOSE: 
// AUTHOR:  BHI  Nov 1999
//

#ifndef INC_STRINGCACHE_HPP
#define INC_STRINGCACHE_HPP

#pragma once
#include <vector>
#pragma pack(push, 1)

const UINT BAD_TAG = UINT(-1);

class CStringCacheBlock
{
private:
   UINT m_uNext;
   char m_szData[2044];

public:
   CStringCacheBlock();

   BOOL   Add(LPCSTR psz, WORD &wTag);
   LPCSTR Get(WORD wOffset) const;
   void   Clear();
};

#pragma pack(pop)

class CStringCache
{
private:   // Attributes
   // Typedefs to simplify code.
   std::vector<CStringCacheBlock *> m_Cache;
   UINT                             m_uNext;

private:   // Unimplemented copy functions.
   CStringCache(const CStringCache &);
   const CStringCache &operator=(const CStringCache &);

private:   // Private functions

public:    // Public interface
   CStringCache();
   ~CStringCache();

   UINT   Add(LPCSTR psz);
   LPCSTR Get(UINT uTag) const;
   void   Clear();

   BOOL   Write(HANDLE hFile, UINT &uOffset) const;
   BOOL   Read(HANDLE hFile, UINT uOffset);
};

#endif      // INC_STRINGCACHE_HPP

