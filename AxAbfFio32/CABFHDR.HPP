//***********************************************************************************************
//
//    Copyright (c) 1994-2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CABFHDR.HPP
// PURPOSE: Contains class definition for CABFHeader class. A class wrapper for the
//          ABFFileHeader structure used in Axon Binary Files.
// AUTHOR:  BHI  Mar 1994
//          PRC  Sep 1998
//          CF   Apr 2000

#ifndef INC_CABFHDR_HPP
#define INC_CABFHDR_HPP
#include "\AxonDev\Comp\common\axodefn.h"
#include "\AxonDev\Comp\common\axodebug.h"
#include "\AxonDev\Comp\AxABFFIO32\abfheadr.h"
#include "\AxonDev\Comp\AxABFFIO32\abfutil.h"

#ifndef  CABFHEADER_FORCE_ACCESSORS
#define  CABFHEADER_FORCE_ACCESSORS             1
#endif

#define  CABFHEADER_HAVE_HIDDEN_INPUT_CHANNELS  0

//==============================================================================================
// CLASS:   CABFHeader.
// PURPOSE: Simple wrapper for the ABFFileHeader structure, building on the functionality
//          exported by ABFHEADR.CPP - part of the Axon File Support Pack.
// NOTES:   This class requires either a static or dynamic linkage to the ABFH_xxx functions
//          provided by ABFHEADR.CPP. This can be in the form of a dynamic link to the AxABFFIO32.DLL
//          dynamic link library, a static link to the ABFLIB.LIB static library, or by including
//          the module ABFHEADR.CPP as part of the project/make file.
//
class CABFHeader : public ABFFileHeader
{
public:     // Public member functions

   // Constructor. Initializes the object.
   CABFHeader();
   
   // // Copy constructor - use default.
   // CABFHeader(CABFHeader &FH);

   // Assignment from base class.
   const CABFHeader &operator=(const ABFFileHeader &FH);

   // Copy constructor from base class.
   CABFHeader(const ABFFileHeader &FH);
   
   // Destructor. Cleans up when object is destroyed.
   ~CABFHeader();

   // Comparison.
   BOOL operator==(CABFHeader &FH);

   // Helper functions
   void Initialize();
   void SetDateTime(UINT uStopWatch);
   void SetDateTimeFromParameter( SYSTEMTIME  SysTime, UINT uStopWatch );
   BOOL  GetStart_FILETIME       (FILETIME *       pStart) const;
   BOOL  GetStart_SYSTEMTIME     (SYSTEMTIME *     pStart) const;
   BOOL  GetStart_ULARGE_INTEGER (ULARGE_INTEGER * pStart) const;

   void GetADCDisplayRange(UINT uChannel, float *pfUUTop, float *pfUUBottom) const;
   void GetADCFullScaleRange(UINT uChannel, float *pfUUMax, float *pfUUMin) const;
   
   void GetADCtoUUFactors(UINT uChannel, float *pfADCToUUFactor, float *pfADCToUUShift) const;
   void GetVoltstoUUFactors(UINT uChannel, float *pfVoltToUUFactor, float *pfVoltToUUShift) const;
   
   short ClipADCValue(long lADCValue, BOOL &bClipped) const;
   float ClipADCUUValue(UINT uChannel, float fValue) const;
   short UUtoADC(UINT uChannel, float fUUValue, BOOL &bClipped) const;
   float ADCtoUU(UINT uChannel, short nADCValue) const;
   
   short VoltstoADC(UINT uChannel, float fVolts, BOOL &bClipped) const;
   float ADCtoVolts(UINT uChannel, short nADCValue) const;
   
   float UUtoVolts(UINT uChannel, float fUUValue) const;
   float VoltstoUU(UINT uChannel, float fVValue) const;
   
   void GetDACtoUUFactors(UINT uChannel, float *pfDACToUUFactor, float *pfDACToUUShift) const;
   short ClipDACValue(long lDACValue, BOOL &bClipped) const;
   float ClipDACUUValue(UINT uChannel, float fValue) const;
   short UUtoDAC(UINT uChannel, float fUUValue, BOOL &bClipped) const;
   float DACtoUU(UINT uChannel, short fDACValue) const;
   
