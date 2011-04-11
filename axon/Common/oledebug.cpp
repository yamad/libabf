//******************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//******************************************************************************
// HEADER:  OLEDEBUG.H
// PURPOSE: Contains utility functions for OLE debugging.
// AUTHOR:  BHI  Jun 1996
// NOTES:   N.B. In 16 bit builds remember to link to OLE2.LIB or else the symbol
//          _IID_IUnknown will be unknown to the linker.
//
#include "wincpp.hpp"
#include "oledebug.h"

//===============================================================================================
// FUNCTION: OLE_IsValidInterface
// PURPOSE:  Checks that the passed interface pointer is in fact a pointer to a COM interface.
// NOTES:    This function is not usually called directly. It is part of the 
//           code expansion for the OLE_IS_VALID_INTERFACE macro.
//
BOOL OLE_IsValidInterface(LPUNKNOWN pInterface, LPCSTR pszInterface, LPCSTR pszFile, int nLine)
{
   BOOL bRval = FALSE;
   if (pInterface)
   {
      LPUNKNOWN pUnknown = NULL;
      HRESULT hr = pInterface->QueryInterface(IID_IUnknown, (LPVOID FAR*)&pUnknown);
      bRval = OLE_SUCCEEDED(hr);
      if (bRval)
         pUnknown->Release();
   }
   if (!bRval)
      TRACE3("Invalid OLE Interface %s(%d): %s\n", pszFile, nLine, pszInterface);
   return bRval;
}

