//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  PollingThread.CPP
// PURPOSE: Generic Polling Thread. 
// AUTHOR:  BHI  Oct 2001
//
#include "wincpp.hpp"
#include <process.h>

#include "PollingThread.hpp"

const UINT c_uSuspendSleep = 0;

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Sets the m_hWnd variable and initializes the object.
//
CPollingThread::CPollingThread()
   : m_SuspendAckEvent(FALSE, TRUE), m_SuspendEvent(FALSE, FALSE)
{
   MEMBERASSERT();
   m_hThread       = NULL;
   m_uThreadID     = 0;
   m_uSleepMS      = 0;
   m_bThreadNeeded = true;
   m_bSuspended    = true;
   m_bInPoll       = false;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleans up on destruction of the object.
//
CPollingThread::~CPollingThread()
{
   MEMBERASSERT();
   ASSERT(m_hThread==NULL);
}

//===============================================================================================
// FUNCTION: IsSuspended
// PURPOSE:  Returns the status of the suspend flag.
//
BOOL CPollingThread::IsSuspended() const
{
   MEMBERASSERT();
   return m_bSuspended;
}

//===============================================================================================
// FUNCTION: IsCurrentThread
// PURPOSE:  Checks whether the current thread is this one.
//
BOOL CPollingThread::IsCurrentThread() const
{
   MEMBERASSERT();
   return GetCurrentThreadId() == m_uThreadID;
}

//===============================================================================================
// FUNCTION: WaitUntilNeeded
// PURPOSE:  Waits until the thread is required.
//
BOOL CPollingThread::WaitUntilNeeded()
{
   MEMBERASSERT();

   // Check if the thread should be suspended.
   if (m_bSuspended)
   {
      TRACE("== Thread suspended ==.\n");
      m_SuspendAckEvent.SetEvent();
      m_SuspendEvent.Lock();
      TRACE("== Thread resumed ==.\n");
   }
   else if (m_uSleepMS)
   {
      // Should not be able to obtain the lock...
      VERIFY(!m_SuspendEvent.Lock(m_uSleepMS));
   }
   return m_bThreadNeeded;
}

//===============================================================================================
// FUNCTION: Resume
// PURPOSE:  Resume the thread from its initial slumber, or its suspended state.
//
BOOL CPollingThread::Resume()
{
   MEMBERASSERT();
   TRACE("ResumeThread()\n");
   ASSERT(m_bSuspended);
   m_bSuspended = false;
   m_SuspendAckEvent.ResetEvent();
   m_SuspendEvent.SetEvent();

   // A little bit of sleep here allows the first portion of the polling code
   // to get done before it is interrupted. 
   Sleep(c_uSuspendSleep);
   return TRUE;
}

//===============================================================================================
// FUNCTION: Suspend
// PURPOSE:  Suspend the thread so that it does not chew up CPU cycles when not needed.
//
BOOL CPollingThread::Suspend()
{
   MEMBERASSERT();
   TRACE("SuspendThread()\n");
   if (!m_bSuspended)
   {
      // Set the suspend flag to prevent stacked suspend calls.
      m_bSuspended = true;

      // Wait for confirmation from the thread.
      if (!IsCurrentThread())
      {
         if (!m_SuspendAckEvent.Lock())
            return false;
         ASSERT(m_bSuspended != m_bInPoll);

         // As there is no object that is signaled when the thread actually goes into suspension,
         // we need to sleep a bit to make sure that the thread actually made it into suspension.
         Sleep(c_uSuspendSleep);
         TRACE("-- Suspended\n");
      }
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: Destroy
// PURPOSE:  Terminate the thread.
//
BOOL CPollingThread::Destroy(UINT uTimeout)
{
   MEMBERASSERT();
   // TRACE("Destroy()\n");
   // Set the suicide flag and resume the thread.
   Suspend();
   ASSERT(m_bThreadNeeded);
   m_bThreadNeeded = false;
   Resume();

   // A thread becomes signalled when it terminates.
   DWORD dwRval = WaitForSingleObject(m_hThread, uTimeout);

   CloseHandle(m_hThread);
   m_hThread = NULL;

   return (dwRval==WAIT_OBJECT_0);
}

//===============================================================================================
// FUNCTION: ThreadProc
// PURPOSE:  Thread procedure used to drive the data pump.
//
UINT CPollingThread::ThreadProc()
{
   MEMBERASSERT();
   // TRACE("== Thread Started ==\n");

   // Wait until the thread is needed, suicide if not.
   while (WaitUntilNeeded())
   {
      m_bInPoll = true;
      Poll();         
      m_bInPoll = false;
   }

   // TRACE("== Thread terminated ==\n");
   return 0;
}

//===============================================================================================
// FUNCTION: ThreadProc
// PURPOSE:  Thread proxy procedure used to call the object version.
//
static UINT WINAPI ThreadProxy( LPVOID pvTDrawWnd )
{
   // No MEMBERASSERT() -- static function.
   CPollingThread *pThread = (CPollingThread *)pvTDrawWnd;
   return pThread->ThreadProc();
}

//===============================================================================================
// FUNCTION: Create
// PURPOSE:  Create a thread in a suspended state..
//
BOOL CPollingThread::Create(int nPriority, DWORD dwClass, UINT uSleepMS)
{
   MEMBERASSERT();
   // TRACE("== Thread Created ==\n");

   // Save the client pointer and set the state flags.
   m_bThreadNeeded = true;
   m_bSuspended    = true;
   m_uSleepMS      = uSleepMS;

   if (dwClass!=NORMAL_PRIORITY_CLASS)
      VERIFY_SYSTEM_CALL(SetPriorityClass( GetCurrentProcess(),  dwClass ));

   // Create the thread in a running state so that it runs straight into suspension.
   // Use _beginthreadex so that clients can call runtime functions during drawing.
   // The cast to a HANDLE is OK -- MFC does this in CWinThread.
   ASSERT(m_hThread==NULL);
   DWORD dwStack   = 2048;
   m_uThreadID = 0;
   m_hThread = (HANDLE)_beginthreadex(NULL, dwStack, ThreadProxy, this, CREATE_SUSPENDED, &m_uThreadID);
   if (!m_hThread)
   {
      SHOW_SYSTEM_ERROR(0);
      return FALSE;
   }

   if (nPriority!=THREAD_PRIORITY_NORMAL)
      VERIFY_SYSTEM_CALL(::SetThreadPriority( m_hThread, nPriority ));

   VERIFY(m_SuspendAckEvent.ResetEvent());

   // Start the thread and let it suspend itself.
   ::ResumeThread(m_hThread);
   VERIFY(m_SuspendAckEvent.Lock());

   // Wait a bit more just to be sure.
   Sleep(c_uSuspendSleep);
   return TRUE;
}

//===============================================================================================
// FUNCTION: SetThreadPriority
// PURPOSE:  Changes the priority of the thread.
//
BOOL CPollingThread::SetThreadPriority(int nPriority)
{
   MEMBERASSERT();
   return ::SetThreadPriority( m_hThread, nPriority ) != FALSE;
}
