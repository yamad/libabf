//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Tooltip.CPP
// PURPOSE: Wrapper for the Win95 tooltip control.
// AUTHOR:  BHII  Jul 1998
//

#include "wincpp.hpp"
#include <string.h>

#include "Tooltip.hpp"

extern HINSTANCE g_hInstance;

#pragma comment(lib, "Comctl32.lib")

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CTooltip::CTooltip()
{
   MEMBERASSERT();
   m_hWnd          = NULL;
   m_hwndNotify    = NULL;
   m_uAddToolFlags = 0;

   static BOOL s_bInitialize = TRUE;
   if (s_bInitialize)
   {
      s_bInitialize = FALSE;
      InitCommonControls();
   }
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CTooltip::~CTooltip()
{
   MEMBERASSERT();
   Destroy();
}

//===============================================================================================
// FUNCTION: Create
// PURPOSE:  Creates the tooltip window.
//
BOOL CTooltip::Create(HWND hwndParent, DWORD dwStyle)
{
   ASSERT(m_hWnd==NULL);
   m_hwndNotify = hwndParent;
   m_hWnd = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, WS_POPUP | dwStyle, // force WS_POPUP
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           hwndParent, NULL, g_hInstance, NULL);
   return (m_hWnd != NULL);
}

//===============================================================================================
// FUNCTION: Destroy
// PURPOSE:  Destroys the tooltip window.
//
void CTooltip::Destroy()
{
   UnhookMessages();

   if (IsWindow(m_hWnd))
      DestroyWindow(m_hWnd);
   m_hWnd = NULL;
}

//===============================================================================================
// FUNCTION: SetToolInfo
// PURPOSE:  Sets the information that a tooltip maintains for a tool.
//
void CTooltip::SetToolInfo(LPTOOLINFO lpToolInfo)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   ::SendMessage(m_hWnd, TTM_SETTOOLINFO, 0, (LPARAM)lpToolInfo);
}

//===============================================================================================
// FUNCTION: RelayEvent
// PURPOSE:  Passes a mouse message to a tooltip control for processing.
//
void CTooltip::RelayEvent(LPMSG lpMsg)
{
   MEMBERASSERT();
   if (IsWindow(m_hWnd))
   {
      HWNDASSERT(m_hWnd);
      ::SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)lpMsg);
   }
}

//===============================================================================================
// FUNCTION: SetDelayTime
// PURPOSE:  Sets a delay time used by the control.
// PARAMETERS:
//    uDelay      time in ms.
//    uDuration   TTDT_AUTOMATIC, TTDT_AUTOPOP, TTDT_INITIAL, TTDT_RESHOW
//
void CTooltip::SetDelayTime(UINT uDuration, UINT nDelay)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   ::SendMessage(m_hWnd, TTM_SETDELAYTIME, uDuration, nDelay);
}

//===============================================================================================
// FUNCTION: GetToolCount
// PURPOSE:  Gets the number of tools registered.
//
int CTooltip::GetToolCount() const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   return (int) ::SendMessage(m_hWnd, TTM_GETTOOLCOUNT, 0, 0L);
}

//===============================================================================================
// FUNCTION: Activate
// PURPOSE:  Activates or de-activates a control.
//
void CTooltip::Activate(BOOL bActivate)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   ::SendMessage(m_hWnd, TTM_ACTIVATE, bActivate, 0);
}

//===============================================================================================
// FUNCTION: FillInToolInfo
// PURPOSE:  Helper function to fill in a TOOLINFO structure corectly.
//
void CTooltip::FillInToolInfo(CToolTipInfo &ti, HWND hWnd) const
{
   ti.hwnd   = m_hwndNotify;
   ti.uId    = (UINT)hWnd;
   ti.uFlags = m_uAddToolFlags | TTF_IDISHWND;
}

