//***********************************************************************************************
//
//    Copyright (C) 1992-1997 Microsoft Corporation
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  AfxUtils.HPP
// PURPOSE: Utility classes "borrowed" from MFC 4.21.
// AUTHOR:  BHI  Jul 1998
//

#ifndef INC_AFXUTILS_HPP
#define INC_AFXUTILS_HPP

#ifndef _MFC_VER

#pragma once

class CSize;
class CPoint;
class CRect;

/////////////////////////////////////////////////////////////////////////////
// CSize - An extent, similar to Windows SIZE structure.

class CSize : public tagSIZE
{
public:
   
   // Constructors
   CSize();
   CSize(int initCX, int initCY);
   CSize(SIZE initSize);
   CSize(POINT initPt);
   CSize(DWORD dwSize);
   
   // Operations
   BOOL operator==(SIZE size) const;
   BOOL operator!=(SIZE size) const;
   void operator+=(SIZE size);
   void operator-=(SIZE size);
   
   // Operators returning CSize values
   CSize operator+(SIZE size) const;
   CSize operator-(SIZE size) const;
   CSize operator-() const;
   
   // Operators returning CPoint values
   CPoint operator+(POINT point) const;
   CPoint operator-(POINT point) const;
   
   // Operators returning CRect values
   CRect operator+(const RECT* pRect) const;
   CRect operator-(const RECT* pRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CPoint - A 2-D point, similar to Windows POINT structure.

class CPoint : public tagPOINT
{
public:
   
   // Constructors
   CPoint();
   CPoint(int initX, int initY);
   CPoint(POINT initPt);
   CPoint(SIZE initSize);
   CPoint(DWORD dwPoint);
   
   // Operations
   void Offset(int xOffset, int yOffset);
   void Offset(POINT point);
   void Offset(SIZE size);
   BOOL operator==(POINT point) const;
   BOOL operator!=(POINT point) const;
   void operator+=(SIZE size);
   void operator-=(SIZE size);
   void operator+=(POINT point);
   void operator-=(POINT point);
   
   // Operators returning CPoint values
   CPoint operator+(SIZE size) const;
   CPoint operator-(SIZE size) const;
   CPoint operator-() const;
   CPoint operator+(POINT point) const;
   CPoint MulDiv(int nMultiplier, int nDivisor) const;
   
   // Operators returning CSize values
   CSize operator-(POINT point) const;
   
   // Operators returning CRect values
   CRect operator+(const RECT* pRect) const;
   CRect operator-(const RECT* pRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CRect - A 2-D rectangle, similar to Windows RECT structure.

typedef const RECT* LPCRECT;    // pointer to read/only RECT

class CRect : public tagRECT
{
public:
   
   // Constructors
   CRect();
   CRect(int l, int t, int r, int b);
   CRect(const RECT& srcRect);
   CRect(LPCRECT lpSrcRect);
   CRect(POINT point, SIZE size);
   CRect(POINT topLeft, POINT bottomRight);
   
   // Attributes (in addition to RECT members)
   int Width() const;
   int Height() const;
   CSize Size() const;
   CPoint& TopLeft();
   CPoint& BottomRight();
   const CPoint& TopLeft() const;
   const CPoint& BottomRight() const;
   CPoint CenterPoint() const;
   
   // convert between CRect and LPRECT/LPCRECT (no need for &)
   operator LPRECT();
   operator LPCRECT() const;
   
   BOOL IsRectEmpty() const;
   BOOL IsRectNull() const;
   BOOL PtInRect(POINT point) const;
   
   // Operations
   void SetRect(int x1, int y1, int x2, int y2);
   void SetRect(POINT topLeft, POINT bottomRight);
   void SetRectEmpty();
   void CopyRect(LPCRECT lpSrcRect);
   BOOL EqualRect(LPCRECT pRect) const;
   
   void InflateRect(int x, int y);
   void InflateRect(SIZE size);
   void InflateRect(LPCRECT pRect);
   void InflateRect(int l, int t, int r, int b);
   void DeflateRect(int x, int y);
   void DeflateRect(SIZE size);
   void DeflateRect(LPCRECT pRect);
   void DeflateRect(int l, int t, int r, int b);
   
   void OffsetRect(int x, int y);
   void OffsetRect(SIZE size);
   void OffsetRect(POINT point);
   void NormalizeRect();
   
   // operations that fill '*this' with result
   BOOL IntersectRect(LPCRECT pRect1, LPCRECT pRect2);
   BOOL UnionRect(LPCRECT pRect1, LPCRECT pRect2);
   BOOL SubtractRect(LPCRECT pRectSrc1, LPCRECT pRectSrc2);
   
   // Additional Operations
   void operator=(const RECT& srcRect);
   BOOL operator==(const RECT& rect) const;
   BOOL operator!=(const RECT& rect) const;
   void operator+=(POINT point);
   void operator+=(SIZE size);
   void operator+=(LPCRECT pRect);
   void operator-=(POINT point);
   void operator-=(SIZE size);
   void operator-=(LPCRECT pRect);
   void operator&=(const RECT& rect);
   void operator|=(const RECT& rect);
   void operator*=(int nFactor);
   void operator/=(int nFactor);
   
   // Operators returning CRect values
   CRect operator+(POINT point) const;
   CRect operator-(POINT point) const;
   CRect operator+(LPCRECT pRect) const;
   CRect operator+(SIZE size) const;
   CRect operator-(SIZE size) const;
   CRect operator-(LPCRECT pRect) const;
   CRect operator&(const RECT& rect2) const;
   CRect operator|(const RECT& rect2) const;
   CRect MulDiv(int nMultiplier, int nDivisor) const;
};

// CSize
inline CSize::CSize()
{
   cx = cy = 0;
}

inline CSize::CSize(int initCX, int initCY)
{
   cx = initCX;
   cy = initCY;
}

inline CSize::CSize(SIZE initSize)
{
   *(SIZE*)this = initSize;
}

inline CSize::CSize(POINT initPt)
{
   *(POINT*)this = initPt;
}

inline CSize::CSize(DWORD dwSize)
{
   cx = (short)LOWORD(dwSize);
   cy = (short)HIWORD(dwSize);
}

inline BOOL CSize::operator==(SIZE size) const
{
   return (cx == size.cx && cy == size.cy);
}

inline BOOL CSize::operator!=(SIZE size) const
{
   return (cx != size.cx || cy != size.cy);
}

inline void CSize::operator+=(SIZE size)
{
   cx += size.cx; cy += size.cy;
}

inline void CSize::operator-=(SIZE size)
{
   cx -= size.cx; cy -= size.cy;
}

inline CSize CSize::operator+(SIZE size) const
{
   return CSize(cx + size.cx, cy + size.cy);
}

inline CSize CSize::operator-(SIZE size) const
{
   return CSize(cx - size.cx, cy - size.cy);
}

inline CSize CSize::operator-() const
{
   return CSize(-cx, -cy);
}

inline CPoint CSize::operator+(POINT point) const
{
   return CPoint(cx + point.x, cy + point.y);
}

inline CPoint CSize::operator-(POINT point) const
{
   return CPoint(cx - point.x, cy - point.y);
}

inline CRect CSize::operator+(const RECT* pRect) const
{
   return CRect(pRect) + *this;
}

inline CRect CSize::operator-(const RECT* pRect) const
{
   return CRect(pRect) - *this;
}

// CPoint
inline CPoint::CPoint()
{
   x = y = 0;
}

inline CPoint::CPoint(int initX, int initY)
{
   x = initX;
   y = initY;
}

inline CPoint::CPoint(POINT initPt)
{
   *(POINT*)this = initPt;
}

inline CPoint::CPoint(SIZE initSize)
{
   *(SIZE*)this = initSize;
}

inline CPoint::CPoint(DWORD dwPoint)
{
   x = (short)LOWORD(dwPoint);
   y = (short)HIWORD(dwPoint);
}

inline void CPoint::Offset(int xOffset, int yOffset)
{
   x += xOffset;
   y += yOffset;
}

inline void CPoint::Offset(POINT point)
{
   x += point.x;
   y += point.y;
}

inline void CPoint::Offset(SIZE size)
{
   x += size.cx;
   y += size.cy;
}

inline BOOL CPoint::operator==(POINT point) const
{
   return (x == point.x && y == point.y);
}

inline BOOL CPoint::operator!=(POINT point) const
{
   return (x != point.x || y != point.y);
}

inline void CPoint::operator+=(SIZE size)
{
   x += size.cx;
   y += size.cy;
}

inline void CPoint::operator-=(SIZE size)
{
   x -= size.cx;
   y -= size.cy;
}

inline void CPoint::operator+=(POINT point)
{
   x += point.x;
   y += point.y;
}

inline void CPoint::operator-=(POINT point)
{
   x -= point.x;
   y -= point.y;
}

inline CPoint CPoint::operator+(SIZE size) const
{
   return CPoint(x + size.cx, y + size.cy);
}

inline CPoint CPoint::operator-(SIZE size) const
{
   return CPoint(x - size.cx, y - size.cy);
}

inline CPoint CPoint::operator-() const
{
   return CPoint(-x, -y);
}

inline CPoint CPoint::operator+(POINT point) const
{
   return CPoint(x + point.x, y + point.y);
}

inline CSize CPoint::operator-(POINT point) const
{
   return CSize(x - point.x, y - point.y);
}

inline CRect CPoint::operator+(const RECT* pRect) const
{
   return CRect(pRect) + *this;
}

inline CRect CPoint::operator-(const RECT* pRect) const
{
   return CRect(pRect) - *this;
}

inline CPoint CPoint::MulDiv(int nMultiplier, int nDivisor) const
{
	return CPoint(::MulDiv(x, nMultiplier, nDivisor),
		           ::MulDiv(y, nMultiplier, nDivisor));
}

// CRect
inline CRect::CRect()
{
   left = top = right = bottom = 0;
}

inline CRect::CRect(int l, int t, int r, int b)
{
   left = l; top = t; right = r; bottom = b;
}

inline CRect::CRect(const RECT& srcRect)
{
   ::CopyRect(this, &srcRect);
}

inline CRect::CRect(LPCRECT lpSrcRect)
{
   ::CopyRect(this, lpSrcRect);
}

inline CRect::CRect(POINT point, SIZE size)
{
   right  = (left = point.x) + size.cx;
   bottom = (top  = point.y) + size.cy;
}

inline CRect::CRect(POINT topLeft, POINT bottomRight)
{
   left   = topLeft.x;
   top    = topLeft.y;
   right  = bottomRight.x;
   bottom = bottomRight.y;
}

inline int CRect::Width() const
{
   return right - left;
}

inline int CRect::Height() const
{
   return bottom - top;
}

inline CSize CRect::Size() const
{
   return CSize(right - left, bottom - top);
}

inline CPoint& CRect::TopLeft()
{
   return *((CPoint*)this);
}

inline CPoint& CRect::BottomRight()
{
   return *((CPoint*)this+1);
}

inline const CPoint& CRect::TopLeft() const
{
   return *((CPoint*)this);
}

inline const CPoint& CRect::BottomRight() const
{
   return *((CPoint*)this+1);
}

inline CPoint CRect::CenterPoint() const
{
   return CPoint((left+right)/2, (top+bottom)/2);
}

inline CRect::operator LPRECT()
{
   return this;
}

inline CRect::operator LPCRECT() const
{
   return this;
}

inline BOOL CRect::IsRectEmpty() const
{
   return ::IsRectEmpty(this);
}

inline BOOL CRect::IsRectNull() const
{
   return (left == 0 && right == 0 && top == 0 && bottom == 0);
}

inline BOOL CRect::PtInRect(POINT point) const
{
   return ::PtInRect(this, point);
}

inline void CRect::SetRect(int x1, int y1, int x2, int y2)
{
   ::SetRect(this, x1, y1, x2, y2);
}

inline void CRect::SetRect(POINT topLeft, POINT bottomRight)
{
   ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

inline void CRect::SetRectEmpty()
{
   ::SetRectEmpty(this);
}

inline void CRect::CopyRect(LPCRECT lpSrcRect)
{
   ::CopyRect(this, lpSrcRect);
}

inline BOOL CRect::EqualRect(LPCRECT pRect) const
{
   return ::EqualRect(this, pRect);
}

inline void CRect::InflateRect(int x, int y)
{
   ::InflateRect(this, x, y);
}

inline void CRect::InflateRect(SIZE size)
{
   ::InflateRect(this, size.cx, size.cy);
}

inline void CRect::DeflateRect(int x, int y)
{
   ::InflateRect(this, -x, -y);
}

inline void CRect::DeflateRect(SIZE size)
{
   ::InflateRect(this, -size.cx, -size.cy);
}

inline void CRect::OffsetRect(int x, int y)
{
   ::OffsetRect(this, x, y);
}

inline void CRect::OffsetRect(POINT point)
{
   ::OffsetRect(this, point.x, point.y);
}

inline void CRect::OffsetRect(SIZE size)
{
   ::OffsetRect(this, size.cx, size.cy);
}

inline BOOL CRect::IntersectRect(LPCRECT pRect1, LPCRECT pRect2)
{
   return ::IntersectRect(this, pRect1, pRect2);
}

inline BOOL CRect::UnionRect(LPCRECT pRect1, LPCRECT pRect2)
{
   return ::UnionRect(this, pRect1, pRect2);
}

inline void CRect::operator=(const RECT& srcRect)
{
   ::CopyRect(this, &srcRect);
}

inline BOOL CRect::operator==(const RECT& rect) const
{
   return ::EqualRect(this, &rect);
}

inline BOOL CRect::operator!=(const RECT& rect) const
{
   return !::EqualRect(this, &rect);
}

inline void CRect::operator+=(POINT point)
{
   ::OffsetRect(this, point.x, point.y);
}

inline void CRect::operator+=(SIZE size)
{
   ::OffsetRect(this, size.cx, size.cy);
}

inline void CRect::operator+=(LPCRECT pRect)
{
   InflateRect(pRect);
}

inline void CRect::operator-=(POINT point)
{
   ::OffsetRect(this, -point.x, -point.y);
}

inline void CRect::operator-=(SIZE size)
{
   ::OffsetRect(this, -size.cx, -size.cy);
}

inline void CRect::operator-=(LPCRECT pRect)
{
   DeflateRect(pRect);
}

inline void CRect::operator&=(const RECT& rect)
{
   ::IntersectRect(this, this, &rect);
}

inline void CRect::operator|=(const RECT& rect)
{
   ::UnionRect(this, this, &rect);
}

inline void CRect::operator*=(int nFactor)
{
   left   *= nFactor;
   top    *= nFactor;
   right  *= nFactor;
   bottom *= nFactor;
}

inline void CRect::operator/=(int nFactor)
{
   ASSERT(nFactor != 0);
   left   /= nFactor;
   top    /= nFactor;
   right  /= nFactor;
   bottom /= nFactor;
}

inline CRect CRect::operator+(POINT pt) const
{
   CRect rect(*this);
   ::OffsetRect(&rect, pt.x, pt.y);
   return rect;
}

inline CRect CRect::operator-(POINT pt) const
{
   CRect rect(*this);
   ::OffsetRect(&rect, -pt.x, -pt.y);
   return rect;
}

inline CRect CRect::operator+(SIZE size) const
{
   CRect rect(*this);
   ::OffsetRect(&rect, size.cx, size.cy);
   return rect;
}

inline CRect CRect::operator-(SIZE size) const
{
   CRect rect(*this);
   ::OffsetRect(&rect, -size.cx, -size.cy);
   return rect;
}

inline CRect CRect::operator+(LPCRECT pRect) const
{
   CRect rect(this);
   rect.InflateRect(pRect);
   return rect;
}

inline CRect CRect::operator-(LPCRECT pRect) const
{
   CRect rect(this);
   rect.DeflateRect(pRect);
   return rect;
}

inline CRect CRect::operator&(const RECT& rect2) const
{
   CRect rect;
   ::IntersectRect(&rect, this, &rect2);
   return rect;
}

inline CRect CRect::operator|(const RECT& rect2) const
{
   CRect rect;
   ::UnionRect(&rect, this, &rect2);
   return rect;
}

inline BOOL CRect::SubtractRect(LPCRECT pRectSrc1, LPCRECT pRectSrc2)
{
   return ::SubtractRect(this, pRectSrc1, pRectSrc2);
}

inline void CRect::NormalizeRect()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

inline void CRect::InflateRect(LPCRECT pRect)
{
   InflateRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
}

inline void CRect::InflateRect(int l, int t, int r, int b)
{
	left   -= l;
	top    -= t;
	right  += r;
	bottom += b;
}

inline void CRect::DeflateRect(LPCRECT pRect)
{
   DeflateRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
}

inline void CRect::DeflateRect(int l, int t, int r, int b)
{
	left   += l;
	top    += t;
	right  -= r;
	bottom -= b;
}

inline CRect CRect::MulDiv(int nMultiplier, int nDivisor) const
{
	return CRect(::MulDiv(left, nMultiplier, nDivisor),
		          ::MulDiv(top, nMultiplier, nDivisor),
		          ::MulDiv(right, nMultiplier, nDivisor),
		          ::MulDiv(bottom, nMultiplier, nDivisor));
}

#endif      // _MFC_VER

#endif      // INC_AFXUTILS_HPP

