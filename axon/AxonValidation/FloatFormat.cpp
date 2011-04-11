/*****************************************************************************
**                                                                          **
**   Copyright (C) 2001 AXON Instruments Inc.                               **
**                                                                          **
**   This program is copyrighted and all modifications to this program      **
**   retain the original copyright. All rights reserved.                    **
**                                                                          **
*****************************************************************************/
//****************************************************************************
//
//    Module Name:   FloatFormat.cpp
//    Description:   Code module to implement floating point formatting.
//
//****************************************************************************  
#include "wincpp.hpp"
#include "AxonValidation.h"

#include <math.h>
#include <float.h>

//****************************************************************************  
// Static functions
//****************************************************************************  

static int   _EraseTrailingZeros(LPSTR lpszDigit, int nDp);
static int   _FmtStrUsingSN(LPSTR lpszDigit, UINT uiBufSize, int nDp, BOOL bNeg, UINT uiTotalSig, 
									 BOOL bFixed);
static int   _FmtString(LPSTR lpszSrc, UINT uiBufSize, int nDecPos, int nSign, 
                        UINT uiToRight, UINT uiTotalSig);
static void  _DetermineRight(double *pdNumber, UINT *puiRight, UINT *puiDecPos, UINT uiTotalSig);
static int   _EraseTrailingZeros(LPSTR lpszDigit, int nDp); 
static int   _LimitDbl(double *pDouble);
static int   _ConvertDblToStr(double dNumber, LPSTR lpszText, UINT uiBufSize, VAL_FMTTYPE FmtType, UINT uiSigToRight, 
                              int *pnError, UINT uiTotalSig, UINT uiDispSig);

//****************************************************************************  
// Constants
//****************************************************************************  
                                    
// define a conservative limit for the type float to hold.
// the real limit is 3.4E+-38                                    
const double   c_dMaxPosFloatVal  = 1E30;
const double   c_dMaxPosDoubleVal = 1E300;
       

const char     c_cSNChar = 'e';                                // character used to signify scientific notation      
const int      c_nMaxTempBufSize = 30;                         // temp buffer size, bif enough for any of our conversions


// use scientific notation if these limits are exceeded
const double   c_dUseSnoteHi = 1E7;
const double   c_dUseSnoteLow= 1E-3;
//****************************************************************************
//
//    Method Name:   VAL_FormatStrToFloat(...)
//    Description:   The purpose of this routine is to format and syncronize a numerical 
//                   string and it numerical representation.  It does this by taking a 
//                   numerical character string and converting it to a double. It then 
//                   converts the double back into a formatted character string.  
//                   The double is then cast into a float.
//
//    Arguments:     lpszSrc     - Contains numerical character string to be converted.
//                   uBufSize    - True size of lpszSrc.  String should be larger to allow for 
//                                 conversion expansion.
//                   pfConverted - This is where formatted number is stored after conversion.
//                   FmtType     - Indicate type 1 or 2 formatting
//                   uSigToRight - Number of digits to right of DP. Valid in type 1 only.
//                   uSNmaxDig   - Max number of display digits in scientific notation.
//                   pnError     - Error return variable, can be NULL.
//
//    Returns:       The number of characters in the conversioin buffer.
//
//****************************************************************************
int WINAPI VAL_FormatStrToFloat( LPSTR lpszSrc,
                                        UINT uBufSize,
                                        float *pfConverted,
                                        VAL_FMTTYPE FmtType,
                                        UINT uSigToRight,
                                        UINT uSNmaxDig,
                                        int *pnError )
{
   LPSZASSERT(lpszSrc);
   WPTRASSERT(pfConverted);
   ASSERT(uBufSize >= VAL_EXTERNBUFSIZE);
   
   double   dTmpNum=0.0;
   int      nConversion=0, nRetVal=0, nRetVal2=0, nRetVal3=0;
   

   nRetVal = VAL_ConvertStrToDouble(lpszSrc, &dTmpNum);                 // convert string to number
   if (nRetVal != VAL_FMT_NOERROR)                                      // if error, return 0 convertion
   {
      if (pnError != NULL)                                         
         *pnError = nRetVal;
         
      return 0;
   }
      
   nRetVal2 = VAL_LimitDblToFloat(&dTmpNum);                            // return error if overflow error

   // convert double to string                      
   nConversion = VAL_ConvertFltToStr((float) dTmpNum, lpszSrc, uBufSize, FmtType, uSigToRight, uSNmaxDig, &nRetVal3);
                 
   // convert formatted string back to a double
   VAL_ConvertStrToDouble(lpszSrc, &dTmpNum);  
   
   // cast double to a float
   *pfConverted = (float) dTmpNum;

   if (pnError != NULL)
   {
      if (nRetVal2 != VAL_FMT_NOERROR)
         *pnError = nRetVal2; 
      else
         *pnError = nRetVal3; 
   }

   return nConversion;
}

