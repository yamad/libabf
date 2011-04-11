//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  TDrawWnd.CPP
// PURPOSE: Generic Threaded background drawing Window Class. 
// AUTHOR:  BHI  Feb 1999
//
#include "wincpp.hpp"
#include <process.h>

#include "TDrawWnd.hpp"
#include "WaitCursor.hpp"
#include "Clipboard.hpp"
#include "\AxonDev\Comp\Axoutils32\Axoutils32.h"

//===============================================================================================
// Macros for setting and getting the pointer to the acquisition object.
#define GWL_TDRAWWND 0      // Offset to CTDrawWnd pointer in Wnd extra bytes.
#define GETOBJPTR(hWnd)       ((CTDrawWnd *)GetWindowLong((hWnd), GWL_TDRAWWND))
#define SETOBJPTR(hWnd, pObj) (SetWindowLong((hWnd), GWL_TDRAWWND, long(pObj)))

// Screen refresh timer constants.
const UINT c_uTimerID     = 42;
const UINT c_uTimerPeriod = 200;

// Time to wait after suspending or resuming a thread.
const UINT c_uSuspendSleep = 0;

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Sets the m_hWnd variable and initializes the object.
//
CTDrawWnd::CTDrawWnd()
{
   MEMBERASSERT();
   m_hWnd        = NULL;
   m_hOldPalette = NULL;
   m_hdcPrint    = NULL;
   m_rClient.SetRectEmpty();
   m_rRedraw.SetRectEmpty();
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleans up on destruction of the object.
//
CTDrawWnd::~CTDrawWnd()
{
   MEMBERASSERT();
   
   // Destroy the window if it exists (benign if it does not).
   DestroyWnd();
}

//===============================================================================================
// FUNCTION: SetHWND
// PURPOSE:  Set associated window handle.
//
void CTDrawWnd::SetHWND(HWND hWnd)
{
   MEMBERASSERT();
   m_hWnd = hWnd;
}

//===============================================================================================
// FUNCTION: GetHWND
// PURPOSE:  Get the HWND associated with the object.
//
HWND CTDrawWnd::GetHWND() const
{
   MEMBERASSERT();
   return m_hWnd;
}

//===============================================================================================
// FUNCTION: SetBackgroundColor
// PURPOSE:  Sets the color that the background will get cleared to.
//
void CTDrawWnd::SetBackgroundColor(COLORREF rgb)
{
   MEMBERASSERT();
   m_Surface.SetBackgroundColor(rgb);
   Redraw();
}

//===============================================================================================
// FUNCTION: GetBackgroundColor
// PURPOSE:  Returns the color that the background will get cleared to.
//
COLORREF CTDrawWnd::GetBackgroundColor() const
{
   MEMBERASSERT();
   return m_Surface.GetBackgroundColor();
}

//===============================================================================================
// FUNCTION: GetClassName
// PURPOSE:  Return a pointer to the name of the window class.
// NOTES:    If a derived class overrides the Register() member function it must also 
//           have a unique class name provided by an overridden GetClassName() function.
//
LPCSTR CTDrawWnd::GetClassName()
{
   MEMBERASSERT();
   return "AXO_CTDrawWndClass32";
}

//===============================================================================================
// FUNCTION: OnNCCreate
// PURPOSE:  Processes the WM_NCCREATE message.
//
bool CTDrawWnd::OnNCCreate(LPCREATESTRUCT lpCreateStruct)
{
   MEMBERASSERT();
   m_Surface.SetWinDC(GetDC(m_hWnd));
   return !!DefWindowProc(WM_NCCREATE, 0, LPARAM(lpCreateStruct));
}

//===============================================================================================
// FUNCTION: OnCreate
// PURPOSE:  Processes the WM_CREATE message.
//
bool CTDrawWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   MEMBERASSERT();
   GetClientRect( m_hWnd, &m_rClient );

#if BACKGROUND_THREADED
   // Start the heartbeat timer that drives the drawing.
   VERIFY(SetTimer(m_hWnd, c_uTimerID, c_uTimerPeriod, NULL));
   VERIFY(m_Thread.Create(this));
#endif

   return true;
}

