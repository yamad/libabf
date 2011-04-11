//***********************************************************************************************
//
//    Copyright (c) 1996-1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  FilePath.cpp
// PURPOSE: CFilePath class.
// AUTHOR:  JFF July 96
//          BHI Nov  97

#include "wincpp.hpp"
#include "FilePath.h"

#include <stdlib.h>     // for _makepath, _splitpath
#include <string.h>     // for strncpy, _strupr

//==============================================================================================
// FUNCTION: strncpyz
// PURPOSE:  Does a strncpy but guarantees that a terminating zero is placed on the 
//           destination string.
// RETURNS:  The destination buffer.
//
#define STRNCPYZ(d, s) strncpyz(d, s, sizeof(d))
static LPSTR strncpyz(LPSTR pszDest, LPCSTR pszSrce, UINT uBufSize)
{
   ASSERT(uBufSize > 0);
   ARRAYASSERT(pszDest, uBufSize);
   LPSZASSERT(pszSrce);

   strncpy(pszDest, pszSrce, uBufSize-1);
   pszDest[uBufSize-1] = '\0';
   return pszDest;
}

//***********************************************************************************************
// FUNCTION: RebuildPath -- private method
// PURPOSE:  Rebuilds the path from its components.
//
LPCSTR CFilePath::RebuildPath( )
{
   MEMBERASSERT();
   _makepath( m_szPath, m_szDrive, m_szDir, m_szFName, m_szExt );
   _splitpath( m_szPath, m_szDrive, m_szDir, m_szFName, m_szExt );
   return m_szPath;
}

//***********************************************************************************************
// FUNCTION: SetPath -- public method
// PURPOSE:  Re-initializes the object with a new path.
//
LPCSTR CFilePath::SetPath( LPCSTR pcszPath )
{
   MEMBERASSERT();
   _splitpath( pcszPath ? pcszPath : "", m_szDrive, m_szDir, m_szFName, m_szExt );
   return RebuildPath();
}

//***********************************************************************************************
// FUNCTION: Copy Constructor
// PURPOSE:  Initializes object from a full path name.
//
CFilePath::CFilePath( LPCSTR pcszPath )
{
   MEMBERASSERT();
   SetPath( pcszPath );
}

//***********************************************************************************************
// FUNCTION: Copy Constructor
// PURPOSE:  Initializes object from a string resource or the EXE/DLL name.
//
CFilePath::CFilePath( HINSTANCE hInstance, UINT uStringID )
{
   MEMBERASSERT();
   if (uStringID)
      VERIFY(LoadString(hInstance, uStringID, m_szPath, sizeof(m_szPath)));
   else
      VERIFY(GetModuleFileName(hInstance, m_szPath, sizeof(m_szPath)));
   SetPath( m_szPath );
}

//***********************************************************************************************
// FUNCTION: Copy Constructor
// PURPOSE:  Initializes object from an other CFilePath object
//
CFilePath::CFilePath( const CFilePath& srcFilePath )
{
   MEMBERASSERT();
   SetPath( srcFilePath.GetPath() );
}

//***********************************************************************************************
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CFilePath::~CFilePath()
{
   MEMBERASSERT();
}

//***********************************************************************************************
// FUNCTION: Assignment Operator
// PURPOSE:  Initializes object from an other CFilePath object
//
const CFilePath& CFilePath::operator=( const CFilePath& srcFilePath )
{
   MEMBERASSERT();

   SetPath(srcFilePath.GetPath());
   return *this;
}

//***********************************************************************************************
// FUNCTION: Cast Operator
// PURPOSE:  Returns the full path as a LPCSTR pointer.
//
CFilePath::operator LPCSTR() const
{
   MEMBERASSERT();
   return GetPath();
}

//***********************************************************************************************
// FUNCTION: SetDrive -- public method
// PURPOSE:  Sets the drive letter of the path.
//
LPCSTR CFilePath::SetDrive( char cDrive )
{
   MEMBERASSERT();

   // convert the char into a string
   m_szDrive[0] = cDrive;
   m_szDrive[1] = '\0';

   return RebuildPath();
}

