//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  StringBuffer.HPP
// PURPOSE: 
// AUTHOR:  BHI  Nov 1999
//

#ifndef INC_STRINGBUFFER_HPP
#define INC_STRINGBUFFER_HPP

#pragma once
#include <vector>
#pragma pack(push, 1)

class CStringBufferBlock
{
private:
   enum { eBLOCK_SIZE = 4092 };
   UINT m_uNext;
   char m_szData[eBLOCK_SIZE];

public:
   CStringBufferBlock();

   BOOL   strcat(LPCSTR psz);
   BOOL   strncat(LPCSTR psz, UINT uLen);
   BOOL   vsprintf(LPCSTR pszFmt, va_list);

   LPCSTR Get() const;
   void   Clear();
   static UINT GetSize() { return eBLOCK_SIZE; }
};

#pragma pack(pop)

class CStringBuffer
{
private:   // Attributes
   // Typedefs to simplify code.
   std::vector<CStringBufferBlock *> m_Cache;

private:   // Unimplemented copy functions.
   CStringBuffer(const CStringBuffer &);
   const CStringBuffer &operator=(const CStringBuffer &);

private:   // Private functions

public:    // Public interface
   CStringBuffer();
   ~CStringBuffer();

   void   Clear();
   BOOL   strcat(LPCSTR psz);
   BOOL   sprintf(LPCSTR pszFormat, ...);
   UINT   strlen() const;
   UINT   GetChunkCount() const     { return m_Cache.size(); }
   LPCSTR GetChunk(UINT uInx) const { return m_Cache[uInx]->Get(); }
};

#endif      // INC_STRINGBUFFER_HPP

