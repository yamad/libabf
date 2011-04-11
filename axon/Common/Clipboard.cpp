//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Clipboard.CPP
// PURPOSE: 
// AUTHOR:  BHI  Dec 1998
//

#include "wincpp.hpp"
#include "Clipboard.hpp"

//###############################################################################################
//###############################################################################################
//###
//###  CLASS:   Clipboard
//###  PURPOSE: Class wrapper for WIN32 clipboard functions
//###
//###############################################################################################
//###############################################################################################

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
Clipboard::Clipboard()
{
   MEMBERASSERT();
   m_bOpen = FALSE;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
Clipboard::~Clipboard()
{
   MEMBERASSERT();
   Close();
}

//===============================================================================================
// FUNCTION: Open
// PURPOSE:  Open the clipboard.
//
BOOL Clipboard::Open(HWND hwndParent)
{
   MEMBERASSERT();
   Close();
   m_bOpen = ::OpenClipboard(hwndParent);
   return m_bOpen;
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Close the clipboard.
//
BOOL Clipboard::Close()
{
   MEMBERASSERT();
   
   if (!m_bOpen)
      return TRUE;

   return ::CloseClipboard();
}

//===============================================================================================
// FUNCTION: IsOpen
// PURPOSE:  Returns TRUE if the clipboard was opened successfully.
//
BOOL Clipboard::IsOpen() const
{
   MEMBERASSERT();
   return m_bOpen;
}

//===============================================================================================
// FUNCTION: Cast to BOOL
// PURPOSE:  Returns TRUE if clipboard is open.
//
Clipboard::operator BOOL() const
{
   MEMBERASSERT();
   return IsOpen();
}

//===============================================================================================
// FUNCTION: Empty
// PURPOSE:  Emptys the clipboard.
//
BOOL Clipboard::Empty()
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::EmptyClipboard();
}

//===============================================================================================
// FUNCTION: SetData
// PURPOSE:  Sets a data block onto the clipboard.
//
BOOL Clipboard::SetData(UINT uFormat, HANDLE hMemory)
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::SetClipboardData( uFormat, hMemory ) != NULL;
}

//===============================================================================================
// FUNCTION: SetData
// PURPOSE:  Sets a data block onto the clipboard.
//
BOOL Clipboard::SetData(LPCSTR pszText)
{
   MEMBERASSERT();
   LPSZASSERT(pszText);
   ASSERT(IsOpen());

   UINT uSize = strlen(pszText)+1;
   ClipboardData Data(uSize);
   if (!Data)
      return FALSE;

   AXU_strncpyz(Data, pszText, uSize);
   return SetData( CF_TEXT, Data.Release() );
}

//===============================================================================================
// FUNCTION: GetData
// PURPOSE:  Gets data or a particular format from the clipboard.
//
HANDLE Clipboard::GetData(UINT uFormat) const
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::GetClipboardData(uFormat);
}

//===============================================================================================
// FUNCTION: CountFormats
// PURPOSE:  Returns the number of formats available.
//
int Clipboard::CountFormats() const
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::CountClipboardFormats();
}

//===============================================================================================
// FUNCTION: EnumFormats
// PURPOSE:  Enumerates the available clipboard formats.
//
UINT Clipboard::EnumFormats(UINT uFormat) const
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::EnumClipboardFormats(uFormat);
}

//===============================================================================================
// FUNCTION: IsFormatAvailable
// PURPOSE:  Queries if a particular format is available.
//
BOOL Clipboard::IsFormatAvailable(UINT uFormat) const
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::IsClipboardFormatAvailable(uFormat);
}

//===============================================================================================
// FUNCTION: GetFormatName
// PURPOSE:  Returns the name of a given format.
//
int Clipboard::GetFormatName(UINT uFormat, LPSTR lpszFormatName, int cchMaxCount) const
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::GetClipboardFormatName(uFormat, lpszFormatName, cchMaxCount);
}

//===============================================================================================
// FUNCTION: GetPriorityFormat
// PURPOSE:  Gets the available formats in a prioritized list.
//
int Clipboard::GetPriorityFormat(UINT *paFormatPriorityList, int cFormats) const
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::GetPriorityClipboardFormat(paFormatPriorityList, cFormats);
}

//===============================================================================================
// FUNCTION: RegisterFormat
// PURPOSE:  Register a new format.
//
UINT Clipboard::RegisterFormat(LPCSTR pszFormat)
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   return ::RegisterClipboardFormat(pszFormat);
}

//###############################################################################################
//###############################################################################################
//###
//###  CLASS:   ClipboardData
//###  PURPOSE: Class wrapper for a global memory block, compatible with the WIN32 clipboard functions
//###
//###############################################################################################
//###############################################################################################

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
ClipboardData::ClipboardData(UINT uBytes)
{
   m_hData = NULL;
   m_pData = NULL;
   Alloc(uBytes);
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
ClipboardData::~ClipboardData()
{
   Free();
}

//===============================================================================================
// FUNCTION: operator LPSTR()
// PURPOSE:  Cast operator for access to the held memory block.
//
ClipboardData::operator LPSTR()
{
   return m_pData;
}

//===============================================================================================
// FUNCTION: Alloc
// PURPOSE:  Allocates a block of memory and locks it ready for access.
//
BOOL ClipboardData::Alloc(UINT uBytes)
{
   Free();

   // Allocate the global memory.
   m_hData = GlobalAlloc( GHND, uBytes );
   m_pData = (LPSTR)GlobalLock(m_hData);

   if (!m_pData)
   {
      Free();
      return FALSE;
   }
   return TRUE;
};

//===============================================================================================
// FUNCTION: Free
// PURPOSE:  Frees any held memory.
//
void ClipboardData::Free()
{
   if (m_pData)
   {
      GlobalUnlock(m_hData);
      m_pData = NULL;
   }
   if (m_hData)
   {
      GlobalFree(m_hData);
      m_hData = NULL;
   }
}

//===============================================================================================
// FUNCTION: Release
// PURPOSE:  Releases and returns any held memory so that it does not get freed by the destructor.
//
HGLOBAL ClipboardData::Release()
{
   // Unlock the memory block if it is locked
   if (m_pData)
   {
      GlobalUnlock(m_hData);
      m_pData = NULL;
   }

   // Save the memory handle and set it to NULL.
   HGLOBAL rval = m_hData;
   m_hData = NULL;
   return rval;
}
