//***********************************************************************************************
//
//    Copyright (c) 2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Dongle.cpp
// PURPOSE: CDongle class implimentation.
// AUTHOR:  RHW 2002
//
#include "wincpp.hpp"
#include "Dongle.hpp"

DongleCodes c_NetworkDongles[] =
{
   { 11201,   26316, 100, 64967, 38382, 52377, 22174,   1 },  // Genepix Pro 3
   { 11201,   26316, 100, 64967, 38382, 52377, 22174,   5 },  // Genepix Pro 4
   { 11201,   26316, 100, 64967, 38382, 52377, 22174,   3 },  // pClamp (all versions)
   { 21157,   32359, 100, 42306, 20616, 57400, 3842,    1 },  // Acuity 1.0
};


static const int AX_NETWORK_WAIT_OBJECT = 500; // in milliseconds
static const int AX_NETWORK_TIMEOUT     = 1000 * 60 * 5; // in milliseconds


//***********************************************************************************************
// CLASS:   CDongleKey
// PURPOSE: Wraps the HASP security dongle (LPT & USB).
//
class CDongleKey : public CDongle
{
protected:
   
   // TRUE, then we can't check the dongle at this time   
   static BOOL          m_bInDongleCheck;      
   
   DongleCodes          m_DongleCodes;
   
   mutable int          m_nLPTNum;
   
   mutable int          m_nError;
   mutable EDONGLEERROR m_eError;

   mutable int          m_nVer1;
   mutable int          m_nVer2;
   mutable int          m_nVer3;
      
public:      
   
   // use for both network and local dogle checking
   // pass in -1 for nNetworkAppIndex, if not using network dongle support
   CDongleKey(const DongleCodes * pDongleCodes);
                          
   virtual ~CDongleKey();
         
   virtual EDONGLEFOUND    Detect(EDONGLEERROR * peError = NULL, int * pnHASPError = NULL) const;               
   

   virtual EDONGLEERROR    GetLastError() const { return m_eError; }
   virtual int             GetLastHASPError() const { return m_nError; }

   virtual void            GetErrorString(EDONGLEERROR eError, int nHaspError, char * pszErrorMsg, int nBufferSize) const;
                           

   virtual BOOL            Finished() const;

// NON VIRTUAL PUBLIC

   // detects local dongle, but does not set error codes
   EDONGLEFOUND            DetectLocal(EDONGLEERROR * peError = NULL, int * pnHASPError = NULL, BOOL bSetInternalError = TRUE) const;   
   
   

   BOOL                    SetLastError(EDONGLEERROR eError, int nError, EDONGLEERROR * peError = NULL, int * pnError = NULL, BOOL bSetInternalError = TRUE) const;   

   // returns LPT port that the dongle was found on, -1 if none
   int                     GetLPTPort() const { return m_nLPTNum; }

   // return the codes
   DongleCodes             GetCodes() const { return m_DongleCodes; }      

   BOOL                    GetDongleChecking() const { return m_bInDongleCheck; }
   void                    SetDongleChecking(BOOL bSet) { m_bInDongleCheck = bSet; }

private:   
   CDongleKey();
   CDongleKey( const CDongleKey & );
   const CDongleKey &operator=( const CDongleKey & );         
   
};




//***********************************************************************************************
// CLASS:   CNetworkDongleKey
// PURPOSE: Wraps the NetHASP security dongle
//
class CNetworkDongleKey : public CDongleKey
{
private:
   
   mutable HANDLE         m_hThread;
   
   mutable EDONGLEFOUND   m_eHaveLicense;
   mutable DWORD          m_dwStartLogin;
         
public:
   
   // use for both network and local dogle checking
   // pass in -1 for nNetworkAppIndex, if not using network dongle support
   CNetworkDongleKey( int nAppIndex );
                          
   ~CNetworkDongleKey();
      
   virtual EDONGLEFOUND    Detect(EDONGLEERROR * peError = NULL, int * pnHASPError = NULL) const;            

   void  SetHaveLicense(EDONGLEFOUND eSet) { m_eHaveLicense = eSet; }   

   virtual BOOL            Finished() const;

private:   
   CNetworkDongleKey();
   CNetworkDongleKey( const CNetworkDongleKey & );
   const CNetworkDongleKey &operator=( const CNetworkDongleKey & );            
};



