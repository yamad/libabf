//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  OleFile.CPP
// PURPOSE: Contains the class implimentation for the COleFile class, a simple wrapper
//          around the COM Compound File API.
// 
#include "wincpp.hpp"
#include "OleFile.hpp"
#include "ComDebug.h"

// #pragma warning(disable : 4310)  // cast truncates constant value
// #pragma warning(disable : 4244)  // 'return' : conversion from 'int' to 'unsigned short', possible loss of data
// #include <comdef.h>

#ifndef __ATLCONV_H__
#include "oleconv.h"
#endif

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
COleFile::COleFile()
{
   MEMBERASSERT();
   m_pStorage = NULL;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
COleFile::~COleFile()
{
   MEMBERASSERT();
   Close();   
}

//===============================================================================================
// FUNCTION: CreateEx
// PURPOSE:  Open the file for reading or writing.
//
BOOL COleFile::Create(LPCTSTR pszFilename, DWORD dwMode)
{
   USES_CONVERSION;
   Close();

   strncpy(m_szFilename, pszFilename, sizeof(m_szFilename)-1);
   m_szFilename[sizeof(m_szFilename)-1] = '\0';

   HRESULT hResult = StgCreateDocfile(T2COLE(pszFilename), dwMode, 0, &m_pStorage);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   return TRUE;
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Close the file.
//
BOOL COleFile::Close()
{
   m_szFilename[0] = '\0';
   if (!m_pStorage)
      return TRUE;

   m_pStorage->Release();
   m_pStorage = NULL;
   return TRUE;
}

//===============================================================================================
// FUNCTION: IsOpen
// PURPOSE:  Returns TRUE if the file is open.
//
BOOL COleFile::IsOpen() const
{
   MEMBERASSERT();
   return m_pStorage!=NULL;
}

//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  .
//
LPCSTR COleFile::GetFileName() const
{
   MEMBERASSERT();
   return m_szFilename;
}

//===============================================================================================
// FUNCTION: GetStorage
// PURPOSE:  Returns the held storage interface.
//
IStorage *COleFile::GetStorage() const
{
	MEMBERASSERT();
	return m_pStorage;
}

//===============================================================================================
// FUNCTION: CreateStream
// PURPOSE:  Creates and opens a stream object with the specified name contained in this storage object. 
//
BOOL COleFile::CreateStream(LPCTSTR pszStreamName, COleStream &Stream, DWORD dwFlags)
{
   MEMBERASSERT();
   
   if (!Stream.Create(m_pStorage, pszStreamName, dwFlags))
      return SetLastError(Stream.GetLastError());
   
   return TRUE;
}

//===============================================================================================
// FUNCTION: OpenStream
// PURPOSE:  Opens an existing stream object within this storage object using the specified access permissions in grfMode. 
//
BOOL COleFile::OpenStream(LPCTSTR pszStreamName, COleStream &Stream, DWORD dwFlags)
{
   MEMBERASSERT();

   if (!Stream.Open(m_pStorage, pszStreamName, dwFlags))
      return SetLastError(Stream.GetLastError());
   
   return TRUE;
}

//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error code and returns FALSE.
//
BOOL COleFile::SetLastError(HRESULT hResult) const
{
   MEMBERASSERT();
   m_hrLastError = hResult;
   return FALSE;
}

//===============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  .
//
HRESULT COleFile::GetLastError() const
{
   MEMBERASSERT();
   return m_hrLastError;
}

//################################################################################################
//################################################################################################
//###
//###
//###
//################################################################################################
//################################################################################################


COleStream::COleStream(IStream *pStream)
{
	m_pStream        = NULL;
	m_szStreamName[0] = '\0';
   VERIFY(Attach(pStream));
}

COleStream::~COleStream()
{
   VERIFY(Close());
}

IStream *COleStream::Detach()
{
	IStream *pStream = m_pStream;
	m_pStream        = NULL;  // detach and transfer ownership of m_pStream
	m_szStreamName[0] = '\0';
	return pStream;
}

BOOL COleStream::GetName()
{
	MEMBERASSERT();
	USES_CONVERSION;

   STATSTG statstg = { 0 };
	HRESULT hResult = m_pStream->Stat(&statstg, 0);
   if (COM_FAILED(hResult))
		return SetLastError(hResult);

	if (statstg.pwcsName != NULL)
	{
		_fullpath(m_szStreamName, OLE2CT(statstg.pwcsName), sizeof(m_szStreamName));
		CoTaskMemFree(statstg.pwcsName);
	}

	return TRUE;
}

BOOL COleStream::Attach(IStream *pStream)
{
	MEMBERASSERT();
   VERIFY(Close());

	if (!pStream)
      return TRUE;

   m_pStream = pStream;
   if (!GetName())
   {
      m_pStream = NULL;
      return FALSE;
   }

   return TRUE;
}

BOOL COleStream::GetStatus(STATSTG *pStatus) const
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	// get status of the stream
	HRESULT hResult = m_pStream->Stat(pStatus, STATFLAG_NONAME);
   if (COM_FAILED(hResult))
		return SetLastError(hResult);

	return TRUE;
}

LPCSTR COleStream::GetName() const
{
	MEMBERASSERT();
	return m_szStreamName;
}

BOOL COleStream::GetPosition(LONGLONG *pPosition) const
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	ULARGE_INTEGER liPosition;
   LARGE_INTEGER liZero = { 0 }; 

	HRESULT hResult = m_pStream->Seek(liZero, STREAM_SEEK_CUR, &liPosition);
   if (COM_FAILED(hResult))
		return SetLastError(hResult);

   if (pPosition)
	   *pPosition = liPosition.QuadPart;
	return TRUE;
}

