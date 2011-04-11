//***********************************************************************************************
//
//    Copyright (c) 1996-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CLowResTimer.hpp
// PURPOSE: A simple class to encapsulate the WIN32 function GetTickCount().
//          This is interchangable with the CHighResTimer class.
// AUTHOR:  BHI  Oct 1996
//
//
#ifndef INC_CLOWRESTIMER_HPP
#define INC_CLOWRESTIMER_HPP

#pragma once

//***********************************************************************************************
// CLASS:   CLowResTimer
// PURPOSE: Wrapper class for the GetTickCount() function.
//
class CLowResTimer
{
private:
   UINT m_uStartTime;      // Counter value saved by the Start() function.

private:
   // Allow the default copy constructor and the default operator=() to be used.

public:
   // Constructor  (marks the time of creation as the start time).
   CLowResTimer();

   // Start the timer at the current time.
   BOOL     Start();

   // Returns the number of milli-seconds that have elapsed since the start time.
   UINT     GetTimeMS() const;

   // Returns the fractional number of seconds that have elapsed since the start time.
   double   GetTime() const;

   // Sets the resolution of the timer.
   BOOL     SetResolution(UINT uPeriod) { return TRUE; }

   // Sleeps until the given time has elapsed.
   void     SleepUntil(UINT uTimeMS) const;
   void     SleepUntil(double dTime) const;
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CLowResTimer::CLowResTimer()
{
   MEMBERASSERT();

   // Start the time here (may be restarted with the Start() call).
   VERIFY(Start());
}

//===============================================================================================
// FUNCTION: Start
// PURPOSE:  Saves the current time as the start time.
//
inline BOOL CLowResTimer::Start()
{
   MEMBERASSERT();
   m_uStartTime = GetTickCount();
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetTime
// PURPOSE:  Returns the current time in fractional seconds.
//
inline double CLowResTimer::GetTime() const
{
   MEMBERASSERT();

   // Return the time since start in fractional seconds.
   return double(GetTickCount()- m_uStartTime) / 1000;
}

//===============================================================================================
// FUNCTION: GetTimeMS
// PURPOSE:  Returns the time since the start tick count in ms.
//
inline UINT CLowResTimer::GetTimeMS() const
{
   MEMBERASSERT();

   // Return the time since start in completed milli-seconds.
   return GetTickCount() - m_uStartTime;
}

//===============================================================================================
// FUNCTION: SleepUntil
// PURPOSE:  Sleeps until the given time has elapsed.
//
inline void CLowResTimer::SleepUntil(UINT uTimeMS) const
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
inline void CLowResTimer::SleepUntil(double dTime) const
{
   MEMBERASSERT();
   double dCurrent = GetTime();
   if (dCurrent < dTime)
      Sleep(int((dTime - dCurrent)*1000));
}

#endif   // INC_CLOWRESTIMER_HPP