BOOL CDongleKey::m_bInDongleCheck = FALSE;

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes a security key.
//
CDongleKey::CDongleKey( const DongleCodes * pDongleCodes )
{      
   memset(&m_DongleCodes, 0, sizeof(m_DongleCodes));
   m_nLPTNum = -1;
   m_nError = 0;
   m_nVer1 = m_nVer2 = m_nVer3 = 0;
   if (pDongleCodes)
      m_DongleCodes = *pDongleCodes;      
}


//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CDongleKey::~CDongleKey()
{
   MEMBERASSERT();   
}


//===============================================================================================
// FUNCTION: SetLastError
//
BOOL CDongleKey::SetLastError(EDONGLEERROR eError, int nError, EDONGLEERROR * peError, int * pnError, BOOL bSetInternalError ) const
{
   if (bSetInternalError)
   {
      m_nError = nError; 
      m_eError = eError; 
   }
   
   if (pnError) 
      *pnError = nError; 

   if (peError) 
      *peError = eError; 
            
   return FALSE;
}

//===============================================================================================
// FUNCTION: DetectLocal
// PURPOSE:  Detects dongle on local ports
//
CDongleKey::EDONGLEFOUND CDongleKey::DetectLocal(EDONGLEERROR * peError, int * pnHASPError, BOOL bSetErrors) const
{
   MEMBERASSERT();            

   if (m_bInDongleCheck)
   {
      Sleep(5);
      return DONGLE_FOUND_MAYBE;
   }
   
   int nLPT=0;

   int n1=0, n2=0, n3=0, n4=0;
   
   m_bInDongleCheck = TRUE;

   __try
   {      
      hasp( LOCALHASP_ISHASP,
         0,
         nLPT,
         0,
         0,
         &n1, &n2, &n3, &n4 );   
      
      // If zero is returned then the driver is present but no dongle is attached.
      if ( n1 == 0 )      
      {
         m_bInDongleCheck = FALSE;
         SetLastError(DONGLE_ERROR_HASPERR, n2, peError, pnHASPError, bSetErrors);      
         return DONGLE_FOUND_NO;
      }
      
      n1 = n2 = n3 = n4 = 0;
      hasp( LOCALHASP_HASPCODE, m_DongleCodes.m_nSeed, nLPT, m_DongleCodes.m_nPassword1, m_DongleCodes.m_nPassword2,
            &n1, &n2, &n3, &n4 );
                        
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      m_bInDongleCheck = FALSE;
      TRACE("Exception thrown by HASP driver.\n");
      SetLastError(DONGLE_ERROR_EXCEPTION, 0, peError, pnHASPError, bSetErrors);            
      return DONGLE_FOUND_NO;
   } 

   // If the result is correct then we have a dongle.
   if (n1 == m_DongleCodes.m_nResponse1 && n2 == m_DongleCodes.m_nResponse2 && n3 == m_DongleCodes.m_nResponse3 && n4 == m_DongleCodes.m_nResponse4)
   {
      int nMemSize=0, nHaspType=0, nVer=0;

      // then get the port found and save it
      hasp( LOCALHASP_HASPSTATUS, 0, 0, m_DongleCodes.m_nPassword1, m_DongleCodes.m_nPassword2, 
            &nMemSize, &nHaspType, &m_nLPTNum, &nVer );            
      
      m_nVer1 = nVer >> 8;
      m_nVer2 = nVer & 0xff;
      m_nVer3 = (int)(( div ( (int)nVer, 100 ) ).rem);
      
      m_bInDongleCheck = FALSE;
      return DONGLE_FOUND_YES;
   }

   m_bInDongleCheck = FALSE;
   
   SetLastError(DONGLE_ERROR_OTHERDONGLE, 0, peError, pnHASPError, bSetErrors);      
   return DONGLE_FOUND_NO;
}

//===============================================================================================
// FUNCTION: Detect
// PURPOSE:  Detects dongle on local ports
//
CDongleKey::EDONGLEFOUND CDongleKey::Detect(EDONGLEERROR * peError, int * pnHASPError) const
{
   MEMBERASSERT();            

   return DetectLocal(peError, pnHASPError);
}
 
