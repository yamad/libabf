//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// HEADER:  TextBuffer.HPP
// PURPOSE: Contains class definition CTextBuffer.
//
#ifndef INC_TEXTBUFFER_HPP
#define INC_TEXTBUFFER_HPP

class CTextBuffer
{
private:
   UINT  m_uBufSize;
   UINT  m_uBytesFree;
   LPSTR m_pszBuffer;

private:
   CTextBuffer();
   CTextBuffer(const CTextBuffer &);
   const CTextBuffer &operator=(const CTextBuffer &);

public:
   CTextBuffer(char *psBuffer, UINT uBufferSize);
   ~CTextBuffer();

   BOOL   AddLine(LPCSTR pszNewLine);
   int    printf(LPCSTR pszFormat, ...);
   LPCSTR GetBuffer() const;
   UINT   GetBytesUsed() const;
};

#endif      // INC_TEXTBUFFER_HPP
