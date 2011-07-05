//***********************************************************************************************
//
//    Copyright (c) 1995 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  cpictwnd.cpp
// PURPOSE: Generic Picture Window Class. 
// AUTHOR:  BHI  Feb 1995
//
#include "wincpp.hpp"
#include "\AxonDev\Comp\Common\cpictwnd.hpp"
#include "\AxonDev\Comp\Axoutils32\Axoutils32.h"

//===============================================================================================
// Macros for setting and getting the pointer to the acquisition object.
#define GWL_PICTUREWND 0      // Offset to CPictureWnd pointer in Wnd extra bytes.
#define GETOBJPTR(hWnd)       ((CPictureWnd *)GetWindowLong((hWnd), GWL_PICTUREWND))
#define SETOBJPTR(hWnd, pObj) (SetWindowLong((hWnd), GWL_PICTUREWND, (long)(pObj)))

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Sets the m_hWnd variable and initializes the object.
//
CPictureWnd::CPictureWnd()
{
   MEMBERASSERT();
   m_hWnd      = NULL;
   m_hInstance = NULL;
   m_hFont     = NULL;
   SetRectEmpty(&m_rClient);
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleans up on destruction of the object.
//
CPictureWnd::~CPictureWnd()
{
   MEMBERASSERT();
   
   // Destroy the window if it exists (benign if it does not).
   DestroyWnd();
   Unregister();
}

//===============================================================================================
// FUNCTION: SetHWND
// PURPOSE:  Set associated window handle.
//
void CPictureWnd::SetHWND(HWND hWnd)
{
   MEMBERASSERT();
   m_hWnd = hWnd;
}

//===============================================================================================
// FUNCTION: GetHWND
// PURPOSE:  Get the HWND associated with the object.
//
HWND CPictureWnd::GetHWND()
{
   MEMBERASSERT();
   return m_hWnd;
}

//===============================================================================================
// FUNCTION: GetClassName
// PURPOSE:  Return a pointer to the name of the window class.
// NOTES:    If a derived class overrides the Register() member function it must also 
//           have a unique class name provided by an overridden GetClassName() function.
//
LPCSTR CPictureWnd::GetClassName()
{
   MEMBERASSERT();
   return "AXO_CPictureWndClass32";
}

//===============================================================================================
// FUNCTION: Invalidate
// PURPOSE:  Invalidate the whole window (if one is connected).
//
void CPictureWnd::Invalidate()
{
   MEMBERASSERT();
   if (IsWindow(m_hWnd))
      InvalidateRect(m_hWnd, NULL, TRUE);
}

//===============================================================================================
// FUNCTION: OnNCCreate
// PURPOSE:  Processes the WM_NCCREATE message.
//
BOOL CPictureWnd::OnNCCreate(CREATESTRUCT FAR* lpCreateStruct)
{
   MEMBERASSERT();
   return (BOOL)DefWindowProc(WM_NCCREATE, 0, LPARAM(lpCreateStruct));
}

//===============================================================================================
// FUNCTION: OnCreate
// PURPOSE:  Processes the WM_CREATE message.
//
BOOL CPictureWnd::OnCreate(CREATESTRUCT FAR* lpCreateStruct)
{
   MEMBERASSERT();
   GetClientRect( m_hWnd, &m_rClient );
   return TRUE;
}

//===============================================================================================
// FUNCTION: OnSize
// PURPOSE:  Processes the WM_SIZE message.
//
void CPictureWnd::OnSize(UINT state, int cx, int cy)
{
   MEMBERASSERT();
   GetClientRect( m_hWnd, &m_rClient );
}

//===============================================================================================
// FUNCTION: OnSetText
// PURPOSE:  Processes the WM_SETTEXT message.
//
void CPictureWnd::OnSetText(LPCSTR lpszText)
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: OnSetFont
// PURPOSE:  Processes the WM_SETFONT message.
//
void CPictureWnd::OnSetFont(HFONT hFont, BOOL fRedraw)
{
   MEMBERASSERT();
   m_hFont = hFont;
   if (fRedraw && m_hWnd)
      InvalidateRect(m_hWnd, NULL, TRUE);
}

//===============================================================================================
// FUNCTION: OnGetFont
// PURPOSE:  Processes the WM_GETFONT message.
//   
HFONT CPictureWnd::OnGetFont()
{
   MEMBERASSERT();
   return m_hFont ? m_hFont : GetStockFont(SYSTEM_FONT);
}

//===============================================================================================
// FUNCTION: OnDraw
// PURPOSE:  Virtual function called by OnPaint to do the actual drawing.
//
void CPictureWnd::OnDraw(HDC hDC)
{
   MEMBERASSERT();
   FillRect(hDC, &m_rClient, GetStockBrush(LTGRAY_BRUSH));
}

//===============================================================================================
// FUNCTION: OnClose
// PURPOSE:  Processes the WM_CLOSE message.
//
void CPictureWnd::OnClose()
{
   MEMBERASSERT();
   if (m_hWnd)
      DestroyWindow(m_hWnd);
}

//===============================================================================================
// FUNCTION: OnDestroy
// PURPOSE:  Processes the WM_DESTROY message.
//
void CPictureWnd::OnDestroy()
{
   MEMBERASSERT();
   SETOBJPTR(m_hWnd, NULL);
   SetHWND(NULL);
}

//===============================================================================================
// FUNCTION: DefWindowProc
// PURPOSE:  Calls the default procedure for processing messages.
//
LRESULT CPictureWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   MEMBERASSERT();
   return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

//===============================================================================================
// FUNCTION: Register
// PURPOSE:  Registers the CPictureWndProc class.
// NOTES:    If a derived class overrides the Register() member function it must also 
//           have a unique class name provided by an overridden GetClassName() function.
//
BOOL CPictureWnd::Register(HINSTANCE hInstance)
{
   m_hInstance = hInstance;
      
   WNDCLASS cls = { 0 };
   if (GetClassInfo(hInstance, GetClassName(), &cls))
      return TRUE;

   cls.hCursor       = LoadCursor(NULL, IDC_ARROW);
   cls.hIcon         = NULL;
   cls.lpszMenuName  = NULL;
   cls.hInstance     = hInstance;
   cls.lpszClassName = GetClassName();
   cls.hbrBackground = NULL;   // (HBRUSH)(COLOR_WINDOW+1);
   cls.lpfnWndProc   = (WNDPROC)CPictureWnd::CPictureWndProc;
   cls.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
   cls.cbWndExtra    = sizeof(void *);
   cls.cbClsExtra    = 0;

   // Don't check the return value of RegisterClass as it may have already been
   // registered by another EXE or DLL in the same task.
   RegisterClass(&cls);
   return TRUE;
}

//===============================================================================================
// FUNCTION: Unregister
// PURPOSE:  Unregisters the CPictureWndProc class.
//
BOOL CPictureWnd::Unregister()
{
   // Do not unregister the class as it could be being used in other EXE or DLL within the same task.
//   if (!UnregisterClass(GetClassName(), m_hInstance))
//      return FALSE;

   return TRUE;
}

//===============================================================================================
// FUNCTION: CreateWnd
// PURPOSE:  Function for creating a Window associated with the CPictureWnd object.
//
BOOL CPictureWnd::CreateWnd(LPCSTR lpszTitle, DWORD dwStyle, RECT &rect, 
                            HWND hwndParent, int nWindowID, HINSTANCE hInstance,
                            DWORD dwExStyle)
{
   MEMBERASSERT();
   HWNDASSERT(hwndParent);
   ASSERT(m_hWnd==NULL);
      
   Register(hInstance);

   if (dwExStyle==0)
      ::CreateWindow( GetClassName(), lpszTitle, dwStyle, 
                     rect.left, rect.top, 
                     rect.right-rect.left, rect.bottom-rect.top,
                     hwndParent, (HMENU)nWindowID, hInstance, this );
   else                
      ::CreateWindowEx( dwExStyle, GetClassName(), lpszTitle, dwStyle, 
                        rect.left, rect.top, 
                        rect.right-rect.left, rect.bottom-rect.top,
                        hwndParent, (HMENU)nWindowID, hInstance, this );

   if (!m_hWnd)
      Unregister();

   return (m_hWnd != NULL);
}

//===============================================================================================
// FUNCTION: DestroyWnd
// PURPOSE:  Destroys the window associated with the CPictureWnd object and resets the object.
//
BOOL CPictureWnd::DestroyWnd()
{
   MEMBERASSERT();
   if (!m_hWnd || !IsWindow(m_hWnd))
   {
      m_hWnd = NULL;
      return FALSE;
   }
      
   VERIFY(DestroyWindow(m_hWnd));
   return TRUE;
}

//===============================================================================================
// FUNCTION: ReplaceControl
// PURPOSE:  Function for replacing a place holder control in a dialog with a CPictureWnd object.
//
BOOL CPictureWnd::ReplaceControl(HWND hWnd, int nControlID)
{
   MEMBERASSERT();
   HWNDASSERT(hWnd);
   HWND hwndControl = GetDlgItem(hWnd, nControlID);
   if (!hwndControl)
      return NULL;

   // Grab the existing window text and font.      
   char szText[80];
   GetWindowText(hwndControl, szText, sizeof(szText));
   HFONT hFont = GetWindowFont(hwndControl);
   
   RECT rect;
   GetWindowRect(hwndControl, &rect);
   MapWindowRect(NULL, hWnd, &rect);
   DWORD dwStyle = WS_CHILD | WS_VISIBLE;
   int nWindowID = GetWindowID(hwndControl);
   HINSTANCE hInstance = GetWindowInstance(hWnd);

   if (!CreateWnd(szText, dwStyle, rect, hWnd, nWindowID, hInstance))
      return FALSE;
   if (hFont)
      SetWindowFont(m_hWnd, hFont, TRUE);
   DestroyWindow(hwndControl);
   return TRUE;
}


//***********************************************************************************************
// Message crackers for re-routing messages to virtual functions.

//===============================================================================================
// FUNCTION: _OnNCCreate
// PURPOSE:  Processes the WM_NCCREATE message.
//
inline BOOL _OnNCCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct)
{
   CPictureWnd *pObj = (CPictureWnd *)lpCreateStruct->lpCreateParams;
   WPTRASSERT(pObj);
   
   SETOBJPTR(hWnd, pObj);
   pObj->SetHWND(hWnd);
   
   return pObj->OnNCCreate(lpCreateStruct);
}

