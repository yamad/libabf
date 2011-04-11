//***********************************************************************************************
//
//    Copyright (c) 2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Dongle.hpp
// PURPOSE: CDongleKey class definition/code.
// AUTHOR:  RHW 2002
//
#ifndef  INC_CDongle_HPP
#define  INC_CDongle_HPP

#include "\AxonDev\3rdParty\Hasp4\Win32api\Msc\Hasp.h"
#pragma comment(lib, "\\AxonDev\\3rdParty\\Hasp4\\win32api\\msc\\hasp.lib")

//***********************************************************************************************
// CLASS:   CDongle
// PURPOSE: Abstract base class for local dongle keys
//
class CDongle
{
public:                  
   CDongle() {};   
      
   enum EDONGLEFOUND
   {
      DONGLE_FOUND_NO   =0,
      DONGLE_FOUND_YES  =1,      
      DONGLE_FOUND_MAYBE=2,
   };
      
   enum EDONGLEERROR
   {
      DONGLE_ERROR_SUCCESS    = 0,
      DONGLE_ERROR_HASPERR    = 1, // HASP specific error
      DONGLE_ERROR_OTHERDONGLE= 2, // a dongle was found, but not the one you asked for
      DONGLE_ERROR_EXCEPTION  = 3, // problem accessing dongle code (exception occurred)      
   };

   virtual EDONGLEFOUND   Detect(EDONGLEERROR * peError = NULL, int * pnHASPError = NULL) const = 0;            

   virtual EDONGLEERROR   GetLastError() const = 0;   
   virtual int            GetLastHASPError() const = 0;

   virtual void           GetErrorString(EDONGLEERROR eError, int nHaspError, char * pszErrorMsg, int nBufferSize) const = 0;

protected:
   // must call AXDONGLE_Delete to destroy class
   virtual ~CDongle() {};

private:         
   CDongle( const CDongle & );
   const CDongle &operator=( const CDongle & );         
   
};

//***********************************************************************************************
// Constants
//***********************************************************************************************
struct DongleCodes
{
   int   m_nPassword1;
   int   m_nPassword2;
   int   m_nSeed;
   int   m_nResponse1;
   int   m_nResponse2;
   int   m_nResponse3;
   int   m_nResponse4;
   int   m_nProgramID; // set to 0 for local dongles
};

// use this constant to choose which index to use in
// AXDONGLE_CreateNetwork( int nAppIndex  );
const int   c_nAxonNetwork_GENEPIX3    = 0;
const int   c_nAxonNetwork_GENEPIX4    = 1;
const int   c_nAxonNetwork_PCLAMP      = 2;
const int   c_nAxonNetwork_ACUITY1     = 3;

const int   c_nAxonNetwork_TOTALAPPS   = 4;


CDongle *   AXDONGLE_CreateLocal(const DongleCodes * pDongleCodes);

CDongle *   AXDONGLE_CreateNetwork(int nAppIndex );

// pass in a non-NULL hWnd to show error messages if logout of network dongle fails and to be asked to try again
// if no window handle is passed in, and the user can't log off, the license will be left checked out
void        AXDONGLE_Delete(HWND hWnd, CDongle * pDongle);


#endif // INC_CDongle_HPP
