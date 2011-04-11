//***********************************************************************************************
//
//    Copyright (C) 1992-1997 Microsoft Corporation
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  FloatRect.HPP
// PURPOSE: Floating point rectangle class (inspired by MFC CRect etc).
// AUTHOR:  BHI  Mar 2000
//

#ifndef INC_FLOATRECT_HPP
#define INC_FLOATRECT_HPP

#pragma once

template <class T> class CSizeTplt;
template <class T> class CPointTplt;
template <class T> class CRectTplt;

/////////////////////////////////////////////////////////////////////////////
// CSizeTplt - An extent, similar to Windows SIZE structure.

template <class T>
class CSizeTplt
{
public:
   T cx, cy;
   
   // Constructors
   CSizeTplt();
   CSizeTplt(T CX, T CY);
   CSizeTplt(CPointTplt<T> pt);
   
   // Operations
   BOOL operator==(CSizeTplt size) const;
   BOOL operator!=(CSizeTplt size) const;
   void operator+=(CSizeTplt size);
   void operator-=(CSizeTplt size);
   
   // Operators returning CSizeTplt values
   CSizeTplt operator+(CSizeTplt size) const;
   CSizeTplt operator-(CSizeTplt size) const;
   CSizeTplt operator-() const;
   
   // Operators returning CPointTplt values
   CPointTplt<T> operator+(CPointTplt<T> pt) const;
   CPointTplt<T> operator-(CPointTplt<T> pt) const;
   
