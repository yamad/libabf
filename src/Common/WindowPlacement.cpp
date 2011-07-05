//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  WindowPlacement.CPP
// PURPOSE: Reads and writes WINDOWPLACEMENT to/from the registry. 
// AUTHOR:  BHI  Aug 1998
//
#include "wincpp.hpp"
#include "WindowPlacement.hpp"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"

DECLARE_STR(showCmd);
DECLARE_STR(flags);
DECLARE_STR(ptMinPosition);
DECLARE_STR(ptMaxPosition);
DECLARE_STR(rcNormalPosition);
DECLARE_STR(Saved);

CWindowPlacement::CWindowPlacement()
{
   MEMBERASSERT();
   WINDOWPLACEMENT *pThis = this;
   memset(pThis, 0, sizeof(*pThis));
	// Note: "length" is inherited from WINDOWPLACEMENT
	length = sizeof(WINDOWPLACEMENT);
}

CWindowPlacement::~CWindowPlacement()
{
   MEMBERASSERT();
}

BOOL CWindowPlacement::LoadFromRegistry(LPCSTR pszSection, LPCSTR pszRegKey)
{
   MEMBERASSERT();
   LPSZASSERT(pszSection);
   LPSZASSERT(pszRegKey);

   // If the position has not been saved return FALSE.
   BOOL bSaved = FALSE;
   if (!AXU_GetPrivateProfileBOOL(pszSection, USE_STR(Saved), FALSE, &bSaved, pszRegKey) ||
       !bSaved)
      return FALSE;

   AXU_GetPrivateProfileUINT(pszSection, USE_STR(showCmd), 0, &showCmd, pszRegKey);
   AXU_GetPrivateProfileUINT(pszSection, USE_STR(flags), 0, &flags, pszRegKey);

   POINT ptDefault = { 0 };
   AXU_GetPrivateProfilePoint(pszSection, USE_STR(ptMinPosition), &ptDefault, &ptMinPosition, pszRegKey);
   AXU_GetPrivateProfilePoint(pszSection, USE_STR(ptMaxPosition), &ptDefault, &ptMaxPosition, pszRegKey);

   RECT rDefault = { 0 };
   AXU_GetPrivateProfileRect(pszSection, USE_STR(rcNormalPosition), &rDefault, &rcNormalPosition, pszRegKey);
   return TRUE;
}

void CWindowPlacement::SaveToRegistry(LPCSTR pszSection, LPCSTR pszRegKey)
{
   MEMBERASSERT();

   LPSZASSERT(pszSection);
   LPSZASSERT(pszRegKey);

   AXU_WritePrivateProfileUINT(pszSection, USE_STR(showCmd), showCmd, pszRegKey);
   AXU_WritePrivateProfileUINT(pszSection, USE_STR(flags), flags, pszRegKey);

   AXU_WritePrivateProfilePoint(pszSection, USE_STR(ptMinPosition), &ptMinPosition, pszRegKey);
   AXU_WritePrivateProfilePoint(pszSection, USE_STR(ptMaxPosition), &ptMaxPosition, pszRegKey);
   AXU_WritePrivateProfileRect(pszSection, USE_STR(rcNormalPosition), &rcNormalPosition, pszRegKey);
   AXU_WritePrivateProfileBOOL(pszSection, USE_STR(Saved), TRUE, pszRegKey);
}

BOOL CWindowPlacement::Restore(HWND hWnd, LPCSTR pszSection, LPCSTR pszRegKey)
{
   MEMBERASSERT();
   LPSZASSERT(pszSection);
   LPSZASSERT(pszRegKey);
   HWNDASSERT(hWnd);

	if (!LoadFromRegistry(pszSection, pszRegKey))
      return FALSE;

   DWORD dwStyle    = GetWindowLong(hWnd, GWL_STYLE);
   BOOL  bResizable = ((dwStyle & WS_THICKFRAME) != 0);
   BOOL  bTitleBar  = ((dwStyle & WS_CAPTION) != 0);

   // If the window is not resizable, leave the size as it is currently.
   if (!bResizable)
   {
      CRect rWindow;
      GetWindowRect(hWnd, &rWindow);
      rcNormalPosition.right  = rcNormalPosition.left + rWindow.Width();
      rcNormalPosition.bottom = rcNormalPosition.top  + rWindow.Height();
   }

   // If the window has a title bar then only check that the title bar intersects the screen.
   CRect rTest = rcNormalPosition;
   if (bTitleBar)
      rTest.bottom = rTest.top + GetSystemMetrics(SM_CYCAPTION) + 
                     GetSystemMetrics(bResizable ? SM_CYFRAME : SM_CYBORDER);

	// Only restore if window intersets a monitor.
   HMONITOR hMonitor = AXU_MonitorFromRect(&rTest, MONITOR_DEFAULTTONULL);
	if (!hMonitor)
		return FALSE;

   if (showCmd == SW_SHOWMINIMIZED)
      showCmd = SW_SHOWNORMAL;

   // Restore the window's position.
   return ::SetWindowPlacement(hWnd, this);
}

void CWindowPlacement::Save(HWND hWnd, LPCSTR pszSection, LPCSTR pszRegKey)
{
   MEMBERASSERT();

   LPSZASSERT(pszSection);
   LPSZASSERT(pszRegKey);
   HWNDASSERT(hWnd);

   ::GetWindowPlacement(hWnd, this);

#if 0
   if (!::IsWindowVisible(hWnd))
      showCmd = SW_HIDE;
   if (::IsIconic(hWnd))
      flags |= WPF_SETMINPOSITION;
#endif

   if (::IsZoomed(hWnd))
      flags |= WPF_RESTORETOMAXIMIZED;

	SaveToRegistry(pszSection, pszRegKey);
}

//===============================================================================================
// FUNCTION: SetShow
// PURPOSE:  .
//
void CWindowPlacement::SetShow(UINT uShowCmd, LPCSTR pszSection, LPCSTR pszRegKey)
{
   MEMBERASSERT();
   AXU_WritePrivateProfileUINT(pszSection, USE_STR(showCmd), uShowCmd, pszRegKey);   
}