//****************************************************************************
//
//    Method Name:   VAL_FormatStrToDouble(...)
//    Description:   The purpose of this routine is to format and syncronize a numerical 
//                   string and it numerical representation.  It does this by taking a 
//                   numerical character string and converting it to a double. It then 
//                   converts the double back into a formatted character string.  
//
//    Arguments:     lpszSrc     - Contains numerical character string to be converted.
//                   uBufSize    - True size of lpszSrc.  String should be larger to allow for 
//                                 conversion expansion.
//                   pdConverted - This is where formatted number is stored after conversion.
//                   FmtType     - Indicate type 1 or 2 formatting
//                   uSigToRight - Number of digits to right of DP. Valid in type 1 only.
//                   uSNmaxDig   - Max number of display digits in scientific notation.
//                   pnError     - Error return variable, can be NULL.
//
//    Returns:       The number of characters in the conversioin buffer.
//
//****************************************************************************
int WINAPI VAL_FormatStrToDouble( LPSTR lpszSrc,
                                         UINT uBufSize,
                                         double *pdConverted,
                                         VAL_FMTTYPE FmtType, 
                                         UINT uSigToRight,
                                         UINT uSNmaxDig,
                                         int *pnError )
{  
   LPSZASSERT(lpszSrc);
   WPTRASSERT(pdConverted);
   ASSERT(uBufSize >= VAL_EXTERNBUFSIZE);
   
   double   dTmpNum=0.0;
   int      nConversion=0, nRetVal=0, nRetVal2=0, nRetVal3=0;   

   nRetVal = VAL_ConvertStrToDouble(lpszSrc, &dTmpNum);                // convert string to number
   if (nRetVal != VAL_FMT_NOERROR)                                         // if error, return 0 convertion
   {
      if (pnError != NULL)                                         
         *pnError = nRetVal;
         
      return 0;
   }
   
   nRetVal2 = _LimitDbl(&dTmpNum);
   
   // convert double to string                   
   nConversion = VAL_ConvertDblToStr(dTmpNum, lpszSrc, uBufSize, FmtType, uSigToRight, uSNmaxDig, &nRetVal3);
                                  
   // convert formatted string back to a double
   VAL_ConvertStrToDouble(lpszSrc, &dTmpNum);  
   
   // pass converted value
   *pdConverted = dTmpNum; 
 
    if (pnError != NULL)
   {
      if (nRetVal2 != VAL_FMT_NOERROR)
         *pnError = nRetVal2; 
      else
         *pnError = nRetVal3; 
   }

   return nConversion;
}

//****************************************************************************
//
//    Method Name:   VAL_ConvertStrToDouble(LPSTR lpszSrc, double *pDouble)
//    Description:   Convert a text string to double.  The text string may                  
//                   only contain '0 - 9', 'e', 'E', '.', '+' asnd '-'.  
//                   Any other charcaters in the string will cause this 
//                   function to return FALSE.
//
//    Arguments:     lpszSrc - Contains numerical character string to be converted.
//                   pDouble - This is where formatted number is stored after conversion.
//
//    Returns:       Returns one of the VAL_FMTER definitions.
//
//****************************************************************************   
int WINAPI VAL_ConvertStrToDouble( LPSTR lpszSrc,
                                          double *pDouble )
{
   LPSZASSERT(lpszSrc);
   WPTRASSERT(pDouble);     

   // check for bad string
   if (VAL_IsStrNull(lpszSrc))                      
      return VAL_FMT_BADSTR_ER;

   // all characters must be valid or return FALSE                                             
   if (!VAL_IsStrNumerical(lpszSrc))
      return VAL_FMT_BADSTR_ER;                 

   // atof() cannot handle more that 100 chars
   if (strlen(lpszSrc) > 100)
      return VAL_FMT_STR_CONV_ER;

   // do actual conversion   
   *pDouble = atof(lpszSrc);

   return VAL_FMT_NOERROR;
}   
   