//===============================================================================================
// FUNCTION: StopDrawing
// PURPOSE:  Stop the thread redrawing.
//
void CTDrawWnd::StopDrawing()
{
   MEMBERASSERT();
#if BACKGROUND_THREADED
   m_Thread.Suspend();
#endif
}

//===============================================================================================
// FUNCTION: OnSize
// PURPOSE:  Processes the WM_SIZE message.
//
void CTDrawWnd::OnSize()
{
   MEMBERASSERT();
   StopDrawing();

   // Resize the off-screen bitmap.
   GetClientRect( m_hWnd, &m_rClient );

   m_Surface.Resize(&m_rClient);
   DrawBackground(&m_rClient);

   // Start the background thread redrawing.
   Redraw();
}

//===============================================================================================
// FUNCTION: OnPaint
// PURPOSE:  Processes the WM_PAINT message.
//
void CTDrawWnd::OnPaint()
{
   MEMBERASSERT();
   PAINTSTRUCT ps = { 0 };
   BeginPaint(m_hWnd, &ps);

   m_Surface.SetDirty();
   m_Surface.Update(&ps.rcPaint);

   EndPaint(m_hWnd, &ps);
}

//===============================================================================================
// FUNCTION: OnEraseBkgnd
// PURPOSE:  Processes the WM_ERASEBKGND message.
//
bool CTDrawWnd::OnEraseBkgnd()
{
   MEMBERASSERT();
#if BACKGROUND_THREADED
   if (m_Thread.IsSuspended() && !m_rRedraw.IsRectEmpty())
   {
      m_Thread.Suspend();
      DrawBackground(&m_rRedraw);
      m_Thread.Resume();
   }
#endif
   return false;
}

//===============================================================================================
// FUNCTION: Redraw
// PURPOSE:  Start the secondary thread redrawing.
//
void CTDrawWnd::Redraw(LPCRECT prRedraw)
{
   MEMBERASSERT();
#if BACKGROUND_THREADED
   m_Thread.Suspend();
   InvalidateRect(prRedraw);
   m_Thread.Resume();
#else
   InvalidateRect(prRedraw);
   DrawProc();
#endif
}

//===============================================================================================
// FUNCTION: InvalidateRect
// PURPOSE:  Adds the given rectangle to the redraw region.
//
void CTDrawWnd::InvalidateRect(LPCRECT prInvalid)
{
   MEMBERASSERT();
   if (!m_hWnd)
      return;

   CRect rInvalid = prInvalid ? prInvalid : m_rClient;
   rInvalid.IntersectRect(rInvalid, m_rClient);
   if (m_rRedraw.IsRectEmpty())
      m_rRedraw = rInvalid;
   else
      m_rRedraw.UnionRect(m_rRedraw, rInvalid);
   ::InvalidateRect(m_hWnd, rInvalid, true);
}

//===============================================================================================
// FUNCTION: WaitUntilDone
// PURPOSE:  Wait until drawing has finished.
//
void CTDrawWnd::WaitUntilDone()
{
   MEMBERASSERT();
#if BACKGROUND_THREADED
   VERIFY(m_Thread.WaitUntilDone());
#endif
}

//===============================================================================================
// FUNCTION: IsDone
// PURPOSE:  .
//
bool CTDrawWnd::IsDone() const
{
   MEMBERASSERT();
   
#if BACKGROUND_THREADED
   return m_Thread.IsSuspended();
#else
   return true;
#endif
}

//===============================================================================================
// FUNCTION: InThread
// PURPOSE:  Tests if the caller is the background thread.
//
bool CTDrawWnd::InThread() const
{
   MEMBERASSERT();
   
#if BACKGROUND_THREADED
   return m_Thread.IsCurrentThread();
#else
   return false;
#endif
}

//===============================================================================================
// FUNCTION: SetThreadPriority
// PURPOSE:  .
//
bool CTDrawWnd::SetThreadPriority(int nPriority)
{
   MEMBERASSERT();
#if BACKGROUND_THREADED
   return m_Thread.SetThreadPriority(nPriority);
#else
   return true;
#endif
}

