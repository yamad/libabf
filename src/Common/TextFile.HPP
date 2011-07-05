//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  TextFile.HPP
// PURPOSE: Contains the class definition for the CTextFile class, a simple text file reader.
// 

#ifndef INC_TEXTFILE_HPP
#define INC_TEXTFILE_HPP

#include "FileIO.hpp"
#include "ArrayPtr.hpp"

class CTextFile
{
private:
   CFileIO           m_File;
   CArrayPtrEx<char> m_szBuffer;
   char             *m_pszNext;
   UINT              m_uLineCount;

private:    // Prevent default copy constructor and operator=()
   CTextFile(const CTextFile &FI);
   const CTextFile &operator=(const CTextFile &FI);

private:
   LPSTR FindTermination();

public:
   CTextFile();
   BOOL  AllocBuffer(int nMaxLen);
   BOOL  Open(LPCSTR pszFilename);
   void  Close();
   LPSTR GetLine();
   UINT  GetCurrentPos();
   void  SetCurrentPos(UINT uPos);
   LPCSTR GetFilename() const;
};

//===============================================================================================
// CLASS:   CTextLineParser
// PURPOSE: Paser class for returning strings from a data line.
//
class CTextLineParser
{
private:
   LPSTR m_pszNext;
   char  m_cSeperator;

public:
   CTextLineParser(LPSTR pszLine, char cSeperator='\t');

   LPSTR GetNextItem(BOOL bCollectSeperators=FALSE);
   LPSTR GetRemaining();
};

#endif   // INC_TEXTFILE_HPP
