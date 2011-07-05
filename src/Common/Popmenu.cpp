//****************************************************************************************
//
//    Copyright (c) 1995-1997 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// MODULE:  POPMENU.CPP
// PURPOSE: Contains class member functions for CPopupMenu class. 
// AUTHOR:  BHI  Apr 1995
//
#include "wincpp.hpp"
#include "popmenu.hpp"
#include "AfxGDIObj.hpp"

//################################################################################################
//################################################################################################
//###
//###  CLASS:   CPopupMenu
//###  PURPOSE: Asynchronous popup menu.
//###
//################################################################################################
//################################################################################################

// Maximum length of a menu item loaded from a resource string.
static const int c_nMAX_ITEM_LENGTH = 128;

//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes the object.
//
CPopupMenu::CPopupMenu(HWND hWnd, HINSTANCE hInstance)
{
   MEMBERASSERT();
   m_hInstance = hInstance;
   m_hMenu = CreatePopupMenu();
   m_hWnd = hWnd;
}

//================================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CPopupMenu::~CPopupMenu()
{
   MEMBERASSERT();
   if (m_hMenu)
      DestroyMenu(m_hMenu);
   m_hMenu = NULL;
}

//================================================================================================
// FUNCTION: IsValid
// PURPOSE:  Checks that the object was created OK.
//
BOOL CPopupMenu::IsValid()
{
   MEMBERASSERT();
   return (m_hMenu!=NULL) && (m_hWnd!=NULL);
}

//================================================================================================
// FUNCTION: AppendMenu
// PURPOSE:  Appends a menu item to the popup menu.
//
BOOL CPopupMenu::AppendMenu(UINT uFlags, UINT uMenuID, LPCSTR lpszText)
{
   MEMBERASSERT();
   return ::AppendMenu(m_hMenu, uFlags, uMenuID, lpszText);
}

//================================================================================================
// FUNCTION: AppendMenu
// PURPOSE:  Appends a menu item to the popup menu.
//
BOOL CPopupMenu::AppendMenu(UINT uFlags, UINT uMenuID, int nStringResourceID)
{
   MEMBERASSERT();
   char szBuffer[c_nMAX_ITEM_LENGTH];
   LoadString(m_hInstance, nStringResourceID, szBuffer, sizeof(szBuffer));
   return AppendMenu(uFlags, uMenuID, szBuffer);
}

//===============================================================================================
// FUNCTION: AppendMenu
// PURPOSE:  Appends a bitmap item to the menu.
//
BOOL CPopupMenu::AppendMenu(UINT uFlags, UINT uMenuID, HBITMAP hBitmap)
{
   MEMBERASSERT();
   return ::AppendMenu(m_hMenu, uFlags | MF_BITMAP, uMenuID, LPCTSTR(hBitmap));
}

//================================================================================================
// FUNCTION: AppendSeparator
// PURPOSE:  Appends a separator item to the popup menu.
//
BOOL CPopupMenu::AppendSeparator()
{
   MEMBERASSERT();
   return AppendMenu(MF_SEPARATOR, 0, LPSTR(NULL));
}

//===============================================================================================
// FUNCTION: InsertMenuItem
// PURPOSE:  Inserts a new menu item at the specified position in a menu.
//
BOOL CPopupMenu::InsertMenuItem(UINT uMenuID, BOOL bByPosition, LPMENUITEMINFO pMII)
{
   MEMBERASSERT();
   return ::InsertMenuItem( m_hMenu, uMenuID, bByPosition, pMII); 
}

//===============================================================================================
// FUNCTION: GetMenuItemCount
// PURPOSE:  Returns the count of items in the menu.
//
int CPopupMenu::GetMenuItemCount() const
{
   MEMBERASSERT();
   return ::GetMenuItemCount(m_hMenu);
}

//================================================================================================
// FUNCTION: CheckItem
// PURPOSE:  Sets a check mark on a menu item.
//
BOOL CPopupMenu::CheckItem( UINT uFlags, UINT uIDItem )
{
   MEMBERASSERT();
   return ::CheckMenuItem(m_hMenu, uIDItem, uFlags ) != 0xFFFFFFFF;
}

//================================================================================================
// FUNCTION: CheckRadioItem
// PURPOSE:  Proces a group of items as a radio-button group.
//
BOOL CPopupMenu::CheckRadioItem( UINT uFlags, UINT uIDFirst, UINT uIDLast, UINT uIDItem )
{
   MEMBERASSERT();
   return ::CheckMenuRadioItem(m_hMenu, uIDFirst, uIDLast, uIDItem, uFlags );
}

//================================================================================================
// FUNCTION: SetDefaultItem
// PURPOSE:  Gives a menu item "default item" status.
//
void CPopupMenu::SetDefaultItem(UINT uMenuID)
{
   MEMBERASSERT();
   SetMenuDefaultItem(m_hMenu, uMenuID, FALSE);
}