//===============================================================================================
// FUNCTION: Lock
// PURPOSE:  Lock the display surface and return the DC.
//
HDC CTDrawWnd::Lock()
{
   MEMBERASSERT();

#if BACKGROUND_THREADED
   // If called from the thread and the suspended flag has been set, 
   // throw an exception to interrupt drawing and suspend the thread.
   if (InThread() && m_Thread.IsSuspended())
      throw CTDrawWndException();
#endif
   return m_hdcPrint ? m_hdcPrint : m_Surface.Lock();
}

//===============================================================================================
// FUNCTION: Unlock
// PURPOSE:  Unlock the display surface and mark it dirty for updating to the screen.
//
void CTDrawWnd::Unlock()
{
   MEMBERASSERT();
   if (m_hdcPrint)
      return;
   m_Surface.SetDirty();
   m_Surface.Unlock();
}

//===============================================================================================
// FUNCTION: Relock
// PURPOSE:  Unlocks the surface for a short period and then relocks it.
//
HDC CTDrawWnd::Relock()
{
   MEMBERASSERT();

   if (!m_hdcPrint)      
      ASSERT(m_Surface.GetLockCount()==1);

   Unlock();
   Sleep(0);
   return Lock();
}

//===============================================================================================
// FUNCTION: OnTimer
// PURPOSE:  Called periodically by a windows timer.
//
void CTDrawWnd::OnTimer()
{
   MEMBERASSERT();

#if BACKGROUND_THREADED
   if (!m_rRedraw.IsRectEmpty())
   {
      // Update the screen if the surface is dirty.
      m_Surface.Update(&m_rRedraw);
      if (IsDone())
         m_rRedraw.SetRectEmpty();
   }
#endif
}

//===============================================================================================
// FUNCTION: OnDestroy
// PURPOSE:  Processes the WM_DESTROY message.
//
void CTDrawWnd::OnDestroy()
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);

   if (m_hOldPalette)
      ::SelectPalette(GetDC(m_hWnd), m_hOldPalette, false);

#if BACKGROUND_THREADED
   VERIFY(m_Thread.End());
   KillTimer(m_hWnd, c_uTimerID);
#endif

   SETOBJPTR(m_hWnd, NULL);
   SetHWND(NULL);
}

//===============================================================================================
// FUNCTION: Register
// PURPOSE:  Registers the CTDrawWndProc class.
// NOTES:    Register must be called prior to creating any windows.
//
bool CTDrawWnd::Register(HINSTANCE hInstance)
{
   // The dialog proc function.
   LRESULT CALLBACK TDrawWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

   WNDCLASS cls = { 0 };
   if (GetClassInfo(hInstance, GetClassName(), &cls))
      return TRUE;

   cls.hCursor       = LoadCursor(NULL, IDC_ARROW);
   cls.hIcon         = NULL;
   cls.lpszMenuName  = NULL;
   cls.hInstance     = hInstance;
   cls.lpszClassName = GetClassName();
   cls.hbrBackground = NULL;   // (HBRUSH)(COLOR_WINDOW+1);
   cls.lpfnWndProc   = TDrawWndProc;
   cls.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;
   cls.cbWndExtra    = sizeof(void *);
   cls.cbClsExtra    = 0;

   // Don't check the return value of RegisterClass as it may have already been
   // registered by another EXE or DLL in the same task.
   RegisterClass(&cls);
   return true;
}

//===============================================================================================
// FUNCTION: Unregister
// PURPOSE:  Unregisters the CTDrawWnd class.
//
bool CTDrawWnd::Unregister(HINSTANCE hInstance)
{
   // Do not unregister the class as it could be being used in other EXE or DLL within the same task.
   return true; // UnregisterClass(GetClassName(), hInstance) ? true : false;
}

