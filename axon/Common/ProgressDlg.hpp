//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  ProgressDlg.HPP
// PURPOSE: Wrapper class for the Axon progress dialog.
// AUTHOR:  BHI  Sep 1999
//

#ifndef INC_PROGRESSDLG_HPP
#define INC_PROGRESSDLG_HPP

#pragma once
#include "\AxonDev\Comp\AxCommonDlg\AxCommonDlg.h"

class CProgressDlg
{
private:   // Attributes
   HWND  m_hWnd;

private:   // Unimplemented copy functions.
   CProgressDlg(const CProgressDlg &);
   const CProgressDlg &operator=(const CProgressDlg &);

private:   // Private functions

public:    // Public interface
   CProgressDlg();
   ~CProgressDlg();

   BOOL Open(HWND hwndNotify, LPCSTR pszTitle, UINT uFlags = APD_HAS_CANCELBUTTON | APD_DISABLEPARENTS);
   void SetTitle(LPCSTR pszTitle);
   void SetStaticText(LPCSTR pszText);
   void SetStaticTextItem(int nItem, LPCSTR pszItem);
   void SetStaticTextItem(int nItem, int nValue);
   BOOL Update(int nPercentage);
   BOOL HasCancelled();
   void PumpMessage();
   void Show();
   void Close();
};

class CAutoCloseProgressDlg
{
private:
   CProgressDlg *m_pDlg;
public:
   CAutoCloseProgressDlg(CProgressDlg *pDlg) { m_pDlg = pDlg; };
   ~CAutoCloseProgressDlg() { if (m_pDlg) m_pDlg->Close(); m_pDlg=NULL; };
};

#endif      // INC_PROGRESSDLG_HPP