//===============================================================================================
// FUNCTION: Finished
// PURPOSE:  Object cleanup.
//
BOOL CDongleKey::Finished() const
{
   return TRUE;
}

//===============================================================================================
// FUNCTION: Login
// PURPOSE:  Thread procedure for the HASP net-login process.
//
static DWORD CALLBACK Login(void * pvNetworkDongleKey)
{   
   CNetworkDongleKey * pNetDongle = (CNetworkDongleKey *) pvNetworkDongleKey;
   ASSERT(pNetDongle);

   // we may have one, but we don't know yet
   pNetDongle->SetHaveLicense(CDongle::DONGLE_FOUND_MAYBE);

   // is someone else trying to access the dongle? if so, don't do anything
   // or we crash and burn
   while (pNetDongle->GetDongleChecking())   
      Sleep(5);   
      
   __try
   {                  
      // okay, lock the object because we're going in, baby!
      pNetDongle->SetDongleChecking(TRUE);            

      DongleCodes codes = pNetDongle->GetCodes();

      int n1=0, n2=0, n3=0, n4=0, nLastStatus=0, n=0, nSysError=0, nWarning=0;            
            
      hasp( NETHASP_LOGIN, codes.m_nSeed, codes.m_nProgramID, codes.m_nPassword1, codes.m_nPassword2, 
            &n1, &n2, &n3, &n4 );      

      hasp (NETHASP_LASTSTATUS, codes.m_nSeed, codes.m_nProgramID, codes.m_nPassword1, codes.m_nPassword2, 
            &nLastStatus, &nSysError, &nWarning, &n );                        

      // okay, we're out and safe, so unlock the object
      pNetDongle->SetDongleChecking(FALSE);      

      if ( nLastStatus != HASPERR_SUCCESS)
      {         
         pNetDongle->SetHaveLicense(CDongle::DONGLE_FOUND_NO);         
         pNetDongle->SetLastError(CDongle::DONGLE_ERROR_HASPERR, nLastStatus);
         return FALSE;
      }
      if (n1 != codes.m_nResponse1 || n2 != codes.m_nResponse2 ||
          n3 != codes.m_nResponse3 || n4 != codes.m_nResponse4)
      {         
         pNetDongle->SetHaveLicense(CDongle::DONGLE_FOUND_NO);         
         pNetDongle->SetLastError(CDongle::DONGLE_ERROR_HASPERR, HASPERR_MH_INVALID_PASSWORDS);
         return FALSE;
      }            
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      pNetDongle->SetDongleChecking(FALSE);      
      TRACE("Exception thrown by HASP driver.\n");         
      pNetDongle->SetHaveLicense(CDongle::DONGLE_FOUND_NO);         
      pNetDongle->SetLastError(CDongle::DONGLE_ERROR_EXCEPTION, 0);
      return FALSE;
   }

   // we got the license!!!
   pNetDongle->SetHaveLicense(CDongle::DONGLE_FOUND_YES);
   return TRUE;
}