//===============================================================================================
// FUNCTION: CreateWnd
// PURPOSE:  Function for creating a Window associated with the CTDrawWnd object.
//
bool CTDrawWnd::CreateWnd(LPCSTR pszTitle, DWORD dwStyle, LPCRECT pRect, 
                          HWND hwndParent, int nWindowID, HINSTANCE hInstance,
                          DWORD dwExStyle)
{
   MEMBERASSERT();
   HWNDASSERT(hwndParent);
   ASSERT(m_hWnd==NULL);
      
   Register(hInstance);

   ::CreateWindowEx( dwExStyle, GetClassName(), pszTitle, dwStyle, 
                     pRect->left, pRect->top, 
                     pRect->right-pRect->left, pRect->bottom-pRect->top,
                     hwndParent, (HMENU)nWindowID, hInstance, this );

   // If the WM_NCCREATE processing has not set the window handling, we are in trouble.
   return IsWindow(m_hWnd) ? true : false;
}

//===============================================================================================
// FUNCTION: DestroyWnd
// PURPOSE:  Destroys the window associated with the CTDrawWnd object and resets the object.
//
bool CTDrawWnd::DestroyWnd()
{
   MEMBERASSERT();
   if (!m_hWnd || !IsWindow(m_hWnd))
   {
      m_hWnd = NULL;
      return false;
   }
      
   VERIFY(DestroyWindow(m_hWnd));
   return true;
}

//===============================================================================================
// FUNCTION: ReplaceControl
// PURPOSE:  Function for replacing a place holder control in a dialog with a CTDrawWnd object.
//
bool CTDrawWnd::ReplaceControl(HWND hWnd, int nControlID)
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
   
   CRect rect;
   GetWindowRect(hwndControl, &rect);
   MapWindowRect(NULL, hWnd, &rect);
   DWORD dwStyle = WS_CHILD | WS_VISIBLE;
   int nWindowID = GetWindowID(hwndControl);
   HINSTANCE hInstance = GetWindowInstance(hWnd);

   if (!CreateWnd(szText, dwStyle, &rect, hWnd, nWindowID, hInstance))
      return false;
   if (hFont)
      SetWindowFont(m_hWnd, hFont, true);
   DestroyWindow(hwndControl);
   return true;
}

//===============================================================================================
// FUNCTION: OnPaletteChanged
// PURPOSE:  Processes the WM_PALETTECHANGED message.
//
void CTDrawWnd::OnPaletteChanged(HWND hwndFocus) 
{
   MEMBERASSERT();
	if (hwndFocus != m_hWnd)
		SelectPalette(true);
}

//===============================================================================================
// FUNCTION: OnQueryNewPalette
// PURPOSE:  Processes the WM_QUERYNEWPALETTE message.
//
bool CTDrawWnd::OnQueryNewPalette() 
{
   MEMBERASSERT();
	return SelectPalette(false);
}

//===============================================================================================
// FUNCTION: SelectPalette
// PURPOSE:  Selects the surface's palette into the device as appropriate for foreground or background.
//
bool CTDrawWnd::SelectPalette(bool bBackground)
{
   MEMBERASSERT();
	HPALETTE hPalette = m_Surface.GetPalette();
   if (!hPalette)
		return false;

   HPALETTE hOldPal = ::SelectPalette(GetDC(m_hWnd), hPalette, bBackground);
	if (::RealizePalette(GetDC(m_hWnd)) > 0)
      ::InvalidateRect(m_hWnd, NULL, true);

   if (m_hOldPalette==NULL)
      m_hOldPalette = hOldPal;

	return true;
}

//===============================================================================================
// FUNCTION: GetBitmap
// PURPOSE:  .
//
BYTE *CTDrawWnd::GetBitmap(BITMAPINFOHEADER *pBIH)
{
   MEMBERASSERT();
   return m_Surface.GetBitmap(pBIH);
}

