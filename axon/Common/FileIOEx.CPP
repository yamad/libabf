//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  FileIOEx.CPP
// PURPOSE: Contains the member functions for the CFileIOEx class.
// NOTES:
// 
// Overlapped file I/O is not available in WIN95 -- PSS ID Number: Q125717
// Windows 95 does not support overlapped operations on files, disk, pipes, or
// mail slots, but does support overlapped operations on serial and parallel
// communication ports. Non-overlapped file write operations are similar to
// overlapped file writes, because Windows 95 uses a lazy-write disk cache.
// Overlapped I/O can be implemented in a Win32-based application by creating
// multiple threads to handle I/O.
//  
// Asynchronous I/O on files, disk, pipes and mail slots is not implemented in
// Windows 95. If a Win32-based application running on Windows 95 attempts to
// perform asynchronous file I/O (such as ReadFile() with any value other than
// NULL in the lpOverlapped field) on any of these objects, the ReadFile()  or
// WriteFile fails and GetLastError() returns ERROR_INVALID_PARAMETER (87).
//  

#include "wincpp.hpp"
#include "FileIOEx.hpp"

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initialize the object
//
CFileIOEx::CFileIOEx()
{
   MEMBERASSERT();
   m_hFileHandle      = INVALID_HANDLE_VALUE;
   m_szFilename[0]    = '\0';
   m_dwLastError      = 0;
   m_pOverlapped      = NULL;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleanup the object when it is deleted.
//
CFileIOEx::~CFileIOEx()
{
   MEMBERASSERT();
   Close();
}

//===============================================================================================
// FUNCTION: Create
// PURPOSE:  Opens a file and stores the filename if successful.
//
BOOL CFileIOEx::Create(LPCSTR szFilename, BOOL bReadOnly, DWORD dwAttributes)
{
   MEMBERASSERT();
   LPSZASSERT(szFilename);
   ASSERT(m_hFileHandle == INVALID_HANDLE_VALUE);

   DWORD dwFlags    = GENERIC_READ;
   DWORD dwCreation = OPEN_EXISTING;
   if (!bReadOnly)
   {
      dwFlags   |= GENERIC_WRITE;
      dwCreation = CREATE_ALWAYS;
   }
   
   return CreateEx(szFilename, dwFlags, FILE_SHARE_READ, dwCreation, dwAttributes);
}

//===============================================================================================
// FUNCTION: CreateEx
// PURPOSE:  Opens a file and stores the filename if successful.
//
BOOL CFileIOEx::CreateEx(LPCSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                         DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes)
{
   MEMBERASSERT();
   LPSZASSERT(szFileName);
   ASSERT(m_hFileHandle == INVALID_HANDLE_VALUE);

   if (dwFlagsAndAttributes & FILE_FLAG_OVERLAPPED)
   {
#ifdef _DEBUG
      OSVERSIONINFO Info = { 0 };
      Info.dwOSVersionInfoSize = sizeof(Info); 
      VERIFY(GetVersionEx(&Info));
      ASSERT(Info.dwPlatformId == VER_PLATFORM_WIN32_NT);
#endif

      m_pOverlapped = new OVERLAPPED;
      if (!m_pOverlapped)
         return SetLastError(ERROR_NOT_ENOUGH_MEMORY);
      memset(m_pOverlapped, 0, sizeof(*m_pOverlapped));
   }

   m_hFileHandle = ::CreateFile(szFileName, dwDesiredAccess, dwShareMode, NULL, 
                                dwCreationDisposition, dwFlagsAndAttributes, NULL);
   if (m_hFileHandle == INVALID_HANDLE_VALUE)
      return SetLastError();
      
   strncpy(m_szFilename, szFileName, _MAX_PATH-1);
   m_szFilename[_MAX_PATH-1] = '\0';
   return TRUE;
}

//===============================================================================================
// FUNCTION: Copy Constructor
// PURPOSE:  Creates a CFileIOEx object from a Win32 HANDLE.
//
CFileIOEx::CFileIOEx(HANDLE hFile)
{
   MEMBERASSERT();
   m_hFileHandle   = hFile;
   m_szFilename[0] = '\0';
   m_dwLastError   = 0;
   m_pOverlapped   = NULL;
}


//===============================================================================================
// FUNCTION: SetFileHandle
// PURPOSE:  Creates a CFileIOEx object from a Win32 HANDLE.
//
void CFileIOEx::SetFileHandle(HANDLE hFile)
{
   MEMBERASSERT();
   ASSERT(m_hFileHandle == INVALID_HANDLE_VALUE);
   m_hFileHandle   = hFile;
   m_szFilename[0] = '\0';
   m_dwLastError   = 0;
   m_pOverlapped   = NULL;
}


//===============================================================================================
// FUNCTION: WaitForCompletion
// PURPOSE:  Wait until the current overlapped operation has completed.
//
BOOL CFileIOEx::WaitForCompletion()
{
   MEMBERASSERT();
   WPTRASSERT(m_pOverlapped);
   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);

   // wait for the operation to complete before continuing.
   DWORD dwBytesTransferred = 0;
   if (!GetOverlappedResult( m_hFileHandle, m_pOverlapped, &dwBytesTransferred, TRUE))
      return SetLastError();

   LARGE_INTEGER Offset;
   Offset.LowPart  = m_pOverlapped->Offset;
   Offset.HighPart = m_pOverlapped->OffsetHigh;
   Offset.QuadPart += dwBytesTransferred;
   m_pOverlapped->Offset     = Offset.LowPart;
   m_pOverlapped->OffsetHigh = Offset.HighPart;
   return TRUE;
}

