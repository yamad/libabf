//***********************************************************************************************
//
//    Copyright (c) 2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  SecurityKey.hpp
// PURPOSE: CSecurityKey class definition/code.
// AUTHOR:  BHI Oct 2000
//
#ifndef  INC_CSECURITYKEY_HPP
#define  INC_CSECURITYKEY_HPP

#include "\AxonDev\3rdParty\Hasp4\Win32api\Msc\Hasp.h"
#pragma comment(lib, "\\AxonDev\\3rdParty\\Hasp4\\win32api\\msc\\hasp.lib")

//***********************************************************************************************
// Constants
//***********************************************************************************************

// Axon NetHASP constants.
const int  c_nAxonNetPassword1 = 11201;
const int  c_nAxonNetPassword2 = 26316;
const int  c_nAxonNetSeed      = 100;
const int  c_nAxonNetResponse1 = 64967;
const int  c_nAxonNetResponse2 = 38382;
const int  c_nAxonNetResponse3 = 52377;
const int  c_nAxonNetResponse4 = 22174;

// Registry of Axon application network IDs.
const int  c_nAxonNetID_GENEPIX  = 1;
const int  c_nAxonNetID_ACUITY   = 2;
const int  c_nAxonNetID_PCLAMP   = 3;
const int  c_nAxonNetID_PFLUORO  = 4;
const int  c_nAxonNetID_GENEPIX4 = 5;

//***********************************************************************************************
// CLASS:   CSecurityKey
// PURPOSE: Wraps the HASP & NetHASP security dongle (LPT & USB).
//
class CSecurityKey
{
private:
   int  m_nPassword1;
   int  m_nPassword2;
   int  m_nSeed;
   int  m_nReturn1;
   int  m_nReturn2;
   int  m_nReturn3;
   int  m_nReturn4;   
   int  m_nNetID;
   mutable int    m_nLPTNum;
   mutable int    m_nNetDongle;
   mutable HANDLE m_hThread;

private:
   CSecurityKey();
   CSecurityKey( const CSecurityKey & );
   const CSecurityKey &operator=( const CSecurityKey & );

   BOOL WaitForLoginDone() const;
   int  DetectLocal(int nLPT) const;
   BOOL DetectNet() const;   

public:
   CSecurityKey( int nNetID, int nPassword1, int nPassword2, int nSeed,
                 int nReturn1, int nReturn2, int nReturn3, int nReturn4, int nLPT=0 );
   ~CSecurityKey();

   int  GetLPTPort() const;
   BOOL Detect(int nLPTPort = -1) const;
};

//***********************************************************************************************
// METHOD:  DetectLocal
// PURPOSE: Attemps to detect for LPT and USB dongles.
//
inline int CSecurityKey::GetLPTPort() const
{
   MEMBERASSERT();

   // m_nNetDongle will be > 0 if network dongle was found
   return (m_nNetDongle > 0) ? 0 : m_nLPTNum;      
}

//***********************************************************************************************
// METHOD:  DetectLocal
// PURPOSE: Attemps to detect for LPT and USB dongles.
//
inline BOOL CSecurityKey::DetectLocal(int nLPT) const
{
   MEMBERASSERT();

   int n1=0, n2=0, n3=0, n4=0;
   __try
   {
      hasp( LOCALHASP_ISHASP, 0, nLPT, m_nPassword1, m_nPassword2,
            &n1, &n2, &n3, &n4 );

      // If zero is returned then the driver is present but no dongle is attached.
      if ( n1 == 0 )
         return FALSE;

      n1 = n2 = n3 = n4 = 0;
      hasp( LOCALHASP_HASPCODE, m_nSeed, nLPT, m_nPassword1, m_nPassword2,
            &n1, &n2, &n3, &n4 );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      TRACE("Exception thrown by HASP driver.\n");
      return FALSE;
   }

   // If the result is correct then we have a dongle.
   if (n1 == m_nReturn1 && n2 == m_nReturn2 && n3 == m_nReturn3 && n4 == m_nReturn4)
   {
      // then get the port found and save it
      hasp( LOCALHASP_HASPSTATUS, 0, 0, m_nPassword1, m_nPassword2, &n1, &n2, &n3, &n4 );
      
      //  n3 holds the port in which the dongle was found
      m_nLPTNum = n3;
      
      return TRUE;
   }

   return FALSE;
}

//***********************************************************************************************
// METHOD:  DetectNet
// PURPOSE: Attemps to detect the key.
//
inline BOOL CSecurityKey::DetectNet() const
{
   MEMBERASSERT();

   if (m_hThread && !WaitForLoginDone())
      return FALSE;

   // Check that the net dongle is still there and that we still have access.
   int n1=0, n2=0, n3=0, n4=0;
   __try
   {
      hasp( NETHASP_HASPCODE, c_nAxonNetSeed, m_nNetID, c_nAxonNetPassword1, c_nAxonNetPassword2,
            &n1, &n2, &n3, &n4 );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      TRACE("Exception thrown by HASP driver.\n");
      return FALSE;
   }

   // If the result is correct then we are OK.
   return n1 == c_nAxonNetResponse1 && n2 == c_nAxonNetResponse2 && 
          n3 == c_nAxonNetResponse3 && n4 == c_nAxonNetResponse4;
}

//***********************************************************************************************
// METHOD:  Detect
// PURPOSE: Attemps to detect the key.
//
inline BOOL CSecurityKey::Detect(int nLPTPort /* =-1 */) const
{
   MEMBERASSERT();

   if (nLPTPort >= 0)
      return DetectLocal(nLPTPort);

   // Now try the net dongle or the port found in the constructor (0=All).
   return (m_nNetDongle > 0) ? DetectNet() : DetectLocal(m_nLPTNum);

}

#endif // INC_CSECURITYKEY_HPP
