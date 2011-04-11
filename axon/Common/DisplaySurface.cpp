//****************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************************
// MODULE:  DisplaySurface.CPP
// PURPOSE: Contains class implimentation of CDisplaySurface class.
// AUTHOR:  BHI  Apr 1999
//
#include "wincpp.hpp"
#include "DisplaySurface.hpp"
#include "Colors.h"

#define DRAWFASTRECT(hdc,lprc)  ExtTextOut(hdc,0,0,ETO_OPAQUE,lprc,NULL,0,NULL)

//==============================================================================================
// FUNCTION: Constructor.
// PURPOSE:  Object initialization.
//
CDisplaySurface::CDisplaySurface(bool bInverted)
{
   MEMBERASSERT();
   m_pDibBits       = NULL;
   m_pBitmapInfo    = NULL;
   m_hdcWin         = NULL;
   m_hdcBM          = CreateCompatibleDC( NULL );
   m_hBitmap        = NULL;
   m_hbmSave        = NULL;
   m_rgbBackground  = RGB_BLACK;
   m_bMatchScreen   = TRUE;
   m_nPaletteSize   = 0;
   m_nBitsPerPixel  = 0;
   m_bDirty         = false;
   m_bInverted      = bInverted;
   m_ActualSize     = CSize(0,0);
   m_rDisplay.SetRectEmpty();
   m_nLockCount     = 0;

   SelectBrush(m_hdcBM, GetStockObject(HOLLOW_BRUSH));
   SetBkMode(m_hdcBM, TRANSPARENT);
}

//==============================================================================================
// FUNCTION: Copy Constructor.
// PURPOSE:  Object initialization.
//
CDisplaySurface::CDisplaySurface(const CDisplaySurface &C)
{
   MEMBERASSERT();
   m_pDibBits       = NULL;
   m_pBitmapInfo    = NULL;
   m_hdcWin         = C.GetWinDC();
   m_hdcBM          = CreateCompatibleDC( NULL );
   m_hBitmap        = NULL;
   m_hbmSave        = NULL;
   m_rgbBackground  = C.m_rgbBackground;
   m_nPaletteSize   = 0;
   m_nBitsPerPixel  = 0;
   m_nLockCount     = 0;

   SelectBrush(m_hdcBM, GetStockObject(HOLLOW_BRUSH));
   SetBkMode(m_hdcBM, TRANSPARENT);

   Resize(&C.m_rDisplay);
}

//==============================================================================================
// FUNCTION: Destructor.
// PURPOSE:  Object cleanup.
//
CDisplaySurface::~CDisplaySurface()
{
   MEMBERASSERT();
   Lock();

   FreeBitmap();
   if (m_hdcBM)
   {
      DeleteDC(m_hdcBM);
      m_hdcBM = NULL;
   }
   Unlock();
}

//==============================================================================================
// FUNCTION: IsOK.
// PURPOSE:  Check for successful creation.
//
bool CDisplaySurface::IsOK() const
{
   MEMBERASSERT();
   return (m_hdcBM != NULL);
}

//==============================================================================================
// FUNCTION: FreeBitmap.
// PURPOSE:  Cleans up the off-screen bitmap.
//
void CDisplaySurface::FreeBitmap()
{
   MEMBERASSERT();

   if (m_hBitmap)
   {
      HDC hDC = Lock();
      ASSERT( m_hbmSave );
      ASSERT( hDC );
      SelectBitmap( hDC, m_hbmSave );
      Unlock();

      DeleteBitmap( m_hBitmap );
      m_hBitmap = NULL;
      m_hbmSave = NULL;
      free(m_pBitmapInfo);
      m_pBitmapInfo = NULL;
   }
}

//==============================================================================================
// FUNCTION: SetWinDC.
// PURPOSE:  Sets the DC for the window..
//
void CDisplaySurface::SetWinDC(HDC hDC)
{
   MEMBERASSERT();
   m_hdcWin = hDC;
}

