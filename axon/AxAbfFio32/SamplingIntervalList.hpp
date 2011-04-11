//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments, Inc.
//    All rights reserved.
//

//***********************************************************************************************
// MODULE:  SamplingIntervalList.hpp
// PURPOSE: Structure to describe the values to use for a sampling interval list
// AUTHOR:  Andrew Dalgleish

#ifndef  INC_SAMPLINGINTERVALLIST_HPP
#define  INC_SAMPLINGINTERVALLIST_HPP

#include "ClockRatio.hpp"

const int   c_sampling_interval_list_length        = 21;    // 3 values/decade * 7 decades
const float c_sampling_interval_maximum_interval   = 1e6F;  // max interval is 1 second per channel

// Note: all values are in user units (ie PER CHANNEL)
// The protocol is in UU / (number of channels)
// The hardware can be be different again, but so far is either UU or protocol units.
struct   AX_sampling_interval_list
{
public:

   float    m_list_entries[c_sampling_interval_list_length];
   int      m_number_of_entries;

   float    m_minimum_interval;
   float    m_maximum_interval;
   float    m_resolution;

public:
   AX_sampling_interval_list();
   void  initialize(ENGN_DigitiserProperties const * pDP, CABFHeader const * pFH);
};

inline
AX_sampling_interval_list::AX_sampling_interval_list()
{
   memset(this, 0, sizeof(*this));
}

inline
void  AX_sampling_interval_list::initialize(ENGN_DigitiserProperties const * pDP, CABFHeader const * pFH)
{
   MEMBERASSERT();

   RPTRASSERT(pDP);
   RPTRASSERT(pFH);
   
   // Get the clock ratio to convert from hardware units to user units
   double   clock_ratio    = AX_get_clock_ratio_hardware_to_user(pDP, pFH);
   
   // Get the hardware limits in user units
   m_minimum_interval  = float(pDP->fMinPeriod          * clock_ratio);
   m_maximum_interval  = float(pDP->fMaxPeriod          * clock_ratio);
   m_resolution        = float(pDP->fSamplingResolution * clock_ratio);
   
   // Limit the sampling interval per-channel to 1 second
   if (m_maximum_interval > c_sampling_interval_maximum_interval)
      m_maximum_interval = c_sampling_interval_maximum_interval;
   
   // Build the list of "nice" sample intervals.
   // (Note that we say we only have one channel)
   m_number_of_entries = ABFH_SetupSamplingList(1, m_minimum_interval, m_maximum_interval, m_list_entries, ELEMENTS_IN(m_list_entries));
}

#endif
