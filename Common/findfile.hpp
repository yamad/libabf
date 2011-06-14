//***********************************************************************************************
//
//    Copyright (c) 1994 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  findfile.hpp
// PURPOSE: Contains class definition and inline functions for the CFindFile class.
// AUTHOR:  BHI  Apr 1994
//

#ifndef INC_FINDFILE_HPP
#define INC_FINDFILE_HPP

#include "..\common\axodebug.h"
#include "..\common\axodefn.h"


//***********************************************************************************************
// Class definition
//

class CFindFile
{
private:
   WIN32_FIND_DATA m_FindData;
   HANDLE m_hFindFile;

public:
   CFindFile();
   CFindFile(LPCSTR lpcszSearchName);
   ~CFindFile();

   BOOL Initialize(LPCSTR lpcszSearchName = NULL);
   UINT GetFileName(char *szFileName, UINT uMaxChars);
   UINT GetFileName(char *szFileName, UINT uMaxChars, DWORD *pdwAttribute);
   UINT GetFileName(char *szFileName, UINT uMaxChars, WIN32_FIND_DATA *pFindData);
};


//***********************************************************************************************
// Inline member functions
//

//===============================================================================================
// FUNCTION: CFindFile
// PURPOSE:  Default Constructor.
//

inline CFindFile::CFindFile()
{
   MEMBERASSERT();
   m_hFindFile = INVALID_HANDLE_VALUE;
   memset(&m_FindData, '\0', sizeof(m_FindData));
}

//===============================================================================================
// FUNCTION: CFindFile
// PURPOSE:  Constructor.
//

inline CFindFile::CFindFile(LPCSTR lpcszSearchName)
{
   MEMBERASSERT();
   m_hFindFile = INVALID_HANDLE_VALUE;
   Initialize(lpcszSearchName);
}

//===============================================================================================
// FUNCTION: ~CFindFile
// PURPOSE:  Destructor.
//

inline CFindFile::~CFindFile()
{
   MEMBERASSERT();
   if (m_hFindFile != INVALID_HANDLE_VALUE)
      FindClose(m_hFindFile);
   m_hFindFile = INVALID_HANDLE_VALUE;
}

//===============================================================================================
// FUNCTION: Initialize
// PURPOSE:  Initializes the object and searches for a file if lpszSearchName!=NULL.
//

inline BOOL CFindFile::Initialize(LPCSTR lpcszSearchName)
{
   MEMBERASSERT();
   if (m_hFindFile != INVALID_HANDLE_VALUE)
      FindClose(m_hFindFile);
   m_hFindFile = INVALID_HANDLE_VALUE;
   
   memset(&m_FindData, '\0', sizeof(m_FindData));
   if (!lpcszSearchName)
      return FALSE;
      
   LPSZASSERT(lpcszSearchName);
   
   m_hFindFile = ::FindFirstFile(lpcszSearchName, &m_FindData);
   return (m_hFindFile != INVALID_HANDLE_VALUE);
}

//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  Returns the current file name and looks for another.
// RETURNS:  Length of file name returned - 0 if unsuccessful.
//
inline UINT CFindFile::GetFileName(char *szFileName, UINT uMaxChars)
{
   MEMBERASSERT();
   ARRAYASSERT(szFileName, uMaxChars);
   if (m_hFindFile==INVALID_HANDLE_VALUE)
      return 0;
      
   strncpy(szFileName, m_FindData.cFileName, uMaxChars-1);
   szFileName[uMaxChars-1] = '\0';
      
   if (!FindNextFile(m_hFindFile, &m_FindData))
   {
      FindClose(m_hFindFile);
      m_hFindFile = INVALID_HANDLE_VALUE;
   }
   return strlen(szFileName);
}

//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  Returns the current file name and looks for another.
// RETURNS:  Length of file name returned - 0 if unsuccessful.
//
inline UINT CFindFile::GetFileName(char *szFileName, UINT uMaxChars, DWORD *pdwAttribute)
{
   MEMBERASSERT();
   WPTRASSERT(pdwAttribute);
   *pdwAttribute = m_FindData.dwFileAttributes;
   return GetFileName(szFileName, uMaxChars);
}

//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  Returns the current file name and looks for another.
// RETURNS:  Length of file name returned - 0 if unsuccessful.
//
inline UINT CFindFile::GetFileName(char *szFileName, UINT uMaxChars, WIN32_FIND_DATA *pFindData)
{
   MEMBERASSERT();
   WPTRASSERT(pFindData);
   *pFindData = m_FindData;
   return GetFileName(szFileName, uMaxChars);
}

#endif  // INC_FINDFILE_HPP


