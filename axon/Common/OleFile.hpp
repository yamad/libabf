//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  OleFile.HPP
// PURPOSE: Contains the class definition for the COleFile class, a simple wrapper
//          around the COM Compound File API.
// 
#ifndef INC_OLEFILE_HPP
#define INC_OLEFILE_HPP

#pragma once

class COleStream;

class COleFile
{
private:    // Member variables and constants.
   TCHAR           m_szFilename[_MAX_PATH]; // The complete filename of the file
   IStorage       *m_pStorage;              // The interface to the top level storage.
   mutable HRESULT m_hrLastError;           // The error code returned by the last interface call that failed.

private:    // Prevent default copy constructor and operator=()
   COleFile(const COleFile &FI);
   const COleFile &operator=(const COleFile &FI);

private:    // Internal functions.
   BOOL    SetLastError(HRESULT hResult) const;
   
public:   
   COleFile();
   ~COleFile();
   
   BOOL    Create(LPCTSTR szFileName, DWORD dwMode = STGM_CREATE | STGM_SHARE_EXCLUSIVE | STGM_READWRITE);
   BOOL    Open(LPCTSTR szFileName, DWORD dwMode = STGM_SHARE_EXCLUSIVE | STGM_READWRITE)
   {       return Create(szFileName, dwMode);  }

   BOOL    Close();
   BOOL    IsOpen() const;
   LPCSTR  GetFileName() const;
   IStorage *GetStorage() const;

   BOOL CreateStream(LPCTSTR pszStreamName, COleStream &Stream,
                     DWORD dwFlags = STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE);
   BOOL OpenStream(LPCTSTR pszStreamName, COleStream &Stream,
                   DWORD dwFlags = STGM_READWRITE|STGM_SHARE_EXCLUSIVE);

   HRESULT GetLastError() const;

#if 0
   // Creates and opens a new storage object within this storage object. 
   CreateStorage 
 
   // Opens an existing storage object with the specified name according to the specified access mode. 
   OpenStorage 
 
   // Copies the entire contents of this open storage object into another storage object. The layout of the destination storage object may differ. 
   CopyTo 
 
   // Copies or moves a substorage or stream from this storage object to another storage object. 
   MoveElementTo 
 
   // Reflects changes for a transacted storage object to the parent level. 
   Commit 
 
   // Discards all changes that have been made to to the storage object since the last commit operation. 
   Revert 
 
   // Returns an enumerator object that can be used to enumerate the storage and stream objects contained within this storage object. 
   EnumElements 
 
   // Removes the specified storage or stream from this storage object. 
   DestroyElement 
 
   // Renames the specified storage or stream in this storage object. 
   RenameElement 
 
   // Sets the modification, access, and creation times of the indicated storage element, if supported by the underlying file system. 
   SetElementTimes 
 
   // Assigns the specified CLSID to this storage object. 
   SetClass 
 
   // Stores up to 32 bits of state information in this storage object. 
   SetStateBits 
 
   // Returns the STATSTG structure for this open storage object. 
   Stat 
#endif

};


/////////////////////////////////////////////////////////////////////////////
// COleStream - implementation of CFile which uses an IStream

class COleStream
{
private:
   LPSTREAM        m_pStream;
   char            m_szStreamName[_MAX_PATH];
   mutable HRESULT m_hrLastError;

private:
   BOOL    GetName();

public:
   COleStream(IStream *pStream = NULL);
   ~COleStream();

   // Operations
   BOOL Open(IStorage *pStorage, LPCTSTR pszStreamName, DWORD dwFlags = STGM_READWRITE|STGM_SHARE_EXCLUSIVE);
   BOOL Create(IStorage *pStorage, LPCTSTR pszStreamName, DWORD dwFlags = STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE);
   BOOL Close();

   // attach & detach can be used when Open/Create functions aren't adequate
   BOOL     Attach(LPSTREAM pStream);
   LPSTREAM Detach();

   // Returns the current stream
   IStream* GetStream() const;

   BOOL   GetStatus(STATSTG *pStatus) const;
   LPCSTR GetName() const;

   // STREAM_SEEK_SET, STREAM_SEEK_CUR, STREAM_SEEK_END
   BOOL Seek(LONGLONG lOffset, UINT uFlag=STREAM_SEEK_SET, LONGLONG *plNewOffset=NULL);
   BOOL GetPosition(LONGLONG *plCurrentPosition) const;

   BOOL SetLength(LONGLONG uNewLen);
   BOOL GetLength(LONGLONG *puLength) const;

   BOOL Read(void* pBuf, UINT nCount, UINT *puBytesRead=NULL);
   BOOL Write(const void* pBuf, UINT nCount, UINT *puBytesWritten=NULL);

   BOOL LockRange(DWORD dwPos, DWORD dwCount);
   BOOL UnlockRange(DWORD dwPos, DWORD dwCount);

   void Abort();
   BOOL Flush();

   BOOL SetLastError(HRESULT hResult) const;
   HRESULT GetLastError() const;
};

#endif // INC_OLEFILE_HPP
