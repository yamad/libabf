//***********************************************************************************************
//
//    Copyright (c) 1995 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  AXPICWND.HPP
// PURPOSE: Contains class definition for the generic CPictureWin class.
// AUTHOR:  BHI  Feb 1995
//

#ifndef INC_CPICTWND_HPP
#define INC_CPICTWND_HPP

//====================================================================================================
// CLASS:   CPictureWnd
// PURPOSE: Provides a generic window class class.
//

class CPictureWnd
{
protected:
   HWND      m_hWnd;
   HINSTANCE m_hInstance;
   RECT      m_rClient;     // Size & position of the client area (maintained through OnSize).
   HFONT     m_hFont;
   
private:    // Unimplemented member functions.
   // CPictureWnd(const CPictureWnd &);
   // const CPictureWnd &operator=(const CPictureWnd &);

protected:

   // Register the window class.
   virtual BOOL Register(HINSTANCE hInstance);
   virtual BOOL Unregister();
   virtual LPCSTR GetClassName();
   
public:
   // Construction/destruction
   CPictureWnd();
   virtual ~CPictureWnd();
   
   // Set/Get the associated window handle  (internal function called during creation).
   void SetHWND(HWND hWnd);
   HWND GetHWND();
   
   BOOL CreateWnd(LPCSTR lpszTitle, DWORD dwStyle, RECT &rect, 
                  HWND hwndParent, int nWindowID, HINSTANCE hInstance,
                  DWORD dwExStyle=0);
   BOOL DestroyWnd();
                  
   // Replace a place holder control.
   BOOL ReplaceControl(HWND hWnd, int nControlID);
   
   // Invalidate the whole window (if one is connected)
   void Invalidate();
   
   // Message functions.   
   virtual BOOL OnNCCreate(CREATESTRUCT FAR* lpCreateStruct);
   virtual BOOL OnCreate(CREATESTRUCT FAR* lpCreateStruct);
   virtual void OnSize(UINT state, int cx, int cy);
   virtual void OnSetText(LPCSTR lpszText);
   virtual void OnSetFont(HFONT hFont, BOOL fRedraw);
   virtual HFONT OnGetFont();
   virtual void OnDraw(HDC hDC);
   virtual void OnClose();
   virtual void OnDestroy();
   virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
   
   // The static dialog proc function.
   static LRESULT CALLBACK EXPORT CPictureWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};



#endif    // __CPICTWND_HPP__
