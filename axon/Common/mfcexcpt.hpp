//****************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// HEADER:  MFCEXCPT.HPP
// PURPOSE: Contains class definitions and macros for catching floating point exceptions.
// AUTHOR:  BHI Mar 1996
//

#ifndef __MFCEXCPT_HPP__
#define __MFCEXCPT_HPP__

#include <eh.h>
#include <float.h>

//#######################################################################################
// MACROS: These macros make it cleaner to specify Structured Exception handling and 
//         provide a wrapper for the MS specific language extensions used to implement
//         structured exception handling.
// 
#define SE_TRY    __try
#define SE_ENDTRY __except(_clearfp(), EXCEPTION_CONTINUE_SEARCH) { }


//#######################################################################################
// CLASS:   CMFCSystemException
// PURPOSE: This class is used to catch structured exceptions that cannot usually
//          be caught with C++ exception handling. This may be a temporary state of affairs
//          while Microsoft gets its act together.
//
class CMFCSystemException : public CException
{
//	DECLARE_DYNAMIC(CMFCSystemException)

private:    // Attributes
   UINT m_uExceptionCode;     // The system code for the exception.

private:    // Unimplemented default constructors
   CMFCSystemException();

protected:
   CMFCSystemException(UINT uExceptionCode, BOOL bAutoDelete=TRUE);
   LPCTSTR  GetExceptionText() const;   // Returns a text description of the exception (debugging)

public:
   // Allow default copy constructor and operator=().
   // CMFCSystemException( const CMFCSystemException& );
   // const CMFCSystemException &operator=( const CMFCSystemException& );

   // Virtual destructor
	virtual ~CMFCSystemException();

   // Accessor function to return the SE exception code for the exception that was thrown.
   UINT GetExceptionCode() const;

   // Overridden base class function to return a textual description of the exception (for debugging).
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

   // Static function that is installed as a SE translator by the CMFCSystemState class.
   static void TranslationFunction( UINT uExceptionCode, _EXCEPTION_POINTERS *pExp);

   // MFC debugging functions.
#ifdef _DEBUG
   virtual void AssertValid( void ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif
};


//#######################################################################################
// CLASS:   CMFCSystemState
// PURPOSE: This class is used to setup the SE environment to catch floating point exceptions
//          and translate them into CMFCSystemExceptions.
//
class CMFCSystemState : public CObject
{
private:    // Member data.
   UINT                    m_uControlFlags;  // Place to save current FPU control flags
   _se_translator_function m_pfn;            // Place to save the existant translator fn.

private:    // Unimplemented copy constructor and operator=()
   CMFCSystemState(const CMFCSystemState &);
   const CMFCSystemState &operator=(const CMFCSystemState &);

public:
   // Constructor and destructor.
   CMFCSystemState(UINT uFlags = _EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW);
   virtual ~CMFCSystemState();

   // MFC debugging functions.
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump( CDumpContext& dc ) const;
#endif
};

#endif   // __MFCEXCPT_HPP__
