//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  ProgressDlg.CPP
// PURPOSE: 
// AUTHOR:  BHI  Sep 1999
//

#include "wincpp.hpp"
#include "ProgressDlg.hpp"
#include "\AxonDev\Comp\AxCommonDlg\AxCommonDlg.h"

extern HINSTANCE g_hInstance;

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CProgressDlg::CProgressDlg()
{
   MEMBERASSERT();
   m_hWnd = NULL;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CProgressDlg::~CProgressDlg()
{
   MEMBERASSERT();
   Close();
}

//===============================================================================================
// FUNCTION: Open
// PURPOSE:  Open the progress dialog.
// NOTES:    The window is initially hidden, and is shown after 1 second if the progress is less than 80%.
//
BOOL CProgressDlg::Open(HWND hwndNotify, LPCSTR pszTitle, UINT uFlags)
{
   MEMBERASSERT();

   ASSERT(m_hWnd==NULL);
   m_hWnd = APD_Open(hwndNotify, g_hInstance, pszTitle, "", uFlags);
   return m_hWnd != NULL;
}

//===============================================================================================
// FUNCTION: SetTitle
// PURPOSE:  .
//
void CProgressDlg::SetTitle(LPCSTR pszTitle)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   APD_SetTitle(m_hWnd, pszTitle);
}

//===============================================================================================
// FUNCTION: SetStaticText
// PURPOSE:  .
//
void CProgressDlg::SetStaticText(LPCSTR pszText)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   APD_SetStaticText(m_hWnd, pszText);
}

//===============================================================================================
// FUNCTION: SetStaticTextItem
// PURPOSE:  .
//
void CProgressDlg::SetStaticTextItem(int nItem, LPCSTR pszItem)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   LPSZASSERT(pszItem);
   APD_SetStaticTextItem(m_hWnd, nItem, pszItem);
}

//===============================================================================================
// FUNCTION: SetStaticTextItem
// PURPOSE:  .
//
void CProgressDlg::SetStaticTextItem(int nItem, int nValue)
{
   MEMBERASSERT();
   char szValue[16];
   itoa(nValue, szValue, 10);
   SetStaticTextItem(0, szValue);
}

//===============================================================================================
// FUNCTION: Update
// PURPOSE:  .
//
BOOL CProgressDlg::Update(int nPercentage)
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   return APD_Update(m_hWnd, nPercentage);
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  .
//
void CProgressDlg::Close()
{
   MEMBERASSERT();
   if (IsWindow(m_hWnd))
      APD_Close(m_hWnd);
   m_hWnd = NULL;
}

//===============================================================================================
// FUNCTION: HasCancelled
// PURPOSE:  .
//
BOOL CProgressDlg::HasCancelled()
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   return APD_HasCancelled(m_hWnd);
}

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
void CProgressDlg::PumpMessage()
{
   MEMBERASSERT();
   HWNDASSERT(m_hWnd);
   APD_PumpMessage(m_hWnd);
}

//===============================================================================================
// FUNCTION: Show
// PURPOSE:  .
//
void CProgressDlg::Show()
{
   MEMBERASSERT();
   if (m_hWnd)
      APD_Show(m_hWnd);
}
