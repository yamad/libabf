//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  TextFile.CPP
// PURPOSE: Contains the member functions for the CTextFile class.
//  

#include "wincpp.hpp"
#include "TextFile.hpp"

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initialize the object
//
CTextFile::CTextFile()
{
   MEMBERASSERT();
   m_pszNext    = NULL;
   m_uLineCount = 0;
}

//===============================================================================================
// FUNCTION: AllocBuffer
// PURPOSE:  Allocates the buffer to read text into
//
BOOL CTextFile::AllocBuffer(int nMaxLen)
{
   MEMBERASSERT();
   m_pszNext = NULL;

   if (!m_szBuffer.Alloc(nMaxLen+1))
      return FALSE;
   m_pszNext    = m_szBuffer;
   m_pszNext[0] = '\0';
   return TRUE;
}

//===============================================================================================
// FUNCTION: Open
// PURPOSE:  Opens the file for reading.
//
BOOL CTextFile::Open(LPCSTR pszFilename)
{
   MEMBERASSERT();
   Close();
   return m_File.Create(pszFilename, TRUE);
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Closes the file.
//
void CTextFile::Close()
{
   m_File.Close();
   m_szBuffer.Free();
   m_pszNext    = NULL;
   m_uLineCount = 0;
}

//===============================================================================================
// FUNCTION: FindTermination
// PURPOSE:  Tries to find a string terminated with a CrLf pair or a single Lf character.
//
LPSTR CTextFile::FindTermination()
{
   LPSTR pszRval = m_pszNext;
   LPSTR psz = strchr(m_pszNext, '\r');
   if (!psz)
   {
      // try for a '\n' character.
      psz = strchr(m_pszNext, '\n');
      if (!psz)
         return NULL;

      // Whack the '\n' character.
      *psz++ = '\0';
   }
   else
   {
      // If we cannot determine the next character, fail so as to get more read in.
      if (psz[1] == '\0')
         return NULL;

      // Whack the '\r' character.
      *psz++ = '\0';

      // If it is followed by a '\n' whack it too.
      if (*psz == '\n')
         *psz++ = '\0';
   }
   // Save the start of the next string for next time.
   m_pszNext = psz;
   return pszRval;
}

//===============================================================================================
// FUNCTION: GetLine
// PURPOSE:  Returns a pointer to the next line from the file.
//
LPSTR CTextFile::GetLine()
{
   ASSERT(m_szBuffer.GetCount() > 0);
   if (!m_szBuffer)
      return NULL;

   m_uLineCount++;
   if (m_pszNext[0])
   {
      LPSTR psz = FindTermination();
      if (psz)
         return psz;

      memmove(m_szBuffer, m_pszNext, strlen(m_pszNext)+1);
      m_pszNext = m_szBuffer;
   }
   else
   {
      m_pszNext = m_szBuffer;
      m_pszNext[0] = '\0';
   }
   UINT uLen = strlen(m_pszNext);
   UINT uMaxRead = m_szBuffer.GetCount() - 1 - uLen;
   DWORD dwBytesRead = 0;
   m_File.Read(m_pszNext+uLen, uMaxRead, &dwBytesRead);

   m_pszNext[uLen+dwBytesRead] = '\0';
   LPSTR psz = FindTermination();
   if (psz)
      return psz;

   psz = m_pszNext;
   m_pszNext += strlen(m_pszNext);
   if (psz[0])
      return psz;
   
   m_uLineCount--;
   return NULL;
}

//===============================================================================================
// FUNCTION: GetCurrentPos
// PURPOSE:  Get the current byte offset in the file.
//
UINT CTextFile::GetCurrentPos()
{
   MEMBERASSERT();

   LONGLONG lCurrentPosition = 0;
   m_File.GetCurrentPosition(&lCurrentPosition);
   UINT uPos = UINT(lCurrentPosition);
   uPos -= strlen(m_pszNext);
   return uPos;
}

//===============================================================================================
// FUNCTION: SetCurrentPos
// PURPOSE:  Set the current file pointer, flushing the cache.
//
void CTextFile::SetCurrentPos(UINT uPos)
{
   MEMBERASSERT();
   m_File.Seek(uPos);
   m_pszNext = m_szBuffer;
   *m_pszNext = '\0';
}

//===============================================================================================
// FUNCTION: GetFilename
// PURPOSE:  .
//
LPCSTR CTextFile::GetFilename() const
{
   MEMBERASSERT();
   return m_File.GetFileName();
}

//##############################################################################################
//##############################################################################################
//###
//###   CLASS: CTextLineParser
//###
//###
//##############################################################################################
//##############################################################################################

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CTextLineParser::CTextLineParser(LPSTR pszLine, char cSeperator)
{
   MEMBERASSERT();
   m_pszNext    = pszLine;
   m_cSeperator = cSeperator;
}

//===============================================================================================
// FUNCTION: StripSpaces
// PURPOSE:  Strips leading and trailing spaces from the string.
//
static LPSTR StripSpaces(LPSTR psz)
{
   LPSZASSERT(psz);
   while (*psz==' ')
      psz++;

   if (*psz)
   {
      LPSTR pszEnd = psz+strlen(psz);
      while ((pszEnd > psz) && (pszEnd[-1]==' '))
         --pszEnd;
      *pszEnd = '\0';
   }
   return psz;
}

//===============================================================================================
// FUNCTION: GetNextItem
// PURPOSE:  Returns a pointer to the next data item.
//
LPSTR CTextLineParser::GetNextItem(BOOL bCollectSeperators)
{
   MEMBERASSERT();
   if (!m_pszNext)
      return NULL;

   if (!m_pszNext[0])
   {
      m_pszNext = NULL;
      return "";
   }

   LPSTR pszStart = m_pszNext;
   if (m_cSeperator != ' ')
      pszStart = StripSpaces(m_pszNext);
   LPSTR pszEnd   = pszStart;

   if (*pszStart=='"')
   {
      pszEnd = strchr(++pszStart, '"');
      if (pszEnd && *pszEnd)
         *pszEnd++ = '\0';
   }

   if (pszEnd)
      pszEnd = strchr(pszEnd, m_cSeperator);

   if (pszEnd)
   {
      *pszEnd++ = '\0';
      if (bCollectSeperators)
         while (*pszEnd==m_cSeperator)
            *pszEnd++ = '\0';
   }

   m_pszNext = pszEnd;
   if (m_cSeperator != ' ')
      pszStart = StripSpaces(pszStart);
   return pszStart;
}

//===============================================================================================
// FUNCTION: GetRemaining
// PURPOSE:  Return the remaining text.
//
LPSTR CTextLineParser::GetRemaining()
{
   MEMBERASSERT();
   if (!m_pszNext || !m_pszNext[0])
      return NULL;
   if (m_cSeperator != ' ')
      return StripSpaces(m_pszNext);
   return m_pszNext;
}
