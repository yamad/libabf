//***********************************************************************************************
//
//    Copyright (c) 1993-2000 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// MODULE:  ABFINFO.CPP   
// PURPOSE: Provides routines for returning a text buffer describing the ABF header.
//
// An ANSI C compiler should be used for compilation.
// Compile with the large memory model option.
// (e.g. CL -c -AL ABFINFO.C)
//
#include "wincpp.hpp"

#include "abfinfo.h"
#include "abfutil.h"
#include "TextBuffer.hpp"
#include "ABFFiles.h"
#include "cabfhdr.cpp"
#include "\AxonDev\Comp\Common\ArrayPtr.hpp"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"

#if defined(_WINDOWS)
   #define MICRO 0xB5   // ANSI micro
#elif defined(_DOS)
   #define MICRO 0xE6   // IBM micro
#else
   #define MICRO 0x75   // u
#endif

char *pszOperationMode[] =
{
   "variable-length event-detection",
   "fixed-length event-detection",
   "gap-free",
   "high-speed oscilloscope",
   "episodic stimulation",
};

char *pszFileType[] =
{
   "ABF",
   "FETCHEX",
   "CLAMPEX"
};

static char *s_szMonths[] =
{
   "Jan",
   "Feb",
   "Mar",
   "Apr",
   "May",
   "Jun",
   "Jul",
   "Aug",
   "Sep",
   "Oct",
   "Nov",
   "Dec"
};

static const char *AutosampleInstrument(int nAutosampleInstrument)
{
   switch (nAutosampleInstrument)
   {
      case ABF_INST_AXOPATCH1:
         return "Axopatch-1/CV4-1/100";
      case ABF_INST_AXOPATCH1_1:
         return "Axopatch-1/CV4-0.1/100";
      case ABF_INST_AXOPATCH1B:
         return "Axopatch-1B/CV4-1/100 (inv)";
      case ABF_INST_AXOPATCH1B_1:
         return "Axopatch-1B/CV4-0.1/100 (inv)";
      case ABF_INST_AXOPATCH201:
         return "Axopatch 200/CV201";
      case ABF_INST_AXOPATCH202:
         return "Axopatch 200/CV202";
      case ABF_INST_GENECLAMP:
         return "Geneclamp 500";
      case ABF_INST_DAGAN3900:
         return "DAGAN 3900";
      case ABF_INST_DAGAN3900A:
         return "DAGAN 3900A";
      case ABF_INST_DAGANCA1_1:
         return "DAGAN CA1 Im=0.1";
      case ABF_INST_DAGANCA1:
         return "DAGAN CA1 Im=1.0";
      case ABF_INST_DAGANCA10:
         return "DAGAN CA1 Im=10";
      case ABF_INST_WARNER_OC725:
         return "Warner OC-725";
      case ABF_INST_WARNER_OC725C:
         return "Warner OC-725C";
      case ABF_INST_AXOPATCH200B:
         return "Axopatch 200B";
      case ABF_INST_DAGANPCONE0_1:
         return "Dagan PC-ONE  Im=0.1";
      case ABF_INST_DAGANPCONE1:
         return "Dagan PC-ONE  Im=1.0";
      case ABF_INST_DAGANPCONE10:
         return "Dagan PC-ONE  Im=10";
      case ABF_INST_DAGANPCONE100:
         return "Dagan PC-ONE  Im=100";
      case ABF_INST_WARNER_BC525C:
         return "Warner BC-525C";
      case ABF_INST_WARNER_PC505:
         return "Warner PC-505";
      case ABF_INST_WARNER_PC501:
         return "Warner PC-501";
      case ABF_INST_MULTICLAMP700:
         return "MultiClamp 700";
      case ABF_INST_OPUSXPRESS6000:
         return "OpusXpress 6000A";

      default:
         return "Unknown instrument";
   }
}

static const char *GetParameterList(int nParamToVary)
{
   switch (nParamToVary)
   {
      case ABF_CONDITNUMPULSES:
         return "number of pulses in train";
      case ABF_CONDITBASELINEDURATION:
         return "baseline duration";
      case ABF_CONDITBASELINELEVEL:
         return "baseline level";
      case ABF_CONDITSTEPDURATION:
         return "step duration";
      case ABF_CONDITSTEPLEVEL:
         return "step level";
      case ABF_CONDITPOSTTRAINDURATION:
         return "post-train duration";
      case ABF_CONDITPOSTTRAINLEVEL:
         return "post-train level";
      case ABF_EPISODESTARTTOSTART:
         return "sweep start-to-start time";
      case ABF_INACTIVEHOLDING:
         return "inactive holding level";
      case ABF_DIGITALHOLDING:
         return "digital intersweep holding";
      case ABF_PNNUMPULSES:
         return "number of Leak sub-sweeps";
      default:
      {
         static char *UserValue[] = 
         {
            "parallel value Z",
            "initial epoch level Z",
            "initial epoch duration Z",
         };
         int nParamClass = (nParamToVary-ABF_PARALLELVALUE)/ABF_EPOCHCOUNT;
         int nEpoch      = (nParamToVary-ABF_PARALLELVALUE)%ABF_EPOCHCOUNT;

         if (nParamClass >= ELEMENTS_IN(UserValue))
         {
            ERRORMSG1("Unexpected user list parameter %d.", nParamToVary);
            return "(unknown)";
         }

         char *rval = UserValue[nParamClass];
         rval[strlen(rval)-1] = char('A'+nEpoch);
         return rval;
      }
   }
};

//===============================================================================================
// FUNCTION: StripSpace
// PURPOSE:  Strips leading and trailing space (ASCII 32) characters out of a string.
//
static char *StripSpace(char *psStr)
{
   if (psStr==NULL)
      return NULL;
      
   char *ps = psStr;
   while (*ps==' ')
      ps++;

   strcpy(psStr, ps);

   if (*psStr != '\0')
   {
      ps = psStr + strlen(psStr);
      while (*(ps-1)==' ')
         ps--;
      *ps = '\0';
   }
   return psStr;
}

//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  Returns a pointer to the filename component of the passed full path.
//
static BOOL GetFileName(LPCSTR pszFilePath, LPSTR szName, UINT uNameLen)
{
   char szPath[_MAX_PATH] = "";
   if (pszFilePath)
   {
      strncpy(szPath, pszFilePath, sizeof(szPath)-1);
      szPath[sizeof(szPath)-1] = '\0';
   }

   StripSpace(szPath);

   // If not descriptive text, return "n/a"
   if ((szPath == NULL) || (szPath[0] == '\0'))
      return FALSE;

   // If "descriptive text" use the descriptive text.
   if (szPath[0]==INFO_TEXTMARKER)
   {
      strncpy(szName, szPath+1, uNameLen-1);
      szName[uNameLen-1] = '\0';
      return TRUE;
   }

   char szFName[_MAX_FNAME];
   char szExt[_MAX_EXT];

   _splitpath(szPath, NULL, NULL, szFName, szExt);
   _makepath(szFName, "", "", szFName, szExt);
   strncpy(szName, szFName, uNameLen-1);
   szName[uNameLen-1] = '\0';

   return TRUE;
}

//===============================================================================================
// FUNCTION: FormatTime
// PURPOSE:  Takes a time value in seconds and formats it into Hours:Minutes:Seconds.
//
#define MAXTIMELEN 16
UINT WINAPI INFO_FormatTime(long lTime, short nMillisecs, char *buf, UINT uBufLen)
{
   ARRAYASSERT(buf, uBufLen);
   ASSERT(uBufLen >= MAXTIMELEN);

   int nHour   = lTime / 3600;
   int nMinute = (lTime % 3600) / 60;
   int nSecond = lTime % 60;
   int nThousandths = nMillisecs;

   char szThousandths[8] = "";
   if (nThousandths)
      sprintf(szThousandths, ".%d", nThousandths);

   int l = _snprintf(buf, uBufLen, "%02d:%02d:%02d%s", nHour, nMinute, nSecond, szThousandths);
   ASSERT(l > 0);

   // Prevent compiler warnings.
   l = l;

   return strlen(buf);
}

//===============================================================================================
// FUNCTION: INFO_FormatDate
// PURPOSE:  Formats an ABF date as a US centric date string.
//
#define MAXDATELEN 16
UINT WINAPI INFO_FormatDate(long lDate, char *buf, UINT uBufLen)
{
   ARRAYASSERT(buf, uBufLen);
   ASSERT(uBufLen >= MAXDATELEN);

   lDate = ABFU_FixFileStartDate ( lDate );

   long lStartDay   = lDate % 100L;
   long lStartMonth = (lDate % 10000L) / 100L;
   long lStartYear  = lDate / 10000L;

   if( (lStartDay > 31) ||
       (lStartMonth > 12) ||
       (lStartYear > 2100) )
   {
      // The date is invalid.
      strcpy( buf, "n/a");
   }
   else
   {
      int l = _snprintf(buf, uBufLen, "%s %ld, %ld", 
               s_szMonths[(int)lStartMonth-1], lStartDay, lStartYear);
      ASSERT(l > 0);

      // Prevent compiler warnings.
      l = l;
   }
   return strlen(buf);
}      

//===============================================================================================
// FUNCTION: GetCreationDate
// PURPOSE:  Builds a string that contains the date and time and stopwatch time.
//
static void GetCreationDate(long lTime, short nMillisecs, long lDate, long lStopwatch, char *buf)
{
   if ((lTime <= 0) || (lDate <= 0))
   {
      strcpy(buf, "n/a");
      return;
   }
   char szDate[MAXDATELEN];
   INFO_FormatDate(lDate, szDate, sizeof(szDate));

   char szTimeBuf[MAXTIMELEN];
   INFO_FormatTime(lTime, nMillisecs, szTimeBuf, sizeof(szTimeBuf));

   char szStopwatchBuf[MAXTIMELEN];
   INFO_FormatTime(lStopwatch, 0, szStopwatchBuf, sizeof(szStopwatchBuf));

   sprintf( buf, "%s, at %s [%s]", szDate, szTimeBuf, szStopwatchBuf);
}     

//===============================================================================================
// FUNCTION: GetAppVersion
// PURPOSE:  Builds a string that contains the major, minor, bugfix and build version numbers
//
#define MAXAPPVERLEN 32
static void GetAppVersion(short nMajor, short nMinor, short nBugfix, short nBuild, char * pszAppVer )
{
   sprintf( pszAppVer, "%d.%d.%d.%d", nMajor, nMinor, nBugfix, nBuild);
}      
 
//===============================================================================================
// FUNCTION: FormatDigitalOuts
// PURPOSE:  Builds a string from a digital out bit battern.
//
char *FormatDigitalOuts(int nDigitalOuts, int nDigitalTrain, char *szBuffer)
{   
   int i;
   for (i=0; i<4; i++)
   {
      if( (nDigitalTrain & (1 << (3-i))) )
         szBuffer[i] = '*';
      else
         szBuffer[i] = (nDigitalOuts & (1 << (3-i))) ? '1' : '0';
   }

   szBuffer[4] = '\0';
   return szBuffer;
}

//===============================================================================================
// FUNCTION: GetADCChannelUnits
// PURPOSE:  Fills the passed buffer with the channel units for a particular ADC channel.
//           Builds a zero terminated "C" string from a fixed length array of bytes.
//
static BOOL GetADCChannelUnits(const ABFFileHeader *pFH, int nADCChannel, char *szBuffer)
{
   strncpy(szBuffer, pFH->sADCUnits[nADCChannel], ABF_ADCUNITLEN);
   szBuffer[ABF_ADCUNITLEN] = '\0';
   StripSpace(szBuffer);
   return (szBuffer[0] != '\0');
}

//===============================================================================================
// FUNCTION: GetADCChannelName
// PURPOSE:  Fills the passed buffer with the channel name for a particular ADC channel.
//           Builds a zero terminated "C" string from a fixed length array of bytes.
//
static BOOL GetADCChannelName(const ABFFileHeader *pFH, int nADCChannel, char *szBuffer)
{
   int nLogicalChannel = pFH->nADCPtoLChannelMap[nADCChannel];
   strncpy(szBuffer, pFH->sADCChannelName[nADCChannel], ABF_ADCNAMELEN);
   szBuffer[ABF_ADCNAMELEN] = '\0';
   StripSpace(szBuffer);
   if (strlen(szBuffer)==0)
      sprintf(szBuffer, "IN%d", nLogicalChannel);
   sprintf(szBuffer+strlen(szBuffer), " (#%d)", nLogicalChannel);
   return (szBuffer[0] != '\0');
}

//===============================================================================================
// FUNCTION: GetDACChannelUnits
// PURPOSE:  Fills the passed buffer with the channel units for a particular DAC channel.
//           Builds a zero terminated "C" string from a fixed length array of bytes.
//
BOOL GetDACChannelUnits(const ABFFileHeader *pFH, int nDACChannel, char *szBuffer)
{
   strncpy(szBuffer, pFH->sDACChannelUnits[nDACChannel], ABF_DACUNITLEN);
   szBuffer[ABF_DACUNITLEN] = '\0';
   StripSpace(szBuffer);
   return (szBuffer[0] != '\0');
}

