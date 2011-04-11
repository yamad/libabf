//***********************************************************************************************
//
//    Copyright (c) 1994 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  VERSION.CPP
// PURPOSE: Contains member functions for the CVersion class.
// AUTHOR:  BHI  Mar 1994
//

#include "wincpp.hpp"
#include "..\Common\AxoDebug.h"
#include "..\Common\Version.hpp"

#pragma comment(lib, "Version.lib")

struct VERTRANS       // a language/character-set id pair
{
   WORD wLang;
   WORD wCharSet;
};

//===============================================================================================
// FUNCTION: Initialize
// PURPOSE:  Sets the object to a known initial state, releasing any allocated memory.
//
VOID CVersion::Initialize()
{
   MEMBERASSERT();
   if (m_lpVerBuffer)
   {
      delete[] m_lpVerBuffer;
      m_lpVerBuffer = NULL;
   }
   m_lpvsffi = NULL;
   m_uTrans = 0;
   m_atrans = NULL;
}


//***********************************************************************************************
// Constructors
//

//===============================================================================================
// FUNCTION: CVersion - default constructor
// PURPOSE:  Initializes the object.
//
CVersion::CVersion()
{
   MEMBERASSERT();
   m_lpVerBuffer = NULL;
   Initialize();
}


//===============================================================================================
// FUNCTION: CVersion(HINSTANCE) - alternate constructor
// PURPOSE:  Initializes the object, and reads the version information for the given instance.
//
CVersion::CVersion(HINSTANCE hInstance)
{
   MEMBERASSERT();
   m_lpVerBuffer = NULL;
   Initialize();
   char szFileName[_MAX_PATH];
   if (GetModuleFileName(hInstance, szFileName, _MAX_PATH))
      ReadVersionInfo(szFileName);
}


//===============================================================================================
// FUNCTION: CVersion(LPSTR) - alternate constructor
// PURPOSE:  Initializes the object, and reads the version information for the given filename.
//
CVersion::CVersion(LPCSTR lpszFileName)
{
   MEMBERASSERT();
   m_lpVerBuffer = NULL;
   ReadVersionInfo(lpszFileName);
}


//***********************************************************************************************
// Destructor
//

//===============================================================================================
// FUNCTION: ~CVersion() - destructor
// PURPOSE:  Cleans up the object, deleting any allocated memory.
//
CVersion::~CVersion()
{
   MEMBERASSERT();
   Initialize();
}

//===============================================================================================
// FUNCTION: IsValid
// PURPOSE:  Checks that the CVersion object is valid and ready for use.
//
BOOL CVersion::IsValid() const
{
   MEMBERASSERT();
   return (m_lpvsffi!=NULL);
}

//===============================================================================================
// FUNCTION: ReadVersionInfo
// PURPOSE:  Reads the version information from the given filename.
// RETURNS:  TRUE if successful.
//
BOOL CVersion::ReadVersionInfo(LPCSTR lpszFileName)
{
   MEMBERASSERT();
   if (IsBadReadPtr(lpszFileName, 1))
   {
      TRACE("CVersion::ReadVersionInfo - Bad filename.\n");
      return FALSE;
   }

   // if a resource already loaded, release it
   Initialize();

   // find out how big the version resource is
   DWORD dwHandle; 
   DWORD dwSize;
   dwSize = GetFileVersionInfoSize((LPTSTR)lpszFileName, &dwHandle);
   if (dwSize == 0)
   {
      TRACE("CVersion::ReadVersionInfo - version resource not found.\n");
      return FALSE;        
   }

   // allocate a buffer big enough to hold the resource
   m_lpVerBuffer = new char[UINT(dwSize)];
   if (!m_lpVerBuffer)
   {
      TRACE("CVersion::ReadVersionInfo - memory allocation failed.\n");
      return FALSE;
   }

   // read the version resource data
   if (!GetFileVersionInfo((LPTSTR)lpszFileName, dwHandle, dwSize, m_lpVerBuffer))
   {
      TRACE("CVersion::ReadVersionInfo - read failed.\n");
      return FALSE;
   }               

   // Locate fixed-length file information
   UINT cbData;
   char szRootBlockKey[] = "\\";
   VerQueryValue(m_lpVerBuffer, szRootBlockKey, (LPVOID *)&m_lpvsffi, &cbData);

   // Locate translations and determine how many there are
   char szTranslationKey[] = "\\VarFileInfo\\Translation";
   VerQueryValue(m_lpVerBuffer, szTranslationKey, (LPVOID *)&m_atrans, &cbData);
   m_uTrans = cbData/sizeof(VERTRANS);

   return TRUE;
}