//===============================================================================================
// FUNCTION: GetErrorString
// PURPOSE:  returns an error string based on given values
//
void CDongleKey::GetErrorString(EDONGLEERROR eError, int nHaspError, char * pszErrorMsg, int nBufferSize) const
{
   LPCSTR pszError = "";

   if (eError == DONGLE_ERROR_SUCCESS)
   {
      pszError = "The operation was successful";   
   }
   else if (eError == DONGLE_ERROR_HASPERR)
   {
      switch (nHaspError)
      {    
      // parallel port busy
      case HASPERR_PORT_BUSY:      
         pszError = "The parallel port is currently unavailable.\r\nAnother device attached to it, such as a printer, is active.\r\nPlease try again.";                     
         break;
   
      case HASPERR_TS_FOUND:         
      case HASPERR_TS_SP3_FOUND:
         pszError = "Dongle checking is not support under NT Terminal Server.";
         break;

      case HASPERR_CANT_OPEN_HDD:
      case HASPERR_CANT_READ_HDD:
      case HASPERR_CANT_CLOSE_HDD:
         pszError = "Access to the HASP device driver was denied.\r\nPlease make sure the device driver is installed on the machine.";
         break;

      case HASPERR_SUCCESS:
         pszError = "No dongle for this application was found.";
         break;

      // then no active NetHASP Licence Manager was found, return immediately
      case HASPERR_NO_ACTIVE_NHLM :      
      case HASPERR_NO_NHLM :   
         pszError = "There is no active NetHASP Licence Manager found on the network.\r\nPossible reasons:\r\n1. You are not connected to the network.\r\n2. The computer connected to the dongle is down.";
         break;
            
      // NetHASP key is not connected to the NetHASP Licence Manager
      case HASPERR_NH_HASPNOTFOUND:
         pszError = "A dongle was found, but is not connected to a NetHASP License Manager.";
         break;

      // A HASP with specified passwords was not found
      case HASPERR_HASP_NOT_FOUND :      
         pszError = "A network dongle with invalid passwords was found.";         
         break;

      // Number of stations exceeded
      case HASPERR_NO_MORE_STATIONS:
      case HASPERR_NO_MORE_ACTIVATIONS:
         pszError = "There are no more seats available for this application.";                  
         break;

      // NetHASP Licence Manager is busy
      case HASPERR_NHLM_BUSY :
         pszError = "The License Manager for detecting network dongles is busy, please try again later.";         
         break;
      
      default:
      {
         char cFormat[512] = "";
         sprintf(cFormat, "Error %d has occurred while trying to detect the dongle. Please contact Axon techincal support.",nHaspError);
         pszError = cFormat;         
         break;
      }
      }
   }
   else if (eError == DONGLE_ERROR_OTHERDONGLE)
   {
      pszError = "A dongle was found on the computer, but it is not a valid dongle for this application.";            
   }
   else if (eError == DONGLE_ERROR_EXCEPTION)
   {
      pszError = "An exception occurred while trying to detect the dongle.\r\nPlease try again later. If the problem persists please reinstall the application or contact Axon technical support.";            
   }
   else
   {
      ASSERT(FALSE);
      pszError = "An unknown INTERNAL error has occurred.";
   }
   
   if (pszErrorMsg)
   {
      strncpy(pszErrorMsg, pszError, nBufferSize-1);
      pszErrorMsg[nBufferSize-1] = '\0';
   }         
}


//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes a security key.
//
CNetworkDongleKey::CNetworkDongleKey( int nAppIndex ) : CDongleKey(NULL)
{
   m_DongleCodes = c_NetworkDongles[nAppIndex];      
   m_eHaveLicense = DONGLE_FOUND_MAYBE;
   m_hThread = NULL;
   m_dwStartLogin = 0;   
}


//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CNetworkDongleKey::~CNetworkDongleKey()
{   
   Finished();   
}

//===============================================================================================
// FUNCTION: Finished
// PURPOSE:  Object cleanup.
//
BOOL CNetworkDongleKey::Finished() const
{
   if (m_hThread)
   {
      // wait to see if the login is done, so wait for about a half second
      DWORD dwStatus = WaitForSingleObject(m_hThread, 0);

      // thread not done?
      if (dwStatus != WAIT_OBJECT_0)
         TerminateThread(m_hThread, 0);      

      CloseHandle(m_hThread);
      m_hThread = NULL;
   }

   BOOL bLogOut = TRUE;

   // if we have a license, release it now
   if (m_eHaveLicense == DONGLE_FOUND_YES)
   {
      int n1=0, n2=0, n3=0, n4=0;      
      
      hasp( NETHASP_HASPCODE, m_DongleCodes.m_nSeed, m_DongleCodes.m_nProgramID, m_DongleCodes.m_nPassword1, m_DongleCodes.m_nPassword2, 
         &n1, &n2, &n3, &n4 );      
               
      if (n1 != m_DongleCodes.m_nResponse1 || n2 != m_DongleCodes.m_nResponse2 ||
          n3 != m_DongleCodes.m_nResponse3 || n4 != m_DongleCodes.m_nResponse4)
      {            
         bLogOut = FALSE;
      }         
      else
      {
         m_eHaveLicense = DONGLE_FOUND_NO;                                      
      }

      // try anyway, it doesn't hurt!
      hasp( NETHASP_LOGOUT, m_DongleCodes.m_nSeed, m_DongleCodes.m_nProgramID, m_DongleCodes.m_nPassword1, m_DongleCodes.m_nPassword2, 
            &n1, &n2, &n3, &n4 );       
   }

   return bLogOut;
}