//===============================================================================================
// FUNCTION: GetDACChannelName
// PURPOSE:  Fills the passed buffer with the channel name for a particular DAC channel.
//           Builds a zero terminated "C" string from a fixed length array of bytes.
//
static void GetDACChannelName(const ABFFileHeader *pFH, int nDACChannel, char *szBuffer)
{
   strncpy(szBuffer, pFH->sDACChannelName[nDACChannel], ABF_DACNAMELEN);
   szBuffer[ABF_DACNAMELEN] = '\0';
   StripSpace(szBuffer);
   if (strlen(szBuffer)==0)
      sprintf(szBuffer, "OUT%d", nDACChannel);
   sprintf(szBuffer+strlen(szBuffer), " (#%d)", nDACChannel);
}

//===============================================================================================
// FUNCTION: FormatFixedWidth
// PURPOSE:  Formats the floating point number so that it contains at most nWidth characters.
//
#define Log10(x) (log(x)/log(10.0))
static void FormatFixedWidth(float fNum, int nWidth, char *dest)
{
   int nDecPlaces = 0;
   char *ps;

   if (fNum != 0.0F)
   {
      if (fNum < 0.0F)
         nWidth--;
      nDecPlaces = nWidth - (int)(Log10(fabs(fNum))) - 2;
      if (nDecPlaces < 0)
         nDecPlaces = 0;
   }

   sprintf(dest, "%.*f", nDecPlaces, fNum);

   // Strip trailing zero's off the resulting string.

   if (nDecPlaces > 0)
   {
      ps = dest+strlen(dest)-1;

      while ((ps > dest) && (*ps == '0'))
         *ps-- = '\0';

      if (*ps == '.')
         *ps = '\0';
   }
}

char *FormatPauses(long lPauses)
{
   if (lPauses==1)
      return "once";
   if (lPauses==2)
      return "twice";
   static char szBuf[32];
   sprintf(szBuf, "%ld times", lPauses);
   return szBuf;
}

//***********************************************************************************************
//***********************************************************************************************
//***
//***  TERSE INFORMATION.
//***
//***********************************************************************************************
//***********************************************************************************************

//===============================================================================================
// FUNCTION: ProtocolName
// PURPOSE:  Formats the Protocol Name in the header.
//
static void ProtocolName( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   WPTRASSERT( pTB );
   ABFH_ASSERT( pFH );

   char szPath[_MAX_PATH] = { 0 };
   strncpy(szPath, pFH->sProtocolPath, ABF_PATHLEN);
   szPath[ABF_PATHLEN] = '\0';
   StripSpace(szPath);

   if( strlen(szPath) > 0 )
      pTB->printf("Protocol: %s\r\n", szPath );
}

//===============================================================================================
// FUNCTION: Annotations
// PURPOSE:  Formats the annotations in the passed CTextBuffer.
//
static void Annotations( CTextBuffer *pTB, const ABFFileHeader *pFH, LPCSTR pszFilePath )
{
   WPTRASSERT( pTB );
   ABFH_ASSERT( pFH );

   if( pFH->lNumAnnotations )
   {
      int nFile         = 0;
      int nError        = 0;
      UINT uMaxSamples  = 0;
      DWORD dwMaxEpi    = 0;

      // Take a copy of the passed in header to ensure it is 6k long.
      ABFFileHeader NewFH;
      ABFH_PromoteHeader( &NewFH, pFH );

      if( ABF_ReadOpen( pszFilePath, &nFile, ABF_DATAFILE, &NewFH, &uMaxSamples, &dwMaxEpi, &nError ) )
      {
         pTB->printf("Annotations:\r\n");

         UINT uMaxSize = ABF_GetMaxAnnotationSize( nFile, &NewFH );
         if( uMaxSize == 0 )
         {
            pTB->printf("none\r\n");
         }
         else
         {
            CArrayPtr<char> pszText( uMaxSize );
            if( !pszText )
               return;

            for( int i=0; i<pFH->lNumAnnotations; i++ )
            {
               if( ABF_ReadAnnotation( nFile, &NewFH, i, pszText, uMaxSize, &nError ) )
               {
                  int ch = '<';
                  char * pszDelimiter = strchr( pszText, ch );
                  if( pszDelimiter )
                  {
                     char szText[256] = { 0 };
                     UINT uNumChars = pszDelimiter - pszText;
                     strncpy( szText, pszText, uNumChars );
                     pTB->printf( "%s = ", szText );

                     ch = '>';
                     pszDelimiter = strchr( pszText, ch );
                     if( pszDelimiter )
                     {
                        pTB->printf( pszDelimiter + 1 );
                     }
                  }
                  else
                     pTB->printf( pszText );

                  pTB->printf("\r\n");
               }
            }
            pTB->printf("\r\n");
         }
      }
      ABF_Close( nFile, &nError );
   }
}      

//===============================================================================================
// FUNCTION: TerseInfo
// PURPOSE:  Formats the "terse" header information component in the passed CTextBuffer.
//
static void TerseInfo( CTextBuffer *pTB, const ABFFileHeader *pFH, LPCSTR pszFilePath )
{
   WPTRASSERT( pTB );
   ABFH_ASSERT( pFH );

   ProtocolName( pTB, pFH );

   char buf[_MAX_PATH];
   BOOL bHasFileName = GetFileName(pszFilePath, buf, sizeof(buf));
   if (bHasFileName)
      pTB->printf("Data File: %s\r\n", buf);
     
   GetCreationDate(pFH->lFileStartTime, pFH->nFileStartMillisecs,
                   pFH->lFileStartDate, pFH->lStopwatchTime, buf);

   if (bHasFileName)
      pTB->printf("File format: %s V%.2f\r\nCreated: %s\r\n", 
                pszFileType[pFH->nFileType-1], pFH->fFileVersionNumber, buf);

   // Get the creator application name and version number if available
   CABFHeader FH( *pFH );
   char szAppName[ABF_CREATORINFOLEN] = {0};
   char szAppVers[ABF_CREATORINFOLEN] = {0};
   FH.GetCreatorInfo(szAppName, sizeof(szAppName), szAppVers, sizeof(szAppVers));
   pTB->printf("Created by: %s %s\r\n", szAppName, szAppVers);

   // Get the modifier application name and version number if available
   FH.GetModifierInfo(szAppName, sizeof(szAppName), szAppVers, sizeof(szAppVers));
   if (pFH->nModifierMajorVersion != 0)
      pTB->printf("Modified by: %s %s\r\n", szAppName, szAppVers);

   pTB->printf("Acquisition mode: %s.\r\n", pszOperationMode[pFH->nOperationMode-1]);
   //abf integer/binary
   char szBuf2[20]={0};
   ASSERT(pFH->nDataFormat==ABF_INTEGERDATA || pFH->nDataFormat==ABF_FLOATDATA);
   if(pFH->nDataFormat==ABF_INTEGERDATA)
      strncpy(szBuf2, "integer", sizeof(szBuf2));   
   else if(pFH->nDataFormat==ABF_FLOATDATA)
      strncpy(szBuf2, "floating point", sizeof(szBuf2));   
   else 
      strncpy(szBuf2, "UNKNOWN DATA FORMAT", sizeof(szBuf2));   
   pTB->printf("Axon binary file (%s)\r\n", szBuf2);
   //Comment
   strncpy(buf, pFH->sFileComment, ABF_FILECOMMENTLEN);
   buf[ABF_FILECOMMENTLEN] = '\0';
   StripSpace(buf);
   if (pFH->lActualAcqLength)
      pTB->printf("%ld samples in this file.\r\n", pFH->lActualAcqLength);
   pTB->printf("Comment: %s\r\n", (strlen(buf) > 0 ? buf : "n/a"));

   pTB->printf("\r\n");
}      

//***********************************************************************************************
//***********************************************************************************************
//***
//***  LONG INFORMATION.
//***
//***********************************************************************************************
//***********************************************************************************************

//===============================================================================================
// FUNCTION: AutoAnalyse
// PURPOSE:  Formats the AutoAnalyse enable and macro name in the header.
//
static void AutoAnalyse( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   WPTRASSERT( pTB );
   ABFH_ASSERT( pFH );

   switch( pFH->nAutoAnalyseEnable )
   {
      case ABF_AUTOANALYSE_RUNMACRO:
      {
         char szName[_MAX_PATH] = { 0 };
         ABFU_GetABFString( szName, ELEMENTS_IN(szName), pFH->sAutoAnalysisMacroName, ABF_MACRONAMELEN );
         if( strlen(szName) > 0 )
            pTB->printf( "Auto analysis script: %s\r\n", szName );
         break;
      }
      case ABF_AUTOANALYSE_DISABLED:
         pTB->printf( "Auto analysis: disabled\r\n" );
         break;
      case ABF_AUTOANALYSE_DEFAULT:
         pTB->printf( "Auto analysis: default\r\n" );
         break;
      default:
         ERRORMSG( "Unknown value of nAutoAnalyseEnable" );
   }
}


//===============================================================================================
// FUNCTION: EnvironmentalInfo
// PURPOSE:  Formats the environmental information (Group 6) in the header.
//
static void EnvironmentalInfo( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   // Identifier #1 = nnn, #2 = nnn, #3 = nnn.
   // Only display if nCommentsEnable is non-zero.

   if (pFH->nCommentsEnable != 0)
   {
      char szID1[32], szID2[32], szID3[32];
      ABFU_FormatDouble(pFH->fCellID1, 6, szID1, sizeof(szID1));
      ABFU_FormatDouble(pFH->fCellID2, 6, szID2, sizeof(szID2));
      ABFU_FormatDouble(pFH->fCellID3, 6, szID3, sizeof(szID3));
      pTB->printf("Identifier #1 = %s, #2 = %s, #3 = %s.\r\n", 
                szID1, szID2, szID3 );
   }

   // Experiment type: voltage/current clamp
   pTB->printf("Experiment type: ");
   switch (pFH->nExperimentType)
   {
      case ABF_VOLTAGECLAMP:
         pTB->printf("voltage clamp.\r\n");
         break;
      case ABF_CURRENTCLAMP:
         pTB->printf("current clamp.\r\n");
         break;
      default:
      case ABF_SIMPLEACQUISITION:
         pTB->printf("other.\r\n");
         break;
   }

   // Display the file GUID.
   char szGuid[64] = { 0 };
   AXU_GUIDToString( pFH->FileGUID, szGuid, ELEMENTS_IN( szGuid ) );
   pTB->printf( "File GUID:       {%s}.\r\n", szGuid); 

   // Instrument telegraphs: disabled.
   // or
   // Geneclamp telegraph parameters:
   // or
   // Manually entered parameters:
   //     Additional gain: x10.
   //     Lowpass filter: bypassed.
   //     Membrane capacitance compensation: 0 pF.

// FIX FIX FIX PRC DEBUG Telegraph changes - check !
   for( int i=0; i<pFH->nADCNumChannels; i++ )
   {
      UINT uChan = pFH->nADCSamplingSeq[i];

      char TempBuf[ABF_ADCNAMELEN*2];
      GetADCChannelName(pFH, uChan, TempBuf);
      pTB->printf("\r\nInput signal: %s.    ", TempBuf);
      if (pFH->nTelegraphEnable[uChan] == 0)
      {
         pTB->printf("Telegraphs disabled.");
         continue;
      }

      if (pFH->nTelegraphEnable[uChan] == 1)
         pTB->printf("%s telegraph parameters:", AutosampleInstrument(pFH->nTelegraphInstrument[uChan]));
      else
         pTB->printf("Manually entered parameters:");

      pTB->printf("\r\n    Additional gain: x%.2f.", pFH->fTelegraphAdditGain[uChan]);

      pTB->printf("\r\n    Lowpass filter ");

      // An AxoPatch can have a filter value of 100k, so only show "bypass" if no instrument in use.
      if( (pFH->fTelegraphFilter[uChan] >= ABF_FILTERDISABLED) &&
          (pFH->nTelegraphInstrument[uChan] == ABF_INST_UNKNOWN) )
         pTB->printf(": bypassed.");
      else
         pTB->printf("frequency: %.2f Hz.", pFH->fTelegraphFilter[uChan]);

      // Membrane capacitance: eg 33 pF. 
      // Do not display membrane capacitance for OpusXPress instruments because 
      // this parameter is not used and can be confused with capacitance neutralization
      if( pFH->nTelegraphInstrument[uChan] != ABF_INST_OPUSXPRESS6000 ) 
         pTB->printf("\r\n    Membrane capacitance: %.2f pF.", pFH->fTelegraphMembraneCap[uChan]);
   }
}

