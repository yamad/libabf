//***********************************************************************************************
//
//    Copyright (c) 1994-2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CABFHDR.CPP
// PURPOSE: Contains member functions for the class CABFHeader. A class wrapper for the
//          ABFFileHeader structure.
// AUTHOR:  BHI  Mar 1994
//          PRC  Sep 1998
//          CF   Apr 2000
//

#include "wincpp.hpp"
#include "\AxonDev\Comp\AxABFFIO32\cabfhdr.hpp"
#include "\AxonDev\Comp\AxoUtils32\axoutils32.h"
#include "\AxonDev\Comp\Common\version.hpp"

#include <math.h>
#include <time.h>
#include <limits.h>

//==============================================================================================
// FUNCTION: Initialize.
// PURPOSE:  Initialize all the fields of the base class structure to valid values.
//
void CABFHeader::Initialize()
{
   MEMBERASSERT();
   ABFH_Initialize(this);
}

//==============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes the object on creation.
//
CABFHeader::CABFHeader()
{
   MEMBERASSERT();
   Initialize();
}

//==============================================================================================
// FUNCTION:   Overloaded assignment operator
// PURPOSE:    Assignment from base class.
//
const CABFHeader& CABFHeader::operator=(const ABFFileHeader &FH)
{
   MEMBERASSERT();
   
   // Check we are working with a "new" header.
   ASSERT( ABFH_IsNewHeader( &FH ) );

   *(ABFFileHeader *)this = FH;
   return *this;
}

//==============================================================================================
// FUNCTION:   Overloaded copy constructor
// PURPOSE:    Copy constructor from base class.
//
CABFHeader::CABFHeader(const ABFFileHeader &FH)
{
   MEMBERASSERT();
   *this = FH;    // Let the overloaded operator=() function do the work.
}

//==============================================================================================
// FUNCTION:   Destructor
// PURPOSE:    Cleans up the object when it is destroyed.
//
CABFHeader::~CABFHeader()
{
   MEMBERASSERT();
}

//==============================================================================================
// FUNCTION: SetDateTime
// PURPOSE:  Sets the date and time for the header to the current date and time.
void CABFHeader::SetDateTime(UINT uStopWatch)
{
   MEMBERASSERT();

   SYSTEMTIME Time = { 0 };
   GetLocalTime(&Time);
   
   SetDateTimeFromParameter( Time, uStopWatch );
}

//==============================================================================================
// FUNCTION: SetDateTimeFromParameter
// PURPOSE:  Sets the file start time and date from a parameter value rather than the local time.
// NOTES:    Year is now (ABF v1.6) stored as a 4 digit year.
//
void CABFHeader::SetDateTimeFromParameter( SYSTEMTIME  SysTime, UINT uStopWatch )
{
   MEMBERASSERT();

   lFileStartDate      = long(SysTime.wYear*10000 + SysTime.wMonth*100 + SysTime.wDay);
   lFileStartTime      = long(((SysTime.wHour*60) + SysTime.wMinute)*60 + SysTime.wSecond);
   nFileStartMillisecs = short(SysTime.wMilliseconds);
   lStopwatchTime      = long(uStopWatch);
}

//==============================================================================================
// FUNCTION: SetADCName
// PURPOSE:  Set an ADC name from a C string.
//
void CABFHeader::SetADCName(UINT uChannel, LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ASSERT(uChannel < ABF_ADCCOUNT);
   ABF_SET_STRING(sADCChannelName[uChannel], ps);
}

//==============================================================================================
// FUNCTION: GetADCName
// PURPOSE:  Return an ADC name as a C string.
//
void CABFHeader::GetADCName(UINT uChannel, char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ASSERT(uChannel < ABF_ADCCOUNT);
   ABF_GET_STRING(ps, sADCChannelName[uChannel], uSize);
   if (ps[0]=='\0')
      _snprintf(ps, uSize, "IN #%d", nADCPtoLChannelMap[uChannel]);
}

//==============================================================================================
// FUNCTION: SetMathUnits
// PURPOSE:  Set the math channel units from a C string.
//
void CABFHeader::SetMathUnits(LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ABF_SET_STRING(sArithmeticUnits, ps);
}

//==============================================================================================
// FUNCTION: GetMathName
// PURPOSE:  Return the name of the math channel as a C string.
//
void CABFHeader::GetMathName(char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABFH_GetMathChannelName(ps, uSize);
}

//==============================================================================================
// FUNCTION: GetMathUnits
// PURPOSE:  Return the math channel units as a C string.
//
void CABFHeader::GetMathUnits(char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABF_GET_STRING(ps, sArithmeticUnits, uSize);
}

//==============================================================================================
// FUNCTION: SetADCUnits
// PURPOSE:  Set an ADC units from a C string.
//
void CABFHeader::SetADCUnits(UINT uChannel, LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ASSERT(uChannel < ABF_ADCCOUNT);
   ABF_SET_STRING(sADCUnits[uChannel], ps);
}

//==============================================================================================
// FUNCTION: GetADCUnits
// PURPOSE:  Return an ADC units as a C string.
//
void CABFHeader::GetADCUnits(UINT uChannel, char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ASSERT(uChannel < ABF_ADCCOUNT);
   ABF_GET_STRING(ps, sADCUnits[uChannel], uSize);
}

//==============================================================================================
// FUNCTION: SetDACName
// PURPOSE:  Sets a DAC name from a C string.
//
void CABFHeader::SetDACName(UINT uChannel, LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ASSERT(uChannel < ABF_DACCOUNT);
   ABF_SET_STRING(sDACChannelName[uChannel], ps);
}

//==============================================================================================
// FUNCTION: GetDACName
// PURPOSE:  Return a DAC name as a C string.
//
void CABFHeader::GetDACName(UINT uChannel, char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ASSERT(uChannel < ABF_DACCOUNT);
   ABF_GET_STRING(ps, sDACChannelName[uChannel], uSize);
   if (ps[0]=='\0')
      _snprintf(ps, uSize, "OUT #%u", uChannel);
}

//==============================================================================================
// FUNCTION: SetDACUnits
// PURPOSE:  Sets a DAC units from a C string.
//
void CABFHeader::SetDACUnits(UINT uChannel, LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ABF_SET_STRING(sDACChannelUnits[uChannel], ps);
}

//==============================================================================================
// FUNCTION: GetDACUnits
// PURPOSE:  Return a DAC units as a C string.
//
void CABFHeader::GetDACUnits(UINT uChannel, char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABF_GET_STRING(ps, sDACChannelUnits[uChannel], uSize);
}

//==============================================================================================
// FUNCTION: SetComment
// PURPOSE:  Set the comment string in the ABF header.
//
void CABFHeader::SetComment(LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ABF_SET_STRING(sFileComment, ps);
}

//==============================================================================================
// FUNCTION: GetComment
// PURPOSE:  Get the comment string from the ABF header.
//
void CABFHeader::GetComment(char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABF_GET_STRING(ps, sFileComment, uSize);
}

static void GetHostExeName(HINSTANCE hInstance, LPSTR pszName, UINT uMaxLen)
{
   char szPath[_MAX_PATH] = "", szName[_MAX_FNAME] = "";
   VERIFY(GetModuleFileName(hInstance, szPath, ELEMENTS_IN(szPath)));
   _splitpath( szPath, NULL, NULL, szName, NULL );
   strncpy(pszName, szName, uMaxLen-1);
   pszName[uMaxLen-1] = '\0';
}

//===============================================================================================
// FUNCTION: SetCreatorInfo
// PURPOSE:  Sets the creator name and version numbers using the instance handle.
//
void CABFHeader::SetCreatorInfo(HINSTANCE hInstance)
{
   MEMBERASSERT();
   
   // Creator name.
   {
      char szName[_MAX_FNAME] = "";
      GetHostExeName(hInstance, szName, ELEMENTS_IN(szName));
      ABF_SET_STRING(sCreatorInfo, szName);
   }

   // Creator version number.
   BYTE byVersion[4] = { 0 };
   {
      char szFileVer[32] = "";
      CVersion AppVer(hInstance);
      AppVer.GetFileVersion(szFileVer, ELEMENTS_IN(szFileVer));
      AXU_SplitVersionNumber( szFileVer, &byVersion[0] );
   }

   nCreatorMajorVersion  = (short)byVersion[0];
   nCreatorMinorVersion  = (short)byVersion[1];
   nCreatorBugfixVersion = (short)byVersion[2];
   nCreatorBuildVersion  = (short)byVersion[3];
}

