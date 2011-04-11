//**********************************************************************************************
//
//    Copyright (c) 1994-1997 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// MODULE:  ACQTIMER.CPP
// PURPOSE: Contains timer callback function and member functions for the CAcquisition class.
// AUTHOR:  BHI  Mar 1994
//
#include "wincpp.hpp"

#pragma warning(disable:4201)
#include <mmsystem.h>

#include "AcqTimer.hpp"

const UINT c_uMAX_MM_PERIOD = 110;

// Uncomment the following line to use a callback proc for the windows timer.
// #define USE_WIN_CALLBACK   1

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CAcqTimer::CAcqTimer()
{
   MEMBERASSERT();
   m_uTimerID        = 0;
   m_uActualID       = 0;
   m_uTimerPeriod    = 0;
   m_hwndParent      = NULL;
   m_bMMTimer        = FALSE;
   m_bMessagePending = FALSE;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CAcqTimer::~CAcqTimer()
{
   MEMBERASSERT();
   Stop();
}

//===============================================================================================
// FUNCTION: Start
// PURPOSE:  Starts either a Windows timer or a Multimedia Timer.
//
BOOL CAcqTimer::Start(HWND hwndParent, UINT uTimerID, UINT uPeriod, BOOL bMMTimer)
{
   MEMBERASSERT();
   ASSERT(m_uTimerID==0);
   ASSERT(uTimerID != 0);
   HWNDASSERT(hwndParent);

   m_hwndParent   = hwndParent;
   m_uTimerPeriod = uPeriod;
   m_uTimerID     = uTimerID;
   m_uActualID    = 0;
   m_Timer.Start();
   return bMMTimer ? StartMMTimer() : StartWinTimer();
}


//===============================================================================================
// FUNCTION: Stop
// PURPOSE:  Stops the timer created by Start().
//
void CAcqTimer::Stop()
{
   MEMBERASSERT();
   if (!m_uTimerID)
      return;

   if (m_bMMTimer)
      StopMMTimer();
   else
      StopWinTimer();

   m_bMMTimer  = FALSE;
   m_uTimerID  = 0;
   m_uActualID = 0;
}


//################################################################################################
//################################################################################################
//###
//###    Windows Timer implementation functions.
//###
//################################################################################################
//################################################################################################


//===============================================================================================
// FUNCTION: StartWinTimer
// PURPOSE:  Starts a Windows timer.
//
BOOL CAcqTimer::StartWinTimer()
{
   MEMBERASSERT();

   m_bMMTimer = FALSE;

#ifdef USE_WIN_CALLBACK
   BOOL bRval = SetTimer(m_hwndParent, m_uTimerID, m_uTimerPeriod, WinTimerProc);
#else
   BOOL bRval = SetTimer(m_hwndParent, m_uTimerID, m_uTimerPeriod, NULL);
#endif

   if (!bRval)
   {
      m_uTimerID = 0;
      return FALSE;
   }
   m_uActualID = m_uTimerID;
   return TRUE;
}

//===============================================================================================
// FUNCTION: StopWinTimer
// PURPOSE:  Stops the Windows timer.
//
void CAcqTimer::StopWinTimer()
{
   MEMBERASSERT();
   ASSERT(m_uActualID);
   ASSERT(!m_bMMTimer);
   KillTimer(m_hwndParent, m_uActualID);
}


//===============================================================================================
// FUNCTION: WinTimerProc
// PURPOSE:  Static callback function called by the Windows timer.
//
void CALLBACK CAcqTimer::WinTimerProc(HWND hWnd, UINT /*uMsg*/, UINT uTimerID, DWORD /*dwTime*/)
{
   HWNDASSERT(hWnd);
   FORWARD_WM_TIMER(hWnd, uTimerID, SendMessage);
}


//################################################################################################
//################################################################################################
//###
//###    Multimedia Timer implementation functions.
//###
//################################################################################################
//################################################################################################


//===============================================================================================
// FUNCTION: ClipPeriod
// PURPOSE:  Clips the requested period for the timer to the supported range.
//
static BOOL ClipPeriod(UINT &uPeriod)
{
   TIMECAPS tc;
   MMRESULT mmResult = timeGetDevCaps(&tc, sizeof(tc));
   if (mmResult != TIMERR_NOERROR)
      return FALSE;

   uPeriod = min(uPeriod, tc.wPeriodMax);
   uPeriod = max(uPeriod, tc.wPeriodMin);
   return TRUE;
}

//===============================================================================================
// FUNCTION: StartMMTimer
// PURPOSE:  Starts a Multimedia timer.
//
BOOL CAcqTimer::StartMMTimer()
{
   MEMBERASSERT();
   m_bMessagePending = FALSE;

   // If it is pretty large, use the standard windows timer.
   if (m_uTimerPeriod > c_uMAX_MM_PERIOD)
      return StartWinTimer();

   // Clip the period to the allowable range for MM timers.
   UINT uPeriod = m_uTimerPeriod;
   if (!ClipPeriod(uPeriod))
      return StartWinTimer();

   // Calculate the accuracy to be used and clip it to range also.
   UINT uAccuracy = uPeriod/2;
   if (!ClipPeriod(uAccuracy))
      return StartWinTimer();

   // Step up the MM timer accuracy if necessary.
   timeBeginPeriod(uAccuracy);

   // Start the MM timer.
   m_bMMTimer  = TRUE;
   m_uActualID = timeSetEvent(uPeriod, uAccuracy, &MMTimerProc,
                              DWORD(this), TIME_PERIODIC);
   if (m_uActualID==0)
   {
      // Restore the MM timer accuracy and give the base class version a try.
      m_bMMTimer = FALSE;
      timeEndPeriod(uAccuracy);

      TRACE("Couldn't get multimedia timer -- using Windows timer instead\n");
      return StartWinTimer();
   }

   m_uTimerPeriod = uPeriod;
   return TRUE;
}


//===============================================================================================
// FUNCTION: StopMMTimer
// PURPOSE:  Stops the Multimedia timer.
//
void CAcqTimer::StopMMTimer()
{
   MEMBERASSERT();
   ASSERT(m_uTimerID);
   ASSERT(m_bMMTimer);

   m_bMessagePending = TRUE;
   timeKillEvent(m_uActualID);

   UINT uAccuracy = m_uTimerPeriod/2;
   ClipPeriod(uAccuracy);
   timeEndPeriod(uAccuracy);
}


//===============================================================================================
// FUNCTION: MMCallback
// PURPOSE:  Callback for the multimedia timer.
//
inline void CAcqTimer::MMCallback()
{
   if (!m_bMessagePending)
   {
      m_bMessagePending = TRUE;

      // Post a WM_TIMER message to the parent window.
      // The indirect timer message is used instead of a direct call
      // because of synchronization problems caused by re-entrance.
      FORWARD_WM_TIMER(m_hwndParent, m_uTimerID, PostMessage);
   }
}

//===============================================================================================
// FUNCTION: MMTimerProc
// PURPOSE:  Callback for the multimedia timer.
//
void CALLBACK CAcqTimer::MMTimerProc(UINT uTimerID, UINT /*uMsg*/, DWORD dwUser, 
                                     DWORD /*dw1*/, DWORD /*dw2*/)
{
   ((CAcqTimer *)dwUser)->MMCallback();
}

//===============================================================================================
// FUNCTION: IsRunning
// PURPOSE:  Returns TRUE if the timer is currently running.
//
BOOL CAcqTimer::IsRunning() const
{
   MEMBERASSERT();
   return (m_uActualID != 0);
}