   float SamplesToMS(int nSamples) const;

   int  MultiplexOffset(UINT uChannel) const;
   int  MultiplexOffset(LPCSTR pszSignal) const;

   // The number of channels/slice we recorded.
   int   get_channel_count_recorded() const;
   void  set_channel_count_recorded(int i);

   /**
   The number of channels/slice we acquired.
   If you think you need this you are probably wrong.
   You only need this if you are doing low-level digitiser stuff,
   or converting the sample rate from per-channel to the actual hardware clock.
   You probably need to use get_channel_count_recorded() instead.
   */
   int   get_channel_count_acquired() const;
   void  set_channel_count_acquired(int i);

   bool  is_channel_recorded(int channel) const;
   bool  is_channel_acquired(int channel) const;

   void  pad_input_channels();

   BOOL CheckTriggerChannel() const;
   BOOL SetThreshold(float fThreshold);
   BOOL ChangeThreshold(float fMultiplier);
   BOOL SetDACHolding(UINT uDACNum, float fDACHolding);
   
   BOOL IsThresholdTrigger() const;
   UINT GetTriggerChannel() const;
   UINT GetMxTriggerOffset() const;
   BOOL GetADCTriggerPolarity() const;
   
   BOOL CheckChannelNames() const;
   BOOL CheckUnits(LPCSTR pszUnits, char cUnitType, float *pfMultiplier);
   BOOL IsVClampMode( UINT uDAC, UINT uChannel, BOOL & bVClampMode );
   BOOL GetADCUnitMultiplier( UINT uChannel, float & m_fADCMultiplier );
   BOOL GetDACUnitMultiplier( UINT uDAC, float & m_fDACMultiplier );
   
   void SetADCName(UINT uChannel, LPCSTR ps);
   void GetADCName(UINT uChannel, char *ps, UINT uSize) const;
   
   void SetADCUnits(UINT uChannel, LPCSTR ps);
   void GetADCUnits(UINT uChannel, char *ps, UINT uSize) const;
   
   void SetMathUnits(LPCSTR ps);
   void GetMathUnits(char *ps, UINT uSize) const;
   void GetMathName(char *ps, UINT uSize) const;

   void SetProtocolPath( LPCSTR ps );
   void GetProtocolPath( LPSTR ps, UINT uSize) const;

   void SetDACName(UINT uChannel, LPCSTR ps);
   void GetDACName(UINT uChannel, char *ps, UINT uSize) const;
   
   void SetDACUnits(UINT uChannel, LPCSTR ps);
   void GetDACUnits(UINT uChannel, char *ps, UINT uSize) const;
   
   void SetCreatorInfo(HINSTANCE hInstance);
   void SetModifierInfo(HINSTANCE hInstance);
   void GetCreatorInfo(char *pszName, UINT uNameSize, char *pszVersion, UINT uVersionSize) const;
   void GetModifierInfo(char *pszName, UINT uNameSize, char *pszVersion, UINT uVersionSize) const;

   void SetComment(LPCSTR ps);
   void GetComment(char *ps, UINT uSize) const;
   
   void SetUserList( UINT uListNum, LPCSTR ps );
   void GetUserList( UINT uListNum, char *ps, UINT uSize ) const;

   void SetDACFilePath(UINT uChannel, LPCSTR ps);   
   void GetDACFilePath(UINT uChannel, char *ps, UINT uSize) const;
   
   BOOL  CheckMathChannels() const;
   BOOL  CheckMathExpression() const;
   float GetMathValue(float fUUValueA, float fUUValueB) const;

   // Clampex specific additions.
   UINT  GetHoldingDuration() const;
   float GetFirstHoldingLevel( UINT uDAC ) const;

