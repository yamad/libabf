//****************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// MODULE:  MFCEXCPT.CPP
// PURPOSE: Contains class implementations for CMFCSystemException and CMFCSystemState.
// AUTHOR:  BHI Mar 1996
//
#include "wincpp.hpp"
#include "..\common\mfcexcpt.hpp"

// MFC macro to do the dynamic object creation hocus pocus.
//IMPLEMENT_DYNAMIC(CMFCSystemException, CException);

//=======================================================================================
// FUNCTION: Constructor
// PURPOSE:  Creates a CMFCSystemException object from a SE exception code.
//
CMFCSystemException::CMFCSystemException(UINT uExceptionCode, BOOL bAutoDelete /*=TRUE*/) 
   : CException(bAutoDelete)
{
   ASSERT_VALID(this);
   m_uExceptionCode = uExceptionCode;
   TRACE1( "CMFCSystemException thrown (code=0x%X)\n", uExceptionCode );
   TRACE1( "%s\n", GetExceptionText() );
}

//=======================================================================================
// FUNCTION: Destructor
// PURPOSE:  Virtual destructor to ensure correct object cleanup and deletion.
//
CMFCSystemException::~CMFCSystemException() 
{
   ASSERT_VALID(this);
}

//=======================================================================================
// FUNCTION: GetExceptionCode
// PURPOSE:  Accessor function to retrieve the SE exception code from the object.
//
UINT CMFCSystemException::GetExceptionCode() const
{
   ASSERT_VALID(this);
   return m_uExceptionCode; 
}

//=======================================================================================
// FUNCTION: TranslationFunction
// PURPOSE:  This static function should be registered as a translation function to enable
//           SE exceptions to be translated into CMFCSystemExceptions.
//
void CMFCSystemException::TranslationFunction( UINT u, _EXCEPTION_POINTERS* /* pExp */ )
{
   // Clear the current FPU status flags to put the FPU into an exception-ready state.
   _fpreset();
   _clearfp();

   throw new CMFCSystemException( u );
}

//=======================================================================================
// FUNCTION: GetErrorMessage
// PURPOSE:  Returns a descriptive string for the exception.
// NOTES:    It would be nice if this could return strings from a string resource, but
//           that becomes problematic when this module is included in a static library.
//           Thus these messages should be used for debugging only. Any app or component
//           catching a CMFCSystemException should do its own exception-code to error msg
//           mapping.
//
BOOL CMFCSystemException::GetErrorMessage(LPTSTR lpszError, UINT uMaxError, PUINT puHelpContext)
{
   ASSERT_VALID(this);
   // Check for sensible parameters.
	ASSERT(lpszError != NULL && AfxIsValidString(lpszError, uMaxError));

   // This was copied from the implementation of other MFC exception classes.
   if (puHelpContext != NULL)
		*puHelpContext = 0;

   // Early return if we did not get sensible arguements in a release build.
   if (!lpszError || !uMaxError)
      return FALSE;

   // Return as much of the string as will fit in the user's buffer.
   strncpy(lpszError, GetExceptionText(), uMaxError-1);
   lpszError[uMaxError-1] = '\0';
	return TRUE;
}


