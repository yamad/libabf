//**********************************************************************************************
//
//    Copyright (c) 1994-1997 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// HEADER:  ACQTIMER.HPP
// PURPOSE: Definition of the CAcquisitionTimer class.
// AUTHOR:  BHI  Jun 1996
//

#ifndef INC_AcqTimer_hpp
#define INC_AcqTimer_hpp

#include "\AxonDev\Comp\common\CHighResTimer.hpp"

class CAcqTimer
{
private:
   CHighResTimer  m_Timer;             // High resolution timer object.
   UINT           m_uTimerID;          // Requested ID of timer.
   UINT           m_uActualID;         // Actual timer ID assigned by the system.
   UINT           m_uTimerPeriod;      // Requested polling period in ms.
   HWND           m_hwndParent;        // Parent window handle for receiving messages etc.
   BOOL           m_bMMTimer;          // TRUE = MultiMedia timer used.
   BOOL           m_bMessagePending;   // Semaphore for MM timers.

private:
   CAcqTimer(const CAcqTimer &);
   const CAcqTimer &operator=(const CAcqTimer &);

private:
   void MMCallback();

   BOOL StartWinTimer();
   void StopWinTimer();
   static void CALLBACK WinTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);

   BOOL StartMMTimer();
   void StopMMTimer();
   static void CALLBACK MMTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

public:
   CAcqTimer();
   ~CAcqTimer();

   BOOL Start(HWND hwndParent, UINT uTimerID, UINT uPeriod, BOOL bMMTimer);
   void Stop();
   BOOL IsRunning() const;

   UINT GetTimeMS() const;
   double GetTime() const;

   void Acknowledge();
};


//===============================================================================================
// FUNCTION: Acknowledge
// PURPOSE:  Clears the semaphore to allow an other message to be sent.
//
inline void CAcqTimer::Acknowledge()
{
   MEMBERASSERT();
   m_bMessagePending = FALSE;
}

//===============================================================================================
// FUNCTION: GetTimeMS
// PURPOSE:  Returns the time since the timer was started in ms.
//
inline UINT CAcqTimer::GetTimeMS() const
{
   MEMBERASSERT();
   return m_Timer.GetTimeMS();
}

//===============================================================================================
// FUNCTION: GetTime
// PURPOSE:  Returns the time since the timer was started in seconds.
//
inline double CAcqTimer::GetTime() const
{
   MEMBERASSERT();
   return m_Timer.GetTime();
}

//###############################################################################################
// CLASS:   CAcqTimerAcknowledge
// PURPOSE: Like an auto_ptr, this class is used to unfailingly acknowledge
//          a MM timer message by the execution of its destructor.
//
class CAcqTimerAcknowledge
{
private:
   CAcqTimer *m_pTimer;

private:
   CAcqTimerAcknowledge();
   CAcqTimerAcknowledge(const CAcqTimerAcknowledge &);
   const CAcqTimerAcknowledge &operator=(const CAcqTimerAcknowledge &);

public:
   CAcqTimerAcknowledge(CAcqTimer *pTimer);
   ~CAcqTimerAcknowledge();
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CAcqTimerAcknowledge::CAcqTimerAcknowledge(CAcqTimer *pTimer)
{
   MEMBERASSERT();
   m_pTimer = pTimer;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Called on destruction of the object.
//
inline CAcqTimerAcknowledge::~CAcqTimerAcknowledge()
{
   MEMBERASSERT();
   m_pTimer->Acknowledge();
   m_pTimer = NULL;
}

#endif   // INC_AcqTimer_hpp
