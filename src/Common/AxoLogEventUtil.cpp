//***************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***************************************************************************************
//
// MODULE:  AxoLogEventUtil.cpp
// PURPOSE: Definition file for the class CLogEvent.
// AUTHOR:  BLB Sept 1998
//
// DESCRIPTION: This class provides logging utilities for creating text blocks
//              and tables that are to be written to the lab book. This class also
//              provides a consistent interface between a source that is logging and
//              the logging object / lab book.
//
//***************************************************************************************

#include "wincpp.hpp"             // Precompiled header file must be first.
#include "AxoLogEventUtil.hpp"

#include "\AxonDev\Comp\AxonValidation\AxonValidation.h"

// Static constants.
static const char c_szNewLine[]   = "\n";
static const char c_szComma[]     = ",";
static const char c_szPeriod[]    = ".";
static const char c_szColon[]     = ":";
static const char c_szSemicolon[] = ";";
static const char c_szSpace[]     = " ";
static const char c_szInt[]       = "%d";
static const char c_szFloat[]     = "%g";

static UINT s_uAppMessage = RegisterWindowMessage(APP_COMMAND_STR);
static const char c_szOutOfMemory[] = "There was insufficient memory available from Windows.\n\nPossible solutions:\n1) Close some windows or applications.\n2) Restart Windows.";

// Define this if you want the logging object to use the old message style
// when sending log messages back to the application.
#define  USE_OLD_MESSAGE_STYLE   1

//=======================================================================================
// FUNCTION:   CLogEvent()
// PURPOSE:    Constructor for the class CLogEvent. Clears the log text and initializes
//             the member variables.
//
CLogEvent::CLogEvent( HWND p_hLogWnd )
   : m_hLogWnd( p_hLogWnd )
{
   MEMBERASSERT();

   m_strLogText   = "";     // The logging text (STL string).
   m_strTable     = "";     // The table (STL string).
   m_uNumCols     = 0;      // The number of table columns.
   m_uRowLength   = 0;      // The user-specified column row length.

   m_bTableIsInitialized = false;   // The table is initialized when MakeTable() is called. 
   m_bBorders            = false;   // Adds upper and lower borders to table if true.

   m_szFileName[ 0 ] = '\0';
   m_szEventType[ 0 ] = '\0';
   m_szEventSubType[ 0 ] = '\0';
}

//=======================================================================================
// FUNCTION:   CLogEvent()
// PURPOSE:    Constructor for the class CLogEvent. Clears the log text and initializes
//             the member variables.
//
CLogEvent::CLogEvent( HWND p_hLogWnd,
                      const char *pszPath,
                      const char *pszEventType,
                      const char *pszEventSubType )
   : m_hLogWnd( p_hLogWnd )
{
   MEMBERASSERT();

   m_strLogText   = "";     // The logging text (STL string).
   m_strTable     = "";     // The table (STL string).
   m_uNumCols     = 0;      // The number of table columns.
   m_uRowLength   = 0;      // The user-specified column row length.

   m_bTableIsInitialized = false;   // The table is initialized when MakeTable() is called. 
   m_bBorders            = false;   // Adds upper and lower borders to table if true.

   if ( pszPath )
      SetFileName( pszPath );
   if ( pszEventType )
      SetEventType( pszEventType );
   if ( pszEventSubType )
      SetEventSubType( pszEventSubType );
}

//=======================================================================================
// FUNCTION: ~CLogEvent
// PURPOSE:   Destructor for the class CLogEvent.
// RETURNS:   n/a.
//
CLogEvent::~CLogEvent()
{
   MEMBERASSERT();
}

//=======================================================================================
// FUNCTION:  SetFileName
// PURPOSE:   Sets the file name attribute.
//
void CLogEvent::SetFileName( const char *pszPath )
{
   MEMBERASSERT();
   AXU_strncpyz( m_szFileName, pszPath, sizeof( m_szFileName ) );
}