//****************************************************************************
//
//    Method Name:   VAL_LimitDblToFloat(double *pDouble)
//    Description:   Limit the value that we will allow a type float to hold.
//
//    Arguments:     pDouble - The number to be check is passed in, if it exceeds
//                             the clamping value it will be replace by the 
//                             clamping value.
//
//    Returns:       Returns one of the VAL_FMTER definitions.
//
//****************************************************************************
int WINAPI VAL_LimitDblToFloat( double *pDouble )
{ 
   WPTRASSERT(pDouble);     
   
   // preserve signess
   BOOL  bIsNeg = (*pDouble < 0);
   
   // see if the value in the float exceeds our limitation on the number.  If
   // it does, replace it with the limit.
   if (fabs(*pDouble) > c_dMaxPosFloatVal)
   {
      *pDouble = c_dMaxPosFloatVal;
      
      if (bIsNeg)                                                  // preserve signess
         *pDouble *= -1.0;       
         
      return VAL_FMT_OVERFLOW_ER;         
   }
   
   return VAL_FMT_NOERROR;      
}

//****************************************************************************
//
//    Method Name:   _LimitDbl(double *pDouble)
//    Description:   Limit the value that we will allow a type float to hold.
//
//    Arguments:     pDouble - The number to be check is passed in, if it exceeds
//                             the clamping value it will be replace by the 
//                             clamping value.
//
//****************************************************************************
static int _LimitDbl( double *pDouble )
{ 
   WPTRASSERT(pDouble);     
    
   // preserve signess
   BOOL  bIsNeg = (*pDouble < 0);
   
   // see if the value in the float exceeds our limitation on the number.  If
   // it does, return overflow error.
   if (fabs(*pDouble) > c_dMaxPosDoubleVal)
   {
      *pDouble = c_dMaxPosDoubleVal;
      
      if (bIsNeg)                                                  // preserve signess
         *pDouble *= -1.0;       
         
      return VAL_FMT_OVERFLOW_ER;         
   }
   
   return VAL_FMT_NOERROR;      
}
//****************************************************************************
//
//    Method Name:   VAL_ConvertFltToStr(...)
//    Description:   Convert a float into a text string.
//
//    Arguments:     fNumber     - The number to conver is pass in this variable.
//                   lpszText    - The formatted text is pointed to by this variable.
//                   uBufSize   - The size of the buffer pointed to by lpszText(1 based).
//                   FmtType     - Indicate type 1 or 2 formatting
//                   uSigToRight- Number of digits to right of DP. Valid in type 1 only.
//                   uSNmaxDig  - Max number of significant digits displayed. Valid in type 2 only.
//                   pnError     - Error return variable, can be NULL.
//
//    return:        The number of digits in the conversion buffer.
//
//****************************************************************************                    
int WINAPI VAL_ConvertFltToStr( float fNumber,
                                       LPSTR lpszText,
                                       UINT uBufSize,
                                       VAL_FMTTYPE FmtType, 
                                       UINT uSigToRight,
                                       UINT uSNmaxDig,
                                       int *pnError )
{
   ARRAYASSERT(lpszText, uBufSize);
   ASSERT(uBufSize >= VAL_EXTERNBUFSIZE);

   BOOL bClamped = FALSE;                      
   // clamp to max float if out of range                      
   if ((uSNmaxDig < 1 || uSNmaxDig > FLT_DIG) && FmtType == VAL_FMT_SIG_DIGITS)
   {
      uSNmaxDig = FLT_DIG;
      bClamped = TRUE;
   }
   else if (FmtType == VAL_FMT_FIXED_DIGITS)
      uSNmaxDig = FLT_DIG;   
                      
   int nRetVal = _ConvertDblToStr((double)fNumber, lpszText, uBufSize, FmtType, uSigToRight, pnError, FLT_DIG, uSNmaxDig);
   
   // provide error information if needed and possible
   if (bClamped && (pnError != NULL))
   {
      if (*pnError == VAL_FMT_NOERROR)
         *pnError = VAL_FMT_SNCLAMP_ER;
   }

   return nRetVal;
}