//===============================================================================================
// FUNCTION: DetectNetwork
// PURPOSE:  Waits for the net-hasp login to complete.
//
CDongle::EDONGLEFOUND CNetworkDongleKey::Detect(EDONGLEERROR * peError, int * pnHASPError) const         
{         
   // detect local first, just in case it's there, but don't set error codes   
   if (CDongleKey::DetectLocal(NULL, NULL, FALSE) == DONGLE_FOUND_YES)
      return DONGLE_FOUND_YES;

   // if we haven't started before, or we already tried, but got a HASP error, try again
   if (m_dwStartLogin == 0 || (!m_hThread && m_eHaveLicense == DONGLE_FOUND_NO && m_eError == DONGLE_ERROR_HASPERR))
   {
      ASSERT(m_hThread == NULL);
                  
      DWORD dwThreadID = 0;
      m_hThread = CreateThread(NULL, 4096, Login, (void *)this, 0, &dwThreadID);
      if (m_hThread)
         SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);      
      else
      {
         m_eHaveLicense = DONGLE_FOUND_NO;
         SetLastError(DONGLE_ERROR_EXCEPTION, 0, peError, pnHASPError);
      }

      m_dwStartLogin = GetTickCount();
      Sleep(100);      
   }

   if (peError)
      *peError = m_eError;   

   if (pnHASPError)
      *pnHASPError = m_nError;   

   // if thread is done, just return what we got, it could be anything
   if (!m_hThread)
      return m_eHaveLicense;   
   
   // wait to see if the login is done, so wait for about a half second
   DWORD dwStatus = WaitForSingleObject(m_hThread, AX_NETWORK_WAIT_OBJECT);

   // thread not done?
   if (dwStatus != WAIT_OBJECT_0)
   {
      // if we still don't have a license by now, something is wrong!
      if (GetTickCount() - m_dwStartLogin > AX_NETWORK_TIMEOUT) 
      {
         TerminateThread(m_hThread, 0);      
         m_bInDongleCheck = FALSE;
         m_eHaveLicense = DONGLE_FOUND_NO;
         CloseHandle(m_hThread);
         m_hThread = NULL;
         SetLastError(DONGLE_ERROR_EXCEPTION, 0, peError, pnHASPError);
      }
   }
   else
   {         
      // Close the thread.
      CloseHandle(m_hThread);
      m_hThread = NULL;
   }

   if (peError)
      *peError = m_eError;   

   if (pnHASPError)
      *pnHASPError = m_nError;   
   
   return m_eHaveLicense;         
}



CDongle * AXDONGLE_CreateLocal(const DongleCodes * pDongleCodes)
{
   ASSERT(pDongleCodes);
   CDongleKey * pDongle = new CDongleKey(pDongleCodes);

   return pDongle;
}



CDongle *  AXDONGLE_CreateNetwork(int nAppIndex )
{
   ASSERT(nAppIndex >= 0 && nAppIndex < c_nAxonNetwork_TOTALAPPS);

   CDongle * pDongle = new CNetworkDongleKey( nAppIndex );

   return pDongle;
}


void AXDONGLE_Delete(HWND hWnd, CDongle * pDongle)
{
   ASSERT(pDongle);

   CDongleKey * pDongleKey = (CDongleKey *) pDongle;

   BOOL bTryAgain = TRUE;      
   BOOL bShowWarning = FALSE;

   while (bTryAgain)
   {
      BOOL bLogOut = pDongleKey->Finished();
      
      if (!bLogOut && hWnd)
      {            
         bShowWarning = TRUE;
         // user not on network anymore so we can't log out!            
         if (::MessageBox(hWnd, "Warning: the application can not log you off of the network dongle.\r\n\r\nPlease check that you are connected to the network.\r\n\r\nClick 'Yes' to try again, or 'No' to keep a network license checked out.", "Network Dongle", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)
            bTryAgain = FALSE;
      }
      else
      {         
         bTryAgain = FALSE;
      }

      if (bLogOut && bShowWarning && hWnd)
      {         
         ::MessageBox(hWnd, "Success: you have logged off of the network dongle.", "Network Dongle", MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
      }
   }   

   delete pDongleKey;
}