//===============================================================================================
// FUNCTION: FixedLengthSweeps
// PURPOSE:  Displays the number of fixed length sweeps.
//
static void FixedLengthSweeps( CTextBuffer *pTB, const ABFFileHeader *pFH, long lSweeps )
{
   pTB->printf("%ld sweep%s of %ld samples",
             lSweeps, (lSweeps > 1 ? "s" : ""), pFH->lNumSamplesPerEpisode);
   if (pFH->nADCNumChannels > 1)
      pTB->printf(" = %ld samples per channel", pFH->lNumSamplesPerEpisode / pFH->nADCNumChannels);
   pTB->printf(".\r\n");
}

//===============================================================================================
// FUNCTION: ProtocolHeirarchyInfo
// PURPOSE:  Formats the information about the trial heirarchy (Group 3).
//
static void ProtocolHeirarchyInfo( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   pTB->printf("Trial length: ");
   if ((pFH->nOperationMode==ABF_GAPFREEFILE) ||
       (pFH->nOperationMode == ABF_VARLENEVENTS) ||
       (pFH->nOperationMode == ABF_FIXLENEVENTS) )
   {
      // Trial length: use available disk space.
      // Trial length: xx:xx:xx (hh:mm:ss).
      if (pFH->fSecondsPerRun==0.0F)
         pTB->printf("use available disk space.\r\n");
      else
      {
         char buf[32];
         ABFU_FormatHMS( UINT(pFH->fSecondsPerRun+0.5F), buf, sizeof(buf) );
         pTB->printf("%s (hh:mm:ss).\r\n", buf);
      }
   }             
   else if (pFH->nOperationMode == ABF_HIGHSPEEDOSC)
   {
      if (pFH->lEpisodesPerRun==0)
         pTB->printf("use available disk space.\r\n");
      else
         pTB->printf("%d sweeps.\r\n", pFH->lEpisodesPerRun);

      // Only HighspeedOsc files support averaging.
      switch (pFH->nAveragingMode)
      {
         case ABF_SAVEAVERAGEONLY:
            pTB->printf("Only the average sweep will be stored.\r\n" );
            break;
         case ABF_AVERAGESAVEALL:
            pTB->printf("The raw sweeps and the average sweep will be saved.\r\n");
            break;
         default:
            ERRORMSG1("Unexpected Averaging Mode '%d'.", pFH->nAveragingMode);
         case ABF_NOAVERAGING:
            break;
      }
   }
   else if (pFH->nOperationMode == ABF_WAVEFORMFILE)
   {
      // 32 runs; 5 sweeps of 1024 samples = 512 samples per channel.
      if (pFH->lRunsPerTrial > 1)
         pTB->printf("%ld run%s; ", pFH->lRunsPerTrial, (pFH->lRunsPerTrial > 1) ? "s" : "");

      FixedLengthSweeps( pTB, pFH, pFH->lEpisodesPerRun );

      // Only the averaged run will be saved.
      if (pFH->lRunsPerTrial > 1)
         pTB->printf("Only the averaged run will be stored.\r\n");
   }      
}

//===============================================================================================
// FUNCTION: FileHeirarchyInfo
// PURPOSE:  Formats the information about the trial heirarchy (Group 3).
//
static void FileHeirarchyInfo( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   if (pFH->nOperationMode == ABF_VARLENEVENTS)
   {
      ASSERT(pFH->lActualEpisodes> 0);

      // nnn variable-length segments.
      pTB->printf("%ld variable-length segment%s.\r\n",
                pFH->lActualEpisodes, (pFH->lActualEpisodes > 1 ? "s" : ""));
   }
   else if (pFH->nOperationMode==ABF_GAPFREEFILE)
   {
      // Acquisition was paused 3 times.   
      if (pFH->lSynchArraySize > 1)
         pTB->printf("Acquisition was paused %s\r\n", FormatPauses(pFH->lSynchArraySize-1));
   }             
   else if (pFH->nOperationMode == ABF_WAVEFORMFILE)
   {
      ASSERT(pFH->lActualEpisodes> 0);

      // 32 runs; 5 sweeps of 1024 samples = 512 samples per channel.
      pTB->printf("Trial length: ");
      if (pFH->lAverageCount > 0)
         pTB->printf("%ld run%s; ", pFH->lAverageCount, (pFH->lAverageCount > 1 ? "s" : ""));

      FixedLengthSweeps( pTB, pFH, pFH->lActualEpisodes );

      if ((pFH->nAveragingMode != ABF_NOAVERAGING) && (pFH->lAverageCount > 0))
      {
         // xxx raw episodes contributed to each averaged episode.
         pTB->printf("%ld raw sweeps contributed to each averaged sweep.\r\n", pFH->lAverageCount);
         pTB->printf("Only the average run was stored.\r\n");
      }
   }
   else if (pFH->nOperationMode == ABF_FIXLENEVENTS)
   {
      FixedLengthSweeps( pTB, pFH, pFH->lActualEpisodes );
   }
   else if (pFH->nOperationMode == ABF_HIGHSPEEDOSC)
   {
      ASSERT(pFH->lActualEpisodes> 0);

      // Only HighspeedOsc files support averaging.
      switch (pFH->nAveragingMode)
      {
         case ABF_NOAVERAGING:
            FixedLengthSweeps( pTB, pFH, pFH->lActualEpisodes );
            break;
         case ABF_SAVEAVERAGEONLY:
            // 1 sweep of 1024 samples = 512 samples per channel.
            pTB->printf("1 sweep of %ld samples = %ld samples per channel.\r\n",
                      pFH->lNumSamplesPerEpisode, 
                      pFH->lNumSamplesPerEpisode / pFH->nADCNumChannels);
            // xxx sweeps contributed to the average sweep.
            if (pFH->lAverageCount > 0)
            {
               pTB->printf("%ld sweep%s contributed to the average sweep.\r\n", 
                         pFH->lAverageCount, (pFH->lAverageCount > 1 ? "s" : ""));
               pTB->printf("Only the average sweep was stored.\r\n" );
            }
            break;
         case ABF_AVERAGESAVEALL:
            FixedLengthSweeps( pTB, pFH, pFH->lActualEpisodes );
            // xxx raw sweeps contributed to each averaged sweep.
            if (pFH->lAverageCount)
               pTB->printf("%ld raw sweeps contributed to the average sweep.\r\n", pFH->lAverageCount);
            break;
         default:
            ERRORMSG1("Unexpected Averaging Mode '%d'.", pFH->nAveragingMode);
            break;
      }
   }
}

//===============================================================================================
// FUNCTION: Triggering
// PURPOSE:  Displays the triggering information.
//
static void Triggering( CTextBuffer *pTB, const ABFFileHeader *pFH, BOOL bFutureTense )
{
   if (pFH->nOperationMode == ABF_GAPFREEFILE)
      return;

   LPSTR pszWillBe = bFutureTense ? "will be" : "were";

   if (pFH->nOperationMode == ABF_WAVEFORMFILE)
   {
      // 4 s between run starts; 0.3 s between sweep starts.
      // Runs externally triggered; Sweeps externally triggered.
      // Runs spacebar triggered; Sweeps spacebar triggered.
      if ((pFH->nTriggerAction == ABF_TRIGGER_STARTRUN) && (pFH->nTriggerSource == ABF_TRIGGERSPACEBAR))
         pTB->printf("Runs %s spacebar triggered; ", pszWillBe);
      else if ((pFH->nTriggerAction == ABF_TRIGGER_STARTRUN) && (pFH->nTriggerSource == ABF_TRIGGEREXTERNAL))
         pTB->printf("Runs %s externally triggered; ", pszWillBe);
      else
      {
         if (pFH->fRunStartToStart == 0.0)
            pTB->printf("Minimum time");
         else
            pTB->printf("%.3f s", pFH->fRunStartToStart);
         pTB->printf(" between run starts.\r\n");
      }

      if ((pFH->nTriggerAction == ABF_TRIGGER_STARTEPISODE) && (pFH->nTriggerSource == ABF_TRIGGERSPACEBAR))
         pTB->printf("Sweeps %s spacebar triggered.", pszWillBe);
      else if ((pFH->nTriggerAction == ABF_TRIGGER_STARTEPISODE) && (pFH->nTriggerSource == ABF_TRIGGEREXTERNAL))
         pTB->printf("Sweeps %s externally triggered.", pszWillBe);
      else
      {
         if (pFH->fEpisodeStartToStart == 0.0)
            pTB->printf("Minimum time");
         else
            pTB->printf("%.3f s", pFH->fEpisodeStartToStart);
         pTB->printf(" between sweep starts.");
      }
      pTB->printf("\r\n");
   }
   else
   {
      // Pre and post trigger samples: XX samples per channel.
      pTB->printf("Pre-");
      if (pFH->nOperationMode == ABF_VARLENEVENTS)
         pTB->printf(" and post-");
      pTB->printf("trigger length: %ld samples", pFH->lPreTriggerSamples/pFH->nADCNumChannels);
      if (pFH->nADCNumChannels!=1)
         pTB->printf(" per channel");
      pTB->printf(".\r\n");

      if (pFH->nTriggerSource == ABF_TRIGGERSPACEBAR)
         pTB->printf("Sweeps %s spacebar triggered", pszWillBe);
      else if (pFH->nTriggerSource == ABF_TRIGGEREXTERNAL)
         pTB->printf("Sweeps %s externally triggered", pszWillBe);
      else if (pFH->nTriggerSource == ABF_TRIGGERTAGINPUT)
         pTB->printf("Sweeps %s tag-input triggered", pszWillBe);
      else
      {
         // Event detection threshold: XX units, rising edge, on ANALOG IN #1.
         pTB->printf("Event detection threshold: %.2f", pFH->fTriggerThreshold);
         int nTriggerChannel = pFH->nTriggerSource;
         if ((pFH->nADCNumChannels==1) || (pFH->nTriggerSource==ABF_TRIGGERFIRSTCHANNEL))
            nTriggerChannel = pFH->nADCSamplingSeq[0];
      
         char TempBuf[ABF_ADCNAMELEN*2];
         if (GetADCChannelUnits(pFH, nTriggerChannel, TempBuf))
            pTB->printf(" %s", TempBuf);
      
         pTB->printf(", %s edge", (pFH->nTriggerPolarity) ? "falling" : "rising");
         if (pFH->nADCNumChannels>1)
         {
            GetADCChannelName(pFH, nTriggerChannel, TempBuf);
            pTB->printf(", on signal %s", TempBuf);
         }
      }
      pTB->printf(".\r\n");
   }
}

//===============================================================================================
// FUNCTION: SampleIntervals
// PURPOSE:  Displays the acquisition sampling intervals.
//
static void SampleIntervals( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   // Report the PER CHANNEL sampling interval.
   // Sampling interval: 50 us.
   // First sampling interval: 50 us.
   // Second sample interval: 100 us.
   // Sample interval change: halfway through sweep.
   // Sample interval change: sample 100.

   char szOptionalK[2] = {0};
   double dADCSampleRate = 0;
   //If the interval is less than 1ms (1000us) then report the frequency
   //in Hz rather than kHz
   if((pFH->fADCSampleInterval * pFH->nADCNumChannels) <= 1e3  )
   {  
      szOptionalK[0] = 'k';
      dADCSampleRate = (double)1e3 / (double)(pFH->fADCSampleInterval * pFH->nADCNumChannels);
   }
   else
   {
      dADCSampleRate = (double)1e6 / (double)(pFH->fADCSampleInterval * pFH->nADCNumChannels);
   }

   
   if ((pFH->nOperationMode == ABF_WAVEFORMFILE) && (pFH->fADCSecondSampleInterval != 0))
   {
      double dADCSecondSampleRate = 0;
      char szSecondOptionalK[2] = {0};
      //If the interval is less than 1ms (1000us) then report the frequency
      //in Hz rather than kHz
      if((pFH->fADCSecondSampleInterval * pFH->nADCNumChannels) <= 1e3  )
      {  
         szSecondOptionalK[0] = 'k';
         dADCSecondSampleRate = (double)1e3 / (double)(pFH->fADCSecondSampleInterval * pFH->nADCNumChannels);
      }
      else
      {
         dADCSecondSampleRate = (double)1e6 / (double)(pFH->fADCSecondSampleInterval * pFH->nADCNumChannels);
      }

      pTB->printf("First sampling interval: %.2f %cs", pFH->fADCSampleInterval * pFH->nADCNumChannels, MICRO);
      if (pFH->nADCNumChannels > 1)
         pTB->printf("/channel (%.2f %sHz)", dADCSampleRate, szOptionalK);
      pTB->printf(".\r\nSecond sample interval: %.2f %cs", pFH->fADCSecondSampleInterval * pFH->nADCNumChannels, MICRO);
      if (pFH->nADCNumChannels > 1)
         pTB->printf("/channel (%.2f %sHz)", dADCSecondSampleRate, szSecondOptionalK);
      pTB->printf(".\r\nSample interval changed ");
      if (pFH->lClockChange == 0)
         pTB->printf("halfway through sweep");
      else
         pTB->printf("on sample %ld", pFH->lClockChange / pFH->nADCNumChannels);
      pTB->printf(".\r\n");
   }
   else
   {
      pTB->printf("Sampling interval: %.2f %cs", pFH->fADCSampleInterval * pFH->nADCNumChannels, MICRO);
      if (pFH->nADCNumChannels > 1)
         pTB->printf("/channel (%.2f %sHz)", dADCSampleRate, szOptionalK);
      pTB->printf(".\r\n\r\n");
   }
}

   
//===============================================================================================
// FUNCTION: SamplingSequence
// PURPOSE:  Displays the current input channel sampling sequence.
//
static void SamplingSequence( CTextBuffer *pTB, const ABFFileHeader *pFH )
{
   char TempBuf[ABF_ADCNAMELEN*2];
   GetADCChannelName(pFH, pFH->nADCSamplingSeq[0], TempBuf);

   // Input signal: IN0 (#0).
   // Input signals: IN0 (#0), IN2 (#2) and IN15 (#15).
   if (pFH->nADCNumChannels==1)
      pTB->printf("Input signal: %s.\r\n", TempBuf);
   else
   {
      UINT uCount = pTB->GetBytesUsed();
      pTB->printf("Input signals: %s", TempBuf);
      for (int i=1; i < pFH->nADCNumChannels-1; i++)
      {
         pTB->printf(", ");
         GetADCChannelName(pFH, pFH->nADCSamplingSeq[i], TempBuf);

         if (pTB->GetBytesUsed()-uCount+strlen(TempBuf) > 60)
         {
            pTB->printf("\r\n");
            uCount = pTB->GetBytesUsed();
         }

         pTB->printf("%s", TempBuf);
      }

      GetADCChannelName(pFH, pFH->nADCSamplingSeq[i], TempBuf);
      if (pTB->GetBytesUsed()-uCount+strlen(TempBuf)+6 > 60)
         pTB->printf("\r\n");
      else
         pTB->printf(" ");
      pTB->printf("and %s.\r\n", TempBuf);
   }
}

