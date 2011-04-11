//***********************************************************************************************
//
//    Copyright (C) 1992-1997 Microsoft Corporation
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  AfxGDIObj.HPP
// PURPOSE: Utility classes "borrowed" from MFC 4.21.
// AUTHOR:  BHI  Nov 1998
//

#ifndef INC_AFXGDIOBJ_HPP
#define INC_AFXGDIOBJ_HPP

#ifndef _MFC_VER

#pragma once

#include "AfxUtils.hpp"

class CPen;
class CBrush;
class CBitmap;
class CPalette;
class CRgn;

#ifdef _INC_WINDOWSX
// The following names from WINDOWSX.H collide with names in this header
#undef CopyRgn
#endif

/////////////////////////////////////////////////////////////////////////////
// CGdiObject abstract class for HDC SelectObject

class CGdiObject
{
public:

// Attributes
   HGDIOBJ m_hObject;                  // must be first data member
//    operator HGDIOBJ() const;

   BOOL Attach(HGDIOBJ hObject);
   HGDIOBJ Detach();

// Constructors
   CGdiObject(); // must Create a derived class object
   ~CGdiObject();

   BOOL DeleteObject();

// Operations
   int GetObject(int nCount, LPVOID lpObject) const;
   UINT GetObjectType() const;
   BOOL CreateStockObject(int nIndex);
   BOOL UnrealizeObject();
   BOOL operator==(const CGdiObject& obj) const;
   BOOL operator!=(const CGdiObject& obj) const;
};

/////////////////////////////////////////////////////////////////////////////
// CGdiObject subclasses (drawing tools)

class CPen : public CGdiObject
{
public:

// Constructors
   CPen();
   ~CPen();

   CPen(int nPenStyle, int nWidth, COLORREF crColor);
   CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
        int nStyleCount = 0, const DWORD* lpStyle = NULL);
   BOOL CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
   BOOL CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
                  int nStyleCount = 0, const DWORD* lpStyle = NULL);
   BOOL CreatePenIndirect(LPLOGPEN lpLogPen);

// Attributes
   operator HPEN() const;
   int GetLogPen(LOGPEN* pLogPen);
   int GetExtLogPen(EXTLOGPEN* pLogPen);
};

class CBrush : public CGdiObject
{
public:

// Constructors
   CBrush();
   ~CBrush();

   CBrush(COLORREF crColor);              // CreateSolidBrush
   CBrush(int nIndex, COLORREF crColor);  // CreateHatchBrush
   CBrush(CBitmap* pBitmap);              // CreatePatternBrush

   BOOL CreateSolidBrush(COLORREF crColor);
   BOOL CreateHatchBrush(int nIndex, COLORREF crColor);
   BOOL CreateBrushIndirect(const LOGBRUSH* lpLogBrush);
   BOOL CreatePatternBrush(HBITMAP hBitmap);
   BOOL CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
   BOOL CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage);
   BOOL CreateSysColorBrush(int nIndex);

// Attributes
   operator HBRUSH() const;
   int GetLogBrush(LOGBRUSH* pLogBrush);
};

class CFont : public CGdiObject
{
public:

// Constructors
   CFont();
   ~CFont();

   BOOL CreateFontIndirect(const LOGFONT* lpLogFont);
   BOOL CreateFont(int nHeight, int nWidth, int nEscapement,
         int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
         BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
         BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
         LPCTSTR lpszFacename);
   BOOL CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hDC = NULL);
   BOOL CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hDC = NULL);

// Attributes
   operator HFONT() const;
   int GetLogFont(LOGFONT* pLogFont);
};

class CBitmap : public CGdiObject
{
public:

// Constructors
   CBitmap();
   ~CBitmap();