   BOOL  CheckSampleIntervals(float fClockResolution) const;
   float GetRawSampleInterval() const;
   UINT  GetRawEpisodeLength() const;
   UINT  GetClockChange() const;
   UINT  GetOutputSequenceLength(BOOL bSeperateDigOut) const;
   UINT  GetNumActiveDACChannels() const;
   UINT  GetDACSamplingSeq(short *pnSamplingSeq, UINT uSeqSize, BOOL bSeperateDigOut) const;

   double GetEpisodeDuration() const;
   double GetPNDuration( UINT uChannel ) const;
   double GetTrainDuration( UINT uChannel ) const;
   double GetMetaEpisodeDuration() const;
   double GetEpisodeStartToStart() const;
   double GetRunDuration() const;
   double GetRunStartToStart() const;
   double GetTrialDuration() const;

   // Get the total number of raw samples to be acquired in a trial.
   UINT   GetRawTrialSamples() const;

   float GetFastInterval() const;
   void  SetFastInterval(float fFastInterval);

   BOOL  GetEpochLimits(int nADCNum, UINT uDACNum, UINT uEpisode, int nEpoch, 
                        UINT *puStart, UINT *puEnd, int *pnError=NULL) const;

   double SynchCountToMS(UINT uCount) const;
   UINT   MSToSynchCount(double dTimeMS) const;

   // Get / Set auto analysis parameters.
   void  SetAutoAnalysis( int nEnable, LPCSTR pszMacroName );
   int   GetAutoAnalysis( char *pszMacroName, UINT uSize ) const;

   // Get search and baseline regions.
   void GetSearchRegion( UINT uStatsRegion, UINT &uStartRegion, UINT &uEndRegion, UINT uEpisode ) const;
   void GetBaselineRegion( UINT &uStartRegion, UINT &uEndRegion, UINT uEpisode ) const;

   UINT GetFirstActiveStatsChannel() const;
   void GetStatsADCNames( char *ps, UINT uSize) const;
   BOOL GetStatRegionState( UINT uRegion ) const;
   int  GetNumberOfStatsRegions( void ) const;
   BOOL IsStatsEnabled() const;
   BOOL IsStatsChannelEnabled( UINT uChannel ) const;
   BOOL IsRegionStatsEnabled() const;
   int  GetChannelPeakPolarity( UINT uChannel ) const;
   void SetChannelPeakPolarity( UINT uChannel, int nPolarity );

   // Epoch resistance addition
   BOOL IsEpochResistanceEnabled( UINT uWaveformChannel ) const;
   BOOL IsEpochResistanceEnabled() const;
   void SetEpochResistanceSignalName( const UINT uWaveformChannel, LPCSTR ps );
   void GetEpochResistanceSignalName( const UINT uWaveformChannel, char *ps, UINT uSize ) const;
   void SetEpochResistanceState( const UINT uWaveformChannel, BOOL bResistanceActive );
   BOOL GetEpochResistanceState( const UINT uWaveformChannel );
   BOOL IsEpochResistanceSignal( UINT uDAC, UINT uChannel, BOOL & bValidSignal );

   // Alternating output addition
   void SetAlternateDACOutputEnable( BOOL bAlternating );
   BOOL GetAlternateDACOutputEnable() const;
   void SetAlternateDigitalOutputEnable( BOOL bAlternating );
   BOOL GetAlternateDigitalOutputEnable() const;
   
   // Refactoring functions from the broker into the header.
   // Mostly these are just arithemetic shortcuts.
   bool  DoesEpochDurationVary(UINT waveform) const;
   bool  DoesEpochDurationVary(UINT waveform, int epoch) const;
   int   GetEpochDurationInc( const UINT uWaveformChannel, const int nEpoch ) const;
   int   GetEpochInitDuration( const UINT uWaveformChannel, const int nEpoch ) const;
   void  GetEpochStartAndFinish(UINT waveform, int epoch, int sweep, int *pStart, int *pFinish) const;
   int   GetEpochType( const UINT uWaveformChannel, const int nEpoch ) const;
   int   GetParameterToVary( UINT uListNum ) const;
   bool  GetUserListEnable( UINT uListNum ) const;
   int   GetWFInfoFirstHoldingInterval() const;