//===============================================================================================
// FUNCTION: SetModifierInfo
// PURPOSE:  Sets the modifier name and version numbers using the instance handle.
//
void CABFHeader::SetModifierInfo(HINSTANCE hInstance)
{
   MEMBERASSERT();
   
   // Creator name.
   {
      char szName[_MAX_FNAME] = "";
      GetHostExeName(hInstance, szName, ELEMENTS_IN(szName));
      ABF_SET_STRING(sModifierInfo, szName);
   }

   // Creator version number.
   BYTE byVersion[4] = { 0 };
   {
      char szFileVer[32] = "";
      CVersion AppVer(hInstance);
      AppVer.GetFileVersion(szFileVer, ELEMENTS_IN(szFileVer));
      AXU_SplitVersionNumber(szFileVer, &byVersion[0]);
   }

   nModifierMajorVersion  = (short)byVersion[0];
   nModifierMinorVersion  = (short)byVersion[1];
   nModifierBugfixVersion = (short)byVersion[2];
   nModifierBuildVersion  = (short)byVersion[3];
}

//==============================================================================================
// FUNCTION:   GetCreatorInfo
// PURPOSE:    Returns the sCreatorInfo and version nyumber fields of the ABF header, stripping blanks.
//
void CABFHeader::GetCreatorInfo(char *pszName, UINT uNameSize, char *pszVersion, UINT uVersionSize) const
{
   MEMBERASSERT();

   if( pszName )
   {
      ARRAYASSERT(pszName, uNameSize);
      ABF_GET_STRING(pszName, sCreatorInfo, uNameSize);

      // in older headers blat engine dll name with the app name
      char szEngnName[] = "AXENGN";
      if (strncmp(pszName, szEngnName, strlen(szEngnName)) == 0)
         AXU_strncpyz(pszName, "clampex", uNameSize);
   }

   if( pszVersion )
   {
      ARRAYASSERT(pszVersion, uVersionSize);
      if (nCreatorMajorVersion == 0)
         AXU_strncpyz(pszVersion, "pre-9.0", uVersionSize);
      else
         AXU_sprintf( pszVersion, uVersionSize, "%d.%d.%d.%d", 
                      nCreatorMajorVersion, nCreatorMinorVersion, nCreatorBugfixVersion, nCreatorBuildVersion );
   }
}

//==============================================================================================
// FUNCTION:   GetModifierInfo
// PURPOSE:    Returns the sCreatorInfo and version nyumber fields of the ABF header, stripping blanks.
//
void CABFHeader::GetModifierInfo(char *pszName, UINT uNameSize, char *pszVersion, UINT uVersionSize) const
{
   MEMBERASSERT();

   if( pszName )
   {
      ARRAYASSERT(pszName, uNameSize);
      ABF_GET_STRING(pszName, sModifierInfo, uNameSize);
   }

   if( pszVersion )
   {
      ARRAYASSERT(pszVersion, uVersionSize);
      AXU_sprintf( pszVersion, uVersionSize, "%d.%d.%d.%d", 
                   nModifierMajorVersion, nModifierMinorVersion, nModifierBugfixVersion, nModifierBuildVersion );
   }
}

//==============================================================================================
// FUNCTION:   SetUserList
// PURPOSE:    Sets the sParamValueList field of the ABF header, padding with blanks.
// PARAMETERS:
//    ps - zero terminated string containing the UserList information.
//
void CABFHeader::SetUserList( UINT uListNum, LPCSTR ps )
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ABF_SET_STRING(sULParamValueList[uListNum], ps);
}

   
//==============================================================================================
// FUNCTION:   GetUserList
// PURPOSE:    Returns the sParamValueList field of the ABF header, stripping blanks.
// PARAMETERS:
//    ps - zero terminated string containing the UserList information.
//
void CABFHeader::GetUserList( UINT uListNum, char *ps, UINT uSize ) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABF_GET_STRING(ps, sULParamValueList[uListNum], uSize);
}

   
//==============================================================================================
// FUNCTION:   SetProtocolPath
// PURPOSE:    Sets the sProtocolPath field of the ABF header, padding with blanks.
// PARAMETERS:
//    ps - zero terminated string containing the ProtocolPath information.
//
void CABFHeader::SetProtocolPath( LPCSTR ps )
{
   MEMBERASSERT();
   LPSZASSERT(ps);

   ABF_SET_STRING( sProtocolPath, ps) ;
}


//==============================================================================================
// FUNCTION:   GetProtocolPath
// PURPOSE:    Gets the sProtocolPath field of the ABF header, unpadding with blanks.
// PARAMETERS:
//    psz - zero terminated string containing the ProtocolPath information.
//
void CABFHeader::GetProtocolPath( LPSTR psz, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(psz, uSize);
   ABF_GET_STRING(psz, sProtocolPath, uSize);
}


//==============================================================================================
// FUNCTION:   SetDACFilePath
// PURPOSE:    Sets the sDACFilePath field of the ABF header, padding with blanks.
// PARAMETERS:
//    ps - zero terminated string containing the DACFilePath information.
//
void CABFHeader::SetDACFilePath(UINT uChannel, LPCSTR ps)
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ABF_SET_STRING(sDACFilePath[uChannel], ps);
}


//==============================================================================================
// FUNCTION:   GetDACFilePath
// PURPOSE:    Returns the sDACFilePath field of the ABF header, stripping blanks.
// PARAMETERS:
//    ps - zero terminated string containing the DACFilePath information.
//
void CABFHeader::GetDACFilePath(UINT uChannel, char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABF_GET_STRING(ps, sDACFilePath[uChannel], uSize);
}

bool CABFHeader::DoesEpochDurationVary(UINT uListNum) const
{
   if (GetUserListEnable(uListNum)) 
   {
      int nParam = GetParameterToVary(uListNum);
      if ((nParam >= ABF_EPOCHINITDURATION) && (nParam < (ABF_EPOCHINITDURATION + ABF_EPOCHCOUNT)))
         return true;
   }
   return false;
}

bool CABFHeader::DoesEpochDurationVary(UINT uListNum, int nEpoch) const
{
   if (GetUserListEnable(uListNum)) 
   {
      int nParam = GetParameterToVary(uListNum);
      if (nParam == (ABF_EPOCHINITDURATION + nEpoch))
         return true;
   }
   return false;
}

//==============================================================================================
// FUNCTION:   GetADCDisplayRange
// PURPOSE:    Calculates the upper and lower limits of the display given the display
//             amplification and offset in the ABF header for this ADC channel.
// PARAMETERS:
//    uChannel   - The physical ADC channel number to get the factors for.
//    pfUUTop    - Pointers to return locations for top and bottom of the display.
//    pfUUBottom
//
void CABFHeader::GetADCDisplayRange(UINT uChannel, float *pfUUTop, float *pfUUBottom) const
{
   // Validate parameters.
   MEMBERASSERT();
   ASSERT(uChannel < ABF_ADCCOUNT);
   WPTRASSERT(pfUUTop);
   WPTRASSERT(pfUUBottom);
   
   ABFH_GetADCDisplayRange(this, uChannel, pfUUTop, pfUUBottom);
}


//==============================================================================================
// FUNCTION:   GetADCFullScaleRange
// PURPOSE:    Calculates the upper and lower limits of full scale range of this
//             ADC channel.
// PARAMETERS:
//    uChannel   - The physical ADC channel number to get the factors for.
//    pfUUMax    - Pointers to return locations for max and min values.
//    pfUUMin
//
void CABFHeader::GetADCFullScaleRange(UINT uChannel, float *pfUUMax, float *pfUUMin) const
{
   // Validate parameters.
   MEMBERASSERT();
   ASSERT(uChannel < ABF_ADCCOUNT);
   WPTRASSERT(pfUUMax);
   WPTRASSERT(pfUUMin);
   float fMax = ADCtoUU(uChannel, short(lADCResolution-1));
   float fMin = ADCtoUU(uChannel, short(-lADCResolution));
   if (fMax < fMin)
   {
      float fTemp = fMax;
      fMax = fMin;
      fMin = fTemp;
   }
   *pfUUMax = fMax;
   *pfUUMin = fMin;
}


void  CABFHeader::pad_input_channels()
{
   MEMBERASSERT();

   channel_count_acquired = 0;
#if CABFHEADER_HAVE_HIDDEN_INPUT_CHANNELS
   set_channel_count_acquired(get_channel_count_recorded());
   // Pad the extra channels until we meet the hardware requirements
   int outputs = GetNumActiveDACChannels();
   if (outputs) 
   {
      while ((channel_count_acquired < outputs) || (channel_count_acquired % outputs)) 
      {
         // Find an unused channel
         int i;
         for (i = 0; i < ABF_ADCCOUNT; ++i) 
         {
            if (!is_channel_acquired(i))
               break;
         }
         if (i == ABF_ADCCOUNT) 
         {
            ASSERT(0 && "pad_input_channels - no available channels");
            break;
         }
         nADCSamplingSeq[channel_count_acquired++] = (short)i;
      }
   }
   // Mark the remaining channels invalid
   for (int i = channel_count_acquired; i < ABF_ADCCOUNT; ++i) 
      nADCSamplingSeq[i] = -1;
#endif
}