   BOOL LoadBitmap(HINSTANCE hInstance, LPCTSTR lpszResourceName);
   BOOL LoadBitmap(HINSTANCE hInstance, UINT nIDResource);
   BOOL LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
   BOOL LoadImage(HINSTANCE hInstance, LPCTSTR lpszName, UINT fuLoad = LR_DEFAULTCOLOR);
   BOOL LoadImage(HINSTANCE hInstance, UINT uIDResource, UINT fuLoad = LR_DEFAULTCOLOR);
   BOOL CreateDIBSection(HDC hDC, CONST BITMAPINFO *pbmi, UINT iUsage, 
                         VOID **ppvBits, HANDLE hSection, DWORD dwOffset);

   //   BOOL LoadMappedBitmap(HINSTANCE hInstance, UINT nIDBitmap, UINT nFlags = 0,
   //                         LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
   BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits);
   BOOL CreateBitmapIndirect(LPBITMAP lpBitmap);
   BOOL CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight);
   BOOL CreateDiscardableBitmap(HDC hDC, int nWidth, int nHeight);

// Attributes
   operator HBITMAP() const;
   int GetBitmap(BITMAP* pBitMap);

// Operations
   DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);
   DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
   CSize SetBitmapDimension(int nWidth, int nHeight);
   CSize GetBitmapDimension() const;
};

class CPalette : public CGdiObject
{
public:

// Constructors
   CPalette();
   ~CPalette();

   BOOL CreatePalette(LPLOGPALETTE lpLogPalette);
   BOOL CreateHalftonePalette(HDC hDC);

// Attributes
   operator HPALETTE() const;
   int GetEntryCount();
   UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
   UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);

// Operations
   void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);
   UINT GetNearestPaletteIndex(COLORREF crColor) const;
   BOOL ResizePalette(UINT nNumEntries);
};

class CRgn : public CGdiObject
{
public:
   operator HRGN() const;

// Constructors
   CRgn();
   ~CRgn();

   BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
   BOOL CreateRectRgnIndirect(LPCRECT lpRect);
   BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
   BOOL CreateEllipticRgnIndirect(LPCRECT lpRect);
   BOOL CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
   BOOL CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode);
   BOOL CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
   BOOL CreateFromPath(HDC hDC);
   BOOL CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData);

// Operations
   void SetRectRgn(int x1, int y1, int x2, int y2);
   void SetRectRgn(LPCRECT lpRect);
   int  CombineRgn(HRGN hRgn1, HRGN hRgn2, int nCombineMode);
   int  CopyRgn(HRGN hRgnSrc);
   BOOL EqualRgn(HRGN hRgn) const;
   int  OffsetRgn(int x, int y);
   int  OffsetRgn(POINT point);
   int  GetRgnBox(LPRECT lpRect) const;
   BOOL PtInRegion(int x, int y) const;
   BOOL PtInRegion(POINT point) const;
   BOOL RectInRegion(LPCRECT lpRect) const;
   int  GetRegionData(LPRGNDATA lpRgnData, int nCount) const;
};

// CGdiObject
inline CGdiObject::CGdiObject()
{
   m_hObject = NULL;
}

inline CGdiObject::~CGdiObject()
{
   DeleteObject();
}

// inline CGdiObject::operator HGDIOBJ() const
// {
//    return m_hObject;
// }

inline BOOL CGdiObject::Attach(HGDIOBJ hObject)
{
	ASSERT(m_hObject == NULL);      // only attach once, detach on destroy
	if (hObject == NULL)
		return FALSE;
	m_hObject = hObject;
	return TRUE;
}

inline HGDIOBJ CGdiObject::Detach()
{
	HGDIOBJ hObject = m_hObject;
	m_hObject = NULL;
	return hObject;
}

inline BOOL CGdiObject::DeleteObject()
{
	if (m_hObject == NULL)
		return FALSE;
	return ::DeleteObject(Detach());
}

inline int CGdiObject::GetObject(int nCount, LPVOID lpObject) const
{
   ASSERT(m_hObject != NULL);
   return ::GetObject(m_hObject, nCount, lpObject);
}