//=======================================================================================
// FUNCTION:  SetEventType
// PURPOSE:   Sets the event type attribute.
//
void CLogEvent::SetEventType( const char *pszEventType )
{
   MEMBERASSERT();
   AXU_strncpyz( m_szEventType, pszEventType, sizeof( m_szEventType ) );
}

//=======================================================================================
// FUNCTION:  SetEventSubType
// PURPOSE:   Sets the event sub-type attribute.
//
void CLogEvent::SetEventSubType( const char *pszEventSubType )
{
   MEMBERASSERT();
   AXU_strncpyz( m_szEventSubType, pszEventSubType, sizeof( m_szEventSubType ) );
}

//=======================================================================================
// FUNCTION:   AddString()
// PURPOSE:    Adds a string to the text block. Optionally appends a new line character.
// PARAMETERS: p_pszString: the pointer to the string that is to be added to the text block.
//             p_bNewLine: Default = TRUE. Appends a new line character to the end of the text if true.
//
// RETURNS:    None.
//
void CLogEvent::AddString( const char *p_pszString, const bool p_bNewLine )
{
   MEMBERASSERT();

   // Add the string to the logging text.
   m_strLogText.append(p_pszString);

   if (p_bNewLine)
      m_strLogText.append(c_szNewLine);
}


//=======================================================================================
// FUNCTION:   Space()
// PURPOSE:    Adds a space to the text block.
// RETURNS:    None.
//
void CLogEvent::Space()
{
   MEMBERASSERT();
   m_strLogText.append(c_szSpace);
}


//=======================================================================================
// FUNCTION:   Punctuate()
// PURPOSE:    Adds a punctuation to the text block. The options are
//             are:
//                   PUNCT_NONE        no punctuation mark appended
//                   PUNCT_PERIOD      appends period
//                   PUNCT_COMMA       appends comma
//                   PUNCT_COLON       appends colon
//                   PUNCT_SEMICOLON   appends semicolon
//
// RETURNS:    None.
//
void CLogEvent::Punctuate( const int p_nPunctuation )
{
   MEMBERASSERT();

   switch (p_nPunctuation)
   {
      case PUNCT_NONE:
         break;
      case PUNCT_PERIOD:
         m_strLogText.append(c_szPeriod);
         break;
      case PUNCT_COMMA:
         m_strLogText.append(c_szComma);
         break;
      case PUNCT_COLON:
         m_strLogText.append(c_szColon);
         break;
      case PUNCT_SEMICOLON:
         m_strLogText.append(c_szSemicolon);
         break;
   }
}


//=======================================================================================
// FUNCTION:   GetTextLength()
// PURPOSE:    Gets the length of the log text.
// RETURNS:    The length of the log text + space for the NULL.
//
UINT CLogEvent::GetTextLength() const
{
   MEMBERASSERT();
   return m_strLogText.length() + 1;
}


//=======================================================================================
// FUNCTION:   GetText()
// PURPOSE:    Copies the logging text to a buffer.
// PARAMETERS: p_pszBuffer: The buffer to hold the logging text.
//             p_uMaxLength: The length of the text to be copied.
// RETURNS:    None.
//
void CLogEvent::GetText( LPSTR p_pszBuffer, UINT p_uMaxLength )
{
   MEMBERASSERT();

   m_strLogText.copy(p_pszBuffer, p_uMaxLength);

   // Clear any trailing carriage return.
   if (p_pszBuffer[p_uMaxLength-2] == '\012')  // Octal 10
      p_pszBuffer[p_uMaxLength-2] = NULL;

   p_pszBuffer[p_uMaxLength-1] = NULL;
}