//==============================================================================================
// FUNCTION: GetWinDC.
// PURPOSE:  Returns the offscreen device context.
//
HDC CDisplaySurface::GetWinDC() const
{
   MEMBERASSERT();
   return m_hdcWin;
}

//==============================================================================================
// FUNCTION: UpdateDisplay.
// PURPOSE:  Updates the screen from the offscreen bitmap.
//
void CDisplaySurface::Update(LPCRECT prUpdate)
{
   MEMBERASSERT();
   HDC hDC = Lock();

   // Transfer from bitmap to display
   if (m_bDirty)
   {
      // Set the rectangle to be updated.
      CRect rUpdate = prUpdate ? *prUpdate : m_rDisplay;
      ::BitBlt( m_hdcWin, rUpdate.left, rUpdate.top, 
                rUpdate.Width(), rUpdate.Height(), 
                hDC, rUpdate.left, rUpdate.top, SRCCOPY );
      m_bDirty = false;
   }
   Unlock();
}

//===============================================================================================
// FUNCTION: BitBlt
// PURPOSE:  .
//
void CDisplaySurface::BitBlt(HDC hDC, POINT ptDest, LPCRECT prUpdate )
{
   MEMBERASSERT();

   // Set the rectangle to be updated.
   CRect  rUpdate = prUpdate ? *prUpdate : m_rDisplay;
   ::BitBlt( hDC, ptDest.x, ptDest.y, 
             rUpdate.Width(), rUpdate.Height(), 
             Lock(), rUpdate.left, rUpdate.top, SRCCOPY );
   Unlock();
}

//===============================================================================================
// FUNCTION: BitBlt
// PURPOSE:  .
//
void CDisplaySurface::BitBlt(HDC hDC, LPCRECT prUpdate)
{
   MEMBERASSERT();

   // Set the rectangle to be updated.
   CRect  rUpdate = prUpdate ? *prUpdate : m_rDisplay;
   ::BitBlt( hDC, rUpdate.left, rUpdate.top, 
             rUpdate.Width(), rUpdate.Height(), 
             Lock(), rUpdate.left, rUpdate.top, SRCCOPY );
   Unlock();
}

//==============================================================================================
// FUNCTION: SetBackgroundColor.
// PURPOSE:  Sets the brush to use for background painting.
//
void CDisplaySurface::SetBackgroundColor( COLORREF rgbBackground )
{
   MEMBERASSERT();
   m_rgbBackground = rgbBackground;
}

//===============================================================================================
// FUNCTION: SetDirty
// PURPOSE:  Sets the dirty flag.
//
void CDisplaySurface::SetDirty()
{
   MEMBERASSERT();
   m_bDirty = true;
}

//===============================================================================================
// FUNCTION: GetDirty
// PURPOSE:  Returns the dirty flag.
//
bool CDisplaySurface::GetDirty() const
{
   MEMBERASSERT();
   return m_bDirty;
}

//===============================================================================================
// FUNCTION: GetPalette
// PURPOSE:  Returns the halftone palette.
//
HPALETTE CDisplaySurface::GetPalette() const
{
   MEMBERASSERT();
   return m_Palette;
}

//==============================================================================================
// FUNCTION: Clear.
// PURPOSE:  Fills a portion of the display surface with a brush.
//
void CDisplaySurface::Clear( LPCRECT prClear )
{
   MEMBERASSERT();
   CRect rClear = prClear ? *prClear : m_rDisplay;

   HDC hDC = Lock();
   COLORREF rgbSave = SetBkColor(hDC, m_rgbBackground);
   DRAWFASTRECT(hDC, &rClear);
   SetBkColor(hDC, rgbSave);
   GdiFlush();
   SetDirty();
   Unlock();
}

//===============================================================================================
// FUNCTION: SetInverted
// PURPOSE:  .
//
void CDisplaySurface::SetInverted(bool bInverted)
{
   MEMBERASSERT();
   m_bInverted = bInverted;
}