//===============================================================================================
// FUNCTION: FillInToolInfo
// PURPOSE:  Helper function to fill in a TOOLINFO structure corectly.
//
void CTooltip::FillInToolInfo(CToolTipInfo &ti, HWND hwndParent, UINT uIDTool, LPCRECT pRect) const
{
   ti.hwnd   = hwndParent;
   ti.uId    = uIDTool;
   ti.uFlags = m_uAddToolFlags;
   if (pRect)
      ti.rect = *pRect;
}

//===============================================================================================
// FUNCTION: AddTool
// PURPOSE:  Adds a tool to the tooltip control.
//
BOOL CTooltip::AddTool(TOOLINFO *pti)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   return (BOOL)::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)pti);
}

//===============================================================================================
// FUNCTION: AddTool
// PURPOSE:  Adds a tool to the tooltip control.
//
BOOL CTooltip::AddTool(HWND hWnd, LPCTSTR lpszText)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   CToolTipInfo ti;
   FillInToolInfo(ti, hWnd);
   ti.lpszText = (LPTSTR)lpszText;
   return AddTool(&ti);
}

//===============================================================================================
// FUNCTION: AddTool
// PURPOSE:  Adds a tool to the tooltip control.
//
BOOL CTooltip::AddTool(HWND hwndParent, UINT nIDTool, LPCRECT pRect, LPCTSTR lpszText)
{
   MEMBERASSERT();
   WPTRASSERT(pRect);
   
   CToolTipInfo ti;
   FillInToolInfo(ti, hwndParent, nIDTool, pRect);
   ti.lpszText = (LPTSTR)lpszText;
   return AddTool(&ti);
}

//===============================================================================================
// FUNCTION: AddTool
// PURPOSE:  Adds a tool to the tooltip control.
//
void CTooltip::DelTool(TOOLINFO *pti)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   ::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)pti);
}

//===============================================================================================
// FUNCTION: DelTool
// PURPOSE:  Deletes a tool from the tooltip control.
//
void CTooltip::DelTool(HWND hWnd)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   CToolTipInfo ti;
   FillInToolInfo(ti, hWnd);
   DelTool(&ti);
}

//===============================================================================================
// FUNCTION: DelTool
// PURPOSE:  Deletes a tool from the tooltip control.
//
void CTooltip::DelTool(HWND hwndParent, UINT nIDTool)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   CToolTipInfo ti;
   FillInToolInfo(ti, hwndParent, nIDTool);
   DelTool(&ti);
}

//===============================================================================================
// FUNCTION: Clear
// PURPOSE:  Clear out all tools.
//
void CTooltip::Clear()
{
   MEMBERASSERT();
   while (GetToolCount())
   {
      CToolTipInfo ti;
      if (!GetToolInfoByIndex(0, ti))
         break;
      DelTool(&ti);
   }
}

//===============================================================================================
// FUNCTION: GetText
// PURPOSE:  Gets the text associated with a tool.
//
void CTooltip::GetText(HWND hWnd, LPSTR pszText, UINT uTextLen) const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   ASSERT(uTextLen >= 256);

   CToolTipInfo ToolInfo;
   FillInToolInfo(ToolInfo, hWnd);
   ToolInfo.lpszText = ToolInfo.szText;

   ::SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM)&ToolInfo);
   strncpy(pszText, ToolInfo.szText, uTextLen);
   pszText[uTextLen] = '\0';
}

//===============================================================================================
// FUNCTION: GetText
// PURPOSE:  Gets the text associated with a tool.
//
void CTooltip::GetText(HWND hwndParent, UINT nIDTool, LPSTR pszText, UINT uTextLen) const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   ASSERT(uTextLen >= 256);

   CToolTipInfo ToolInfo;
   FillInToolInfo(ToolInfo, hwndParent, nIDTool);
   ToolInfo.lpszText = ToolInfo.szText;

   ::SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM)&ToolInfo);
   strncpy(pszText, ToolInfo.szText, uTextLen);
   pszText[uTextLen] = '\0';
}

