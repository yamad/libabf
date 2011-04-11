//***********************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  FilePath.H
// PURPOSE: CFilePath class.
// AUTHOR:  JFF July'96
//          BHI Nov  97

#ifndef INC_FILEPATH_H
#define INC_FILEPATH_H

#include <stdlib.h>
#include <tchar.h>

#include "..\common\axodebug.h"

class CFilePath
{
private:
   char              m_szPath[_MAX_PATH];
   char              m_szDrive[_MAX_DRIVE];
   char              m_szDir[_MAX_DIR];
   char              m_szFName[_MAX_FNAME];
   char              m_szExt[_MAX_EXT];

private:
   LPCSTR            RebuildPath( void );

public:
   CFilePath( LPCSTR pcszPath=NULL );
   CFilePath( HINSTANCE hInstance, UINT uStringID=0 );
   CFilePath( const CFilePath& );
   ~CFilePath();

   const CFilePath&  operator=( const CFilePath& );
   operator LPCSTR() const;

   LPCSTR            GetPath( void ) const;
   LPCSTR            SetPath( LPCSTR pcszPath );

   LPCSTR            GetDrive( void ) const;
   LPCSTR            SetDrive( char cDrive );

   LPCSTR            GetDir( void ) const;
   LPCSTR            SetDir( LPCSTR pcszDir );

   LPCSTR            GetFileName( void ) const;
   LPCSTR            SetFileName( LPCSTR pcszFileName );

   LPCSTR            GetExtension( void ) const;
   LPCSTR            SetExtension( LPCSTR pcszExtension );

   void              GetNameAndExtension( LPSTR pszNameAndExtension, UINT uLength ) const;
   LPCSTR            SetNameAndExtension( LPCSTR pcszNameAndExtension );

   void              GetDriveAndDirectory( LPSTR pszDriveAndDirectory, UINT uLength ) const;
   LPCSTR            SetDriveAndDirectory( LPCSTR pszDriveAndDirectory );

   LPCSTR            ConvertToFilter( char* pszFilters[], int nCompare = 1 );
};

//==============================================================
// FUNCTION: GetPath
// PURPOSE:  returns the path string
//
inline LPCSTR CFilePath::GetPath() const
{
   MEMBERASSERT();
   return m_szPath;
}

//==============================================================
// FUNCTION: GetDrive
// PURPOSE:  returns the drive string
//
inline LPCSTR CFilePath::GetDrive() const
{
   MEMBERASSERT();
   return m_szDrive;
}

//==============================================================
// FUNCTION: GetDir
// PURPOSE:  returns the directory string
//
inline LPCSTR CFilePath::GetDir() const
{
   MEMBERASSERT();
   return m_szDir;
}

//==============================================================
// FUNCTION: GetFileName
// PURPOSE:  returns the file name string
//
inline LPCSTR CFilePath::GetFileName() const
{
   MEMBERASSERT();
   return m_szFName;
}

//==============================================================
// FUNCTION: GetExtension
// PURPOSE:  returns the extension string
//
inline LPCSTR CFilePath::GetExtension() const
{
   MEMBERASSERT();
   return m_szExt;
}

#endif //INC_FILEPATH_H