   int   GetEpochDuration(UINT uDACChannel, UINT uEpisode, int nEpoch);
   float GetEpochLevel(UINT uDACChannel, UINT uEpisode, int nEpoch);
   bool  GetEpochLevelRange(UINT uDACChannel, int nEpoch, float *pfMin, float *pfMax);
   bool GetWaveformLevelRange(UINT uDACChannel, float *pfMin, float *pfMax );
   bool GetPNLevelRange(UINT uDACChannel, float *pfMin, float *pfMax );

   // Digidata 132x scope trigger.
   void SetDD132xTriggerOut( BOOL bEnable );
   BOOL GetDD132xTriggerOut() const;

   int GetOperationMode() const;

   // GUID Get / Set.
   void GetGUID( char *pszGuid, UINT uBufLen );
   bool SetGUID( const char *pszGuid );
   void SetGUID( const GUID guid );

   // To force the use of accessor functions we mark some of our members private
#if   CABFHEADER_FORCE_ACCESSORS
private:
   using ABFFileHeader::nADCNumChannels;
   using ABFFileHeader::channel_count_acquired;
#endif
};

STATIC_ASSERT(sizeof(CABFHeader) == sizeof(ABFFileHeader));

//==============================================================================================
// FUNCTION:   GetADCtoUUFactors
// PURPOSE:    Calculates the scaling factors used to convert ADC values to UserUnits.
// PARAMETERS:
//    uChannel        - The physical channel number to get the factors for.
//    pfADCToUUFactor - Pointers to return locations for scale and offset.
//    pfADCToUUShift    UserUnits = ADCValue * fADCToUUFactor + fADCToUUShift;
//
inline void CABFHeader::GetADCtoUUFactors(UINT uChannel, float *pfADCToUUFactor, float *pfADCToUUShift) const
{
   // Validate parameters.
   MEMBERASSERT();
   ASSERT(uChannel < ABF_ADCCOUNT);
   WPTRASSERT(pfADCToUUShift);
   WPTRASSERT(pfADCToUUFactor);
   
   ABFH_GetADCtoUUFactors(this, int(uChannel), pfADCToUUFactor, pfADCToUUShift);
}

//==============================================================================================
// FUNCTION:   ClipADCValue
// PURPOSE:    Limits the ADC value to the range of the A/D converter.
//
inline short CABFHeader::ClipADCValue(long lADCValue, BOOL &bClipped) const
{
   MEMBERASSERT();
   bClipped = TRUE;
   if (lADCValue >= lADCResolution)
      return short(lADCResolution-1);
   if (lADCValue <= -lADCResolution)
      return short(-lADCResolution+1);
   bClipped = FALSE;
   return short(lADCValue);
}

//==============================================================================================
// FUNCTION:   ClipADCUUValue
// PURPOSE:    Limits the UU value to the range of the A/D converter.
//
inline float CABFHeader::ClipADCUUValue(UINT uChannel, float fValue) const
{
   // Validate parameters.
   MEMBERASSERT();
   ASSERT(uChannel < ABF_ADCCOUNT);
   
   ABFH_ClipADCUUValue(this, int(uChannel), &fValue);
   return fValue;
}

//==============================================================================================
// FUNCTION:   UUtoADC
// PURPOSE:    Converts a floating point UU value to a two byte ADC value.
//
inline short CABFHeader::UUtoADC(UINT uChannel, float fUUValue, BOOL &bClipped) const
{
   MEMBERASSERT();
   float fADCToUUFactor, fADCToUUShift;
   GetADCtoUUFactors(uChannel, &fADCToUUFactor, &fADCToUUShift);
   return ClipADCValue(long((fUUValue-fADCToUUShift)/fADCToUUFactor+0.5F), bClipped);
}

