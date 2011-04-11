//**********************************************************************
// FILE:
// RegistrySectionKey.h
//
// PURPOSE:
// Declare & define the class CRegistrySectionKey
//
// AUTHOR:
// Andrew Dalgleish
//
// COPYRIGHT:
// Copyright (c) 1996 Axon Instruments.
// All rights reserved.

//----------------------------------------------------------------------
// Prevent multiple inclusions
#ifndef  INC_REGISTRYSECTIONKEY_H
#define  INC_REGISTRYSECTIONKEY_H

//----------------------------------------------------------------------
// Check this is a C++ compilation
// It would be better to use .HPP, but the VC4 IDE expects .H
#ifndef  __cplusplus
#error   "__cplusplus not defined - this header file is for C++ ONLY."
#endif

//**********************************************************************
// CLASS:
// CRegistrySectionKey
//
// PURPOSE:
// Manage a HKEY resource obtained by calling CWinApp::GetSectionKey()
class CRegistrySectionKey
{
public:
   CRegistrySectionKey(LPCTSTR lpszSection);
   ~CRegistrySectionKey();
   BOOL     IsHandleOk() const;
   operator HKEY ();
private:
   // prevent copying by making these private and unimplemented
   CRegistrySectionKey(const CRegistrySectionKey &);
   const CRegistrySectionKey & operator = (const CRegistrySectionKey &);
   // The managed HKEY resource
   HKEY		m_hKey;
};

//----------------------------------------------------------------------
// METHOD:
// CRegistrySectionKey
//
// PURPOSE:
// Default constructor
// Obtain a HKEY resource from CWinApp::GetSectionKey();
//
// RETURNS:
// none
inline CRegistrySectionKey::CRegistrySectionKey (LPCTSTR lpszSection)
:m_hKey(NULL)
{
   // Check the section key name is ok
   ASSERT (lpszSection != NULL);
   // Check the application pointer we de-reference below
   ASSERT (AfxGetApp() != NULL);
   // Get the section key
   m_hKey = AfxGetApp()->GetSectionKey(lpszSection);
}

//----------------------------------------------------------------------
// METHOD:
// ~CRegistrySectionKey
// 
// PURPOSE:
// Destructor
// Close the HKEY resource from CWinApp::GetSectionKey();
//
// RETURNS:
// none
inline CRegistrySectionKey::~CRegistrySectionKey()
{
   // If we have a valid key
   if (m_hKey != NULL)
   {
      // Flush the key to disk
      RegFlushKey (m_hKey);
      // Close the key
      RegCloseKey (m_hKey);
   }
}

//----------------------------------------------------------------------
// METHOD:
// IsHandleOk
// 
// PURPOSE:
// Test whether the managed HKEY resource is not NULL
//
// RETURNS:
// BOOL
//    TRUE     The key is not NULL
//    FALSE    The key is NULL
inline BOOL     CRegistrySectionKey::IsHandleOk() const
{
   return m_hKey != NULL;
}

//----------------------------------------------------------------------
// METHOD:
// operator HKEY
// 
// PURPOSE:
// Return the managed HKEY resource
//
// RETURNS:
// HKEY     The managed HKEY resource
inline CRegistrySectionKey::operator HKEY ()
{
   return m_hKey;
}

//----------------------------------------------------------------------
// Prevent multiple inclusions
#endif   // #ifndef  INC_REGISTRYSECTIONKEY_H

//**********************************************************************
// end of file