BOOL COleStream::Open(IStorage *pStorage, LPCTSTR pszStreamName, DWORD dwFlags)
{
	MEMBERASSERT();
	USES_CONVERSION;

   Close();

	ASSERT(pStorage != NULL);
	LPSZASSERT(pszStreamName);

	HRESULT hResult = pStorage->OpenStream(T2COLE(pszStreamName), NULL, dwFlags, 0, &m_pStream);
   if (COM_FAILED(hResult))
		return SetLastError(hResult);

	ASSERT(m_pStream != NULL);
   VERIFY(GetName());
	return TRUE;
}

BOOL COleStream::Create(IStorage *pStorage, LPCTSTR pszStreamName, DWORD dwFlags)
{
	MEMBERASSERT();
	USES_CONVERSION;
	ASSERT(pStorage != NULL);
	LPSZASSERT(pszStreamName);

   Close();

	HRESULT hResult = pStorage->CreateStream(T2COLE(pszStreamName), dwFlags, 0, 0, &m_pStream);
   if (COM_FAILED(hResult))
		return SetLastError(hResult);

	ASSERT(m_pStream != NULL);
   VERIFY(GetName());
	return TRUE;
}

// STREAM_SEEK_SET, STREAM_SEEK_CUR, STREAM_SEEK_END
BOOL COleStream::Seek(LONGLONG lOffset, UINT uFlag, LONGLONG *plNewOffset)
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	LARGE_INTEGER liOff;
   liOff.QuadPart = lOffset;

   ULARGE_INTEGER liNewPosition = { 0 };
	HRESULT hResult = m_pStream->Seek(liOff, uFlag, &liNewPosition);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   if (plNewOffset)
	   *plNewOffset = liNewPosition.QuadPart;

   return TRUE;
}

BOOL COleStream::SetLength(LONGLONG uNewLen)
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	ULARGE_INTEGER liNewLen;
   liNewLen.QuadPart = uNewLen;

	HRESULT hResult = m_pStream->SetSize(liNewLen);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   return TRUE;
}

BOOL COleStream::GetLength(LONGLONG *puLength) const
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	// get status of the stream
   STATSTG statstg = { 0 };
	HRESULT hResult = m_pStream->Stat(&statstg, STATFLAG_NONAME);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   if (puLength)
	   *puLength = statstg.cbSize.QuadPart;

   return TRUE;
}


BOOL COleStream::Read(void* pBuf, UINT uCount, UINT *puBytesRead)
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);
	ARRAYASSERT(PBYTE(pBuf), uCount);

	DWORD dwBytesRead = 0;
   HRESULT hResult = m_pStream->Read(pBuf, uCount, &dwBytesRead);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

	// always return number of bytes read
   if (puBytesRead)
	   *puBytesRead = dwBytesRead;

   return TRUE;
}

BOOL COleStream::Write(const void* pBuf, UINT uCount, UINT *puBytesWritten)
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);
	ARRAYASSERT(PBYTE(pBuf), uCount);

	// write to the stream
	DWORD dwBytesWritten = 0;
	if (uCount > 0)
   {
	   HRESULT hResult = m_pStream->Write(pBuf, uCount, &dwBytesWritten);
      if (COM_FAILED(hResult))
         return SetLastError(hResult);
   }

   // if no error, all bytes should have been written
	ASSERT(dwBytesWritten == uCount);
   if (puBytesWritten)
      *puBytesWritten = dwBytesWritten;

   return TRUE;
}

BOOL COleStream::LockRange(DWORD dwPos, DWORD dwCount)
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	// convert parameters to long integers
	ULARGE_INTEGER liPos;
	ULISet32(liPos, dwPos);
	ULARGE_INTEGER liCount;
	ULISet32(liCount, dwCount);

	// then lock the region
	HRESULT hResult = m_pStream->LockRegion(liPos, liCount, LOCK_EXCLUSIVE);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   return TRUE;
}

BOOL COleStream::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	// convert parameters to long integers
	ULARGE_INTEGER liPos;
	ULISet32(liPos, dwPos);
	ULARGE_INTEGER liCount;
	ULISet32(liCount, dwCount);

	// then lock the region
	HRESULT hResult = m_pStream->UnlockRegion(liPos, liCount, LOCK_EXCLUSIVE);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   return TRUE;
}

void COleStream::Abort()
{
	MEMBERASSERT();

	if (m_pStream != NULL)
	{
		m_pStream->Revert();
		m_pStream->Release();
      m_pStream = NULL;
	}

	m_szStreamName[0] = '\0';
}

BOOL COleStream::Flush()
{
	MEMBERASSERT();
	ASSERT(m_pStream != NULL);

	// commit will return an error only if the stream is transacted
	HRESULT hResult = m_pStream->Commit(0);
   if (COM_FAILED(hResult))
      return SetLastError(hResult);

   return TRUE;
}

BOOL COleStream::Close()
{
	MEMBERASSERT();

	if (m_pStream != NULL)
	{
		// commit the stream via Flush (which can be overriden)
		if (!Flush())
         return FALSE;

		m_pStream->Release();
      m_pStream = NULL;
	}

	m_szStreamName[0] = '\0';
   return TRUE;
}

IStream *COleStream::GetStream() const
{
	MEMBERASSERT();
	return m_pStream;
}

//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error code and returns FALSE.
//
BOOL COleStream::SetLastError(HRESULT hResult) const
{
   MEMBERASSERT();
   m_hrLastError = hResult;
   return FALSE;
}

//===============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  .
//
HRESULT COleStream::GetLastError() const
{
   MEMBERASSERT();
   return m_hrLastError;
}
