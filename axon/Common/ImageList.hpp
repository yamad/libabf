//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  ImageList.HPP
// PURPOSE: 
// AUTHOR:  BHI  Dec 1998
//

#ifndef CImageList
#ifndef INC_IMAGELIST_HPP
#define INC_IMAGELIST_HPP

#pragma once

class CImageList
{
private:   // Attributes
   HIMAGELIST m_hImageList;

private:   // Unimplemented copy functions.
   CImageList(const CImageList &);
   const CImageList &operator=(const CImageList &);

private:   // Private functions

public:    // Public interface
   CImageList();
   ~CImageList();
   operator HIMAGELIST() const;

   void DeleteImageList();
   BOOL Attach(HIMAGELIST hImageList);
   HIMAGELIST Release();

   BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow);
   BOOL Create(HINSTANCE hInstance, UINT uBitmapID, int cx, int nGrow, COLORREF crMask);
   BOOL Create(HINSTANCE hInstance, LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask);
   BOOL Create(HIMAGELIST hImageList1, int nImage1,
                           HIMAGELIST hImageList2, int nImage2, int dx, int dy);

   int GetImageCount() const;
   int Add(HBITMAP hbmImage, HBITMAP hbmMask);
   int Add(HBITMAP hbmImage, COLORREF crMask);
   BOOL Remove(int nImage);
   BOOL Replace(int nImage, HBITMAP hbmImage, HBITMAP hbmMask);

   int Add(HICON hIcon);
   int AddSmallIcon(HINSTANCE hInstance, UINT uID);
   int AddLargeIcon(HINSTANCE hInstance, UINT uID);
   int Replace(int nImage, HICON hIcon);
   HICON ExtractIcon(int nImage);

   BOOL Draw(HDC hDC, int nImage, POINT pt, UINT nStyle);
   COLORREF SetBkColor(COLORREF cr);
   COLORREF GetBkColor() const;
   BOOL SetOverlayImage(int nImage, int nOverlay);
   BOOL GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const;

   BOOL BeginDrag(int nImage, CPoint ptHotSpot);
   void EndDrag();

   BOOL DragMove(CPoint pt);
   BOOL SetDragCursorImage(int nDrag, CPoint ptHotSpot);
   BOOL DragShowNolock(BOOL bShow);
   HIMAGELIST GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot);
   BOOL DragEnter(HWND hwndLock, CPoint point);
   BOOL DragLeave(HWND hwndLock);
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CImageList::CImageList()
{
   MEMBERASSERT();
   m_hImageList = NULL;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
inline CImageList::~CImageList()
{
   MEMBERASSERT();
   DeleteImageList();
}

inline CImageList::operator HIMAGELIST() const
{
   MEMBERASSERT();
   return m_hImageList;
}

inline void CImageList::DeleteImageList()
{
   MEMBERASSERT();
   if (m_hImageList)
      ImageList_Destroy(m_hImageList);
   m_hImageList = NULL;
}

inline BOOL CImageList::Attach(HIMAGELIST hImageList)
{
   MEMBERASSERT();
	if (m_hImageList)
      DeleteImageList();
   m_hImageList = hImageList;
   return (hImageList != NULL);
}

inline HIMAGELIST CImageList::Release()
{
   MEMBERASSERT();
   HIMAGELIST rval = m_hImageList;
   m_hImageList = NULL;
   return rval;
}

inline BOOL CImageList::Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow)
{
   MEMBERASSERT();
   return Attach(ImageList_Create(cx, cy, nFlags, nInitial, nGrow));
}

inline BOOL CImageList::Create(HINSTANCE hInstance, UINT uBitmapID, int cx, int nGrow, COLORREF crMask)
{
   MEMBERASSERT();
   ASSERT(HIWORD(uBitmapID) == 0);
   return Create(hInstance, MAKEINTRESOURCE(uBitmapID), cx, nGrow, crMask);
}

inline BOOL CImageList::Create(HINSTANCE hInstance, LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask)
{
   MEMBERASSERT();
   return Attach(ImageList_LoadBitmap(hInstance, lpszBitmapID, cx, nGrow, crMask));
}