//==============================================================================================
// FUNCTION:   ADCtoUU
// PURPOSE:    Converts a two byte ADC value to a floating point UU value.
//
inline float CABFHeader::ADCtoUU(UINT uChannel, short nADCValue) const
{
   MEMBERASSERT();
   float fADCToUUFactor, fADCToUUShift;
   GetADCtoUUFactors(uChannel, &fADCToUUFactor, &fADCToUUShift);
   return float(nADCValue*fADCToUUFactor+fADCToUUShift);
}


//==============================================================================================
// FUNCTION:   UUtoVolts
// PURPOSE:    Converts a floating point UU value to Volts at the ADC.
//
inline float CABFHeader::UUtoVolts(UINT uChannel, float fUUValue) const
{
   MEMBERASSERT();
   float fVoltToUUFactor, fVoltToUUShift;
   GetVoltstoUUFactors(uChannel, &fVoltToUUFactor, &fVoltToUUShift);
   return (fUUValue - fVoltToUUShift) / fVoltToUUFactor;
//   return (fUUValue - fInstrumentOffset[uChannel]) * fInstrumentScaleFactor[uChannel];
}


//==============================================================================================
// FUNCTION:   VoltstoUU
// PURPOSE:    Converts a floating point Voltage (at ADC) to user units.
//
inline float CABFHeader::VoltstoUU(UINT uChannel, float fVValue) const
{
   MEMBERASSERT();
   float fVoltToUUFactor, fVoltToUUShift;
   GetVoltstoUUFactors(uChannel, &fVoltToUUFactor, &fVoltToUUShift);
   return fVValue * fVoltToUUFactor + fVoltToUUShift;
//   return (fVValue / fInstrumentScaleFactor[uChannel]) + fInstrumentOffset[uChannel];
}


//==============================================================================================
// FUNCTION:   VoltstoADC
// PURPOSE:    Converts a value in Volts to an ADC value.
//
inline short CABFHeader::VoltstoADC(UINT uChannel, float fVolts, BOOL &bClipped) const
{
   MEMBERASSERT();
   long lADCValue = long(fVolts/fADCRange*lADCResolution*fADCProgrammableGain[uChannel]+0.5F);
   return ClipADCValue(lADCValue, bClipped);
}


//==============================================================================================
// FUNCTION:   UUtoVolts
// PURPOSE:    Converts an ADC value to Volts at the ADC.
//
inline float CABFHeader::ADCtoVolts(UINT uChannel, short nADCValue) const
{
   MEMBERASSERT();
   return float(nADCValue/fADCProgrammableGain[uChannel])*fADCRange/lADCResolution;
}

   
//==============================================================================================
// FUNCTION:   GetDACtoUUFactors
// PURPOSE:    Calculates the scaling factors used to convert DAC values to UserUnits.
// PARAMETERS:
//    uChannel        - The physical channel number to get the factors for.
//    pfDACToUUFactor - Pointers to return locations for scale and offset.
//    pfDACToUUShift    UserUnits = DACValue * fDACToUUFactor + fDACToUUShift;
//
inline void CABFHeader::GetDACtoUUFactors(UINT uChannel, float *pfDACToUUFactor, float *pfDACToUUShift) const
{
   // Validate parameters.
   MEMBERASSERT();
   ASSERT(uChannel < ABF_DACCOUNT);
   WPTRASSERT(pfDACToUUShift);
   WPTRASSERT(pfDACToUUFactor);
   
   ABFH_GetDACtoUUFactors(this, int(uChannel), pfDACToUUFactor, pfDACToUUShift);
}

//==============================================================================================
// FUNCTION:   ClipDACValue
// PURPOSE:    Limits the DAC value to the range of the A/D converter.
//
inline short CABFHeader::ClipDACValue(long lDACValue, BOOL &bClipped) const
{
   MEMBERASSERT();
   bClipped = TRUE;
   if (lDACValue >= lDACResolution)
      return short(lDACResolution-1);
   if (lDACValue <= -lDACResolution)
      return short(-lDACResolution+1);
   bClipped = FALSE;
   return short(lDACValue);
}