//=======================================================================================
// FUNCTION:   SendEvent()
// PURPOSE:    Send the text block to the log file.
// PARAMETERS: p_nEventPriority: The priority of the current log message.
//
// RETURNS:    None.
//
void CLogEvent::SendEvent( const int p_nEventPriority, const int p_nTimeStamp )
{
   MEMBERASSERT();
   ASSERT( m_hLogWnd );

   // Don't send if the log text is empty.
   if (m_strLogText == "")
      return;

   UINT uTextLength = m_strLogText.length() + 1;
   char *pszLogString = NULL;
   
   try
   {
      pszLogString = new char [uTextLength];
   }
   catch (CMemoryException *pEx)
   {
      pEx->Delete();

      AMD_MessageBox( AfxGetMainWnd()->GetSafeHwnd(), 
                      AfxGetInstanceHandle(),
                      c_szOutOfMemory, 
                      AMD_ERROR );
      return;
   }

   m_strLogText.copy(pszLogString, uTextLength);

   pszLogString[uTextLength-1] = NULL;

   // Clear any trailing carriage return.
   if (pszLogString[uTextLength-2] == '\012')  // Octal 10
      pszLogString[uTextLength-2] = NULL;

#if USE_OLD_MESSAGE_STYLE
   // Structure defined in "\AxonDev\Comp\Common\axappmsg.h"
   AM_LogEntryRec    Entry = { 0 };

   // Fill in the version 00 fields
   Entry.m_uVersion           = AXOAPP_LOG_EVENT_VERSION;
   Entry.m_lFlags             = p_nTimeStamp;
   Entry.m_uPriority          = p_nEventPriority;
   Entry.m_lpszEntryText      = pszLogString;

   // Send the log text.
   SendMessage( m_hLogWnd, s_uAppMessage, AM_LOGENTRY, (LONG)&Entry );
#else
   // Structure defined in "\AxonDev\Comp\Common\axappmsg.h"
   sLABBOOK_ENTRY    Entry;

   // Fill in the version 00 fields
   Entry.u.m_sData02.m_lFlags             = p_nTimeStamp;
   Entry.u.m_sData02.m_uPriority          = p_nEventPriority;
   Entry.u.m_sData02.m_lpszEntryText      = pszLogString;
   Entry.u.m_sData02.m_szFileName         = m_szFileName;
   Entry.u.m_sData02.m_szEventType        = m_szEventType;
   Entry.u.m_sData02.m_szEventSubType     = m_szEventSubType;

   // Send the log text.
   SendMessage( m_hLogWnd, s_uAppMessage, AM_LOGENTRY, (LONG)&Entry );
#endif

   // Clear the text.
   ClearTextBlock();

   // If there is a table then clear the row register and reset the status boolean.
   if (m_bTableIsInitialized)
   {
      FillRowWithSpaces();
      m_bTableIsInitialized = false;
   }

   // Delete the text buffer.
   delete [] pszLogString;
   pszLogString = NULL;
}


//=======================================================================================
// FUNCTION:   MakeTable()
// PURPOSE:    Creates a new table. There is no preset limit to the number of rows but
//             the number of columns is limited to c_uMaxNumCols.
// PARAMETERS: p_uNumCols: The number of columns in the table (max = c_uMaxNumCols).
//             p_uColWidth: An array of the column widths.
//             p_pszTitle: An optional table title (default = NULL).
//             p_bBorders: Add borders to top and bottom of table if true (default = false).
//
// NOTE:       Column titles are specified by using AddColumnEntry() to enter the titles.
//
// RETURNS:    None.
//
void CLogEvent::MakeTable( const UINT  p_uNumCols,
                           const UINT *p_uColWidth,
                           const char *p_pszTitle = NULL,
                           const bool  p_bBorders = false )
{
   MEMBERASSERT();
   RARRAYASSERT(p_uColWidth, p_uNumCols);

   if (m_bTableIsInitialized)
   {
      TRACE("Tried to create a table when it is already initialized.");
      return;
   }

   if (p_uNumCols > c_uMaxNumCols ||  p_uNumCols <= 0)
   {
      if (p_uNumCols > c_uMaxNumCols)
         TRACE1("The maximum column number of %d has been exceeded.\n", c_uMaxNumCols);

      if (p_uNumCols <= 0)
         TRACE("The number of columns must be greater than zero.\n");

      return;
   }

   m_bBorders   = p_bBorders;
   m_uNumCols   = p_uNumCols;
   m_uRowLength = 0;

   UINT uColIndex  = 0;  // Column starting position.

   for (UINT uI = 0; uI < p_uNumCols; uI++)
   {
      m_uColWidth[uI] = p_uColWidth[uI];  // First column at index 0.
      m_uRowLength += p_uColWidth[uI];    // Used to size the borders.

      m_uColIndex[uI] = uColIndex;   // Set the position of the columns in the row.
      uColIndex += p_uColWidth[uI];  // Set the index to the beginning of the next column.

      if (m_uRowLength > c_uMaxRowLength)
      {
         TRACE1("The maximum row length of %d has been exceeded.\n", c_uMaxRowLength);
         break;
      }
   }

   // Clear the table and fill the row array with spaces.
   ClearTable();
   FillRowWithSpaces();

   if (m_bBorders)
   {
      MakeBorder();
      m_strTable.append(c_szNewLine);
      m_strTable.append(c_szNewLine);
   }

   // Add the optional title to the table.
   if (p_pszTitle != NULL)
   {
      m_strTable.append(p_pszTitle);
      m_strTable.append(c_szNewLine);
      m_strTable.append(c_szNewLine);
   }
      
   m_bTableIsInitialized = true;
}


