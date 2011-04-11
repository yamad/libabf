/*****************************************************************************
**                                                                          **
**   Copyright (C) 1994 AXON Instruments Inc.                               **
**                                                                          **
**   This program is copyrighted and all modifications to this program      **
**   retain the original copyright. All rights reserved.                    **
**                                                                          **
*****************************************************************************/
//****************************************************************************
//
//    Module Name:   Validation.cpp
//    Description:   Code module to implement validation routines.
//
//****************************************************************************
#include "wincpp.hpp"
#include "AxonValidation.h"

#include <math.h>

//****************************************************************************
//
//    Method Name:   _MagnitudeDifferenceFloat(float fValA, float fValB)
//    Description:   If the difference between the number and the granularity is too
//                   great the the FPU will have problems processing it and return
//                   an 'float: invalid' error.  We want to avoid that, so we check that
//                   the difference in magnitude between the data and the granularity isn't
//                   greater than 1.0e6 (returned by _MagnitudeDifferenceXXX as 6.0)
//
//****************************************************************************
static float _MagnitudeDifferenceFloat(float fValA, float fValB)
{
   if (fValA == 0 && fValB == 0)
      return 0.0f;
   else if (fValA == 0)
      return (float)log10(fabs((double)fValB));
   else if (fValB == 0)
      return (float)log10(fabs((double)fValA));
   else
      return (float) (log10(fabs((double)fValA)) - log10(fabs((double)fValB)));
}

//****************************************************************************
//
//    Method Name:   _MagnitudeDifferenceDouble(double dValA, double dValB)
//    Description:   If the difference between the number and the granularity is too
//                   great the the FPU will have problems processing it and return
//                   an 'float: invalid' error.  We want to avoid that, so we check that
//                   the difference in magnitude between the data and the granularity isn't
//                   greater than 1.0e6 (returned by _MagnitudeDifferenceXXX as 6.0)
//
//****************************************************************************
static double _MagnitudeDifferenceDouble(double dValA, double dValB)
{
   if (dValA == 0 && dValB == 0)
      return 0;
   else if (dValA == 0)
      return log10(fabs(dValB));
   else if (dValB == 0)
      return log10(fabs(dValA));
   else
      return log10(fabs(dValA)) - log10(fabs(dValB));
}

//****************************************************************************
//
//    Method Name:   VAL_IntRange(int nDataToCheck, int nLowVal, int nHiVal)
//    Description:   Return TRUE if nDataToCheck is within the specified range.
//
//****************************************************************************
BOOL WINAPI VAL_IntRange(int nDataToCheck, int nLowVal, int nHiVal)
{ 
   ASSERT(nLowVal <= nHiVal);
   return (nDataToCheck <= nHiVal && nDataToCheck >= nLowVal);
}

//****************************************************************************
//
//    Method Name:   VAL_LongRange(long lDataToCheck, long lLowVal, long lHiVal)
//    Description:   Return TRUE if lDataToCheck is within the specified range.
//
//****************************************************************************
BOOL WINAPI VAL_LongRange(long lDataToCheck, long lLowVal, long lHiVal)
{
   ASSERT(lLowVal <= lHiVal);
   return (lDataToCheck <= lHiVal && lDataToCheck >= lLowVal);
}

//****************************************************************************
//
//    Method Name:   VAL_FloatRange(float fDataToCheck, float fLowVal, float fHiVal)
//    Description:   Return TRUE if fDataToCheck is within the specified range.
//
//****************************************************************************
BOOL WINAPI VAL_FloatRange(float fDataToCheck, float fLowVal, float fHiVal)
{                           
   ASSERT(fLowVal <= fHiVal);
   return (fDataToCheck <= fHiVal && fDataToCheck >= fLowVal);
}

//****************************************************************************
//
//    Method Name:   VAL_DoubleRange(double dDataToCheck, double dLowVal, double dHiVal)
//    Description:   Return TRUE if dDataToCheck is within the specified range.
//
//****************************************************************************
BOOL WINAPI VAL_DoubleRange(double dDataToCheck, double dLowVal, double dHiVal)
{
   ASSERT(dLowVal <= dHiVal);
   return (dDataToCheck <= dHiVal && dDataToCheck >= dLowVal);
}
 