//==============================================================================================
// FUNCTION:   ClipDACUUValue
// PURPOSE:    Limits the UU value to the range of the D/A converter.
//
inline float CABFHeader::ClipDACUUValue(UINT uChannel, float fValue) const
{
   // Validate parameters.
   MEMBERASSERT();
   ASSERT(uChannel < ABF_DACCOUNT);
   
   ABFH_ClipDACUUValue(this, int(uChannel), &fValue);
   return fValue;
}

//==============================================================================================
// FUNCTION:   UUtoDAC
// PURPOSE:    Converts a floating point UU value to a two byte DAC value.
//
inline short CABFHeader::UUtoDAC(UINT uChannel, float fUUValue, BOOL &bClipped) const
{
   MEMBERASSERT();
   float fDACToUUFactor, fDACToUUShift;
   GetDACtoUUFactors(uChannel, &fDACToUUFactor, &fDACToUUShift);
   return ClipDACValue(long((fUUValue-fDACToUUShift)/fDACToUUFactor+0.5F), bClipped);
}

//==============================================================================================
// FUNCTION:   DACtoUU
// PURPOSE:    Converts a two byte DAC value to a floating point UU value.
//
inline float CABFHeader::DACtoUU(UINT uChannel, short nDACValue) const
{
   MEMBERASSERT();
   float fDACToUUFactor, fDACToUUShift;
   GetDACtoUUFactors(uChannel, &fDACToUUFactor, &fDACToUUShift);
   return float(nDACValue*fDACToUUFactor+fDACToUUShift);
}

//==============================================================================================
// FUNCTION:   IsThresholdTrigger
// PURPOSE:    Checks whether the trigger type is data-threshold crossing.
//
inline BOOL CABFHeader::IsThresholdTrigger() const
{
   MEMBERASSERT();
   return (nTriggerSource >= 0) || (nTriggerSource==ABF_TRIGGERFIRSTCHANNEL);
}

//==============================================================================================
// FUNCTION:   GetTriggerChannel
// PURPOSE:    Gets the channel number of the data stream to be used for threshold crossing detection.
//
inline UINT CABFHeader::GetTriggerChannel() const
{
   MEMBERASSERT();
   if (nTriggerSource >= 0)
      return UINT(nTriggerSource);
   ASSERT(nTriggerSource==ABF_TRIGGERFIRSTCHANNEL);
   return UINT(nADCSamplingSeq[0]);
}

//==============================================================================================
// FUNCTION: GetMathValue
// PURPOSE:  Combines the two channel values according to the math channel parameters.
//
inline float CABFHeader::GetMathValue(float fUUValueA, float fUUValueB) const
{
   MEMBERASSERT();
   float fRval = 0.0F;
   ABFH_GetMathValue(this, fUUValueA, fUUValueB, &fRval);
   return fRval;
}

//===============================================================================================
// METHOD:     GetEpochDurationInc
// TYPE:       Public Function
// PURPOSE:    Gets the duration increment of the specified epoch.
// ARGUMENTS:  nEpoch - The epoch.
// RETURNS:    The duration increment of the specified epoch
//
inline int CABFHeader::GetEpochDurationInc( const UINT uWaveformChannel, const int nEpoch ) const
{
   MEMBERASSERT();  
   return lEpochDurationInc[uWaveformChannel][ nEpoch ];
}

//===============================================================================================
// METHOD:     GetEpochInitDuration
// TYPE:       Public Function
// PURPOSE:    Gets the initial duration of the specified epoch.
// ARGUMENTS:  nEpoch - The epoch.
// RETURNS:    The initial duration of the specified epoch
//
inline int CABFHeader::GetEpochInitDuration( const UINT uWaveformChannel, const int nEpoch ) const
{
   MEMBERASSERT();  
   return lEpochInitDuration[uWaveformChannel][ nEpoch ];
}