//===============================================================================================
// FUNCTION: _OnCreate
// PURPOSE:  Processes the WM_CREATE message.
//
inline BOOL _OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct)
{
   CPictureWnd *pObj = GETOBJPTR(hWnd);
   if (!pObj)
      return FALSE;
   return pObj->OnCreate(lpCreateStruct);
}

//===============================================================================================
// FUNCTION: _OnSize
// PURPOSE:  Processes the WM_SIZE message.
//
inline void _OnSize(HWND hWnd, UINT state, int cx, int cy)
{
   GETOBJPTR(hWnd)->OnSize(state, cx, cy);
}

//===============================================================================================
// FUNCTION: _OnSetText
// PURPOSE:  Processes the WM_SETTEXT message.
//
inline void _OnSetText(HWND hWnd, LPCSTR lpszText)
{
   GETOBJPTR(hWnd)->OnSetText(lpszText);
}

//===============================================================================================
// FUNCTION: _OnSetFont
// PURPOSE:  Processes the WM_SETFONT message.
//
inline void _OnSetFont(HWND hWnd, HFONT hFont, BOOL fRedraw)
{
   GETOBJPTR(hWnd)->OnSetFont(hFont, fRedraw);
}

//===============================================================================================
// FUNCTION: _OnGetFont
// PURPOSE:  Processes the WM_GETFONT message.
//
inline HFONT _OnGetFont(HWND hWnd)
{
   return GETOBJPTR(hWnd)->OnGetFont();
}