//==============================================================================================
// FUNCTION: SamplesToMS
// PURPOSE:  Converts a sample offset within an episode into ms since the start of the episode.
//
float CABFHeader::SamplesToMS(int nSamples) const
{
   UINT uClockChange = GetClockChange();

   if ((nOperationMode!=ABF_WAVEFORMFILE) || (nSamples <= int(uClockChange)))
      return nSamples * fADCSampleInterval / 1000.0F;
   
   return (uClockChange * fADCSampleInterval + 
           (nSamples-uClockChange) * fADCSecondSampleInterval) / 1000.0F;
}

//==============================================================================================
// FUNCTION:   SetThreshold
// PURPOSE:    Sets the threshold value, limiting the new value to the range of the A/D converter.
//
BOOL CABFHeader::SetThreshold(float fThreshold)
{
   MEMBERASSERT();
   if (!IsThresholdTrigger())
      return FALSE;
   fTriggerThreshold = ClipADCUUValue(GetTriggerChannel(), fThreshold);
   return (fTriggerThreshold==fThreshold);
}


//==============================================================================================
// FUNCTION:   Change threshold
// PURPOSE:    Changes the threshold value by a percentage of the full scale range, limiting
//             the calculated value to the range of the A/D converter.
// PARAMETERS:
//    fPercent - The percentage change requested.
//
BOOL CABFHeader::ChangeThreshold(float fPercent)
{
   MEMBERASSERT();
   if (!IsThresholdTrigger())
      return FALSE;
   float fADCToUUFactor, fADCToUUShift;
   GetADCtoUUFactors(GetTriggerChannel(), &fADCToUUFactor, &fADCToUUShift);

   float fUURange = (lADCResolution-1) * fADCToUUFactor;
   fTriggerThreshold += fUURange * fPercent/100.0F;
   
   // Limit the threshold level to values representable in 16 bit ADC values,
   // keeping one ADC count away from the absolute extremes of the range.
   float fUUMax = (lADCResolution-2) * fADCToUUFactor + fADCToUUShift;
   float fUUMin = (-lADCResolution+1)* fADCToUUFactor + fADCToUUShift;
   if (fUUMax < fUUMin)
   {
      float fTemp = fUUMax;
      fUUMax      = fUUMin;
      fUUMin      = fTemp;
   }
   if (fTriggerThreshold > fUUMax)
   {
      fTriggerThreshold = fUUMax;
      return FALSE;
   }
   if (fTriggerThreshold < fUUMin)
   {
      fTriggerThreshold = fUUMin;
      return FALSE;
   }
   return TRUE;
}

//==============================================================================================
// FUNCTION:   SetDACHolding
// PURPOSE:    Sets the holding level output on a D/A channel.
//
BOOL CABFHeader::SetDACHolding(UINT uDACChannel, float fDACHolding)
{
   MEMBERASSERT();
   if (uDACChannel >= ABF_DACCOUNT)
      return FALSE;
      
   float fDACToUUFactor, fDACToUUShift;
   GetDACtoUUFactors(uDACChannel, &fDACToUUFactor, &fDACToUUShift);

   // Limit the threshold level to values representable in 16 bit ADC values,
   // keeping one ADC count away from the absolute extremes of the range.
   float fUUTop    = (lDACResolution-2) * fDACToUUFactor + fDACToUUShift;
   float fUUBottom = (-lDACResolution+1)* fDACToUUFactor + fDACToUUShift;

   // Negative scaling factors will cause the "top" to be less than the "bottom"
   if (fUUTop < fUUBottom)
   {
      float fTemp = fUUBottom;
      fUUBottom   = fUUTop;
      fUUTop      = fTemp;
   }
   
   BOOL bRval = TRUE;
   if (fDACHolding > fUUTop)
   {
      fDACHolding = fUUTop;
      bRval = FALSE;
   }
   if (fDACHolding < fUUBottom)
   {
      fDACHolding = fUUBottom;
      bRval = FALSE;
   }
   fDACHoldingLevel[uDACChannel] = fDACHolding;
   return bRval;
}


