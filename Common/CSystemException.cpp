//****************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// MODULE:  CSystemException.CPP
// PURPOSE: Contains class implementations for CSystemException and CSystemState.
// AUTHOR:  BHI Mar 1996
//
#include "wincpp.hpp"
#include "..\common\CSystemException.hpp"

//=======================================================================================
// FUNCTION: Constructor
// PURPOSE:  Creates a CSystemException object from a SE exception code.
//
CSystemException::CSystemException(UINT uExceptionCode) 
   : m_uExceptionCode(uExceptionCode)
{
   TRACE1( "CSystemException thrown (code=0x%X)\n", uExceptionCode );
   TRACE1("%s\n", CSystemException::_GetExceptionText(uExceptionCode));
}

//=======================================================================================
// FUNCTION: GetExceptionCode
// PURPOSE:  Accessor function to retrieve the exception code from the object.
//
UINT CSystemException::GetExceptionCode()
{
   return m_uExceptionCode; 
}

//=======================================================================================
// FUNCTION: TranslationFunction
// PURPOSE:  This static function should be registered as a translation function to enable
//           SE exceptions to be translated into CSystemExceptions.
//
void CSystemException::TranslationFunction( UINT u, _EXCEPTION_POINTERS* /* pExp */ )
{
   // Clear the current FPU status flags to put the FPU into an exception-ready state.
   _fpreset();
   _clearfp();

   throw CSystemException( u );
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
UINT CSystemException::GetErrorMessage(LPTSTR pszError, UINT uMaxError) const
{
   MEMBERASSERT();

   // Check for sensible parameters.
	LPSZASSERT(pszError);

   // Early return if we did not get sensible arguements in a release build.
   if (!pszError || !uMaxError)
      return 0;

   // Return as much of the string as will fit in the user's buffer.
   strncpy(pszError, _GetExceptionText(m_uExceptionCode), uMaxError-1);
   pszError[uMaxError-1] = '\0';
	return strlen(pszError);
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
LPCSTR CSystemException::_GetExceptionText(UINT uExceptionCode)
{
   switch (uExceptionCode)
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
   sprintf(szBuf, "Unknown exception code: 0x%X.", uExceptionCode);
   TRACE1("%s\n", szBuf);
   ASSERT(FALSE);
   return szBuf;
}


//#######################################################################################
//#######################################################################################
//###
//###  Implementation of CSystemState.
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
CSystemState::CSystemState(UINT uFlags)
{
   MEMBERASSERT();

   // Clear the current FPU status flags to put the FPU into an exception-ready state.
   _clearfp();

   // Clear all control flags passed in as a parameter.
   m_uControlFlags = ControlFPU(0, 0);
   ControlFPU(~uFlags, _MCW_EM );

   // Set the SE translation function that will translate SE exceptions into C++ exceptions.
   m_pfn = _set_se_translator( CSystemException::TranslationFunction );
}

//=======================================================================================
// FUNCTION: Destructor
// PURPOSE:  Restores the system state to the way it was before this object was created.
//
CSystemState::~CSystemState()
{
   MEMBERASSERT();

   // Reinstate the previous translation function.
   _set_se_translator( m_pfn );

   // reinstate the previous FPU control flags.
   ControlFPU( m_uControlFlags, UINT(-1) );

   // Clear the error bits from the FPU status word.
   _clearfp();
}


//#################################################################################
//#################################################################################
//###
//###  TESTBED / Example code.
//###
//###  To compile the testbed code:
//###  cl -GX -D__TESTBED__ CSystemException.cpp
//###
//#################################################################################
//#################################################################################

//#define __TESTBED__
#ifdef __TESTBED__

//=================================================================================
// Usage notes:
//
// 1) Code to be protected against FPU exceptions should be bracketed with the
//    SE_TRY / SE_ENDTRY macros (see the following example code).
// 2) Place the SE_TRY/SE_ENDTRY bracketing as close as possible to where the 
//    FPU exception will occur.
// 3) At a higher level create a CSystemState object to set up the system to throw
//    SEH exceptions then a FPU exception occurs.
// 4) It is reccommended that the CSystemState is contained within the C++ exception
//    handling "try" block that brackets the code that may provoke a FPU exception.
//    This ensures that the FPU state is reset when the exception is caught.
// 5) Code bracketed by SE_TRY / SE_ENDTRY cannot be in the same function as the 
//    C++ exception handling code (try/catch).
// 6) It is important that C++ objects are not created/destroyed within the code 
//    bracketed by the SE_TRY / SE_ENDTRY macros. C++ stack unwinding is not done 
//    by SEH exceptions.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//=================================================================================
// FUNCTION: DivideByZero
// PURPOSE:  Causes a FPU divide by zero exception.
// NOTES:    If the printf statement is executed the SEH handling was not setup correctly.
//
static void DivideByZero()
{
   printf("\nDivideByZero\n");
   SE_TRY
   {
      float f1=0.0F;
      float f2 = 5 / f1;        // This exception handled by outer block
      printf("Should not get here!  f2 = %g\n", f2);
   }
   SE_ENDTRY
}

//=================================================================================
// FUNCTION: Overflow
// PURPOSE:  Causes a FPU overflow exception.
// NOTES:    If the printf statement is executed the SEH handling was not setup correctly.
//
static void Overflow()
{
   printf("\nOverflow\n");
   SE_TRY
   {
      float f1 = 1E20F, f2=1E20F;
      float f3 = f1 * f2;
      printf("Should not get here!  f3 = %g\n", f3);
   }
   SE_ENDTRY
}

//=================================================================================
// FUNCTION: SqrtNegOne
// PURPOSE:  Causes an exception within the C runtime library.
// NOTES:    If the printf statement is executed the SEH handling was not setup correctly.
//
static void SqrtNegOne()
{
   printf("\nSqrtNegOne\n");
   SE_TRY
   {
      float f1 = -1.0F;
      float f2 = (float)sqrt(f1);
      printf("f2 = %g\n", f2);
   }
   SE_ENDTRY
}

//=================================================================================
// FUNCTION: ShowExceptionCode
// PURPOSE:  Prints descriptive information about the passed exception object.
// NOTES:    The call to GetExceptionCode() should be used for debugging only.
//           Any implimentation should map exception codes into application specific
//           error strings that inform the user why the exception occured, and possible
//           ways to avoid them in future.
//
static void ShowExceptionCode(CSystemException &e)
{
   char szMsg[256];
   e.GetErrorMessage(szMsg, sizeof(szMsg));
   UINT uCode = e.GetExceptionCode();

   printf( "Caught a CSystemException.\nExceptionCode = 0x%X\n%s\n", uCode, szMsg );
}

//=================================================================================
// FUNCTION: main
// PURPOSE:  Entrypoint for the testbed code.
//
int main()
{
   try 
   {
      CSystemState State;
      DivideByZero();
   }
   catch( CSystemException e )
   {
      ShowExceptionCode(e);
   }

   try 
   {
      CSystemState State;
      Overflow();
   }
   catch( CSystemException e )
   {
      ShowExceptionCode(e);
   }

   try 
   {
      CSystemState State;
      SqrtNegOne();
   }
   catch( CSystemException e )
   {
      ShowExceptionCode(e);
   }

   return 0;
}

 
#endif