//=======================================================================================
// FUNCTION: GetExceptionText
// PURPOSE:  Internal function that maps static strings to SE exception codes.
// NOTES:    It would be nice if this could return strings from a string resource, but
//           that becomes problematic when this module is included in a static library.
//           Thus these messages should be used for debugging only. Any app or component
//           catching a CMFCSystemException should do its own exception-code to error msg
//           mapping.
//
LPCSTR CMFCSystemException::GetExceptionText() const
{
   ASSERT_VALID(this);
   switch (m_uExceptionCode)
   {
      case EXCEPTION_ACCESS_VIOLATION:
	      return "The thread attempted to read from or write to a virtual address for which "
                "it does not have the appropriate access.";
      case EXCEPTION_BREAKPOINT:
	      return "A breakpoint was encountered.";
      case EXCEPTION_DATATYPE_MISALIGNMENT:
	      return "The thread attempted to read or write data that is misaligned on hardware "
                "that does not provide alignment. For example, 16-bit values must be aligned "
                "on 2-byte boundaries, 32-bit values on 4-byte boundaries, and so on.";
      case EXCEPTION_SINGLE_STEP:
	      return "A trace trap or other single-instruction mechanism signaled that one "
                "instruction has been executed.";
      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
	      return "The thread attempted to access an array element that is out of bounds, and "
                "the underlying hardware supports bounds checking.";
      case EXCEPTION_FLT_DENORMAL_OPERAND:
	      return "One of the operands in a floating-point operation is denormal. A denormal "
                "value is one that is too small to represent as a standard floating-point value.";
      case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	      return "The thread attempted to divide a floating-point value by a floating-point divisor of zero.";
      case EXCEPTION_FLT_INEXACT_RESULT:
	      return "The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
      case EXCEPTION_FLT_INVALID_OPERATION:
	      return "This exception represents any floating-point exception not included in this list.";
      case EXCEPTION_FLT_OVERFLOW:
	      return "The exponent of a floating-point operation is greater than the magnitude allowed "
                "by the corresponding type.";
      case EXCEPTION_FLT_STACK_CHECK:
	      return "The stack overflowed or underflowed as the result of a floating-point operation.";
      case EXCEPTION_FLT_UNDERFLOW:
	      return "The exponent of a floating-point operation is less than the magnitude allowed "
                "by the corresponding type.";
      case EXCEPTION_INT_DIVIDE_BY_ZERO:
	      return "The thread attempted to divide an integer value by an integer divisor of zero.";
      case EXCEPTION_INT_OVERFLOW:
	      return "The result of an integer operation caused a carry out of the most significant bit of the result.";
      case EXCEPTION_PRIV_INSTRUCTION:
	      return "The thread attempted to execute an instruction whose operation is not allowed in the current machine mode.";
      case EXCEPTION_NONCONTINUABLE_EXCEPTION:
	      return "The thread attempted to continue execution after a noncontinuable exception occurred.";
   }

   // Unknown exception received. Check the exception #defines in winbase.h and the books online.
   static char szBuf[80];
   sprintf(szBuf, "Unknown exception code: 0x%X.", m_uExceptionCode);
   TRACE1("%s\n", szBuf);
   ASSERT(FALSE);
   return szBuf;
}


// MFC debugging functions.
#ifdef _DEBUG

void CMFCSystemException::AssertValid() const
{
   // Nothing to add as yet...
   CException::AssertValid();
}

void CMFCSystemException::Dump( CDumpContext& dc ) const
{
   // Dump the exception code and then the base class.
   static char szBuf[80];
   sprintf(szBuf, "Exception code: 0x%X.", m_uExceptionCode);
   dc << szBuf;
   CException::Dump(dc);
}

#endif

//#######################################################################################
//#######################################################################################
//###
//###  Implementation of CMFCSystemState.
//###
//#######################################################################################
//#######################################################################################


//=======================================================================================
// FUNCTION: ControlFPU
// PURPOSE:  Changes and returns the current FPU word.
// NOTES:    It was found that the _controlfp function did not work identically to the 
//           _control87 function, some flags were not returned correctly. 
//           So the _control87 function is preferable on x86/x87 class chips.
//
inline UINT ControlFPU(UINT uNew, UINT uMask)
{
#ifdef _M_IX86
   return _control87(uNew, uMask);
#else
   return _controlfp(uNew, uMask);
#endif
}

//=======================================================================================
// FUNCTION: Constructor
// PURPOSE:  Sets up the system state to enable certain floating point exceptions.
//
CMFCSystemState::CMFCSystemState(UINT uFlags)
{
   ASSERT_VALID(this);

   // Clear the current FPU status flags to put the FPU into an exception-ready state.
   _clearfp();

   // Clear all control flags passed in as a parameter.
   m_uControlFlags = ControlFPU(0, 0);
   ControlFPU(~uFlags, _MCW_EM );

   // Set the SE translation function that will translate SE exceptions into C++ exceptions.
   m_pfn = _set_se_translator( CMFCSystemException::TranslationFunction );
}


//=======================================================================================
// FUNCTION: Destructor
// PURPOSE:  Restores the system state to the way it was before this object was created.
//
CMFCSystemState::~CMFCSystemState()
{
   ASSERT_VALID(this);

   // Reinstate the previous translation function.
   _set_se_translator( m_pfn );

   // reinstate the previous FPU control flags.
   ControlFPU( m_uControlFlags, UINT(-1) );

   // Clear the error bits from the FPU status word.
   _clearfp();
}

// MFC debugging functions.
#ifdef _DEBUG

void CMFCSystemState::AssertValid() const
{
   // Nothing to add as yet...
   CObject::AssertValid();
}

void CMFCSystemState::Dump( CDumpContext& dc ) const
{
   // Dump the exception code and then the base class.
   static char szBuf[80];
   sprintf(szBuf, "Saved SE translator: 0x%X.", m_pfn);
   dc << szBuf;
   sprintf(szBuf, "Saved FPU control flags: 0x%X.", m_uControlFlags);
   dc << szBuf;
   CObject::Dump(dc);
}

#endif
