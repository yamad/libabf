//***********************************************************************************************
//
//    Copyright (c) 2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  SecurityKey.cpp
// PURPOSE: CSecurityKey class implimentation.
// AUTHOR:  BHI Oct 2000
//
#include "wincpp.hpp"
#include "SecurityKey.hpp"
#include <stdlib.h>     // for atexit().
#include "\AxonDev\Comp\Common\WaitCursor.hpp"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"

// To enable the network security key (nethasp), create a .reg file as follows:
// REGEDIT4
// 
// [HKEY_CURRENT_USER\Software\Axon\Security\NetDongle]
// "TimeOut"="1500"

//***********************************************************************************************
// Constants
//***********************************************************************************************

// Environment variable to restrict the dongle search to a particular LPT.
const char c_pszDongleEnvironmentVariable[] = "axon_donglelpt";

const int  c_nMaxDongleLPT = 3;

static int  s_nNetID = 0;
static void Logout();

//***********************************************************************************************
// Static functions
//***********************************************************************************************

//===============================================================================================
// FUNCTION: ExitHandler
// PURPOSE:  .
//
static void ExitHandler()
{
   // TRACE("ExitHandler\n");
   Logout();
}

//===============================================================================================
// FUNCTION: Login
// PURPOSE:  Thread procedure for the HASP net-login process.
//
static DWORD CALLBACK Login(void *pvNetID)
{
   __try
   {
      int nNetID = int(pvNetID);
      int n1=0, n2=0, n3=0, n4=0, nLastStatus=0, n;
      
      n1 = n2 = n3 = n4 = 0;
      hasp( NETHASP_LOGIN, c_nAxonNetSeed, nNetID, c_nAxonNetPassword1, c_nAxonNetPassword2, 
            &n1, &n2, &n3, &n4 );
      hasp (NETHASP_LASTSTATUS, c_nAxonNetSeed, nNetID, c_nAxonNetPassword1, c_nAxonNetPassword2, 
            &nLastStatus, &n, &n, &n );

      if ( nLastStatus!=0 || 
           n1 != c_nAxonNetResponse1 || n2 != c_nAxonNetResponse2 ||
           n3 != c_nAxonNetResponse3 || n4 != c_nAxonNetResponse4)
         return FALSE;

      ASSERT(s_nNetID==0);
      s_nNetID = nNetID;
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      TRACE("Exception thrown by HASP driver.\n");
      return FALSE;
   }

   atexit(ExitHandler);
   return TRUE;
}

//===============================================================================================
// FUNCTION: Logout
// PURPOSE:  Net-hasp logout procedure.
//
static void Logout()
{
   if (s_nNetID)
   {
      __try
      {
         int n1=0, n2=0, n3=0, n4=0;
         hasp( NETHASP_LOGOUT, c_nAxonNetSeed, s_nNetID, c_nAxonNetPassword1, c_nAxonNetPassword2, 
               &n1, &n2, &n3, &n4 );
         s_nNetID = 0;
      }
      __except(EXCEPTION_EXECUTE_HANDLER)
      {
         TRACE("Exception thrown by HASP driver.\n");
      }
   }
}

//###############################################################################################
//###############################################################################################
//###
//###  CLASS: CSecurityKey
//###
//###############################################################################################
//###############################################################################################

//===============================================================================================
// FUNCTION: WaitForLoginDone
// PURPOSE:  Waits for the net-hasp login to complete.
//
BOOL CSecurityKey::WaitForLoginDone() const
{
   CWaitCursor Wait;

   DWORD dwExitCode = 0;

   // Wait for the thread to complete
   DWORD dwStatus = WaitForSingleObject(m_hThread, m_nNetDongle);
   if (dwStatus != WAIT_OBJECT_0)
      TerminateThread(m_hThread, 0);
   else
      // Get the exit code from the thread.
      GetExitCodeThread(m_hThread, &dwExitCode);

   // Close the thread.
   CloseHandle(m_hThread);
   m_hThread = NULL;

   // If the login did not complete OK, clear the net dongle flag.
   if (!dwExitCode)
   {
      m_nNetDongle = -1;
      return FALSE;
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: Logout
// PURPOSE:  Initializes a security key.
//
CSecurityKey::CSecurityKey( int nNetID, int nPassword1, int nPassword2, int nSeed,
                            int nReturn1, int nReturn2, int nReturn3, int nReturn4,
                            int nLPT)
{
   MEMBERASSERT();
   m_nPassword1 = nPassword1;
   m_nPassword2 = nPassword2;
   m_nSeed      = nSeed;
   m_nReturn1   = nReturn1;
   m_nReturn2   = nReturn2;
   m_nReturn3   = nReturn3;
   m_nReturn4   = nReturn4;
   m_nLPTNum    = nLPT;
   m_nNetDongle = 0;
   m_nNetID     = nNetID;
   m_hThread    = NULL;

   LPCSTR pszEnvVar = getenv( c_pszDongleEnvironmentVariable );
   if ( pszEnvVar )
   {
      int nLPT = atoi( pszEnvVar );
      if ((nLPT >= 0) && (nLPT < c_nMaxDongleLPT))
         m_nLPTNum = nLPT;
   }

   // Use the following line to set the timeout key.
   // AXU_WritePrivateProfileInt("NetDongle", "TimeOut", 1500, "Security");

   ASSERT(nLPT >= 0);
   
   //  first check the port the user wants us to check
   if (!DetectLocal(nLPT))
   {
      // if user sent in a particular LPT port and it didn't find it, then look at all ports      
      // if we didn't find the local dongle, check if user has network dongle checking set-up      
      if (nLPT == 0 || !DetectLocal(0))      
         AXU_GetPrivateProfileInt("NetDongle", "TimeOut", 0, &m_nNetDongle, "Security");
   }

   if (m_nNetDongle > 0)
   {
      DWORD dwThreadID = 0;
      m_hThread = CreateThread(NULL, 4096, Login, (void *)nNetID, 0, &dwThreadID);
      if (m_hThread)
         SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);
   }
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CSecurityKey::~CSecurityKey()
{
   MEMBERASSERT();
   Logout();   
}