//==============================================================================================
// FUNCTION: GetEpochLimits
// PURPOSE:  Gets the mx samples that bound a given epoch. 
//           ABFH_FIRSTHOLDING and ABFH_LASTHOLDING are both supported.
//
inline BOOL CABFHeader::GetEpochLimits(int nADCNum, UINT uDACNum, UINT uEpisode, int nEpoch, 
                                       UINT *puStart, UINT *puEnd, int *pnError) const
{
   MEMBERASSERT();
   return ABFH_GetEpochLimitsEx(this, nADCNum, uDACNum, uEpisode, nEpoch, puStart, puEnd, pnError);
}

//----------------------------------------------------------------------
// GetEpochStartAndFinish
//
// RETURNS:
// pStart      Receives the start of the epoch in samples (may be NULL)
// pFinish     Receives the end of the epoch in samples (may be NULL)
//
inline void CABFHeader::GetEpochStartAndFinish(UINT waveform, int epoch, int sweep, int *pStart, int *pFinish) const
{
   // Get the duration of first holding level (in samples)
   int nHolding = GetWFInfoFirstHoldingInterval();
   
   // Set the start of the epoch at the end of the first holding level
   int nStart    = nHolding;
   int nDuration = 0;
   int nDelta    = 0;
   
   // Search through the epochs until we get to our epoch.
   for (int nSearchEpoch = 0; nSearchEpoch <= epoch; ++nSearchEpoch) 
   {
      // Advance to the start of this epoch, using the duration from the previous epoch
      nStart += nDuration;
      
      // If this epoch is disabled it has zero length
      if ( GetEpochType( waveform, nSearchEpoch ) == ABF_WAVEFORMDISABLED) 
      {
         nDuration = 0;
         continue;
      }
      
      // If this is list epoch, it has zero length
      if (DoesEpochDurationVary( waveform, nSearchEpoch)) 
      {
         nDuration = 0;
         continue;
      }
      
      // Get the duration of this epoch, in samples
      nDelta      = GetEpochDurationInc( waveform, nSearchEpoch );
      nDuration   = GetEpochInitDuration( waveform, nSearchEpoch ) +  sweep * nDelta;
      
      // Do not allow the duration to go negative
      if ( nDuration < 0 )
         nDuration = 0;
   }
   
   // Get the end of the epoch (in samples)
   int   nFinish = nStart + nDuration;
   
   if (pStart)
   {
      WPTRASSERT(pStart);
      *pStart = nStart;
   }
   
   if (pFinish)
   {
      WPTRASSERT(pFinish);
      *pFinish = nFinish;
   }
}

//===============================================================================================
// METHOD:     GetEpochType
//
// PURPOSE:    Returns the epoch type of the specified epoch.
// ARGUMENTS:  nEpoch - The epoch.
// RETURNS:    Returns the epoch type of the specified epoch.
//
inline int CABFHeader::GetEpochType( const UINT uWaveformChannel, const int nEpoch ) const
{
   MEMBERASSERT();  
   return nEpochType[uWaveformChannel][ nEpoch ];
}


//===============================================================================================
// FUNCTION: SynchTimeToMS
// PURPOSE:  Converts the count in a synch entry to time in milli-seconds.
//
inline double CABFHeader::SynchCountToMS(UINT uCount) const
{
   MEMBERASSERT();
   double dTimeMS = 0.0;
   ABFH_SynchCountToMS(this, uCount, &dTimeMS);
   return dTimeMS;
}

//===============================================================================================
// FUNCTION: MSToSynchCount
// PURPOSE:  Converts a time value to a synch time count.
//
inline UINT CABFHeader::MSToSynchCount(double dTimeMS) const
{
   MEMBERASSERT();
   return ABFH_MSToSynchCount(this, dTimeMS);
}