//===============================================================================================
// FUNCTION: WindowProc
// PURPOSE:  Processes window messages.
//
LRESULT CTDrawWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   MEMBERASSERT();

   switch(uMsg)
   {
      case WM_NCCREATE:
         return OnNCCreate(LPCREATESTRUCT(lParam));

      case WM_CREATE:
         return OnCreate(LPCREATESTRUCT(lParam));

      case WM_TIMER:
         OnTimer();
         return 0;

      case WM_PAINT:
         OnPaint();
         return 0;

      case WM_ERASEBKGND:
         return OnEraseBkgnd();

      case WM_PALETTECHANGED:
         OnPaletteChanged(HWND(wParam));
         return 0;

      case WM_QUERYNEWPALETTE:
         return OnQueryNewPalette();

      case WM_DESTROY:
         OnDestroy();
         return 0;

      case WM_DISPLAYCHANGE:
         OnDisplayChange();
         // FALL THROUGH to WM_SIZE case.
      case WM_SIZE:
         OnSize();
         return 0;

      default:
         return DefWindowProc(uMsg, wParam, lParam);
   }
}

//===============================================================================================
// FUNCTION: WindowProc
// PURPOSE:  Processes window messages.
//
LRESULT CTDrawWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   MEMBERASSERT();
   return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

//===============================================================================================
// FUNCTION: DrawProc
// PURPOSE:  The drawing proc called by the thread.
//
void CTDrawWnd::DrawProc()
{
   MEMBERASSERT();
   if (!m_rRedraw.IsRectEmpty())
   {
      // Take a copy of the rect in case it is changed by the foreground thread.
      CRect rRedraw = m_rRedraw;
      Draw(rRedraw);
   }
}

//===============================================================================================
// FUNCTION: RenderToDC
// PURPOSE:  Render to a different DC.
//
void CTDrawWnd::RenderToDC(HDC hDC, LPCRECT prDraw, BOOL bBackground)
{
   MEMBERASSERT();

   // Wait for the background thread to finish.
   WaitUntilDone();

   m_hdcPrint = hDC;
   if (bBackground)
      DrawBackground(prDraw);
   Draw(prDraw);
   m_hdcPrint = NULL;
}

//=============================================================================
// FUNCTION: CopyToMetaFile
// PURPOSE:  Copies the GW to a metafile
//
BOOL CTDrawWnd::CopyAsMetaFile()
{
   MEMBERASSERT();
   CWaitCursor WaitCursor;

   Clipboard CB;
   if (!CB.Open(m_hWnd) || !CB.Empty())
      return false;

   // Copy as ENHANCED METAFILE
   HDC hMetaDC = CreateEnhMetaFile(NULL, NULL, NULL, NULL);      
   if (!hMetaDC)
      return false;

   // Give a bogus rectangle so that the system does not have to guess the 
   // initial dimentions of this metafile.
   // This is for the backward compatibility of the Windows metafiles (WMF).
   CRect rClient = m_rClient;
   rClient.InflateRect( 2, 2 );
   AXU_HollowRectangle( hMetaDC, rClient, GetStockPen(NULL_PEN) );

   RenderToDC(hMetaDC, m_rClient);
  
   HENHMETAFILE hMeta = CloseEnhMetaFile(hMetaDC);
   BOOL bOK = CB.SetData(CF_ENHMETAFILE, (HANDLE)hMeta);
   if (!bOK)
      DeleteEnhMetaFile(hMeta);

   return bOK;
}

//=============================================================================
// FUNCTION: CopyAsBitmap
// PURPOSE:  Copies the graph to the clipboard
//
BOOL CTDrawWnd::CopyAsBitmap()
{                                                         
   MEMBERASSERT();
   CWaitCursor WaitCursor;

   Clipboard CB;
   if (!CB.Open(m_hWnd) || !CB.Empty())
      return false;

   // Copy as ENHANCED METAFILE
   HDC hMetaDC = CreateEnhMetaFile(NULL, NULL, NULL, NULL);      
   if (!hMetaDC)
      return false;

   // Render to the metafile DC.
   m_Surface.BitBlt(hMetaDC);

   HENHMETAFILE hMeta = CloseEnhMetaFile(hMetaDC);

   BOOL bOK = CB.SetData(CF_ENHMETAFILE, (HANDLE)hMeta);
   if (!bOK)
      DeleteEnhMetaFile(hMeta);

   return bOK;
}