//===============================================================================================
// FUNCTION: DACHoldingInfo
// PURPOSE:  Formats the DAC holding levels (Group 7, Part 1)
//
static void DACHoldingInfo(CTextBuffer *pTB, const ABFFileHeader *pFH)
{
   char TempBuf[ABF_DACNAMELEN*2];
   
   // ANALOG OUT #0 holding level: -80 mV.
   // ANALOG OUT #1 holding level: -90 mV.
   for (int i=0; i<2; i++)
   {
      GetDACChannelName(pFH, i, TempBuf);
      pTB->printf("Signal %s holding level: %.2f", TempBuf, pFH->fDACHoldingLevel[i]);
      if (GetDACChannelUnits(pFH, i, TempBuf))
         pTB->printf(" %s", TempBuf);
      pTB->printf(".\r\n");
   }
}

//===============================================================================================
// FUNCTION: GetEpochType
// PURPOSE:  Returns the string that corresponds to the epoch type.
//
static LPCSTR GetEpochType(const ABFFileHeader *pFH, UINT uDACChannel, UINT uEpoch)
{
   ASSERT( uDACChannel < ABF_WAVEFORMCOUNT );

   switch (pFH->nEpochType[uDACChannel][uEpoch])
   {
      case 0:
         return "Off    ";
      case 1:
         return "Step   ";
      case 2:
         return "Ramp   ";
      case 3:
         return "Train  ";
      case 4:
         return "Trngl  ";
      case 5:
         return "Cos    ";
      case 6:
         return "Rstnce ";
      case 7:
         return "Biphsc ";

      default:
         ERRORMSG( "Unexpected epoch type." );
         return "       ";
   }
}

//===============================================================================================
// FUNCTION: WaveformInfo
// PURPOSE:  Formats the information about the waveform generated. (Group 9)
//           (ABF_WAVEFORM files only).
//
static void WaveformInfo( CTextBuffer *pTB, const ABFFileHeader *pFH, UINT uDACChannel )
{
   ASSERT( uDACChannel < ABF_WAVEFORMCOUNT );

   char LineBuf[80], TempBuf[80];
   int i;

   // DIGITAL OUT holding: 0101; Holding value used for Inter-sweep pattern.
   // DIGITAL OUT holding: 0101; Last epoch value used for Inter-sweep pattern.
   // DIGITAL OUT pattern: disabled.
   if( pFH->nActiveDACChannel == (int)uDACChannel )
   {
      if( pFH->nDigitalEnable == 0 )
         pTB->printf("DIGITAL OUT pattern: disabled.\r\n");
      else
      {
         pTB->printf("DIGITAL OUT holding: %s; ", FormatDigitalOuts(pFH->nDigitalHolding, 0, LineBuf));
      
         if (pFH->nDigitalInterEpisode == 0)
            pTB->printf("Holding");
         else
            pTB->printf("Last epoch");
         pTB->printf(" value used for intersweep pattern.\r\n");
      }
      pTB->printf("\r\n" );   // Blank line
   }
   
   // Waveform on ANALOG OUT #0:
   // Waveform on ANALOG OUT #0: taken from c:\data\DACFILE.DAT
   // ANALOG OUT waveform: disabled.
   if (pFH->nWaveformEnable[uDACChannel] == FALSE)
   {
      pTB->printf("ANALOG OUT waveform #%d: disabled.\r\n", uDACChannel);
      return;
   }
   
   pTB->printf("Waveform on ANALOG OUT #%d:", uDACChannel);
   if (pFH->nWaveformSource[uDACChannel] == ABF_DACFILEWAVEFORM)
   {
      char szPath[_MAX_PATH];
      char szName[_MAX_FNAME];
      char szExt[_MAX_EXT];
      strncpy(szPath, pFH->sDACFilePath[uDACChannel], ABF_PATHLEN);
      szPath[ABF_PATHLEN] = '\0';
      StripSpace(szPath);
      _splitpath(szPath, NULL, NULL, szName, szExt);
      _makepath(szPath, NULL, NULL, szName, szExt);
      pTB->printf(" taken from %s\r\n", szPath);

      // using Channel #0, sweep 5.
      // using Channel #0, all sweeps.
      // using Channel #0, all sweeps except first.
      pTB->printf("using Channel #%d, ", pFH->nDACFileADCNum[uDACChannel]);
      switch (pFH->lDACFileEpisodeNum[uDACChannel])
      {
         case ABF_DACFILE_SKIPFIRSTSWEEP:
            pTB->printf("all sweeps except first.\r\n");
            break;
         case ABF_DACFILE_USEALLSWEEPS:
            pTB->printf("all sweeps.\r\n");
            break;
         default:
            pTB->printf("sweep %d.\r\n", pFH->lDACFileEpisodeNum[uDACChannel]);
            break;
      }

      // Scale factor: x20; Offset: 30 mV.
      pTB->printf("Scale factor: x%.2f; Offset: %.2f", pFH->fDACFileScale[uDACChannel], pFH->fDACFileOffset[uDACChannel]);
      if (GetDACChannelUnits(pFH, uDACChannel, TempBuf))
         pTB->printf(" %s", TempBuf);
      pTB->printf(".\r\n");
      return;
   }

   pTB->printf("\r\n");

   // EPOCH                    A      B      C      D      E      F      G      H
   pTB->printf("EPOCH                    A      B      C      D      E      F      G      H\r\n");

   // Type                     Off    Step   Ramp   Off    Step   Ramp   Off    Step
   pTB->printf("Type                     ");
   for (i = 0; i < 8; i++)
      pTB->printf(GetEpochType(pFH, uDACChannel, i));

   // First level (mV)         22     33     44     55     66     77     88     99
   
   if (GetDACChannelUnits(pFH, uDACChannel, TempBuf))
      sprintf(LineBuf, "(%s)", TempBuf);
   else
      LineBuf[0] = '\0';
   pTB->printf("\r\nFirst level %-13.13s", LineBuf);
   
   for (i = 0; i <= 7; i++)
   {
      FormatFixedWidth(pFH->fEpochInitLevel[uDACChannel][i], 6, TempBuf);
      pTB->printf("%-7.7s", TempBuf);
   }

   // Delta level (mV)         10     10     0      0      0      0      0      0
   pTB->printf("\r\nDelta level %-13.13s", LineBuf);
   for (i = 0; i <= 7; i++)
   {
      FormatFixedWidth(pFH->fEpochLevelInc[uDACChannel][i], 6, TempBuf);
      pTB->printf("%-7.7s", TempBuf);
   }

   // First duration (samples) 100    200    300    400    500    600    700    800
   pTB->printf("\r\nFirst duration (samples) ");
   for (i = 0; i <= 7; i++)
      pTB->printf("%-7d", pFH->lEpochInitDuration[uDACChannel][i]);

   // Delta duration (samples) 0      55     0      0      0      0      0      0
   pTB->printf("\r\nDelta duration (samples) ");
   for (i = 0; i <= 7; i++)
      pTB->printf("%-7d", pFH->lEpochDurationInc[uDACChannel][i]);

   // Digital pattern          101*   1111   1010   1111   1010   1111   1010   1111
   if( (pFH->nActiveDACChannel == (int)uDACChannel) &&
       (pFH->nDigitalEnable != 0) )
   {
      pTB->printf("\r\nDigital pattern          ");
      for (i = 0; i <= 7; i++)
         pTB->printf("%-7.7s", FormatDigitalOuts(pFH->nDigitalValue[i], pFH->nDigitalTrainValue[i],TempBuf));
   }
   // Train Period (samples)   100    0      0      0      0      0      0      0
   pTB->printf("\r\nTrain Period (samples)   ");
   for (i = 0; i <= 7; i++)
      pTB->printf("%-7d", pFH->lEpochPulsePeriod[uDACChannel][i]);

   // Pulse Width (samples)    10     0      0      0      0      0      0      0
   pTB->printf("\r\nPulse Width (samples)    ");
   for (i = 0; i <= 7; i++)
      pTB->printf("%-7d", pFH->lEpochPulseWidth[uDACChannel][i]);

   pTB->printf("\r\n");

   // Last epoch amplitude used for Inter-sweep ANALOG OUT.
   // Inter-sweep ANALOG OUT: same as ANALOG OUT #n.
   if (pFH->nInterEpisodeLevel[uDACChannel] == ABF_INTEREPI_USEHOLDING)
   {
      GetDACChannelName(pFH, uDACChannel, TempBuf);
      pTB->printf("Intersweep holding: same as for signal %s.\r\n", TempBuf);
   }
   else
      pTB->printf("Last epoch amplitude used for intersweep holding.\r\n");

   // Display the active logic setting for digital trains if train is on
   bool bTrainOn = false;
   for (i = 0; i <= 7; i++)
   {
      if( pFH->nDigitalTrainValue[i] != 0 )
         bTrainOn = true;
   }
   if( bTrainOn )
      pTB->printf("Digital train active logic: %d.\r\n", pFH->nDigitalTrainActiveLogic );

   // Blank line
   pTB->printf("\r\n");
}

//===============================================================================================
// FUNCTION: DigitalPulseInfo
// PURPOSE:  Shows information stored for the Digital Pulses (Group 8) (Superceded)
//
static BOOL DigitalPulseInfo(CTextBuffer *pTB, const ABFFileHeader *pFH)
{
   if (pFH->nOUTEnable == 0)
      return FALSE;

   // First and last sweeps for digital pulses           3      0
   pTB->printf("First and last sweeps for digital pulses           %-7d%d\r\n",
             pFH->nFirstEpisodeOUT, pFH->nLastEpisodeOUT);

   // Sample number and duration for ch1 digital pulse     10     10 ms
   pTB->printf("Sample number and duration for ch1 digital pulse     %-7d%d ms\r\n",
          pFH->nSampleNumberOUT1, pFH->nPulseSamplesOUT1);

   // Sample number and duration for ch2 digital pulse     0      10 ms
   pTB->printf("Sample number and duration for ch2 digital pulse     %-7d%d ms\r\n",
          pFH->nSampleNumberOUT2, pFH->nPulseSamplesOUT2);
   return TRUE;
}

//===============================================================================================
// FUNCTION: ConditioningTrainInfo
// PURPOSE:  Parameters that describe the presweep (conditioning) train. (Group 11)
//
static void ConditioningTrainInfo(CTextBuffer *pTB, const ABFFileHeader *pFH, UINT uDAC)
{
   char TempBuf[128];

   // Presweep train on ANALOG OUT #1. 450 pulses in train.
   // Presweep train: disabled.

   // If both trains are disabled refer to both.
   if( (pFH->nConditEnable[0] == 0) &&
       (pFH->nConditEnable[1] == 0) )
   {
      if( uDAC == 0 )
         pTB->printf("Pre-sweep trains: disabled.\r\n");
      return;
   }

   
   pTB->printf("Pre-sweep train (#%d)", uDAC);
   if (pFH->nConditEnable[uDAC] == 0)
   {
      pTB->printf(": disabled.\r\n");
      return;
   }

   pTB->printf(" on ANALOG OUT #%d. %ld pulses in train.\r\n", 
               uDAC, pFH->lConditNumPulses[uDAC]);

   // Baseline, step and post-train period (ms) = 10, 30, 200
   pTB->printf("Baseline, step and post-train period (ms) = %.2f, %.2f, %.2f\r\n", 
            pFH->fBaselineDuration[uDAC], pFH->fStepDuration[uDAC], pFH->fPostTrainPeriod[uDAC]);

   // Baseline, step and post-train level (mV) = -90, 50, -90
   pTB->printf("Baseline, step and post-train level ");
   if (GetDACChannelUnits(pFH, uDAC, TempBuf))
      pTB->printf("(%s) ", TempBuf);
   pTB->printf("= %.2f, %.2f %.2f\r\n", pFH->fBaselineLevel[uDAC], pFH->fStepLevel[uDAC], 
               pFH->fPostTrainLevel[uDAC]);
}