inline BOOL CGdiObject::CreateStockObject(int nIndex)
{
   m_hObject = ::GetStockObject(nIndex);
   return (m_hObject != NULL);
}

inline BOOL CGdiObject::UnrealizeObject()
{
   ASSERT(m_hObject != NULL);
   return ::UnrealizeObject(m_hObject);
}

inline UINT CGdiObject::GetObjectType() const
{
   return (UINT)::GetObjectType(m_hObject);
}

inline BOOL CGdiObject::operator==(const CGdiObject& obj) const
{
   return m_hObject == obj.m_hObject;
}

inline BOOL CGdiObject::operator!=(const CGdiObject& obj) const
{
   return m_hObject != obj.m_hObject;
}

// CPen
inline CPen::operator HPEN() const
{
   return (HPEN)(this == NULL ? NULL : m_hObject);
}

inline CPen::CPen()
{}

inline CPen::~CPen()
{}

inline BOOL CPen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
{
   return Attach(::CreatePen(nPenStyle, nWidth, crColor));
}

inline BOOL CPen::CreatePenIndirect(LPLOGPEN lpLogPen)
{
   return Attach(::CreatePenIndirect(lpLogPen));
}

inline BOOL CPen::CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
                            int nStyleCount, const DWORD* lpStyle)
{
   return Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle));
}

inline int CPen::GetExtLogPen(EXTLOGPEN* pLogPen)
{
   ASSERT(m_hObject != NULL);
   return ::GetObject(m_hObject, sizeof(EXTLOGPEN), pLogPen);
}
inline int CPen::GetLogPen(LOGPEN* pLogPen)
{
   ASSERT(m_hObject != NULL);
   return ::GetObject(m_hObject, sizeof(LOGPEN), pLogPen);
}

// CBrush
inline CBrush::operator HBRUSH() const
{
   return (HBRUSH)(this == NULL ? NULL : m_hObject);
}

inline CBrush::CBrush()
{}

inline CBrush::~CBrush()
{}

inline BOOL CBrush::CreateSolidBrush(COLORREF crColor)
{
   return Attach(::CreateSolidBrush(crColor));
}

inline BOOL CBrush::CreateHatchBrush(int nIndex, COLORREF crColor)
{
   return Attach(::CreateHatchBrush(nIndex, crColor));
}

inline BOOL CBrush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
{
   return Attach(::CreateBrushIndirect(lpLogBrush));
}

inline BOOL CBrush::CreatePatternBrush(HBITMAP hBitmap)
{
   return Attach(::CreatePatternBrush(hBitmap));
}

inline BOOL CBrush::CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage)
{
   return Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
}

inline BOOL CBrush::CreateSysColorBrush(int nIndex)
{
   return Attach(::GetSysColorBrush(nIndex));
}

inline int CBrush::GetLogBrush(LOGBRUSH* pLogBrush)
{
   ASSERT(m_hObject != NULL);
   return ::GetObject(m_hObject, sizeof(LOGBRUSH), pLogBrush);
}

// CFont
inline CFont::operator HFONT() const
{
   return (HFONT)(this == NULL ? NULL : m_hObject);
}

inline CFont::CFont()
{}

inline CFont::~CFont()
{}

inline BOOL CFont::CreateFontIndirect(const LOGFONT* lpLogFont)
{
   return Attach(::CreateFontIndirect(lpLogFont));
}

inline BOOL CFont::CreateFont(int nHeight, int nWidth, int nEscapement,
                              int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
                              BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
                              BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
                              LPCTSTR lpszFacename)
{
   return Attach(::CreateFont(nHeight, nWidth, nEscapement,
                              nOrientation, nWeight, bItalic, bUnderline, cStrikeOut,
                              nCharSet, nOutPrecision, nClipPrecision, nQuality,
                              nPitchAndFamily, lpszFacename));
}