//=======================================================================================
// FUNCTION:   NewLine()
// PURPOSE:    Adds a new line to the text block.
// RETURNS:    None.
//
void CLogEvent::NewLine()
{
   MEMBERASSERT();
   m_strLogText.append(c_szNewLine);
}


//=======================================================================================
// FUNCTION:   AppendRow()
// PURPOSE:    Appends the current row and clears the row array.
// RETURNS:    None.
//
void CLogEvent::AppendRow()
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before AppendRow() is called.\n");
      return;
   }

   // Add a NULL character to the last row position.
   m_szRowText[m_uRowLength] = '\0';

   // Strip the white space from the ends of the table row text.
   AXU_StripWhiteSpace( m_szRowText );

   m_strTable.append(m_szRowText);
   m_strTable.append(c_szNewLine);

   // Clear the row array.
   FillRowWithSpaces();
}


//=======================================================================================
// FUNCTION:   CloseTable()
// PURPOSE:    Appends the last row to the table and appends the table to the text block.
//             If a table is to be sent to the log file with a different priority from
//             the text block then the table must be created and sent independently of
//             the text block.
// RETURNS:    None.
//
void CLogEvent::CloseTable()
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before CloseTable() is called.\n");
      return;
   }

   // Add the bottom border if requested.
   if (m_bBorders)
      MakeBorder();

   // Add the table to the text block, If an extra space is desired after
   // the end of the table an explicit call must be made to NewLine();
   m_strLogText.append(m_strTable);

   m_bTableIsInitialized = false;
}


//=======================================================================================
// FUNCTION:   AddRowString
// PURPOSE:    Adds a string entry to a new row. This should not be called until all column
//             entries for the previous row have been filled.
// PARAMETERS: p_strString: The CString table entry.
//
// RETURNS:    None.
//
void CLogEvent::AddRowString( CString p_strString )
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before AddRowString() is called.\n");
      return;
   }

   // Strip the white space from the ends of the table row text.
   m_strTable.append(c_szNewLine);
   m_strTable.append(p_strString);
}