   // Operators returning CRectTplt values
   CRectTplt<T> operator+(const CRectTplt<T> *pRect) const;
   CRectTplt<T> operator-(const CRectTplt<T> *pRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CPointTplt - A 2-D point, similar to Windows CPointTplt structure.

template <class T>
class CPointTplt
{
public:
   T x, y;
   
   // Constructors
   CPointTplt();
   CPointTplt(T X, T Y);
   CPointTplt(CSizeTplt<T> size);
   
   // Operations
   void Offset(T xOffset, T yOffset);
   void Offset(CPointTplt pt);
   void Offset(CSizeTplt<T> size);

   BOOL operator==(CPointTplt pt) const;
   BOOL operator!=(CPointTplt pt) const;
   void operator+=(CSizeTplt<T> size);
   void operator-=(CSizeTplt<T> size);
   void operator+=(CPointTplt pt);
   void operator-=(CPointTplt pt);
   
   // Operators returning CPointTplt values
   CPointTplt operator+(CSizeTplt<T> size) const;
   CPointTplt operator-(CSizeTplt<T> size) const;
   CPointTplt operator-() const;
   CPointTplt operator+(CPointTplt pt) const;
   CPointTplt operator*(T dMultiplier) const;
   
   // Operators returning CSizeTplt values
   CSizeTplt<T> operator-(CPointTplt pt) const;
   
   // Operators returning CRectTplt values
   CRectTplt<T> operator+(const CRectTplt<T> *pRect) const;
   CRectTplt<T> operator-(const CRectTplt<T> *pRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CRectTplt - A 2-D rectangle, similar to Windows CRectTplt structure.

template <class T>
class CRectTplt
{
public:
   T left, top, right, bottom;
   
   // Constructors
   CRectTplt();
   CRectTplt(T l, T t, T r, T b);
   CRectTplt(const CRectTplt* lpSrcRect);
   CRectTplt(CPointTplt<T> pt, CSizeTplt<T> size);
   CRectTplt(CPointTplt<T> topLeft, CPointTplt<T> bottomRight);
   
   // Attributes (in addition to CRectTplt members)
   T Width() const;
   T Height() const;

   CSizeTplt<T> Size() const;
   CPointTplt<T> &TopLeft();
   CPointTplt<T> &BottomRight();
   const CPointTplt<T> &TopLeft() const;
   const CPointTplt<T> &BottomRight() const;
   CPointTplt<T> CenterPoint() const;
   
   operator const CRectTplt*();
   operator const CRectTplt*() const;
   
   BOOL IsRectEmpty() const;
   BOOL IsRectNull() const;
   BOOL PtInRect(CPointTplt<T> pt) const;
   
   // Operations
   void SetRect(T x1, T y1, T x2, T y2);
   void SetRect(CPointTplt<T> topLeft, CPointTplt<T> bottomRight);
   void SetRectEmpty();
   void CopyRect(const CRectTplt* lpSrcRect);
   BOOL EqualRect(const CRectTplt* pRect) const;
   
   void InflateRect(T x, T y);
   void InflateRect(CSizeTplt<T> size);
   void InflateRect(const CRectTplt* pRect);
   void InflateRect(T l, T t, T r, T b);
   void DeflateRect(T x, T y);
   void DeflateRect(CSizeTplt<T> size);
   void DeflateRect(const CRectTplt* pRect);
   void DeflateRect(T l, T t, T r, T b);
   
   void OffsetRect(T x, T y);
   void OffsetRect(CSizeTplt<T> size);
   void OffsetRect(CPointTplt<T> pt);
   void NormalizeRect();
   
   // operations that fill '*this' with result
   BOOL IntersectRect(const CRectTplt* pRect1, const CRectTplt* pRect2);
   BOOL UnionRect(const CRectTplt* pRect1, const CRectTplt* pRect2);
   
   // Additional Operations
   BOOL operator==(const CRectTplt& rect) const;
   BOOL operator!=(const CRectTplt& rect) const;
   void operator+=(CPointTplt<T> pt);
   void operator+=(CSizeTplt<T> size);
   void operator+=(const CRectTplt* pRect);
   void operator-=(CPointTplt<T> pt);
   void operator-=(CSizeTplt<T> size);
   void operator-=(const CRectTplt* pRect);
   void operator&=(const CRectTplt& rect);
   void operator|=(const CRectTplt& rect);
   void operator*=(T dFactor);
   void operator/=(T dFactor);
   
   // Operators returning CRectTplt values
   CRectTplt operator+(CPointTplt<T> pt) const;
   CRectTplt operator-(CPointTplt<T> pt) const;
   CRectTplt operator+(const CRectTplt* pRect) const;
   CRectTplt operator+(CSizeTplt<T> size) const;
   CRectTplt operator-(CSizeTplt<T> size) const;
   CRectTplt operator-(const CRectTplt* pRect) const;
   CRectTplt operator&(const CRectTplt& rect2) const;
   CRectTplt operator|(const CRectTplt& rect2) const;
   CRectTplt operator*(T dMultiplier) const;
};

// CSizeTplt
template <class T>
inline CSizeTplt<T>::CSizeTplt()
{
   cx = cy = 0;
}

template <class T>
inline CSizeTplt<T>::CSizeTplt(T CX, T CY)
{
   cx = CX;
   cy = CY;
}

template <class T>
inline CSizeTplt<T>::CSizeTplt(CPointTplt<T> pt)
{
   *(CPointTplt*)this = pt;
}

template <class T>
inline BOOL CSizeTplt<T>::operator==(CSizeTplt size) const
{
   return (cx == size.cx && cy == size.cy);
}

template <class T>
inline BOOL CSizeTplt<T>::operator!=(CSizeTplt size) const
{
   return (cx != size.cx || cy != size.cy);
}

template <class T>
inline void CSizeTplt<T>::operator+=(CSizeTplt size)
{
   cx += size.cx; cy += size.cy;
}

template <class T>
inline void CSizeTplt<T>::operator-=(CSizeTplt size)
{
   cx -= size.cx; cy -= size.cy;
}

template <class T>
inline CSizeTplt<T> CSizeTplt<T>::operator+(CSizeTplt size) const
{
   return CSizeTplt(cx + size.cx, cy + size.cy);
}

template <class T>
inline CSizeTplt<T> CSizeTplt<T>::operator-(CSizeTplt size) const
{
   return CSizeTplt(cx - size.cx, cy - size.cy);
}

template <class T>
inline CSizeTplt<T> CSizeTplt<T>::operator-() const
{
   return CSizeTplt(-cx, -cy);
}

template <class T>
inline CPointTplt<T> CSizeTplt<T>::operator+(CPointTplt<T> pt) const
{
   return CPointTplt(cx + pt.x, cy + pt.y);
}

template <class T>
inline CPointTplt<T> CSizeTplt<T>::operator-(CPointTplt<T> pt) const
{
   return CPointTplt(cx - pt.x, cy - pt.y);
}

template <class T>
inline CRectTplt<T> CSizeTplt<T>::operator+(const CRectTplt<T> *pRect) const
{
   return CRectTplt(pRect) + *this;
}

template <class T>
inline CRectTplt<T> CSizeTplt<T>::operator-(const CRectTplt<T> *pRect) const
{
   return CRectTplt(pRect) - *this;
}

// CPointTplt
template <class T>
inline CPointTplt<T>::CPointTplt()
{
   x = y = 0;
}

template <class T>
inline CPointTplt<T>::CPointTplt(T X, T Y)
{
   x = X;
   y = Y;
}

template <class T>
inline CPointTplt<T>::CPointTplt(CSizeTplt<T> size)
{
   *(CSizeTplt*)this = size;
}

template <class T>
inline void CPointTplt<T>::Offset(T xOffset, T yOffset)
{
   x += xOffset;
   y += yOffset;
}

template <class T>
inline void CPointTplt<T>::Offset(CPointTplt pt)
{
   x += pt.x;
   y += pt.y;
}

template <class T>
inline void CPointTplt<T>::Offset(CSizeTplt<T> size)
{
   x += size.cx;
   y += size.cy;
}

template <class T>
inline BOOL CPointTplt<T>::operator==(CPointTplt pt) const
{
   return (x == pt.x && y == pt.y);
}

template <class T>
inline BOOL CPointTplt<T>::operator!=(CPointTplt pt) const
{
   return (x != pt.x || y != pt.y);
}

template <class T>
inline void CPointTplt<T>::operator+=(CSizeTplt<T> size)
{
   x += size.cx;
   y += size.cy;
}

template <class T>
inline void CPointTplt<T>::operator-=(CSizeTplt<T> size)
{
   x -= size.cx;
   y -= size.cy;
}

template <class T>
inline void CPointTplt<T>::operator+=(CPointTplt pt)
{
   x += pt.x;
   y += pt.y;
}

template <class T>
inline void CPointTplt<T>::operator-=(CPointTplt pt)
{
   x -= pt.x;
   y -= pt.y;
}

template <class T>
inline CPointTplt<T> CPointTplt<T>::operator+(CSizeTplt<T> size) const
{
   return CPointTplt(x + size.cx, y + size.cy);
}

template <class T>
inline CPointTplt<T> CPointTplt<T>::operator-(CSizeTplt<T> size) const
{
   return CPointTplt(x - size.cx, y - size.cy);
}

template <class T>
inline CPointTplt<T> CPointTplt<T>::operator-() const
{
   return CPointTplt(-x, -y);
}

template <class T>
inline CPointTplt<T> CPointTplt<T>::operator+(CPointTplt pt) const
{
   return CPointTplt(x + pt.x, y + pt.y);
}

template <class T>
inline CSizeTplt<T> CPointTplt<T>::operator-(CPointTplt pt) const
{
   return CSizeTplt(x - pt.x, y - pt.y);
}

template <class T>
inline CRectTplt<T> CPointTplt<T>::operator+(const CRectTplt<T> *pRect) const
{
   return CRectTplt(pRect) + *this;
}

template <class T>
inline CRectTplt<T> CPointTplt<T>::operator-(const CRectTplt<T> *pRect) const
{
   return CRectTplt(pRect) - *this;
}

template <class T>
inline CPointTplt<T> CPointTplt<T>::operator*(T dMultiplier) const
{
	return CPointTplt(x * dMultiplier, y * dMultiplier);
}

// CRectTplt
template <class T>
inline CRectTplt<T>::CRectTplt()
{
   left = top = right = bottom = 0;
}

template <class T>
inline CRectTplt<T>::CRectTplt(T l, T t, T r, T b)
{
   left = l; top = t; right = r; bottom = b;
}

template <class T>
inline CRectTplt<T>::CRectTplt(const CRectTplt* pRect)
{
   *this = *pRect;
}

template <class T>
inline CRectTplt<T>::CRectTplt(CPointTplt<T> pt, CSizeTplt<T> size)
{
   right  = (left = pt.x) + size.cx;
   bottom = (top  = pt.y) + size.cy;
}

template <class T>
inline CRectTplt<T>::CRectTplt(CPointTplt<T> topLeft, CPointTplt<T> bottomRight)
{
   SetRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

template <class T>
inline T CRectTplt<T>::Width() const
{
   return right - left;
}

template <class T>
inline T CRectTplt<T>::Height() const
{
   return bottom - top;
}

template <class T>
inline CSizeTplt<T> CRectTplt<T>::Size() const
{
   return CSizeTplt(right - left, bottom - top);
}

template <class T>
inline CPointTplt<T> &CRectTplt<T>::TopLeft()
{
   return *((CPointTplt<T>*)this);
}

template <class T>
inline CPointTplt<T> &CRectTplt<T>::BottomRight()
{
   return *((CPointTplt<T>*)this+1);
}

template <class T>
inline const CPointTplt<T> &CRectTplt<T>::TopLeft() const
{
   return *((CPointTplt<T>*)this);
}

template <class T>
inline const CPointTplt<T> &CRectTplt<T>::BottomRight() const
{
   return *((CPointTplt<T>*)this+1);
}

template <class T>
inline CPointTplt<T> CRectTplt<T>::CenterPoint() const
{
   return CPointTplt<T>((left+right)/2, (top+bottom)/2);
}

template <class T>
inline CRectTplt<T>::operator const CRectTplt<T> *()
{
   return this;
}

template <class T>
inline CRectTplt<T>::operator const CRectTplt<T> *() const
{
   return this;
}

template <class T>
inline BOOL CRectTplt<T>::IsRectEmpty() const
{
   return (left==right) || (top==bottom);
}

template <class T>
inline BOOL CRectTplt<T>::IsRectNull() const
{
   return (left == 0 && right == 0 && top == 0 && bottom == 0);
}

template <class T>
inline BOOL CRectTplt<T>::PtInRect(CPointTplt<T> pt) const
{
   CRectTplt r = *this;
   r.NormalizeRect();
   return (pt.x >= r.left && pt.x < r.right && pt.y >= r.top && pt.y < r.bottom);
}

template <class T>
inline void CRectTplt<T>::SetRect(T x1, T y1, T x2, T y2)
{
   left = x1; top = y1; right = x2; bottom = y2;
}

template <class T>
inline void CRectTplt<T>::SetRect(CPointTplt<T> topLeft, CPointTplt<T> bottomRight)
{
   SetRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

template <class T>
inline void CRectTplt<T>::SetRectEmpty()
{
   left = top = right = bottom = 0;
}

template <class T>
inline void CRectTplt<T>::CopyRect(const CRectTplt* pRect)
{
   *this = *pRect;
}

template <class T>
inline BOOL CRectTplt<T>::EqualRect(const CRectTplt* pRect) const
{
   CRectTplt r1 = *this;
   r1.NormalizeRect();
   CRectTplt r2 = *pRect;
   r2.NormalizeRect();

   return memcmp(&r1, &r2, sizeof(r1))==0;
}

template <class T>
inline void CRectTplt<T>::InflateRect(T x, T y)
{
   left   -= x;
   right  += x;
   top    -= y;
   bottom += y;
}

template <class T>
inline void CRectTplt<T>::InflateRect(CSizeTplt<T> size)
{
   InflateRect(size.cx, size.cy);
}

template <class T>
inline void CRectTplt<T>::DeflateRect(T x, T y)
{
   InflateRect(-x, -y);
}

template <class T>
inline void CRectTplt<T>::DeflateRect(CSizeTplt<T> size)
{
   InflateRect(-size.cx, -size.cy);
}

template <class T>
inline void CRectTplt<T>::OffsetRect(T x, T y)
{
   left   += x;
   right  += x;
   top    += y;
   bottom += y;
}

template <class T>
inline void CRectTplt<T>::OffsetRect(CPointTplt<T> pt)
{
   OffsetRect(pt.x, pt.y);
}

template <class T>
inline void CRectTplt<T>::OffsetRect(CSizeTplt<T> size)
{
   OffsetRect(size.cx, size.cy);
}

template <class T>
inline BOOL CRectTplt<T>::IntersectRect(const CRectTplt* pR1, const CRectTplt* pR2)
{
   if (pR2->right <= pR1->left)
      return FALSE;
   if (pR2->left >= pR1->right)
      return FALSE;
   if (pR2->bottom <= pR1->top)
      return FALSE;
   if (pR2->top >= pR1->bottom)
      return FALSE;
   left  = max(pR1->left, pR2->left);
   top   = max(pR1->top, pR2->top);
   right = min(pR1->right, pR2->right);
   bottom= min(pR1->bottom, pR2->bottom);
   return TRUE;
}

template <class T>
inline BOOL CRectTplt<T>::UnionRect(const CRectTplt* pR1, const CRectTplt* pR2)
{
   left  = min(pR1->left, pR2->left);
   top   = min(pR1->top, pR2->top);
   right = max(pR1->right, pR2->right);
   bottom= max(pR1->bottom, pR2->bottom);
   return !IsRectEmpty();
}

template <class T>
inline BOOL CRectTplt<T>::operator==(const CRectTplt& rect) const
{
   return EqualRect(&rect);
}

template <class T>
inline BOOL CRectTplt<T>::operator!=(const CRectTplt& rect) const
{
   return !EqualRect(&rect);
}

template <class T>
inline void CRectTplt<T>::operator+=(CPointTplt<T> pt)
{
   OffsetRect(pt.x, pt.y);
}

template <class T>
inline void CRectTplt<T>::operator+=(CSizeTplt<T> size)
{
   OffsetRect(size.cx, size.cy);
}

template <class T>
inline void CRectTplt<T>::operator+=(const CRectTplt* pRect)
{
   InflateRect(pRect);
}

template <class T>
inline void CRectTplt<T>::operator-=(CPointTplt<T> pt)
{
   OffsetRect(-pt.x, -pt.y);
}

template <class T>
inline void CRectTplt<T>::operator-=(CSizeTplt<T> size)
{
   OffsetRect(-size.cx, -size.cy);
}

template <class T>
inline void CRectTplt<T>::operator-=(const CRectTplt* pRect)
{
   DeflateRect(pRect);
}

template <class T>
inline void CRectTplt<T>::operator&=(const CRectTplt& rect)
{
   IntersectRect(this, &rect);
}

template <class T>
inline void CRectTplt<T>::operator|=(const CRectTplt& rect)
{
   UnionRect(this, &rect);
}

template <class T>
inline void CRectTplt<T>::operator*=(T dFactor)
{
   left   *= dFactor;
   top    *= dFactor;
   right  *= dFactor;
   bottom *= dFactor;
}

template <class T>
inline void CRectTplt<T>::operator/=(T dFactor)
{
   ASSERT(dFactor != 0);
   left   /= dFactor;
   top    /= dFactor;
   right  /= dFactor;
   bottom /= dFactor;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator+(CPointTplt<T> pt) const
{
   CRectTplt rect(*this);
   rect.OffsetRect(pt.x, pt.y);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator-(CPointTplt<T> pt) const
{
   CRectTplt rect(*this);
   rect.OffsetRect(-pt.x, -pt.y);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator+(CSizeTplt<T> size) const
{
   CRectTplt rect(*this);
   rect.OffsetRect(size.cx, size.cy);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator-(CSizeTplt<T> size) const
{
   CRectTplt rect(*this);
   rect.OffsetRect(-size.cx, -size.cy);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator+(const CRectTplt* pRect) const
{
   CRectTplt rect(this);
   rect.InflateRect(pRect);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator-(const CRectTplt* pRect) const
{
   CRectTplt rect(this);
   rect.DeflateRect(pRect);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator&(const CRectTplt& rect2) const
{
   CRectTplt rect;
   rect.IntersectRect(this, &rect2);
   return rect;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator|(const CRectTplt& rect2) const
{
   CRectTplt rect;
   rect.UnionRect(this, &rect2);
   return rect;
}

template <class T>
inline void CRectTplt<T>::NormalizeRect()
{
	if (left > right)
	{
		T dTemp = left;
		left  = right;
		right = dTemp;
	}
	if (top > bottom)
	{
		T dTemp = top;
		top    = bottom;
		bottom = dTemp;
	}
}

template <class T>
inline void CRectTplt<T>::InflateRect(const CRectTplt* pRect)
{
   InflateRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
}

template <class T>
inline void CRectTplt<T>::InflateRect(T l, T t, T r, T b)
{
	left   -= l;
	top    -= t;
	right  += r;
	bottom += b;
}

template <class T>
inline void CRectTplt<T>::DeflateRect(const CRectTplt* pRect)
{
   DeflateRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
}

template <class T>
inline void CRectTplt<T>::DeflateRect(T l, T t, T r, T b)
{
	left   += l;
	top    += t;
	right  -= r;
	bottom -= b;
}

template <class T>
inline CRectTplt<T> CRectTplt<T>::operator *(T dMultiplier) const
{
	return CRectTplt(left  * dMultiplier, top    * dMultiplier,
		               right * dMultiplier, bottom * dMultiplier);
}

typedef CSizeTplt<float>   CFloatSize;
typedef CPointTplt<float>  CFloatPoint;
typedef CRectTplt<float>   CFloatRect;

typedef CSizeTplt<double>  CDoubleSize;
typedef CPointTplt<double> CDoublePoint;
typedef CRectTplt<double>  CDoubleRect;

#endif      // INC_FLOATRECT_HPP