//===============================================================================================
// FUNCTION: GetToolInfo
// PURPOSE:  Gets the full TOOLINFO associated with a tool.
//
BOOL CTooltip::GetToolInfo(HWND hWnd, CToolTipInfo& ToolInfo) const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   FillInToolInfo(ToolInfo, hWnd);
   ToolInfo.lpszText = ToolInfo.szText;
   return (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ToolInfo);
}

//===============================================================================================
// FUNCTION: GetToolInfo
// PURPOSE:  Gets the full TOOLINFO associated with a tool.
//
BOOL CTooltip::GetToolInfo(HWND hwndParent, UINT nIDTool, CToolTipInfo& ToolInfo) const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   FillInToolInfo(ToolInfo, hwndParent, nIDTool);
   ToolInfo.lpszText = ToolInfo.szText;
   return (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ToolInfo);
}

//===============================================================================================
// FUNCTION: GetToolInfoByIndex
// PURPOSE:  Gets the TOOLINFO associated with a tool by zero based index.
//
BOOL CTooltip::GetToolInfoByIndex(UINT uToolInx, CToolTipInfo& ToolInfo) const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   return (BOOL)::SendMessage(m_hWnd, TTM_ENUMTOOLS, uToolInx, (LPARAM)&ToolInfo);
}

//===============================================================================================
// FUNCTION: HitTest
// PURPOSE:  Test whether a point hits a tooltip item.
//
BOOL CTooltip::HitTest(HWND hWnd, POINT pt, LPTOOLINFO lpToolInfo) const
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   WPTRASSERT(lpToolInfo);

   TTHITTESTINFO hti = { 0 };
   hti.ti.cbSize     = sizeof(hti.ti);
   hti.hwnd          = hWnd;
   hti.pt            = pt;
   if (!::SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM)&hti))
      return FALSE;

   *lpToolInfo = hti.ti;
   return TRUE;
}

//===============================================================================================
// FUNCTION: SetToolRect
// PURPOSE:  Sets the active rectangle for an item.
//
void CTooltip::SetToolRect(HWND hwndParent, UINT nIDTool, LPCRECT pRect)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   CToolTipInfo ti;
   FillInToolInfo(ti, hwndParent, nIDTool, pRect);
   ::SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
}

//===============================================================================================
// FUNCTION: UpdateTipText
// PURPOSE:  Update the tip test for an item.
//
void CTooltip::UpdateTipText(HWND hWnd, LPCTSTR lpszText)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   CToolTipInfo ti;
   FillInToolInfo(ti, hWnd);
   ti.lpszText = (LPTSTR)lpszText;
   ::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

//===============================================================================================
// FUNCTION: UpdateTipText
// PURPOSE:  Update the tip test for an item.
//
void CTooltip::UpdateTipText(HWND hwndParent, UINT nIDTool, LPCTSTR lpszText)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   CToolTipInfo ti;
   FillInToolInfo(ti, hwndParent, nIDTool);
   ti.lpszText = (LPTSTR)lpszText;
   ::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

//===============================================================================================
// FUNCTION: SetMaxTipWidth
// PURPOSE:  Sets the maximum width of a tip before it gets broken into multiple lines.
//
int CTooltip::SetMaxTipWidth(int nWidth)
{
   MEMBERASSERT();
   return ::SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)nWidth);
}

//===============================================================================================
// FUNCTION: GetMaxTipWidth
// PURPOSE:  Returns the maximum tip width.
//
int CTooltip::GetMaxTipWidth()
{
   MEMBERASSERT();
   return ::SendMessage(m_hWnd, TTM_GETMAXTIPWIDTH, 0, 0);
}

//===============================================================================================
// FUNCTION: SetNotifyWnd
// PURPOSE:  Sets the notification window.
//
void CTooltip::SetNotifyWnd(HWND hwndNotify)
{
   MEMBERASSERT();
   m_hwndNotify = hwndNotify;
}

