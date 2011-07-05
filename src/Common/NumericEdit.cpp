//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  NumericEdit.cpp
// PURPOSE: 
// AUTHOR:  NCF Nov 2001
//

#include "wincpp.hpp"
#include "NumericEdit.hpp"
#include "\AxonDev\Comp\AxonValidation\AxonValidation.h"

//===============================================================================================
// MFC message map
//
BEGIN_MESSAGE_MAP( CNumericEdit, NumericEdit_BASE )
	//{{AFX_MSG_MAP( CNumericEdit )
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===============================================================================================
// FUNCTION: OnChar
// PURPOSE:  .
//
void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString Holder = "`~!@#$%^&*()_+|=\\qwertyuiop[]asdfghjkl;'zxcvbnm,/QWERTYUIOP{}ASDFGHJKL:/ZXCVBNM<>?/";

	// 1st pass on unwanted characters
	if (Holder.Find(nChar) != -1)
		return;

	// length, selection info
	int num_typed = NumericEdit_BASE::GetWindowTextLength();
	int start_char, end_char;
	GetSel(start_char, end_char);

	// current CEdit text
	CString temp_str;
	GetWindowText(temp_str);

	// state variables
	bool  bSignTyped   = false;
	bool  bPeriodTyped = false;
	TCHAR temp_char;

	// most of the work done in here..
	for (int i=0; i<temp_str.GetLength(); i++) 
	{
		// selected chars don't count..
		if ((i>=start_char) && (i<end_char))
			continue;

		temp_char = temp_str[i];

		if ((temp_char == '+') || (temp_char == '-')) {
			bSignTyped = true;
		} else if (temp_char == '.') {
			bPeriodTyped = true;
		}
	}

	// allow sign only once (first char)
	if ((nChar == '+') || (nChar == '-')) {
		if (bSignTyped || (start_char != 0))
			return;
	}

	// allow period only once
	if ((nChar == '.') && bPeriodTyped)
		return;


	NumericEdit_BASE::OnChar(nChar, nRepCnt, nFlags);
}

//===============================================================================================
// FUNCTION: GetWindowDouble
// PURPOSE:  Get window text as double value.
//
double CNumericEdit::GetWindowDouble() const
{
   MEMBERASSERT();

   char szBuffer[VAL_EXTERNBUFSIZE];
   NumericEdit_BASE::GetWindowText(szBuffer, VAL_EXTERNBUFSIZE);
   
   double dDoubleValue = 0.0;
   if( !szBuffer || szBuffer[0]=='\0' )
   {
      return dDoubleValue;
   }

   int nError = 0;
   
   VAL_FormatStrToDouble(szBuffer, VAL_EXTERNBUFSIZE, &dDoubleValue,
                         VAL_FMT_SIG_DIGITS, 0, FLT_DIG, &nError); 

   return dDoubleValue;
}