//==============================================================================================
// FUNCTION:   CheckChannelNames
// PURPOSE:    Checks that all AI names are not empty.
//
BOOL CABFHeader::CheckChannelNames() const
{
   MEMBERASSERT();
   
   // Characters that are regarded as white space.
   static char szWhiteSpace[] = " \t\r\n";
   char szName[ABF_ADCNAMELEN+1];
   
   for (short i=0; i<nADCNumChannels; i++)
   {
      // Get a pointer to the next channel name.
      strncpy(szName, sADCChannelName[nADCSamplingSeq[i]], ABF_ADCNAMELEN);
      szName[ABF_ADCNAMELEN] = '\0';
      
      // Skip leading white space.
      char *p = szName + strspn(szName, szWhiteSpace);
      
      // If nothing left, return.
      if (*p=='\0')
         return FALSE;
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: CheckUnits
// PURPOSE:  Check that the units string is a particular type modified by an SI multiplier.
//
BOOL CABFHeader::CheckUnits(LPCSTR pszUnits, char cUnitType, float *pfMultiplier)
{
   LPSZASSERT(pszUnits);

   if (pfMultiplier)
      *pfMultiplier = 1.0F;
      
   if (toupper(pszUnits[0]) != toupper(cUnitType))
   {
      if (!AXU_SIPrefixToMultiplier( pszUnits[0], pfMultiplier ))
         return FALSE;
      if (pszUnits[1] != cUnitType)
         return FALSE;
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: IsVClampMode
// PURPOSE:  Check the input and output units are compatiable with either VClamp or IClamp modes
//
// PARAMETERS:
//    uChannel    Physical channel number to check.
//    bVClampMode Filled in as TRUE if VClamp Mode, FALSE if IClamp mode
// RETURNS:       TRUE if either VClamp or IClamp mode otherwise FALSE

BOOL CABFHeader::IsVClampMode( UINT uDAC, UINT uChannel, BOOL & bVClampMode )
{
   MEMBERASSERT();

   bVClampMode = FALSE;
   float fADCMultiplier = 1.0F;
   float fDACMultiplier = 1.0F;

   // Decode units description in the ABF header.   
   char szADCUnits[ ABF_ADCUNITLEN + 1 ];
   GetADCUnits( uChannel, szADCUnits, sizeof(szADCUnits) );

   char szDACUnits[ ABF_DACUNITLEN + 1 ];
   GetDACUnits( uDAC, szDACUnits, sizeof(szDACUnits) );
   
   BOOL bInputIsCurrent  = CheckUnits( szADCUnits, 'A', &fADCMultiplier );
   BOOL bInputIsVoltage  = CheckUnits( szADCUnits, 'V', &fADCMultiplier );
   BOOL bOutputIsCurrent = CheckUnits( szDACUnits, 'A', &fDACMultiplier );
   BOOL bOutputIsVoltage = CheckUnits( szDACUnits, 'V', &fDACMultiplier );
   
   if( bInputIsCurrent && bOutputIsVoltage )
   {
      bVClampMode = TRUE;
      return TRUE;
   }
   else if( bInputIsVoltage && bOutputIsCurrent )
   {
      bVClampMode = FALSE;
      return TRUE;
   }
   return FALSE;
}

//===============================================================================================
// FUNCTION: GetADCUnitMultiplier
// PURPOSE:  Get the input multiplier to calibrate in absolute units
//
BOOL CABFHeader::GetADCUnitMultiplier( UINT uChannel, float & m_fADCMultiplier )
{
   MEMBERASSERT();

   // Get the multiplier for selected input channel
   char szADCUnits[ ABF_ADCUNITLEN + 1 ];
   GetADCUnits( uChannel, szADCUnits, sizeof(szADCUnits) );
   AXU_SIPrefixToMultiplier( szADCUnits[0], &m_fADCMultiplier );

   return TRUE;
}

//===============================================================================================
// FUNCTION: GetDACUnitMultiplier
// PURPOSE:  Get the output multiplier to calibrate in absolute units
//
BOOL CABFHeader::GetDACUnitMultiplier( UINT uDAC, float & m_fDACMultiplier )
{
   MEMBERASSERT();

   // Get the multiplier for selected output channel
   char szDACUnits[ ABF_ADCUNITLEN + 1 ];
   GetDACUnits( uDAC, szDACUnits, sizeof(szDACUnits) );
   AXU_SIPrefixToMultiplier( szDACUnits[0], &m_fDACMultiplier );
   
   return TRUE;
}

//==============================================================================================
// FUNCTION:   MultiplexOffset
// PURPOSE:    Returns an index into the sampling sequence that corresponds to the given ADC
//             channel.
// PARAMETERS:
//    uChannel - Physical channel number to look for.
// RETURNS:    > 0 - The position that the ADC channel was first found in the sampling sequence.
//             -1  - The ADC channel is not present in the sampling sequence.
//
int CABFHeader::MultiplexOffset(UINT uChannel) const
{
   MEMBERASSERT();
   for (int i=0; i<nADCNumChannels; i++)
      if (UINT(nADCSamplingSeq[i])==uChannel)
         return i;
   return ABF_UNUSED_CHANNEL;
}

//==============================================================================================
// FUNCTION:   MultiplexOffset
// PURPOSE:    Returns an index into the sampling sequence that corresponds to the given ADC
//             signal name.
// PARAMETERS:
//    uChannel - Physical channel number to look for.
// RETURNS:    > 0 - The position that the ADC channel was first found in the sampling sequence.
//             -1  - The ADC channel is not present in the sampling sequence.
//
int CABFHeader::MultiplexOffset(LPCSTR pszSignal) const
{
   MEMBERASSERT();
   char szName[ABF_ADCNAMELEN+1];
   for (int i=0; i<nADCNumChannels; i++)
   {
      GetADCName(nADCSamplingSeq[i], szName, sizeof(szName));
      if (strcmp(szName, pszSignal)==0)
         return i;
   }
   return ABF_UNUSED_CHANNEL;
}

//==============================================================================================
// FUNCTION:   CheckTriggerChannel
// PURPOSE:    Checks that the value set in the nTriggerSource parameter is valid.
//
BOOL CABFHeader::CheckTriggerChannel() const
{
   MEMBERASSERT();
   if (nOperationMode==ABF_WAVEFORMFILE)
      return TRUE;
   if (!IsThresholdTrigger())
      return TRUE;
   return ( MultiplexOffset( GetTriggerChannel() ) != ABF_UNUSED_CHANNEL );
}

//==============================================================================================
// FUNCTION: GetMxTriggerOffset
// PURPOSE:  Get the multiplexed offset to the data channel that holds the data to be triggered off.      
//
UINT CABFHeader::GetMxTriggerOffset() const
{
   MEMBERASSERT();
   if (!IsThresholdTrigger())
      return nADCNumChannels-1;
   return MultiplexOffset(GetTriggerChannel());
}

//==============================================================================================
// FUNCTION: GetADCTriggerPolarity
// PURPOSE:  Convert the trigger polarity WRT UserUnits to polarity WRT ADC units.      
//
BOOL CABFHeader::GetADCTriggerPolarity() const
{
   MEMBERASSERT();
   if (IsThresholdTrigger())
   {      
      float fADCToUUFactor, fADCToUUShift;
      GetADCtoUUFactors(GetTriggerChannel(), &fADCToUUFactor, &fADCToUUShift);
      if (fADCToUUFactor < 0)
         return !nTriggerPolarity;
   }
   return nTriggerPolarity;
}

//==============================================================================================
// FUNCTION: GetHoldingDuration
// PURPOSE:  Returns the number of waveform points to be set to holding level at the start and
//           end of each episode.
//
UINT CABFHeader::GetHoldingDuration() const
{
   MEMBERASSERT();
   return ABFH_GetHoldingDuration(this);
}

//===============================================================================================
// METHOD:     GetUserListEnable
// TYPE:       Public Function
// PURPOSE:    Returns the enables/disable status of the user list.
// ARGUMENTS:  None
// RETURNS:    The enables/disable status of the user list.
//
bool CABFHeader::GetUserListEnable( UINT uListNum ) const
{
   MEMBERASSERT();
   return nULEnable[uListNum] ? true : false;
}

//==============================================================================================
// FUNCTION: GetVoltstoUUFactors
// PURPOSE:  Calculates conversion factors to go from Volts (at the ADC input) to UserUnits.
//
void CABFHeader::GetVoltstoUUFactors(UINT uChannel, float *pfVoltToUUFactor, float *pfVoltToUUShift) const
{
   MEMBERASSERT();
   float fScaleFactor = fInstrumentScaleFactor[uChannel];

   // FIX FIX FIX PRC DEBUG Telegraph changes - check !
   if( nTelegraphEnable[uChannel] )
      fScaleFactor *= fTelegraphAdditGain[uChannel];

   if (nSignalType != 0)
      fScaleFactor *= fSignalGain[uChannel];

   ASSERT(fScaleFactor != 0.0F);
   if (fScaleFactor==0.0F)
      fScaleFactor = 1.0F;

   float fOffset = fInstrumentOffset[uChannel];
   if (nSignalType != 0)
      fOffset -= fSignalOffset[uChannel];

   *pfVoltToUUFactor = 1.0F / fScaleFactor;
   *pfVoltToUUShift  = fOffset;
   // UU = Volts * fVoltToUUFactor + fVoltToUUShift;
}

//===============================================================================================
// METHOD:     GetWFInfoFirstHoldingInterval
//
// PURPOSE:    Gets the number of samples in the first holding level.
//
int   CABFHeader::GetWFInfoFirstHoldingInterval() const
{
   MEMBERASSERT();

   return GetHoldingDuration() / nADCNumChannels;
}

//==============================================================================================
// FUNCTION: CheckMathChannels
// PURPOSE:  Checks that the channels selected for combination as a math channel are both in the
//           acquisition sequence.
//
BOOL CABFHeader::CheckMathChannels() const
{
   MEMBERASSERT();
   return ( MultiplexOffset( nArithmeticADCNumA ) != ABF_UNUSED_CHANNEL) &&
          ( MultiplexOffset( nArithmeticADCNumB ) != ABF_UNUSED_CHANNEL);
}


//==============================================================================================
// FUNCTION: CheckMathExpression
// PURPOSE:  Checks that the parameters in the math channel expression are sensible.
//
BOOL CABFHeader::CheckMathExpression() const
{
   MEMBERASSERT();
   // Check that the display range is sensible.
   if (fArithmeticUpperLimit == fArithmeticLowerLimit)
      return FALSE;

   // Check for division by zero.
   if (sArithmeticOperator[0]=='/')
   {
      if (fArithmeticK3==0.0F && fArithmeticK4==0.0F)
         return FALSE;
   }
   return TRUE;
}


//==============================================================================================
// FUNCTION: CheckSampleIntervals
// PURPOSE:  Checks that the sample intervals in the header are valid.
//
BOOL CABFHeader::CheckSampleIntervals(float fClockResolution) const
{
   return ABFH_CheckSampleIntervals(this, fClockResolution, NULL);
}


//==============================================================================================
// Table of sample intervals, their assigned oversampled "raw interval" and the expansion factor.
//==============================================================================================
// Interval       <20  25   30   35   40   45   50   55   60   65   70   75   80   85   90   95   100
// Raw Interval    "  12.5 15.0 17.5 20.0 15.0 12.5 11.0 12.0 13.0 14.0 15.0 16.0 17.0 18.0 19.0 20.0
// Divisor         1    2    2    2    2    3    4    5    5    5    5    5    5    5    5    5    5

// Interval       105  110  115  120  125  130  135  140  145  150  155  160  165  170  175  180
// Raw Interval   15.0 11.0 11.5 12.0 12.5 13.0 13.5 14.0 14.5 15.0 15.5 16.0 16.5 17.0 17.5 18.0
// Divisor          7   10   10   10   10   10   10   10   10   10   10   10   10   10   10   10

// Interval       185  190  195  200  210  220  230  240  250  260  270  280  290  300  310  320
// Raw Interval   18.5 19.0 19.5 20.0 15.0 11.0 11.5 12.0 12.5 13.0 13.5 14.0 14.5 15.0 15.5 16.0
// Divisor         10   10   10   10   14   20   20   20   20   20   20   20   20   20   20   20

// Interval       330  240  350  360  370  380  390  400  >400
// Raw Interval   16.5 17.0 17.5 28.0 18.5 19.0 19.5 20.0 20.0
// Divisor         20   20   20   20   20   20   20   20   20 

inline int _Divisor(const double dMinInterval)
{
   // If less than 20us allow full board resolution.
   if (dMinInterval <= 20.0)
      return 1;

   // >20us but <=200us must be multiple of 5us.
   int nMinInterval = int(dMinInterval + 0.5);
   if (nMinInterval <= 40)
      return 2;
   switch (nMinInterval)
   {
      case 45:
         return 3;
      case 50:
         return 4;
      case 105:
         return 7;
      case 210:
         return 14;
   }
   if (nMinInterval <= 100)
      return 5;
   if (nMinInterval <= 200)
      return 10;

   // >200us but <=400us must be multiple of 10us.
   // >400us must be multiple of 20us.
   // Either way use multiples of 20us
   // This assumes that the digitizer resolution is at least 0.5us
   return 20;
}

//==============================================================================================
// FUNCTION:   GetRawSampleInterval
// PURPOSE:    Get the sample interval to use as the underlying sample interval for the acquisition.
// NOTES:      The assumption on which the following code is based is that the underlying sample 
//             interval must be kept in the range 10 us - 20 us (except for sub-10 us acquisitions)
//             in order for the host computer to support concurrent AI/AO and still have enough
//             bus and CPU bandwidth to do other useful work.
//
float CABFHeader::GetRawSampleInterval() const
{
   MEMBERASSERT();
   // The following ASSERT caused problems when you select 3 channels with the demo digitiser.
   // It is trying to assert that the sample interval is a multiple of 0.5uS
   // Problem is, the sample interval is in protocol units.
   // (ie the user selected sample interval, divided by the number of channels)
   // This means for 100uS, 6 channels the interval is 16.66*uS
   //ASSERT(CheckSampleIntervals(0.5F));
   if (nOperationMode!=ABF_WAVEFORMFILE)
      return fADCSampleInterval;
   
   double dMinInterval = fADCSampleInterval;
   if ((fADCSecondSampleInterval != 0.0F) && 
       (fADCSecondSampleInterval < fADCSampleInterval))
      dMinInterval = fADCSecondSampleInterval;

   int nDivisor = _Divisor(dMinInterval);
   return float(dMinInterval / nDivisor);
}


//==============================================================================================
// FUNCTION:   GetRawEpisodeLength
// PURPOSE:    Get the length of an episode in "Raw" samples.
//
UINT CABFHeader::GetRawEpisodeLength() const
{
   MEMBERASSERT();
   float fRawSampleInterval = GetRawSampleInterval();

   UINT uLength = 0;   
   if ((nOperationMode==ABF_WAVEFORMFILE) &&
       (fADCSecondSampleInterval != 0.0F))
   {
      UINT uClockChange      = GetClockChange();
      UINT uFirstDecimation  = UINT(fADCSampleInterval / fRawSampleInterval + 0.5F);
      UINT uSecondDecimation = UINT(fADCSecondSampleInterval / fRawSampleInterval + 0.5F);
      uLength = uClockChange * uFirstDecimation + UINT(lNumSamplesPerEpisode - uClockChange) * uSecondDecimation;
   }
   else
   {
      UINT uDecimation  = UINT(fADCSampleInterval / fRawSampleInterval + 0.5F);
      uLength = UINT(lNumSamplesPerEpisode) * uDecimation;
   }
   return uLength;
}


//==============================================================================================
// FUNCTION: GetFirstHoldingLevel
// PURPOSE:  Gets the holding level at the start of a Waveform episode.
//
float CABFHeader::GetFirstHoldingLevel( UINT uDAC ) const
{
   ASSERT( uDAC < ABF_WAVEFORMCOUNT );

   // No conditioning present or conditioning on different channel
   if ((nConditEnable[uDAC] == 0) || 
       (fPostTrainPeriod[uDAC] == 0.0F))
      return fDACHoldingLevel[uDAC];
   if (fPostTrainPeriod[uDAC] > 0.0F)
      return fPostTrainLevel[uDAC];
   return fBaselineLevel[uDAC];
}

//==============================================================================================
// FUNCTION: GetClockChange
// PURPOSE:  Gets the point at which the sampling interval changes if split clock.
//
UINT CABFHeader::GetClockChange() const
{
   MEMBERASSERT();
   return ABFH_GetClockChange(this);
}


//==============================================================================================
// FUNCTION: GetNumActiveDACChannels
// PURPOSE:  Calculate the number of active DAC channels in the output sequence.
//           This will be less than or equal to the output sequence length.
// NOTES:    The actual number of output may be padded with extra holding channels to make
//           the input sequence length an integer multiple of the output sequence length.
//
UINT CABFHeader::GetNumActiveDACChannels() const
{
   MEMBERASSERT();
   if (nOperationMode != ABF_WAVEFORMFILE)
      return 0;

   UINT uDACChannels = 0;
   for( int i=0; i<ABF_WAVEFORMCOUNT; i++ )
   {
      if( nWaveformEnable[i] &&
          nWaveformSource[i]!=ABF_WAVEFORMDISABLED)
         uDACChannels++;
      else if (nConditEnable[i])
         uDACChannels++;
   }
   return uDACChannels;
}

//==============================================================================================
// FUNCTION: GetDACSamplingSeq
// PURPOSE:  Get the sampling sequence for the output data stream.
// PARAMETERS:
//    bSeparateDigOut - TRUE if synchronous DigitalOUTs have a separate output channel.
//                      FALSE if synchronous DigitalOUTs are folded into the low 4 bits of the DACs
// RETURNS:  Number of items in the list. 
// NOTES:    The return value may not necessarily be the same as the value returned by
//           GetNumActiveDACChannels() because of the digital output channel.
//           The actual number of output may be padded with extra holding channels to make
//           the input sequence length an integer multiple of the output sequence length.
//
UINT CABFHeader::GetDACSamplingSeq(short *pnSamplingSeq, UINT uSeqSize, BOOL bSeparateDigOut) const
{
   MEMBERASSERT();
   if (nOperationMode != ABF_WAVEFORMFILE)
      return 0;

   UINT uDACChannels = 0;
   short anDACChannels[ABF_ADCCOUNT] = { 0 };

   // First add the waveform channels & conditioning channel
   for( int i=0; i<ABF_WAVEFORMCOUNT; i++ )
   {
      if( ((nWaveformEnable[i] && (nWaveformSource[i]!=ABF_WAVEFORMDISABLED)) || (nConditEnable[i])) )
         anDACChannels[uDACChannels++] = (short)i;
   }

   // Lastly add in the Digital Out channel
   if (nDigitalEnable)
   {
      if (bSeparateDigOut)
      {
         anDACChannels[uDACChannels++] = ABF_DIGITAL_OUT_CHANNEL;

         // Pad to the ADC sequence length.
         while (uDACChannels < UINT(nADCNumChannels))
            anDACChannels[uDACChannels++] = ABF_PADDING_OUT_CHANNEL;  // Any channel will do.
      }
      else if (uDACChannels==0)
         anDACChannels[uDACChannels++] = nActiveDACChannel;  // The channel the digital outs are assigned to.
   }

   // If the user wants the sequence, fill in what will fit.
   if (pnSamplingSeq)
   {
      WARRAYASSERT(pnSamplingSeq, uSeqSize);
      ASSERT(uSeqSize >= uDACChannels);
      for (UINT i=0; i<min(uDACChannels, uSeqSize); i++)
         pnSamplingSeq[i] = anDACChannels[i];
   }
   return uDACChannels;
}

//==============================================================================================
// FUNCTION: GetOutputSequenceLength
// PURPOSE:  Calculate the number of output channels that are required in the output sequence.
// PARAMETERS:
//    bSeparateDigOut - TRUE if synchronous DigitalOUTs have a Separate output channel.
//                      FALSE if synchronous DigitalOUTs are folded into the low 4 bits of the DACs
// NOTES:    The actual number of output may be padded with extra holding channels to make
//           the input sequence length an integer multiple of the output sequence length.
//
UINT CABFHeader::GetOutputSequenceLength(BOOL bSeparateDigOut) const
{
   MEMBERASSERT();
   return GetDACSamplingSeq(NULL, 0, bSeparateDigOut);
}

//==============================================================================================
// FUNCTION: GetEpisodeDuration
// PURPOSE:  Gets the duration of the Waveform Episode (in us), allowing for split clock etc.
//
double CABFHeader::GetEpisodeDuration() const
{
   MEMBERASSERT();
   double dEpisodeDuration = 0.0;
   ABFH_GetEpisodeDuration(this, &dEpisodeDuration);
   return dEpisodeDuration;
}


//===============================================================================================
// METHOD:     GetParameterToVary
// TYPE:       Public Function
// PURPOSE:    Returns the parameter to vary with the user list.
// ARGUMENTS:  None
// RETURNS:    The parameter to vary with the user list.
//
int CABFHeader::GetParameterToVary( UINT uListNum ) const
{
   MEMBERASSERT();
   return nULParamToVary[uListNum];
}

//==============================================================================================
// FUNCTION: GetPNDuration
// PURPOSE:  Gets the duration of a P/N sequence (in us), including settling times.
//
double CABFHeader::GetPNDuration( UINT uChannel) const
{
   MEMBERASSERT();
   double dPNDuration = 0.0;
   ABFH_GetPNDurationEx(this, uChannel, &dPNDuration);
   return dPNDuration;
}


//==============================================================================================
// FUNCTION: GetTrainDuration
// PURPOSE:  Gets the duration of a presweep (conditioning) train in us.
//
double CABFHeader::GetTrainDuration( UINT uChannel ) const
{
   MEMBERASSERT();
   double dTrainDuration = 0.0;
   ABFH_GetTrainDurationEx( this, uChannel, &dTrainDuration );
   return dTrainDuration;
}

//==============================================================================================
// FUNCTION: GetMetaEpisodeDuration
// PURPOSE:  Gets the duration of a whole meta-episode (in us).
//
double CABFHeader::GetMetaEpisodeDuration() const
{
   MEMBERASSERT();
   double dMetaEpisodeDuration = 0.0;
   ABFH_GetMetaEpisodeDuration(this, &dMetaEpisodeDuration);
   return dMetaEpisodeDuration;
}

//==============================================================================================
// FUNCTION: GetEpisodeStartToStart
// PURPOSE:  Gets the start to start period for the episode in us.
//
double CABFHeader::GetEpisodeStartToStart() const
{
   MEMBERASSERT();
   double dEpisodeStartToStart = 0.0;
   ABFH_GetEpisodeStartToStart(this, &dEpisodeStartToStart);
   return dEpisodeStartToStart;
}

//==============================================================================================
// FUNCTION: GetRunDuration
// PURPOSE:  Gets the duration on one run in us.
//
double CABFHeader::GetRunDuration() const
{
   MEMBERASSERT();
   return GetEpisodeStartToStart() * (max(lEpisodesPerRun,1)-1) + GetMetaEpisodeDuration();
}

//==============================================================================================
// FUNCTION: GetRunStartToStart
// PURPOSE:  Gets the start to start period for the run in us.
//
double CABFHeader::GetRunStartToStart() const
{
   MEMBERASSERT();
   double dRval = GetRunDuration();
   return max(dRval, double(fRunStartToStart)*1E6F);
}

//==============================================================================================
// FUNCTION: GetTrialSamples
// PURPOSE:  The number of raw samples that make up the trial.
//
UINT CABFHeader::GetRawTrialSamples() const
{
   MEMBERASSERT();
   double dTrialSamples = 0.0;
   if (nOperationMode==ABF_WAVEFORMFILE)
   {
      // If the trial is externally triggered we have no way of calculating the number of
      // samples that it will take to complete it, so we just check for what we can in this case.
      double dTrialDuration = 0.0;
      if ( !IsThresholdTrigger() || 
         (nULEnable[0] && (nULParamToVary[0]==ABF_EPISODESTARTTOSTART)) ||
         (nULEnable[1] && (nULParamToVary[1]==ABF_EPISODESTARTTOSTART)) )
      {
         dTrialDuration = double(lEpisodesPerRun) * GetMetaEpisodeDuration();
      }
      else
      {
         int nRunsPerTrial  = max(lRunsPerTrial, 1);
         dTrialDuration = (nRunsPerTrial-1) * GetRunStartToStart() + GetRunDuration();
      }
      dTrialSamples = dTrialDuration / GetRawSampleInterval();
   }
   else if (nOperationMode==ABF_HIGHSPEEDOSC)
      dTrialSamples = double(lEpisodesPerRun) * lNumSamplesPerEpisode;
   else
      dTrialSamples = double(fSecondsPerRun)*1E6 / fADCSampleInterval;

   // If "continuous" set the number of samples to one less than the absolute max.
   if (dTrialSamples <= 0.0)
      dTrialSamples = INT_MAX-1;

   // Limit the return value to the range of the UINT (32-bit of course).
   return UINT(min(double(UINT_MAX), floor(dTrialSamples+0.5)));
}

//==============================================================================================
// FUNCTION: GetFastInterval
// PURPOSE:  Return the fastest of the two sample intervals.
//
float CABFHeader::GetFastInterval() const
{
   MEMBERASSERT();
   if (nOperationMode!=ABF_WAVEFORMFILE)
      return fADCSampleInterval;

   float fSecondInterval = fADCSecondSampleInterval;
   if (fSecondInterval == 0.0F)
      fSecondInterval = fADCSampleInterval;
   return min(fADCSampleInterval, fSecondInterval);
}

//==============================================================================================
// FUNCTION: SetFastInterval
// PURPOSE:  Set the fast interval to a new value, retaining the ratio with the slow interval.
//
void CABFHeader::SetFastInterval(float fFastInterval)
{
   MEMBERASSERT();
   if ((nOperationMode!=ABF_WAVEFORMFILE) ||
       (fADCSecondSampleInterval==0.0F))
   {
      fADCSampleInterval = fFastInterval;
      return;
   }

   if (fADCSampleInterval < fADCSecondSampleInterval)
   {
      fADCSecondSampleInterval *= fFastInterval / fADCSampleInterval;
      fADCSampleInterval        = fFastInterval;
   }
   else
   {
      fADCSampleInterval      *= fFastInterval / fADCSecondSampleInterval;
      fADCSecondSampleInterval = fFastInterval;
   }
}


//===============================================================================================
// FUNCTION: SetAutoAnalysis
// PURPOSE:  Sets the auto analysis parameters in the ABF header.
//
void CABFHeader::SetAutoAnalysis( int nEnable, LPCSTR pszMacroName )
{
   MEMBERASSERT();
   
   ASSERT( nEnable >= ABF_AUTOANALYSE_DISABLED );
   ASSERT( nEnable <= ABF_AUTOANALYSE_RUNMACRO );

   nAutoAnalyseEnable = (short)nEnable;

   if( nEnable == ABF_AUTOANALYSE_RUNMACRO)
   {
      LPSZASSERT( pszMacroName );
      ABF_SET_STRING( sAutoAnalysisMacroName, pszMacroName);
   }
}

//===============================================================================================
// FUNCTION: GetAutoAnalysis
// PURPOSE:  Gets the auto analysis parameters from the ABF header.
//
int CABFHeader::GetAutoAnalysis( char *pszMacroName, UINT uSize ) const
{
   MEMBERASSERT();
   
   if( nAutoAnalyseEnable == ABF_AUTOANALYSE_RUNMACRO )
   {
      ARRAYASSERT(pszMacroName, uSize);
      ABF_GET_STRING( pszMacroName, sAutoAnalysisMacroName, uSize );
   }
      
   return nAutoAnalyseEnable;
}

//===============================================================================================
// FUNCTION: operator==
// PURPOSE:  Compares two CABFHeaders, returns TRUE if they are the same.
//
BOOL CABFHeader::operator==(CABFHeader &FH)
{
   MEMBERASSERT();
   
   BOOL bSame = ( memcmp( &FH, this, sizeof( CABFHeader ) ) == 0 );

#if 0 // _DEBUG
   // For debug builds, if the headers are different log the point where they differ.
   if( !bSame )
   {
      char *pThis = (char *)this;
      char *pThat = (char *)&FH;
      BOOL bOverride = TRUE;
      BOOL bInclude  = TRUE;

      // Elements we know may differ.
      UINT uExcludeList[] = { 20, 21, 22, 23, 24, 25, 28, 190, 366, 367 };

      for( UINT i=0; i<sizeof(ABFFileHeader); i++ )
      {
         if( pThis[i] != pThat[i] )
         {
            // See if it is in the Exclude List
            for( UINT j=0; j<ELEMENTS_IN(uExcludeList); j++ )
            {
               if( i == uExcludeList[j] )
               {
                  bInclude = FALSE;
                  break;
               }
            }

            if( bInclude && ( i < sizeof(ABFFileHeader) ) )
            {
               bOverride = FALSE;
               TRACE1( "Headers differ at byte %d.\n", i );
            }
         }
      }

      // If the differences were all in the Exclude List, then treat the 
      // headers as being the same.
      if( bOverride )
         bSame = TRUE;
   }
#endif   // _DEBUG

   return bSame;
}

//===============================================================================================
// FUNCTION: GetSearchRegion
// PURPOSE:  Returns the search region start and end boundaries.
//           Search region is calculated from the first active statistics signal.
void CABFHeader::GetSearchRegion( UINT uStatsRegion, UINT &uStartRegion, UINT &uEndRegion, UINT uEpisode ) const
{
   MEMBERASSERT();
   UINT uSearchChannel = 0;
   int nEpoch          = 0;

   if( nStatsSearchMode[ uStatsRegion ] >= ABF_EPOCHCOUNT )
      uSearchChannel  = 1;

   ASSERT( nStatsSearchMode[ uStatsRegion ] < ABF_EPOCHCOUNT*2 );
   switch( nStatsSearchMode[ uStatsRegion ] )
   {
      case ABF_PEAK_SEARCH_SPECIFIED:
         uStartRegion = UINT(lStatsStart[ uStatsRegion ]);
         uEndRegion   = UINT(lStatsEnd[ uStatsRegion ]);

         // Correct m_uSearchEnd to full range if zero.
         if (uEndRegion == 0)
         {
            uEndRegion = lNumSamplesPerEpisode - 1;
         }
         break;

      case ABF_PEAK_SEARCH_ALL:
         uStartRegion = 0;
         uEndRegion   = lNumSamplesPerEpisode - 1;
         break;

      default:
         nEpoch = nStatsSearchMode[ uStatsRegion ] - uSearchChannel * ABF_EPOCHCOUNT;
         GetEpochLimits( GetFirstActiveStatsChannel(), uSearchChannel, uEpisode, 
                         nEpoch, &uStartRegion, &uEndRegion);
         uStartRegion *= nADCNumChannels;
         uEndRegion   *= nADCNumChannels;
         break;
   }
}
//===============================================================================================
// FUNCTION: GetBaselineRegion
// PURPOSE:  Returns the baseline region start and end boundaries.
//           Search region is calculated from the first active statistics signal.
void CABFHeader::GetBaselineRegion( UINT &uStartRegion, UINT &uEndRegion, UINT uEpisode ) const
{
   MEMBERASSERT();
   ASSERT( uEpisode >= 0 );
   ASSERT( uStartRegion >= 0 );
   ASSERT( uEndRegion >= 0 );

   int uSearchChannel = 0;
   int nEpoch         = 0;

   if( nStatsBaseline >= ABF_EPOCHCOUNT )
      uSearchChannel = 1;

   switch( nStatsBaseline )
   {
      case ABF_PEAK_BASELINE_NONE:
         uStartRegion = 0;
         uEndRegion   = lNumSamplesPerEpisode - 1;
         break;

      case ABF_PEAK_BASELINE_SPECIFIED:
         uStartRegion = UINT(lStatsBaselineStart);
         uEndRegion   = UINT(lStatsBaselineEnd);

         // Correct m_uSearchEnd to full range if zero.
         if (uEndRegion == 0)
            uEndRegion = lNumSamplesPerEpisode - 1;
         break;

      case ABF_PEAK_BASELINE_FIRSTHOLDING:
         ASSERT(nOperationMode==ABF_WAVEFORMFILE);
         // Average the holding points at the start of the episode
         uStartRegion = MultiplexOffset( GetFirstActiveStatsChannel() );
         uEndRegion   = GetHoldingDuration() - 1;
         break;

      case ABF_PEAK_BASELINE_LASTHOLDING:
         ASSERT(nOperationMode==ABF_WAVEFORMFILE);
         // Average the holding points at the end of the episode
         GetEpochLimits( GetFirstActiveStatsChannel(), uSearchChannel, uEpisode,
                         ABFH_LASTHOLDING, &uStartRegion, &uEndRegion );
         uStartRegion *= nADCNumChannels;
         uEndRegion   *= nADCNumChannels;
         break;

      default:
         ASSERT(nOperationMode==ABF_WAVEFORMFILE);
         nEpoch = nStatsBaseline - uSearchChannel * ABF_EPOCHCOUNT; // Convert epoch to a 0 to 9 epoch number if uSearchChannel is > 0
         GetEpochLimits( GetFirstActiveStatsChannel(), uSearchChannel, uEpisode, 
                         nEpoch, &uStartRegion, &uEndRegion);
         uStartRegion *= nADCNumChannels;
         uEndRegion   *= nADCNumChannels;

         break;
   }
}

//===============================================================================================
// METHOD:     GetFirstActiveStatsChannel
// TYPE:       Public Function
// PURPOSE:    Returns the first active statistics channel.
// ARGUMENTS:  The Stats Region ID.
// RETURNS:    The first active ADC channel.
//
UINT CABFHeader::GetFirstActiveStatsChannel() const
{
   MEMBERASSERT();

   for( UINT uChannel = 0; uChannel < ABF_ADCCOUNT; uChannel ++ )
   {
      UINT uBitMask = 0x01 << uChannel;
      if ( nStatsActiveChannels & uBitMask )
         return uChannel;
   }
   
   // If process reaches this point no channel is selected.
   ASSERT( uChannel != 0); 
   return 0;
}

//==============================================================================================
// FUNCTION: GetStart_FILETIME
// PURPOSE:  Get the start time in FILETIME format
//
BOOL CABFHeader::GetStart_FILETIME(FILETIME * pStart) const
{
   MEMBERASSERT();
   WPTRASSERT(pStart);
   
   SYSTEMTIME start;
   if (!GetStart_SYSTEMTIME(&start))
      return FALSE;

   FILETIME zero = {0};
   *pStart = zero;

   return SystemTimeToFileTime(&start, pStart);
}

//==============================================================================================
// FUNCTION: GetStart_SYSTEMTIME
// PURPOSE:  Get the start time in SYSTEMTIME format
//
BOOL CABFHeader::GetStart_SYSTEMTIME(SYSTEMTIME * pStart) const
{
   MEMBERASSERT();
   WPTRASSERT(pStart);
   
   SYSTEMTIME zero = {0};
   *pStart = zero;
   
   pStart->wYear           = (WORD) (lFileStartDate / 10000L);
   pStart->wMonth          = (WORD)((lFileStartDate % 10000L) / 100L);
   pStart->wDay            = (WORD) (lFileStartDate % 100L);
   pStart->wHour           = (WORD) (lFileStartTime / 3600L);
   pStart->wMinute         = (WORD)((lFileStartTime % 3600L)  / 60L);
   pStart->wSecond         = (WORD) (lFileStartTime % 60L);
   pStart->wMilliseconds   = (WORD) (nFileStartMillisecs);

   return TRUE;
}

//==============================================================================================
// FUNCTION: GetStart_ULARGE_INTEGER
// PURPOSE:  Get the start time in FILETIME format converted to a ULARGE_INTEGER
//
BOOL CABFHeader::GetStart_ULARGE_INTEGER(ULARGE_INTEGER * pStart) const
{
   MEMBERASSERT();
   WPTRASSERT(pStart);
   
   FILETIME start;
   if (!GetStart_FILETIME(&start))
      return FALSE;

   ULARGE_INTEGER zero = {0};
   *pStart = zero;

   pStart->LowPart   = start.dwLowDateTime;
   pStart->HighPart  = start.dwHighDateTime;

   return TRUE;
}

//==============================================================================================
// FUNCTION: GetStatsADCNames
// PURPOSE:  Return a string of the selected statistics signals.
//
void CABFHeader::GetStatsADCNames( char *ps, UINT uSize) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);

   const UINT c_uSignalName = 64; // Size of signal heading.
   char szTempString[ c_uSignalName ] = {0};

   // Display selected channels in the selected static display.
   for ( UINT uSignal = 0; uSignal < ABF_ADCCOUNT; uSignal++)
   {
      UINT uBitMask = 0x01 << uSignal;

      // If the current signal is active add to string.
      if ( nStatsActiveChannels & uBitMask )
      {
         char szSignal[ ABF_ADCNAMELEN ];

         // Set the input signal name.
         ABF_GET_STRING( szSignal, sADCChannelName[ uSignal ], sizeof( szSignal ));
         UINT uSizeLeftInBuffer = uSize - strlen( szTempString );
         UINT uDelim = 3; // Characters needed for the delimiter.
         if( uSizeLeftInBuffer <= strlen( szSignal ) + uDelim  )
         {
            strncat( szTempString, "...", uSizeLeftInBuffer );
            break;
         }

         strncat(szSignal,"  ", 2 );

         // Add to selected string.
         strncat(szTempString, szSignal, sizeof(szSignal) );
      }
   }

   strncpy( ps, szTempString, uSize );
}

//==============================================================================================
// FUNCTION: GetStatRegionState()
// PURPOSE:  Returns the state of the specified region
//
BOOL CABFHeader::GetStatRegionState( UINT uRegion ) const
{
   MEMBERASSERT();

   BOOL bStatRegionState = FALSE;

   // Shift mask to region bit
   int nBitMask = ABF_PEAK_SEARCH_REGION0 << uRegion;
   
   // Region is active if non zero
   if( (nStatsSearchRegionFlags & nBitMask) != 0 )
      bStatRegionState = TRUE;

   return bStatRegionState;
}

//==============================================================================================
// FUNCTION: GetNumberOfStatsRegions()
// PURPOSE:  Returns the number of stats regions enabled.
//
int CABFHeader::GetNumberOfStatsRegions( void ) const
{
   MEMBERASSERT();

   int nRegionCount = 0;

   for( int nRegion = 0; nRegion < ABF_STATS_REGIONS; nRegion++ )
   {
      int nBitMask = ABF_PEAK_SEARCH_REGION0 << nRegion;
      if( nStatsSearchRegionFlags & nBitMask )
         nRegionCount++;
   }
   return nRegionCount;
}

//===============================================================================================
// METHOD:     GetChannelPeakPolarity
// PURPOSE:    Gets the polarity of the specified channel for peak detection
//
int   CABFHeader::GetChannelPeakPolarity( UINT uChannel ) const
{
   MEMBERASSERT();
   ASSERT( uChannel < ABF_ADCCOUNT );

   return nStatsChannelPolarity[ uChannel ];
}

//===============================================================================================
// METHOD:     SetChannelPeakPolarity
// PURPOSE:    Sets the polarity of the specified channel for peak detection
//
void CABFHeader::SetChannelPeakPolarity( UINT uChannel, int nPolarity )
{
   MEMBERASSERT();
   ASSERT( uChannel < ABF_ADCCOUNT );

   nStatsChannelPolarity[ uChannel ] = (short)nPolarity;
}


//===============================================================================================
// FUNCTION: GetTrialDuration
// PURPOSE:  Returns to total duration of the trial in us.
//
double CABFHeader::GetTrialDuration() const
{
   MEMBERASSERT();   
   
   return GetRunStartToStart() * (max(lRunsPerTrial,1)-1) + GetRunDuration();
}

bool CABFHeader::is_channel_recorded(int channel) const
{
   MEMBERASSERT();
   ASSERT(0 <= channel && channel < ABF_ADCCOUNT);

   for (int i = 0; i < get_channel_count_recorded(); ++i) 
   {
      if (nADCSamplingSeq[i] == channel)
         return true;
   }
   return false;
}

bool CABFHeader::is_channel_acquired(int channel) const
{
   MEMBERASSERT();
   ASSERT(0 <= channel && channel < ABF_ADCCOUNT);

   for (int i = 0; i < get_channel_count_acquired(); ++i) 
   {
      if (nADCSamplingSeq[i] == channel)
         return true;
   }
   return false;
}

//===============================================================================================
// FUNCTION:IsEpochResistanceEnabled
// PURPOSE: Search for epochs set to resistance within the specified waveform
//
BOOL CABFHeader::IsEpochResistanceEnabled( UINT uWaveformChannel ) const
{
   MEMBERASSERT();

   for( UINT uEpochIndex = 0; uEpochIndex < ABF_EPOCHCOUNT; uEpochIndex++)
      if ( nEpochType[ uWaveformChannel ][ uEpochIndex ] == ABF_EPOCH_TYPE_RESISTANCE )
         return TRUE;

   return FALSE;
}

//===============================================================================================
// FUNCTION:IsEpochResistanceEnabled
// PURPOSE: Search for epochs set to resistance across all waveforms
//
BOOL CABFHeader::IsEpochResistanceEnabled() const
{
   MEMBERASSERT();

   for( UINT uWaveformChannel = 0; uWaveformChannel < ABF_WAVEFORMCOUNT; uWaveformChannel++)
      if( IsEpochResistanceEnabled( uWaveformChannel ) == TRUE )
         return TRUE;

   return FALSE;
}

//===============================================================================================
// METHOD:     SetEpochResistanceSignalName
// PURPOSE:    Sets the signal of the epoch resistance signal specified
//
void CABFHeader::SetEpochResistanceSignalName( const UINT uWaveformChannel, LPCSTR ps )
{
   MEMBERASSERT();
   LPSZASSERT(ps);
   ABF_SET_STRING(sEpochResistanceSignalName[ uWaveformChannel ], ps);
}

//===============================================================================================
// METHOD:     GetEpochResistanceSignalName
// PURPOSE:    Gets the name of the epoch resistance signal specified
//
void CABFHeader::GetEpochResistanceSignalName( const UINT uWaveformChannel, char *ps, UINT uSize ) const
{
   MEMBERASSERT();
   ARRAYASSERT(ps, uSize);
   ABF_GET_STRING(ps, sEpochResistanceSignalName[ uWaveformChannel ], uSize);
   if (ps[0]=='\0')
      AXU_sprintf(ps, uSize, "IN #0");
}

//===============================================================================================
// FUNCTION: IsEpochResistanceSignal
// PURPOSE:  Checks whether the input name matches signal designated for epoch resistance
//           
BOOL CABFHeader::IsEpochResistanceSignal( UINT uDAC, UINT uChannel, BOOL & bValidSignal )
{
   MEMBERASSERT();
   ASSERT( uDAC <= ABF_WAVEFORMCOUNT);
   ASSERT( uChannel < ABF_ADCCOUNT );
   
   bValidSignal = FALSE;
   char szInName[ABF_ADCNAMELEN+1], szERSignalName[ABF_DACNAMELEN+1];

   // Get epoch resistance input signal name for specified DAC channel
   GetEpochResistanceSignalName( uDAC, szERSignalName, sizeof(szERSignalName) );

   // Get input signal name on specified ADC channel
   GetADCName(uChannel, szInName, sizeof(szInName) );

   // Check signals are the same
   if( strcmp( szInName, szERSignalName ) == 0 )
      bValidSignal = TRUE;

   return TRUE;
}

//===============================================================================================
// METHOD:     IsStatsChannelEnabled
// PURPOSE:    Returns a value to indicate that a channel is enabled for statistics.
//
BOOL CABFHeader::IsStatsChannelEnabled( UINT uChannel ) const
// CAUTION: This does not return 0 or 1, it returns 0 or the bitmask!!!
//
{
   MEMBERASSERT();
   ASSERT( uChannel < ABF_ADCCOUNT );

   UINT uBitMask = 0x01 << uChannel;
   return nStatsActiveChannels & uBitMask;
}

//==============================================================================================
// FUNCTION: SetDD132xScopeTrigger
// PURPOSE:  Sets the dd132x trigger out flag.
//
void CABFHeader::SetDD132xTriggerOut( BOOL bEnable )
{
   MEMBERASSERT();

   nDD132xTriggerOut = (short)bEnable;
}  

//==============================================================================================
// FUNCTION: GetDD132xScopeTrigger
// PURPOSE:  Returns the dd132x trigger out flag
//
BOOL CABFHeader::GetDD132xTriggerOut() const
{
   MEMBERASSERT();

   return (BOOL)nDD132xTriggerOut;
}

//==============================================================================================
// FUNCTION: GetOperationMode
// PURPOSE:  Returns the operation mode
//
int CABFHeader::GetOperationMode() const
{
   MEMBERASSERT();
   
   return (int)nOperationMode;
}

//===============================================================================================
// FUNCTION: GetGUID
// PURPOSE:  Returns the file GUID as a string.
//
void CABFHeader::GetGUID( char *pszGuid, UINT uBufLen )
{
   MEMBERASSERT();
   WARRAYASSERT( pszGuid, uBufLen );

   AXU_GUIDToString( FileGUID, pszGuid, uBufLen);
}

//===============================================================================================
// FUNCTION: SetGUID
// PURPOSE:  Sets the file GUID from a guid string.
//
bool CABFHeader::SetGUID( const char *pszGuid )
{
   MEMBERASSERT();
   LPSZASSERT( pszGuid );

   GUID guid;
   if( AXU_StringToGUID( pszGuid, guid) )
   {
      FileGUID = guid;
      return true;
   }
   return false;
}

//===============================================================================================
// FUNCTION: SetGUID
// PURPOSE:  Sets the file GUID from a guid.
//
void CABFHeader::SetGUID( const GUID guid )
{
   MEMBERASSERT();
   FileGUID = guid;
}

int CABFHeader::GetEpochDuration(UINT uDACChannel, UINT uEpisode, int nEpoch)
{
   return ABFH_GetEpochDuration(this, uDACChannel, uEpisode, nEpoch);
}

float CABFHeader::GetEpochLevel(UINT uDACChannel, UINT uEpisode, int nEpoch)
{
   return ABFH_GetEpochLevel(this, uDACChannel, uEpisode, nEpoch);
}

bool CABFHeader::GetEpochLevelRange(UINT uDACChannel, int nEpoch, float *pfMin, float *pfMax)
{
   return ABFH_GetEpochLevelRange(this, uDACChannel, nEpoch, pfMin, pfMax) == TRUE;
}

bool CABFHeader::GetWaveformLevelRange(UINT uDACChannel, float *pfMin, float *pfMax )
{
   bool bValid = false;
   float fMin = 1e9;
   float fMax = -1e9;
   for ( int nEpoch = 0; nEpoch < ABF_EPOCHCOUNT; nEpoch++ )
   {
      if ( nEpochType[uDACChannel][nEpoch] != ABF_EPOCHDISABLED )
      {
         float fEpochMinLevel;
         float fEpochMaxLevel;
         // Takes into account number of sweeps, delta levels and user lists.
         if ( GetEpochLevelRange( uDACChannel, nEpoch, &fEpochMinLevel, &fEpochMaxLevel ) )
         {
            bValid = true;
            fMin = min( fMin, fEpochMinLevel );
            fMax = max( fMax, fEpochMaxLevel );
         }
      }
   } // for each epoch

   if ( bValid )
   {
      *pfMin = fMin;
      *pfMax = fMax;
   }
   return bValid;
}

bool CABFHeader::GetPNLevelRange(UINT uDACChannel, float *pfMin, float *pfMax )
{
   *pfMin = 0.0;
   *pfMax = 0.0;

   if ( ! nPNEnable )
      return false;

   float fMin;
   float fMax;
   if ( ! GetWaveformLevelRange( uDACChannel, &fMin, &fMax ) )
      return false;

   // Include holding level as part of range.
   fMin = min( fMin, fDACHoldingLevel[uDACChannel] );
   fMax = max( fMax, fDACHoldingLevel[uDACChannel] );

   // Make relative to holding level.
   fMin -= fDACHoldingLevel[uDACChannel];
   fMax -= fDACHoldingLevel[uDACChannel];

   UINT uMaxPNSubsweeps = ABFH_GetMaxPNSubsweeps(this, uDACChannel);

   // Scale by 1/N.
   fMin /= (float)uMaxPNSubsweeps;
   fMax /= (float)uMaxPNSubsweeps;

   // Apply to P/N holding level.
   if ( nPNPolarity[uDACChannel] == ABF_PN_SAME_POLARITY )
   {
      fMin = fPNHoldingLevel[uDACChannel] + fMin;
      fMax = fPNHoldingLevel[uDACChannel] + fMax;
   }
   else
   {
      fMin = fPNHoldingLevel[uDACChannel] - fMin;
      fMax = fPNHoldingLevel[uDACChannel] - fMax;
   }

   // Deal with inversion due to polarity.
   *pfMin = min(fMin,fMax);   
   *pfMax = max(fMin,fMax);   

   return true;
}