//===============================================================================================
// FUNCTION: SetResolution
// PURPOSE:  .
//
void CDisplaySurface::SetResolution(int nBitsPerPixel)
{
   MEMBERASSERT();
   ASSERT((nBitsPerPixel==16) || (nBitsPerPixel==24) || (nBitsPerPixel==32));
   m_bMatchScreen  = false;
   m_nBitsPerPixel = nBitsPerPixel;
   m_nPaletteSize  = 0;
}

//==============================================================================================
// FUNCTION: Resize.
// PURPOSE:  Sizes/Resizes the bitmap to the dimensions given.
//
void CDisplaySurface::Resize(LPCRECT prDisplay)
{
   MEMBERASSERT();
   ASSERT(m_hdcBM);
   WPTRASSERT(prDisplay);

   int nPaletteSize  = m_nPaletteSize;
   int nBitsPerPixel = m_nBitsPerPixel;
   if (m_bMatchScreen)
   {
      ASSERT(m_hdcWin);
      nPaletteSize  = GetDeviceCaps(m_hdcWin, SIZEPALETTE);
      nBitsPerPixel = GetDeviceCaps(m_hdcWin, BITSPIXEL);
   }

   // If nothing has changed, just get out.
   if (m_rDisplay.EqualRect(prDisplay) && 
       (nPaletteSize  == m_nPaletteSize) &&
       (nBitsPerPixel == m_nBitsPerPixel))
      return;

   HDC hDC = Lock();

   // Save the new surface properties.
   m_nPaletteSize  = nPaletteSize;
   m_nBitsPerPixel = nBitsPerPixel;
   m_rDisplay      = *prDisplay;

   // If a DIB was created previously, delete it.
   FreeBitmap();

   int nWidth = m_rDisplay.Width();
   nWidth = max(nWidth, 1);

   int nHeight = m_rDisplay.Height();
   nHeight = max(nHeight, 1);

   // Match the screen color depth.
   UINT uDIBWidthBits  = nWidth * nBitsPerPixel;
   UINT uDIBWidthBytes = ((uDIBWidthBits + 31) & (~31)) >> 3;
   m_ActualSize.cx     = uDIBWidthBytes;
   m_ActualSize.cy     = nHeight;

   if (m_bInverted)
      nHeight = -nHeight;
   
   // add the storage for the DIB colors, if we have any!
   // m_nPaletteSize will be 256 for an 8 bit DIB, and 0 for all other DIBs
   int nBytes = sizeof(BITMAPINFOHEADER) + m_nPaletteSize * sizeof(RGBQUAD);
         
   ASSERT(m_pBitmapInfo==NULL);
   m_pBitmapInfo = (BITMAPINFO *)malloc(nBytes);
   memset(m_pBitmapInfo, 0, nBytes);
   
   // if we've run out of memory, get out of here
   if (!m_pBitmapInfo)
      return;
     
   // set up the information that we need
   BITMAPINFOHEADER *pBitmapInfoHeader = &(m_pBitmapInfo->bmiHeader);

   pBitmapInfoHeader->biSize          = sizeof(BITMAPINFOHEADER); // always this
   pBitmapInfoHeader->biWidth         = nWidth;                   // the width of the bitmap
   pBitmapInfoHeader->biHeight        = nHeight;                  // the height, but negative so we display it upside down
   pBitmapInfoHeader->biPlanes        = 1;                        // Always 1
   pBitmapInfoHeader->biBitCount      = WORD(m_nBitsPerPixel);    // BPP for DIB
   pBitmapInfoHeader->biCompression   = BI_RGB;                   // no compression
   pBitmapInfoHeader->biSizeImage     = 0;                        // Calculation not needed for BI_RGB
   pBitmapInfoHeader->biXPelsPerMeter = 0;                        // These are arbitrary   
   pBitmapInfoHeader->biYPelsPerMeter = 0;                        // These are arbitrary   
   pBitmapInfoHeader->biClrUsed       = m_nPaletteSize;           // Use biBitCount to determine colors used
   pBitmapInfoHeader->biClrImportant  = 0;                        // All colors important, set to 0
   
   m_Palette.DeleteObject();
   if (m_nPaletteSize > 0)
   {
#ifdef _MFC_VER
      m_Palette.CreateHalftonePalette( CDC::FromHandle(m_hdcWin) );
#else
      m_Palette.CreateHalftonePalette(m_hdcWin);
#endif

      ASSERT(m_nPaletteSize <= 256);
      PALETTEENTRY PalEntries[256]         = { 0 };
      PALETTEENTRY IdentityPalEntries[256] = { 0 };

      // fill them in
      int nEntries = m_Palette.GetPaletteEntries(0, m_nPaletteSize, PalEntries);
      LPPALETTEENTRY pPalEntries = PalEntries;

      CreateIdentityPalette(PalEntries, IdentityPalEntries, nEntries);
      pPalEntries = IdentityPalEntries;

      // now convert to DIB color
      RGBQUAD *pDIBColorTable = m_pBitmapInfo->bmiColors;
      for (int i = 0; i < m_nPaletteSize; i++)    
      {
         pDIBColorTable[i].rgbBlue     = pPalEntries[i].peBlue;
         pDIBColorTable[i].rgbGreen    = pPalEntries[i].peGreen;
         pDIBColorTable[i].rgbRed      = pPalEntries[i].peRed;
         pDIBColorTable[i].rgbReserved = 0;
      }
   }

   // Create the DIB section.
   m_hBitmap = CreateDIBSection( hDC, m_pBitmapInfo, DIB_RGB_COLORS, (LPVOID *)(&m_pDibBits), NULL, 0);
   ASSERT(m_hBitmap);

   m_hbmSave = SelectBitmap( hDC, m_hBitmap );
   SetViewportOrgEx( hDC, -m_rDisplay.left, -m_rDisplay.top, NULL);

   Clear( &m_rDisplay );
   Unlock();
}