//================================================================================================
// FUNCTION: TrackMenu
// PURPOSE:  This function should be called once the menu has been built to display
//           it and wait for a user selection. See TrackPopupMenu for more info.
//
int CPopupMenu::TrackMenu(int x, int y, UINT uFlags)
{
   MEMBERASSERT();
   CPoint pt(x, y);
   ClientToScreen(m_hWnd, &pt);
   SetCursor(LoadCursor(NULL, IDC_ARROW));
   return TrackPopupMenu(m_hMenu, uFlags, pt.x, pt.y, 0, m_hWnd, NULL);
}

//================================================================================================
// FUNCTION: TrackMenuEx
// PURPOSE:  Like TrackMenu, but synchronous.
//           Returns the selection rather than sending it to the parent.
//
int CPopupMenu::TrackMenuEx(int x, int y, UINT uFlags)
{
   MEMBERASSERT();
   return TrackMenu(x, y, uFlags|TPM_NONOTIFY|TPM_RETURNCMD);
}

//################################################################################################
//################################################################################################
//###
//###  CLASS:   CPopupMenuEx
//###  PURPOSE: Popup menu that can handle owner-draw menu items itself.
//###
//################################################################################################
//################################################################################################

DECLARE_STR(AxonCPopupMenuEx);
DECLARE_STR(ThisPointer);

//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes the object.
//
CPopupMenuEx::CPopupMenuEx(HWND hwndParent, HINSTANCE hInstance) 
   : CPopupMenu(NULL, hInstance)
{
   MEMBERASSERT();
   CreateWin(hwndParent);
}

//================================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CPopupMenuEx::~CPopupMenuEx()
{
   MEMBERASSERT();
   DestroyWin();
}

//===============================================================================================
// FUNCTION: OnMeasureItem
// PURPOSE:  Processes the message to return the dimensions of an owner-draw menu item.
//
static void OnMeasureItem(HWND hwnd, MEASUREITEMSTRUCT * lpMeasureItem)
{
   CPopupMenuEx *pThis = (CPopupMenuEx *)GetProp(hwnd, USE_STR(ThisPointer));
   pThis->OnMeasureItem(lpMeasureItem);
}

//===============================================================================================
// FUNCTION: OnDrawItem
// PURPOSE:  Processes the message to draw an owner-draw menu item.
//
static void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
   CPopupMenuEx *pThis = (CPopupMenuEx *)GetProp(hwnd, USE_STR(ThisPointer));
   pThis->OnDrawItem(lpDrawItem);
}

//===============================================================================================
// FUNCTION: OnCommand
// PURPOSE:  Processes menu selections.
//
static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT uCodeNotify)
{
   CPopupMenuEx *pThis = (CPopupMenuEx *)GetProp(hwnd, USE_STR(ThisPointer));
   pThis->OnCommand(id, hwndCtl, uCodeNotify);
}

//===============================================================================================
// FUNCTION: OnHelp
// PURPOSE:  Processes the Help message.
//
static void OnHelp(HWND hwnd, LPHELPINFO pHelpInfo)
{
   CPopupMenuEx *pThis = (CPopupMenuEx *)GetProp(hwnd, USE_STR(ThisPointer));
   pThis->OnHelp(pHelpInfo);
}

/* void Cls_OnHelp(HWND hwnd, LPHELPINFO pHelpInfo) */
#define HANDLE_WM_HELP(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (LPHELPINFO)(lParam)), 0L)
#define FORWARD_WM_HELP(hwnd, pHelpInfo, fn) \
    (void)(fn)((hwnd), WM_HELP, 0, (LPARAM)(LPHELPINFO)(pHelpInfo))

//===============================================================================================
// FUNCTION: WndProc
// PURPOSE:  Window proc for the hidden command target window.
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
      HANDLE_MSG(hWnd, WM_COMMAND,     OnCommand);
      HANDLE_MSG(hWnd, WM_HELP,        OnHelp);
      HANDLE_MSG(hWnd, WM_MEASUREITEM, OnMeasureItem);
      HANDLE_MSG(hWnd, WM_DRAWITEM,    OnDrawItem);
   }
   return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===============================================================================================
// FUNCTION: CreateWindow
// PURPOSE:  Create the hidden window that is used as a command target.
//
void CPopupMenuEx::CreateWin(HWND hwndParent)
{
   MEMBERASSERT();
   WNDCLASS cls      = { 0 };
   cls.hInstance     = m_hInstance;
   cls.lpszClassName = USE_STR(AxonCPopupMenuEx);
   cls.lpfnWndProc   = WndProc;
   if (!RegisterClass(&cls))
      return;
   
   m_hWnd = ::CreateWindow( USE_STR(AxonCPopupMenuEx), "", WS_CHILD, 
                            0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                            hwndParent, (HMENU)0, m_hInstance, 0 );
   if (!m_hWnd)
      return;

   SetProp(m_hWnd, USE_STR(ThisPointer), this);
}

