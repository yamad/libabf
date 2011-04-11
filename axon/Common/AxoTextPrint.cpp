//*****************************************************************************
//
//    Copyright (c) 1996-1997 Axon Instruments.
//    All rights reserved.
//
//*****************************************************************************
// MODULE:  AxoTextPrint.cpp
// PURPOSE: This file contains the prototypes for the text printing
//          abstract class.
// AUTHOR:  JL  July 1997
//

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@                                                                         @@
//@@ Axon Generic Application Framework Component                            @@
//@@                                                                         @@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include "wincpp.hpp"

// Standard includes
#include "AxoTextPrint.h"

#include <WINSPOOL.H>

//=============================================================================
// Static constants
//=============================================================================
static const float s_fMarginLeft          = 1.0f;  // 1 inch.
static const float s_fMarginTop           = 1.0f;  // 1 inch.
static const float s_fMarginRight         = 1.0f;  // 1 inch.
static const float s_fMarginBottom        = 1.0f;  // 1 inch.


static const int s_nDefFontSize           = 10;    // 10 points.
static const char s_pszDefFontName[]      = "Courier New";

//=============================================================================
// Method Name:   CAxoTextPrint
// Purpose:       Constructor
//
CAxoTextPrint::CAxoTextPrint( HWND hOwnerWnd, BOOL bShowPrintDialog, LPCSTR lpszDocName, LPCTSTR pszProfileName )
{
   MEMBERASSERT();
   
   // Initialize data members.
   m_uMaxPage           = UINT_MAX;
   m_fMarginLeft        = s_fMarginLeft;
   m_fMarginTop         = s_fMarginTop;
   m_fMarginRight       = s_fMarginRight;
   m_fMarginBottom      = s_fMarginBottom;
   m_bShowPrintDialog   = bShowPrintDialog;
   
   ASSERT( hOwnerWnd );
   m_hOwnerWnd          = hOwnerWnd;
   
   WPTRASSERT( lpszDocName );
   strncpy( m_szDocName, lpszDocName, sizeof( m_szDocName ) );
   m_szDocName[ sizeof( m_szDocName ) - 1 ] = '\0';
   
   m_pszProfileName = pszProfileName;
}

