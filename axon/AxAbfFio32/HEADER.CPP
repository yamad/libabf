//***********************************************************************************************
//
//    Copyright (c) 1993-2002  Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// FILE: HEADER.C   The main module for the program HEADER.EXE.
//
// This program displays the header of a AXON data file created by either
// FETCHEX, CLAMPEX, or AXOTAPE.  See the usage screen for further details.
//
// Either Microsoft C or Borland C++ is required.
// 
// Other files required: ABFFILES.C, ABFFILES.H,
//                       ABFHEADR.C, ABFHEADR.H
//                       ABFOLDNX.H, HEADER.MAK
//
// (main() entry point is located at the end of this file)

//-----------------------------------------------------------------------------
// Include files required for standard ANSI-C library functions.

#include "wincpp.hpp"
#include <sys\stat.h>   // stat
#include <stdio.h>      // fputc, stdout, fileno, fseek, puts, etc
#include <stdlib.h>     // _MAX_PATH, exit, strtol, strtod, _splitpath, etc
#include <string.h>     // strlen, strchr, strncpy
#include <ctype.h>      // isalpha
#include <sys\utime.h>  // utime
#include <conio.h>      // kbhit

// Include file required for ABFFILES structure definitions and prototypes

#include "abffiles.h"
#include "abfutil.h"
#include "msbincvt.h"
#include "abfinfo.h"
#include "cparam.h"
#include "cabfhdr.hpp"
#include "\AxonDev\Comp\common\ArrayPtr.hpp"
#include "\AxonDev\Comp\common\Version.hpp"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"

//-----------------------------------------------------------------------------
// Constant definitions.
#define REQESTEDEPISIZE    512
#define CLPXPARAMS         1
#define FTCXPARAMS         2
#define AXOSCOPEPARAMS     3
#define BASE_10            10
#define ERROR_MESSAGE_SIZE 80

// global variables set by command line switches
char    g_szExeName[_MAX_PATH];
UINT    g_uDetail             = INFO_TERSE;
BOOL    g_bShowWaveform       = FALSE;
UINT    g_uWaveformEpisodeNum = 1;
int     g_nWaveformChannelNum = -2;
BOOL    g_bPauseAtEndOfScreen = FALSE;

BOOL    g_bShowEpisode = FALSE;
UINT    g_uEpisodeNum  = 1;
int     g_nChannelNum  = -2;

UINT    g_uParameterFile;
BOOL    g_bCopyToABF  = FALSE;
BOOL    g_bSetParam   = FALSE;
UINT    g_uPatchType  = 0;

enum EDumpType
{
   eDumpNone, 
   eDumpSynchRaw, 
   eDumpSynchAdjusted, 
   eDumpDeltas,
   eDumpTags,
   eDumpScopes,
   eDumpDACFile,
   eDumpHeader
} g_eDumpType;

#define DONT_PLAY_VOICETAGS UINT(-1)
#define PLAY_ALL_VOICETAGS  UINT(-2)
UINT    g_uVoiceTag = DONT_PLAY_VOICETAGS;

int     g_nPatchVal        = 0;
float   g_fPatchVal        = 0.0F;
long    g_lPatchVal        = 0;
WORD    g_wPatchVal        = 0;
char    *g_pszPatchVal     = NULL;
long    g_lPatchPoint      = 0;
char    *g_pszParamName    = NULL;
char    *g_pszParamVal     = NULL;
int     g_nParamIndex      = 0;
BOOL    g_bShowIndexAsChar = FALSE;
BOOL    g_bFixDataParams   = FALSE;
BOOL    g_bForceToGapFree  = FALSE;

// Other global variables

char    g_szDataFile[_MAX_PATH];         // Name of current file being processed

#define REALPATCH   1
#define LONGPATCH   2
#define INTPATCH    3
#define WORDPATCH   4
#define STRINGPATCH 6

#define COPYTOABF_PROMPT    "\nThe following files will be converted to ABF V%.1f format:"
#define FIXHEADER_PROMPT    "\nThe following files will have their header corrected:"
#define FORCEGAPFREE_PROMPT "\nThe following files will be forced into Gap-free mode:"

//****************************************************************************
//

//==============================================================================================
// GLOBAL VARS:
//

// Number of lines drawn since pause called.
int       g_nLines      = 0;

// Number of rows in one screen.
int       g_nScreenRows = 20;

// Gap at end of screen to ensure all lines displayed.
const int c_nScreenGap  = 6;

// ESC key code
const int c_nEsc        = 27;

//==============================================================================================
// FUNCTION:   SetupScreenPause
// PURPOSE:    Sets up pause at end of screen functionality
// PARAMETERS:
//    none
//
static void SetupScreenPause( void )
{
   CONSOLE_SCREEN_BUFFER_INFO Info;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
   g_nScreenRows = Info.dwSize.Y - c_nScreenGap;
}

//==============================================================================================
// FUNCTION:   Pause_printf
// PURPOSE:    fprintf with pause at end of screen; terminates program if ESC hit
// PARAMETERS:
//    none
//
static void Pause_printf( char *fmt, ...)
{
   // Get the variable arguments.
   va_list args;
   va_start(args, fmt);

   // Output to screen
   vfprintf( stdout, fmt, args );

   g_nLines++;

   // Check for pause if needed
   if( g_bPauseAtEndOfScreen && g_nLines > g_nScreenRows )
   {
      g_nLines = 0;
      fprintf(stdout, "\n\nPress any key to continue ");
      
      // Wait for key pressed.
      while( !_kbhit() );
      
      // Throw the key away.
      int nKey = getch();
      if( nKey == c_nEsc ) 
         exit(0);

      fprintf( stdout, "\n\n\n" );
   }
}

//==============================================================================================
// FUNCTION:   ShowCopyright
// PURPOSE:    Displays the copyright message and program version information
// PARAMETERS:
//    none
//
static void ShowCopyright(void)
{
   LPSZASSERT(g_szExeName);
   CVersion Ver(g_szExeName);
   ASSERT(Ver.IsValid());

   char szExeName[_MAX_PATH];
   char szVersion[20];
   char szCopyright[128];
   char szDescription[128];

   Ver.GetString("OriginalFilename", szExeName, sizeof(szExeName));
   Ver.GetProductVersion(szVersion, sizeof(szVersion));
   Ver.GetString("LegalCopyright", szCopyright, sizeof(szCopyright));
   Ver.GetString("FileDescription", szDescription, sizeof(szDescription));

   Pause_printf( "%s Version %s -- %s\n%s\n", szExeName, szVersion, szDescription, szCopyright);   
}

