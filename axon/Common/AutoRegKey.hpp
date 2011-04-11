//***********************************************************************************************
//
//    Copyright (c) 1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  AutoRegKey.HPP
// PURPOSE: Define the class CAutoRegKey an auto pointer for HKEYs.
// AUTHOR:  BHI  Aug 1997
//
//
#ifndef INC_AutoRegKey_hpp
#define INC_AutoRegKey_hpp

//***********************************************************************************************
// CLASS:   CAutoRegKey
// PURPOSE: A smart HKEY class for Registry keys.
//
class CAutoRegKey
{
private:    // Private data.
   HKEY m_hKey;
   LONG m_lLastError;

private:    // Prevent copy constructor and operator=().
   CAutoRegKey(const CAutoRegKey &);
   const CAutoRegKey &operator=(const CAutoRegKey &);

public:     // Public member functions.

   // Constructors & destructor. See notes below.
   CAutoRegKey(HKEY hKey = NULL);
   ~CAutoRegKey();
   const CAutoRegKey &operator=(HKEY hKey);
   
   // Operations on the key.
   BOOL Open(HKEY hkeyParent, LPCTSTR pszSubKey, REGSAM samDesired);
   BOOL Create(HKEY hkeyParent, LPCTSTR pszSubKey, DWORD dwOptions, REGSAM samDesired);
   BOOL DeleteSubKey(LPCTSTR pszSubKey);

   BOOL SetValue(LPCTSTR pValueName, DWORD dwType, const void *pvData, DWORD cbData);
   BOOL QueryValue(LPCTSTR pValueName, PDWORD pdwType, void *pvData, PDWORD pdwData);

   BOOL SetDWORD(LPCTSTR pValueName, DWORD dwValue);
   BOOL QueryDWORD(LPCTSTR pValueName, DWORD *pdwValue);

   BOOL SetString(LPCTSTR pValueName, LPCTSTR pszData);
   BOOL QueryString(LPCTSTR pValueName, LPTSTR pszData, DWORD dwMaxData);

   BOOL DeleteValue(LPCTSTR pszValue);
   BOOL EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, 
                LPTSTR lpClass, LPDWORD lpcbClass, PFILETIME lpftLastWriteTime);
   BOOL Close();
   BOOL Flush();

   // Accessor functions to get at the wrapped array.
   operator HKEY () const;
   HKEY Get() const;

   // Non-destructive release of the held pointer.
   HKEY Release();

   // Get the last error that occurred with this key.
   LONG GetLastError();
   BOOL SetLastError(LONG lError);
};

//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  Create a new object that wraps a passed HKEY.
// NOTES:    If the passed HKEY is non-NULL, it *MUST* be a valid HKEY.
//
//           The definition of this constructor contains a default parameter of NULL 
//           so that CAutoRegKey objects can be created that do not hold anything.
//
inline CAutoRegKey::CAutoRegKey(HKEY hKey)
{
   MEMBERASSERT();
   m_hKey = hKey;
}

//================================================================================================
// FUNCTION: Destructor
// PURPOSE:  Closes a held HKEY if non-NULL.
//
inline CAutoRegKey::~CAutoRegKey()
{
   MEMBERASSERT();
   VERIFY(Close());
}

//================================================================================================
// FUNCTION: operator=
// PURPOSE:  Releases any held key and assigns a new one.
//
inline const CAutoRegKey &CAutoRegKey::operator=(HKEY hKey)
{
   MEMBERASSERT();
   VERIFY(Close());
   m_hKey = hKey;
   return *this;
}

//================================================================================================
// FUNCTION: Open
// PURPOSE:  Closes any held key and opens a new one.
//
inline BOOL CAutoRegKey::Open(HKEY hkeyParent, LPCTSTR pszSubKey, REGSAM samDesired)
{
   MEMBERASSERT();
#ifdef _DEBUG
   if (pszSubKey)
      LPSZASSERT(pszSubKey);
#endif

   // Close any existing key.
   VERIFY(Close());

   return SetLastError(RegOpenKeyEx(hkeyParent, pszSubKey, 0, samDesired, &m_hKey));
}

