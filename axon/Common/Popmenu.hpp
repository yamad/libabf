//****************************************************************************************
//
//    Copyright (c) 1995-1997 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// HEADER:  POPMENU.HPP
// PURPOSE: Contains class definition for CPopupMenu class.
// AUTHOR:  BHI  Apr 1995
//

#ifndef INC_POPMENU_HPP
#define INC_POPMENU_HPP

//################################################################################################
//################################################################################################
//###
//###  CLASS:   CPopupMenu
//###  PURPOSE: Asynchronous popup menu.
//###
//################################################################################################
//################################################################################################

class CPopupMenu
{
protected:
   HINSTANCE   m_hInstance;   // Instance handle for loading string resources
   HMENU       m_hMenu;       // The menu handle for the encapsulated menu.
   HWND        m_hWnd;        // The window handle to send a message to.

private:    // Disable default constructor and copy constructors.
   CPopupMenu();
   CPopupMenu(const CPopupMenu &);
   const CPopupMenu &operator=(const CPopupMenu &);
   
public:
   CPopupMenu(HWND hWnd, HINSTANCE hInstance);
   ~CPopupMenu();
   
   BOOL IsValid();
   BOOL AppendMenu(UINT uFlags, UINT uMenuID, LPCSTR lpszText);
   BOOL AppendMenu(UINT uFlags, UINT uMenuID, int nStringResourceID);
   BOOL AppendMenu(UINT uFlags, UINT uMenuID, HBITMAP hBitmap);
   BOOL InsertMenuItem(UINT uMenuID, BOOL bByPosition, LPMENUITEMINFO pMII); 

   BOOL AppendSeparator();
   int  GetMenuItemCount() const; 

   BOOL CheckItem( UINT uFlags, UINT nIDCheckItem );
   BOOL CheckRadioItem(UINT uFlags, UINT uIDFirst, UINT uIDLast, UINT uIDItem);

   void SetDefaultItem(UINT uMenuID);
   int  TrackMenu(int x, int y, UINT uFlags = TPM_LEFTALIGN|TPM_RIGHTBUTTON);
   int  TrackMenuEx(int x, int y, UINT uFlags = TPM_LEFTALIGN|TPM_RIGHTBUTTON);
};

//################################################################################################
//################################################################################################
//###
//###  CLASS:   CPopupMenuEx
//###  PURPOSE: Synchronous popup menu.
//###
//################################################################################################
//################################################################################################

class CPopupMenuEx : public CPopupMenu
{
private:

private:    // Disable default constructor and copy constructors.
   CPopupMenuEx();
   CPopupMenuEx(const CPopupMenuEx &);
   const CPopupMenuEx &operator=(const CPopupMenuEx &);

private:
   void CreateWin(HWND hwndParent);
   void DestroyWin();
   
public:
   CPopupMenuEx(HWND hwndParent, HINSTANCE hInstance);
   ~CPopupMenuEx();

   virtual void OnCommand(int nID, HWND hwndCtl, UINT uCodeNotify);
   virtual void OnHelp(LPHELPINFO pHelpInfo);
   virtual void OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem);
   virtual void OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem);
};

//################################################################################################
//################################################################################################
//###
//###  CLASS:   CPopupMenuEx
//###  PURPOSE: Synchronous popup menu.
//###
//################################################################################################
//################################################################################################

class CIconMenu : public CPopupMenuEx
{
private:
   int m_nButtonSize;

private:    // Disable default constructor and copy constructors.
   CIconMenu();
   CIconMenu(const CIconMenu &);
   const CIconMenu &operator=(const CIconMenu &);

public:
   CIconMenu(HWND hwndParent, HINSTANCE hInstance);
   ~CIconMenu();

   void SetButtonSize(int nButtonSize);
   BOOL AppendMenu(UINT uFlags, UINT uMenuID, int nIconResourceID);

   virtual void OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem);
   virtual void OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem);
};

#endif   // INC_POPMENU_HPP