//==============================================================================================
// FUNCTION:   ShowUsage
// PURPOSE:    Displays the command line options and exits program
// PARAMETERS:
//    none
//
static void ShowUsage(void)
{
   LPSZASSERT(g_szExeName);

   char szExeName[_MAX_PATH];
   {
      CVersion Ver(g_szExeName);
      ASSERT(Ver.IsValid());

      Ver.GetString("OriginalFilename", szExeName, sizeof(szExeName));
   }

   Pause_printf( "USAGE:  %s FILENAME[.dat] [filename[.dat] ...] <options>\n\n", szExeName);
   Pause_printf( "Legal options are:\n");
   Pause_printf( "  /?       - display this screen of usage information.\n");
   Pause_printf( "  /LT      - terse listing%s.\n", g_uDetail==INFO_TERSE ? " (default)" : "");
   Pause_printf( "  /LL      - long listing%s.\n",  g_uDetail==INFO_LONG ? " (default)" : "");
   Pause_printf( "  /LV      - verbose listing%s.\n", g_uDetail==INFO_VERBOSE ? " (default)" : "");
   Pause_printf( "  /S       - Show the value of parameter p.\n");
   Pause_printf( "             e.g.  header /SfDACHoldingLevel(0) *.DAT\n");
   Pause_printf( "             e.g.  header /SnADCSamplingSeq(7) FILENAME\n");
   Pause_printf( "             e.g.  header /SfEpochLevelInc(C) FILENAME.DAT\n");
   Pause_printf( "  /C[p=m]  - Copy (old) files to ABF %.1f format and optionally\n", ABF_CURRENTVERSION);
   Pause_printf( "             set the value of parameter p to the value m.\n");
   Pause_printf( "             (.BAK files are created with the original data intact)\n");
   Pause_printf( "             e.g.  header /C FILENAME\n");
   Pause_printf( "             e.g.  header /CfCellID1=1.0 FILENAME.DAT\n");
   Pause_printf( "             e.g.  header /CsADCUnits(2)=nA FILENAME\n");
   Pause_printf( "             e.g.  header /Cfepochlevelinc(c)=0.125 FILENAME.DAT\n");
   Pause_printf( "             e.g.  header \"/Csfilecomment=New file comment\" FILENAME.DAT\n");
   Pause_printf( "  /En[Cm]  - print episode (sweep) number n from channel number m.\n");
   Pause_printf( "  /We      - print waveform epoch durations.\n");
   Pause_printf( "  /Ws      - print output waveform in DAC user units.\n");
   Pause_printf( "  /Dr      - Show synch array in (r)aw format.\n");
   Pause_printf( "  /Da      - Show synch array (a)djusted for a %d byte data chunk read length.\n", REQESTEDEPISIZE);
   Pause_printf( "  /Dt      - Show tags.\n");
   Pause_printf( "  /Dd      - Show deltas.\n");
   Pause_printf( "  /Ds      - Show the scope configurations saved in the file.\n");
   Pause_printf( "  /Df      - Show the data stored in the DAC file section.\n");
   Pause_printf( "  /Do      - Show the byte offsets of all header parameters.\n");
   Pause_printf( "  /Vn      - Play voice tag n if present.\n");
   Pause_printf( "  /VA      - Play all voice tags in file.\n");
   Pause_printf( "  /F       - Fix the header to reflect the size of the data in the file.\n");
   Pause_printf( "  /G       - Force the data file to Gap-Free mode.\n");
   Pause_printf( "  /OnRm    - patches file offset n with real value m.\n");
   Pause_printf( "  /OnLm    - patches file offset n with long value m.\n");
   Pause_printf( "  /OnIm    - patches file offset n with two byte integer value m.\n");
   Pause_printf( "  /OnUm    - patches file offset n with two byte unsigned integer value m.\n");
   Pause_printf( "  /OnSs    - patches file offset n with string s.\n");
   Pause_printf( "  /P       - pause display at the end of the each screen.\n" );
   Pause_printf( "             if the string is to contain any spaces, the whole\n");
   Pause_printf( "             option must be enclosed in double quotes.\n");
   Pause_printf( "             e.g.  header \"/O310SNew file comment\" FILENAME.DAT\n");
   
   exit(-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ALL THE FOLLOWING Get... ROUTINES GET VALUES FROM THE COMMAND LINE ARGUMENTS AND SET 
// THE APPROPRIATE GLOBAL FLAGS
//
////////////////////////////////////////////////////////////////////////////////////////////////


//==============================================================================================
// FUNCTION:   GetDumpType
// PURPOSE:    Determine the display method (Raw or Adjusted) to use when showing dump of synch array
// PARAMETERS:
//    pszArg  - pointer to argument string
// RETURNS:
//    current argument string
//
static char *GetDumpType(char *pszArg)
{
   switch (toupper(*pszArg))
   {
      case 'R':
         g_eDumpType = eDumpSynchRaw;
         break;
      case 'A':
         g_eDumpType = eDumpSynchAdjusted;
         break;
      case 'D':
         g_eDumpType = eDumpDeltas;
         break;
      case 'S':
         g_eDumpType = eDumpScopes;
         break;
      case 'F':
         g_eDumpType = eDumpDACFile;
         break;
      case 'O':
         g_eDumpType = eDumpHeader;
         break;
      default:
         if (*pszArg=='\0')
            return pszArg;
         break;
   }
   return pszArg+1;
}

//==============================================================================================
// FUNCTION:   GetListingType
// PURPOSE:    Determines the level of information provided to the user (Terse / Verbose)
// PARAMETERS:
//    pszArg - pointer to the argument string
// RETURNS:
//    current argument string
//
static char *GetListingType(char *pszArg)
{
   g_uDetail = INFO_LONG;
      
   if (toupper(*(pszArg+1)) == 'T')
   {
      g_uDetail = INFO_TERSE;
      pszArg++;
   }
   else if (toupper(*(pszArg+1)) == 'V')
   {
      g_uDetail = INFO_VERBOSE;
      pszArg++;
   }
   return pszArg;
}

//==============================================================================================
// FUNCTION:   GetEpisodeNum
// PURPOSE:    Get the requested episode number
// PARAMETERS:
//    pszArg         - pointer to the argument string
//    puEpisodeNum - pointer to the Episode Number
//    pnChannelNum - pointer to the Channel Number
// RETURNS:
//    current argument string
//
static char *GetEpisodeNum(char *pszArg, UINT *puEpisodeNum, int *pnChannelNum)
{
   int nChannel = -2;
   UINT uEpisode = (UINT)strtol(++pszArg, &pszArg, BASE_10);
   if (uEpisode < 1)
      uEpisode = 1;

   if (toupper(*pszArg) == 'C')
   {
      nChannel = (int)strtol(++pszArg, &pszArg, BASE_10);
      if ((nChannel < -1) || (nChannel >= ABF_ADCCOUNT))
      {
         Pause_printf( "Invalid channel number given \"%d\".\n", nChannel);
         Pause_printf( "Channel numbers must be in the range [-1 ... %d]\n\n", ABF_ADCCOUNT-1);
         ShowUsage();
      }
   }
   *puEpisodeNum = uEpisode;
   *pnChannelNum = nChannel;
   return pszArg;
}

//==============================================================================================
// FUNCTION:   GetPatchVals
// PURPOSE:    
//    Gets the patch values from the command line
// PARAMETERS:
//    pszArg - pointer to the command line argument string 
// NOTES:
//    Sets the global variables ?PatchVal & uPatchType

static char *GetPatchVals(char *pszArg)
{
   g_lPatchPoint = strtol(++pszArg, &pszArg, BASE_10);  //  was strtoul
   switch (toupper(*pszArg))
   {
      case 'R':   g_uPatchType = REALPATCH;
                  g_fPatchVal = (float)strtod(++pszArg, &pszArg);
                  break;
      case 'L':   g_uPatchType = LONGPATCH;
                  g_lPatchVal = strtol(++pszArg, &pszArg, BASE_10);
                  break;
      case 'I':   g_uPatchType = INTPATCH;
                  g_nPatchVal = strtol(++pszArg, &pszArg, BASE_10);
                  break;
      case 'U':   g_uPatchType = WORDPATCH;
                  g_wPatchVal = (WORD)strtoul(++pszArg, &pszArg, BASE_10);
                  break;
      case 'S':   g_uPatchType = STRINGPATCH;
                  g_pszPatchVal = ++pszArg;
                  pszArg += strlen(pszArg);
                  break;
      default:    pszArg = "   ";   // pointing pszArg to spaces
                  break;          // will produce an error report
   }
   return pszArg;
}

//==============================================================================================
// FUNCTION:   SetParam
// PURPOSE:    Sets a parameter to a value
//             
// PARAMETERS:
//    pszArg      - pointer to the command line argument string
//    bAssignment - TRUE if parameter is to be assigned
//
//
static char *SetParam(char *pszArg, BOOL bAssignment)
{
   char *psz;

   g_bSetParam = TRUE;
   g_pszParamName = ++pszArg;
   pszArg += strlen(pszArg);

   if (bAssignment)
   {
      g_pszParamVal = strchr(g_pszParamName, '=');
      if (g_pszParamVal != NULL)
         *g_pszParamVal++ = '\0';
   }
   else
      g_pszParamVal = NULL;

   g_bShowIndexAsChar = FALSE;
   g_nParamIndex = 0;

   psz = strchr(g_pszParamName, '(');
   if (psz != NULL)
   {
      *psz++ = '\0';
      if (*psz >= 'A')
      {
         g_bShowIndexAsChar = FALSE;
         *(int *)psz = toupper(*(int *)psz);
         *psz -= ('A' - '0');
      }
      g_nParamIndex = (int)strtol(psz, &psz, BASE_10);
   }
   return pszArg;
}

//==============================================================================================
// FUNCTION:   GetVoiceTagNumber
// PURPOSE:    Gets the requested Voice Tag
//             
// PARAMETERS:
//    pszArg        - pointer to the command line argument string
//
char *GetVoiceTagNumber(char *pszArg)
{
   if (toupper(*(pszArg+1)) == 'A')
   {
      g_uVoiceTag = PLAY_ALL_VOICETAGS;
      pszArg++;
   }
   else
      g_uVoiceTag = (UINT)strtol(++pszArg, &pszArg, BASE_10);
   return pszArg;
}

//==============================================================================================
// FUNCTION:   CheckParameters
// PURPOSE:    Checks the command line arguments and assigns values o the globals
//             
// PARAMETERS:
//    nArgCount - count of number of command line arguments
//    ppArgVals - pointer to pointer of argument values
//
// RETURNS:
//    number of command line arguments remaining
//
//
static int CheckParameters(int nArgCount, char **ppArgVals)
{
   char *pszArg = NULL;
   int  i = 0;
   int  j = 0;

   // Search for the "/P" switch first, then search for all others.
   i = 0;
   int nOriginalArgCount = nArgCount;
   while (i < nArgCount)
   {
      // increment i only if argument switch
      if ((*ppArgVals[i] != '-') && (*ppArgVals[i] != '/'))
      {
         i++;
         continue;
      }
      
      for (pszArg=ppArgVals[i]+1; *pszArg!='\0'; pszArg++)
      {
         if( toupper(*pszArg) == 'P' )
         {
            // Set Pause mode.
            g_bPauseAtEndOfScreen = TRUE;
            SetupScreenPause();
            break;
         }

         if (*pszArg == '\0')
            break;
      }
      nArgCount--;                         // decrement argument count
      for (j=i; j<nArgCount; j++)
         ppArgVals[j] = ppArgVals[j+1];    // shift following arguments down one
   }

   // Now search for all other switches.
   i = 0;
   nArgCount = nOriginalArgCount;
   while (i < nArgCount)
   {
      // increment i only if argument switch
      if ((*ppArgVals[i] != '-') && (*ppArgVals[i] != '/'))
      {
         i++;
         continue;
      }

      
      for (pszArg=ppArgVals[i]+1; *pszArg!='\0'; pszArg++)
      {
         switch (toupper(*pszArg))
         {
            case 'D':
               pszArg = GetDumpType(++pszArg);
               break;

            case 'L':
               pszArg = GetListingType(pszArg);
               break;

            case 'E':
               g_bShowEpisode = TRUE;
               pszArg = GetEpisodeNum(pszArg, &g_uEpisodeNum, &g_nChannelNum);
               break;

            case 'W':
               g_bShowWaveform = TRUE;
               pszArg = GetEpisodeNum(pszArg, &g_uWaveformEpisodeNum, &g_nWaveformChannelNum);
               break;

            case 'F':
               g_bFixDataParams = TRUE;
               break;

            case 'G':
               g_bForceToGapFree = TRUE;
               break;

            case 'O':
               pszArg = GetPatchVals(pszArg);
               break;

            case 'C':
               g_bCopyToABF = TRUE;
               if (isalpha(*(pszArg+1)))
                  pszArg = SetParam(pszArg, TRUE);
               break;

            case 'P':
               // Do nothing here, because it was handled above.
               break;

            case 'S':
               pszArg = SetParam(pszArg, FALSE);
               break;

            case 'V':
               pszArg = GetVoiceTagNumber(pszArg);
               break;

            case '?':
               ShowUsage();
               break;

            default:
               Pause_printf( "Invalid option given \"%s\".\n\n", ppArgVals[i]);
               ShowUsage();
         }

         if (*pszArg == '\0')
            break;
      }
      nArgCount--;                         // decrement argument count
      for (j=i; j<nArgCount; j++)
         ppArgVals[j] = ppArgVals[j+1];    // shift following arguments down one
   }

   if (nArgCount < 1)
      ShowUsage();
      
   if (g_bCopyToABF)
      g_uDetail = INFO_TERSE;

   if ((nArgCount > 1) && (g_pszParamVal != NULL))
   {
      Pause_printf( "Parameter setting in multiple files is not allowed.\n");
      exit(-1);
   }
   
   return nArgCount;
}

//==============================================================================================
// FUNCTION:   ShowABFError
// PURPOSE:    Display ABF error message
//             
// PARAMETERS:
//    nErrorNum   - ABF error number
//    pszFileName - File name
//
static void ShowABFError(int nErrorNum, char *pszFileName)
{
   char szTxt[ERROR_MESSAGE_SIZE];
   ABF_BuildErrorText(nErrorNum, pszFileName, szTxt, ERROR_MESSAGE_SIZE);
   Pause_printf( "\nERROR: %s\n", szTxt);
}

//==============================================================================================
// FUNCTION:   ShowFileHeader
// PURPOSE:    Display the file header
//             
// PARAMETERS:
//    pFH         - File Header
//    pszDataFile - File Name
//
static void ShowFileHeader(ABFFileHeader *pFH, char *pszDataFile)
{
   UINT uBlockSize = INFO_GetBufferSize(pFH, pszDataFile, g_uDetail);
   CArrayPtr<char> psBuffer(uBlockSize);
   if (!psBuffer)
   {
      Pause_printf( "\nINSUFFICIENT MEMORY.\n");
      return;
   }
   INFO_GetInfo(pFH, pszDataFile, g_uDetail, psBuffer, uBlockSize);

   char *ps = strtok(psBuffer, "\r");
   while (ps != NULL)
   {
      Pause_printf( "%-79.79s", ps);
      ps = strtok(NULL, "\r");
   }
   Pause_printf( "\n");
}

//==============================================================================================
// FUNCTION:   ShowScopes
// PURPOSE:    Display the Scope settings
//             
// PARAMETERS:
//    nFile       - ABF file number
//    pFH         - File Header
//    pszDataFile - File Name
//
static void ShowScopes(int nFile, ABFFileHeader *pFH, char *pszFileName)
{
   #define MAXSCOPES 4
   #define RGBCOLORS 10
   static ABFScopeConfig Cfg[MAXSCOPES];
   int nError;
   if (!ABF_ReadScopeConfig(nFile, pFH, Cfg, MAXSCOPES, &nError))
   {
      ShowABFError(nError, pszFileName);
      return;
   }
   
   for (int i=0; i<pFH->lNumScopes; i++)
   {
      Pause_printf( "\nScope %d\n=======\n", i);
      Pause_printf( "dwFlags = 0x%lX\n", Cfg[i].dwFlags);
      Pause_printf( "rgbColors = 0x%lX", Cfg[i].rgbColor[0]);
      for (int j=1; j<RGBCOLORS; j++)
         Pause_printf( ", 0x%lX", Cfg[i].rgbColor[j]);
      Pause_printf( "\nfDisplayStart = %g\n", Cfg[i].fDisplayStart);
      Pause_printf( "fDisplayEnd   = %g\n", Cfg[i].fDisplayEnd);

      Pause_printf( "bMaximized = %s\n", Cfg[i].bMaximized ? "TRUE" : "FALSE");
      Pause_printf( "bMinimized = %s\n", Cfg[i].bMinimized ? "TRUE" : "FALSE");
      Pause_printf( "xLeft      = %d\n", Cfg[i].xLeft);
      Pause_printf( "yTop       = %d\n", Cfg[i].yTop);
      Pause_printf( "xRight     = %d\n", Cfg[i].xRight);
      Pause_printf( "yBottom    = %d\n", Cfg[i].yBottom);
      Pause_printf( "LogFont:\n");
      Pause_printf( "   nHeight = %d\n", Cfg[i].LogFont.nHeight);
      Pause_printf( "   nWeight = %d\n", Cfg[i].LogFont.nWeight);
      Pause_printf( "   cPitchAndFamily = 0x%X\n", Cfg[i].LogFont.cPitchAndFamily);
      Pause_printf( "   szFaceName = %s\n", Cfg[i].LogFont.szFaceName);
      for (j=0; j<pFH->nADCNumChannels; j++)
      {
         Pause_printf( "Signal %d\n", j);
         Pause_printf( "   szName = %s\n", Cfg[i].TraceList[j].szName);
         Pause_printf( "   rgbColor = 0x%lX\n", Cfg[i].TraceList[j].rgbColor);
         Pause_printf( "   nPenWidth = %d\n", Cfg[i].TraceList[j].nPenWidth);
         Pause_printf( "   bDrawPoints = %s\n", Cfg[i].TraceList[j].bDrawPoints ? "TRUE" : "FALSE");
         Pause_printf( "   bHidden = %s\n", Cfg[i].TraceList[j].bHidden ? "TRUE" : "FALSE");
         Pause_printf( "   bFloatData = %s\n", Cfg[i].TraceList[j].bFloatData ? "TRUE" : "FALSE");
         Pause_printf( "   fVertProportion = %g\n", Cfg[i].TraceList[j].fVertProportion);
         Pause_printf( "   fDisplayGain = %g\n", Cfg[i].TraceList[j].fDisplayGain);
         Pause_printf( "   fDisplayOffset = %g\n", Cfg[i].TraceList[j].fDisplayOffset);
      }
   }
}

//==============================================================================================
// FUNCTION:   CheckFileName
// PURPOSE:    Checks that the passed data file name is valid & returns validated file name
//             
// PARAMETERS:
//    pszFileName - input file name
//    pszDataFile - validated File Name
// NOTES:
//    uses extension to determine source of file & sets uParameterFile
//
static void CheckFileName(char *pszFileName, char *pszDataFile)
{
   char szDrive[_MAX_DRIVE], szDir[_MAX_DIR],
        szFName[_MAX_FNAME], szExt[_MAX_EXT];

   _splitpath(pszFileName, szDrive, szDir, szFName, szExt);
   _makepath(pszDataFile, szDrive, szDir, szFName, (szExt[0] == '\0') ? "DAT" : szExt);

   if (strnicmp(szExt, ".bpa", 4)==0)
      g_uParameterFile = CLPXPARAMS;
   if (strnicmp(szExt, ".bpf", 4)==0)
      g_uParameterFile = FTCXPARAMS;
   if (strnicmp(szExt, ".pro", 4)==0)
      g_uParameterFile = AXOSCOPEPARAMS;
      
   // Disable various options that are meaningless for parameter files.
   if (g_uParameterFile)
   {
      if (g_eDumpType != eDumpScopes && g_eDumpType != eDumpHeader)
         g_eDumpType = eDumpNone;
   }
}

//==============================================================================================
// FUNCTION:   ShowEpochs
// PURPOSE:    Displays description of the epochs in the requested episode 
//             
// PARAMETERS:
//    pFH - File Header
//    uEpisode - requested episode
//
static void ShowEpochs(ABFFileHeader *pFH, UINT uEpisode)
{
   int i, j;
   char szBuf[20];
   UINT uEpochStart, uEpochEnd;

   if( (!pFH->nWaveformEnable[0] || (pFH->nWaveformSource[0] != ABF_EPOCHTABLEWAVEFORM)) && 
       (!pFH->nWaveformEnable[1] || (pFH->nWaveformSource[1] != ABF_EPOCHTABLEWAVEFORM)) && 
       !pFH->nDigitalEnable )
   {
      Pause_printf( "No epoch descriptions present.\n");
      return;
   }

   // Show the epoch durations

   Pause_printf( "Epoch durations, Episode %d:\nEpoch   ", uEpisode);
   for (i=0; i<pFH->nADCNumChannels; i++)
   {
      int ch = pFH->nADCSamplingSeq[i];
      ch = pFH->nADCPtoLChannelMap[ch];
      Pause_printf( " Channel %-2d  ", ch);
   }
   Pause_printf( "\nHolding ");
   for (j=0; j<pFH->nADCNumChannels; j++)
   {
      int ch = pFH->nADCSamplingSeq[j];
      if (!ABFH_GetEpochLimitsEx(pFH, ch, pFH->nActiveDACChannel, uEpisode, ABFH_FIRSTHOLDING, &uEpochStart, 
                               &uEpochEnd, NULL))
         break;
      sprintf(szBuf, "%d->%d  ", uEpochStart, uEpochEnd);
      Pause_printf( "%12.12s ", szBuf);
   }
   for (i=0; i<9; i++)
   {
      for (j=0; j<pFH->nADCNumChannels; j++)
      {
         int ch = pFH->nADCSamplingSeq[j];
         if (!ABFH_GetEpochLimitsEx(pFH, ch, pFH->nActiveDACChannel, 1, i, &uEpochStart, &uEpochEnd, NULL))
            break;
         if (j==0)
            Pause_printf( "\n %c      ", 'A'+i);
         sprintf(szBuf, "%d->%d  ", uEpochStart, uEpochEnd);
         Pause_printf( "%12.12s ", szBuf);
      }
   }
   Pause_printf( "\nHolding ");
   for (j=0; j<pFH->nADCNumChannels; j++)
   {
      int ch = pFH->nADCSamplingSeq[j];
      if (!ABFH_GetEpochLimitsEx(pFH, ch, pFH->nActiveDACChannel, 1, ABFH_LASTHOLDING, &uEpochStart, &uEpochEnd, NULL))
         break;
      sprintf(szBuf, "%d->%d  ", uEpochStart, uEpochEnd);
      Pause_printf( "%12.12s ", szBuf);
   }
   Pause_printf( "\n");
}

//==============================================================================================
// FUNCTION:   ShowWaveform
// PURPOSE:    Displays the requested waveform 
//             
// PARAMETERS:
//    pszFileName - File name
//    pFH         - File header
//    nChannel    - channel number
//
static void ShowWaveform(char *pszFileName, ABFFileHeader *pFH)
{
   if (pFH->nOperationMode != ABF_WAVEFORMFILE)
   {
      Pause_printf( "HEADER can only display waveforms from episodic stimulation files.\n");
      return;
   }
   
   // Show the epochs that make up the waveform.
   ShowEpochs(pFH, g_uWaveformEpisodeNum);
   
   // Open the file
   int     hFileHandle;
   UINT    uMaxSamples = 0;
   DWORD   dwMaxEpi = 0;
   int nError;
   if (!ABF_ReadOpen(pszFileName, &hFileHandle, ABF_DATAFILE, pFH, &uMaxSamples, 
                     &dwMaxEpi, &nError))
   {
      ShowABFError(nError, pszFileName);
      return;
   }
   
   UINT uPerChannel = 0;
   UINT uCounter    = 0;

   uPerChannel = (UINT)(pFH->lNumSamplesPerEpisode / pFH->nADCNumChannels);
   CArrayPtr<float> pfUUBuffer(uPerChannel);
   CArrayPtr<double> pdTimeBuffer(uPerChannel);
   if (!pfUUBuffer || !pdTimeBuffer)
   {
      Pause_printf( "Out of memory, free some system RAM and try again.\n\n");
      goto SW_Cleanup;
   }

   Pause_printf( "Episode %u, ", g_uEpisodeNum);

   if (!ABF_GetWaveformEx(hFileHandle, pFH, pFH->nActiveDACChannel, g_uWaveformEpisodeNum, 
                        pfUUBuffer, &nError))
   {
      ShowABFError(nError, pszFileName);
      goto SW_Cleanup;
   }

   ABFH_GetTimebaseEx(pFH, 0.0F, pdTimeBuffer, uPerChannel);

   Pause_printf( "          Time  Amplitude\n");
   for (uCounter=0; uCounter<uPerChannel; uCounter++)
   {
      Pause_printf( "%4d %10.5g %10.5g\n", uCounter+1, pdTimeBuffer[uCounter], pfUUBuffer[uCounter]);
   }
   Pause_printf( "\n");

SW_Cleanup:
   ABF_Close(hFileHandle, NULL);
}

//==============================================================================================
// FUNCTION:   ShowEpisode
// PURPOSE:    Displays the requested episode 
//             
// PARAMETERS:
//    pszFileName - File name
//    pFH         - File header
//    nChannel    - channel number
//
static void ShowEpisode(char *pszFileName, ABFFileHeader *pFH, int nChannel)
{
   int     hFileHandle;
   UINT    uMaxSamples = 0;
   DWORD   dwMaxEpi = 0;
   int     nPhysicalChannelNum, nError = 0;

   // Open the file
   if (!ABF_ReadOpen(pszFileName, &hFileHandle, ABF_DATAFILE, pFH, &uMaxSamples, 
                     &dwMaxEpi, &nError))
   {
      ShowABFError(nError, pszFileName);
      return;
   }
   
   double  dStartTime;
   UINT    i;
   UINT uPerChannel = (UINT)(pFH->lNumSamplesPerEpisode / pFH->nADCNumChannels);
   CArrayPtr<float> pfUUBuffer(uPerChannel);
   CArrayPtr<double> pdTimeBuffer(uPerChannel);
   if (!pfUUBuffer || !pdTimeBuffer)
   {
      Pause_printf( "Out of memory, free some system RAM and try again.\n\n");
      goto SE_Cleanup;
   }

   Pause_printf( "Episode %u, ", g_uEpisodeNum);

   if (nChannel < -1)
   {
      nPhysicalChannelNum = pFH->nADCSamplingSeq[0];
      nChannel = pFH->nADCPtoLChannelMap[nPhysicalChannelNum];
   }
   else if (nChannel > -1)
      nPhysicalChannelNum = pFH->nADCPtoLChannelMap[nChannel];
   else
      nPhysicalChannelNum = -1;

   if (nChannel == -1)
      Pause_printf( "Math Channel.\n");
   else if (nPhysicalChannelNum == nChannel)
      Pause_printf( "Channel %d.\n", nChannel);
   else if (nPhysicalChannelNum != -1)
      Pause_printf( "Logical Channel %d (Physical Channel %d).\n",
              nChannel, nPhysicalChannelNum);
   else
   {
      Pause_printf( "Channel %d was not sampled.\n", nChannel);
      goto SE_Cleanup;
   }
   
   if (!ABF_ReadChannel(hFileHandle, pFH, nPhysicalChannelNum, g_uEpisodeNum, pfUUBuffer, 
                        &uPerChannel, &nError))
   {
      ShowABFError(nError, pszFileName);
      goto SE_Cleanup;
   }

   if (!ABF_GetStartTime(hFileHandle, pFH, nPhysicalChannelNum, g_uEpisodeNum, &dStartTime, &nError))
   {
      ShowABFError(nError, pszFileName);
      goto SE_Cleanup;
   }

   ABFH_GetTimebaseEx(pFH, 0.0F, pdTimeBuffer, uPerChannel);

   Pause_printf( "\nStart time = %lg.\n", dStartTime);

   Pause_printf( "          Time  Amplitude\n");
   for (i=0; i<uPerChannel; i++)
      Pause_printf( "%4d %10.5g %10.5g\n", i+1, pdTimeBuffer[i], pfUUBuffer[i]);
   Pause_printf( "\n");
   
SE_Cleanup:
   ABF_Close(hFileHandle, NULL);
}


//==============================================================================================
// FUNCTION:   DumpRawABFSynchArray
// PURPOSE:    Displays the ABF synch array without any processing 
//             
// PARAMETERS:
//    pszDataFile - File name
//    pFH         - File header
//
static void DumpRawABFSynchArray(char *pszDataFile, ABFFileHeader *pFH)
{
   UINT uBufferSize = max(pFH->lSynchArraySize, 10);   // why 10 ?
   CArrayPtr<ABFSynch> pSynchArray(uBufferSize);
   if (!pSynchArray)
   {
      Pause_printf( "Couldn't allocate memory for Synch Array.\n");
      return;
   }
   
   HANDLE hFile = CreateFile(pszDataFile, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
   {
      Pause_printf( "Couldn't open file '%s'.\n", pszDataFile);
      return;
   }
   
   long lSynchCountOffset = LPSTR(&(pFH->lSynchArraySize)) - LPSTR(pFH);
   long lRawSynchCount = 0;
   SetFilePointer(hFile, lSynchCountOffset, NULL, FILE_BEGIN);
   DWORD dwBytesRead;
   ReadFile(hFile, &lRawSynchCount, sizeof(lRawSynchCount), &dwBytesRead, NULL);
   
   if (SetFilePointer(hFile, pFH->lSynchArrayPtr * 512L, NULL, FILE_BEGIN) == -1) 
   {
      Pause_printf( "Couldn't seek to Synch Array.\n");
      CloseHandle(hFile);
      return;
   }

   Pause_printf( "Raw ABF 1.x Synch Array at block %ld, %ld entries long:\n",
           pFH->lSynchArrayPtr, lRawSynchCount);
           
   DWORD dwEntries = lRawSynchCount;
   DWORD dwCount = 0;
   Pause_printf( "           Offset      Length\n");
   while (dwEntries > 0)
   {
      UINT uNumToRead = (dwEntries < (DWORD)uBufferSize ? (UINT)dwEntries : uBufferSize);
      UINT uBytesToRead = uNumToRead * sizeof(ABFSynch);
      dwBytesRead = 0;
      if (!ReadFile(hFile, pSynchArray, uBytesToRead, &dwBytesRead, NULL))
         Pause_printf( "Error reading Synch Array.\n");
   
      for (UINT i=0; i<uNumToRead; i++)
         Pause_printf( "%5lu  %10lu  %10lu  (%9lX  %9lX)\n", ++dwCount,
            pSynchArray[i].lStart, pSynchArray[i].lLength,
            pSynchArray[i].lStart, pSynchArray[i].lLength);
      dwEntries -= uNumToRead;
   }
   CloseHandle(hFile);
}

//==============================================================================================
// FUNCTION:   DumpRawOldSynchArray
// PURPOSE:    Displays an "old" synch array without any processing 
//             
// PARAMETERS:
//    pszDataFile - File name
//    pFH         - File header
//
static void DumpRawOldSynchArray(char *pszDataFile, ABFFileHeader *pFH)
{
   #define MAXSYNCHSIZE 8000
   CArrayPtr<short> pSynchArray(MAXSYNCHSIZE * 2);
   if (!pSynchArray)
   {
      Pause_printf( "Couldn't allocate memory for Synch Array.\n");
      return;
   }
   
   HANDLE hFile = CreateFile(pszDataFile, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
   {
      Pause_printf( "Couldn't open file '%s'.\n", pszDataFile);
      return;
   }
   
   if (SetFilePointer(hFile, pFH->lSynchArrayPtr * 512L, NULL, FILE_BEGIN) == -1) 
   {
      Pause_printf( "Couldn't seek to Synch Array.\n");
      CloseHandle(hFile);
      return;
   }

   // Read the Synch array into the allocated buffer

   DWORD dwNumToRead, dwNumRead, i;
   if (pFH->lActualEpisodes > MAXSYNCHSIZE)
      dwNumToRead = (UINT)MAXSYNCHSIZE * 4;
   else
      dwNumToRead = (UINT)pFH->lActualEpisodes * 4;

   if (!ReadFile(hFile, pSynchArray, dwNumToRead, &dwNumRead, NULL))
      Pause_printf( "Error reading Synch Array.\n");

   Pause_printf( "Raw FETCHEX Synch Array at block %ld, %ld entries long:\n",
           pFH->lSynchArrayPtr, pFH->lActualEpisodes);
   Pause_printf( "\n           Count      Length\n");
   for (i=0; i<dwNumRead/4; i++)
      Pause_printf( "%3ld   %10d  %10d  (%9X  %9X)\n", i+1,
         pSynchArray[i*2], pSynchArray[i*2+1],
         pSynchArray[i*2], pSynchArray[i*2+1]);
   CloseHandle(hFile);
}

//==============================================================================================
// FUNCTION:   DumpSynchArray
// PURPOSE:    Displays information about the ABF synch array 
//             
// PARAMETERS:
//    nFile - ABF File number
//    pFH   - File header
//
static void DumpSynchArray(int nFile, ABFFileHeader *pFH)
{
   DWORD dwStart = 0;
   UINT  uLength = 0;
   int   nError = 0;
   DWORD dwMissingSynchCount = 0;

   Pause_printf( "Adjusted Synch Array has %ld entries:\n", pFH->lActualEpisodes);
   if (pFH->lActualEpisodes == 0L)
      return;

   Pause_printf( "          Offset      Length     Missing\n");
   for (DWORD i=1; i<=(DWORD)pFH->lActualEpisodes; i++)
   {
      if (!ABF_SynchCountFromEpisode(nFile, pFH, i, &dwStart, &nError))
      {
         ShowABFError(nError, g_szDataFile);
         return;
      }
      if (!ABF_GetNumSamples(nFile, pFH, i, &uLength, &nError))
      {
         ShowABFError(nError, g_szDataFile);
         return;
      }
      if (!ABF_GetMissingSynchCount(nFile, pFH, i, &dwMissingSynchCount, &nError))
      {
         ShowABFError(nError, g_szDataFile);
         return;
      }
      Pause_printf( "%3lu   %10lu  %10u  %10u\n", i, dwStart, uLength, dwMissingSynchCount);
   }
}

//==============================================================================================
// FUNCTION:   ShowTags
// PURPOSE:    Displays information about the tags in the file 
//             
// PARAMETERS:
//    pszDataFile - File name
//    pFH         - File header
//
static void ShowTags(char *pszDataFile, ABFFileHeader *pFH)
{
   int    nFile;
   int    nErrorNum   = 0;
   UINT   uMaxSamples = 0;
   DWORD  dwMaxEpi    = 0;
   
   if (!ABF_ReadOpen(pszDataFile, &nFile, ABF_DATAFILE, pFH, &uMaxSamples, &dwMaxEpi, &nErrorNum))
   {
      ShowABFError(nErrorNum, g_szDataFile);
      return;
   }
   
   if ((pFH->lTagSectionPtr <= 4) || (pFH->lNumTagEntries < 1))
   {
      ABF_Close(nFile, NULL);
      Pause_printf( "Data file does not contain any tags.\n");
      return;
   }
   
   ABFTag Tag;
   Pause_printf( " Tag #  Offset   Type    Text\n");
   for (DWORD i=0; i<(DWORD)pFH->lNumTagEntries; i++)
   {
      if (!ABF_ReadTags(nFile, pFH, i, &Tag, 1, &nErrorNum))
      {
         ABF_Close(nFile, NULL);
         ShowABFError(nErrorNum, g_szDataFile);
         return;
      }
      Pause_printf( "%5lu %8ld %8d %55.55s\n", i+1, Tag.lTagTime, Tag.nTagType, Tag.sComment);
   }
   ABF_Close(nFile, NULL);
}
   
//==============================================================================================
// FUNCTION:   ShowDACFileData
// PURPOSE:    Displays information about the deltas in the file 
//             
// PARAMETERS:
//    pszDataFile - File name
//    pFH         - File header
//
static void ShowDACFileData(char *pszDataFile, ABFFileHeader *pFH)
{
   int    nFile;
   int    nErrorNum   = 0;
   UINT   uMaxSamples = 0;
   DWORD  dwMaxEpi    = 0;
   
   if (!ABF_ReadOpen(pszDataFile, &nFile, ABF_DATAFILE, pFH, &uMaxSamples, &dwMaxEpi, &nErrorNum))
   {
      ShowABFError(nErrorNum, g_szDataFile);
      return;
   }
   
   if ((pFH->lDACFilePtr[0] <= 4) || (pFH->lDACFilePtr[1] <= 4) || 
       (pFH->lDACFileNumEpisodes[0] < 1) || (pFH->lDACFileNumEpisodes[1] < 1))
   {
      ABF_Close(nFile, NULL);
      Pause_printf( "Data file does not contain any DAC File data.\n");
      return;
   }
   
   CArrayPtr<DAC_VALUE> pnDACArray(pFH->lNumSamplesPerEpisode);
   if (!pnDACArray)
   {
      ABF_Close(nFile, NULL);
      Pause_printf( "Out of memory, free some system RAM and try again.\n\n");
      return;
   }

   for (DWORD i=0; i<(DWORD)pFH->lDACFileNumEpisodes; i++)
   {
      Pause_printf( "\nSweep %d\n", i+1);
      if (!ABF_ReadDACFileEpiEx(nFile, pFH, pnDACArray, pFH->nActiveDACChannel, i+1, &nErrorNum))
      {
         ABF_Close(nFile, NULL);
         ShowABFError(nErrorNum, g_szDataFile);
         return;
      }
      for (UINT j=0; j<UINT(pFH->lNumSamplesPerEpisode); j++)
         Pause_printf( "%d   %8d\n", j+1, pnDACArray[j]);
   }
   ABF_Close(nFile, NULL);
}

//==============================================================================================
// FUNCTION:   ShowDeltas
// PURPOSE:    Displays information about the deltas in the file 
//             
// PARAMETERS:
//    pszDataFile - File name
//    pFH         - File header
//
static void ShowDeltas(char *pszDataFile, ABFFileHeader *pFH)
{
   int    nFile;
   int    nErrorNum   = 0;
   UINT   uMaxSamples = 0;
   DWORD  dwMaxEpi    = 0;
   
   if (!ABF_ReadOpen(pszDataFile, &nFile, ABF_DATAFILE, pFH, &uMaxSamples, &dwMaxEpi, &nErrorNum))
   {
      ShowABFError(nErrorNum, g_szDataFile);
      return;
   }
   
   if ((pFH->lDeltaArrayPtr <= 4) || (pFH->lNumDeltas < 1))
   {
      ABF_Close(nFile, NULL);
      Pause_printf( "Data file does not contain any deltas.\n");
      return;
   }
   
   ABFDelta Delta;
   Pause_printf( " Delta #  Offset   Type            Value\n");
   for (DWORD i=0; i<(DWORD)pFH->lNumDeltas; i++)
   {
      if (!ABF_ReadDeltas(nFile, pFH, i, &Delta, 1, &nErrorNum))
      {
         ABF_Close(nFile, NULL);
         ShowABFError(nErrorNum, g_szDataFile);
         return;
      }
      Pause_printf( "%7lu %8ld   ", i+1, Delta.lDeltaTime);
      switch (Delta.lParameterID)
      {
         case ABF_DELTA_HOLDING0:
         case ABF_DELTA_HOLDING1:
         case ABF_DELTA_HOLDING2:
         case ABF_DELTA_HOLDING3:
            Pause_printf( "Holding #%u      %g\n", Delta.lParameterID-ABF_DELTA_HOLDING0, Delta.fNewParamValue);
            break;

         case ABF_DELTA_DIGITALOUTS:
            Pause_printf( "Digital OUTs    %04X\n", Delta.lNewParamValue);
            break;

         case ABF_DELTA_THRESHOLD:
            Pause_printf( "Threshold       %g\n", Delta.fNewParamValue);
            break;

         case ABF_DELTA_PRETRIGGER:
            Pause_printf( "Pre-trigger     %d\n", Delta.lNewParamValue);
            break;

         default:
            if ((Delta.lParameterID >= ABF_DELTA_AUTOSAMPLE_GAIN) && 
                (Delta.lParameterID < ABF_DELTA_AUTOSAMPLE_GAIN+ABF_ADCCOUNT))
            {
               Pause_printf( "Autosample gain %g", Delta.fNewParamValue);
               break;
            }
            Pause_printf( "Unknown delta type.\n");
            break;
      }
   }
   ABF_Close(nFile, NULL);
}
   
//==============================================================================================
// FUNCTION:   ProcessVoiceTags
// PURPOSE:    Plays the voice tags
//             
// PARAMETERS:
//    pszDataFile - File name
//    pFH         - File header
//
static void ProcessVoiceTags(char *pszDataFile, ABFFileHeader *pFH)
{
   int   nFile;
   int   nErrorNum   = 0;
   UINT  uMaxSamples = 0;
   DWORD dwMaxEpi    = 0;
   
   if (!ABF_ReadOpen(pszDataFile, &nFile, ABF_DATAFILE, pFH, &uMaxSamples, &dwMaxEpi, &nErrorNum))
   {
      ShowABFError(nErrorNum, pszDataFile);
      return;
   }
   
   if ((pFH->lVoiceTagPtr == 0) || (pFH->lVoiceTagEntries == 0))
   {
      ABF_Close(nFile, NULL);
      Pause_printf( "Data file does not contain any voice tags.\n");
      return;
   }

   if (g_uVoiceTag != PLAY_ALL_VOICETAGS)   
      ABF_PlayVoiceTag( nFile, pFH, g_uVoiceTag, &nErrorNum);
   else
   {
      for (UINT i=0; i< UINT(pFH->lVoiceTagEntries); i++)
         if (!ABF_PlayVoiceTag( nFile, pFH, i, &nErrorNum))
            break;
   }
   ABF_Close(nFile, NULL);

   if (nErrorNum)
      ShowABFError(nErrorNum, g_szDataFile);
}


//==============================================================================================
// FUNCTION:   SpinningWheel
// PURPOSE:    spinning wheel on screen to indicate that something is happening
//             
// PARAMETERS:
//    status - selector for symbol to display
//
void SpinningWheel(long status)          
{                                       
   char *symbols = "|/-\\ ";
   int i = ((status < 0) ? 4 : (int)(status % 4L));
   Pause_printf( "%c\b", symbols[i]);
}

//==============================================================================================
// FUNCTION:   SetModiferInfo
// PURPOSE:    Set the SetModiferInfo parameter in the ABF header
//             
// PARAMETERS:
//    pFH - ABF header
//
void SetModiferInfo( ABFFileHeader *pFH )
{
   CABFHeader FH(*pFH);
   FH.SetModifierInfo(HINSTANCE(NULL));
   pFH->nModifierMajorVersion  = FH.nModifierMajorVersion;
   pFH->nModifierMinorVersion  = FH.nModifierMinorVersion;
   pFH->nModifierBugfixVersion = FH.nModifierBugfixVersion;
   pFH->nModifierBuildVersion  = FH.nModifierBuildVersion;
   AXU_strncpyz(pFH->sModifierInfo, FH.sModifierInfo, ABF_CREATORINFOLEN);
}

//==============================================================================================
// FUNCTION:  CopyToABF
// PURPOSE:   Copy old file to ABF format
// PARAMETERS:
//    pszFileName - file name
// RETURNS:
//    TRUE  if copy successful
//    FALSE if copy unsuccesful
//
static BOOL CopyToABF(char *pszFileName)
{
   char   szBakName[_MAX_PATH], szTempName[_MAX_PATH];

   char   szDrive[_MAX_DRIVE], szDir[_MAX_DIR],
          szFName[_MAX_FNAME], szExt[_MAX_EXT];

   // Build a temporary file name.        
   _splitpath(pszFileName, szDrive, szDir, szFName, szExt);
   _makepath(szBakName, szDrive, szDir, szFName, "BAK");
   _makepath(szTempName, szDrive, szDir, "TEMPNAME", "TMP");

   // Open the source file
   int    nFileIn   = ABF_INVALID_HANDLE;
   int    nFileOut  = ABF_INVALID_HANDLE;
   int    nErrorNum = 0;
   static ABFFileHeader FI, FO;
   UINT   uMaxSamples = 0;
   DWORD  dwMaxEpi = 0;   
   void  *pvBuffer = NULL;
   DWORD  i = 0;
   UINT   uSizeInSamples, uSampleSize;

   UINT  uOpenFlag = (g_uParameterFile ? ABF_PARAMFILE : ABF_DATAFILE);
   if (!ABF_ReadOpen(pszFileName, &nFileIn, uOpenFlag, &FI, &uMaxSamples, &dwMaxEpi, &nErrorNum))
      goto ShowError;

   // copy the source file's parameters.
   FO = FI;

   SetModiferInfo( &FO );

   if (!ABF_WriteOpen(szTempName, &nFileOut, ABF_DATAFILE, &FO, &nErrorNum))
      goto ShowError;

   if (FI.lNumScopes && FI.lScopeConfigPtr)
   {
      static ABFScopeConfig Cfg[4];
      if (!ABF_ReadScopeConfig( nFileIn, &FI, Cfg, ELEMENTS_IN(Cfg), &nErrorNum))
         goto ShowError;
      if (!ABF_WriteScopeConfig( nFileOut, &FO, FI.lNumScopes, Cfg, &nErrorNum))
         goto ShowError;
   }

   if (FI.lStatisticsConfigPtr)
   {
      static ABFScopeConfig StatsCfg;
      if (!ABF_ReadStatisticsConfig( nFileIn, &FI, &StatsCfg, &nErrorNum))
         goto ShowError;
      if (!ABF_WriteStatisticsConfig( nFileOut, &FO, &StatsCfg, &nErrorNum))
         goto ShowError;
   }
                                        
   uSizeInSamples = uMaxSamples * (UINT)FI.nADCNumChannels;
   uSampleSize    = FI.nDataFormat==ABF_INTEGERDATA ? sizeof(short) : sizeof(float);

   pvBuffer = malloc(uSizeInSamples * uSampleSize);
   if (!pvBuffer)
   {
      nErrorNum = ABF_OUTOFMEMORY;
      goto ShowError;
   }

   for (i=1; i<=dwMaxEpi; i++)
   {
      SpinningWheel((long)i-1);

      if (!ABF_MultiplexRead(nFileIn, &FI, i, pvBuffer, &uSizeInSamples, &nErrorNum))
         goto ShowError;

      DWORD dwEpiStart = 0;
      ABF_SynchCountFromEpisode(nFileIn, &FI, i, &dwEpiStart, NULL);

      UINT uFlag = 0;
      if (FI.nOperationMode == ABF_VARLENEVENTS)
      {
         DWORD dwMissingSynchCount = 0;
         ABF_GetMissingSynchCount(nFileIn, &FI, i, &dwMissingSynchCount, NULL);
         if (dwMissingSynchCount == 0)
            uFlag = ABF_APPEND;
      }

      if (!ABF_MultiplexWrite(nFileOut, &FO, uFlag, pvBuffer, dwEpiStart, 
                              uSizeInSamples, &nErrorNum))
         goto ShowError;
   }

   ABFTag Tag;
   for (i=0; i<(DWORD)FI.lNumTagEntries; i++)
   {
      if (!ABF_ReadTags(nFileIn, &FI, i, &Tag, 1, &nErrorNum))
         goto ShowError;
      if (!ABF_WriteTag(nFileOut, &FO, &Tag, &nErrorNum))
         goto ShowError;

      if ( Tag.nTagType == ABF_VOICETAG )
      {
         char szWAVFile[_MAX_PATH];
         VERIFY(AXU_GetTempFileName("wav", 0, szWAVFile));

         // Extract the voice tag to the temp file and save it to the pending list
         ABFVoiceTagInfo VTI;
         if ( !ABF_GetVoiceTag( nFileIn, &FI, i, szWAVFile, 0, &VTI, &nErrorNum) ||
              !ABF_SaveVoiceTag( nFileOut, szWAVFile, 0, &VTI, &nErrorNum))
         {
            DeleteFile( szWAVFile );
            goto ShowError;
         }
         DeleteFile( szWAVFile );
      }
   }

   ABFDelta Delta;
   for (i=0; i<(DWORD)FI.lNumDeltas; i++)
   {
      if (!ABF_ReadDeltas(nFileIn, &FI, i, &Delta, 1, &nErrorNum))
         goto ShowError;
      if (!ABF_WriteDelta(nFileOut, &FO, &Delta, &nErrorNum))
         goto ShowError;
   }

   if( FI.nWaveformEnable[FI.nActiveDACChannel] && 
       (FI.nWaveformSource[FI.nActiveDACChannel] == ABF_DACFILEWAVEFORM) )
   {
      DAC_VALUE *pnWorkBuffer = (DAC_VALUE *)pvBuffer;

      // Go from the start of the selection to the end of the 
      // selection saving each episode.
      for ( i=1; i <= (DWORD)FI.lDACFileNumEpisodes; i++ )
      {
         if (!ABF_ReadDACFileEpiEx( nFileIn, &FI, pnWorkBuffer, FI.nActiveDACChannel, i, &nErrorNum ))
            goto ShowError;
   
         if (!ABF_WriteDACFileEpiEx( nFileOut, &FO, FO.nActiveDACChannel, pnWorkBuffer, &nErrorNum))
            goto ShowError;
      }
   }

   if (!ABF_UpdateHeader(nFileOut, &FO, &nErrorNum))
      goto ShowError;
      
   ABF_Close(nFileOut, NULL);
   nFileOut = ABF_INVALID_HANDLE;
   ABF_Close(nFileIn, NULL);
   nFileIn = ABF_INVALID_HANDLE;
   
   free(pvBuffer);

   remove(szBakName);
   rename(pszFileName, szBakName);
   rename(szTempName, pszFileName);

   SpinningWheel(-1);              // clear spinning wheel
   return TRUE;

ShowError:
   SpinningWheel(-1);              // clear spinning wheel
   remove(szTempName);
   if (pvBuffer)
      free(pvBuffer);
   if (nFileOut != ABF_INVALID_HANDLE)
      ABF_Close(nFileOut, NULL);
   if (nFileIn != ABF_INVALID_HANDLE)
      ABF_Close(nFileIn, NULL);

   ShowABFError(nErrorNum, g_szDataFile);
   return FALSE;
}

//==============================================================================================
// FUNCTION:   PatchFile
// PURPOSE:   Applies the patches the file
// PARAMETERS:
//    pszFileName  - file name
//    bMSBinFormat - TRUE if using MS Binary Format for float's / FALSE if using IEEE
//
static void PatchFile(char *pszDataFile, int bMSBinFormat)
{
   struct stat StatBuf = { 0 };
   stat(pszDataFile, &StatBuf);

   HANDLE hFile = CreateFile(pszDataFile, GENERIC_WRITE, 0, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
   {
      Pause_printf( "Couldn't open file '%s'.\n", pszDataFile);
      return;
   }

   if (SetFilePointer(hFile, g_lPatchPoint, NULL, FILE_BEGIN) == -1L)
      Pause_printf( "Offset %lu is out of range.\n", g_lPatchPoint);

   Pause_printf( "Patching offset %lu", g_lPatchPoint);
   DWORD dwBytesWritten;
   switch (g_uPatchType)
   {
      case REALPATCH:
         Pause_printf( " with %g as 4-byte real in %s format.\n",
                 g_fPatchVal, bMSBinFormat ? "Microsoft Binary" : "IEEE");
         if (bMSBinFormat)
            fIeeetoMSBin(&g_fPatchVal, &g_fPatchVal);
         WriteFile(hFile, &g_fPatchVal, sizeof(float), &dwBytesWritten, NULL);
         break;
      case LONGPATCH:
         Pause_printf( " with %lu as 4-byte long.\n", g_lPatchVal);
         WriteFile(hFile, &g_lPatchVal, sizeof(long), &dwBytesWritten, NULL);
         break;
      case INTPATCH:
         Pause_printf( " with %d as 2-byte signed integer.\n", g_nPatchVal);
         WriteFile(hFile, &g_nPatchVal, sizeof(int), &dwBytesWritten, NULL);
         break;
      case WORDPATCH:
         Pause_printf( " with %u as 2-byte unsigned integer.\n", g_wPatchVal);
         WriteFile(hFile, &g_wPatchVal, sizeof(WORD), &dwBytesWritten, NULL);
         break;
      case STRINGPATCH:
         Pause_printf( " with \"%s\" as a string.\n", g_pszPatchVal);
         WriteFile(hFile, g_pszPatchVal, strlen(g_pszPatchVal), &dwBytesWritten, NULL);
         break;
      // no default case since disallowed patch types are trapped in GetPatchVals
   }

   CloseHandle(hFile);

   struct utimbuf UTimeBuf = { 0 };
   UTimeBuf.modtime = StatBuf.st_mtime;
   UTimeBuf.actime  = StatBuf.st_mtime;
   VERIFY( utime(pszDataFile, &UTimeBuf)==0 );
}

//==============================================================================================
// FUNCTION:   FixDataParams
// PURPOSE:    Writes sensible values to the data section of the file
// PARAMETERS:
//    pszDataFile - file name
//
// NOTES:
//    There is a similar function in FixHeader.cpp (used by ABFInfo & Clampex) - any changes made
//    to this function MUST be duplicated in FixHeader !!!!

static void FixDataParams(char *pszDataFile)
{
   struct stat StatBuf = { 0 };
   stat(pszDataFile, &StatBuf);

   HANDLE hFile = CreateFile(pszDataFile, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
   {
      Pause_printf( "Couldn't open file '%s'.\n", pszDataFile);
      return;
   }

   ABFFileHeader FH; 
   DWORD dwBytesRead;
   if (!ReadFile(hFile, &FH, sizeof(FH), &dwBytesRead, NULL))
   {
      CloseHandle(hFile);
      Pause_printf( "Couldn't read header.\n");
      return;
   }

   if (FH.lFileSignature != ABF_NATIVESIGNATURE)
   {
      CloseHandle(hFile);
      Pause_printf( "File is not an ABF data file.\n");
      return;
   }

   if (int(FH.fFileVersionNumber) != int(ABF_CURRENTVERSION))
   {
      CloseHandle(hFile);
      Pause_printf( "File is pre ABF 1.0.\n");
      return;
   }

   if (FH.lDataSectionPtr <= 0)
   {
      CloseHandle(hFile);
      Pause_printf( "File does not have a data section.\n");
      return;
   }
   if (FH.lActualAcqLength > 0)
   {
      CloseHandle(hFile);
      Pause_printf( "lActualAcqLength has already been set to %d.\n", FH.lActualAcqLength);
      return;
   }

   UINT uActualFileSize  = GetFileSize(hFile, NULL);
   UINT uDataBytes       = uActualFileSize - FH.lDataSectionPtr * ABF_BLOCKSIZE;
   UINT uDataPoints      = uDataBytes / (FH.nDataFormat==ABF_INTEGERDATA ? sizeof(ADC_VALUE) : sizeof(float));

   // Ensure uDataPoints is an integral number of sampling sequences.
   uDataPoints          -= uDataPoints % FH.nADCNumChannels;
   FH.lActualAcqLength = long(uDataPoints);

   if ((FH.nOperationMode!=ABF_GAPFREEFILE) && (FH.nOperationMode!=ABF_WAVEFORMFILE))
      FH.nOperationMode = ABF_GAPFREEFILE;
   FH.lSynchArrayPtr     = 0;
   FH.lSynchArraySize    = 0;
   if (FH.nOperationMode==ABF_WAVEFORMFILE)
      FH.lActualEpisodes = FH.lActualAcqLength / FH.lNumSamplesPerEpisode;

   SetModiferInfo( &FH );

   SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
   DWORD dwBytesWritten;
   WriteFile(hFile, &FH, sizeof(FH), &dwBytesWritten, NULL);
   CloseHandle(hFile);

   struct utimbuf UTimeBuf = { 0 };
   UTimeBuf.modtime = StatBuf.st_mtime;
   UTimeBuf.actime  = StatBuf.st_mtime;
   VERIFY( utime(pszDataFile, &UTimeBuf)==0 );
}

//==============================================================================================
// FUNCTION:   ForceToGapFree
// PURPOSE:    Forces the data file to Gap-free mode
// PARAMETERS:
//    pszDataFile - file name
//
static void ForceToGapFree(char *pszDataFile)
{
   struct stat StatBuf = { 0 };
   stat(pszDataFile, &StatBuf);

   HANDLE hFile = CreateFile(pszDataFile, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
   {
      Pause_printf( "Couldn't open file '%s'.\n", pszDataFile);
      return;
   }

   ABFFileHeader FH; 
   DWORD dwBytesRead;
   if (!ReadFile(hFile, &FH, sizeof(FH), &dwBytesRead, NULL))
   {
      CloseHandle(hFile);
      Pause_printf( "Couldn't read header.\n");
      return;
   }

   if (FH.lFileSignature != ABF_NATIVESIGNATURE)
   {
      CloseHandle(hFile);
      Pause_printf( "File is not an ABF data file.\n");
      return;
   }

   if (int(FH.fFileVersionNumber) != int(ABF_CURRENTVERSION))
   {
      CloseHandle(hFile);
      Pause_printf( "File is pre ABF 1.0.\n");
      return;
   }

   if (FH.lDataSectionPtr <= 0)
   {
      CloseHandle(hFile);
      Pause_printf( "File does not have a data section.\n");
      return;
   }

   FH.nOperationMode  = ABF_GAPFREEFILE;
   FH.lSynchArrayPtr  = 0;
   FH.lSynchArraySize = 0;

   SetModiferInfo( &FH );

   SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
   DWORD dwBytesWritten;
   WriteFile(hFile, &FH, sizeof(FH), &dwBytesWritten, NULL);
   CloseHandle(hFile);

   struct utimbuf UTimeBuf = { 0 };
   UTimeBuf.modtime = StatBuf.st_mtime;
   UTimeBuf.actime  = StatBuf.st_mtime;
   VERIFY( utime(pszDataFile, &UTimeBuf)==0 );
}

//==============================================================================================
// FUNCTION:   ProcessInputFile
// PURPOSE:   Performs the actual processing on the data file
// PARAMETERS:
//    pszFileName  - file name
//
static void ProcessInputFile(char *pszFileName)
{
   CheckFileName(pszFileName, g_szDataFile);

   ABFFileHeader FH;

   int   nFile;
   int   nErrorNum;
   DWORD dwMaxEpi    = 0;
   UINT  uMaxSamples = 0;
   UINT  uOpenFlag   = (g_uParameterFile ? ABF_PARAMFILE : ABF_DATAFILE);
   
   if (!ABF_ReadOpen(g_szDataFile, &nFile, uOpenFlag, &FH, &uMaxSamples, &dwMaxEpi, &nErrorNum))
   {
      ShowABFError(nErrorNum, g_szDataFile);
      if (nErrorNum == ABF_EOPENFILE)
         return;
   }
   else
   {
      // Show the information contained in the ABF file header.
      ShowFileHeader(&FH, g_szDataFile);
      
      if (g_eDumpType == eDumpScopes)
         ShowScopes(nFile, &FH, g_szDataFile);
      
      // If the synch array is to be dumped as it is seen after an ABF open, do it now.
      if (g_eDumpType == eDumpSynchAdjusted)
         DumpSynchArray(nFile, &FH);
      
      // Close the ABF file.      
      ABF_Close(nFile, NULL);
      
      if (g_bCopyToABF && !CopyToABF(g_szDataFile))
         return;
      
      if (g_bShowWaveform)
         ShowWaveform(g_szDataFile, &FH);
      
      if (g_bShowEpisode)
         ShowEpisode(g_szDataFile, &FH, g_nChannelNum);
      
      if (g_eDumpType == eDumpTags)
         ShowTags(g_szDataFile, &FH);
      
      if (g_eDumpType == eDumpDeltas)
         ShowDeltas(g_szDataFile, &FH);
      
      if (g_eDumpType == eDumpDACFile)
         ShowDACFileData(g_szDataFile, &FH);
      
      if (g_uVoiceTag != UINT(-1))
         ProcessVoiceTags(g_szDataFile, &FH);
   }
   
   // If the raw synch entries are to be shown, do it now.
   if (g_eDumpType == eDumpSynchRaw)
   {
      if (FH.nFileType == ABF_ABFFILE)
         DumpRawABFSynchArray(g_szDataFile, &FH);
      else 
         DumpRawOldSynchArray(g_szDataFile, &FH);
   }

   if (g_eDumpType == eDumpHeader)
   {
      nErrorNum = CP_DumpHeaderOffsets(g_szDataFile);
      if (nErrorNum)
         CP_ShowError(nErrorNum);
   }

   if (g_uPatchType)
      PatchFile(g_szDataFile, FH.nMSBinFormat);

   if (g_bSetParam)
   {
      nErrorNum = CP_SetShowParam(g_szDataFile, g_pszParamName, g_pszParamVal,
                                  g_nParamIndex, g_bShowIndexAsChar);
      if (nErrorNum)
         CP_ShowError(nErrorNum);
   }

   if (g_bFixDataParams)
      FixDataParams(g_szDataFile);

   if (g_bForceToGapFree)
      ForceToGapFree(g_szDataFile);
}

//==============================================================================================
// FUNCTION:   PromptToContinue
// PURPOSE:   Prompt the user & wait for confirmation
// PARAMETERS:
//    nArgCount - number of arguments to display
//    ppArgVals - the argument values
//    pszPrompt - prompt message to display
//
static void PromptToContinue(int nArgCount, char **ppArgVals, char *pszPrompt)
{
   int i, ch;

   Pause_printf( pszPrompt, ABF_CURRENTVERSION);
   for (i=0; i<nArgCount; i++, ppArgVals++)
      Pause_printf( "\n%s", *ppArgVals);
      
   Pause_printf( "\n\nContinue [Y,N]? ");
   ch = getchar();
   if (toupper(ch) != 'Y')
      exit(1);
}

//-----------------------------------------------------------------------------
static void GetExeName(LPCSTR pszArgv0)
{
   char drive[_MAX_DRIVE];
   char dir[_MAX_DIR];
   char fname[_MAX_FNAME];

   _splitpath( pszArgv0, drive, dir, fname, NULL );
   _makepath( g_szExeName, drive, dir, fname, ".exe" );
}

void cdecl main(int nArgCount, char **ppszArgVals)
{

   GetExeName(*ppszArgVals);

   --nArgCount;
   ++ppszArgVals;

   ShowCopyright();

   nArgCount = CheckParameters(nArgCount, ppszArgVals);
   
   if (g_bCopyToABF)
      PromptToContinue(nArgCount, ppszArgVals, COPYTOABF_PROMPT);
   else if (g_bFixDataParams)
      PromptToContinue(nArgCount, ppszArgVals, FIXHEADER_PROMPT);
   else if (g_bForceToGapFree)
      PromptToContinue(nArgCount, ppszArgVals, FORCEGAPFREE_PROMPT);

   while (nArgCount--)
      ProcessInputFile(*ppszArgVals++);

   exit(0);
}

