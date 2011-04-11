//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  NOTIFY.HPP
// PURPOSE: Contains the class definition for the CABFNotify class.
// 

#ifndef __NOTIFY_HPP__
#define __NOTIFY_HPP__

#include "abffiles.h"               // ABF file I/O API and error codes.

class CABFNotify
{
private:
   ABFCallback   m_fnCallback;            // Function pointer for an error callback function.
   void         *m_pvThisPointer;         // Context pointer to be passed when the callback is called.
   
public:
   CABFNotify();
   void RegisterCallback(ABFCallback fnCallback, void *pvThisPointer);
   BOOL Notify(int nError);
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes the object
//
inline CABFNotify::CABFNotify()
{
   MEMBERASSERT();
   m_fnCallback    = NULL;
   m_pvThisPointer = NULL;
}

//===============================================================================================
// FUNCTION: RegisterCallback
// PURPOSE:  Registers the callback function and context data.
//
inline void CABFNotify::RegisterCallback(ABFCallback fnCallback, void *pvThisPointer)
{
   MEMBERASSERT();
   m_fnCallback    = fnCallback;
   m_pvThisPointer = pvThisPointer;
}

//===============================================================================================
// FUNCTION: GetNextItem
// PURPOSE:  Returns the pointer to the next item in the list.
//
inline BOOL CABFNotify::Notify(int nError)
{
   MEMBERASSERT();
   if (!m_fnCallback)
      return FALSE;
   FNPTRASSERT(m_fnCallback);
   return m_fnCallback(m_pvThisPointer, nError);
}

#endif      // __NOTIFY_HPP__
