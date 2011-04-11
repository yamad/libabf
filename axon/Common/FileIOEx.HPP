//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  FileIOEx.HPP
// PURPOSE: Contains the class definition for the CFileIOEx class, a simple wrapper
//          around the WIN32 file I/O API.
// 

#ifndef INC_FileIOEx_HPP
#define INC_FileIOEx_HPP

class CFileIOEx
{
private:    // Member variables and constants.
   char         m_szFilename[_MAX_PATH]; // The complete filename of the file
   HANDLE       m_hFileHandle;           // The DOS file handle for data file
   DWORD        m_dwLastError;           // Error number for last error.
   OVERLAPPED  *m_pOverlapped;           // Pointer to the overlapped structure

private:    // Prevent default copy constructor and operator=()
   CFileIOEx(const CFileIOEx &FI);
   const CFileIOEx &operator=(const CFileIOEx &FI);

private:    // Internal functions.
   BOOL SeekFailure(DWORD dwOffset);
   BOOL WaitForCompletion();
   
public:   
   CFileIOEx();
   CFileIOEx(HANDLE hFile);
   ~CFileIOEx();
   
   BOOL  Create(LPCSTR szFilename, BOOL bReadOnly, DWORD dwAttributes=FILE_ATTRIBUTE_NORMAL);
   BOOL  CreateEx(LPCSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                  DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
   BOOL  Close();
   BOOL  IsOpen() const;

   BOOL  Write(const void *pvBuffer, DWORD dwSizeInBytes, DWORD *pdwBytesWritten=NULL);
   BOOL  Read(void *pvBuffer, DWORD dwSizeInBytes, DWORD *pdwBytesRead=NULL);
   BOOL  Seek(LONGLONG lOffset, UINT uFlag, LONGLONG *plNewOffset=NULL);
   BOOL  GetCurrentPosition(LONGLONG *plCurrentPosition);
   BOOL  Flush();

   BOOL     SetEndOfFile();
   LONGLONG GetFileSize();
   BOOL     GetFileTime( LPFILETIME pCreationTime, LPFILETIME pLastAccessTime=NULL, 
                         LPFILETIME pLastWriteTime=NULL);
   BOOL   SetFileTime( LPFILETIME pCreationTime, LPFILETIME pLastAccessTime=NULL, 
                       LPFILETIME pLastWriteTime=NULL);

   BOOL   GetFileInformation(LPBY_HANDLE_FILE_INFORMATION lpFileInformation);

   void   SetFileHandle(HANDLE hFile);
   HANDLE GetFileHandle() const;   
   LPCSTR GetFileName() const;
   HANDLE Release();   
   
   BOOL   Duplicate(CFileIOEx *pNewFile, BOOL bInheritable=TRUE);
   BOOL   SetInheritable(BOOL bInheritable=TRUE);

   BOOL   SetLastError();
   BOOL   SetLastError(DWORD nError);
   DWORD  GetLastError() const;
};


//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  Get the name of the file.
//
inline LPCSTR CFileIOEx::GetFileName() const
{
   MEMBERASSERT();
   return m_szFilename;
}

//===============================================================================================
// FUNCTION: GetFileHandle
// PURPOSE:  Returns the file handle opened in the object.
//
inline HANDLE CFileIOEx::GetFileHandle() const
{
   MEMBERASSERT();
   return m_hFileHandle;
}


//===============================================================================================
// FUNCTION: IsOpen
// PURPOSE:  Returns TRUE if an open file handle is held.
//
inline BOOL CFileIOEx::IsOpen() const
{
   MEMBERASSERT();
   return (m_hFileHandle != INVALID_HANDLE_VALUE);
}

//===============================================================================================
// FUNCTION: SetEndOfFile
// PURPOSE:  Truncates the file to the current position.
//
inline BOOL CFileIOEx::SetEndOfFile()
{
   MEMBERASSERT();
   return ::SetEndOfFile(m_hFileHandle) ? TRUE : SetLastError();
}

#endif   // INC_FileIOEx_HPP