//****************************************************************************
//
//    Method Name:   VAL_IntList(int nDataToCheck, int *pnIntList, int nElemInList)
//    Description:   Return TRUE if nDataToCheck is within the specified list.
//
//****************************************************************************
BOOL WINAPI VAL_IntList(int nDataToCheck, int *pnIntList, int nElemInList)
{  
   ASSERT(nElemInList > 0);     
   ARRAYASSERT(pnIntList, nElemInList);  
   
   for (int nCnt = 0; nCnt < nElemInList; nCnt++)
      if (nDataToCheck == *pnIntList++)
         return TRUE;
         
   return FALSE;         
}

//****************************************************************************
//
//    Method Name:   VAL_LongList(long lDataToCheck, long *plLongList, int nElemInList)
//    Description:   Return TRUE if lDataToCheck is within the specified list.
//
//****************************************************************************
BOOL WINAPI VAL_LongList(long lDataToCheck, long *plLongList, int nElemInList)
{       
   ASSERT(nElemInList > 0);     
   ARRAYASSERT(plLongList, nElemInList);  
   
   for (int nCnt = 0; nCnt < nElemInList; nCnt++)
      if (lDataToCheck == *plLongList++)
         return TRUE;
         
   return FALSE;         
}

//****************************************************************************
//
//    Method Name:  VAL_FloatList(float fDataToCheck, float *pfFloatList, int nElemInList)
//    Description:   Return TRUE if fDataToCheck is within the specified list.
//
//****************************************************************************
BOOL WINAPI VAL_FloatList(float fDataToCheck, float *pfFloatList, int nElemInList)
{ 
   ASSERT(nElemInList > 0);     
   ARRAYASSERT(pfFloatList, nElemInList);  
   
   for (int nCnt = 0; nCnt < nElemInList; nCnt++)
      if (fDataToCheck == *pfFloatList++)
         return TRUE;
         
   return FALSE;         
}

//****************************************************************************
//
//    Method Name:   VAL_DoubleList(double dDataToCheck, double *pdDoubleList, int nElemInList)
//    Description:   Return TRUE if dDataToCheck is within the specified list.
//
//****************************************************************************
BOOL WINAPI VAL_DoubleList(double dDataToCheck, double *pdDoubleList, int nElemInList)
{       
   ASSERT(nElemInList > 0);     
   ARRAYASSERT(pdDoubleList, nElemInList);  
   
   for (int nCnt = 0; nCnt < nElemInList; nCnt++)
      if (dDataToCheck == *pdDoubleList++)
         return TRUE;
         
   return FALSE;         
}

//****************************************************************************
//
//    Method Name:    VAL_CharList(...)
//    Description:   Return TRUE if lpszStrToCheck is contained in ppszStrList.
//
//****************************************************************************
BOOL WINAPI VAL_CharList(LPSTR lpszStrToCheck, char **ppszStrList, int nElemInList, BOOL bIsCaseSen)
{       
   RPTRASSERT(lpszStrToCheck);
   RPTRASSERT(ppszStrList);
   ASSERT(nElemInList > 0);     
   
   for (int nCnt = 0; nCnt < nElemInList; nCnt++)
   {
      if (!bIsCaseSen)
      {
         if (!lstrcmp(lpszStrToCheck, *(ppszStrList+nCnt)))
            return TRUE;
      }
      else
      {
         if (!lstrcmpi(lpszStrToCheck, *(ppszStrList+nCnt)))
            return TRUE;
      }
  }     
   return FALSE;         
}