//=======================================================================================
// FUNCTION:   AddColumnEntry(string overload)
// PURPOSE:    Adds a string entry to a column in the current row. If the entry is greater
//             than the column width it will be truncated.
// PARAMETERS: p_uColNum: The column number.
//             p_pszString: The table entry.
//             p_uPrecision: the number of decimal places. No formatting if NULL.
//             p_bUsePrecision: Use precision (number of decimal places) if true
//
// RETURNS:    None.
//
void CLogEvent::AddColumnEntry( const UINT  p_uColNum,
                                const char *p_pszString,
                                const UINT *p_uPrecision,
                                const bool  p_bUsePrecision )
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before AddColumnEntry() is called.\n");
      return;
   }

   // Ensure that the column number is within the allotted range.
   if (p_uColNum > m_uNumCols || p_uColNum < 1)
   {
      if (p_uColNum > m_uNumCols)
         TRACE1("The maximum of %d columns exceeded in AddColumnEntry(string).\n", m_uNumCols);
      else
         TRACE1("The column number is less than 1 in AddColumnEntry(string).\n", m_uNumCols);

      return;
   }

   // Get the index corresponding to the column number.
   UINT uColInx = p_uColNum - 1;

   // Get the starting index into the row for the current column.
   UINT uIndex = m_uColIndex[uColInx];
   UINT uSize  = m_uColWidth[uColInx];

   // Check that the column entry will fit into the row.
   if (uIndex + uSize > c_uMaxRowLength)
   {
      TRACE("The table width has been exceeded in AddColumnEntry(string).\n");
      return;
   }

   // Copy the string to conform to the required formatting size.
   char cszFormStr[VAL_EXTERNBUFSIZE] = { 0 };
   AXU_strncpyz( cszFormStr, p_pszString, VAL_EXTERNBUFSIZE );

   if (NULL != p_uPrecision && p_bUsePrecision)
   {
      int *pnError = NULL;
      FormatFloatString( cszFormStr, *p_uPrecision, uSize, pnError );

      // Revert to the original string if error.
      if (NULL != pnError)
         AXU_strncpyz( cszFormStr, p_pszString, uSize );
   }

   AXU_strncpyz( &(m_szRowText[uIndex]), cszFormStr, uSize );
   RemoveNulls( uIndex );
}


//=======================================================================================
// FUNCTION:   AddColumnEntry(integer overload)
// PURPOSE:    Adds an entry to a column using a integer argument. If the entry is greater
//             than the column width it will be truncated.
// PARAMETERS: p_uColNum: The column number.
//             p_nEntry: The table entry.
//             p_uPrecision: the number of decimal places. This is not used by
//                           the integer overload but is included for symmetry.
//             p_bUsePrecision: include for symmetry - not used.
//
// RETURNS:    None.
//
void CLogEvent::AddColumnEntry( const UINT  p_uColNum,
                                const int   p_nEntry,
                                const UINT *p_uPrecision,
                                const bool  p_bUsePrecision )
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before AddColumnEntry() is called.\n");
      return;
   }

   // Ensure that the column number is within the allotted range.
   if (p_uColNum > m_uNumCols || p_uColNum < 1)
   {
      if (p_uColNum > m_uNumCols)
         TRACE1("The maximum of %d columns exceeded in AddColumnEntry(int).\n", m_uNumCols);
      else
         TRACE1("The column number is less than 1 in AddColumnEntry(int).\n", m_uNumCols);

      return;
   }

   // Get the index corresponding to the column number.
   UINT uColInx = p_uColNum - 1;

   // Get the starting index into the row for the current column.
   UINT uIndex = m_uColIndex[uColInx];

   // Check that the column entry will fit into the row.
   if (uIndex + m_uColWidth[uColInx] > c_uMaxRowLength)
   {
      TRACE("The table width has been exceeded in AddColumnEntry(int).\n");
      return;
   }

   AXU_sprintf( &(m_szRowText[uIndex]), m_uColWidth[uColInx], c_szInt, p_nEntry );
   RemoveNulls( uIndex );
}


