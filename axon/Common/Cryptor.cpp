//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Cryptor.CPP
// PURPOSE: 
// AUTHOR:  BHI  Jun 2001
//

#include "wincpp.hpp"
#include "Cryptor.hpp"
#include <wincrypt.h>   // For Crypt functions.
#include <memory.h>     // For memcpy & memmove.

#define BLOCK_SIZE 160

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CCryptor::CCryptor()
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CCryptor::~CCryptor()
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: SetData
// PURPOSE:  .
//
bool CCryptor::SetData(const void *pBlob, UINT uSize)
{
   MEMBERASSERT();
   const BYTE *pData = (const BYTE *)pBlob;
   RARRAYASSERT(pData, uSize);

   return !!m_Buffer.Clone(pData, uSize);
}

//===============================================================================================
// FUNCTION: GetData
// PURPOSE:  .
//
bool CCryptor::GetData(void *pBlob, UINT uSize) const
{
   MEMBERASSERT();
   ASSERT(uSize == GetSize());
   memcpy(pBlob, m_Buffer, uSize);
   return true;
}

//===============================================================================================
// FUNCTION: SaveToRegistry
// PURPOSE:  .
//
bool CCryptor::SaveToRegistry(LPCSTR pszSection, LPCSTR pszEntry, LPCSTR pszAppName) const
{
   MEMBERASSERT();
   return !!AXU_WritePrivateProfileBlob( pszSection, pszEntry, m_Buffer, m_Buffer.GetCount(), pszAppName);
}

//===============================================================================================
// FUNCTION: LoadFromRegistry
// PURPOSE:  .
//
bool CCryptor::LoadFromRegistry(LPCSTR pszSection, LPCSTR pszEntry, LPCSTR pszAppName)
{
   MEMBERASSERT();

   UINT uSize = AXU_GetPrivateProfileBlobSize( pszSection, pszEntry, pszAppName);

   if ((uSize==0) || !m_Buffer.Realloc(uSize))
   {
      m_Buffer.Free();
      return false;
   }
   return !!AXU_GetPrivateProfileBlob( pszSection, pszEntry, m_Buffer, m_Buffer.GetCount(), pszAppName);
}

//===============================================================================================
// CLASS:   CCryptContext
// PURPOSE: Simple wrapper on the encryption context to simplify cleanup.
//
class CCryptContext
{
private:
   HCRYPTPROV m_hProv;
   DWORD      m_dwLastError;

public:
   CCryptContext()
   {
      m_hProv       = NULL;
      m_dwLastError = 0;
   }
   ~CCryptContext()
   {
      Release();
   }
   BOOL Acquire(LPCTSTR pszProvider=MS_DEF_PROV, DWORD dwFlags=0)
   {
      Release();
	   if (!CryptAcquireContext(&m_hProv, NULL, pszProvider, PROV_RSA_FULL, dwFlags))
         return SetLastError();
      return TRUE;
   }
   void Release()
   {
      if (m_hProv)
		   CryptReleaseContext(m_hProv, 0);
      m_hProv = NULL;
   }
   HCRYPTKEY GenKey(ALG_ID Algid, DWORD dwFlags)
   {
      HCRYPTKEY hKey = NULL;
      if (!CryptGenKey(m_hProv, Algid, dwFlags, &hKey))
         return (HCRYPTKEY)SetLastError();
      return hKey;
   }
	HCRYPTKEY GetUserKey(ALG_ID Algid)
   {
      HCRYPTKEY hKey = NULL;
   	if (!CryptGetUserKey(m_hProv, Algid, &hKey))
         return (HCRYPTKEY)SetLastError();
      return hKey;
   }
	HCRYPTKEY ImportKey(PBYTE buffer, DWORD dwBlobLen)
   {
      HCRYPTKEY hKey = NULL;
	   if (!CryptImportKey(m_hProv, buffer, dwBlobLen, 0, 0, &hKey))
         return (HCRYPTKEY)SetLastError();
      return hKey;
   }

   BOOL SetLastError()
   {
      m_dwLastError = ::GetLastError();
      SHOW_SYSTEM_ERROR(m_dwLastError);
      return false;
   }
   DWORD GetLastError() const
   {
      return m_dwLastError;
   }
};

//===============================================================================================
// CLASS:   CCryptContext
// PURPOSE: Simple wrapper on an encryption key to simplify cleanup.
//
class CAutoCryptKey
{
private:
   HCRYPTKEY m_hKey;

public:
   CAutoCryptKey(HCRYPTKEY hKey)
   {
      m_hKey = hKey;
   }
   ~CAutoCryptKey()
   {
      if (m_hKey)
	      CryptDestroyKey(m_hKey);
      m_hKey = NULL;
   }
   operator HCRYPTKEY() const { return m_hKey; }
};