//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Write a buffer of data to the file.
//
BOOL CFileIOEx::Write(const void *pvBuffer, DWORD dwSizeInBytes, DWORD *pdwBytesWritten)
{
   MEMBERASSERT();
   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);

   // Wait until done if overlapped I/O
   if (m_pOverlapped && !WaitForCompletion())
      return FALSE;

   DWORD dwBytesWritten = 0;
   BOOL bRval = ::WriteFile(m_hFileHandle, pvBuffer, dwSizeInBytes, &dwBytesWritten, m_pOverlapped);

   if (!m_pOverlapped)
   {
      if (pdwBytesWritten)
         *pdwBytesWritten = dwBytesWritten;
   }
   else if (!bRval && (::GetLastError() == ERROR_IO_PENDING))
      return TRUE;

   return bRval ? TRUE : SetLastError();
}


//===============================================================================================
// FUNCTION: Flush
// PURPOSE:  Flush the current file to disk.
//
BOOL CFileIOEx::Flush()
{
   MEMBERASSERT();
   // Wait until done if overlapped I/O
   if (m_pOverlapped && !WaitForCompletion())
      return FALSE;

   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);
   return ::FlushFileBuffers(m_hFileHandle) ? TRUE : SetLastError();
}


//===============================================================================================
// PROCEDURE: Read
// PURPOSE:   Reads a block and returnd FALSE on ERROR.
//
BOOL CFileIOEx::Read(LPVOID lpBuf, DWORD dwBytesToRead, DWORD *pdwBytesRead)
{
   MEMBERASSERT();
   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);

   // Wait until done if overlapped I/O
   if (m_pOverlapped && !WaitForCompletion())
      return FALSE;

   DWORD dwBytesRead = 0;
   BOOL bRval = ::ReadFile(m_hFileHandle, lpBuf, dwBytesToRead, &dwBytesRead, m_pOverlapped);

   if (!m_pOverlapped)
   {
      if (pdwBytesRead)
         *pdwBytesRead = dwBytesRead;
      if (bRval && (dwBytesRead!=dwBytesToRead))
         return SetLastError(ERROR_HANDLE_EOF);
   }
   else if (!bRval && (::GetLastError() == ERROR_IO_PENDING))
      return TRUE;

   return bRval ? TRUE : SetLastError();
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Closes a previously opened file.
//
BOOL CFileIOEx::Close()
{
   MEMBERASSERT();
   if (m_hFileHandle != INVALID_HANDLE_VALUE)
   {
      // Wait until done if overlapped I/O
      if (m_pOverlapped && !WaitForCompletion())
         return FALSE;

      delete m_pOverlapped;
      m_pOverlapped = NULL;

      if (!::CloseHandle(m_hFileHandle))
         return SetLastError();

      m_hFileHandle = INVALID_HANDLE_VALUE;
   }
   m_szFilename[0] = '\0';
   return TRUE;
}
      
