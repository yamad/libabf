//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CMultimediaTimer.hpp
// PURPOSE: A simple class to encapsulate the WIN32 function mmGetTime().
//          This is interchangable with the CHighResTimer class.
// AUTHOR:  BHI  Oct 1996
//
//
#ifndef INC_CMULTIMEDIATIMER_HPP
#define INC_CMULTIMEDIATIMER_HPP

#pragma once
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//***********************************************************************************************
// CLASS:   CMultimediaTimer
// PURPOSE: Wrapper class for the GetTickCount() function.
//
class CMultimediaTimer
{
private:
   UINT m_uStartTime;      // Counter value saved by the Start() function.
   UINT m_uPeriod;         // Minimum granularity of the timer.

private:
   // Allow the default copy constructor and the default operator=() to be used.

public:
   // Constructor  (marks the time of creation as the start time).
   CMultimediaTimer();
   ~CMultimediaTimer();

   // Start the timer at the current time.
   BOOL     Start();

   // Returns the number of milli-seconds that have elapsed since the start time.
   UINT     GetTimeMS() const;

   // Returns the fractional number of seconds that have elapsed since the start time.
   double   GetTime() const;

   // Sets the resolution of the timer.
   BOOL     SetResolution(UINT uPeriod);

   // Sleeps until the given time has elapsed.
   void     SleepUntil(UINT uTimeMS) const;
   void     SleepUntil(double dTime) const;
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CMultimediaTimer::CMultimediaTimer()
{
   MEMBERASSERT();
   m_uPeriod = 0;    // Default period.
   // Start the time here (may be restarted with the Start() call).
   VERIFY(Start());
}

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CMultimediaTimer::~CMultimediaTimer()
{
   MEMBERASSERT();
   if (m_uPeriod)
      timeEndPeriod( m_uPeriod );
}

//===============================================================================================
// FUNCTION: Start
// PURPOSE:  Saves the current time as the start time.
//
inline BOOL CMultimediaTimer::Start()
{
   MEMBERASSERT();
   m_uStartTime = timeGetTime();
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetTime
// PURPOSE:  Returns the current time in fractional seconds.
//
inline double CMultimediaTimer::GetTime() const
{
   MEMBERASSERT();

   // Return the time since start in fractional seconds.
   return double(timeGetTime()- m_uStartTime) / 1000;
}

//===============================================================================================
// FUNCTION: GetTimeMS
// PURPOSE:  Returns the time since the start tick count in ms.
//
inline UINT CMultimediaTimer::GetTimeMS() const
{
   MEMBERASSERT();

   // Return the time since start in completed milli-seconds.
   return timeGetTime() - m_uStartTime;
}

//===============================================================================================
// FUNCTION: SetResolution
// PURPOSE:  Sets the resolution of the timer.
//
inline BOOL CMultimediaTimer::SetResolution(UINT uPeriod)
{
   MEMBERASSERT();

   // If the period has been set, reset it before changing it again.
   if (m_uPeriod)
   {
      timeEndPeriod( m_uPeriod );
      m_uPeriod = 0;
   }

   // Try and set the period.
   if (timeBeginPeriod( uPeriod )!=TIMERR_NOERROR)
      return FALSE;

   // Save the period so that it can be restored later.
   m_uPeriod = uPeriod;
   return TRUE;
}
 
//===============================================================================================
// FUNCTION: SleepUntil
// PURPOSE:  Sleeps until the given time has elapsed.
//
inline void CMultimediaTimer::SleepUntil(UINT uTimeMS) const
{
   MEMBERASSERT();
   UINT uCurrentMS = GetTimeMS();
   if (uCurrentMS < uTimeMS)
      Sleep(uTimeMS - uCurrentMS);
}

//===============================================================================================
// FUNCTION: SleepUntil
// PURPOSE:  Sleeps until the given time has elapsed.
//
inline void CMultimediaTimer::SleepUntil(double dTime) const
{
   MEMBERASSERT();
   double dCurrent = GetTime();
   if (dCurrent < dTime)
      Sleep(int((dTime - dCurrent)*1000));
}

#endif   // INC_CMULTIMEDIATIMER_HPP
