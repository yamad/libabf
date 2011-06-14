//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  PollingThread.HPP
// PURPOSE: Contains class definition for the generic CPollingThread class.
// AUTHOR:  BHI  Oct 2001
//

#ifndef INC_POLLINGTHREAD_HPP
#define INC_POLLINGTHREAD_HPP

#pragma once

#include "..\Common\AfxMT.hpp"

//====================================================================================================
// CLASS:   CPollingThread
// PURPOSE: Encapsulates a Win32 thread.
//
class CPollingThread
{
protected:
   HANDLE m_hThread;          // The handle to the thread.
   UINT   m_uThreadID;        // ID of the background thread.
   CEvent m_SuspendEvent;     // Event that is used to suspend the thread.
   CEvent m_SuspendAckEvent;  // Event that is signaled when the thread suspends.
   UINT   m_uSleepMS;         // Time to sleep each time round the loop.
   BOOL   m_bThreadNeeded;    // Set this flag FALSE to kill the thread.
   BOOL   m_bSuspended;       // true if thread is suspended.
   BOOL   m_bInPoll;          // true if we're in client's drawing code

private:    // Unimplemented member functions.
   CPollingThread(const CPollingThread &);
   const CPollingThread &operator=(const CPollingThread &);

protected:
   // Waits until the thread is required.
   BOOL WaitUntilNeeded();

public:
   // Construction/destruction
   CPollingThread();
   virtual ~CPollingThread();
   
   // Thread procedure used to drive the data pump.
   virtual UINT ThreadProc();

   // The callback interface.
   virtual BOOL Poll() = 0;

   //                        THREAD_PRIORITY_TIME_CRITICAL    
   //                        THREAD_PRIORITY_HIGHEST      
   //                        THREAD_PRIORITY_ABOVE_NORMAL 
   //                        THREAD_PRIORITY_NORMAL                REALTIME_PRIORITY_CLASS
   //                        THREAD_PRIORITY_BELOW_NORMAL          HIGH_PRIORITY_CLASS
   //                        THREAD_PRIORITY_IDLE                  NORMAL_PRIORITY_CLASS
   //                        THREAD_PRIORITY_LOWEST                IDLE_PRIORITY_CLASS
   // Create a thread in a suspended state..
   BOOL Create(int nPriority=THREAD_PRIORITY_NORMAL, DWORD dwClass=NORMAL_PRIORITY_CLASS, 
               UINT uSleepMS=0);
   BOOL Destroy(UINT uTimeout=1000);

   // Changes the priority of the thread.
   BOOL SetThreadPriority(int nPriority);

   // Resume the thread from its initial slumber, or its suspended state.
   BOOL Resume();

   // Suspend the thread so that it does not chew up CPU cycles when not needed.
   BOOL Suspend();

   // Returns the status of the suspend flag.
   BOOL IsSuspended() const;

   // Checks whether the current thread is this one.
   BOOL IsCurrentThread() const;
};

#endif    // INC_POLLINGTHREAD_HPP
