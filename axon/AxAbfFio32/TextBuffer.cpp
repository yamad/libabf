//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  TextBuffer.CPP   
// PURPOSE: A CTextBuffer object that expands as needed.
//
#include "wincpp.hpp"
#include <stdarg.h>
#include "TextBuffer.hpp"

CTextBuffer::CTextBuffer(char *psBuffer, UINT uBufferSize)
{
   MEMBERASSERT();

   m_uBufSize   = uBufferSize;
   m_uBytesFree = uBufferSize;
   m_pszBuffer  = psBuffer;
   if (psBuffer)
      *m_pszBuffer = '\0';
   m_uBytesFree--;
}

CTextBuffer::~CTextBuffer()
{
   MEMBERASSERT();
}

BOOL CTextBuffer::AddLine(LPCSTR pszNewLine)
{
   MEMBERASSERT();

   UINT l = (UINT)strlen(pszNewLine);
   if (l > m_uBytesFree)
      return FALSE;
      
   if (m_pszBuffer)
      strcat(m_pszBuffer, pszNewLine);
      
   m_uBytesFree -= l;
   return TRUE;
}

int CTextBuffer::printf(LPCSTR pszFormat, ...)
{
   MEMBERASSERT();

   char buf[512];
   va_list args;
   va_start(args, pszFormat);
   int nRval = _vsnprintf(buf, sizeof(buf), pszFormat, args);
   return AddLine(buf) ? nRval : 0;
}

LPCSTR CTextBuffer::GetBuffer() const
{
   MEMBERASSERT();

   return m_pszBuffer;
}

UINT CTextBuffer::GetBytesUsed() const
{
   MEMBERASSERT();

   return m_uBufSize - m_uBytesFree;
}

