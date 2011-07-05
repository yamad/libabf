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
//    Module Name:   TextFormat.cpp
//    Description:   Code module to implement ascii formatting.
//
//****************************************************************************
#include "wincpp.hpp"
#include "AxonValidation.h"

//****************************************************************************
//
//    Method Name:   ValidChar(char cMask, cChar)   
//    Description:   Test if character is valid for the mask associated 
//                   with it.
//
//****************************************************************************
ValidChar(char cMask, char cChar)   
{
   if ((cMask == '@' && isalpha(cChar)) || (cMask == '#' && isdigit(cChar)) || 
       (cMask == '!' && ispunct(cChar)) || (cMask == '&' && isalnum(cChar)) || 
       (cMask == '*' && isprint(cChar)))
       return TRUE;
   else
      return FALSE;
}             
                             
//****************************************************************************
//
//    Method Name:   FillChar(char cMask)
//    Description:   Generates output characters.
//
//****************************************************************************
char FillChar(char cMask)
{
   if (cMask == '!')
      return '-';
   else if (cMask == '#')
      return '_';
   else 
      return ' ';
}

//****************************************************************************
//
//    Method Name:   strnzcpy(LPSTR lpszDest, LPSTR lpszSrc, int nMaxLen)
//    Description:   Home brew version of strncpy().
//
//****************************************************************************
int strnzcpy(LPSTR lpszDest, LPSTR lpszSrc, int nMaxLen)
{   
   RPTRASSERT(lpszSrc);
   WPTRASSERT(lpszDest);
   ASSERT(nMaxLen > 0); 

   int iStrLen = lstrlen(lpszSrc);                    
   
   if (!nMaxLen)
      return 0;                                                               
           
   if (nMaxLen < 0)
      nMaxLen = iStrLen;
   else            
      nMaxLen = min(iStrLen, nMaxLen-1);
   
   _fstrncpy(lpszDest, lpszSrc, nMaxLen);
   lpszDest[nMaxLen] = '\0';
   return nMaxLen;
}

//****************************************************************************
//
//    Method Name:   VAL_FmtText(LPSTR lpszSrc, LPSTR lpszFormat, LPSTR lpszDest, 
//                                    int nOutLen,   BOOL bBare)
//    Description:   Format Src string using Format string and put into Dest.
// 
//                   The argument 'bBare':
//                      If TRUE, means do not copy any of the format string into
//                      the destination string.  If FALSE, non token items are
//                      copied into the destination string.
//
//                   The tokens are:
//                      # - Any numeric digit(0 - 9).
//                      @ - Any alphabetic character(a - z, A - Z)
//                      ! - Any punctuation character.
//                      * - Any single character.
//                      \ - Causes next token to be treated as a literal.
//                      & - Any alphanumeric character.
//
//                   If the format string is a NULL, this routine copies the source
//                   string to the destination string.
//****************************************************************************
int WINAPI VAL_FmtText(LPSTR lpszSrc, LPSTR lpszFormat, LPSTR lpszDest, int nOutLen, BOOL bBare)
{
   RPTRASSERT(lpszSrc);
   WPTRASSERT(lpszDest);
   ASSERT(nOutLen > 0); 
 
   LPSTR lpStop, lpStart;

   if (VAL_IsStrNull(lpszSrc) || VAL_IsStrNull(lpszDest) || nOutLen < 1)
      return 0;

   //  If there is no format copy the string
   if (!lpszFormat || *lpszFormat == 0)
      return strnzcpy(lpszDest, lpszSrc, nOutLen);

   //  While there are format chars and room in the output buffer
   for (lpStart = lpszDest, lpStop = lpStart + nOutLen - 1; *lpszFormat && lpszDest < lpStop; lpszFormat++)
   {
      switch (*lpszFormat)
      {
         case '@':                                       // Alphabetic chars
         case '#':                                       // Digits (0-9)
         case '!':                                       // Punctuation
         case '&':                                       // Alphanumeric
         case '*':                                       // Printable
            if (bBare)
            {
               if (*lpszSrc && !ValidChar(*lpszFormat, *lpszSrc))
               {
                  *lpszDest = 0;
                  return (nOutLen - (int)(lpStop - lpszDest));
               }
            }
            else     
               for (; *lpszSrc && !ValidChar(*lpszFormat, *lpszSrc); lpszSrc++) ; //Find next suitable
              
            if (*lpszSrc)
               *lpszDest++ = *lpszSrc++;
            else
               *lpszDest++ = FillChar(*lpszFormat);

            break;

         //  Take next as literal, e.g. \@ or \# for @ and # literals
         case '\\':
            lpszFormat++;
            // fall through

         //  Literals
         default:
            if ((bBare || !isalnum(*lpszFormat)) && *lpszFormat == *lpszSrc)   //  Advance source ptr if it matches format and is not alpha numeric
               lpszSrc++;  
               
            if (!bBare)             //  Copy the literal
               *lpszDest++ = *lpszFormat;
               
            break;
      }
   }

   *lpszDest = 0;

   return (int)(lpszDest - lpStart);
}

