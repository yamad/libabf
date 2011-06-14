//***********************************************************************************************
//
//    Copyright (c) 1996-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CHighResTimer.hpp
// PURPOSE: A simple class to encapsulate the high resoultion timer exposed by the
//          WIN32 function QueryPerformanceCounter().
//          This is interchangable with the CLowResTimer class.
// AUTHOR:  BHI  Oct 1996
//
//
#ifndef INC_CHIGHRESTIMER_HPP
#define INC_CHIGHRESTIMER_HPP

#pragma once

//***********************************************************************************************
// CLASS:   CHighResTimer
// PURPOSE: Wrapper class for the QueryPerformanceCounter() function.
//
class CHighResTimer
{
private:
   LARGE_INTEGER m_liFrequency;
   LARGE_INTEGER m_liStart;

private:
   // Allow the default copy constructor and the default operator=() to be used.

public:
   // Constructor  (marks the time of creation as the start time).
   CHighResTimer();

   // Start the timer at the current time.
   BOOL     Start();

   // Returns the number of milli-seconds that have elapsed since the start time.
   UINT     GetTimeMS() const;

   // Returns the fractional number of seconds that have elapsed since the start time.
   double   GetTime() const;

   // Returns the number of clock ticks that have elapsed since the start time.
   UINT     GetTicks() const;

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
inline CHighResTimer::CHighResTimer()
{
   MEMBERASSERT();

   // Get the frequency of the timer.
   VERIFY(QueryPerformanceFrequency(&m_liFrequency));

   // Start the time here (may be restarted with the Start() call).
   VERIFY(Start());
}

//===============================================================================================
// FUNCTION: Start
// PURPOSE:  Saves the current time as the start time.
//
inline BOOL CHighResTimer::Start()
{
   MEMBERASSERT();
   return QueryPerformanceCounter(&m_liStart);
}

//===============================================================================================
// FUNCTION: GetTime
// PURPOSE:  Returns the current time in fractional seconds.
//
inline double CHighResTimer::GetTime() const
{
   MEMBERASSERT();

   // Get the current time.
   LARGE_INTEGER liNow;
   QueryPerformanceCounter(&liNow);

   // Calculate the time elapsed since the start in ticks.
   LONGLONG liDelta = liNow.QuadPart - m_liStart.QuadPart;

   // Return the time since start in fractional seconds.
   return double(liDelta) / m_liFrequency.QuadPart;
}

//===============================================================================================
// FUNCTION: GetTimeMS
// PURPOSE:  Returns the time since the start tick count in ms.
//
inline UINT CHighResTimer::GetTimeMS() const
{
   MEMBERASSERT();

   // Get the current time.
   LARGE_INTEGER liNow;
   QueryPerformanceCounter(&liNow);

   // Calculate the time elapsed since the start in ticks.
   LONGLONG liDelta = liNow.QuadPart - m_liStart.QuadPart;

   // Return the time since start in completed milli-seconds.
   return UINT((liDelta*1000) / m_liFrequency.QuadPart);
}

//===============================================================================================
// FUNCTION: GetTicks
// PURPOSE:  Returns the time since the start in ticks.
//
inline UINT CHighResTimer::GetTicks() const
{
   MEMBERASSERT();

   // Get the current time.
   LARGE_INTEGER liNow;
   QueryPerformanceCounter(&liNow);

   // Calculate the time elapsed since the start in ticks.
   return liNow.LowPart - m_liStart.LowPart;
}

//===============================================================================================
// FUNCTION: SleepUntil
// PURPOSE:  Sleeps until the given time has elapsed.
//
inline void CHighResTimer::SleepUntil(UINT uTimeMS) const
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
inline void CHighResTimer::SleepUntil(double dTime) const
{
   MEMBERASSERT();
   double dCurrent = GetTime();
   if (dCurrent < dTime)
      Sleep(int((dTime - dCurrent)*1000));
}

#endif   // INC_CHIGHRESTIMER_HPP
