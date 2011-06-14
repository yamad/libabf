//***********************************************************************************************
//
//    Copyright (c) 1997-1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  ATFFileIO.hpp
// PURPOSE: Contains class definition for CATFFileIO class.
//          This is a class wrapper for the ATF file API.
// AUTHOR:  BHI  May 1997
//

#ifndef INC_ATFFILEIO_HPP
#define INC_ATFFILEIO_HPP

#pragma once

#include "\AxonDev\Comp\AxATFFio32\AxATFFio32.h"

//===============================================================================================
// CLASS:   CATFFileIO
// PURPOSE: Object wrapper for the ATF file I/O functions.
//
class CATFFileIO
{
private:
   int   m_hFile;                   // Handle to the ATF file.
   char  m_szFilename[_MAX_PATH];   // The filename of the ATF file.
   int   m_nLastError;              // Last ATF error number.

private:
   CATFFileIO(const CATFFileIO &);
   const CATFFileIO &operator=(const CATFFileIO &);
   
public:
   // Constructor / destructor
   CATFFileIO();
   ~CATFFileIO();

   /////////////////////////////////////////////////////////////////////////
   // ATF writing functions

   BOOL WriteOpen(LPCSTR pszFilename, int nColumns);
   void Close();

   BOOL WriteHeader(LPCSTR pszText);
   BOOL WriteHeader(UINT uTplt, ...);
   BOOL SprintfHeader(LPCSTR pszFormat, ...);
   BOOL SetColumnHeading(LPCSTR pszTitle, LPCSTR pszUnits=NULL);

   BOOL WriteData(float *pfArray, UINT uCount, LPCSTR pszComment=NULL);
   BOOL WriteData(double *pdArray, UINT uCount, LPCSTR pszComment=NULL);
   BOOL WriteData(LPCSTR pszData, LPCSTR pszComment=NULL);
   BOOL WriteDataNoEOL(LPCSTR pszData);
   BOOL WriteDataNoEOL(double dData);
   BOOL WriteTextItem(LPCSTR pszData);
   BOOL WriteItem(LPCSTR pszData);
   BOOL WriteEndOfLine();

   /////////////////////////////////////////////////////////////////////////
   // ATF reading functions

   BOOL ReadOpen(LPCSTR pszFileName, int *pnColumns=NULL, int *pnHeaders=NULL);
   BOOL ReadHeader(LPSTR pszHeaderText, int nMaxSize);
   BOOL GetColumnHeading(int nColumn, LPSTR pszTitleText, int nMaxTitle, 
                         LPSTR pszUnitsText=NULL, int nMaxUnit=0);
   BOOL CountDataLines(long *plNumLines);
   BOOL ReadData(int nCount, double *pdVals, LPSTR pszComment, int nMaxLen);
   BOOL ReadData(LPSTR pszText, int nMaxLen);
   BOOL Rewind();

   /////////////////////////////////////////////////////////////////////////
   // Get/Set last error

   BOOL SetLastError(int nError);
   int  GetLastError() const;
   void BuildErrorText( LPSTR pszTxtBuf, int nMaxLen) const;
   LPCSTR GetFileName() const;
};

//===============================================================================================
// CLASS:   CATFLineParser
// PURPOSE: Paser class for returning strings from an ATF data line.
//
class CATFLineParser
{
private:
   LPSTR m_pszNext;

public:
   CATFLineParser(LPSTR pszLine);

   LPSTR GetNextItem();
   LPSTR GetRemaining();
};

#endif   // INC_ATFFILEIO_HPP