//***********************************************************************************************
// FUNCTION: SetDir -- public method
// PURPOSE:  Sets the directory component of the path.
//
LPCSTR CFilePath::SetDir( LPCSTR pcszDir )
{
   MEMBERASSERT();
   STRNCPYZ( m_szDir, pcszDir );
   return RebuildPath();
}

//***********************************************************************************************
// FUNCTION: SetFileName -- public method
// PURPOSE:  Sets the filename component of the path.
//
LPCSTR CFilePath::SetFileName( LPCSTR pcszFileName )
{
   MEMBERASSERT();
   STRNCPYZ( m_szFName, pcszFileName );
   return RebuildPath();
}

//***********************************************************************************************
// FUNCTION: SetExtension -- public method
// PURPOSE:  Sets the extension component of the path.
//
LPCSTR CFilePath::SetExtension( LPCSTR pcszExtension )
{
   MEMBERASSERT();
   STRNCPYZ( m_szExt, pcszExtension );
   return RebuildPath();
}

//***********************************************************************************************
// FUNCTION: SetNameAndExtension -- public method
// PURPOSE:  Sets both the name and the extension components of the path.
//
LPCSTR CFilePath::SetNameAndExtension( LPCSTR pcszNameAndExtension )
{
   MEMBERASSERT();
   LPSZASSERT(pcszNameAndExtension);

   _splitpath( pcszNameAndExtension, NULL, NULL, m_szFName, m_szExt );
   return RebuildPath();
}

//***********************************************************************************************
// FUNCTION: GetNameAndExtension -- public method
// PURPOSE:  Sets both the name and the extension components of the path.
//
void CFilePath::GetNameAndExtension( LPSTR pszNameAndExtension, UINT uLength ) const
{
   MEMBERASSERT();
   ARRAYASSERT(pszNameAndExtension, uLength);

   char szFileName[_MAX_PATH];
   _makepath( szFileName, NULL, NULL, m_szFName, m_szExt );
   strncpyz( pszNameAndExtension, szFileName, uLength );
}

//***********************************************************************************************
// FUNCTION: SetDriveAndDirectory -- public method
// PURPOSE:  Sets both the name and the extension components of the path.
//
LPCSTR CFilePath::SetDriveAndDirectory( LPCSTR pcszDriveAndDirectory )
{
   MEMBERASSERT();
   LPSZASSERT(pcszDriveAndDirectory);
   _makepath( m_szPath, NULL, pcszDriveAndDirectory, m_szFName, m_szExt );
   _splitpath( m_szPath, m_szDrive, m_szDir, m_szFName, m_szExt );
   return m_szPath;
}

//***********************************************************************************************
// FUNCTION: GetDriveAndDirectory -- public method
// PURPOSE:  Sets both the name and the extension components of the path.
//
void CFilePath::GetDriveAndDirectory( LPSTR pszDriveAndDirectory, UINT uLength ) const
{
   MEMBERASSERT();

   char szFileName[_MAX_PATH];
   _makepath( szFileName, m_szDrive, m_szDir, NULL, NULL );
   strncpyz( pszDriveAndDirectory, szFileName, uLength );
}

//***********************************************************************************************
// FUNCTION: ConvertToFilter -- public method
// PURPOSE:  
//
LPCSTR CFilePath::ConvertToFilter( char* pszFilters[], int nCompare )
{
   MEMBERASSERT();

   // set the FName to '*'
   strcpy(m_szFName, "*");

   // find the right filter to set
   for ( int n=0; pszFilters[n]; n++ )
   {
      if (strnicmp(m_szExt, pszFilters[n], nCompare)==0)
      {
         STRNCPYZ( m_szExt, pszFilters[n] );
         break;
      }
   }

   // rebuild the path
   return RebuildPath();
}