//================================================================================================
// FUNCTION: Create
// PURPOSE:  Closes any held HKEY and creates a new key.
//
inline BOOL CAutoRegKey::Create(HKEY hkeyParent, LPCTSTR pszSubKey, DWORD dwOptions, REGSAM samDesired)
{
   MEMBERASSERT();
   LPSZASSERT(pszSubKey);

   // Close any existing key.
   VERIFY(Close());

   DWORD dwDisposition = 0;
   return SetLastError(RegCreateKeyEx(hkeyParent, pszSubKey, 0, REG_NONE, dwOptions, 
                                      samDesired, NULL, &m_hKey, &dwDisposition));
}

//================================================================================================
// FUNCTION: Close
// PURPOSE:  Closes the held HKEY. Benign NOP if no key held.
//
inline BOOL CAutoRegKey::Close()
{
   MEMBERASSERT();
   if (!m_hKey)
      return TRUE;

   LONG lError = RegCloseKey(m_hKey);
   m_hKey      = NULL;

   return SetLastError(lError);
}

//================================================================================================
// FUNCTION: Flush
// PURPOSE:  Flushs the held HKEY. Benign NOP if no key held.
//
inline BOOL CAutoRegKey::Flush()
{
   MEMBERASSERT();
   if (!m_hKey)
      return TRUE;

   return SetLastError(RegFlushKey(m_hKey));
}

//================================================================================================
// FUNCTION: DeleteSubKey
// PURPOSE:  Deletes a subkey below this key.
//
inline BOOL CAutoRegKey::DeleteSubKey(LPCTSTR pszSubKey)
{
   MEMBERASSERT();
   if (!m_hKey)
      return FALSE;

   return SetLastError(RegDeleteKey(m_hKey, pszSubKey));
}

//================================================================================================
// FUNCTION: SetValue
// PURPOSE:  Sets a value for this key.
//
inline BOOL CAutoRegKey::SetValue(LPCTSTR pValueName, DWORD dwType, const void *pvData, DWORD cbData)
{
   MEMBERASSERT();
   LPSZASSERT(pValueName);
   if (!m_hKey)
      return FALSE;

   return SetLastError(RegSetValueEx(m_hKey, pValueName, NULL, dwType, (CONST BYTE *)pvData, cbData));
}

//================================================================================================
// FUNCTION: QueryValue
// PURPOSE:  Gets a value for this key.
//
inline BOOL CAutoRegKey::QueryValue(LPCTSTR pValueName, PDWORD pdwType, void *pvData, PDWORD pdwCount)
{
   MEMBERASSERT();
   LPSZASSERT(pValueName);
   if (!m_hKey)
      return FALSE;

   return SetLastError(RegQueryValueEx(m_hKey, pValueName, NULL, pdwType, (BYTE *)pvData, pdwCount));
}

//================================================================================================
// FUNCTION: SetString
// PURPOSE:  Sets a string value for this key.
//
inline BOOL CAutoRegKey::SetString(LPCTSTR pValueName, LPCTSTR pszData)
{
   MEMBERASSERT();
   LPSZASSERT(pValueName);
   LPSZASSERT(pszData);
   if (!m_hKey)
      return FALSE;

   return SetValue(pValueName, REG_SZ, pszData, (strlen(pszData)+1)*sizeof(TCHAR));
}

//================================================================================================
// FUNCTION: QueryString
// PURPOSE:  Gets a string value for this key.
//
inline BOOL CAutoRegKey::QueryString(LPCTSTR pValueName, LPTSTR pszData, DWORD dwMaxCount)
{
   MEMBERASSERT();
   LPSZASSERT(pValueName);
   ARRAYASSERT(pszData, dwMaxCount);
   if (!m_hKey)
      return FALSE;

   // Get the type and size of the value.
   DWORD dwType  = REG_SZ;
   DWORD dwBytes = 0;
   if (!QueryValue(pValueName, &dwType, NULL, &dwBytes))
      return FALSE;

   // If it is not a string, fail.
   if (dwType != REG_SZ)
      return SetLastError(ERROR_BAD_TOKEN_TYPE);

   // clip to the buffer size.
   DWORD dwCount = dwBytes / sizeof(TCHAR);
   if ( dwCount > dwMaxCount )
      dwCount = dwMaxCount - 1;

   // get the data and zero terminate it to be sure.
   VERIFY(QueryValue(pValueName, &dwType, pszData, &dwCount));
   pszData[dwMaxCount-1] = '\0';
   return TRUE;
}