//****************************************************************************
//
//    Method Name:   VAL_ConvertDblToStr(...)
//    Description:   Convert a double into a text string.
//
//    Arguments:     dNumber    - The number to conver is pass in this variable.
//                   lpszText   - The formatted text is pointed to by this variable.
//                   uBufSize  - The size of the buffer pointed to by lpszText(1 based).
//                   FmtType     - Indicate type 1 or 2 formatting
//                   uSigToRight- Number of digits to right of DP. Valid in type 1 only.
//                   uSNmaxDig  - Max number of significant digits displayed. Valid in type 2 only.
//                   pnError     - Error return variable, can be NULL.
//
//    return:        The number of digits in the conversion buffer.
//
//****************************************************************************                    
int WINAPI VAL_ConvertDblToStr( double dNumber,
                                       LPSTR lpszText,
                                       UINT uBufSize,
                                       VAL_FMTTYPE FmtType, 
                                       UINT uSigToRight,
                                       UINT uSNmaxDig,
                                       int *pnError )
{
   ARRAYASSERT(lpszText, uBufSize);
   ASSERT(uBufSize >= VAL_EXTERNBUFSIZE);
                      
   BOOL bClamped = FALSE;                      
   // clamp to max double if out of range                      
   if ((uSNmaxDig < 1 || uSNmaxDig > DBL_DIG) && FmtType == VAL_FMT_SIG_DIGITS)
   {
      uSNmaxDig = DBL_DIG;
      bClamped = TRUE;
   }
   else if (FmtType == VAL_FMT_FIXED_DIGITS)
      uSNmaxDig = DBL_DIG;   
                      
   BOOL nRetVal = _ConvertDblToStr(dNumber, lpszText, uBufSize, FmtType, uSigToRight, pnError, DBL_DIG, uSNmaxDig);
                                
   // provide error information if needed and possible
   if (bClamped && (pnError != NULL))
   {
      if (*pnError == VAL_FMT_NOERROR)
         *pnError = VAL_FMT_SNCLAMP_ER;
   }

   return nRetVal;
}
                                                                                