//==============================================================================================
// FUNCTION: CTDrawWndProc
// PURPOSE:  Dialog proc for the window.
//
LRESULT CALLBACK TDrawWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   HWNDASSERT( hWnd );

   if (uMsg==WM_NCCREATE)
   {
      LPCREATESTRUCT lpCreateStruct = LPCREATESTRUCT(lParam);
      CTDrawWnd *pObj = (CTDrawWnd *)lpCreateStruct->lpCreateParams;
      if (!pObj)
         return false;
      WPTRASSERT(pObj);
      SETOBJPTR(hWnd, pObj);
      pObj->SetHWND(hWnd);
   }

   CTDrawWnd *pObj = GETOBJPTR(hWnd);
   if (pObj)
      return pObj->WindowProc(uMsg, wParam, lParam);

   return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//###############################################################################################
//###############################################################################################
//###
//###    Thread class implimentation.
//###
//###############################################################################################
//###############################################################################################


//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Sets the m_hWnd variable and initializes the object.
//
CDrawThread::CDrawThread()
   : m_SuspendAckEvent(false, true), m_SuspendEvent(false, false)
{
   MEMBERASSERT();
   m_pClient    = NULL;
   m_bNeeded    = true;
   m_bSuspended = true;
   m_bInDrawing = false;
   m_hThread    = NULL;
   m_uThreadID  = 0;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleans up on destruction of the object.
//
CDrawThread::~CDrawThread()
{
   MEMBERASSERT();
   ASSERT(m_hThread==NULL);
}

//===============================================================================================
// FUNCTION: SetThreadPriority
// PURPOSE:  Changes the priority of the thread.
//
bool CDrawThread::SetThreadPriority(int nPriority)
{
   MEMBERASSERT();
   return ::SetThreadPriority( m_hThread, nPriority ) ? true : false;
}

//===============================================================================================
// FUNCTION: IsSuspended
// PURPOSE:  Returns the status of the suspend flag.
//
bool CDrawThread::IsSuspended() const
{
   MEMBERASSERT();
   return m_bSuspended;
}

//===============================================================================================
// FUNCTION: IsCurrentThread
// PURPOSE:  Checks whether the current thread is this one.
//
bool CDrawThread::IsCurrentThread() const
{
   MEMBERASSERT();
   return GetCurrentThreadId() == m_uThreadID;
}

//===============================================================================================
// FUNCTION: WaitUntilNeeded
// PURPOSE:  Waits until the thread is required.
//
bool CDrawThread::WaitUntilNeeded()
{
   MEMBERASSERT();

   // Check if the thread should be suspended.
   // TRACE("== Thread suspended ==.\n");
   m_bSuspended = true;
   m_SuspendAckEvent.SetEvent();
   m_SuspendEvent.Lock();
   // TRACE("== Thread resumed ==.\n");
   return m_bNeeded;
}

//===============================================================================================
// FUNCTION: Resume
// PURPOSE:  Resume the thread from its initial slumber, or its suspended state.
//
bool CDrawThread::Resume()
{
   MEMBERASSERT();
   // TRACE("ResumeThread()\n");
   ASSERT(m_bSuspended==true);
   m_bSuspended = false;
   m_SuspendAckEvent.ResetEvent();
   m_SuspendEvent.SetEvent();

   // A little bit of sleep here allows the first portion of the draw code (up to the first Relock())
   // to get done before it is interrupted. This looks better when the window is being drag-sized.
   Sleep(c_uSuspendSleep);
   return true;
}

//===============================================================================================
// FUNCTION: Suspend
// PURPOSE:  Suspend the thread so that it does not chew up CPU cycles when not needed.
//
bool CDrawThread::Suspend()
{
   MEMBERASSERT();
   // TRACE("SuspendThread()\n");
   if (!m_bSuspended)
   {
      // Set the suspend flag to prevent stacked suspend calls.
      m_bSuspended = true;

      // Wait for confirmation from the thread.
      m_SuspendAckEvent.Lock();
      ASSERT(m_bSuspended != m_bInDrawing);

      // As there is no object that is signaled when the thread actually goes into suspension,
      // we need to sleep a bit to make sure that the thread actually made it into suspension.
      Sleep(c_uSuspendSleep);
   }
   return true;
}

//===============================================================================================
// FUNCTION: WaitUntilDone
// PURPOSE:  Waits until the thread goes into suspension on completion of drawing.
//
bool CDrawThread::WaitUntilDone()
{
   MEMBERASSERT();

   if (!m_bSuspended)
      VERIFY(m_SuspendAckEvent.Lock());

   ASSERT(m_bSuspended);
   ASSERT(!m_bInDrawing);

   return m_bSuspended;
}

//===============================================================================================
// FUNCTION: End
// PURPOSE:  Terminate the thread.
//
bool CDrawThread::End()
{
   MEMBERASSERT();
   // TRACE("End()\n");
   // Set the suicide flag and resume the thread.
   Suspend();
   ASSERT(m_bNeeded==true);
   m_bNeeded = false;
   Resume();

   // A thread becomes signalled when it terminates.
   DWORD dwRval = WaitForSingleObject(m_hThread, 2000);
   ASSERT(dwRval==WAIT_OBJECT_0);

   CloseHandle(m_hThread);
   m_hThread = NULL;
   return true;
}

//===============================================================================================
// FUNCTION: ThreadProc
// PURPOSE:  Thread procedure used to drive the data pump.
//
DWORD CDrawThread::ThreadProc()
{
   MEMBERASSERT();
   // TRACE("== Thread Started ==\n");

   // Wait until the thread is needed, suicide if not.
   while (WaitUntilNeeded())
   {
      m_bInDrawing = true;
      try
      {         
         m_pClient->DrawProc();         
      }
      catch (CTDrawWndException)
      {
      }
      catch (...)
      {
         ERRORMSG(">> Unexpected exception in CDrawThread.<<");
         throw;
      }
      m_bInDrawing = false;
   }

   // TRACE("== Thread terminated ==\n");
   return 0;
}

//===============================================================================================
// FUNCTION: ThreadProc
// PURPOSE:  Thread proxy procedure used to call the object version.
//
static UINT WINAPI ThreadProxy( LPVOID pvTDrawWnd )
{
   // No MEMBERASSERT() -- static function.
   CDrawThread *pThread = (CDrawThread *)pvTDrawWnd;
   return pThread->ThreadProc();
}

//===============================================================================================
// FUNCTION: Create
// PURPOSE:  Create a thread in a suspended state..
//
bool CDrawThread::Create(CTDrawWnd *pClient)
{
   MEMBERASSERT();
   // TRACE("== Thread Created ==\n");

   // Save the client pointer and set the state flags.
   m_pClient    = pClient;
   m_bNeeded    = true;
   m_bSuspended = true;

   // Create the thread in a running state so that it runs straight into suspension.
   // Use _beginthreadex so that clients can call runtime functions during drawing.
   // The cast to a HANDLE is OK -- MFC does this in CWinThread.
   ASSERT(m_hThread==NULL);
   DWORD dwStack   = 2048;
   m_uThreadID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, dwStack, ThreadProxy, this, CREATE_SUSPENDED, &m_uThreadID);
   // VERIFY_SYSTEM_CALL(SetPriorityClass( GetCurrentProcess(),  HIGH_PRIORITY_CLASS ));
   // VERIFY_SYSTEM_CALL(SetThreadPriority( m_hThread, THREAD_PRIORITY_BELOW_NORMAL ));
   // THREAD_PRIORITY_TIME_CRITICAL
   // THREAD_PRIORITY_HIGHEST      
   // THREAD_PRIORITY_ABOVE_NORMAL 
   // THREAD_PRIORITY_NORMAL       
   // THREAD_PRIORITY_BELOW_NORMAL 
   // THREAD_PRIORITY_IDLE         
   // THREAD_PRIORITY_LOWEST       

   if (!m_hThread)
   {
      SHOW_SYSTEM_ERROR(0);
      return false;
   }

   // Changes the priority of the thread.
   //VERIFY(SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL));

   VERIFY(m_SuspendAckEvent.ResetEvent());

   // Start the thread and let it suspend itself.
   ::ResumeThread(m_hThread);
   VERIFY(m_SuspendAckEvent.Lock());

   // Wait a bit more just to be sure.
   Sleep(c_uSuspendSleep);
   return true;
}