//===============================================================================================
// FUNCTION: GetTranslation
// PURPOSE:  Reads the nth translation value into the return buffer.
// RETURNS:  TRUE if successful.
//
BOOL CVersion::GetTranslation(UINT uTrans, VERTRANS *pvt) const
{
   MEMBERASSERT();
   if (IsBadWritePtr(pvt, sizeof(VERTRANS)))
   {
      TRACE("CVersion::GetTranslation - Bad output buffer.\n");
      ASSERT(0);
      return FALSE;
   }

   if (!m_atrans)
   {
      TRACE("CVersion::GetTranslation - No translation available\n");
      ASSERT(0);
      return FALSE;
   }

   if (uTrans >= m_uTrans)
   {
      TRACE("CVersion::GetTranslation - Bad index.\n");
      ASSERT(0);
      return FALSE;
   }

   *pvt = m_atrans[uTrans];
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetString
// PURPOSE:  Reads the version string corresponding to the given key and translation values.
// RETURNS:  Length of string found - 0 if unsuccessful.
//
UINT CVersion::GetString(LPCSTR lpcszKey, VERTRANS &rvt, LPSTR lpszValue, UINT cbValue) const
{
   MEMBERASSERT();
   if ((cbValue == 0) || IsBadWritePtr(lpszValue, cbValue))
   {
      TRACE("CVersion::GetString - Bad output buffer.\n");
      ASSERT(0);
      return 0;
   }
   lpszValue[0] = '\0';

   if (!m_lpVerBuffer)
   {
      TRACE("CVersion::GetString - No resource available.\n");
      ASSERT(0);
      return 0;
   }

   // generate name of string value
   const char szStringKeyFmt[] = "\\StringFileInfo\\%04x%04x\\%s";
   char szKey[128];
   _snprintf(szKey, sizeof(szKey), szStringKeyFmt, rvt.wLang, rvt.wCharSet, lpcszKey);

   // locate data in version information
   LPVOID lpData;
   UINT   cbData;
   if (!VerQueryValue(m_lpVerBuffer, szKey, &lpData, &cbData))
   {
#ifdef _DEBUG   
      char buf[120];
      sprintf(buf, "CVersion::GetString - %s not found for translation\n", lpcszKey);
      TRACE(buf);
#endif
      return 0;
   }

   // copy data into output buffer, ensuring that there is a terminating null.
   lpszValue[--cbValue] = '\0';
   UINT cbCopy = min(cbData, cbValue);
   memcpy(lpszValue, lpData, cbCopy);
   return strlen(lpszValue);
}

//===============================================================================================
// FUNCTION: GetString
// PURPOSE:  Reads the version string corresponding to the given key, default translation value.
// RETURNS:  Length of string found - 0 if unsuccessful.
//
UINT CVersion::GetString(LPCSTR lpcszKey, LPSTR lpszValue, UINT cbValue) const
{
   MEMBERASSERT();
   if (!m_uTrans || !m_atrans)
   {
      TRACE("CVersion::GetString - no translations available.\n");
      return 0;
   }
   return GetString(lpcszKey, m_atrans[0], lpszValue, cbValue);
}

//===============================================================================================
// FUNCTION: GetFileVersion
// PURPOSE:  Gets the file version as Most Significant DWORD and Least Significant DWORD.
//
//  [     MS     ][     LS     ]
//  [ MSH ][ MSL ][ LSH ][ LSL ]
//  Version string = "MSH.MSL.LSH.LSL"
//
BOOL CVersion::GetFileVersion(DWORD *pdwMS, DWORD *pdwLS) const
{
   MEMBERASSERT();
   
   if (!m_lpvsffi)
      return FALSE;
      
   WPTRASSERT(m_lpvsffi);
   if (pdwMS)
   {
      WPTRASSERT(pdwMS);
      *pdwMS = m_lpvsffi->dwFileVersionMS;
   }
   if (pdwLS)
   {
      WPTRASSERT(pdwLS);
      *pdwLS = m_lpvsffi->dwFileVersionLS;
   }
   return TRUE;
}


//===============================================================================================
// FUNCTION: GetFileVersion
// PURPOSE:  Gets the file version as a string.
// RETURNS:  Number of characters in version string.
//           lpsBuffer is filled on return.
//
UINT CVersion::GetFileVersion(LPSTR lpsBuffer, UINT uBufSize) const
{
   MEMBERASSERT();
   return GetString("FileVersion", lpsBuffer, uBufSize);
#if 0   
   // Check that we have read a version resource.
   if (!m_lpvsffi)
      return 0;
      
   WPTRASSERT(m_lpvsffi);
   DWORD dwMS = m_lpvsffi->dwFileVersionMS;
   DWORD dwLS = m_lpvsffi->dwFileVersionLS;
      
   // During development, check full version string because of changing interface.
   // The least significant word is only included if the module carries the VS_FF_PRERELEASE flag.
   char szVersion[30];
   if (m_lpvsffi->dwFileFlags & VS_FF_PRERELEASE)
      sprintf(szVersion, "%d.%d.%d.%02d", HIWORD(dwMS), LOWORD(dwMS), HIWORD(dwLS), LOWORD(dwLS));
   else
      sprintf(szVersion, "%d.%d.%d", HIWORD(dwMS), LOWORD(dwMS), HIWORD(dwLS));
      
   strncpy(lpsBuffer, szVersion, uBufSize);
   lpsBuffer[uBufSize-1] = '\0';
   return strlen(lpsBuffer);
#endif   
}
 
//===============================================================================================
// FUNCTION: GetProductVersion
// PURPOSE:  Gets the file version as Most Significant DWORD and Least Significant DWORD.
//           see GetFileVersion
//
BOOL CVersion::GetProductVersion(DWORD *pdwMS, DWORD *pdwLS) const
{
   MEMBERASSERT();
   
   if (!m_lpvsffi)
      return FALSE;
   WPTRASSERT(m_lpvsffi);

   if (pdwMS)
   {
      WPTRASSERT(pdwMS);
      *pdwMS = m_lpvsffi->dwProductVersionMS;
   }
   if (pdwLS)
   {
      WPTRASSERT(pdwLS);
      *pdwLS = m_lpvsffi->dwProductVersionLS;
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetProductVersion
// PURPOSE:  Gets the product version as a string.
// RETURNS:  Number of characters in version string.
//           lpsBuffer is filled on return.
//
UINT CVersion::GetProductVersion(LPSTR lpsBuffer, UINT uBufSize) const
{
   MEMBERASSERT();
   return GetString("ProductVersion", lpsBuffer, uBufSize);
}

//===============================================================================================
// FUNCTION: GetFileFlagsMask
// PURPOSE:  Gets the mask that specifies valid file flags.
//
DWORD CVersion::GetFileFlagsMask() const
{
   MEMBERASSERT();
   WPTRASSERT(m_lpvsffi);
   return m_lpvsffi ? m_lpvsffi->dwFileFlagsMask : 0;
}

//===============================================================================================
// FUNCTION: GetFileFlags
// PURPOSE:  Gets the file flags.
//
DWORD CVersion::GetFileFlags() const
{
   MEMBERASSERT();
   WPTRASSERT(m_lpvsffi);
   return m_lpvsffi ? m_lpvsffi->dwFileFlags : 0;
}

//===============================================================================================
// FUNCTION: GetFileOS
// PURPOSE:  Gets the ID for the operating system that the file was compiled for.
//
DWORD CVersion::GetFileOS() const
{
   MEMBERASSERT();
   WPTRASSERT(m_lpvsffi);
   return m_lpvsffi ? m_lpvsffi->dwFileOS : 0;
}

//===============================================================================================
// FUNCTION: GetFileType
// PURPOSE:  Gets the type ID for the file.
//
DWORD CVersion::GetFileType() const
{
   MEMBERASSERT();
   WPTRASSERT(m_lpvsffi);
   return m_lpvsffi ? m_lpvsffi->dwFileType : 0;
}

//===============================================================================================
// FUNCTION: GetFileSubtype
// PURPOSE:  Gets the subtype ID for the file.
//
DWORD CVersion::GetFileSubtype() const
{
   MEMBERASSERT();
   WPTRASSERT(m_lpvsffi);
   return m_lpvsffi ? m_lpvsffi->dwFileSubtype : 0;
}

//===============================================================================================
// FUNCTION: GetFileDate
// PURPOSE:  Gets the date of the file.
//
BOOL CVersion::GetFileDate(DWORD *pdwMS, DWORD *pdwLS) const
{
   MEMBERASSERT();
   
   if (!m_lpvsffi)
      return FALSE;
   WPTRASSERT(m_lpvsffi);
   if (pdwMS)
   {
      WPTRASSERT(pdwMS);
      *pdwMS = m_lpvsffi->dwFileDateMS;
   }
   if (pdwLS)
   {
      WPTRASSERT(pdwLS);
      *pdwLS = m_lpvsffi->dwFileDateLS;
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetNumTranslation
// PURPOSE:  Gets the number of translations supported by the file.
//
UINT CVersion::GetNumTranslation() const
{
   MEMBERASSERT();
   return m_uTrans;
}

