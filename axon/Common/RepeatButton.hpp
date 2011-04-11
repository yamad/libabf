//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  RepeatButton.hpp
// PURPOSE: An MFC compatible repeat button, much like the ADM_RepeatButton.
// AUTHOR:  NCF Oct 2001
//

#ifndef INC_REPEATBUTTON_HPP
#define INC_REPEATBUTTON_HPP

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define RepeatButton_BASE CButton

class CRepeatButton : public RepeatButton_BASE
{
private:    // Attributes
   UINT     m_uStartTickCount;
   UINT     m_uNextMsg;
   UINT     m_uMsgDelta;

private:   // Unimplemented constructor/copy functions.
   CRepeatButton(const CRepeatButton&);
   const CRepeatButton& operator=(const CRepeatButton&);

protected:
	//{{AFX_MSG( CRepeatButton )
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
   afx_msg void OnTimer( UINT nIDEvent );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:   // Private functions
   void EndTracking();
   void SendCommand();

public:    // Public interface
   CRepeatButton();
   virtual ~CRepeatButton();
};

#endif      // INC_REPEATBUTTON_HPP