//===============================================================================================
// FUNCTION: GetNotifyWnd
// PURPOSE:  Returns the owning HWND.
//
HWND CTooltip::GetNotifyWnd() const
{
   MEMBERASSERT();
   return m_hwndNotify;
}

//===============================================================================================
// Global data for the message hook
//
static HHOOK     s_hMessageHook = NULL;
static CTooltip *s_pTooltip     = NULL;

//===============================================================================================
// FUNCTION: TOOLTIP_MessageFunc
// PURPOSE:  Callback function for the message hook.
//
static LRESULT CALLBACK TOOLTIP_MessageFunc(int nCode, WPARAM wParam, LPARAM lParam )
{
   // Pass the message on down the line.
   LRESULT lRval = CallNextHookEx(s_hMessageHook, nCode, wParam, lParam);

   // Post a message containing the key parameters.
   if ((nCode >= 0) && (s_pTooltip != NULL))
   {
      MSG *pMsg = (MSG *)lParam;

      // Only forward on the message if it belongs to our notification target or its decendants.
      HWND hwndNotify = s_pTooltip->GetNotifyWnd();
      if ((hwndNotify==pMsg->hwnd) || IsChild(hwndNotify, pMsg->hwnd))
      {   
         switch (pMsg->message)
         { 
            case WM_MOUSEMOVE: 
            case WM_LBUTTONDOWN: 
            case WM_LBUTTONUP: 
            case WM_RBUTTONDOWN: 
            case WM_RBUTTONUP: 
            {
               s_pTooltip->RelayEvent(pMsg);
               break; 
            }
         }
      }
   }

   return lRval;
}

//===============================================================================================
// FUNCTION: HookMessages
// PURPOSE:  Install filter function for the WH_GETMESSAGE
// NOTES:  * If tooltips are to be used in modeless dialog boxes, they need a message hook
//           in order to get at the messages needed to drive the tooltip functionality.
//
//         * HookMessages() should be called in response to WM_ACTIVATE with wParam!=WA_INACTIVE
//         * UnhookMessages() should be called in response to WM_ACTIVATE with wParam==WA_INACTIVE
//
//         * Normal windows do not need to install the message hook, they should use the RelayEvent()
//           call to forward on the following messages:
//           WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP.
//
BOOL CTooltip::HookMessages()
{
   if (s_hMessageHook)
      UnhookMessages();

   // Setup the message hook.
   s_hMessageHook = SetWindowsHookEx(WH_GETMESSAGE, TOOLTIP_MessageFunc, g_hInstance, 
                                     GetWindowThreadProcessId(m_hWnd, NULL));
   if (!s_hMessageHook)
      return FALSE;

   // Save a global pointer to this object.
   s_pTooltip = this;
   return TRUE;
}

//===============================================================================================
// FUNCTION: UnhookMessages
// PURPOSE:  Remove filter function for the WH_GETMESSAGE
// NOTES:    See HookMessages().
//
void CTooltip::UnhookMessages()
{
   // Clear the global pointer to this object.
   s_pTooltip = NULL;

   // Remove the message hook.
   if (s_hMessageHook)
      UnhookWindowsHookEx(s_hMessageHook);
   s_hMessageHook = NULL;
}


//===============================================================================================
// FUNCTION: SetAddToolFlags
// PURPOSE:  Sets the flags to be applied by AddTool.
//
void CTooltip::SetAddToolFlags(UINT uFlags)
{
   MEMBERASSERT();
   m_uAddToolFlags = uFlags;   
}

//===============================================================================================
// FUNCTION: GetAddToolFlags
// PURPOSE:  Gets the flags to be applied by AddTool.
//
UINT CTooltip::GetAddToolFlags() const
{
   MEMBERASSERT();
   return m_uAddToolFlags;
}

//===============================================================================================
// FUNCTION: GetHWND
// PURPOSE:  .
//
HWND CTooltip::GetHWND() const
{
   MEMBERASSERT();
   return m_hWnd;
}