// nPointSize is actually scaled 10x
inline BOOL CFont::CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hDC)
{
	LPSZASSERT(lpszFaceName);

   LOGFONT logFont = { 0 };
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight  = nPointSize;
	strncpy(logFont.lfFaceName, lpszFaceName, ELEMENTS_IN(logFont.lfFaceName));
	logFont.lfFaceName[ELEMENTS_IN(logFont.lfFaceName)-1] = '\0';

	return CreatePointFontIndirect(&logFont, hDC);
}

// pLogFont->nHeight is interpreted as PointSize * 10
inline BOOL CFont::CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hDC)
{
	RPTRASSERT(lpLogFont);
   HDC hAttribDC = (hDC == NULL) ? ::GetDC(NULL) : hDC;

	// convert nPointSize to logical units based on pDC
	LOGFONT logFont = *lpLogFont;
   CRect r;
   r.bottom  = ::GetDeviceCaps(hAttribDC, LOGPIXELSY) * logFont.lfHeight;
	r.bottom /= 720;    // 72 points/inch, 10 decipoints/point
	::DPtoLP(hAttribDC, &r.TopLeft(), 2);
	logFont.lfHeight = -abs(r.Height());

	if (hDC == NULL)
		ReleaseDC(NULL, hAttribDC);

	return CreateFontIndirect(&logFont);
}

inline int CFont::GetLogFont(LOGFONT* pLogFont)
{
   ASSERT(m_hObject != NULL);
   return ::GetObject(m_hObject, sizeof(LOGFONT), pLogFont);
}

// CBitmap
inline CBitmap::operator HBITMAP() const
{
   return (HBITMAP)(this == NULL ? NULL : m_hObject);
}

inline CBitmap::CBitmap()
{}

inline CBitmap::~CBitmap()
{}

inline BOOL CBitmap::CreateBitmap(int nWidth, int nHeight, UINT nPlanes,
                                  UINT nBitcount, const void* lpBits)
{
   return Attach(::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits));
}

inline BOOL CBitmap::CreateBitmapIndirect(LPBITMAP lpBitmap)
{
   return Attach(::CreateBitmapIndirect(lpBitmap));
}

inline DWORD CBitmap::SetBitmapBits(DWORD dwCount, const void* lpBits)
{
   return ::SetBitmapBits((HBITMAP)m_hObject, dwCount, lpBits);
}

inline DWORD CBitmap::GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
{
   return ::GetBitmapBits((HBITMAP)m_hObject, dwCount, lpBits);
}

inline BOOL CBitmap::LoadBitmap(HINSTANCE hInstance, LPCTSTR lpszResourceName)
{
   return Attach(::LoadBitmap(hInstance, lpszResourceName));
}

// inline BOOL CBitmap::LoadMappedBitmap(HINSTANCE hInstance, UINT nIDBitmap, UINT nFlags, LPCOLORMAP lpColorMap, int nMapSize)
// {
//    return Attach(::CreateMappedBitmap(hInstance, nIDBitmap, (WORD)nFlags, lpColorMap, nMapSize));
// }

inline BOOL CBitmap::CreateDIBSection(HDC hDC, CONST BITMAPINFO *pbmi, UINT iUsage, 
                                      VOID **ppvBits, HANDLE hSection, DWORD dwOffset)
{
   return Attach(::CreateDIBSection(hDC, pbmi, iUsage, ppvBits, hSection, dwOffset));
}

inline CSize CBitmap::SetBitmapDimension(int nWidth, int nHeight)
{
   SIZE size;
   VERIFY(::SetBitmapDimensionEx((HBITMAP)m_hObject, nWidth, nHeight, &size));
   return size;
}

inline CSize CBitmap::GetBitmapDimension() const
{
   SIZE size;
   VERIFY(::GetBitmapDimensionEx((HBITMAP)m_hObject, &size));
   return size;
}