//===============================================================================================
// FUNCTION: _OnPaint
// PURPOSE:  Processes the WM_PAINT message.
//
inline void _OnPaint(HWND hWnd)
{
   PAINTSTRUCT ps;
   HDC   hDC   = BeginPaint(hWnd, &ps);
   CPictureWnd *pObj = GETOBJPTR(hWnd);

   if (pObj)
   {
      HFONT hFont = SelectFont(hDC, pObj->OnGetFont());
      pObj->OnDraw(hDC);
      SelectFont(hDC, hFont);
   }
   EndPaint(hWnd, &ps);
}

//===============================================================================================
// FUNCTION: _OnClose
// PURPOSE:  Processes the WM_CLOSE message.
//
inline void _OnClose(HWND hWnd)
{
   GETOBJPTR(hWnd)->OnClose();
}

//===============================================================================================
// FUNCTION: _OnDestroy
// PURPOSE:  Processes the WM_DESTROY message.
//
inline void _OnDestroy(HWND hWnd)
{
   GETOBJPTR(hWnd)->OnDestroy();
}

//===============================================================================================
// FUNCTION: _OnOtherMessage
// PURPOSE:  Processes the messages not processed by the other message functions.
//
inline LRESULT _OnOtherMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   CPictureWnd *pObj = GETOBJPTR(hWnd);
   if (pObj)
      return pObj->DefWindowProc(uMsg, wParam, lParam);
   return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//==============================================================================================
// FUNCTION: CPictureWndProc
// PURPOSE:  Dialog proc for the window.
//
LRESULT CALLBACK CPictureWnd::CPictureWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   HWNDASSERT( hWnd );
   switch(uMsg)
   {
      HANDLE_MSG(hWnd, WM_NCCREATE, _OnNCCreate);
      HANDLE_MSG(hWnd, WM_CREATE,   _OnCreate);
      HANDLE_MSG(hWnd, WM_SIZE,     _OnSize);
      HANDLE_MSG(hWnd, WM_SETTEXT,  _OnSetText);
      HANDLE_MSG(hWnd, WM_SETFONT,  _OnSetFont);
      HANDLE_MSG(hWnd, WM_GETFONT,  _OnGetFont);
      HANDLE_MSG(hWnd, WM_PAINT,    _OnPaint);
      HANDLE_MSG(hWnd, WM_CLOSE,    _OnClose);
      HANDLE_MSG(hWnd, WM_DESTROY,  _OnDestroy);
      default:
         return _OnOtherMessage( hWnd, uMsg, wParam, lParam );
   }
}


