////////////////////////////////////////////////////////////////
// AXOTRACE Copyright 1995-1996 Microsoft Systems Journal. 
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// AXOTRACE is a development tool that displays MFC TRACE output in
// a window, even if you don't run under the debugger. To use AxoTrace,
// simply #include this file in your main .cpp file, the one that contains
// your CWinApp-derived app, and call 
//
//		MfxTraceInit();
//
// from your app's InitInstance function. You also need the AXOTRACE applet
// itself, which you can download from any MSJ bulletin board. NOTE: since 
// AxoTrace.h contains code, you should only #include it in once place in
// your app. (Don't #include it in stdafx.h.)
//
// NOTE: I would have defined a static instance to initialize MfxTrace, so
// calling MfxTraceInit would be unecessary, but there's a problem with the
// order of intializations. CFileTrace requires that afxDump be initialized
// first, which can fail to happen if CFileTrace is initialized as a static
// object.
//
// Modification history
// 4-1-95:   Created
// 10-12-95: Use WM_COPYDATA instead of atoms. Also note the name of this 
//           file is changed to AXOTRACE.H.
// 11-10-95: Added "Keep WIndow on Top" feature and fixed a bug 
//           relating to saved window pos.
// 11-15-95: Save/Restore position with CWindowPlacement.
// 12-05-95: Implemented tracer object with "new" instead of static, so
//           it remains around for reporting memory leaks, if any.
// 15-05-96: BHI Create version that uses Atoms so as to be compatible
//           with non-MFC use.

#ifndef __AXOTRACE_H__
#define __AXOTRACE_H__

#ifdef _WIN32	// AxoTrace only used for WIN32

#define AXOTRACE_CLASSNAME "AxonTraceWindow"
#define AXOTRACE_MESSAGE   "*AXON_TRACE_MSG"
#define AXOTRACE_SIGNATURE (*PDWORD(LPVOID("BHI ")))

#ifdef _DEBUG

//////////////////
// CFileTrace looks like a CFile, but "writes" to the trace window
//
class CFileTrace : public CFile 
{
private:
   CFileTrace() {}			// private constructor
   
public:
   virtual void Write(const void* lpBuf, UINT nCount);
   static void Init();
};

/////////////////
// Initialize tracing. Replace global afxDump.m_pFile with me.
//
void CFileTrace::Init()
{
   // This initialization function creates the only instance ever
   // of a CFileTrace object, and it's never deleted. This gurantees
   // that when MFC gets around to reporting memory leaks, they will
   // show up in the AXOTRACE window. If I were to instantiate CFileTrace 
   // as a static object, it would be destructed before the CRT gets
   // around to reporting memory leaks, and the information would be lost.
   
   static CFileTrace* pTracer;
   if (afxDump.m_pFile == NULL)
   {
      // Since I don't want this allocation to be reported as a leak,
      // turn off memory tracking
      BOOL bEnable = AfxEnableMemoryTracking(FALSE);
      afxDump.m_pFile = pTracer = new CFileTrace;
      AfxEnableMemoryTracking(bEnable);
      
   }
   else if (afxDump.m_pFile != pTracer)
      TRACE("afxDump is already using a file; AXOTRACE not installed.\n");
}

//////////////////
// Override to Write to AXOTRACE applet instead of file.
//
void CFileTrace::Write(const void *lpBuf, UINT uCount)
{
   if (!afxTraceEnabled)
      return;
   
   HWND hTraceWnd = FindWindow(AXOTRACE_CLASSNAME, NULL);
   if (hTraceWnd) 
   {
#ifdef USE_GLOBAL_ATOM
      static UINT WM_TRACE_MSG = RegisterWindowMessage(AXOTRACE_MESSAGE);
      
      // Found Trace window: send message there as a global atom.
      // Delete atom after use.
      //
      ATOM atom = GlobalAddAtom((LPCSTR)lpBuf);
      SendMessage(hTraceWnd,WM_TRACE_MSG, (WPARAM)atom, 0);
      GlobalDeleteAtom(atom);
#else
      // Found Trace window: send string with WM_COPYDATA
      //
      COPYDATASTRUCT cds;
      cds.dwData = AXOTRACE_SIGNATURE;
      cds.cbData = uCount;
      cds.lpData = (void*)lpBuf;
      SendMessage(hTraceWnd,WM_COPYDATA, NULL, LPARAM(&cds));
//      (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), 
#endif

   }
   else
   {
      // No trace window: do normal debug thing
      //
      ::OutputDebugString((LPCSTR)lpBuf);	
   }
}

#define MfxTraceInit CFileTrace::Init

#else  // Not _DEBUG

#define MfxTraceInit()

#endif // _DEBUG

#else  // Not _WIN32

#define MfxTraceInit()

#endif // _WIN32

#endif // __AXOTRACE_H__
