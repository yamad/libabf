//***********************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  CUNKNOWN.CPP
// PURPOSE: Contains an implementation of a wrapper base class for COM objects.
// AUTHOR:  BHI  Jun 1996
//
// NOTES:
//    This module was developed as part of a methodology to make COM objects easier to work with
// in a C++ environment. Two main areas were addressed: 1) To automate the COM reference counting
// calls so that Release() would get called automatically for an interface when it was no longer 
// required; and b) to make it easier to work with interface calls by turning the HRESULT based
// interface into methods with a simple BOOL return type and an internal variable to hold the 
// error code. Removing the need to sprinkle SUCCEEDED and FAILED macro calls in the client code.
//
// Two main approaches were examined during this design process: "smart pointers" and 
// "wrapper classes". In the end I chose to go with wrapper classes as they enabled me 
// to a) add value to the COM objects where appropriate, and b) hide elements of the COM 
// interfaces (such as the Release() method) that were either obsolete or not necessary 
// for the user to work with the objects.
//
// Subsequent to the creation of this module I found that the topic had also been covered by 
// one of the Microsoft technical writers on the MSDN:
// The ariticles are:
//   "Calling COM Objects with Smart Interface Pointers," -- Dale Rogerson
//   "Calling COM Objects with interface Wrappers" -- Dale Rogerson
//
// USAGE:
// Create a new class derived from CUnknown (defined in CUnknown.hpp and implemented here)
// and add call through functions that expose those portions of the COM interface thought 
// necessary.
//
// At runtime the interface pointer can be wrapped through the creation of the CUnknown
// derived class's PUNKNOWN copy constructor, or by calling the SetInterface method on 
// an instance of the CUnknown derived class.
//
// For an example of a CUnknown derived object implementation see 
// CDrvLxEx.hpp & CDrvLxEx.cpp in the AxonDev\Comp\Axengine32 project.
//
// For usage of this derived class see the DriverLINX Extensions testbed.
//
#include "wincpp.hpp"
#include "CUnknown.hpp"

//##########################################################################################
//##########################################################################################
//###
//### CLASS:   CUnknown
//###
//##########################################################################################
//##########################################################################################

//===============================================================================================
// FUNCTION: Copy constructor
// PURPOSE:  Initializes object with an interface pointer.
//
CUnknown::CUnknown(PUNKNOWN pInterface)
{
   MEMBERASSERT();
   m_pInterface  = NULL;
   m_hrLastError = S_OK;

   SetInterface(pInterface);
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CUnknown::~CUnknown()
{
   MEMBERASSERT();
   
   // If an object is curently wrapped, release the object.
   SetInterface(NULL);
}

//===============================================================================================
// FUNCTION: SetInterface
// PURPOSE:  Sets the interface to be wrapped by the object.
// NOTES:    The held interface can be released by calling SetInterface(NULL).
//
void CUnknown::SetInterface(PUNKNOWN pInterface)
{
   MEMBERASSERT();

   // If an object is curently wrapped, release the object.
   if (m_pInterface)
   {
      WPTRASSERT(m_pInterface);
      m_pInterface->Release();
   }
      
   // Set the new interface pointer and clear the last error variable.
   m_pInterface = pInterface;   
   m_hrLastError = S_OK;
}
  
//===============================================================================================
// FUNCTION: GetInterface
// PURPOSE:  Returns the interface being wrapped without disturbing the object.
// NOTE:     This function is a blatent breach of object incapsulation, but 
//           unfortunately it is necessary as sometimes the held interface
//           must be passed as a parameter to other objects.
//
PUNKNOWN CUnknown::GetInterface() const
{
   MEMBERASSERT();
   return m_pInterface;   
}
  
//===============================================================================================
// FUNCTION: RemoveInterface
// PURPOSE:  Extracts and returns the interface being wrapped, clearing the interface pointer so
//           that the interface doesn't get released by the destructor.
//
PUNKNOWN CUnknown::RemoveInterface()
{
   MEMBERASSERT();
   PUNKNOWN pRval = m_pInterface;
   m_pInterface  = NULL;   
   m_hrLastError = S_OK;
   return pRval;   
}
  
//===============================================================================================
// FUNCTION: _QueryInterface
// PURPOSE:  Internal helper method for QueryInterface.
//
BOOL CUnknown::_QueryInterface(REFIID riid, LPVOID FAR *ppvObj)
{
   MEMBERASSERT();
   ASSERT(m_pInterface!=NULL);
   WPTRASSERT(ppvObj);

   HRESULT hr = m_pInterface->QueryInterface(riid, ppvObj);
   if (OLE_SUCCEEDED(hr))
      return TRUE;

   return _SetLastError(hr);
}
  
//===============================================================================================
// FUNCTION: QueryInterface
// PURPOSE:  This method returns a reference to the request interface, if it exists.
//
BOOL CUnknown::QueryInterface(REFIID riid, CUnknown *pUnknown)
{
   MEMBERASSERT();
   WPTRASSERT(pUnknown);
   ASSERT(m_pInterface!=NULL);
   
   // Clear the interface in the return object in case of failure.
   pUnknown->SetInterface(NULL);

   // Query for the interface.   
   void *pInterface = NULL;
   if (!_QueryInterface(riid, &pInterface))
      return FALSE;
      
   // Set the return value.      
   pUnknown->SetInterface(PUNKNOWN(pInterface));
   return TRUE;
}
  
//===============================================================================================
// FUNCTION: _SetLastError
// PURPOSE:  Convenience function to set an error code and return false.
//
BOOL CUnknown::_SetLastError(HRESULT hr)
{
   MEMBERASSERT();
   m_hrLastError = hr;
   return FALSE;
}
  
#ifdef _DEBUG
//====================================================================================
// Debug time tracing for the CUnknown object.

//===============================================================================================
// FUNCTION: Dump
// PURPOSE:  Dumps the current state of the object to the trace context.
//
BOOL CUnknown::Dump()
{
   MEMBERASSERT();
   if (!m_pInterface)
   {
      TRACE("ERROR: Dump called on NULL interface.\n");
      return FALSE;
   }
   return OLE_IS_VALID_INTERFACE(m_pInterface);
}

//===============================================================================================
// FUNCTION: CheckHResult
// PURPOSE:  Checks a return code for interface specific error conditions.
//
BOOL CUnknown::CheckHResult(HRESULT hResult, LPCSTR pszFile, int nLine)
{
   MEMBERASSERT();
   return OLE_CheckHResult(hResult, pszFile, nLine);
}

#endif


