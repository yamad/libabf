//***********************************************************************************************
//
//    Copyright (C) 1992-1997 Microsoft Corporation
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  AfxMT.HPP
// PURPOSE: Utility classes "borrowed" and simplified from MFC 4.21.
//          AFXMT - MFC Multithreaded Extensions (Syncronization Objects)
// AUTHOR:  BHI  Jul 1998
//

#ifndef INC_AFXMT_HPP
#define INC_AFXMT_HPP

#pragma once

#ifdef _MFC_VER
#include <afxmt.h>
#else

/////////////////////////////////////////////////////////////////////////////
// Basic synchronization object

class CSyncObject
{
protected:
   HANDLE  m_hObject;
   char    m_szName[64];

public:
	CSyncObject(LPCTSTR pszName)
   {
	   m_hObject   = NULL;
      m_szName[0] = '\0';

      if (pszName)
      {
	      strncpy(m_szName, pszName, ELEMENTS_IN(m_szName)-1);
         m_szName[ELEMENTS_IN(m_szName)-1] = '\0';
      }
   }

	~CSyncObject()
   {
	   if (m_hObject != NULL)
		   ::CloseHandle(m_hObject);
      m_hObject = NULL;
   }

   operator HANDLE() const
   {  return m_hObject;  }

	BOOL Lock(DWORD dwTimeout=INFINITE)
   {  return (::WaitForSingleObject(m_hObject, dwTimeout) == WAIT_OBJECT_0);  }
};

/////////////////////////////////////////////////////////////////////////////
// CSemaphore

class CSemaphore : public CSyncObject
{
public:
	CSemaphore(LONG lInitialCount=1, LONG lMaxCount=1, LPCTSTR pstrName=NULL)
	   : CSyncObject(pstrName)
   {
	   ASSERT(lMaxCount > 0);
	   ASSERT(lInitialCount <= lMaxCount);
	   m_hObject = ::CreateSemaphore(NULL, lInitialCount, lMaxCount, pstrName);
   }

	BOOL Unlock()
   {  return Unlock(1, NULL);  }

	BOOL Unlock(LONG lCount, LPLONG pPrevCount = NULL)
   {  return ::ReleaseSemaphore(m_hObject, lCount, pPrevCount);  }
};

/////////////////////////////////////////////////////////////////////////////
// CMutex

class CMutex : public CSyncObject
{
public:
	CMutex(BOOL bInitiallyOwn = FALSE, LPCTSTR pszName = NULL)
   	: CSyncObject(pszName)
   {  m_hObject = ::CreateMutex(NULL, bInitiallyOwn, pszName);  }

	BOOL Unlock()
   {	return ::ReleaseMutex(m_hObject);  }
};

/////////////////////////////////////////////////////////////////////////////
// CEvent

class CEvent : public CSyncObject
{
public:
	CEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE, LPCTSTR pszName = NULL)
   	: CSyncObject(pszName)
   {  m_hObject = ::CreateEvent(NULL, bManualReset, bInitiallyOwn, pszName);  }

	BOOL SetEvent()
   {
      ASSERT(m_hObject != NULL);
      return ::SetEvent(m_hObject); 
   }
	BOOL PulseEvent()
   {
      ASSERT(m_hObject != NULL); 
      return ::PulseEvent(m_hObject);
   }
	BOOL ResetEvent()
   {
      ASSERT(m_hObject != NULL);
      return ::ResetEvent(m_hObject);
   }
};

/////////////////////////////////////////////////////////////////////////////
// CCriticalSection

class CCriticalSection
{
protected:
	CRITICAL_SECTION m_sect;

public:
	CCriticalSection()
   {  ::InitializeCriticalSection(&m_sect);  }

	~CCriticalSection()
   {  ::DeleteCriticalSection(&m_sect);  }

	operator CRITICAL_SECTION*()
   {  return &m_sect;  }

	BOOL Lock()
   {  ::EnterCriticalSection(&m_sect); return TRUE;  }

#if 0   
	BOOL TryLock()
   {  return ::TryEnterCriticalSection(&m_sect);  }
	
   BOOL Lock(DWORD dwTimeout, UINT uGranularity = 100)
   {  
      while (dwTimeout >= uGranularity)
      {
         if (TryLock())
            return TRUE;
         Sleep(c_uGranularity);
         dwTimeout -= c_uGranularity;
      }
      return TryLock();
   }
#endif

   BOOL Unlock()
   {  ::LeaveCriticalSection(&m_sect); return TRUE;  }
};

#endif   // _MFC_VER

template <class T>
class CAutoLock
{
private:
   T *m_pObj;
public:
   CAutoLock(T *pObj) { m_pObj = pObj; pObj->Lock(); }
   ~CAutoLock()       { m_pObj->Unlock(); m_pObj = NULL; }
};

#endif   // INC_AFXMT_HPP

