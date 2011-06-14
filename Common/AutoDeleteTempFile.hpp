//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  AutoDeleteTempFile.HPP
// PURPOSE: Define the class AutoDeleteTempFile an auto pointer for temp files.
// AUTHOR:  BHI Dec 1997
//
#ifndef INC_AUTODELETETEMPFILE_HPP
#define INC_AUTODELETETEMPFILE_HPP

//============================================================================
// CLASS:   CAutoDeleteTempFile
// PURPOSE: Destroys a file when the object goes out of scope.
//
class CAutoDeleteTempFile
{
private:
   char m_szFilename[ _MAX_PATH ];

public:
   CAutoDeleteTempFile();
   CAutoDeleteTempFile(LPCSTR pszPath);
   ~CAutoDeleteTempFile();
   operator LPCSTR () const;
};

//============================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CAutoDeleteTempFile::CAutoDeleteTempFile()
{
   MEMBERASSERT();
   m_szFilename[0] = '\0';
   AXU_GetTempFileName( "Axo", 0, m_szFilename);
}

//============================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CAutoDeleteTempFile::CAutoDeleteTempFile(LPCSTR pszPath)
{
   MEMBERASSERT();
   AXU_strncpyz(m_szFilename, pszPath, sizeof(m_szFilename));
   HANDLE hFile = CreateFile( m_szFilename, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
   CloseHandle(hFile);
}

//============================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
inline CAutoDeleteTempFile::~CAutoDeleteTempFile()
{
   MEMBERASSERT();
   if (m_szFilename[0])
   {
      VERIFY(DeleteFile(m_szFilename));
      m_szFilename[0] = '\0';
   }
}

//============================================================================
// FUNCTION: Cast operator
// PURPOSE:  Returns the filename of the temp file.
//
inline CAutoDeleteTempFile::operator LPCSTR() const
{
   MEMBERASSERT();
   return m_szFilename;
}


//============================================================================
// CLASS:   CAutoDeleteDirectory
// PURPOSE: Destroys a file when the object goes out of scope.
//
class CAutoDeleteDirectory
{
private:
   char m_szDirectory[ _MAX_PATH ];

public:
   CAutoDeleteDirectory();
   CAutoDeleteDirectory(LPCSTR pszPath);
   ~CAutoDeleteDirectory();
   operator LPCSTR () const;
};

//============================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CAutoDeleteDirectory::CAutoDeleteDirectory()
{
   MEMBERASSERT();
   m_szDirectory[0] = '\0';
   AXU_GetTempFileName( "Axo", 0, m_szDirectory);
   DeleteFile(m_szDirectory);
   VERIFY(CreateDirectory(m_szDirectory, NULL));
}

//============================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CAutoDeleteDirectory::CAutoDeleteDirectory(LPCSTR pszPath)
{
   MEMBERASSERT();
   AXU_strncpyz(m_szDirectory, pszPath, sizeof(m_szDirectory));
   VERIFY(CreateDirectory(m_szDirectory, NULL));
}

//============================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
inline CAutoDeleteDirectory::~CAutoDeleteDirectory()
{
   MEMBERASSERT();
   if (m_szDirectory[0])
   {
      VERIFY(RemoveDirectory(m_szDirectory));
      m_szDirectory[0] = '\0';
   }
}

//============================================================================
// FUNCTION: Cast operator
// PURPOSE:  Returns the filename of the temp file.
//
inline CAutoDeleteDirectory::operator LPCSTR() const
{
   MEMBERASSERT();
   return m_szDirectory;
}


#endif          // INC_AUTODELETETEMPFILE_HPP