//****************************************************************************
//
//    Method Name:   VAL_IntGranularity(int nDataToCheck, int nGranularity)
//    Description:   Check nDataToCheck against nGranularity and make it a 
//                   mutiple.  Return TRUE if no errors.
//
//****************************************************************************
BOOL WINAPI VAL_IntGranularity(int *pnDataToCheck, int nGranularity)
{
   ASSERT(nGranularity != 0);
   WPTRASSERT(pnDataToCheck); 
        
   if (nGranularity == 0)                                      // check for divide by zero
      return FALSE;                                            // abort

   BOOL     bDataNeg = ((*pnDataToCheck < 0) ^ (nGranularity < 0));
   double   dRemainder = fabs(((double) *pnDataToCheck) / nGranularity);
   long     lLeftOfDP  = (long) dRemainder;

   dRemainder -= lLeftOfDP;
   if (dRemainder)                                             // check if nDataToCheck is a multiple of 
   {                                                           // nGranularity
      if (dRemainder >= 0.5)                                   // see if we round up or down
         lLeftOfDP++;                                          // round up
         
      *pnDataToCheck = (int) (lLeftOfDP * abs(nGranularity));  // create granular number
      if (bDataNeg)                                            // check if number should be negative
         *pnDataToCheck *= -1;
   }

   return TRUE;                                                // no errors 
}

//****************************************************************************
//
//    Method Name:   VAL_LongGranularity(Long lDataToCheck, long lGranularity)
//    Description:   Check lDataToCheck against lGranularity and make it a 
//                   mutiple.  Return True if no errors.
//
//****************************************************************************
BOOL WINAPI VAL_LongGranularity(long *plDataToCheck, long lGranularity)
{
   ASSERT(lGranularity != 0);
   WPTRASSERT(plDataToCheck);      
     
   if (lGranularity == 0)                                      // check for divide by zero
      return FALSE;                                            // abort

   BOOL     bDataNeg = ((*plDataToCheck < 0L) ^ (lGranularity < 0L));
   double   dRemainder = fabs(((double) *plDataToCheck) / lGranularity);
   long     lLeftOfDP  = (long) dRemainder;

   dRemainder -= lLeftOfDP;
   if (dRemainder)                                             // check if lDataToCheck is a multiple of 
   {                                                           // lGranularity
      if (dRemainder >= 0.5)                                   // see if we round up or down
         lLeftOfDP++;                                          // round up
         
      *plDataToCheck = (lLeftOfDP * labs(lGranularity));       // create granular number
      if (bDataNeg)                                            // check if number should be negative
         *plDataToCheck *= -1L;
   }

   return TRUE;                                                // no error
}

//****************************************************************************
//
//    Method Name:   VAL_FloatGranularity(float fDataToCheck, float fGranularity)
//    Description:   Check fDataToCheck against fGranularity and make it a 
//                   mutiple.  Return corrected value.
//
//****************************************************************************
BOOL WINAPI VAL_FloatGranularity(float *pfDataToCheck, float fGranularity)
{                                                                  
   ASSERT(fGranularity != 0);
   WPTRASSERT(pfDataToCheck);      

   if (fGranularity == 0.0)                                    // protect against divide by zero
      return FALSE;

   // If the difference between the number and the granularity is too
   // great the the FPU will have problems processing it and return
   // an 'float: invalid' error.  We want to avoid that, so we check that
   // the difference in magnitude between the data and the granularity isn't
   // greater than 1.0e6 (returned by _MagnitudeDifferenceXXX as 6.0)
   if ( _MagnitudeDifferenceFloat((*pfDataToCheck), fGranularity) > 6.0 )
      return FALSE;

   BOOL     bDataNeg = ((*pfDataToCheck < 0.0f) ^ (fGranularity < 0.0f));
   double   dRemainder = fabs((double)(*pfDataToCheck / fGranularity));
   long     lLeftOfDP  = (long) dRemainder;
   
   dRemainder -= lLeftOfDP;
   if (dRemainder)                                             // check if fDataToCheck is a multiple of 
   {                                                           // fGranularity
      if (dRemainder >= 0.5)                                   // see if we round up or down
         lLeftOfDP++;                                          // round up
         
      *pfDataToCheck = (float)(lLeftOfDP * fabs(fGranularity));// create granular number
      if (bDataNeg)                                            // check if number should be negative
         *pfDataToCheck *= -1.0f;
   }

   return TRUE;                                                // return no errors
}