//===============================================================================================
// FUNCTION: Release
// PURPOSE:  Releases the file handle held in the object.
//
HANDLE CFileIOEx::Release()
{
   MEMBERASSERT();
   HANDLE hRval    = m_hFileHandle;
   m_hFileHandle   = INVALID_HANDLE_VALUE;
   m_szFilename[0] = '\0';
   return hRval;
}


//===============================================================================================
// FUNCTION: SeekFailure
// PURPOSE:  Returns TRUE if an open file handle is held.
//
BOOL CFileIOEx::SeekFailure(DWORD dwOffset)
{
   MEMBERASSERT();
   if (dwOffset == 0xFFFFFFFF)
   { 
      DWORD dwError = GetLastError();
      if (dwError != NO_ERROR )
      {
         SetLastError(dwError);
         return TRUE;
      }
   }
   return FALSE;
}

//===============================================================================================
// FUNCTION: Seek
// PURPOSE:  Change the current position of the file pointer.
//
BOOL CFileIOEx::Seek(LONGLONG lOffset, UINT uFlag, LONGLONG *plNewOffset)
{
   MEMBERASSERT();
   if (m_pOverlapped)
   {
      ASSERT(uFlag==FILE_BEGIN);
      // Wait until done.
      if (!WaitForCompletion())
         return FALSE;

      LARGE_INTEGER Offset;
      Offset.QuadPart = lOffset;
      m_pOverlapped->Offset     = Offset.LowPart;
      m_pOverlapped->OffsetHigh = Offset.HighPart;
      return TRUE;
   }

   LARGE_INTEGER Offset;
   Offset.QuadPart = lOffset;
   Offset.LowPart = ::SetFilePointer(m_hFileHandle, Offset.LowPart, &Offset.HighPart, uFlag);
   if (SeekFailure(Offset.LowPart))
      return FALSE;
   if (plNewOffset)
      *plNewOffset = Offset.QuadPart;
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetCurrentPosition
// PURPOSE:  Get the current position of the file pointer.
//
BOOL CFileIOEx::GetCurrentPosition(LONGLONG *plCurrentPosition)
{
   MEMBERASSERT();
   return Seek(0, FILE_CURRENT, plCurrentPosition);
}

//===============================================================================================
// FUNCTION: GetFileSize
// PURPOSE:  Returns the size of a file in bytes.
//
LONGLONG CFileIOEx::GetFileSize()
{
   MEMBERASSERT();
   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);

   // Wait until done if overlapped I/O
   if (m_pOverlapped && !WaitForCompletion())
      return 0;

   LARGE_INTEGER FileSize;
   FileSize.QuadPart = 0;

   FileSize.LowPart = ::GetFileSize(m_hFileHandle, LPDWORD(&FileSize.HighPart));
   if (SeekFailure(FileSize.LowPart))
      return 0;

   return FileSize.QuadPart;
}
      
//===============================================================================================
// FUNCTION: GetFileTime
// PURPOSE:  Gets time values for the file.
//
BOOL CFileIOEx::GetFileTime( LPFILETIME pCreationTime, LPFILETIME pLastAccessTime, LPFILETIME pLastWriteTime)
{
   MEMBERASSERT();
   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);
   return ::GetFileTime(m_hFileHandle, pCreationTime, pLastAccessTime, pLastWriteTime);
}

//===============================================================================================
// FUNCTION: SetFileTime
// PURPOSE:  Sets time values for the file.
//
BOOL CFileIOEx::SetFileTime( LPFILETIME pCreationTime, LPFILETIME pLastAccessTime, LPFILETIME pLastWriteTime)
{
   MEMBERASSERT();
   ASSERT(m_hFileHandle != INVALID_HANDLE_VALUE);
   return ::SetFileTime(m_hFileHandle, pCreationTime, pLastAccessTime, pLastWriteTime);
}

//===============================================================================================
// FUNCTION: GetFileInformation
// PURPOSE:  .
//
BOOL CFileIOEx::GetFileInformation(LPBY_HANDLE_FILE_INFORMATION pFileInformation)
{
   MEMBERASSERT();
   return ::GetFileInformationByHandle(m_hFileHandle, pFileInformation) ? TRUE : SetLastError();
}