//==============================================================================================
// FUNCTION:   IsStatsEnabled
// PURPOSE:    Checks whether region stats or threshold triggered stats are enabled
//
inline BOOL CABFHeader::IsStatsEnabled() const
{
   MEMBERASSERT();

   BOOL bStatisticsEnabled = FALSE;
   
   // Enabled statistics is applicable to all stats regions.
   if( nOperationMode == ABF_HIGHSPEEDOSC || nOperationMode == ABF_WAVEFORMFILE )
      bStatisticsEnabled = (BOOL)nStatsEnable;
   else
      bStatisticsEnabled = (BOOL)nChannelStatsStrategy;

   return bStatisticsEnabled;
}

//==============================================================================================
// FUNCTION:   IsRegionStatsEnabled
// PURPOSE:    Checks whether region stats are enabled
//
inline BOOL CABFHeader::IsRegionStatsEnabled() const
{
   MEMBERASSERT();

   // First array member is stats enable flag
   BOOL bRegionStatsEnabled = FALSE;

   if( nStatsEnable )
      bRegionStatsEnabled = TRUE;

   return bRegionStatsEnabled;
}

//===============================================================================================
// METHOD:     SetEpochResistanceState
// PURPOSE:    Set the resistance epoch state
//             Determines whether the resistance epoch can be applied
//
inline void CABFHeader::SetEpochResistanceState( const UINT uWaveformChannel, BOOL bResistanceActive )
{
   MEMBERASSERT();
   ASSERT(uWaveformChannel <= ABF_WAVEFORMCOUNT );

   nEpochResistanceState[ uWaveformChannel ] = (short)bResistanceActive;
}

//===============================================================================================
// METHOD:     GetEpochResistanceState
// PURPOSE:    Get the resistance epoch state
//
inline BOOL CABFHeader::GetEpochResistanceState( const UINT uWaveformChannel )
{
   MEMBERASSERT();
   ASSERT(uWaveformChannel <= ABF_WAVEFORMCOUNT );

   return (BOOL)nEpochResistanceState[ uWaveformChannel ];
}

//===============================================================================================
// FUNCTION: SetAlternateDACOutputsEnable
// PURPOSE:  Set the alternating DAC output state
//           
inline void CABFHeader::SetAlternateDACOutputEnable( BOOL bAlternating )
{
   MEMBERASSERT();
   
   nAlternateDACOutputState = (short)bAlternating;
}

//===============================================================================================
// FUNCTION: GetAlternateDACOutputsEnable
// PURPOSE:  Get the alternating DAC output state
//           
inline BOOL CABFHeader::GetAlternateDACOutputEnable() const
{
   MEMBERASSERT();
   
   return (BOOL)nAlternateDACOutputState;
}

//===============================================================================================
// FUNCTION: SetAlternateDigitalOutputsEnable
// PURPOSE:  Set the alternating digital output state
//           
inline void CABFHeader::SetAlternateDigitalOutputEnable( BOOL bAlternating )
{
   MEMBERASSERT();
   
   nAlternateDigitalOutputState = (short)bAlternating;
}

//===============================================================================================
// FUNCTION: GetAlternateDigitalOutputsEnable
// PURPOSE:  Get the alternating Digital output state
//           
inline BOOL CABFHeader::GetAlternateDigitalOutputEnable() const
{
   MEMBERASSERT();
   
   return (BOOL)nAlternateDigitalOutputState;
}

inline int CABFHeader::get_channel_count_recorded() const
{
   MEMBERASSERT();
   
   return (int) nADCNumChannels;
}

inline void CABFHeader::set_channel_count_recorded(int i)
{
   MEMBERASSERT();
   ASSERT(0 <= i && i <= ABF_ADCCOUNT);
   
   nADCNumChannels = (short) i;
}

inline int CABFHeader::get_channel_count_acquired() const
{
   MEMBERASSERT();
   
   int count = (int) channel_count_acquired;

   // Support for early file versions.
   if (!count)
      count = (int) nADCNumChannels;

   return count;
}

inline void CABFHeader::set_channel_count_acquired(int i)
{
   MEMBERASSERT();
   ASSERT(0 <= i && i <= ABF_ADCCOUNT);
   
   channel_count_acquired = (short) i;
}

#endif   // INC_CABFHDR_HPP
