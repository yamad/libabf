//******************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//******************************************************************************
// HEADER:  OLEDEBUG.H
// PURPOSE: Contains utility macros and defines for Axon OLE debugging.
// AUTHOR:  BHI  Jun 1996
//
#ifndef INC_OLEDEBUG_H
#define INC_OLEDEBUG_H

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
// MACRO:   OLE_IS_VALID_INTERFACE
// PURPOSE: Test the validity of an interface pointer.
// RETURNS: BOOL -- TRUE = Interface is a COM interface.
//
#define OLE_IS_VALID_INTERFACE(pI) (OLE_IsValidInterface(pI, #pI, __FILE__, __LINE__))

//===============================================================================================
// MACROS:  OLE_SUCCEEDED, OLE_FAILED
// PURPOSE: Use these macros instead of the standard COM "SUCCEEDED" and "FAILED" macros
//          to get debug time dumping of the HRESULT on failure of an interface call.
//
#ifdef _DEBUG
   #define OLE_SUCCEEDED(hResult) (OLE_CheckHResult(hResult, __FILE__, __LINE__))
   #define OLE_FAILED(hResult)    (!OLE_CheckHResult(hResult, __FILE__, __LINE__))
#else
   #define OLE_SUCCEEDED(hResult) SUCCEEDED(hResult)
   #define OLE_FAILED(hResult)    FAILED(hResult)
#endif

//===============================================================================================
// Helper functions for the above macros.
//
BOOL  OLE_IsValidInterface(PUNKNOWN pInterface, LPCSTR pszInterface, LPCSTR pszFile, int nLine);
SCODE OLE_ShowHResult(HRESULT hResult, LPCSTR pszFile, int nLine);
BOOL  OLE_CheckHResult(HRESULT hResult, LPCSTR pszFile, int nLine);

#endif      // INC_OLEDEBUG_H