//===============================================================================================
// FUNCTION: CreateIdentityPalette
// PURPOSE:  Fills on output in pRGBIdentityOut with the identity based palette based on the RGB
//           values passed into this function. The colors given should be in order of importance, 
//           since the RGB colors at the end of the list will probably not be included in the
//           creation of the palette, since 20 static colors need to be preserved.      
//           If there is no system palette, the function fails.
// PARAMETERS:
//    pRGBEntryIn       The array of RGB values to be used to make an identity palette
//    pRGBIdentityOut   Pointer to PALETTEENTRYs. This array is filled with an 
//                      identity system palette. Number of elements must be the
//                      same as lNumRGBEntriesIn
//    lNumRGBEntriesIn  Number of elements in pRGBEntryIn
//
void CDisplaySurface::CreateIdentityPalette(const LPPALETTEENTRY pRGBEntryIn, LPPALETTEENTRY pRGBIdentityOut, int nNumEntries)
{   
   // make sure the # of RGB entries is at least as big as the size we're going to create
   RARRAYASSERT(pRGBEntryIn, nNumEntries);   
   WARRAYASSERT(pRGBIdentityOut, nNumEntries);   
  
   // get size of system palette, 256 for 8 bit display
   // if we are greater than 8 bits per pixel, we don't have a palette at all
   // m_nPaletteSize will be 0 for screen depths > 8
   ASSERT(m_nPaletteSize > 0);   // make sure we are in 8 bit display

   // get the system palette entries, since we need to make an identity palette   
   GetSystemPaletteEntries(m_hdcWin, 0, m_nPaletteSize, pRGBIdentityOut);

   // Get number of static colors and read entire system palette        
   // If the system has no static palette colors just copy everything   
   // the number of static colors in the first half (lower section) of the palette are
   // the number of static colors divided by 2, since the second half of the
   // static colors are located at the top (upper) section of the palette
   int nStaticColors = 0;
   if (GetSystemPaletteUse(m_hdcWin) == SYSPAL_NOSTATIC)
      nStaticColors = 0;
   else
      nStaticColors = max(GetDeviceCaps(m_hdcWin, NUMCOLORS) / 2, 0);
   
   // The palette goes something like this:
   //
   //   <--static colors--|------usuable colors-------|--static colors-->
   //   The static colors are usually from 0-9 and from 246-255 and the entries
   //   that we can use are in the range 10-245. So nStaticColors will usually equal 10
  
   // Set low part of static entry palette flags to zero   
   for (int i=0; i < nStaticColors; i++)      
      pRGBIdentityOut[i].peFlags = 0;

   // Now copy over the available part of the system palette
         
   // we need to find where the last usable color slot in the palette
   // is, this is usually at 256-nStaticColors-1=245 (256-10-1=245) slot, 
   // that is the palette size minus half of the static colors
   // (since half of them reside in the upper half of the palette)    
   // so nFirstUpperStaticColor = 256-10 = 246
   int nFirstUpperStaticColor = m_nPaletteSize-nStaticColors;      

   // we've dealt with the first section of the palette
   // so now we can look at the entries that we will fill up with the given
   // RGB colors (from 10-245)   

   int nColorIndex=0;         
   for (i=nStaticColors; i < nFirstUpperStaticColor && nColorIndex < nNumEntries; i++, nColorIndex++)
   {            
      BYTE nFlag = PC_NOCOLLAPSE;         
      pRGBIdentityOut[i] = pRGBEntryIn[nColorIndex];               

      // we need to look at the upper static colors (upper 10) to see if this color happens
      // to be a duplicate of one of these, if it is, we need to set this entry as PC_RESERVED 
      // so that we don't allow Windows to match the upper 10 matching color to this color later
      // if it is not a duplicate, we simply mark it as PC_NOCOLLAPSE so Windows doesn't match 
      // this color to another color in the palette      
      for (int n = nFirstUpperStaticColor; n < m_nPaletteSize; n++) 
      {
         if (pRGBIdentityOut[n].peRed   == pRGBEntryIn[nColorIndex].peRed &&
             pRGBIdentityOut[n].peGreen == pRGBEntryIn[nColorIndex].peGreen &&
             pRGBIdentityOut[n].peBlue  == pRGBEntryIn[nColorIndex].peBlue)              
         {
            nFlag = PC_RESERVED;                
            break;
         }
      }
      pRGBIdentityOut[i].peFlags = nFlag;             
   }
   
   // Finish by setting botton part of static entry's palette flags
   // Set the peFlags of the upper static colors to zero, to match it normally
   for (; i < m_nPaletteSize; i++)
      pRGBIdentityOut[i].peFlags = 0;                                   
}