//=======================================================================================
// FUNCTION:   AddColumnEntry(float overload)
// PURPOSE:    Adds an entry to a column using a float argument. If the entry is greater
//             than the column width it will be truncated.
// PARAMETERS: p_uColNum: The column number.
//             p_fEntry: The table entry.
//             p_uPrecision: the number of decimal places. No formatting if NULL.
//             p_bUsePrecision: Use precision (number of decimal places) if true
//
// RETURNS:    None.
//
void CLogEvent::AddColumnEntry( const UINT  p_uColNum,
                                const float p_fEntry,
                                const UINT *p_uPrecision,
                                const bool  p_bUsePrecision )
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before AddColumnEntry() is called.\n");
      return;
   }

   // Ensure that the column number is within the allotted range.
   if (p_uColNum > m_uNumCols || p_uColNum < 1)
   {
      if (p_uColNum > m_uNumCols)
         TRACE1("The maximum of %d columns exceeded in AddColumnEntry(float).\n", m_uNumCols);
      else
         TRACE1("The column number is less than 1 in AddColumnEntry(float).\n", m_uNumCols);

      return;
   }

   // Get the index corresponding to the column number.
   UINT uColInx = p_uColNum - 1;

   // Get the starting index into the row for the current column.
   UINT uIndex = m_uColIndex[uColInx];
   UINT uSize  = m_uColWidth[uColInx];

   // Check that the column entry will fit into the row.
   if (uIndex + uSize > c_uMaxRowLength)
   {
      TRACE("The table width has been exceeded in AddColumnEntry(float).\n");
      return;
   }

   // Copy the string to conform to the required formatting size.
   char cszFormStr[VAL_EXTERNBUFSIZE] = { 0 };

   AXU_sprintf( cszFormStr, VAL_EXTERNBUFSIZE, c_szFloat, p_fEntry );

   if (NULL != p_uPrecision && p_bUsePrecision)
   {
      int *pnError = NULL;
      FormatFloatString( cszFormStr, *p_uPrecision, uSize, pnError );

      // Revert to the original string if error.
      if (NULL != pnError)
         AXU_sprintf( cszFormStr, uSize, c_szFloat, p_fEntry );
   }

   AXU_strncpyz( &(m_szRowText[uIndex]), cszFormStr, uSize );
   RemoveNulls( uIndex );
}


//=======================================================================================
// FUNCTION:   AddColumnEntry(double overload)
// PURPOSE:    Adds an entry to a column using a double argument. If the entry is greater
//             than the column width it will be truncated.
// PARAMETERS: p_uColNum: The column number.
//             p_dEntry: The table entry.
//             p_uPrecision: the number of decimal places. No formatting if NULL.
//             p_bUsePrecision: Use precision (number of decimal places) if true
//
// RETURNS:    None.
//
void CLogEvent::AddColumnEntry( const UINT   p_uColNum,
                                const double p_dEntry,
                                const UINT  *p_uPrecision,
                                const bool   p_bUsePrecision )
{
   MEMBERASSERT();

   if (!m_bTableIsInitialized)
   {
      TRACE("The table must be created before AddColumnEntry() is called.\n");
      return;
   }

   // Ensure that the column number is within the allotted range.
   if (p_uColNum > m_uNumCols || p_uColNum < 1)
   {
      if (p_uColNum > m_uNumCols)
         TRACE1("The maximum of %d columns exceeded in AddColumnEntry(double).\n", m_uNumCols);
      else
         TRACE1("The column number is less than 1 in AddColumnEntry(double).\n", m_uNumCols);

      return;
   }

   // Get the index corresponding to the column number.
   UINT uColInx = p_uColNum - 1;

   // Get the starting index into the row for the current column.
   UINT uIndex = m_uColIndex[uColInx];
   UINT uSize  = m_uColWidth[uColInx];

   // Check that the column entry will fit into the row.
   if (uIndex + uSize > c_uMaxRowLength)
   {
      TRACE("The table width has been exceeded in AddColumnEntry(double).\n");
      return;
   }

   // Copy the string to conform to the required formatting size.
   char cszFormStr[VAL_EXTERNBUFSIZE] = { 0 };
   AXU_sprintf( cszFormStr, uSize, c_szFloat, p_dEntry );

   if (NULL != p_uPrecision && p_bUsePrecision)
   {
      int *pnError = NULL;
      FormatDoubleString( cszFormStr, *p_uPrecision, uSize, pnError );

      // Revert to the original string if error.
      if (NULL != pnError)
         AXU_sprintf( cszFormStr, uSize, c_szFloat, p_dEntry );
   }

   AXU_strncpyz( &(m_szRowText[uIndex]), cszFormStr, uSize );
   RemoveNulls( uIndex );
}


//=======================================================================================
// FUNCTION:   FillRowWithSpaces()
// PURPOSE:    Fills the row string with spaces.
// RETURNS:    None.
//
void CLogEvent::FillRowWithSpaces()
{
   MEMBERASSERT();

   // Clear the row string by filling with spaces.
   memset(m_szRowText, ' ', ELEMENTS_IN(m_szRowText));
}


