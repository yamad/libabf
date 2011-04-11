//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments, Inc.
//    All rights reserved.
//

//***********************************************************************************************
// MODULE:  ClockRatio.hpp
// PURPOSE: Convert between various sample interval definitions
// AUTHOR:  Andrew Dalgleish

#ifndef  INC_CLOCKRATIO_HPP
#define  INC_CLOCKRATIO_HPP

//===============================================================================================
// user_units = pDP->fSamplingResolution * get_clock_ratio_hardware_to_user();
// user_units = pFH->fADCSampleInterval * get_clock_ratio_protocol_to_user();
// ASSERT(pFH->fADCSampleInterval * get_clock_ratio_protocol_to_hardware() >= pDP->fMinPeriod);
inline double AX_get_clock_ratio_hardware_to_user(const ENGN_DigitiserProperties *pDP, const CABFHeader *pFH)
{
   switch (pDP->input_clock_divider_type) 
   {
      default:
         ERRORMSG1("Unexpected input_clock_divider_type (%d)", pDP->input_clock_divider_type);
      case ENGN_DigitiserProperties::NONE:
         return 1.0;
      case ENGN_DigitiserProperties::ACTIVE_INPUT_CHANNELS:
         return pFH->get_channel_count_acquired();
   }
}

inline double AX_get_clock_ratio_protocol_to_hardware(const ENGN_DigitiserProperties *pDP, const CABFHeader *pFH)
{
   switch (pDP->input_clock_divider_type) 
   {
      default:
         ERRORMSG1("Unexpected input_clock_divider_type (%d)", pDP->input_clock_divider_type);
      case ENGN_DigitiserProperties::NONE:
         return pFH->get_channel_count_acquired();
      case ENGN_DigitiserProperties::ACTIVE_INPUT_CHANNELS:
         return 1.0;
   }
}

inline double WINAPI AX_get_clock_ratio_protocol_to_user(const ENGN_DigitiserProperties *pDP, const CABFHeader *pFH)
{
   switch (pDP->input_clock_divider_type) 
   {
      default:
         ERRORMSG1("Unexpected input_clock_divider_type (%d)", pDP->input_clock_divider_type);
      case ENGN_DigitiserProperties::NONE:
         return pFH->get_channel_count_acquired();
      case ENGN_DigitiserProperties::ACTIVE_INPUT_CHANNELS:
         return pFH->get_channel_count_acquired();
   }  
}

#endif