//==============================================================================================
// FUNCTION: Lock.
// PURPOSE:  Returns the offscreen device context.
//
HDC CDisplaySurface::Lock() const
{
   MEMBERASSERT();
   // TRACE1("Lock (%d)\n", m_nLockCount);
   ASSERT(m_nLockCount >= 0);
   m_CS.Lock();
   m_nLockCount++;
   return m_hdcBM;
}

//===============================================================================================
// FUNCTION: Unlock
// PURPOSE:  .
//
void CDisplaySurface::Unlock() const
{
   MEMBERASSERT();
   --m_nLockCount;
   ASSERT(m_nLockCount >= 0);
   // TRACE1("Unlock (%d)\n", m_nLockCount);
   m_CS.Unlock();
}

//===============================================================================================
// FUNCTION: GetBitmap
// PURPOSE:  Exposes the bitmap internals.
//
BYTE *CDisplaySurface::GetBitmap(BITMAPINFOHEADER *pBIH)
{
   // To support palette devices we need to transfer the color table also...
   ASSERT(m_nPaletteSize == 0);
   if (pBIH)
   {
      if (m_pBitmapInfo)
         *pBIH = m_pBitmapInfo->bmiHeader;
      else
      {
         UINT uSize = pBIH->biSize;
         if (!uSize)
            uSize = sizeof(BITMAPINFOHEADER);
         memset(pBIH, 0, uSize);
         pBIH->biSize = uSize;
      }
   }
   return m_pDibBits;
}
