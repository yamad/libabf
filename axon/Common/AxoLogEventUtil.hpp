//***************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***************************************************************************************
//
// MODULE:  AxoLogEventUtil.hpp
// PURPOSE: Declaration file for the class CLogEvent.
// AUTHOR:  BLB Sept 1998
//
//***************************************************************************************

#if !defined INC_AXOLOGEVENTUTIL_HPP
#define      INC_AXOLOGEVENTUTIL_HPP

#include "\Axondev\Comp\Common\Axappmsg.h"
#include <string>
using namespace std;

// The following disables warning message 4786 that states that the 
// debug information has been truncated to 255 characters or less-an STL
// implementation issue.
#pragma warning(disable:4786)

// Static constants.
static const int c_uMaxRowLength = 1024;
static const int c_uMaxNumCols   = 64;

// Puntuation constant definitions.
static const int PUNCT_NONE      = 0;
static const int PUNCT_PERIOD    = 1;
static const int PUNCT_COMMA     = 2;
static const int PUNCT_COLON     = 3;
static const int PUNCT_SEMICOLON = 4;


//---------------------------------------------------------------------------------------
// CLogEvent: THE LOGGING INTERFACE TEXT FORMATTING CLASS

class CLogEvent
{
private:
   // Prevent the implementation of the copy constructor.
   CLogEvent( const CLogEvent & );
   const CLogEvent & operator = ( const CLogEvent & );

public:
   CLogEvent( HWND p_hLogWnd );
   CLogEvent( HWND p_hLogWnd,
              const char *pszPath,
              const char *pszEventType,
              const char *pszEventSubType );
   ~CLogEvent();

private:
   bool m_bTableIsInitialized;
   bool m_bBorders;
   HWND m_hLogWnd;

   UINT m_uNumCols;
   UINT m_uRowLength;

   char m_szRowText[c_uMaxRowLength];
   UINT m_uColIndex[c_uMaxNumCols];
   UINT m_uColWidth[c_uMaxNumCols];

   string m_strLogText;
   string m_strTable;

   char m_szFileName[ _MAX_PATH ];
   char m_szEventType[ _MAX_PATH ];       // There should be a constant for this
   char m_szEventSubType[ _MAX_PATH ];    // however this seemed to be reasonably long.

public:
   void SendEvent( const int p_nEventPriority,
                   const int p_nTimeStamp = AXOAPP_LOG_FLAG_MAND_STAMP );

   // Log text functions.
   void AddString( const char *p_pszString, const bool p_bNewLine = true );

   void GetText( LPSTR p_pszBuffer, UINT p_uMaxLength );
   void NewLine();
   void Space();
   void Punctuate( const int p_nPunctuation ); 

   // Table functions.
   void MakeTable( const UINT  p_uNumCols,
                   const UINT *p_uColWidth,
                   const char *p_pszTitle,
                   const bool  p_bBorders );

   void AppendRow();
   void AddColumnEntry( const UINT p_uColNum, const char *p_pszString, const UINT *p_uPrecision = NULL, const bool p_bUsePrecision = false );
   void AddColumnEntry( const UINT p_uColNum, const int p_nEntry, const UINT *p_uPrecision = NULL, const bool p_bUsePrecision = false );
   void AddColumnEntry( const UINT p_uColNum, const float p_fEntry, const UINT *p_uPrecision = NULL, const bool p_bUsePrecision = false );
   void AddColumnEntry( const UINT p_uColNum, const double p_dEntry, const UINT *p_uPrecision = NULL, const bool p_bUsePrecision = false );
   void AddRowString( CString p_strString );
   void CloseTable();

   void SetFileName( const char *pszPath );
   void SetEventType( const char *pszEventType );
   void SetEventSubType( const char *pszEventSubType );
   void ClearTextBlock();

   UINT GetTextLength() const;

private:
   void FillRowWithSpaces();
   void ClearTable();
   void MakeBorder();
   void RemoveNulls( UINT uStartPos );

   void FormatFloatString( LPSTR p_pszFormatStr, UINT p_uPrecision, UINT p_uMaxDigits, int *p_pnError );
   void FormatDoubleString( LPSTR p_pszFormatStr, UINT p_uPrecision, UINT p_uMaxDigits, int *p_pnError );
};

#endif      //INC_AXOLOGEVENTUTIL_HPP
