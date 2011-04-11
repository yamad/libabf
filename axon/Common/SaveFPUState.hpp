//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  SaveFPUState.HPP
// PURPOSE: Saves the FPU state on construction, and restores it on destruction.
// AUTHOR:  BHI  Aug 1998
//

#ifndef INC_SAVEFPUSTATE_HPP
#define INC_SAVEFPUSTATE_HPP

#pragma once

#include <float.h>

class CSaveFPUState
{
private:   // Attributes
   UINT  m_uFPUControlWord;

public:    // Public interface
   CSaveFPUState();
   ~CSaveFPUState();
};

inline CSaveFPUState::CSaveFPUState()  
{
#ifdef _M_IX86
   m_uFPUControlWord = _control87(0, 0);
#else
   m_uFPUControlWord = _controlfp(0, 0);
#endif
   // TRACE1("FPU is  0x%X\n", m_uFPUControlWord);
}

inline CSaveFPUState::~CSaveFPUState()
{
#ifdef _M_IX86
   _control87(m_uFPUControlWord, UINT(-1));
#else
   _controlfp(m_uFPUControlWord, UINT(-1));
#endif
}

class CSetDefaultFPUState : public CSaveFPUState
{
public:    // Public interface
   CSetDefaultFPUState();
};

inline CSetDefaultFPUState::CSetDefaultFPUState()
   : CSaveFPUState()
{
#ifdef _M_IX86
   _control87(_CW_DEFAULT, UINT(-1));
#else
   _controlfp(_CW_DEFAULT, UINT(-1));
#endif
   // TRACE1("FPU is  0x%X\n", m_uFPUControlWord);
}

#endif      // INC_SAVEFPUSTATE_HPP