inline BOOL CBitmap::LoadBitmap(HINSTANCE hInstance, UINT nIDResource)
{
   return Attach(::LoadBitmap(hInstance, MAKEINTRESOURCE(nIDResource)));
}

inline BOOL CBitmap::LoadOEMBitmap(UINT nIDBitmap)
{
   return Attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap)));
}

inline BOOL CBitmap::LoadImage(HINSTANCE hInstance, LPCTSTR lpszName, UINT fuLoad)
{
   return Attach(::LoadImage(hInstance, lpszName, IMAGE_BITMAP, 0, 0, fuLoad));
}

inline BOOL CBitmap::LoadImage(HINSTANCE hInstance, UINT uIDResource, UINT fuLoad)
{
   return Attach(::LoadImage(hInstance, MAKEINTRESOURCE(uIDResource), IMAGE_BITMAP, 0, 0, fuLoad));
}

inline BOOL CBitmap::CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight)
{
   return Attach(::CreateCompatibleBitmap(hDC, nWidth, nHeight));
}

inline BOOL CBitmap::CreateDiscardableBitmap(HDC hDC, int nWidth, int nHeight)
{
   return Attach(::CreateDiscardableBitmap(hDC, nWidth, nHeight));
}

inline int CBitmap::GetBitmap(BITMAP* pBitMap)
{
   ASSERT(m_hObject != NULL);
   return ::GetObject(m_hObject, sizeof(BITMAP), pBitMap);
}

// CPalette
inline CPalette::operator HPALETTE() const
{
   return (HPALETTE)(this == NULL ? NULL : m_hObject);
}

inline CPalette::CPalette()
{}

inline CPalette::~CPalette()
{}

inline BOOL CPalette::CreatePalette(LPLOGPALETTE lpLogPalette)
{
   return Attach(::CreatePalette(lpLogPalette));
}

inline BOOL CPalette::CreateHalftonePalette(HDC hDC)
{
   ASSERT(hDC != NULL);
   return Attach(::CreateHalftonePalette(hDC));
}

inline UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
{
   ASSERT(m_hObject != NULL);
   return ::GetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}

inline UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
{
   ASSERT(m_hObject != NULL);
   return ::SetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}

inline void CPalette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
{
   ASSERT(m_hObject != NULL);
   ::AnimatePalette((HPALETTE)m_hObject, nStartIndex, nNumEntries,lpPaletteColors);
}

inline UINT CPalette::GetNearestPaletteIndex(COLORREF crColor) const
{
   ASSERT(m_hObject != NULL);
   return ::GetNearestPaletteIndex((HPALETTE)m_hObject, crColor);
}

inline BOOL CPalette::ResizePalette(UINT nNumEntries)
{
   ASSERT(m_hObject != NULL);
   return ::ResizePalette((HPALETTE)m_hObject, nNumEntries);
}

inline int CPalette::GetEntryCount()
{
   ASSERT(m_hObject != NULL);
   WORD nEntries;
   ::GetObject(m_hObject, sizeof(WORD), &nEntries); 
   return (int)nEntries;
}

// CRgn
inline CRgn::operator HRGN() const
{
   return (HRGN)(this == NULL ? NULL : m_hObject);
}

inline CRgn::CRgn()
{}

inline CRgn::~CRgn()
{}

inline BOOL CRgn::CreateRectRgn(int x1, int y1, int x2, int y2)
{
   return Attach(::CreateRectRgn(x1, y1, x2, y2));
}

inline BOOL CRgn::CreateRectRgnIndirect(LPCRECT lpRect)
{
   return Attach(::CreateRectRgnIndirect(lpRect));
}

inline BOOL CRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2)
{
   return Attach(::CreateEllipticRgn(x1, y1, x2, y2));
}

inline BOOL CRgn::CreateEllipticRgnIndirect(LPCRECT lpRect)
{
   return Attach(::CreateEllipticRgnIndirect(lpRect));
}