//================================================================================================
// FUNCTION: SetDWORD
// PURPOSE:  Sets a DWORD value for this key.
//
inline BOOL CAutoRegKey::SetDWORD(LPCTSTR pValueName, DWORD dwValue)
{
   MEMBERASSERT();
   LPSZASSERT(pValueName);
   if (!m_hKey)
      return FALSE;

   return SetValue(pValueName, REG_DWORD, &dwValue, sizeof(dwValue));
}

//================================================================================================
// FUNCTION: QueryDWORD
// PURPOSE:  Gets a DWORD value for this key.
//
inline BOOL CAutoRegKey::QueryDWORD(LPCTSTR pValueName, DWORD *pdwValue)
{
   MEMBERASSERT();
   LPSZASSERT(pValueName);
   WPTRASSERT(pdwValue);
   if (!m_hKey)
      return FALSE;

   // Get the type and size of the value.
   DWORD dwType  = REG_DWORD;
   DWORD dwCount = sizeof(DWORD);
   if (!QueryValue(pValueName, &dwType, pdwValue, &dwCount))
      return FALSE;

   // If it is not a DWORD, fail.
   if (dwType != REG_DWORD)
   {
      *pdwValue = 0;
      return SetLastError(ERROR_BAD_TOKEN_TYPE);
   }
   return TRUE;
}

//================================================================================================
// FUNCTION: DeleteValue
// PURPOSE:  Deletes a value from this key.
//
inline BOOL CAutoRegKey::DeleteValue(LPCTSTR pszValue)
{
   MEMBERASSERT();
   if (!m_hKey)
      return FALSE;

   return SetLastError(RegDeleteValue(m_hKey, pszValue));
}

//================================================================================================
// FUNCTION: Get
// PURPOSE:  Returns the held HKEY without giving up ownership of it.
//
inline HKEY CAutoRegKey::Get() const
{
   MEMBERASSERT();
   return m_hKey; 
}

//================================================================================================
// FUNCTION: Overloaded cast operator
// PURPOSE:  Returns the held HKEY without giving up ownership of it.
//
inline CAutoRegKey::operator HKEY() const
{
   MEMBERASSERT();
   return Get();
}

//================================================================================================
// FUNCTION: Release
// PURPOSE:  Returns the held pointer, giving up ownership of it.
//
inline HKEY CAutoRegKey::Release()
{
   MEMBERASSERT();
   HKEY rval = m_hKey;
   m_hKey    = NULL;
   return rval; 
}

//================================================================================================
// FUNCTION: Release
// PURPOSE:  Returns the held pointer, giving up ownership of it.
//
inline BOOL CAutoRegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, 
                                 LPTSTR lpClass, LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
   MEMBERASSERT();
   return SetLastError(RegEnumKeyEx(m_hKey, dwIndex, lpName, lpcbName, 0, 
                                    lpClass, lpcbClass, lpftLastWriteTime));
}

//================================================================================================
// FUNCTION: GetLastError
// PURPOSE:  Return the last error that occurred on this key.
//
inline LONG CAutoRegKey::GetLastError()
{
   MEMBERASSERT();
   
   return m_lLastError; 
}

//================================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Set an error that occurred on this key and return TRUE or FALSE.
//
inline BOOL CAutoRegKey::SetLastError(LONG lError)
{
   MEMBERASSERT();
   if (lError!=ERROR_SUCCESS)
   {
      m_lLastError = lError;
      // SHOW_SYSTEM_ERROR(lError);
   }
   return (lError==ERROR_SUCCESS); 
}

#endif          // INC_AutoRegKey_hpp
