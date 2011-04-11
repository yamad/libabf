//****************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// HEADER:  DISPSURF.HPP
// PURPOSE: Contains class definition for CDisplaySurface class and associated derived classes.
// AUTHOR:  BHI  Mar 1996
//

#ifndef INC_DISPSURF_HPP
#define INC_DISPSURF_HPP

#pragma once
#include "AfxGdiObj.hpp"
#include "AfxMT.hpp"

//===============================================================================================

class CDisplaySurface
{
private:    // Private member variables.
   mutable CCriticalSection m_CS;        // For syncronised access to the bitmap DC.
   mutable int      m_nLockCount;

   BITMAPINFO      *m_pBitmapInfo;       // DIB header.
   BYTE            *m_pDibBits;	        // Pointer to the DIB bits.
   CSize            m_ActualSize;        // Actual physical size of the bitmap.
   HDC              m_hdcBM;             // Device context associated with this display surface
   HDC              m_hdcWin;            // Device context for the window.
   HBITMAP          m_hBitmap;           // Offscreen bitmap used to draw on.
   HBITMAP          m_hbmSave;           // Place to save Bitmap that was originally selected in the DC.
   CPalette         m_Palette;           // Palette used for display.
   bool             m_bMatchScreen;      // TRUE if the bitmap should match the screen res.
   int              m_nPaletteSize;      // Number of entries in the palette (0 if not 8bit mode).
   int              m_nBitsPerPixel;     // Number of bits per pixel in the screen DC.
   CRect            m_rDisplay;          // Current dimensions of the display surface in client coords.
   COLORREF         m_rgbBackground;     // Color to use for background.
   bool             m_bDirty;            // TRUE if the bitmap has been modified since last update.
   bool             m_bInverted;         // TRUE if the bitmap inverted.

private:    // Unimplimented copy constructor.
   const CDisplaySurface &operator=(const CDisplaySurface &C);

private:    // Private functions.
   void FreeBitmap();
   void CreateIdentityPalette(const LPPALETTEENTRY pRGBEntryIn, LPPALETTEENTRY pRGBIdentityOut, int nNumEntries);

public:     // Public interface.
   CDisplaySurface(bool bInverted=true);
   CDisplaySurface(const CDisplaySurface &C);
   ~CDisplaySurface();

   bool IsOK() const;

   void SetWinDC(HDC hDC);
   HDC  GetWinDC() const;
   HDC  GetDC() const { return m_hdcBM; }

   HDC  Lock() const;
   void Unlock() const;
   int  GetLockCount() const           { return m_nLockCount; }

   void SetInverted(bool bInverted);
   void SetResolution(int nBitsPerPixel);
   void SetBackgroundColor(COLORREF rgbBackground);

   bool     GetInverted() const        { return m_bInverted;     }
   int      GetResolution() const      { return m_nBitsPerPixel; }
   COLORREF GetBackgroundColor() const { return m_rgbBackground; }
   LPCRECT  GetSize() const            { return m_rDisplay;      }
   UINT     GetScanWidth() const       { return m_ActualSize.cx; }

   void Resize( LPCRECT prDisplay );
   void Clear( LPCRECT pRect=NULL );
   void Update( LPCRECT prUpdate=NULL );

   void BitBlt(HDC hDC, LPCRECT prUpdate=NULL );
   void BitBlt(HDC hDC, POINT ptDest, LPCRECT prUpdate=NULL );

   HPALETTE GetPalette() const;

   void SetDirty();
   bool GetDirty() const;

   BYTE *GetBitmap(BITMAPINFOHEADER *pBIH);
};

#endif

