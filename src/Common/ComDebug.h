//******************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//******************************************************************************
// HEADER:  ComDebug.h
// PURPOSE: Contains utility macros and defines for Axon COM debugging.
// AUTHOR:  BHI  Jun 1996
//
#ifndef INC_COMDEBUG_H
#define INC_COMDEBUG_H

#pragma once

// Include the minimum header files necessary to define the COM interface fundamentals.
#ifndef STDMETHOD
   #include <objbase.h>
#endif

// LPUNKNOWN is a standard typedef, but PUNKNOWN is not.
#ifndef PUNKNOWN
   typedef IUnknown *PUNKNOWN;
#endif

//===============================================================================================
// MACRO:   COM_IS_VALID_INTERFACE
// PURPOSE: Test the validity of an interface pointer.
// RETURNS: BOOL -- TRUE = Interface is a COM interface.
//
#define COM_IS_VALID_INTERFACE(pI)     (COM_IsValidInterface(pI, #pI, __FILE__, __LINE__))
#define COM_ASSERT_VALID_INTERFACE(pI) ASSERT(COM_IsValidInterface(pI, #pI, __FILE__, __LINE__))

//===============================================================================================
// MACROS:  COM_SUCCEEDED, COM_FAILED
// PURPOSE: Use these macros instead of the standard COM "SUCCEEDED" and "FAILED" macros
//          to get debug time dumping of the HRESULT on failure of an interface call.
//
#ifdef _DEBUG
   #define COM_SUCCEEDED(hResult) (COM_CheckHResult(hResult, __FILE__, __LINE__))
   #define COM_FAILED(hResult)    (!COM_CheckHResult(hResult, __FILE__, __LINE__))
#else
   #define COM_SUCCEEDED(hResult) SUCCEEDED(hResult)
   #define COM_FAILED(hResult)    FAILED(hResult)
#endif

#define COM_ASSERT(hr)            ASSERT(COM_SUCCEEDED(hr))
#define COM_VERIFY(exp)           VERIFY(COM_SUCCEEDED(exp))

//===============================================================================================
// Helper functions for the above macros.
//
BOOL  COM_IsValidInterface(PUNKNOWN pInterface, LPCSTR pszInterface, LPCSTR pszFile, int nLine);
SCODE COM_ShowHResult(HRESULT hResult, LPCSTR pszFile, int nLine);
BOOL  COM_CheckHResult(HRESULT hResult, LPCSTR pszFile, int nLine);

#endif      // INC_COMDEBUG_H