//=======================================================================================
// FUNCTION:   ClearTextBlock()
// PURPOSE:    Clears the text block.
// RETURNS:    None.
//
void CLogEvent::ClearTextBlock()
{
   MEMBERASSERT();
   m_strLogText = "";
}


//=======================================================================================
// FUNCTION:   ClearTable()
// PURPOSE:    Clears the table.
// RETURNS:    None.
//
void CLogEvent::ClearTable()
{
   MEMBERASSERT();
   m_strTable = "";
}


//=======================================================================================
// FUNCTION:   RemoveNulls()
// PURPOSE:    Replaces all the NULL characters from the row text with spaces.
// PARAMETERS: uStartPos: The starting position in the string.
// RETURNS:    None.
//
void CLogEvent::RemoveNulls( UINT uStartPos )
{
   MEMBERASSERT();
   ASSERT(uStartPos >= 0 && uStartPos < c_uMaxRowLength);
   ASSERT(m_uRowLength < c_uMaxRowLength);

   for (UINT uI = uStartPos; uI < m_uRowLength; uI++)
   {
      if (m_szRowText[uI] == '\0')
         strncpy(&(m_szRowText[uI]), " ", 1);
   }

   m_szRowText[m_uRowLength] = '\0';
}


//=======================================================================================
// FUNCTION:   MakeBorder()
// PURPOSE:    Makes a solid border line.
// RETURNS:    None.
//
void CLogEvent::MakeBorder()
{
   MEMBERASSERT();
   ASSERT(m_uRowLength >= 0 && m_uRowLength < c_uMaxRowLength);

   if (m_uRowLength < 0 && m_uRowLength >= c_uMaxRowLength)
      return;

   for (UINT uI = 0; uI < m_uRowLength; uI++)
      strncpy(&(m_szRowText[uI]), "_", 1);

   AXU_StripWhiteSpace( m_szRowText );

   m_strTable.append(m_szRowText);

   FillRowWithSpaces();   
}


//=======================================================================================
// FUNCTION:   FormatFloatString()
// PURPOSE:    Formats a string to a fixed-decimal float.
// RETURNS:    The formatted float value.
//
void CLogEvent::FormatFloatString( LPSTR p_pszFormatStr,
                                   UINT  p_uPrecision,
                                   UINT  p_uMaxDigits,
                                   int  *p_pnError )
{
   MEMBERASSERT();

   if (!VAL_IsStrNumerical( p_pszFormatStr ))
      return;

   float fConverted = 0.0f;

   VAL_FormatStrToFloat( p_pszFormatStr,
                         VAL_EXTERNBUFSIZE,
                         &fConverted,
                         VAL_FMT_FIXED_DIGITS,
                         p_uPrecision,
                         p_uMaxDigits,
                         p_pnError );

   if (NULL == p_pnError)
      AXU_sprintf( p_pszFormatStr, VAL_EXTERNBUFSIZE, c_szFloat, fConverted );
}


//=======================================================================================
// FUNCTION:   FormatDoubleString()
// PURPOSE:    Formats a string to a fixed-decimal double.
// RETURNS:    The formatted float value.
//
void CLogEvent::FormatDoubleString( LPSTR p_pszFormatStr,
                                    UINT  p_uPrecision,
                                    UINT  p_uMaxDigits,
                                    int  *p_pnError )
{
   MEMBERASSERT();

   if (!VAL_IsStrNumerical( p_pszFormatStr ))
      return;

   double dConverted = 0.0;
   int    *pnError   = NULL;

   VAL_FormatStrToDouble( p_pszFormatStr,
                          VAL_EXTERNBUFSIZE,
                          &dConverted,
                          VAL_FMT_FIXED_DIGITS,
                          p_uPrecision,
                          p_uMaxDigits,
                          p_pnError );

   if (NULL == p_pnError)
      AXU_sprintf( p_pszFormatStr, VAL_EXTERNBUFSIZE, c_szFloat, dConverted );
}
