//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Cryptor.HPP
// PURPOSE: 
// AUTHOR:  BHI  Jun 2001
//

#ifndef INC_CRYPTOR_HPP
#define INC_CRYPTOR_HPP

#pragma once

#include "ArrayPtr.hpp"

class CCryptor
{
private:   // Attributes
   CArrayPtrEx<BYTE> m_Buffer;

private:   // Unimplemented copy functions.
   CCryptor(const CCryptor &);
   const CCryptor &operator=(const CCryptor &);

private:   // Private functions

public:    // Public interface
   CCryptor();
   ~CCryptor();

   const void *Get() const { return m_Buffer.Get(); }
   UINT GetSize() const    { return m_Buffer.GetCount(); }

   bool SetData(const void *pBlob, UINT uSize);
   bool GetData(void *pBlob, UINT uSize) const;

   bool Encrypt();
   bool Decrypt();

   bool SaveToRegistry(LPCSTR pszSection, LPCSTR pszEntry, LPCSTR pszAppName) const;
   bool LoadFromRegistry(LPCSTR pszSection, LPCSTR pszEntry, LPCSTR pszAppName);
};

#endif      // INC_CRYPTOR_HPP

