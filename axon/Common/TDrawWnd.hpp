//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  TDrawWnd.HPP
// PURPOSE: Contains class definition for the generic CTDrawWin class.
// AUTHOR:  BHI  Feb 1999
//

#ifndef INC_TDRAWWND_HPP
#define INC_TDRAWWND_HPP

#include "AfxUtils.hpp"
#include "AfxMT.hpp"
#include "DisplaySurface.hpp"

class CTDrawWnd;

// Something to throw to abort drawing.
class CTDrawWndException
{
};

#ifndef BACKGROUND_THREADED
#define BACKGROUND_THREADED 1
#endif

//====================================================================================================
// CLASS:   CDrawThread
// PURPOSE: Encapsulates a Win32 thread.
//
class CDrawThread
{
protected:
   CTDrawWnd        *m_pClient;     // Client object that does the drawing.
   bool              m_bNeeded;     // Set this flag FALSE to kill the thread.
   bool              m_bSuspended;  // true if thread is suspended.
   bool              m_bInDrawing;  // true if we're in client's drawing code
   HANDLE            m_hThread;     // The handle to the thread.
   UINT              m_uThreadID;   // ID of the background thread.
   CEvent            m_SuspendEvent;// Event that is used to suspend the thread.
   CEvent            m_SuspendAckEvent;// Event that is signaled when the thread suspends.

private:    // Unimplemented member functions.
   CDrawThread(const CDrawThread &);
   const CDrawThread &operator=(const CDrawThread &);

public:
   // Construction/destruction
   CDrawThread();
   ~CDrawThread();
   
   // Create a thread in a suspended state..
   bool Create(CTDrawWnd *pClient);

   // Changes the priority of the thread.
   bool SetThreadPriority(int nPriority);

   // Waits until the thread is required.
   bool WaitUntilNeeded();

   // Resume the thread from its initial slumber, or its suspended state.
   bool Resume();

   // Suspend the thread so that it does not chew up CPU cycles when not needed.
   bool Suspend();

   // Returns the status of the suspend flag.
   bool IsSuspended() const;

   // Checks whether the current thread is this one.
   bool IsCurrentThread() const;

   // Waits until the thread goes into suspension on completion of drawing.
   bool WaitUntilDone();

   // Terminate the thread.
   bool End();

   // Thread procedure used to drive the data pump.
   DWORD ThreadProc();
};

//====================================================================================================
// CLASS:   CTDrawWnd
// PURPOSE: Provides a generic window class class.
//
class CTDrawWnd
{
protected:
   HWND            m_hWnd;          // The window handle of the attached window
   CRect           m_rClient;       // Size & position of the client area (maintained through OnSize).
   CRect           m_rRedraw;       // Rectangle of region to be redrawn.
   HPALETTE        m_hOldPalette;   // Save the original palette in the hDC.
   HDC             m_hdcPrint;      // Printing device context.
   CDisplaySurface m_Surface;       // The off-screen display surface.
#if BACKGROUND_THREADED
   CDrawThread     m_Thread;        // The thread object.
#endif

private:    // Unimplemented member functions.
   CTDrawWnd(const CTDrawWnd &);
   const CTDrawWnd &operator=(const CTDrawWnd &);

protected:
   // Register the window class.
   virtual bool Register(HINSTANCE hInstance);
   virtual bool Unregister(HINSTANCE hInstance);
   virtual LPCSTR GetClassName();

   bool     SetThreadPriority(int nPriority);
   void     SetInverted(bool bInverted)      { m_Surface.SetInverted(bInverted);      }

   void     SetBackgroundColor(COLORREF rgb);
   COLORREF GetBackgroundColor() const;

   bool SelectPalette(bool bBackground);

   // Message functions.   
   virtual bool OnNCCreate(LPCREATESTRUCT pCreateStruct);
   virtual bool OnCreate(LPCREATESTRUCT pCreateStruct);
   virtual void OnDisplayChange() {};
   virtual void OnSize();

   virtual void OnPaint();
   virtual bool OnEraseBkgnd();
   virtual void OnPaletteChanged(HWND hwndFocus);
   virtual bool OnQueryNewPalette();
   virtual void OnTimer();
   virtual void OnDestroy();

   // Default message proc that gets called if nothing else handles a message.
   virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

   friend class CDrawThread;
   virtual void Draw(LPCRECT prPaint) = 0;
   virtual void DrawBackground(LPCRECT prPaint) {};

public:
   // Construction/destruction
   CTDrawWnd();
   virtual ~CTDrawWnd();
   
   // Set/Get the associated window handle  (internal function called during creation).
   void SetHWND(HWND hWnd);
   HWND GetHWND() const;

   // Create the window.
   bool CreateWnd(LPCSTR pszTitle, DWORD dwStyle, LPCRECT pRect, HWND hwndParent, 
                  int nWindowID, HINSTANCE hInstance, DWORD dwExStyle=0);

   // Destroy the window.
   bool DestroyWnd();
                  
   // Replace a place holder control.
   bool ReplaceControl(HWND hWnd, int nControlID);

   // Internal functions to get a DC.
   virtual HDC  Lock();
   virtual void Unlock();
   virtual HDC  Relock();

   // Start the thread redrawing.
   void Redraw(LPCRECT prRedraw=NULL);

   // Stop the thread redrawing.
   void StopDrawing();

   // Adds the given rectangle to the redraw region.
   void InvalidateRect(LPCRECT prInvalid=NULL);

   // Wait until drawing has finished.
   void WaitUntilDone();
   bool IsDone() const;

   // Tests if the caller is the background thread.
   bool InThread() const;

   // Render to a different DC.
   void RenderToDC(HDC hDC, LPCRECT prDraw, BOOL bBackground=FALSE);

   // Return the background DIB bits.
   BYTE *GetBitmap(BITMAPINFOHEADER *pBIH);

   // Copy to clipboard functions.
   BOOL CopyAsMetaFile();
   BOOL CopyAsBitmap();

   // The main message proc.
   virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

   // The drawing proc called by the thread.
   virtual void DrawProc();
};

#endif    // INC_TDRAWWND_HPP