//****************************************************************************
//
//    Method Name:   _ConvertDblToStr(...)
//    Description:   Convert a double into a text string.
//
//    Arguments:     dNumber    - The number to conver is pass in this variable.
//                   lpszText   - The formatted text is pointed to by this variable.
//                   uBufSize  - The size of the buffer pointed to by lpszText(1 based).
//                   FmtType     - Indicate type 1 or 2 formatting
//                   uSigToRight- Number of digits to right of DP. Valid in type 1 only.
//                   pnError     - Error return variable, can be NULL.
//                   uTotalSig  - total number of significant digits in a number.
//                   uDispSig   - Max number of significant digits displayed. Valid in type 2 only.
//
//    return:        The number of digits in the conversion buffer.
//
//****************************************************************************                    
static int _ConvertDblToStr( double dNumber,
                             LPSTR lpszText,
                             UINT uBufSize,
                             VAL_FMTTYPE FmtType,
                             UINT uSigToRight, 
                             int *pnError,
                             UINT uTotalSig,
                             UINT uDispSig )
{  
   ASSERT(uBufSize >= VAL_EXTERNBUFSIZE);
   ARRAYASSERT(lpszText, uBufSize);
   
   if (pnError != NULL)
      *pnError = VAL_FMT_NOERROR;
                                                                              // we may add during formatting   
   char   szTmpBuf[c_nMaxTempBufSize] = "";
   char   *pszTmpBuf = szTmpBuf;
   int    nDecPos=0, nSign=0, nSigDig=0, nTemp=0;
   UINT   uToLeft=0;
   double dToLeft=0.0, dScale=0.0, dAbsNum = fabs(dNumber);
   BOOL   bFixed = (dAbsNum <= c_dUseSnoteHi && dAbsNum >= c_dUseSnoteLow) || dNumber == 0.0;

   // dAbsNum < 1e6 is a special case to force non-sn when you have full precision
   if (dAbsNum > c_dUseSnoteHi || FmtType == VAL_FMT_SIG_DIGITS)
   {
      pszTmpBuf = ecvt(dNumber, uDispSig, &nDecPos, &nSign);      
      _FmtStrUsingSN(pszTmpBuf, uBufSize, nDecPos, nSign, uTotalSig, bFixed);// convert to scientific notation      
   }   
   else
   {      
      // Calc # digits to left      
      dToLeft  = (dNumber == 0.0) ? -1.0 : log10(fabs(dNumber));      
      uToLeft = (dToLeft < 0.0) ? 0 : (int) dToLeft + 1;

      // used to force rounding                                 
      if (uToLeft == 0 && dNumber != 0.0)
         _DetermineRight(&dNumber, &uSigToRight, (UINT *) &nTemp, uTotalSig);
      else
         nTemp = uSigToRight;
         
         
      // Calc # significant digits
      nSigDig = uToLeft + nTemp;
      

      if (nSigDig > (int) (uTotalSig+1)) // we exceeded the bounds of a float
      {
         nSigDig = uTotalSig;         
         if (uToLeft > (uTotalSig+1)) // our number is too big to fit into float/double, so we need to truncate
         {
            // Scale number from 12345678 to 12345600, for example
            dScale  = pow(double(10.0),(double)(uToLeft-(uTotalSig+1)));
            dNumber = floor(dNumber / dScale + 0.5);
            dNumber = dNumber * dScale;
         }
      }
      
      pszTmpBuf = ecvt(dNumber, nSigDig, &nDecPos, &nSign);
      
      _FmtString(pszTmpBuf, uBufSize, nDecPos, nSign, uSigToRight, uTotalSig);
   }   

   strncpy(lpszText, pszTmpBuf, uBufSize-1);   
   *(lpszText+uBufSize-1) = '\0';   
   nTemp = lstrlen(lpszText);    
   if (pnError != NULL)
      *pnError = nTemp ? VAL_FMT_NOERROR : VAL_FMT_FORMAT_ER;
      
   return nTemp;
}
      
