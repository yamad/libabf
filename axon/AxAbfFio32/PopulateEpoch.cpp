//**********************************************************************************************
//
//    Copyright (c) 2002 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// MODULE:  PopulateEpoch.cpp
// PURPOSE: Contains functions that populate each epoch type.
// AUTHOR:  GRB  Mar 2002
//

#include "wincpp.hpp"
#include "PopulateEpoch.hpp"

const UINT c_uRESISTANCE_PULSEWIDTH_PROPORTION = 2;

void PopulateStep( int nDuration, double dLevel, double * pdValue )
{
   // Load the step level into the waveform array.
   for( int nSample = 0; nSample < nDuration; nSample++ )
      pdValue[nSample] = dLevel;
}

void PopulateRamp( int nDuration, double dStartLevel, double dLevel, double * pdValue )
{
   // Load the ramp into the waveform array.
   double dVal = dStartLevel;
   double dInc = (dLevel - dStartLevel ) / nDuration;
   for (int nSample = 0; nSample < nDuration; ++nSample)
   {
      dVal += dInc;
      pdValue[nSample] = dVal;
   }
}

void PopulateCosine( int nDuration, double dStartLevel, double dLevel, int nPeriod, double * pdValue )
{
   double const dTWOPI = atan2(0.0, -1.0) * 2.0;
   double const dScaleX = dTWOPI / double(nPeriod);

   // Compute the end of the last whole pulse
   // This can be made conditional if we allow partial pulses
   int nPartianPeriod  = nDuration % nPeriod;
   int nEndOfLastPulse = nDuration - nPartianPeriod;

   // Fill as many pulses as we can
   int nSample = 0;
   for (; nSample < nEndOfLastPulse; ++nSample) 
   {
      int nSampleWithinCurrentPeriod = nSample % nPeriod;
      
      // Compute the value
      pdValue[nSample] = cos(double(nSampleWithinCurrentPeriod) * dScaleX);
      // Flip it upside down and shift it so we start at zero, rise to +1, and fall to zero
      pdValue[nSample] = pdValue[nSample] * -0.5 + 0.5;
      // Scale it by the user's amplitude, and shift it by the last epoch/holding level
      pdValue[nSample] = pdValue[nSample] * (dLevel-dStartLevel) + dStartLevel;
   }

   // Fill the rest of the epoch with the starting level
   for (; nSample < nDuration; ++nSample) 
      pdValue[nSample] = dStartLevel;
}

void PopulateRectangle( int nDuration, double dStartLevel, double dLevel, int nPeriod, int nWidth, double * pdValue )
{
   // Compute the end of the last whole pulse
   // This can be made conditional if we allow partial pulses
   int nPartianPeriod = nDuration % nPeriod;
   int nEndOfLastPulse = nDuration;
   if (nPartianPeriod < nWidth)
      nEndOfLastPulse = nDuration - nPartianPeriod;

   // Fill as many pulses as we can
   int nSample = 0;
   for (; nSample < nEndOfLastPulse; ++nSample) 
   {
      if ((nSample % nPeriod) < nWidth)
         pdValue[nSample] = dLevel;
      else 
         pdValue[nSample] = dStartLevel;
   }

   // Fill the rest of the epoch with the starting level
   for (; nSample < nDuration; ++nSample) 
      pdValue[nSample] = dStartLevel;
}

void PopulateBiphasic( int nDuration, double dStartLevel, double dLevel, int nPeriod, int nWidth, double * pdValue )
{
   // Compute the end of the last whole pulse
   // This can be made conditional if we allow partial pulses
   int nPartianPeriod = nDuration % nPeriod;
   int nEndOfLastPulse = nDuration;
   if (nPartianPeriod < nWidth)
      nEndOfLastPulse = nDuration - nPartianPeriod;

   // Fill as many pulses as we can
   int nSample = 0;
   for (; nSample < nEndOfLastPulse; ++nSample) 
   {
      if ((nSample % nPeriod) < nWidth/2)     // First half of pulse
         pdValue[nSample] = dLevel;
      else if ((nSample % nPeriod) < nWidth)  // Second half of pulse
         pdValue[nSample] = dStartLevel - (dLevel - dStartLevel);
      else 
         pdValue[nSample] = dStartLevel;
   }

   // Fill the rest of the epoch with the starting level
   for (; nSample < nDuration; ++nSample) 
      pdValue[nSample] = dStartLevel;
}

void PopulateTriangle( int nDuration, double dStartLevel, double dLevel, int nPeriod, int nWidth, double * pdValue )
{
   // Compute the rise and fall slopes
   double dRiseSlope   = (dLevel - dStartLevel) / nWidth;
   double dFallSlope   = (dStartLevel - dLevel) / (nPeriod - nWidth);
   int nEndOfLastPulse = nDuration;

   // Compute the end of the last whole pulse
   // This can be made conditional if we allow partial pulses
   ASSERT( nPeriod != 0 );
   int nPartialPeriod = nDuration % nPeriod;
   nEndOfLastPulse = nDuration - nPartialPeriod;

   int nSample = 0;
   {
      // Fill as many pulses as we can
      for (; nSample < nEndOfLastPulse; ++nSample) 
      {
         int nSampleWithinCurrentPeriod = nSample % nPeriod;
         
         // Compute the value
         if (nSampleWithinCurrentPeriod < nWidth)
            pdValue[nSample] = dStartLevel + dRiseSlope * nSampleWithinCurrentPeriod;
         else 
            pdValue[nSample] = dLevel + dFallSlope * (nSampleWithinCurrentPeriod - nWidth);
      }
      // Fill the rest of the epoch with the inactive level
      for (; nSample < nDuration; ++nSample) {
         pdValue[nSample] = dStartLevel;
      }
   }
}

void PopulateResistance( int nDuration, double dLevel, double dHolding, double * pdValue )
{
   WARRAYASSERT( pdValue, nDuration );
   if( !pdValue )
      return;

   PopulateRectangle( nDuration, dLevel, dHolding, nDuration, nDuration / c_uRESISTANCE_PULSEWIDTH_PROPORTION, pdValue );
}