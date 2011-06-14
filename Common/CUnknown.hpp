//***********************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CUNKNOWN.HPP
// PURPOSE: Contains a declaration for a wrapper base class for COM objects.
// AUTHOR:  BHI  Jun 1996
//
#ifndef INC_CUNKNOWN_HPP
#define INC_CUNKNOWN_HPP

#pragma once

#include "oledebug.h"   // Contains OLE #includes and debugging macros/prototypes.

//##########################################################################################
//##########################################################################################
//###
//### CLASS:   CUnknown
//###
//##########################################################################################
//##########################################################################################

class CUnknown
{
protected:
   PUNKNOWN m_pInterface;     // The interface wrapped by this object.
   HRESULT  m_hrLastError;    // The error code returned by the last interface call that failed.

private: // Prevent the use of the copy constructor and the operator=.
   CUnknown(const CUnknown &);
   const CUnknown &operator=(const CUnknown &);
   
private:   // Internal (implementation) functions.
   BOOL _QueryInterface(REFIID riid, LPVOID FAR *ppvObj);

protected:
   // Convenience function for setting an error code and returning FALSE.
   BOOL _SetLastError(HRESULT hr);
   
public:
   // Constructor/destructor
   CUnknown(PUNKNOWN pInterface = NULL);
   virtual ~CUnknown();

   // Release any existing interface and set a new one (maybe NULL).      
   void SetInterface(PUNKNOWN pInterface);
   
   // Return the held interface - NULL if nothing held.
   PUNKNOWN GetInterface() const;
   
   // Remove the interface from this object, holding onto the reference count.
   PUNKNOWN RemoveInterface();
   
   // Query for another interface.
   BOOL QueryInterface(REFIID riid, CUnknown *pUnknown);
   
   // Get the result code of the last interface call that failed.
   HRESULT GetLastError();

#ifdef _DEBUG  // Diagnostic routines.
   virtual BOOL CheckHResult(HRESULT hResult, LPCSTR pszFile, int nLine);
   virtual BOOL Dump();
#else
   BOOL Dump();
#endif
};

#ifndef _DEBUG // Diagnostic routines -- minimal if not _DEBUG build.
//===============================================================================================
// FUNCTION: Dump
// PURPOSE:  Dumps the current state of the object to the trace context.
//
inline BOOL CUnknown::Dump()
{
   return (m_pInterface!=NULL);
}
#endif         // _DEBUG

#endif         // INC_CUNKNOWN_HPP