//****************************************************************************
//
//    Method Name:   VAL_DoubleGranularity(double *pdDataToCheck, double dGranularity)    
//    Description:   Check fDataToCheck against fGranularity and make it a 
//                   mutiple.  Return corrected value.
//
//****************************************************************************
BOOL WINAPI VAL_DoubleGranularity(double *pdDataToCheck, double dGranularity)    
{                                                                  
   ASSERT(dGranularity != 0);
   WPTRASSERT(pdDataToCheck);      
     
   if (dGranularity == 0.0)                                    // protect against divide by zero
      return FALSE;
      
   // If the difference between the number and the granularity is too
   // great the the FPU will have problems processing it and return
   // an 'float: invalid' error.  We want to avoid that, so we check that
   // the difference in magnitude between the data and the granularity isn't
   // greater than 1.0e6 (returned by _MagnitudeDifferenceXXX as 6.0)
   if ( _MagnitudeDifferenceDouble((*pdDataToCheck), dGranularity) > 6.0 )
      return FALSE;

   BOOL     bDataNeg = ((*pdDataToCheck < 0.0) ^ (dGranularity < 0.0));
   double   dRemainder = fabs((*pdDataToCheck / dGranularity));
   long     lLeftOfDP  = (long) dRemainder;
   
   dRemainder -= lLeftOfDP;
   if (dRemainder)                                             // check if dDataToCheck is a multiple of 
   {                                                           // dGranularity
      if (dRemainder >= 0.5)                                   // see if we round up or down
         lLeftOfDP++;                                          // round up
         
      *pdDataToCheck = (lLeftOfDP * fabs(dGranularity));       // create granular number
      if (bDataNeg)                                            // check if number should be negative
         *pdDataToCheck *= -1.0f;
   }
   
   return TRUE;                                                // return no errors
}

//****************************************************************************
//
//    Method Name:   VAL_IsStrNull(LPSTR lpszStr)
//    Description:   Returns TRUE if pointer is NULL or first element of 
//                   string is NULL.
//
//****************************************************************************
BOOL WINAPI VAL_IsStrNull(LPSTR lpszStr)
{
   RPTRASSERT(lpszStr);      
   
   if (lpszStr == NULL || *lpszStr == NULL)
      return TRUE;
      
   return FALSE;      
}

//****************************************************************************
//
//    Method Name:   VAL_IsStrValid(LPSTR lpszSource, LPSTR lpszCheckStr)
//    Description:   Returns TRUE if all characters in lpszSource are members
//                   of lpszCheckStr.
//
//****************************************************************************
BOOL WINAPI VAL_IsStrValid(LPSTR lpszSource, LPSTR lpszCheckStr)
{
   RPTRASSERT(lpszSource);
   RPTRASSERT(lpszCheckStr);   
   
   int nNumSrcChar = lstrlen(lpszSource);                      // find number of char's in string
   int nNumChkChar = lstrlen(lpszCheckStr);   
   
   for (int nSrcCnt = 0; nSrcCnt < nNumSrcChar; nSrcCnt++)
   { 
      BOOL bFlag = FALSE;
      for (int nChkCnt = 0; nChkCnt < nNumChkChar; nChkCnt++)  // check source char against all chars
      {                                                        // in chkstring
         if (*(lpszSource+nSrcCnt) == *(lpszCheckStr+nChkCnt)) // if valid char set validity flag and 
         {                                                     // check next character
            bFlag = TRUE;
            break;
         }
      }
      if (!bFlag)
         return FALSE;                                         // source char is not a member of the 
   }                                                           // check char array
   return TRUE;   
}                                                              // all characters are valid

