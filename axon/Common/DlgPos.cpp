//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  DlgPos.CPP
// PURPOSE: Saves and restores a dialog position on screen
// AUTHOR:  RHW  Jun 1998
//

#include "wincpp.hpp"
#include "DlgPos.hpp"
#include "\Axondev\comp\axoutils32\axoutils32.h"

DECLARE_STR(DlgPosX);
DECLARE_STR(DlgPosY);
DECLARE_STR(DlgPosWidth);
DECLARE_STR(DlgPosHeight);

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CDlgPos::CDlgPos()
{
   MEMBERASSERT();   
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CDlgPos::~CDlgPos()
{
   MEMBERASSERT();
}


//===============================================================================================
// METHOD:   RestoreSize
// PURPOSE:  Restores the window size.
//
void CDlgPos::RestoreSize( HWND hWnd, const char * pszIniFile, const char * pSectionName )
{
   MEMBERASSERT();
   LPSZASSERT(pszIniFile);
   LPSZASSERT(pSectionName);
   HWNDASSERT(hWnd);

   int nXPosition = 0, nYPosition = 0;
   int nXSize = 0, nYSize = 0;
   
   AXU_GetPrivateProfileInt( pSectionName, USE_STR(DlgPosX), 0, &nXPosition, pszIniFile );
   AXU_GetPrivateProfileInt( pSectionName, USE_STR(DlgPosY), 0, &nYPosition, pszIniFile );
   AXU_GetPrivateProfileInt( pSectionName, USE_STR(DlgPosWidth), 0, &nXSize, pszIniFile );
   AXU_GetPrivateProfileInt( pSectionName, USE_STR(DlgPosHeight), 0, &nYSize, pszIniFile );

   if ( nXPosition < GetSystemMetrics( SM_CXSCREEN ) &&
        nYPosition < GetSystemMetrics( SM_CYSCREEN ) &&
        nXSize != 0 &&
        nYSize != 0 )
   {
      SetWindowPos( hWnd, NULL,
                    nXPosition, nYPosition,
                    nXSize, nYSize,
                    ( SWP_NOACTIVATE |
                      SWP_NOOWNERZORDER |
                      SWP_NOREDRAW |
                      SWP_NOZORDER ) );
   }
   else
   {
      AXU_CenterWindow( hWnd, GetParent(hWnd));      
   }   
}

//===============================================================================================
// METHOD:   SaveSize
// PURPOSE:  Saves the window size.
//
void CDlgPos::SaveSize( HWND hWnd, const char * pszIniFile, const char * pSectionName ) const
{
   MEMBERASSERT();
   LPSZASSERT(pszIniFile);
   LPSZASSERT(pSectionName);
   HWNDASSERT(hWnd);
      
   RECT rRect;
   ::GetWindowRect( hWnd, &rRect );
   int nXPosition = rRect.left;
   int nYPosition = rRect.top;
   int nXSize = rRect.right - rRect.left;
   int nYSize = rRect.bottom - rRect.top;
   
   AXU_WritePrivateProfileInt( pSectionName, USE_STR(DlgPosX), nXPosition, pszIniFile );
   AXU_WritePrivateProfileInt( pSectionName, USE_STR(DlgPosY), nYPosition, pszIniFile );
   AXU_WritePrivateProfileInt( pSectionName, USE_STR(DlgPosWidth), nXSize, pszIniFile );
   AXU_WritePrivateProfileInt( pSectionName, USE_STR(DlgPosHeight), nYSize, pszIniFile );   
}