//===============================================================================================
// FUNCTION: Encrypt
// PURPOSE:  .
//
bool CCryptor::Encrypt()
{
   MEMBERASSERT();
   
   CCryptContext Prov;
   if (!Prov.Acquire(MS_DEF_PROV, 0))
   {
      // Check reasons for the failure...
		DWORD err = Prov.GetLastError();
		if ((err!=NTE_KEYSET_NOT_DEF) && (err!=NTE_BAD_KEYSET))
         return false;

      // Create a new keyset.
      if (!Prov.Acquire(MS_DEF_PROV, CRYPT_NEWKEYSET))
         return false;

      CAutoCryptKey SignKey = Prov.GenKey(AT_SIGNATURE, 0);
      if (!SignKey)
         return false;

      CAutoCryptKey XchgKey = Prov.GenKey(AT_KEYEXCHANGE, 0);
      if (!XchgKey)
         return false;

      // Try again.
      if (!Prov.Acquire(MS_DEF_PROV, 0))
         return false;
	}
	
	// Get a handle to key exchange key.
   CAutoCryptKey XchgKey = Prov.GetUserKey(AT_KEYEXCHANGE);
   if (!XchgKey)
   {
      CAutoCryptKey SignKey = Prov.GenKey(AT_SIGNATURE, 0);
      if (!SignKey)
         return false;

      XchgKey = Prov.GenKey(AT_KEYEXCHANGE, 0);
      if (!XchgKey)
         return false;
   }

	// Create a random block cipher session key.
   CAutoCryptKey Key = Prov.GenKey(CALG_RC4, CRYPT_EXPORTABLE);
   if (!Key)
      return false;
	
	// Determine the size of the key blob and the data portion.
	DWORD dwBlobLen = 0;
	VERIFY(CryptExportKey(Key, XchgKey, SIMPLEBLOB, 0, NULL, &dwBlobLen));
	
   DWORD dwDataLen = GetSize();
	if (!CryptEncrypt(Key, 0, TRUE, 0, NULL, &dwDataLen, 0))
   {
      Prov.SetLastError();
      return false;
	}

   // Allocate sufficient space for key, and data
	DWORD dwSize = dwBlobLen + sizeof(DWORD) + dwDataLen;
   CArrayPtrEx<BYTE> Buffer;
	if (!Buffer.Alloc(dwSize))
      return false;
   PBYTE pDestination = Buffer;
   UINT  uDestination = Buffer.GetCount();
	
	// Export the key into a simple key blob.
	*(DWORD*)pDestination = dwBlobLen;
	if (!CryptExportKey(Key, XchgKey, SIMPLEBLOB, 0, pDestination+sizeof(DWORD), &dwBlobLen))
   {
      Prov.SetLastError();
      return false;
	}
   pDestination += sizeof(dwBlobLen) + dwBlobLen;
   uDestination -= sizeof(dwBlobLen) + dwBlobLen;
	
   PBYTE pSource = m_Buffer;
   UINT  uSource = m_Buffer.GetCount();

	while (uSource)
   {    
		DWORD dwCount = min(BLOCK_SIZE, uSource);	
		memcpy(pDestination, pSource, dwCount);

		pSource += dwCount;
      uSource -= dwCount;
		
		if (!CryptEncrypt(Key, 0, uSource==0, 0, pDestination, &dwCount, uDestination))
      {
         Prov.SetLastError();
         return false;
		}
      pDestination += dwCount;
      uDestination -= dwCount;
	}
   ASSERT(uDestination==0);

   return !!m_Buffer.Clone(Buffer.Get(), Buffer.GetCount());
}

//===============================================================================================
// FUNCTION: Decrypt
// PURPOSE:  .
//
bool CCryptor::Decrypt()
{
   MEMBERASSERT();
   
	// Get a handle to the default provider.
   CCryptContext Prov;
   if (!Prov.Acquire(MS_DEF_PROV, 0))
      return false;

   CArrayPtrEx<BYTE> Buffer;
   if (!Buffer.Clone(m_Buffer, m_Buffer.GetCount()))
      return false;

   PBYTE pSource = Buffer;
   UINT  uSource = Buffer.GetCount();

	DWORD dwBlobLen = *(DWORD*)pSource;
	// Determine the size of the key blob and allocate memory.
	CAutoCryptKey Key = Prov.ImportKey(pSource+sizeof(DWORD), dwBlobLen);
	if (!Key)
      return false;

   pSource += sizeof(dwBlobLen) + dwBlobLen;
   uSource -= sizeof(dwBlobLen) + dwBlobLen;

   PBYTE pDestination = Buffer;
   UINT  uDestination = 0;

	while (uSource)
   {    
		DWORD dwCount = min(BLOCK_SIZE, uSource);	
      uSource -= dwCount;
      PBYTE pNext = pSource + dwCount;

		if (!CryptDecrypt(Key, 0, uSource==0, 0, pSource, &dwCount))
      {
         Prov.SetLastError();
         return false;
	   }

      memmove(pDestination, pSource, dwCount);
      pDestination += dwCount;
      uDestination += dwCount;

      pSource = pNext;
	}
   return !!m_Buffer.Clone(Buffer, uDestination);
}