//===============================================================================================
// FUNCTION: OLE_ShowHResult
// PURPOSE:  Displays the information embedded in an HRESULT return value from a COM interface call.
// NOTES:    This function is not usually called directly. It is part of the 
//           code expansion for the OLE_SUCCEEDED and OLE_FAILED macros.
//
SCODE OLE_ShowHResult(HRESULT hResult, LPCSTR pszFile, int nLine)
{
   SCODE SCode = GetScode(hResult);
   
   // Split out known severity conditions:
   LPCSTR pszSeverity = "";
   switch (SCODE_SEVERITY(SCode))
   {
      case SEVERITY_ERROR:
         pszSeverity = "ERROR";
         break;
      default:
      case SEVERITY_SUCCESS:
         pszSeverity = "SUCCESS";
         break;
   }
   
   // Split out known facility sub types.
   LPCSTR pszFacility = "";
   switch (SCODE_FACILITY(SCode))
   {
      case FACILITY_RPC:
         pszFacility = "RPC ";
         break;
      case FACILITY_DISPATCH:
         pszFacility = "DISPATCH ";
         break;
      case FACILITY_STORAGE:
         pszFacility = "STORAGE ";
         break;
      case FACILITY_ITF:
         pszFacility = "INTERFACE ";
         break;
      default:
      case FACILITY_NULL:
         break;
   }
   TRACE5("OLE %s%s: %s(%d) = 0x%08lX\n", 
          pszFacility, pszSeverity, pszFile, nLine, SCODE_CODE(SCode));
   
   // Split out known/common SCODE values.
   switch (SCode)
   {          
      case E_UNEXPECTED:
         TRACE("Unexpected failure.\n");
         break;
      case E_NOTIMPL:
         TRACE("Not implemented.\n");
         break;
      case E_OUTOFMEMORY:
         TRACE("Ran out of memory.\n");
         break;
      case E_INVALIDARG:
         TRACE("One or more arguments are invalid.\n");
         break;
      case E_NOINTERFACE:
         TRACE("No such interface supported.\n");
         break;
      case E_POINTER:
         TRACE("Invalid pointer.\n");
         break;
      case E_HANDLE:
         TRACE("Invalid handle.\n");
         break;
      case E_ABORT:
         TRACE("Operation aborted.\n");
         break;
      case E_FAIL:
         TRACE("Unspecified error.\n");
         break;
      case E_ACCESSDENIED:
         TRACE("General access denied error.\n");
         break;
      case CO_E_OBJNOTCONNECTED:
         TRACE("Handler created but object application is not running.\n");
         break;
      case REGDB_E_IIDNOTREG:
         TRACE("Object application is running, but riid cannot be remoted because no information exists in the registration database to indicate how it is to be done.\n");
         break;
      case CO_E_NOTINITIALIZED:
         TRACE("CoInitialize has not been called and must be.\n");
         break;
      case CO_E_ALREADYINITIALIZED:
         TRACE("CoInitialize has already been called and cannot be called again (temporary).\n");
         break;
      case CO_E_CANTDETERMINECLASS:
         TRACE("can't determine clsid (e.g., extension not in reg.dat).\n");
         break;
      case CO_E_CLASSSTRING:
         TRACE("the string form of the clsid is invalid (including ole1 classes).\n");
         break;
      case CO_E_IIDSTRING:
         TRACE("the string form of the iid is invalid.\n");
         break;
      case CO_E_APPNOTFOUND:
         TRACE("application not found.\n");
         break;
      case CO_E_APPSINGLEUSE:
         TRACE("application cannot be run more than once.\n");
         break;
      case CO_E_ERRORINAPP:
         TRACE("some error in the app program file.\n");
         break;
      case CO_E_DLLNOTFOUND:
         TRACE("dll not found.\n");
         break;
      case CO_E_ERRORINDLL:
         TRACE("some error in the dll file.\n");
         break;
      case CO_E_WRONGOSFORAPP:
         TRACE("app written for other version of OS or other OS altogether.\n");
         break;
      case CO_E_OBJNOTREG:
         TRACE("object is not registered.\n");
         break;
      case CO_E_OBJISREG:
         TRACE("object is already registered.\n");
         break;
      case CO_E_APPDIDNTREG:
         TRACE("app was launched, but didn't registered a class factory.\n");
         break;
      case CO_E_INIT_TLS:
         TRACE("Thread local storage failure.\n");
         break;
      case CO_E_INIT_SHARED_ALLOCATOR:
         TRACE("Get shared memory allocator failure.\n");
         break;
      case CO_E_INIT_MEMORY_ALLOCATOR:
         TRACE("Get memory allocator failure.\n");
         break;
      case CO_E_INIT_CLASS_CACHE:
         TRACE("Unable to initialize class cache.\n");
         break;
      case CO_E_INIT_RPC_CHANNEL:
         TRACE("Unable to initialize RPC services.\n");
         break;
      case CO_E_INIT_TLS_SET_CHANNEL_CONTROL:
         TRACE("Cannot set thread local storage channel control.\n");
         break;
      case CO_E_INIT_TLS_CHANNEL_CONTROL:
         TRACE("Could not allocate thread local storage channel control.\n");
         break;
      case CO_E_INIT_UNACCEPTED_USER_ALLOCATOR:
         TRACE("The user supplied memory allocator is unacceptable.\n");
         break;
      case CO_E_INIT_SCM_MUTEX_EXISTS:
         TRACE("The OLE service mutex already exists.\n");
         break;
      case CO_E_INIT_SCM_FILE_MAPPING_EXISTS:
         TRACE("The OLE service file mapping already exists.\n");
         break;
      case CO_E_INIT_SCM_MAP_VIEW_OF_FILE:
         TRACE("Unable to map view of file for OLE service.\n");
         break;
      case CO_E_INIT_SCM_EXEC_FAILURE:
         TRACE("Failure attempting to launch OLE service.\n");
         break;
      case CO_E_INIT_ONLY_SINGLE_THREADED:
         TRACE("There was an attempt to call CoInitialize a second time while single threaded.\n");
         break;
   }
   return SCode;
}

//===============================================================================================
// FUNCTION: OLE_CheckHResult
// PURPOSE:  Tests the passed HRESULT value, returning TRUE if success.
//           If error dumps the HRESULT code to the TRACE context.
// NOTES:    This function is not usually called directly. It is part of the 
//           code expansion for the OLE_SUCCEEDED and OLE_FAILED macros.
//
BOOL OLE_CheckHResult(HRESULT hResult, LPCSTR pszFile, int nLine)
{
   if (SUCCEEDED(hResult))
      return TRUE;
      
   OLE_ShowHResult(hResult, pszFile, nLine);
   return FALSE;
}