//****************************************************************************
//
//    Method Name:   VAL_IsStrPrintable(LPSTR lpszSource)
//    Description:   Returns TRUE if all characters in string are printable.
//
//****************************************************************************
BOOL WINAPI VAL_IsStrPrintable(LPSTR lpszSource)
{ 
   RPTRASSERT(lpszSource);
   
   int nNumChar = lstrlen(lpszSource);
   
   for (int nCnt = 0; nCnt < nNumChar; nCnt++)
   {
      if (!isprint(*lpszSource++))
         return FALSE;
   }
   return TRUE;
}

//****************************************************************************
//
//    Method Name:   VAL_IsStrNumerical(LPSTR lpszSource)
//    Description:   Return TRUE if all elements of lpszSource contain digits
//                   between 0 - 9, ., +, -.
//
//****************************************************************************
BOOL WINAPI VAL_IsStrNumerical(LPSTR lpszSource)
{
   RPTRASSERT(lpszSource);
    
   int nNumChar = lstrlen(lpszSource);
   
   for (int nCnt = 0; nCnt < nNumChar; nCnt++)
   {   
      char  cTestChar = *lpszSource++;
      
      if (!isdigit(cTestChar) && !(cTestChar == '.' || cTestChar == '+' || 
                                   cTestChar == '-' || cTestChar == 'e' ||
                                   cTestChar == 'E'))
         return FALSE;
   }
   return TRUE;
}

//****************************************************************************
//
//    Method Name:   VAL_IsStrAlpha(LPSTR lpszSource)
//    Description:   Return TRUE if all elements of lpszSource contain digits
//                   between a - z or A - Z.
//
//****************************************************************************
BOOL WINAPI VAL_IsStrAlpha(LPSTR lpszSource)
{ 
   RPTRASSERT(lpszSource);
   
   int nNumChar = lstrlen(lpszSource);
   
   for (int nCnt = 0; nCnt < nNumChar; nCnt++)
   {   
      if (!isalpha(*lpszSource++))
         return FALSE;
   }
   return TRUE;
}

//****************************************************************************
//
//    Method Name:   VAL_IsLegalName( LPSTR lpszFilename )
//    Description:   Make sure this string will make a valid file/path name. 
//
//                   It is ok to use a full path name.
//
//****************************************************************************

static BOOL _HasLowValueCharacters( LPSTR lpszFilename )
{
   LPSZASSERT(lpszFilename);

   int nStrlen = strlen( lpszFilename );
   for( int nIndex = 0; nIndex < nStrlen; nIndex++ )
   {
      if ( lpszFilename[ nIndex ] < 31 )
         return TRUE;
   }
   return FALSE;
}

BOOL WINAPI VAL_IsLegalName( LPSTR lpszFilename )
{  
   LPSZASSERT(lpszFilename);

   // The MSDN tells us we can't have characters in a file name with values
   // between 0 and 31, so we will check for that.
   if ( _HasLowValueCharacters( lpszFilename ) )
      return FALSE;

   // Next we break up the name
   char szDrive[ _MAX_DRIVE ], szName[ _MAX_FNAME ], szDirectory[ _MAX_DIR ], szExtension[ _MAX_EXT ];
   _splitpath( lpszFilename, szDrive, szDirectory, szName, szExtension );

   // Make sure we have a file name
   if ( *szName == '\0' )
      return FALSE;

   // Next we make sure we have a valid drive name
   // with the format ('x:') where 'x' is defined as
   // between A-Z or a-z.
   if ( *szDrive != '\0' )
   {
      if ( isalpha( szDrive[ 0 ] ) == FALSE )
         return FALSE;
      if ( szDrive[ 1 ] != ':' )
         return FALSE;
   }

   // Next we search all of the other components for bad characters.
   // This list is basically from the Explorer. When you type in a bad
   // character Explorer gives you a list of the bad characters it's looking
   // for.
   const char c_szBadFileChars[] = "/<>\"?*:|";
   
   if ( strpbrk( szDirectory, c_szBadFileChars ) != NULL )
      return FALSE;

   if ( strpbrk( szName, c_szBadFileChars ) != NULL )
      return FALSE;

   if ( strpbrk( szExtension, c_szBadFileChars ) != NULL )
      return FALSE;

   return TRUE;
} 

