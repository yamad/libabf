//****************************************************************************
//                                                                          **
//   Copyright (C) 1994 AXON Instruments Inc.                               **
//                                                                          **
//   This program is copyrighted and all modifications to this program      **
//   retain the original copyright. All rights reserved.                    **
//                                                                          **
//****************************************************************************

//****************************************************************************
//
//    Module Name:   AxonValidation.H
//    Description:   This module is a generic header file for AxoVal.CPP.
//                   
//****************************************************************************


#ifndef     __AxonValidation_H__
#define     __AxonValidation_H__

#define     VAL_EXTERNBUFSIZE  31                  /* anybody calling methods using conversion */
                                                   /* buffers MUST use this definition when declaring */
                                                   /* buffers - size can be larger */

enum VAL_FMTER {VAL_FMT_NOERROR,                   // no error condition 
                VAL_FMT_OVERFLOW_ER,               // string overflowed when converted
                VAL_FMT_STR_CONV_ER,               // string to float coversion error
                VAL_FMT_BADSTR_ER,                 // string had invalid characters
                VAL_FMT_FORMAT_ER,                 // error during formatting
                VAL_FMT_SNCLAMP_ER};               // display digits clamped within range
                
// Assume C declarations for C++. Now the DLL can be handled by C or C++
// applications. Also, C++ name mangling won't occur to the exported method names.
#ifdef __cplusplus
extern "C" {
#endif 

// These enums were changed in order to make the names easy to correlate with
// their function.  This is the translation table:
//
// VAL_FMT_TYPE1 -> VAL_FMT_FIXED_DIGITS 
// VAL_FMT_TYPE2 -> VAL_FMT_SIG_DIGITS 
//
typedef enum {VAL_FMT_FIXED_DIGITS,  VAL_FMT_SIG_DIGITS } VAL_FMTTYPE; 

BOOL WINAPI VAL_IntRange(int nDataToCheck, int nLowVal, int nHiVal);
BOOL WINAPI VAL_LongRange(long lDataToCheck, long lLowVal, long lHiVal);
BOOL WINAPI VAL_FloatRange(float fDataToCheck, float fLowVal, float fHiVal);
BOOL WINAPI VAL_DoubleRange(double dDataToCheck, double dLowVal, double dHiVal);
BOOL WINAPI VAL_IntList(int nDataToCheck, int *pnIntList, int nElemInList);
BOOL WINAPI VAL_LongList(long lDataToCheck, long *plLongList, int nElemInList);
BOOL WINAPI VAL_FloatList(float fDataToCheck, float *pfFloatList, int nElemInList);
BOOL WINAPI VAL_DoubleList(double dDataToCheck, double *pdDoubleList, int nElemInList); 
BOOL WINAPI VAL_CharList(LPSTR lpszStrToCheck, char **ppszStrList, int nElemInList, BOOL bIsCaseSen);
BOOL WINAPI VAL_IntGranularity(int *pnDataToCheck, int nGranularity);
BOOL WINAPI VAL_LongGranularity(long *plDataToCheck, long lGranularity);
BOOL WINAPI VAL_FloatGranularity(float *pfDataToCheck, float fGranularity);
BOOL WINAPI VAL_DoubleGranularity(double *pdDataToCheck, double dGranularity); 
BOOL WINAPI VAL_IsStrNull(LPSTR lpszStr);
BOOL WINAPI VAL_IsStrValid(LPSTR lpszSource, LPSTR lpszCheckStr);
BOOL WINAPI VAL_IsStrPrintable(LPSTR lpszSource);
BOOL WINAPI VAL_IsStrNumerical(LPSTR lpszSource);
BOOL WINAPI VAL_IsStrAlpha(LPSTR lpszSource);
BOOL WINAPI VAL_IsLegalName(LPSTR lpszFilename);
int  WINAPI VAL_FmtText(LPSTR lpszSrc, LPSTR lpszFormat, LPSTR lpszDest, int nOutLen, BOOL bBare);
int  WINAPI VAL_ConvertDblToStr(double dNumber, LPSTR lpszText, UINT uBufSize, VAL_FMTTYPE FmtType, 
                                       UINT uSigToRight, UINT uSNmaxDig, int *pnError);
BOOL WINAPI VAL_ConvertStrToDouble(LPSTR lpszSrc, double *pDouble);
int  WINAPI VAL_LimitDblToFloat(double *pDouble);
int  WINAPI VAL_FormatStrToFloat(LPSTR lpszSrc, UINT uBufSize, float *pfConverted, VAL_FMTTYPE FmtType, 
                                        UINT uSigToRight, UINT uSNmaxDig, int *pnError);
int  WINAPI VAL_FormatStrToDouble(LPSTR lpszSrc, UINT uBufSize, double *pdConverted, VAL_FMTTYPE FmtType,
                                         UINT uSigToRight, UINT uSNmaxDig, int *pnError);
int  WINAPI VAL_ConvertFltToStr(float fNumber, LPSTR lpszText, UINT uBufSize, VAL_FMTTYPE FmtType, 
                                       UINT uSigToRight, UINT uSNmaxDig, int *pnError);                                        
// End of the extern "C" needed for C declarations in C++
#ifdef __cplusplus
}
#endif
 
#endif      // __AxonValidation_H__