//****************************************************************************
//
//    Method Name:   _FmtString(...)
//    Description:   Format string using fixed point notaion.
//
//    Arguments:     lpszSrc     - String to be formatted is passed i and out through this 
//                                 pointer.
//                   uBufSize    - True size of buffer pointed to by lpszSrc.  Buffer
//                                 should have extra room for format expansion.
//                   nDecPos     - Decimal point position.
//                   nSign       - Specifies if the number is negative.
//                   uToRight    - Specifies the number of digits to the right of the decimal point.
//                   uTotalSig   - total number of significant digits in a number.
//                                                                              
//    return:        The number of digits in the conversion buffer.
//
//****************************************************************************
static int _FmtString( LPSTR lpszSrc,
                       UINT uBufSize,
                       int nDecPos,
                       int nSign,
                       UINT uToRight, 
                       UINT uTotalSig )
{   
   ASSERT(uBufSize >= VAL_EXTERNBUFSIZE);
   ARRAYASSERT(lpszSrc, uBufSize);
   
   char  szTemp[c_nMaxTempBufSize] = "";          
   LPSTR pszElemPtr = szTemp;
   int   nPos=0, nCnt=0;
   UINT  uJ=0;
   BOOL  bDp = TRUE;
   
   int nStrLen = (int)strlen(lpszSrc);      
   if (nSign != 0)                                          // negative number, print that - character
      *pszElemPtr++ = '-';                   
      
   if (nDecPos <= 0)                                        // decimal place is to the far left of the number (i.e.  .233)
   {
      *pszElemPtr++ = '0';                                  // so print a beginnin zero
      nDecPos = abs(nDecPos);
      if (uToRight != 0)
      {
         *pszElemPtr++ = '.';                               // so print a decimal point
         bDp = FALSE;
      }   
          
      if (uToRight < (UINT) nDecPos)
        nDecPos = (int) uToRight;
          
      for (nPos = 0; nPos < nDecPos; nPos++)
          *pszElemPtr++ = '0';
                       
      // no false significance                       
      if (nPos >= (int) uTotalSig)
         uToRight = 0;
                
      nDecPos = 0;       
                
      for (nCnt = 0; nStrLen-- && (nPos < (int) uToRight); nPos++, nCnt++)
         *pszElemPtr++ = lpszSrc[nCnt];
   }
   else
   {
      for (nPos=0; nPos < nStrLen; nPos++)                  // go through a print the number
      {
         if (nDecPos == nPos && bDp)                        // print the decimal place in the right place
            *pszElemPtr++ = '.';
   
         *pszElemPtr++ = lpszSrc[nPos];
      }     
   }  
      
   if (nPos < nDecPos)                                      // if we have yet to reach the decimal point, we need to pad with zeros
   {
      for (; nPos < nDecPos; nPos++)                        // pad out whole number with zeros (i.e. 13450000000)
          *pszElemPtr++ = '0';   
   }  
      
   if (nPos-nDecPos < (int) uToRight)                      // if we haven't yet displayed all the decimal digits, we need to pad (i.e. 3.4000000)                                   
   {     
      if (nPos == nDecPos && bDp)                           // print decimal point since we will be printing trailing zeros and we yet to print a .
         *pszElemPtr++ = '.';
         
      for (uJ=0; uJ < uToRight-(nPos-nDecPos); uJ++)    // we have to pad out the number to the right of the decimal place with zeros
         *pszElemPtr++ = '0';
   }
            
   *pszElemPtr = '\0';                                      // terminate
   strncpy(lpszSrc, szTemp, uBufSize-1); 
   lpszSrc[uBufSize-1] = '\0';
   return strlen(lpszSrc);
}   
      
//****************************************************************************
//
//    Method Name:   _FmtStrUsingSN(...)
//    Description:   Format string using scientific notation.
//
//    Arguments:     lpszSrc     - String to be formatted is passed i and out through this 
//                                 pointer.
//                   uBufSize    - True size of buffer pointed to by lpszSrc.  Buffer
//                                 should have extra room for format expansion.
//                   nDecPos     - Decimal point position.
//                   bNeg        - Specifies if the number is negative.
//                   uTotalSig   - total significant digits.
//                   bFixed      - If TRUE use fixed point notation.
//
//    return:        The number of digits in the conversion buffer.
//
//**************************************************************************** 
static int   _FmtStrUsingSN( LPSTR lpszSrc,
                             UINT uBufSize,
                             int nDecPos,
                             BOOL bNeg,
                             UINT uTotalSig, 
                             BOOL bFixed )
{   
   ARRAYASSERT(lpszSrc, uBufSize);
   
   char  szTemp[c_nMaxTempBufSize] = "";          
   LPSTR pszElemPtr = szTemp;
   int   nPos = 0, nCnt=0, nDecSav=0;
   BOOL  bSuppress = FALSE;

   _EraseTrailingZeros(lpszSrc, 0);      
   
   // if number would cause an exponent of 0, do straight format   
   if (bFixed && nDecPos > 0)
      return _FmtString(lpszSrc, uBufSize, nDecPos, (int) bNeg, 0, uTotalSig);     
   else if (bFixed && abs(nDecPos) >= 0)
      return _FmtString(lpszSrc, uBufSize, nDecPos, (int) bNeg, strlen(lpszSrc) + abs(nDecPos), uTotalSig);     
                          
   int nStrLen = (int)strlen(lpszSrc);      
   
   if (bNeg != 0)                                              // negative number, print that - character
      *pszElemPtr++ = '-';                   
   
   nDecSav = nDecPos;
   if (nDecPos <= 0)                                           // decimal place is to the far left of the number (i.e.  .233)
   {                                        
      nDecSav = nDecPos;
      nDecPos = abs(nDecPos);
      if (*lpszSrc == '\0')                                    //  the number is 0 supress the exxxx
         bSuppress = TRUE;                                 
   }
      
   if (*lpszSrc == '\0')                                       // print 1st digit
      *pszElemPtr++ = '0';
   else
      *pszElemPtr++ = lpszSrc[nPos++];

   if (nStrLen > nPos)                                         // after first digit comes the dp
   {
      *pszElemPtr++ = '.';      
      uTotalSig = nStrLen;
   }
   else
      uTotalSig = 1;
         
   for (nCnt = nPos; nCnt < nStrLen && lpszSrc[nPos]; nCnt++)  // xfer the amount of string needed
      *pszElemPtr++ = lpszSrc[nPos++];   

   for (; nCnt < (int) uTotalSig; nCnt++)                      // pad with zero's if nessasary
      *pszElemPtr++ = '0';   
              
   *pszElemPtr = '\0';                                         // terminate
   
   if (bSuppress == FALSE)
   {   
      *pszElemPtr++ = c_cSNChar;
      wsprintf(pszElemPtr, "%d", nDecSav-1);                   // append number of dp places        
   }
                                 
   strncpy(lpszSrc, szTemp, uBufSize-1); 
   lpszSrc[uBufSize-1] = '\0';
   return strlen(lpszSrc);
}