inline BOOL CImageList::Create(HIMAGELIST hImageList1, int nImage1,
                               HIMAGELIST hImageList2, int nImage2, int dx, int dy)
{
   MEMBERASSERT();
   return Attach(ImageList_Merge(hImageList1, nImage1, hImageList2, nImage2, dx, dy));
}

inline int CImageList::GetImageCount() const
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_GetImageCount(m_hImageList);
}

inline int CImageList::Add(HBITMAP hbmImage, HBITMAP hbmMask)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_Add(m_hImageList, hbmImage, hbmMask);
}

inline int CImageList::Add(HBITMAP hbmImage, COLORREF crMask)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_AddMasked(m_hImageList, hbmImage, crMask);
}

inline BOOL CImageList::Remove(int nImage)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_Remove(m_hImageList, nImage);
}

inline BOOL CImageList::Replace(int nImage, HBITMAP hbmImage, HBITMAP hbmMask)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_Replace(m_hImageList, nImage, hbmImage, hbmMask);
}

inline int CImageList::Add(HICON hIcon)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_AddIcon(m_hImageList, hIcon);
}

inline int CImageList::AddSmallIcon(HINSTANCE hInstance, UINT uID)
{
   return Add((HICON)LoadImage(hInstance, MAKEINTRESOURCE(uID), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
}

inline int CImageList::AddLargeIcon(HINSTANCE hInstance, UINT uID)
{
   return Add(LoadIcon(hInstance, MAKEINTRESOURCE(uID)));
}

inline int CImageList::Replace(int nImage, HICON hIcon)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_ReplaceIcon(m_hImageList, nImage, hIcon);
}

inline HICON CImageList::ExtractIcon(int nImage)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_ExtractIcon(NULL, m_hImageList, nImage);
}

inline BOOL CImageList::Draw(HDC hDC, int nImage, POINT pt, UINT nStyle)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   ASSERT(hDC != NULL);
   return ImageList_Draw(m_hImageList, nImage, hDC, pt.x, pt.y, nStyle);
}

inline COLORREF CImageList::SetBkColor(COLORREF cr)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_SetBkColor(m_hImageList, cr);
}

inline COLORREF CImageList::GetBkColor() const
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_GetBkColor(m_hImageList);
}

inline BOOL CImageList::SetOverlayImage(int nImage, int nOverlay)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_SetOverlayImage(m_hImageList, nImage, nOverlay);
}

inline BOOL CImageList::GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_GetImageInfo(m_hImageList, nImage, pImageInfo);
}

inline BOOL CImageList::BeginDrag(int nImage, CPoint ptHotSpot)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_BeginDrag(m_hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
}

inline void CImageList::EndDrag()
{
   MEMBERASSERT();
   ImageList_EndDrag();
}

inline BOOL CImageList::DragMove(CPoint pt)
{
   MEMBERASSERT();
   return ImageList_DragMove(pt.x, pt.y);
}

inline BOOL CImageList::SetDragCursorImage(int nDrag, CPoint ptHotSpot)
{
   MEMBERASSERT();
   ASSERT(m_hImageList != NULL); 
   return ImageList_SetDragCursorImage(m_hImageList, nDrag, ptHotSpot.x, ptHotSpot.y);
}

inline BOOL CImageList::DragShowNolock(BOOL bShow)
{
   MEMBERASSERT();
   return ImageList_DragShowNolock(bShow);
}

inline HIMAGELIST CImageList::GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot)
{
   MEMBERASSERT();
   return ImageList_GetDragImage(lpPoint, lpPointHotSpot);
}

inline BOOL CImageList::DragEnter(HWND hwndLock, CPoint point)
{
   MEMBERASSERT();
   return ImageList_DragEnter(hwndLock, point.x, point.y);
}

inline BOOL CImageList::DragLeave(HWND hwndLock)
{
   MEMBERASSERT();
   return ImageList_DragLeave(hwndLock);
}

#endif      // INC_IMAGELIST_HPP
#endif      // CImageList

