//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  RepeatButton.cpp
// PURPOSE: 
// AUTHOR:  NCF Oct 2001
//

#include "wincpp.hpp"
#include "RepeatButton.hpp"

//===============================================================================================
// constants

const int c_nTimerID           = 333;
const int c_uSlowFastThreshold = 2000;  // two second wait to accelerate rate.
const int c_uFirstRepeat       = 200;   // 0.2 second wait to accelerate rate.
const int c_uFastMsgPeriod     = 50;

//===============================================================================================
// MFC message map

BEGIN_MESSAGE_MAP( CRepeatButton, CButton )
	//{{AFX_MSG_MAP( CRepeatButton )
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===============================================================================================
// FUNCTION: IsButtonDown
// PURPOSE:  Returns TRUE if the left mouse button is down.
//
inline BOOL IsButtonDown()
{
   return ( GetAsyncKeyState(VK_LBUTTON) < 0 );
}

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CRepeatButton::CRepeatButton()
   : RepeatButton_BASE()
{
   ASSERT_VALID(this);

   m_uStartTickCount = 0;
   m_uNextMsg        = 0;
   m_uMsgDelta       = c_uFastMsgPeriod;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CRepeatButton::~CRepeatButton()
{
   ASSERT_VALID(this);
}

//=============================================================================================
// Method Name:   OnLButtonDown
// Purpose:       Process the left-button down message.
// Params:        (Standard event processor arguments.)
// Returns:       (Standard event processor returns.)
//
void CRepeatButton::OnLButtonDown( UINT nFlags, CPoint point )
{
   ASSERT_VALID(this);

   RepeatButton_BASE::OnLButtonDown(nFlags, point);

   // Send a command to the parent.
   SendCommand();

   // If the mouse button is not down, do not start the timer etc.
   if ( !IsButtonDown() )
      return;

   // Set a timer to keep giving us messages even though the mouse doesn't move
   SetTimer(c_nTimerID, c_uFastMsgPeriod, NULL);
   m_uStartTickCount = GetTickCount();
   m_uNextMsg        = c_uFirstRepeat;
   m_uMsgDelta       = c_uFastMsgPeriod;

   return;
}

//=============================================================================================
// Method Name:   OnLButtonUp
// Purpose:       Process the left-button down message.
// Params:        (Standard event processor arguments.)
// Returns:       (Standard event processor returns.)
//
void CRepeatButton::OnLButtonUp( UINT nFlags, CPoint point )
{
   ASSERT_VALID(this);

   RepeatButton_BASE::OnLButtonUp(nFlags, point);

   EndTracking();
   return;
}

//===============================================================================================
// FUNCTION: EndTracking
// PURPOSE:  Terminates the button repeat processing.
//
void CRepeatButton::EndTracking()
{
   ASSERT_VALID(this);

   KillTimer(c_nTimerID);
   m_uNextMsg = 0;
}

//===============================================================================================
// FUNCTION: OnMouseMove
// PURPOSE:  Processes the mouse move message.
//
void CRepeatButton::OnMouseMove(UINT nFlags, CPoint point)
{
   ASSERT_VALID(this);

   // Make sure the user still has the mouse button down
   if ( !IsButtonDown() )
      EndTracking();

   // Don't call base class so that repeat continues even if finger is off button.
   return;
}

//===============================================================================================
// FUNCTION: OnTimer
// PURPOSE:  Called on each timer tick.
//
void CRepeatButton::OnTimer( UINT nIDEvent ) 
{
   ASSERT_VALID(this);

   if( nIDEvent==c_nTimerID )
   {
      // Make sure the user still has the mouse button down
      if ( !IsButtonDown() )
      {
         EndTracking();
         return;
      }
         
      UINT uTickCount = GetTickCount() - m_uStartTickCount;

      // If not time for next message return FALSE.
      if (uTickCount < m_uNextMsg)
         return;

      // If we have passed the threshold for accelerating the repetition rate, boost the delta.
      if (uTickCount > c_uSlowFastThreshold)
         m_uMsgDelta = c_uFastMsgPeriod;

      // Step the "next message" count on until it is in the future...
      while (m_uNextMsg < uTickCount)
         m_uNextMsg += m_uMsgDelta;

      // send a command out to the parent.
      SendCommand();

      return;
   }

   // Unhandled timer events.
   CWnd::OnTimer(nIDEvent);
}

//===============================================================================================
// FUNCTION: SendCommand
// PURPOSE:  Send a registered message to the parent window.
//
void CRepeatButton::SendCommand()
{
   ASSERT_VALID(this);

   ::SendMessage( GetParent()->GetSafeHwnd(),
                  WM_COMMAND,
                  MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED),
                  (LPARAM)m_hWnd  );
}