//****************************************************************************
//
//    Method Name:   _DetermineRight(...)
//    Description:   This routine works by looking the number of digits to the rightof the dp (ruRight) 
//                   asked for by the user and the number of zero's to the right of the dp 
//                   from nDecPos.  Since this routine is only used for numbers < 0, 
//                   nDecPos is always negative. By adding nDecPos and ruRight we get a remainder
//                   that follows a constant pattern: <= 0 = no rounding, all digits to the right
//                   of the dp will be 0.  ==1, check if we need to round, if we do, set ruDecPos=0,
//                   ruDecPos is used by ecvt() in the calling function.  else do not change anything.
//
//    Arguments:     pdNumber    The number to work on. 
//                   puRight     Number of places to the right of the dp.
//                   puDecPos    Number of dp for _ecvt().
//                   uTotalSig   The number of significant digits in the number.
//
//    Return:        None.
//
//****************************************************************************
static void _DetermineRight( double *pdNumber,
                             UINT *puRight,
                             UINT *puDecPos,
                             UINT uTotalSig )
{
   int   nDecPos=0, nCheck=0, nSign=0;

   ecvt(*pdNumber, uTotalSig, &nDecPos, &nSign);
   nCheck = nDecPos + *puRight; 
   if (nCheck < 0)
   {                                           
      *pdNumber = 0.0;
      *puDecPos = 0;
   }
   else
      *puDecPos = nCheck;
}

//****************************************************************************
//
//    Method Name:   _EraseTrailingZeros(LPSTR lpszDigit, int nDp)
//    Description:   Erases trailing zeros from the end to the beggining.
//
//    Arguments:     lpszDigit - String of digits.
//                   nDp       - where decimal point goes.
//                                      
//    Return:        Returns the number of chatacters in the buffer.
//****************************************************************************
static int _EraseTrailingZeros( LPSTR lpszDigit,
                                int nDp )
{
   WPTRASSERT(lpszDigit);

   int   nStrLen  = lstrlen(lpszDigit);               // get number of chars in string
   BOOL  bNeg     = (nDp < 0);
   int   nAbsDp  = abs(nDp);
   
   if (bNeg)
      nAbsDp++;                                        // zero based
   
   while (nAbsDp < nStrLen)
   {
      if (lpszDigit[--nStrLen] != '0')
         break;                                       // pre-decrement since were zero based
         
      lpszDigit[nStrLen] = 0;                         // put NULL where zero was
   }
   
   return lstrlen(lpszDigit);                          // return number of chars in string
}


   