//===============================================================================================
// FUNCTION: LeakSubtractionInfo
// PURPOSE:  Parameters that describe the leak subtraction (Group 15).
//
static void LeakSubtractionInfo(CTextBuffer *pTB, const ABFFileHeader *pFH, UINT uDAC)
{
   ABFH_ASSERT(pFH);
   ASSERT( uDAC < ABF_WAVEFORMCOUNT );


   char TempBuf[128];

   // Leak subtraction on ANALOG IN #1.
   // Leak subtraction: disabled.
   
   // If both P/N's are disabled refer to both.
   if( (pFH->nPNEnable[0] == 0) &&
       (pFH->nPNEnable[1] == 0) )
   {
      if( uDAC == 0 )
         pTB->printf("Leak subtraction: disabled.\r\n");
      return;
   }

   if (pFH->nPNEnable[uDAC] == 0)
   {
      pTB->printf("Leak subtraction (#%d)", uDAC);
      pTB->printf(": disabled.\r\n");
      return;
   }

   pTB->printf("Leak subtraction (#%d) signals: ", uDAC);
   for( int i=0; i<pFH->nPNNumADCChannels[uDAC]; i++ )
   {
      GetADCChannelName(pFH, pFH->nPNADCSamplingSeq[uDAC][i], TempBuf);
      pTB->printf("  %s,", TempBuf);
   }
   pTB->printf("\r\n");

   // 4 subpulses of opposite polarity to waveform executed before sweep.
   pTB->printf("%d subpulses of ", pFH->nPNNumPulses);
   if (pFH->nPNPolarity[uDAC] == 1)
      pTB->printf("same polarity as");
   else
      pTB->printf("opposite polarity to");
   pTB->printf(" waveform executed ");
   if (pFH->nPNPosition == 1)
      pTB->printf("after");
   else
      pTB->printf("before");
   pTB->printf(" sweep.\r\n");

   // Subpulse holding level: -120 mV. Settling time: 300 ms.
   pTB->printf("Subpulse holding level: %.2f", pFH->fPNHoldingLevel[uDAC]);
   if (GetDACChannelUnits(pFH, uDAC, TempBuf))
      pTB->printf(" %s", TempBuf);

   pTB->printf(".\r\nSettling time: %.2f ms.\r\n", pFH->fPNSettlingTime);

   // Interpulse time: 20 ms.
   pTB->printf("Interpulse time: %.2f ms.\r\n", pFH->fPNInterpulse);
}

//===============================================================================================
// FUNCTION: UserListInfo
// PURPOSE:  Describes the user list and the parameter that it is attached to (Group 12).
//
static void UserListInfo(CTextBuffer *pTB, const ABFFileHeader *pFH, UINT uListNum)
{
   char LineBuf[ABF_USERLISTLEN+1] = {0};

   // User list: disabled.
   // User list applied to the number of pulses in train parameter.

   // If both lists are disabled refer to both.
   if( (pFH->nULEnable[0] == 0) &&
       (pFH->nULEnable[1] == 0) )
   {
      if( uListNum == 0 )
         pTB->printf("User list: disabled.\r\n");
      return;
   }

   pTB->printf("User list (#%d)", uListNum);

   if (pFH->nULEnable[uListNum] == 0)
   {
      pTB->printf(": disabled.\r\n");
      return;
   }

   pTB->printf(" applied to the %s parameter. ", GetParameterList(pFH->nULParamToVary[uListNum]));

   // Repeat option
   if( pFH->nULRepeat[uListNum] == 1 )
      pTB->printf("%s\r\n", "Repeat enabled.");
   else
      pTB->printf("%s\r\n", "Repeat disabled.");

   // List =
   pTB->printf("List (#%d) = ", uListNum);

   // 9,18,75,34,67,98,51,6,34,9,165,93,4,8,71,6,59,8,7,34,6,5,9
   strncpy(LineBuf, pFH->sULParamValueList[uListNum], ABF_USERLISTLEN);
   LineBuf[ABF_USERLISTLEN+1] = '\0';
   StripSpace(LineBuf);
   pTB->printf("%s\r\n", LineBuf);
}

//===============================================================================================
// FUNCTION: MathChannelInfo
// PURPOSE:  Describes the parameters and formulae for the math channel (Group 14).
//
static void MathChannelInfo(CTextBuffer *pTB, const ABFFileHeader *pFH)
{
   char TempBuf[40];

   pTB->printf("Channel math");
   // Channel math evaluated using the ratio dyes expression, with
   // Channel math: disabled.
   if (pFH->nArithmeticEnable == 0)
   {
      pTB->printf(": disabled.\r\n");
      return;
   }

   pTB->printf(" evaluated using the ");
   if (pFH->nArithmeticExpression == ABF_SIMPLE_EXPRESSION)
      pTB->printf("general purpose");
   else
      pTB->printf("ratio dyes");
   pTB->printf(" expression, with\r\n");

   char SignalA[ABF_ADCNAMELEN*2], SignalB[ABF_ADCNAMELEN*2];
   GetADCChannelName(pFH, pFH->nArithmeticADCNumA, SignalA);
   GetADCChannelName(pFH, pFH->nArithmeticADCNumB, SignalB);

   if (pFH->nArithmeticExpression == ABF_RATIO_EXPRESSION)
   {
      pTB->printf("    Result = (%.2f * R + %.2f) ", pFH->fArithmeticK1, pFH->fArithmeticK2);
      pTB->printf("%c (%.2f * R + %.2f)", 
                pFH->sArithmeticOperator[0], pFH->fArithmeticK3, pFH->fArithmeticK4);
      
      strncpy(TempBuf, pFH->sArithmeticUnits, ABF_ARITHMETICUNITSLEN);
      TempBuf[ABF_ARITHMETICUNITSLEN] = '\0';
      StripSpace(TempBuf);
      if (TempBuf[0]!='\0')
         pTB->printf(" %s", TempBuf);
      pTB->printf(",\r\n");

      pTB->printf("where\r\n");
      pTB->printf("    R = [Signal %s + %.2f] / [Signal %s + %.2f].\r\n", 
         SignalA, pFH->fArithmeticK5, SignalB, pFH->fArithmeticK6);
   }
   else
   {
      pTB->printf("    Result = [%.2f * Signal %s + %.2f] %c ", 
         pFH->fArithmeticK1, SignalA, pFH->fArithmeticK2, pFH->sArithmeticOperator[0]);
      pTB->printf("(%.2f * Signal %s + %.2f)", 
         pFH->fArithmeticK3, SignalB, pFH->fArithmeticK4);
         
      strncpy(TempBuf, pFH->sArithmeticUnits, ABF_ARITHMETICUNITSLEN);
      TempBuf[ABF_ARITHMETICUNITSLEN] = '\0';
      StripSpace(TempBuf);
      if (TempBuf[0]!='\0')
         pTB->printf(" %s", TempBuf);
      pTB->printf(".\r\n");
   }
}

