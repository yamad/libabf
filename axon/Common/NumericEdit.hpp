//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  NumericEdit.hpp
// PURPOSE: An MFC compatible edit control, for numbers.
// AUTHOR:  NCF Nov 2001
//

#ifndef INC_NUMERICEDIT_HPP
#define INC_NUMERICEDIT_HPP

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define NumericEdit_BASE CEdit

class CNumericEdit : public NumericEdit_BASE
{
private:

public:
   double GetWindowDouble() const;

protected:
	//{{AFX_MSG( CNumericEdit )
   afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif      // INC_NUMERICEDIT_HPP