//===============================================================================================
// FUNCTION: DestroyWindow
// PURPOSE:  Destroy the window that is used as a command target.
//
void CPopupMenuEx::DestroyWin()
{
   MEMBERASSERT();
   ::DestroyWindow(m_hWnd);
   UnregisterClass(USE_STR(AxonCPopupMenuEx), m_hInstance);
}

//===============================================================================================
// FUNCTION: OnCommand
// PURPOSE:  Called in response to a menu selection.
//
void CPopupMenuEx::OnCommand(int nID, HWND hwndCtl, UINT uCodeNotify)
{
   MEMBERASSERT();
   if (::GetParent(m_hWnd))
      FORWARD_WM_COMMAND(::GetParent(m_hWnd), nID, hwndCtl, uCodeNotify, SendMessage);
}

//===============================================================================================
// FUNCTION: OnHelp
// PURPOSE:  Process help message.
//
void CPopupMenuEx::OnHelp(LPHELPINFO pHelpInfo)
{
   MEMBERASSERT();
   HWND hwndParent = ::GetParent(m_hWnd);
   if (hwndParent)
      FORWARD_WM_HELP(hwndParent, pHelpInfo, SendMessage);
}

//===============================================================================================
// FUNCTION: OnMeasureItem
// PURPOSE:  Return measurement info for an owner-draw item.
//
void CPopupMenuEx::OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem)
{
   MEMBERASSERT();
   ERRORMSG("CPopupMenuEx::OnMeasureItem needs to be overridden");
}

//===============================================================================================
// FUNCTION: OnDrawItem
// PURPOSE:  Draw an owner-draw item.
//
void CPopupMenuEx::OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem)
{
   MEMBERASSERT();
   ERRORMSG("CPopupMenuEx::OnDrawItem needs to be overridden");
}

//################################################################################################
//################################################################################################
//###
//###  CLASS:   CIconMenu
//###  PURPOSE: Synchronous popup menu built from icons.
//###
//################################################################################################
//################################################################################################

//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes the object.
//
CIconMenu::CIconMenu(HWND hwndParent, HINSTANCE hInstance) 
   : CPopupMenuEx(hwndParent, hInstance)
{
   MEMBERASSERT();
   m_nButtonSize = GetSystemMetrics(SM_CYICON);
}

//================================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CIconMenu::~CIconMenu()
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: SetButtonSize
// PURPOSE:  Sets the size of the buttons.
//
void CIconMenu::SetButtonSize(int nButtonSize)
{
   MEMBERASSERT();
   m_nButtonSize = nButtonSize;
}

//===============================================================================================
// FUNCTION: AppendMenu
// PURPOSE:  Adds an ownerdraw icon to the menu.
//
BOOL CIconMenu::AppendMenu(UINT uFlags, UINT uMenuID, int nIconResourceID)
{
   MEMBERASSERT();
   HICON hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(nIconResourceID));
   return ::AppendMenu(m_hMenu, uFlags|MF_OWNERDRAW, uMenuID, LPCTSTR(hIcon));
}

//===============================================================================================
// FUNCTION: OnMeasureItem
// PURPOSE:  Return measurement info for an owner-draw item.
//
void CIconMenu::OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem)
{
   MEMBERASSERT();
   lpMeasureItem->itemWidth  = m_nButtonSize;
   lpMeasureItem->itemHeight = m_nButtonSize;
}

//===============================================================================================
// FUNCTION: OnDrawItem
// PURPOSE:  Draw an owner-draw item.
//
void CIconMenu::OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem)
{
   MEMBERASSERT();
   HDC   hDC     = lpDrawItem->hDC;
   CRect rItem   = lpDrawItem->rcItem;
   HICON hIcon   = (HICON)lpDrawItem->itemData;
   BOOL bChecked = (lpDrawItem->itemState & ODS_CHECKED);
   BOOL bSelected= (lpDrawItem->itemState & ODS_SELECTED);

   CBrush hbrBackground;
   hbrBackground.CreateSysColorBrush(bSelected ? COLOR_HIGHLIGHT : COLOR_MENU);

   FillRect(hDC, rItem, hbrBackground);
   DrawIcon(hDC, rItem.right-m_nButtonSize, rItem.top, hIcon);

   if (bChecked)
   {
      CBrush hbrSpot;
      hbrSpot.CreateSysColorBrush(COLOR_MENUTEXT);

      CRect rSpot = rItem;
      rSpot.right -= m_nButtonSize;

      CPoint pt = rSpot.CenterPoint();
      int nRadius = 4;

      CRgn rgn;
      rgn.CreateEllipticRgn(pt.x-nRadius, pt.y-nRadius, pt.x+nRadius, pt.y+nRadius);

      FillRgn(hDC, rgn, hbrSpot);
   }
}