//=============================================================================
// Method Name:   DoPrint
// Purpose:       Prints the text for the file
// Params:        none
// Returns:       BOOL TRUE - success, FALSE - failure.
//
BOOL CAxoTextPrint::DoPrint()
{
   MEMBERASSERT();
   
   // Fill out PRINTDLG structure which contains information that the PrintDlg
   // function uses to initialize the Print common dialog box.
   PRINTDLG pd = { 0 };
   
   pd.lStructSize = sizeof( pd );
   pd.hwndOwner = m_hOwnerWnd;
   CreateDeviceHandles( &pd.hDevNames, &pd.hDevMode );
   if ( pd.hDevNames != NULL && m_bShowPrintDialog == FALSE )
   {
      // Use most recently used printer in the registry.
      LPDEVNAMES lpDevNames = (LPDEVNAMES)GlobalLock( pd.hDevNames );
      LPDEVMODE  lpDevMode = ( pd.hDevMode != NULL ) ?
         (LPDEVMODE)GlobalLock( pd.hDevMode ) : NULL;
      
      if ( lpDevNames == NULL )
         return FALSE;
      
      pd.hDC = CreateDC( (LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
         (LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
         (LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
         lpDevMode );
      
      GlobalUnlock( pd.hDevNames );
      if ( pd.hDevMode != NULL )
         GlobalUnlock( pd.hDevMode );
   }
   else
   {
      // Show / hide the 'Print' dialog box.
      if ( m_bShowPrintDialog )
         pd.Flags = PD_RETURNDC;
      else
         pd.Flags = PD_RETURNDC | PD_RETURNDEFAULT;
      
      // Bring up the Print dialog box or in silent mode depending on
      // the m_bShowPrintDialog flag. Note that the Print Setup dialog box has been
      // superseded by the Page Setup common dialog box in WIN32.
      
      if ( PrintDlg( &pd ) == 0 )
         return FALSE;
   }
   
   DOCINFO docInfo = { 0 };
   
   docInfo.cbSize = sizeof( docInfo );
   docInfo.lpszDocName = m_szDocName;
   docInfo.lpszOutput = NULL;
   docInfo.lpszDatatype = NULL;
   docInfo.fwType = 0;
   
   // Obtain a handle to the device context.
   ASSERT( pd.hDC );
   if ( pd.hDC == NULL )
      return FALSE;
   
   // Set the printing alignment etc.
   SetPrintDC( pd.hDC );
   
   // Find out the printable area margins
   RECT rectPage = { 0 }, rectMargins = { 0 };
   
   GetDefaults( pd.hDC, rectPage, rectMargins );
   
   // Set the printer font.
   LOGFONT logFont = { 0 };
   
   // Set default font height.  note: 1 point = 1/72 inches.
   logFont.lfHeight = -MulDiv( s_nDefFontSize,
      GetDeviceCaps( pd.hDC, LOGPIXELSY ), 72 );
   logFont.lfWidth = 0;
   
   // Set default font name.
   strncpy( logFont.lfFaceName, s_pszDefFontName, sizeof( logFont.lfFaceName ) );
   logFont.lfFaceName[ sizeof( logFont.lfFaceName ) - 1 ] = '\0';
   
   HFONT hNewFont = CreateFontIndirect( &logFont );
   ASSERT( hNewFont );
   
   HFONT hOldFont = SelectFont( pd.hDC, hNewFont );
   
   // Begin a new print job.
   VERIFY( StartDoc( pd.hDC, &docInfo ) > 0 );
   
   for ( UINT uPage = 0; uPage < m_uMaxPage; uPage++ )
   {
      // Begin a new page.
      VERIFY( StartPage( pd.hDC ) > 0 );
      
      SetPrintDC( pd.hDC );
      
      SelectFont( pd.hDC, hNewFont );
      
      BOOL bContinue =  OnPrintPage( pd.hDC, uPage, rectPage );
      
      // End a page.
      VERIFY( EndPage( pd.hDC ) > 0 );
      
      // Terminated in the derivatives.
      if ( bContinue == FALSE )
         break;
   }
   
   // End a print job.
   VERIFY( EndDoc( pd.hDC ) > 0 );
   
   // Restore the old font.
   SelectFont( pd.hDC, hOldFont );
   
   if ( hNewFont )            
      DeleteFont( hNewFont );  
   
   // Clean up
   DeleteDC( pd.hDC );
   
   if ( pd.hDevMode != NULL )
      GlobalFree( pd.hDevMode );
   if ( pd.hDevNames != NULL )
      GlobalFree( pd.hDevNames );
   
   return TRUE;
}

//============================================================================
// Method Name:   SetPrintDC
// Purpose:       Sets mapping mode for the printing.
// Params:        none
// Returns:       none
//
void CAxoTextPrint::SetPrintDC( HDC hDC )
{
   MEMBERASSERT();
   
   // Be sure that the printer DC is in text mode.
   SetMapMode( hDC, MM_TEXT );
   
   // Do not assume that switching to MM_TEXT will reset these params for you.
   SetWindowOrgEx( hDC, 0, 0, NULL );
   SetViewportOrgEx( hDC, 0, 0, NULL );
}

//============================================================================
// Method Name:   GetDefaults
// Purpose:       Gets the default printable area, margins etc.
// Params:        none
// Returns:       none
//
void CAxoTextPrint::GetDefaults( HDC hDC, RECT &rectPage, RECT &rectMargins )
{
   MEMBERASSERT();
   
   ASSERT( GetMapMode(hDC) == MM_TEXT );
   
   // Start by getting the dimensions of the unprintable part of the
   // page (in device units). GETPRINTINGOFFSET will tell us the left
   // and upper unprintable area.
   
   POINT ptOffset = { 0 };
   
   Escape( hDC, GETPRINTINGOFFSET, 0, NULL, (LPSTR)&ptOffset );
   rectMargins.left = ptOffset.x;
   rectMargins.top  = ptOffset.y;
   
   // To get the right and lower unprintable area, we need to take
   // the entire width and height of the paper (GETPHYSPAGESIZE) and
   // subtract everything else.
   POINT ptSize = { 0 };
   ptSize.x = GetDeviceCaps( hDC, PHYSICALWIDTH );
   ptSize.y = GetDeviceCaps( hDC, PHYSICALHEIGHT );
   
   int nPrintableWidth = GetDeviceCaps( hDC, HORZRES );
   int nPrintableHeight = GetDeviceCaps( hDC, VERTRES );
   rectMargins.right  = ptSize.x                         // total paper width
      - nPrintableWidth                // printable width
      - rectMargins.left;              // left unprtable margin
   
   rectMargins.bottom = ptSize.y                         // total paper height
      - nPrintableHeight               // printable height
      - rectMargins.top;               // top unprtable margin
   
   // At this point, rectMargins contains the widths of the
   // unprintable regions on all four sides of the page in device units.
   int nLogX = GetDeviceCaps( hDC, LOGPIXELSX );         // dpi in X direction
   int nLogY = GetDeviceCaps( hDC, LOGPIXELSY );         // dpi in Y direction
   
   // Margins
   rectMargins.top = int( m_fMarginTop * nLogY ) - rectMargins.top;
   rectMargins.bottom = int( m_fMarginBottom * nLogY ) - rectMargins.bottom;
   rectMargins.left = int( m_fMarginLeft * nLogX ) - rectMargins.left;
   rectMargins.right = int( m_fMarginRight * nLogX ) - rectMargins.right;
   
   // rectMargins now contains the values used to shrink the printable
   // area of the page. Could check rectMargins here for negative values
   // to prevent setting margins outside the printable area of the page.
   if ( rectMargins.top < 0 )
      rectMargins.top = 0;
   if ( rectMargins.bottom < 0 )
      rectMargins.bottom = 0;
   if ( rectMargins.left < 0 )
      rectMargins.left = 0;
   if ( rectMargins.right < 0 )
      rectMargins.right = 0;
   
   // Calculate the printable area
   rectPage.left = rectMargins.left;
   rectPage.top = rectMargins.top;
   rectPage.right = nPrintableWidth - rectMargins.right;
   rectPage.bottom = nPrintableHeight - rectMargins.bottom;
}

//*****************************************************************************
// Static constants
//*****************************************************************************
static const char s_szLastPrinter[] = "LastPrinter";
static const char s_szDeviceName[]  = "DeviceName";

//============================================================================
// Method Name:   GetDefaults
// Purpose:       Gets the default printable area, margins etc.
// Params:        none
// Returns:       none
//
void CAxoTextPrint::CreateDeviceHandles( HANDLE *phDevNames, HANDLE *phDevMode )
{
   MEMBERASSERT();
   WPTRASSERT( phDevNames );
   WPTRASSERT( phDevMode );
   
   // Handles
   *phDevNames = NULL;
   *phDevMode = NULL;
   if ( m_pszProfileName == NULL )
      return;
   
   char szDeviceName[_MAX_PATH] = { 0 };
   AXU_GetPrivateProfileString( s_szLastPrinter, s_szDeviceName, "", szDeviceName, sizeof( szDeviceName ), m_pszProfileName );
   
   if ( *szDeviceName == '\0' )
      return;
   
   // Open printer and obtain PRINTER_INFO_2 structure.
   HANDLE hPrinter = NULL;
   if ( OpenPrinter( szDeviceName, &hPrinter, NULL ) == FALSE )
      return;
   DWORD dwBytesReturned = 0, dwBytesNeeded = 0;
   GetPrinter( hPrinter, 2, NULL, 0, &dwBytesNeeded );
   PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)LocalAlloc( LPTR, dwBytesNeeded );
   GetPrinter( hPrinter, 2, (LPBYTE)p2, dwBytesNeeded, &dwBytesReturned );
   DWORD dwDevModeSize = DocumentProperties( NULL, hPrinter, szDeviceName, NULL, NULL, 0 );
   VERIFY( ClosePrinter( hPrinter ) );
   
   // Allocate a global handle for DEVMODE and copy DEVMODE data.
   HGLOBAL  hDevMode = GlobalAlloc( GPTR, dwDevModeSize );
   DEVMODE* pDevMode = (DEVMODE*)GlobalLock(hDevMode);
   memcpy( pDevMode, p2->pDevMode, dwDevModeSize );
   GlobalUnlock( hDevMode );
   
   // Compute size of DEVNAMES structure you'll need.
   DWORD drvNameLen = strlen(p2->pDriverName);  // driver name
   DWORD ptrNameLen = strlen(p2->pPrinterName); // printer name
   DWORD porNameLen = strlen(p2->pPortName);    // port name
   DWORD devNameSize = sizeof(DEVNAMES) +
      ptrNameLen + porNameLen + drvNameLen + 3; // Three '\0' terminators.
   
   // Allocate a global handle big enough to hold DEVNAMES.
   HGLOBAL   hDevNames = GlobalAlloc( GPTR, devNameSize );
   DEVNAMES* pDevNames = (DEVNAMES*)GlobalLock( hDevNames );
   
   // Copy the DEVNAMES information from PRINTER_INFO_2 structure.
   pDevNames->wDriverOffset = sizeof( DEVNAMES );
   memcpy( (char*)pDevNames + pDevNames->wDriverOffset,
      p2->pDriverName, drvNameLen );
   
   pDevNames->wDeviceOffset = WORD( sizeof( DEVNAMES ) + drvNameLen + 1 );
   memcpy( (char*)pDevNames + pDevNames->wDeviceOffset,
      p2->pPrinterName, ptrNameLen );
   
   pDevNames->wOutputOffset = WORD( sizeof( DEVNAMES ) + drvNameLen + ptrNameLen + 2 );
   memcpy( (char*)pDevNames + pDevNames->wOutputOffset,
      p2->pPortName, porNameLen );
   
   pDevNames->wDefault = 0;
   
   GlobalUnlock( hDevNames );
   LocalFree(p2);   // free PRINTER_INFO_2
   
   // Handles
   *phDevNames = hDevNames;
   *phDevMode = hDevMode;
}