inline BOOL CRgn::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
{
   return Attach(::CreatePolygonRgn(lpPoints, nCount, nMode));
}

inline BOOL CRgn::CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
{
   return Attach(::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode));
}

inline BOOL CRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
{
   return Attach(::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3));
}

inline BOOL CRgn::CreateFromPath(HDC hDC)
{
   ASSERT(hDC != NULL);
   return Attach(::PathToRegion(hDC));
}

inline BOOL CRgn::CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
{
   return Attach(::ExtCreateRegion(lpXForm, nCount, pRgnData));
}

inline int CRgn::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
{
   ASSERT(m_hObject != NULL);
   return (int)::GetRegionData((HRGN)m_hObject, nDataSize, lpRgnData);
}

inline void CRgn::SetRectRgn(int x1, int y1, int x2, int y2)
{
   ASSERT(m_hObject != NULL);
   ::SetRectRgn((HRGN)m_hObject, x1, y1, x2, y2);
}

inline void CRgn::SetRectRgn(LPCRECT lpRect)
{
   ASSERT(m_hObject != NULL);
   ::SetRectRgn((HRGN)m_hObject, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

inline int CRgn::CombineRgn(HRGN hRgn1, HRGN hRgn2, int nCombineMode)
{
   ASSERT(m_hObject != NULL);
   return ::CombineRgn((HRGN)m_hObject, hRgn1, hRgn2, nCombineMode);
}

inline int CRgn::CopyRgn(HRGN hRgnSrc)
{
   ASSERT(m_hObject != NULL);
   return ::CombineRgn((HRGN)m_hObject, hRgnSrc, NULL, RGN_COPY);
}

inline BOOL CRgn::EqualRgn(HRGN hRgn) const
{
   ASSERT(m_hObject != NULL);
   return ::EqualRgn((HRGN)m_hObject, hRgn);
}

inline int CRgn::OffsetRgn(int x, int y)
{
   ASSERT(m_hObject != NULL);
   return ::OffsetRgn((HRGN)m_hObject, x, y);
}

inline int CRgn::OffsetRgn(POINT point)
{
   ASSERT(m_hObject != NULL);
   return ::OffsetRgn((HRGN)m_hObject, point.x, point.y);
}

inline int CRgn::GetRgnBox(LPRECT lpRect) const
{
   ASSERT(m_hObject != NULL);
   return ::GetRgnBox((HRGN)m_hObject, lpRect);
}

inline BOOL CRgn::PtInRegion(int x, int y) const
{
   ASSERT(m_hObject != NULL);
   return ::PtInRegion((HRGN)m_hObject, x, y);
}

inline BOOL CRgn::PtInRegion(POINT point) const
{
   ASSERT(m_hObject != NULL);
   return ::PtInRegion((HRGN)m_hObject, point.x, point.y);
}

inline BOOL CRgn::RectInRegion(LPCRECT lpRect) const
{
   ASSERT(m_hObject != NULL);
   return ::RectInRegion((HRGN)m_hObject, lpRect);
}

#endif      // _MFC_VER

//===================================================================================
// CLASS:   CRgnEx
// PURPOSE: Wrapper class for a CRgn to fix a bug in ellipse creation.
// NOTES:   Windows regions have a bug when creating ellipses.
//          The ellipse created results in a bounding rectangle one pixel off
//          (right & bottom) from the bounding rectangle passed in.
//
class CRgnEx : public CRgn
{
public:
   BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2)
   {
      return CRgn::CreateEllipticRgn(x1, y1, x2+1, y2+1);
   }
   BOOL CreateEllipticRgnIndirect(LPCRECT lpRect)
   {
      CRect r(lpRect);
      return CRgn::CreateEllipticRgn(r.left, r.top, r.right+1, r.bottom+1);
   }
};

#endif      // INC_AFXGDIOBJ_HPP

