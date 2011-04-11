//****************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// HEADER:  EXCEPTN.HPP
// PURPOSE: Contains class definitions and macros for catching floating point exceptions.
// AUTHOR:  BHI Mar 1996
//

#ifndef __CSystemException_hpp__
#define __CSystemException_hpp__

#include <eh.h>
#include <float.h>

//#######################################################################################
// MACROS: These macros make it cleaner to specify Structured Exception handling
// 
#define SE_TRY    __try
#define SE_ENDTRY __except(_clearfp(), EXCEPTION_CONTINUE_SEARCH) { }

//#######################################################################################
// CLASS:   CSystemException
// PURPOSE: This class is used to catch structured exceptions that cannot usually
//          be caught with C++ exception handling. This may be a temporary state of affairs
//          while Microsoft gets its act together.
//
class CSystemException
{
private:
   UINT m_uExceptionCode;     // The system code for the exception.

private:    // Unimplemented default constructors
   CSystemException();

protected:
   CSystemException(UINT uExceptionCode);
   static LPCSTR _GetExceptionText(UINT uExceptionCode);

public:
   // Allow default copy constructor.
   // CSystemException( const CSystemException& );

   UINT GetExceptionCode();      // Returns the SE exception code.

   static void TranslationFunction( UINT uExceptionCode, _EXCEPTION_POINTERS *pExp);

   virtual UINT GetErrorMessage(LPSTR buf, UINT uBufSize) const;
};


//#######################################################################################
// CLASS:   CSystemState
// PURPOSE: This class is used to setup the SE environment to catch floating point exceptions
//          and translate them into CSystemExceptions.
//
class CSystemState
{
private:
   UINT                    m_uControlFlags;
   _se_translator_function m_pfn;

private:    // Unimplemented copy constructor and operator=()
   CSystemState(const CSystemState &);
   const CSystemState &operator=(const CSystemState &);

public:
   CSystemState(UINT uFlags = _EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW);
   ~CSystemState();
};

#endif   // __CSystemException_hpp__