//===============================================================================================
// FUNCTION: GetLogChannelNum
// PURPOSE:  Fills the buffer with the logical channel that corresponds to an index into the
//           sampling sequence.
//
static char *GetLogChannelNum(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
      sprintf(LineBuf, "%d", pFH->nADCPtoLChannelMap[nPhysicalChannel]);
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetFilterType
// PURPOSE:  Gets the filter type for a particular index into the sampling sequence.
//
static char *GetFilterType(const ABFFileHeader *pFH, int nSeqIndex, BOOL bLowPass, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
   {
      short nType = bLowPass ? pFH->nLowpassFilterType[nPhysicalChannel]
                             : pFH->nHighpassFilterType[nPhysicalChannel];

      switch( nType )
      {
         case ABF_FILTER_NONE:
            strcpy(LineBuf, "Disabled");
            break;

         case ABF_FILTER_EXTERNAL:
            strcpy(LineBuf, "CyberAmp");
            break;

         case ABF_FILTER_SIMPLE_RC:
            strcpy(LineBuf, "Software");
            break;

         case ABF_FILTER_BESSEL:
            strcpy(LineBuf, "Bessel");
            break;

         case ABF_FILTER_BUTTERWORTH:
            strcpy(LineBuf, "Butterworth");
            break;
      }

   }
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetLowpassFilterValue
// PURPOSE:  Gets the lowpass filter value for a particular index into the sampling sequence.
//
static char *GetLowpassFilterValue(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
   {
      if( pFH->nLowpassFilterType[nPhysicalChannel] == ABF_FILTER_NONE )
         strcpy(LineBuf, "n/a");
      else
      {
         float fFilter = pFH->fSignalLowpassFilter[nPhysicalChannel];
         if (fFilter == ABF_FILTERDISABLED || fFilter < 0.0F)
            strcpy(LineBuf, "Bypass");
         else
            sprintf(LineBuf, "%.0f", fFilter);
      }
   }
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetHighpassFilterValue
// PURPOSE:  Gets the Highpass filter value for a particular index into the sampling sequence.
//
static char *GetHighpassFilterValue(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
   {
      if( pFH->nHighpassFilterType[nPhysicalChannel] == ABF_FILTER_NONE )
         strcpy(LineBuf, "n/a");
      else
      {
         float fFilter = pFH->fSignalHighpassFilter[nPhysicalChannel];
         if (fFilter < 0.0F)
            strcpy(LineBuf, "GND");
         else
            sprintf(LineBuf, "%.1f", fFilter);
      }
   }
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetPostProcessLowpassFilterType
// PURPOSE:  Gets the post-processing lowpasss filter type for a particular index into the sampling sequence.
//
static char *GetPostProcessLowpassFilterType(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
   {
      short nType = pFH->nPostProcessLowpassFilterType[nPhysicalChannel];

      switch( nType )
      {
         case ABF_POSTPROCESS_FILTER_NONE:
            strcpy(LineBuf, "Disabled");
            break;

         case ABF_POSTPROCESS_FILTER_BESSEL:
            strcpy(LineBuf, "Bessel (8 pole)");
            break;

         case ABF_POSTPROCESS_FILTER_BOXCAR:
            strcpy(LineBuf, "Boxcar");
            break;

         case ABF_POSTPROCESS_FILTER_BUTTERWORTH:
            strcpy(LineBuf, "Butterworth (8 pole)");
            break;

         case ABF_POSTPROCESS_FILTER_CHEBYSHEV:
            strcpy(LineBuf, "Chebyshev (8 pole)");
            break;

         case ABF_POSTPROCESS_FILTER_GAUSSIAN:
            strcpy(LineBuf, "Gaussian");
            break;

         case ABF_POSTPROCESS_FILTER_RC:
            strcpy(LineBuf, "RC (single pole)");
            break;

         case ABF_POSTPROCESS_FILTER_RC8:
            strcpy(LineBuf, "RC (8 coincident pole)");
            break;

         case ABF_POSTPROCESS_FILTER_ADAPTIVE:
            strcpy(LineBuf, "Adaptive");
            break;

         case ABF_POSTPROCESS_FILTER_NOTCH:
            strcpy(LineBuf, "Notch");
            break;

         default:
            strcpy(LineBuf, "ERROR - corrupt file");
            break;

      }
   }
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetPostProcessLowpassFilterValue
// PURPOSE:  Gets the post-processing lowpass filter value for a particular index into the sampling sequence.
//
static char *GetPostProcessLowpassFilterValue(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
   {
      if( pFH->nPostProcessLowpassFilterType[nPhysicalChannel] == ABF_POSTPROCESS_FILTER_NONE )
         strcpy(LineBuf, "n/a");
      else
      {
         float fFilter = pFH->fPostProcessLowpassFilter[nPhysicalChannel];
         if (fFilter == ABF_FILTERDISABLED || fFilter < 0.0F)
            strcpy(LineBuf, "Bypass");
         else
            sprintf(LineBuf, "%.0f", fFilter);
      }
   }
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetGainValue
// PURPOSE:  Gets the gain value for a particular index into the sampling sequence.
//
static char *GetGainValue(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
      sprintf(LineBuf, "%.1fx", pFH->fSignalGain[nPhysicalChannel]);
   return LineBuf;
}

//===============================================================================================
// FUNCTION: GetOffsetValue
// PURPOSE:  Gets the offset value for a particular index into the sampling sequence.
//
static char *GetOffsetValue(const ABFFileHeader *pFH, int nSeqIndex, char *LineBuf)
{
   int nPhysicalChannel = pFH->nADCSamplingSeq[nSeqIndex];
   if (nPhysicalChannel == -1)
      LineBuf[0] = '\0';
   else
      sprintf(LineBuf, "%.1f", pFH->fSignalOffset[nPhysicalChannel]);
   return LineBuf;
}

//===============================================================================================
// FUNCTION: AddSignalCondValues
// PURPOSE:  Write out the signal conditioner values for a given range of entries in the
//           sampling sequence.
//
static void AddSignalCondValues(CTextBuffer *pTB, const ABFFileHeader *pFH, 
                                int nFirstChannel, int nLastChannel)
{
   int i;
   char LineBuf[40];

   // If this is the second time through this sub, and no further channels
   // remain to be displayed, simply return.
   if (pFH->nADCSamplingSeq[nFirstChannel] < 0)
      return;

   // ADC channel:      0      1      2      3      4      5      6      7
   pTB->printf("ADC channel:   ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetLogChannelNum(pFH, i, LineBuf));
   pTB->printf("\r\n");

   // Lowpass Type:     Software    CyberAmp    Disabled    Disabled    Disabled    Disabled    Disabled    Software
   pTB->printf("Lowpass Type:  ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetFilterType(pFH, i, TRUE, LineBuf));
   pTB->printf("\r\n");

   // Lowpass (Hz):     n/a    n/a    n/a    n/a    n/a    n/a    n/a    100
   pTB->printf("Lowpass (Hz):  ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetLowpassFilterValue(pFH, i, LineBuf));
   pTB->printf("\r\n");

   // Highpass Type:     Software    CyberAmp    Disabled    Disabled    Disabled    Disabled    Disabled    Software
   pTB->printf("Highpass Type: ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetFilterType(pFH, i, FALSE, LineBuf));
   pTB->printf("\r\n");

   // Highpass (Hz):    n/a    n/a    n/a    n/a    n/a    n/a    n/a    Bypass
   pTB->printf("Highpass (Hz): ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetHighpassFilterValue(pFH, i, LineBuf));
   pTB->printf("\r\n");

   // Gain:             n/a    n/a    n/a    n/a    n/a    n/a    n/a    x20,000
   pTB->printf("Gain:          ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetGainValue(pFH, i, LineBuf));
   pTB->printf("\r\n");

   // Offset:           n/a    n/a    n/a    n/a    n/a    n/a    n/a    12.23
   pTB->printf("Offset:        ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetOffsetValue(pFH, i, LineBuf));
   pTB->printf("\r\n");

   // Units:            n/a    n/a    n/a    n/a    n/a    n/a    n/a    ft-cndl
   pTB->printf("Units:         ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
   {
      LineBuf[0] = '\0';
      int nPhysicalChannel = pFH->nADCSamplingSeq[i];
      if (nPhysicalChannel >= 0)
         GetADCChannelUnits(pFH, nPhysicalChannel, LineBuf);
      pTB->printf("%-10.10s", LineBuf);
   }
   pTB->printf("\r\n");

   // Lowpass Type:     Software    CyberAmp    Disabled    Disabled    Disabled    Disabled    Disabled    Software
   pTB->printf("Lowpass Type:  ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetFilterType(pFH, i, TRUE, LineBuf));
   pTB->printf("\r\n");

   // Lowpass (Hz):     n/a    n/a    n/a    n/a    n/a    n/a    n/a    100
   pTB->printf("Lowpass (Hz):  ");
   for (i = nFirstChannel; i <= nLastChannel; i++)
      pTB->printf("%-10.10s", GetLowpassFilterValue(pFH, i, LineBuf));
   pTB->printf("\r\n");
}

//===============================================================================================
// FUNCTION: SignalConditioningInfo
// PURPOSE:  Describes the signal conditioner values for sampled channels.
//
static void SignalConditioningInfo(CTextBuffer *pTB, const ABFFileHeader *pFH)
{
   if (pFH->nSignalType == 0)
   {
      // No signal conditioner used.

      pTB->printf("No signal conditioner.\r\n" );
      return;
   }

   // Signal conditioner settings:

   pTB->printf("Signal conditioner settings:\r\n" );
   AddSignalCondValues (pTB, pFH, 0, 7);

   pTB->printf("\r\n");
   AddSignalCondValues (pTB, pFH, 8, 15);
}

//===============================================================================================
// FUNCTION: LongInfo
// PURPOSE:  Assembles the text for the INFO_LONG style of header formatting.
//
static void LongInfo(CTextBuffer *pTB, const ABFFileHeader *pFH)
{
   // If there is no data in the file, assume that it is a protocol and format the text in the future tense.
   BOOL bFutureTense = (pFH->lActualAcqLength==0);

   pTB->printf("\r\n");    // Blank line.
   EnvironmentalInfo(pTB, pFH);

   pTB->printf("\r\n" );   // Blank line
   if (bFutureTense)
      ProtocolHeirarchyInfo(pTB, pFH);
   else
      FileHeirarchyInfo(pTB, pFH);
   Triggering( pTB, pFH, bFutureTense );
   SampleIntervals( pTB, pFH );
   SamplingSequence( pTB, pFH );

   pTB->printf("\r\n" );   // Blank line
   DACHoldingInfo(pTB, pFH);

   if (pFH->nOperationMode == ABF_WAVEFORMFILE)
   {
      for( UINT i=0; i<ABF_WAVEFORMCOUNT; i++ )
         WaveformInfo(pTB, pFH, i);

      pTB->printf("\r\n" );   // Blank line

      if (DigitalPulseInfo(pTB, pFH))
         pTB->printf("\r\n");    // Blank line
      
      for( i=0; i<ABF_WAVEFORMCOUNT; i++ )
         ConditioningTrainInfo(pTB, pFH, i);

      pTB->printf("\r\n" );   // Blank line

      for( i=0; i<ABF_WAVEFORMCOUNT; i++ )
         LeakSubtractionInfo(pTB, pFH, i);

      pTB->printf("\r\n" );   // Blank line
      for( i=0; i<ABF_WAVEFORMCOUNT; i++ )
         UserListInfo(pTB, pFH, i);
   }

   if ((pFH->nOperationMode == ABF_WAVEFORMFILE) ||
       (pFH->nOperationMode == ABF_HIGHSPEEDOSC))
   {
      pTB->printf("\r\nStatistics measurements: %s.\r\n", pFH->nStatsEnable ? "enabled" : "disabled");
   }

   pTB->printf("\r\n" );   // Blank line
   MathChannelInfo(pTB, pFH);

   pTB->printf("\r\n" );   // Blank line
   SignalConditioningInfo(pTB, pFH);

   pTB->printf("\r\n");    // Blank line.
   AutoAnalyse(pTB, pFH);
}

//***********************************************************************************************
//***********************************************************************************************
//***
//***  VERBOSE INFORMATION.
//***
//***********************************************************************************************
//***********************************************************************************************

//===============================================================================================
// FUNCTION: VerboseInfo
// PURPOSE:  Assembles the text for the INFO_VERBOSE style of header formatting.
//
static void VerboseInfo(CTextBuffer *pTB, const ABFFileHeader *pFH)
{
   WPTRASSERT( pTB );
   ABFH_ASSERT( pFH );

   int i;
   char buf[100];
   pTB->printf("\r\n");    // Blank line.
   
   pTB->printf("\r\nlFileSignature =                %4.4s", &(pFH->lFileSignature));
   pTB->printf("\r\nnOperationMode =           %8d", pFH->nOperationMode);
   pTB->printf("\r\nfFileVersionNumber =       %8g", pFH->fFileVersionNumber);
   pTB->printf("\r\nlActualAcqLength =         %8d", pFH->lActualAcqLength);
      
   pTB->printf("\r\nnNumPointsIgnored =        %8d",  pFH->nNumPointsIgnored);
   pTB->printf("\r\nlActualEpisodes =          %8d", pFH->lActualEpisodes);
   pTB->printf("\r\nlFileStartDate =           %8d", pFH->lFileStartDate);
   pTB->printf("\r\nlFileStartTime =           %8d", pFH->lFileStartTime);
   pTB->printf("\r\nnFileStartMillisecs =      %8d",  pFH->nFileStartMillisecs);
   pTB->printf("\r\nlStopwatchTime =           %8d", pFH->lStopwatchTime);
   pTB->printf("\r\nfHeaderVersionNumber =     %8g", pFH->fHeaderVersionNumber);
   pTB->printf("\r\nnFileType =                %8d", pFH->nFileType);
   pTB->printf("\r\nnMSBinFormat =             %8d", pFH->nMSBinFormat);
      
   pTB->printf("\r\n\r\nlDataSectionPtr =          %8d", pFH->lDataSectionPtr);
   pTB->printf("\r\nlTagSectionPtr =           %8d", pFH->lTagSectionPtr);
   pTB->printf("\r\nlNumTagEntries =           %8d", pFH->lNumTagEntries);
   pTB->printf("\r\nlScopeConfigPtr =          %8d", pFH->lScopeConfigPtr);
   pTB->printf("\r\nlNumScopes =               %8d", pFH->lNumScopes);
   pTB->printf("\r\nlDACFilePtr[0] =           %8d", pFH->lDACFilePtr[0]);
   pTB->printf("\r\nlDACFilePtr[1] =           %8d", pFH->lDACFilePtr[1]);
   pTB->printf("\r\nlDACFileNumEpisodes[0] =   %8d", pFH->lDACFileNumEpisodes[0]);
   pTB->printf("\r\nlDACFileNumEpisodes[1] =   %8d", pFH->lDACFileNumEpisodes[1]);
   pTB->printf("\r\nlDeltaArrayPtr =           %8d", pFH->lDeltaArrayPtr);
   pTB->printf("\r\nlNumDeltas =               %8d", pFH->lNumDeltas);
   pTB->printf("\r\nlSynchArrayPtr =           %8d", pFH->lSynchArrayPtr);
   pTB->printf("\r\nlSynchArraySize =          %8d", pFH->lSynchArraySize);
   pTB->printf("\r\nnDataFormat =              %8d",  pFH->nDataFormat);
   pTB->printf("\r\nnSimultaneousScan =        %8d", pFH->nSimultaneousScan);
      
   pTB->printf("\r\n\r\nnADCNumChannels =          %8d",  pFH->nADCNumChannels);
   pTB->printf("\r\nfADCSampleInterval =       %8g",  pFH->fADCSampleInterval);
   pTB->printf("\r\nfADCSecondSampleInterval = %8g",  pFH->fADCSecondSampleInterval);
   pTB->printf("\r\nfSynchTimeUnit =           %8g",  pFH->fSynchTimeUnit);
   pTB->printf("\r\nfSecondsPerRun =           %8g",  pFH->fSecondsPerRun);
   pTB->printf("\r\nlNumSamplesPerEpisode =    %8d", pFH->lNumSamplesPerEpisode);
   pTB->printf("\r\nlPreTriggerSamples =       %8d", pFH->lPreTriggerSamples);
   pTB->printf("\r\nlEpisodesPerRun =          %8d", pFH->lEpisodesPerRun);
   pTB->printf("\r\nlRunsPerTrial =            %8d", pFH->lRunsPerTrial);
   pTB->printf("\r\nlNumberOfTrials =          %8d", pFH->lNumberOfTrials);
   pTB->printf("\r\nnAveragingMode =           %8d",  pFH->nAveragingMode);
   pTB->printf("\r\nlAverageCount =            %8d",  pFH->lAverageCount);
   pTB->printf("\r\nnUndoRunCount =            %8d",  pFH->nUndoRunCount);
   pTB->printf("\r\nnFirstEpisodeInRun =       %8d",  pFH->nFirstEpisodeInRun);
   pTB->printf("\r\nfTriggerThreshold =        %8g",  pFH->fTriggerThreshold);
   pTB->printf("\r\nnTriggerSource =           %8d",  pFH->nTriggerSource);
   pTB->printf("\r\nnTriggerAction =           %8d",  pFH->nTriggerAction);
   pTB->printf("\r\nnTriggerPolarity =         %8d",  pFH->nTriggerPolarity);
   pTB->printf("\r\nfScopeOutputInterval =     %8g",  pFH->fScopeOutputInterval);
   pTB->printf("\r\nfEpisodeStartToStart =     %8g",  pFH->fEpisodeStartToStart);
   pTB->printf("\r\nfRunStartToStart =         %8g",  pFH->fRunStartToStart);
   pTB->printf("\r\nfTrialStartToStart =       %8g",  pFH->fTrialStartToStart);
   pTB->printf("\r\nlClockChange =             %8d", pFH->lClockChange);
          
   pTB->printf("\r\n\r\nnDrawingStrategy =         %8d",  pFH->nDrawingStrategy);
   pTB->printf("\r\nnTiledDisplay =            %8d",  pFH->nTiledDisplay);
   pTB->printf("\r\nnEraseStrategy =           %8d",  pFH->nEraseStrategy);
   pTB->printf("\r\nnDataDisplayMode =         %8d",  pFH->nDataDisplayMode);
   pTB->printf("\r\nlDisplayAverageUpdate =    %8d", pFH->lDisplayAverageUpdate);
   pTB->printf("\r\nnChannelStatsStrategy =    %8d",  pFH->nChannelStatsStrategy);
   pTB->printf("\r\nlCalculationPeriod =       %8d", pFH->lCalculationPeriod);
   pTB->printf("\r\nlSamplesPerTrace =         %8d", pFH->lSamplesPerTrace);
   pTB->printf("\r\nlStartDisplayNum =         %8d", pFH->lStartDisplayNum);
   pTB->printf("\r\nlFinishDisplayNum =        %8d", pFH->lFinishDisplayNum);
   pTB->printf("\r\nnMultiColor =              %8d",  pFH->nMultiColor);
   pTB->printf("\r\nnShowPNRawData =           %8d",  pFH->nShowPNRawData);
   pTB->printf("\r\nfStatisticsPeriod =        %8g",  pFH->fStatisticsPeriod);
   pTB->printf("\r\nlStatisticsMeasurements =  %8lXh", pFH->lStatisticsMeasurements);
   pTB->printf("\r\nnStatisticsSaveStrategy =  %8d",  pFH->nStatisticsSaveStrategy);
   pTB->printf("\r\nnStatisticsClearStrategy = %8d",  pFH->nStatisticsClearStrategy);
          
   pTB->printf("\r\n\r\nfADCRange =                %8g",  pFH->fADCRange);
   pTB->printf("\r\nfDACRange =                %8g",  pFH->fDACRange);
   pTB->printf("\r\nlADCResolution =           %8d", pFH->lADCResolution);
   pTB->printf("\r\nlDACResolution =           %8d", pFH->lDACResolution);
      
   pTB->printf("\r\n\r\nnExperimentType =          %8d",pFH->nExperimentType);

   // Display the file GUID.
   char szGuid[64] = { 0 };
   AXU_GUIDToString( pFH->FileGUID, szGuid, ELEMENTS_IN( szGuid ) );
   pTB->printf( "\r\nFile GUID =              {%s}.", szGuid); 

   pTB->printf("\r\n\r\nnAutosampleEnable =        %8d", pFH->_nAutosampleEnable); 
   pTB->printf("\r\nnAutosampleInstrument =    %8d", pFH->_nAutosampleInstrument);
   pTB->printf("\r\nfAutosampleAdditGain =     %8g", pFH->_fAutosampleAdditGain);
   pTB->printf("\r\nfAutosampleFilter =        %8g", pFH->_fAutosampleFilter);
   pTB->printf("\r\nfAutosampleMembraneCap =   %8g", pFH->_fAutosampleMembraneCap);
   pTB->printf("\r\nnAutosampleADCNum =        %8d", pFH->_nAutosampleADCNum); 

   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("\r\nnTelegraphEnable[%d] =      %8d", uChan, pFH->nTelegraphEnable[uChan]);
      pTB->printf("\r\nnTelegraphMode[%d] =        %8d", uChan, pFH->nTelegraphMode[uChan]);
      pTB->printf("\r\nnTelegraphInstrument[%d] =  %8d", uChan, pFH->nTelegraphInstrument[uChan]);
      pTB->printf("\r\nfTelegraphAdditGain[%d] =   %8g", uChan, pFH->fTelegraphAdditGain[uChan]);
      pTB->printf("\r\nfTelegraphFilter[%d] =      %8g", uChan, pFH->fTelegraphFilter[uChan]);
      pTB->printf("\r\nfTelegraphMembraneCap[%d] = %8g", uChan, pFH->fTelegraphMembraneCap[uChan]);
   }
   pTB->printf("\r\n\r\nnCommentsEnable =          %8d",  pFH->nCommentsEnable);
   pTB->printf("\r\nnManualInfoStrategy =      %8d",  pFH->nManualInfoStrategy);
   pTB->printf("\r\nfCellID1 =                 %8g",  pFH->fCellID1);
   pTB->printf("\r\nfCellID2 =                 %8g",  pFH->fCellID2);
   pTB->printf("\r\nfCellID3 =                 %8g",  pFH->fCellID3);
   pTB->printf("\r\nnSignalType =              %8d",  pFH->nSignalType);
      
   pTB->printf("\r\n\r\nADC channel information:");
   pTB->printf("\r\nNo.  sADCChannelName  sADCUnits");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("\r\n%2d     %10.10s     %8.8s", uChan, pFH->sADCChannelName[uChan], pFH->sADCUnits[uChan]);
   }
   pTB->printf("\r\n\r\nNo.  fInstrumentOffset  fADCDisplayOffset  fSignalOffset");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("\r\n%2d     % 10.4f       % 10.4f       % 10.4f",
              uChan, pFH->fInstrumentOffset[uChan], pFH->fADCDisplayOffset[uChan],
              pFH->fSignalOffset[uChan]);
   }
   pTB->printf("\r\n\r\n    fInstrumentScaleFactor             fADCProgrammableGain");
   pTB->printf("\r\nNo.               fADCDisplayAmplification                   fSignalGain");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("\r\n%2d     % 10.4f       % 10.4f       % 10.4f        % 10.4f",
              uChan, pFH->fInstrumentScaleFactor[uChan], pFH->fADCDisplayAmplification[uChan],
              pFH->fADCProgrammableGain[uChan], pFH->fSignalGain[uChan]);
   }
   pTB->printf("\r\n\r\nNo.  fSignalLowpassFilter  fSignalHighpassFilter");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("\r\n%2d      % 12.4f           % 12.4f",
              uChan, pFH->fSignalLowpassFilter[uChan], pFH->fSignalHighpassFilter[uChan]);
   }
   pTB->printf("\r\n\r\nADC channel mapping:");
   pTB->printf("\r\nnADCChannel         ");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("%2d ", uChan);
   }
   pTB->printf("\r\nnADCPtoLChannelMap  ");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("%2d ", pFH->nADCPtoLChannelMap[uChan]);
   }
   pTB->printf("\r\nnADCSamplingSeq     ");
   for (i=0; i<pFH->nADCNumChannels; i++)
      pTB->printf("%2d ", pFH->nADCSamplingSeq[i]);
   pTB->printf("\r\n\r\nNo.  nPostProcessLowpassFilterType  fPostProcessLowpassFilter");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      char buf2[100] = {0};
      GetPostProcessLowpassFilterType(pFH, uChan, buf);
      GetPostProcessLowpassFilterValue(pFH, uChan, buf2);
      pTB->printf("\r\n%2d               % 12.12s              % 12.4s", uChan, buf, buf2);
   }
   pTB->printf("\r\n\r\nDAC channel information:");
   pTB->printf("\r\nNo. sDACChannelName  sDACChannelUnits  fDACScaleFactor  fDACHoldingLevel");
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      UINT uChan = pFH->nADCSamplingSeq[i];
      pTB->printf("\r\n%2d      %10.10s       %8.8s        % 10.4f        % 10.4f",
              uChan, pFH->sDACChannelName[uChan], pFH->sDACChannelUnits[uChan],
              pFH->fDACScaleFactor[uChan], pFH->fDACHoldingLevel[uChan]);
   }
   pTB->printf("\r\nnDigitalHolding =       %8d", pFH->nDigitalHolding);
      
   if (pFH->nOperationMode == ABF_WAVEFORMFILE)
   {
      pTB->printf("\r\n\r\nSynchronous timer outputs:");
      pTB->printf("\r\nnOUTEnable =            %8.8s", pFH->nOUTEnable ? "TRUE" : "FALSE");
      pTB->printf("\r\nnSampleNumberOUT1 =     %8d", pFH->nSampleNumberOUT1);
      pTB->printf("\r\nnSampleNumberOUT2 =     %8d", pFH->nSampleNumberOUT2);
      pTB->printf("\r\nnFirstEpisodeOUT =      %8d", pFH->nFirstEpisodeOUT);
      pTB->printf("\r\nnLastEpisodeOUT =       %8d", pFH->nLastEpisodeOUT);
      pTB->printf("\r\nnPulseSamplesOUT1 =     %8d", pFH->nPulseSamplesOUT1);
      pTB->printf("\r\nnPulseSamplesOUT2 =     %8d", pFH->nPulseSamplesOUT2);
      
      pTB->printf("\r\n\r\nEpoch Output Waveform and Pulses:");
      pTB->printf("\r\nnDigitalEnable =        %8.8s", pFH->nDigitalEnable ? "TRUE" : "FALSE");
      pTB->printf("\r\nnActiveDACChannel =     %8d", pFH->nActiveDACChannel);
      pTB->printf("\r\nnDigitalInterEpisode =  %8d", pFH->nDigitalInterEpisode);
      
      for( UINT j=0; j<ABF_WAVEFORMCOUNT; j++ )
      {
         pTB->printf("\r\n\r\nEpoch Output Waveform - Channel %d:", j);
         pTB->printf("\r\nnWaveformEnable[%d] =       %8d", j, pFH->nWaveformEnable[j]);
         pTB->printf("\r\nnWaveformSource[%d] =       %8d", j, pFH->nWaveformSource[j]);
         pTB->printf("\r\nnInterEpisodeLevel[%d] =    %8d", j, pFH->nInterEpisodeLevel[j]);
         
         // Make text version of the waveform table - with or without alternate digital pattern
         char TempBuf[80] = {0};
         char TempBuf2[80] = {0};
         if( pFH->nAlternateDigitalOutputState && (pFH->nDigitalEnable != 0) )  // with alternate dig pattern
         {
            pTB->printf("\r\n\r\n  nEpochType[%d]      fEpochLevelInc[%d]         nEpochDurationInc[%d]      nAlternateDigital[%d]        lEpochPulseWidth[%d]", j,j,j,j,j);
            pTB->printf("\r\nNo.      fEpochInitLevel[%d]     nEpochInitDuration[%d]         nDigitalValue[%d]       lEpochPulsePeriod[%d]", j,j,j,j);
            for (i=0; i<ABF_EPOCHCOUNT; i++)
            {
               FormatDigitalOuts(pFH->nDigitalValue[i], pFH->nDigitalTrainValue[i],TempBuf);
               FormatDigitalOuts(pFH->nAlternateDigitalValue[i], pFH->nAlternateDigitalTrainValue[i],TempBuf2);
               pTB->printf("\r\n%2d   %4s   % 9.3f  % 9.3f   %8d      %8d      %s       %s       %8d       %8d",
                  i, GetEpochType(pFH, j, i),
                  pFH->fEpochInitLevel[j][i], pFH->fEpochLevelInc[j][i],
                  pFH->lEpochInitDuration[j][i], pFH->lEpochDurationInc[j][i],
                  TempBuf, TempBuf2, pFH->lEpochPulsePeriod[j][i], pFH->lEpochPulseWidth[j][i]);
            }
         }
         // without alternate dig pattern
         else
         {
            pTB->printf("\r\n\r\n  nEpochType[%d]      fEpochLevelInc[%d]         nEpochDurationInc[%d]     lEpochPulsePeriod[%d]", j,j,j,j);
            pTB->printf("\r\nNo.      fEpochInitLevel[%d]     nEpochInitDuration[%d]          nDigitalValue[%d]          lEpochPulseWidth[%d]", j,j,j,j);
            for (i=0; i<ABF_EPOCHCOUNT; i++)
            {
               FormatDigitalOuts(pFH->nDigitalValue[i], pFH->nDigitalTrainValue[i],TempBuf);
               pTB->printf("\r\n%2d   %4s   % 9.3f  % 9.3f   %8d      %8d      %s       %8d       %8d",
                  i, GetEpochType(pFH, j, i),
                  pFH->fEpochInitLevel[j][i], pFH->fEpochLevelInc[j][i],
                  pFH->lEpochInitDuration[j][i], pFH->lEpochDurationInc[j][i],
                  TempBuf, pFH->lEpochPulsePeriod[j][i], pFH->lEpochPulseWidth[j][i]);
            }
         }

         pTB->printf("\r\n\r\nAnalog Output File Replay Waveform - Channel %d:", j);
         pTB->printf("\r\nfDACFileScale[%d] =         %8g", j, pFH->fDACFileScale[j]);
         pTB->printf("\r\nfDACFileOffset[%d] =        %8g", j, pFH->fDACFileOffset[j]);
         pTB->printf("\r\nlDACFileEpisodeNum[%d] =    %8d", j, pFH->lDACFileEpisodeNum[j]);
         pTB->printf("\r\nnDACFileADCNum[%d] =        %8d", j, pFH->nDACFileADCNum[j]);
         pTB->printf("\r\nsDACFilePath[%d] =          %84.84s", j, pFH->sDACFilePath[j]);
         pTB->printf("\r\n\r\nPre-sweep pulse train %d:", j);
         pTB->printf("\r\nnConditEnable[%d] =         %8.8s", j, pFH->nConditEnable[j] ? "TRUE" : "FALSE");
         pTB->printf("\r\nlConditNumPulses[%d] =      %8d", j, pFH->lConditNumPulses[j]);
         pTB->printf("\r\nfBaselineDuration[%d] =     %8g", j, pFH->fBaselineDuration[j]);
         pTB->printf("\r\nfBaselineLevel[%d] =        %8g", j, pFH->fBaselineLevel[j]);
         pTB->printf("\r\nfStepDuration[%d] =         %8g", j, pFH->fStepDuration[j]);
         pTB->printf("\r\nfStepLevel[%d] =            %8g", j, pFH->fStepLevel[j]);
         pTB->printf("\r\nfPostTrainPeriod[%d] =      %8g", j, pFH->fPostTrainPeriod[j]);
         pTB->printf("\r\nfPostTrainLevel[%d] =       %8g", j, pFH->fPostTrainLevel[j]);

         pTB->printf("\r\n\r\nVariable parameter listChannel %d:", j);
         pTB->printf("\r\nnULEnable[%d] =           %8.8s", j, pFH->nULEnable[j] ? "TRUE" : "FALSE");
         pTB->printf("\r\nnULParamToVary[%d] =          %8d", j, pFH->nULParamToVary[j]);
         pTB->printf("\r\nsULParamValueList[%d] =       %80.80s", j, pFH->sULParamValueList[j]);
         pTB->printf("\r\nnULRepeat[%d] =           %8.8s", j, pFH->nULRepeat[j] ? "TRUE" : "FALSE");
      }
      
   }
   
   if ((pFH->nOperationMode == ABF_WAVEFORMFILE) ||
       (pFH->nOperationMode == ABF_HIGHSPEEDOSC))
   {
      pTB->printf("\r\n\r\nStatistics: %8.8s", pFH->nStatsEnable ? "Enabled" : "Disabled");
      if( pFH->nStatsEnable ) 
      {
         // Applys to specific channels
         for (i=0; i<ABF_ADCCOUNT; i++)
         {
            UINT uBitMask = 0x01 << i;
            if( pFH->nStatsActiveChannels & uBitMask )
            {
               char TempBuf[ABF_ADCNAMELEN*2];
               GetADCChannelName(pFH, i, TempBuf);
               pTB->printf("\r\nStatistics signal:               %s.", TempBuf);
               pTB->printf("\r\nPolarity (%s) =           %8d", TempBuf, pFH->nStatsChannelPolarity[i]);
            }
         }

         // Applys to specific regions
         for (UINT uRegionID=0; uRegionID<ABF_STATS_REGIONS; uRegionID++)
         {
            UINT uBitMask = 0x01 << uRegionID;
            if( pFH->nStatsSearchRegionFlags & uBitMask )
            {
               pTB->printf("\r\nSearch Mode =                    %8d", pFH->nStatsSearchMode[uRegionID]);
               pTB->printf("\r\nRegion %d Start (samples) =       %8d", uRegionID, pFH->lStatsStart[uRegionID]+1);
               pTB->printf("\r\nRegion %d End (samples) =         %8d", uRegionID, pFH->lStatsEnd[uRegionID]+1);
               pTB->printf("\r\nRegion %d Measurements =         %8lXh", uRegionID, pFH->lStatsMeasurements[uRegionID]);
               pTB->printf("\r\nRegion %d RiseBottomPercentile =  %8d", uRegionID, pFH->nRiseBottomPercentile[uRegionID]);
               pTB->printf("\r\nRegion %d RiseTopPercentile =     %8d", uRegionID, pFH->nRiseTopPercentile[uRegionID]);
               pTB->printf("\r\nRegion %d DecayBottomPercentile = %8d", uRegionID, pFH->nDecayBottomPercentile[uRegionID]);
               pTB->printf("\r\nRegion %d DecayTopPercentile =    %8d", uRegionID, pFH->nDecayTopPercentile[uRegionID]);   
            }
         }

         // Applys to all regions and channels
         pTB->printf("\r\nSmoothing window =               %8d", pFH->nStatsSmoothing);
         pTB->printf("\r\nBaseline =                       %8d", pFH->nStatsBaseline);
         pTB->printf("\r\nBaseline Start (samples) =       %8d", pFH->lStatsBaselineStart+1);
         pTB->printf("\r\nBaseline End (samples) =         %8d", pFH->lStatsBaselineEnd+1);
      }
   }
   
   pTB->printf("\r\n\r\nChannel Arithmetic:");
   pTB->printf("\r\nnArithmeticEnable =     %8.8s", pFH->nArithmeticEnable ? "TRUE" : "FALSE");
   pTB->printf("\r\nnArithmeticExpression = %8d", pFH->nArithmeticExpression);
   pTB->printf("\r\nfArithmeticUpperLimit = %8g", pFH->fArithmeticUpperLimit);
   pTB->printf("\r\nfArithmeticLowerLimit = %8g", pFH->fArithmeticLowerLimit);
   pTB->printf("\r\nnArithmeticADCNumA =    %8d", pFH->nArithmeticADCNumA);
   pTB->printf("\r\nnArithmeticADCNumB =    %8d", pFH->nArithmeticADCNumB);
   pTB->printf("\r\nfArithmeticK1 =         %8g", pFH->fArithmeticK1);
   pTB->printf("\r\nfArithmeticK2 =         %8g", pFH->fArithmeticK2);
   pTB->printf("\r\nfArithmeticK3 =         %8g", pFH->fArithmeticK3);
   pTB->printf("\r\nfArithmeticK4 =         %8g", pFH->fArithmeticK4);
   pTB->printf("\r\nfArithmeticK5 =         %8g", pFH->fArithmeticK5);
   pTB->printf("\r\nfArithmeticK6 =         %8g", pFH->fArithmeticK6);
   pTB->printf("\r\nsArithmeticOperator =   %1.1s", pFH->sArithmeticOperator);
   pTB->printf("\r\nsArithmeticUnits =      %8.8s", pFH->sArithmeticUnits);
      
   if (pFH->nOperationMode == ABF_WAVEFORMFILE)
   {
      for( UINT j=0; j<ABF_WAVEFORMCOUNT; j++ )
      {
         pTB->printf("\r\n\r\nOn-line subtraction - Channel %d:", j);
         pTB->printf("\r\nnPNEnable[%d] =       %8.8s", j, pFH->nPNEnable[j] ? "TRUE" : "FALSE");
         pTB->printf("\r\nnPNPolarity[%d] =         %d", j, pFH->nPNPolarity[j]);
         pTB->printf("\r\nnPNPosition =            %d", j, pFH->nPNPosition);
         pTB->printf("\r\nnPNNumPulses =           %d", j, pFH->nPNNumPulses);
         
         pTB->printf("\r\nnPNADCSamplingSeq[%d] =", j);
         for( int i=0; i<pFH->nPNNumADCChannels[j]; i++ )
            pTB->printf("%4d", pFH->nPNADCSamplingSeq[j][i]);

         pTB->printf("\r\nfPNHoldingLevel[%d] =     %.1f", j, pFH->fPNHoldingLevel[j]);
         pTB->printf("\r\nfPNSettlingTime =        %.2f", j, pFH->fPNSettlingTime);
         pTB->printf("\r\nfPNInterpulse =          %.2f", j, pFH->fPNInterpulse);
         pTB->printf("\r\n");
      }
   }       

   pTB->printf("\r\n\r\nBells:                   0            1");
   pTB->printf("\r\nnBellEnable       %8.8s     %8.8s",
         pFH->nBellEnable[0] ? "TRUE" : "FALSE", pFH->nBellEnable[1] ? "TRUE" : "FALSE");
   pTB->printf("\r\nnBellLocation     %8d     %8d",
         pFH->nBellLocation[0], pFH->nBellLocation[1]);
   pTB->printf("\r\nnBellRepetitions  %8d     %8d",
         pFH->nBellRepetitions[0], pFH->nBellRepetitions[1]);

   pTB->printf("\r\nnLevelHysteresis =           %8d",  pFH->nLevelHysteresis);
   pTB->printf("\r\nlTimeHysteresis =            %8d",  pFH->lTimeHysteresis);
   pTB->printf("\r\nnAllowExternalTags =         %8d",  pFH->nAllowExternalTags);
   pTB->printf("\r\nnAverageAlgorithm =          %8d",  pFH->nAverageAlgorithm);
   pTB->printf("\r\nfAverageWeighting =          %8g",  pFH->fAverageWeighting);
   pTB->printf("\r\nnUndoPromptStrategy =        %8d",  pFH->nUndoPromptStrategy);
   pTB->printf("\r\nnTrialTriggerSource =        %8d",  pFH->nTrialTriggerSource);
   pTB->printf("\r\nnStatisticsDisplayStrategy = %8d",  pFH->nStatisticsDisplayStrategy);
   pTB->printf("\r\n");

   // LTP specific fields
   for( UINT j=0; j<ABF_WAVEFORMCOUNT; j++ )
   {
      pTB->printf("\r\nnLTPType[%d] =                %8d", j, pFH->nLTPType);
      pTB->printf("\r\nnLTPUsageOfDAC[%d] =          %8d", j, pFH->nLTPUsageOfDAC[j]);
      pTB->printf("\r\nnLTPPresynapticPulses[%d] =   %8d", j, pFH->nLTPPresynapticPulses[j]);
   }
   pTB->printf("\r\n");

   // Trigger enable for DD1323x
   pTB->printf("\r\nDD132xTriggerOut = %8d", pFH->nDD132xTriggerOut);
   pTB->printf("\r\n");

   // Epoch resistamce
   for( j=0; j<ABF_WAVEFORMCOUNT; j++ )
   {
      char TempBuf[ABF_ADCNAMELEN+1] = {0};
      strncpy(TempBuf, pFH->sEpochResistanceSignalName[j],sizeof(TempBuf));
      TempBuf[ABF_ADCNAMELEN] = 0; 
      pTB->printf("\r\nsEpochResistanceSignalName[%d] = %s", j, TempBuf);
      pTB->printf("\r\nnEpochResistanceState[%d]      = %d", j, pFH->nEpochResistanceState[j]);
   }
   pTB->printf("\r\n");

   // Alternating episodic settings
   pTB->printf("\r\nnAlternateDACOutputState     =  %d", pFH->nAlternateDACOutputState);
   pTB->printf("\r\nnAlternateDigitalOutputState =  %d", pFH->nAlternateDigitalOutputState);

   // Autoanalysis macro:
   pTB->printf("\r\nnAutoAnalysisEnable          =  %d", pFH->nAutoAnalyseEnable);
   
   char TempBuf[ABF_MACRONAMELEN+1] = {0};
   ABFU_GetABFString( TempBuf, ELEMENTS_IN(TempBuf), pFH->sAutoAnalysisMacroName, ABF_MACRONAMELEN );
   pTB->printf("\r\nnAutoAnalysisMacroName       =  %s", TempBuf );
   
   // Leave some blank space at the end.
   pTB->printf("\r\n");
}

//***********************************************************************************************
//***********************************************************************************************
//***
//***  EXPORTED API FUNCTIONS.
//***
//***********************************************************************************************
//***********************************************************************************************

//===============================================================================================
// FUNCTION:   INFO_GetInfo
// PURPOSE:    Fills the buffer (up to a maximum of uBufSize bytes) with a textual description 
//             of the header.
// PARAMETERS:
//    pFH         Pointer to an ABF file header to be described.
//    szDataFile  Data file name or descriptive text.
//    psBuffer    Buffer to put the formatted text.
//    uBufSize    Maximum amount of formatted text that will fit in psBuffer.
//
// RETURNS:  The number of characters written to the buffer.
//
UINT WINAPI INFO_GetInfo( const ABFFileHeader *pFH, LPCSTR szDataFile, int nDisplayMode, 
                          char *psBuffer, UINT uBufSize)
{
   CTextBuffer TB(psBuffer, uBufSize);

   // Take a copy of the passed in header to ensure it is 6k long.
   ABFFileHeader NewFH;
   ABFH_PromoteHeader( &NewFH, pFH );
   
   if( nDisplayMode == INFO_ANNOTATIONS )
   {
      Annotations(&TB, &NewFH, szDataFile);
   }
   else
   {
      // Always put the terse information in at the top.   
      TerseInfo(&TB, &NewFH, szDataFile);

      switch (nDisplayMode)
      {
         case INFO_LONG:
            LongInfo(&TB, &NewFH);
            break;
      
         case INFO_VERBOSE:
            VerboseInfo(&TB, &NewFH);
            break;

         default:
            ERRORMSG1("Unexpected display mode '%d'.", nDisplayMode);
         case INFO_TERSE:
            break;   // Nothing more to add.
      }
   }

   // Return the number of bytes written (plus a bit just to be sure...)
   return TB.GetBytesUsed() + 10;
}

//===============================================================================================
// FUNCTION: INFO_BufferSize
// PURPOSE:  Returns the size of the buffer that is required to hold the text generated by
//           INFO_GetInfo with the same parameters.
//
UINT WINAPI INFO_GetBufferSize( const ABFFileHeader *pFH, LPCSTR szDataFile, int nDisplayMode)
{
   return INFO_GetInfo( pFH, szDataFile, nDisplayMode, NULL, 0);
}