//===============================================================================================
// FUNCTION: Duplicate
// PURPOSE:  Makes a copy of the handle into another CFileIO object.
//
BOOL CFileIOEx::Duplicate(CFileIOEx *pNewFile, BOOL bInheritable)
{
   MEMBERASSERT();
   HANDLE hNewHandle  = NULL;
   HANDLE *pNewHandle = NULL;
   if (pNewFile)
   {
      WPTRASSERT(pNewFile);
      pNewHandle = &hNewHandle;
   }

   if (!DuplicateHandle(GetCurrentProcess(), // source process
                        m_hFileHandle,       // handle to duplicate
                        GetCurrentProcess(), // destination process
                        pNewHandle,          // new handle
                        0,                   // new access flags - ignored since DUPLICATE_SAME_ACCESS
                        bInheritable,        // it's inheritable
                        DUPLICATE_SAME_ACCESS))
      return SetLastError();

   if (pNewFile)
      pNewFile->SetFileHandle(hNewHandle);

   return TRUE;
}

//===============================================================================================
// FUNCTION: SetInheritable
// PURPOSE:  Sets the "inheritability" of the file handle.
//
BOOL CFileIOEx::SetInheritable(BOOL bInheritable)
{
   MEMBERASSERT();
   return Duplicate(NULL, bInheritable);
}

//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error value and always returns FALSE for convenience.
//
BOOL CFileIOEx::SetLastError()
{
   MEMBERASSERT();
   return SetLastError(::GetLastError());
}


//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error value and always returns FALSE for convenience.
//
BOOL CFileIOEx::SetLastError(DWORD dwError)
{
   MEMBERASSERT();
   m_dwLastError = dwError;
   TRACE1("CFileIOEx::SetLastError(%u)\n", dwError);
   SHOW_SYSTEM_ERROR(dwError);
   return FALSE;           // convenience.
}


//===============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  Gets the last error condition provoked by this object.
//
DWORD CFileIOEx::GetLastError() const
{
   MEMBERASSERT();
   return m_dwLastError;
}

#ifdef TESTBED

#define BUFFER_SIZE 1024*1024/2  // 1MB buffer.
#define REPETITIONS 200

#include /* no dependancy */ "CHighResTimer.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   static short anRand[BUFFER_SIZE];

   // Fill disk cache.
   {
      const char szFilename[] = "D:\\file0.tmp";
      AXODBG_printf("Filling disk cache ... \n");

      srand(1);
      DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

      CFileIOEx File;
      VERIFY(File.Create(szFilename, FALSE, dwFlagsAndAttributes));

      for (int j=0; j<BUFFER_SIZE; j++)
         anRand[j] = (short)rand();

      for (int i=0; i<REPETITIONS; i++)
         VERIFY(File.Write(anRand, sizeof(anRand)));

      VERIFY(File.Close());
   }

   // Non-Overlapped test.
   {
      const char szFilename[] = "D:\\file2.tmp";
      AXODBG_printf("Start non-overlapped test ... ");

      srand(1);
      DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

      CFileIOEx File;
      VERIFY(File.Create(szFilename, FALSE, dwFlagsAndAttributes));

      CHighResTimer T;

      for (int i=0; i<REPETITIONS; i++)
      {
         Sleep(200);
         VERIFY(File.Write(anRand, sizeof(anRand)));
      }

      VERIFY(File.Close());

      AXODBG_printf("Time=%g\n", T.GetTime());
   }

   // Overlapped test.
   {
      const char szFilename[] = "D:\\file1.tmp";
      AXODBG_printf("Start overlapped test ... ");

      srand(1);
      DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED;

      CFileIOEx File;
      VERIFY(File.Create(szFilename, FALSE, dwFlagsAndAttributes));

      CHighResTimer T;

      for (int i=0; i<REPETITIONS; i++)
      {
         Sleep(200);
         VERIFY(File.Write(anRand, sizeof(anRand)));
      }

      VERIFY(File.Close());

      AXODBG_printf("Time=%g\n\n", T.GetTime());
   }

   return 0;
}

#endif  // TESTBED
