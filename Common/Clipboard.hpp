//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  Clipboard.HPP
// PURPOSE: 
// AUTHOR:  BHI  Dec 1998
//

#ifndef INC_CLIPBOARD_HPP
#define INC_CLIPBOARD_HPP

#pragma once

//###############################################################################################
//###############################################################################################
//###
//###  CLASS:   Clipboard
//###  PURPOSE: Class wrapper for WIN32 clipboard functions
//###
//###############################################################################################
//###############################################################################################

class Clipboard
{
private:   // Attributes
   BOOL m_bOpen;

private:   // Unimplemented copy functions.
   Clipboard(const Clipboard &);
   const Clipboard &operator=(const Clipboard &);

private:   // Private functions

public:    // Public interface
   Clipboard();
   ~Clipboard();

   // Open/close the clipboard.
   BOOL Open(HWND hwndParent);
   BOOL Close();

   // Returns TRUE if the clipboard was opened successfully.
   BOOL IsOpen() const;
   operator BOOL() const;

   // Empty the clipboard.
   BOOL Empty();

   // Sets a data block onto the clipboard.
   BOOL SetData(UINT uFormat, HANDLE hMemory);
   BOOL SetData(LPCSTR pszText);

   // Gets data or a particular format from the clipboard.
   HANDLE GetData(UINT uFormat) const;

   // Returns the number of formats available.
   int  CountFormats() const;

   // Enumerates the available clipboard formats.
   UINT EnumFormats(UINT uFormat) const;

   // Queries if a particular format is available.
   BOOL IsFormatAvailable(UINT uFormat) const;

   // Returns the name of a given format.
   int  GetFormatName(UINT uFormat, LPSTR lpszFormatName, int cchMaxCount) const;

   // Gets the available formats in a prioritized list.
   int  GetPriorityFormat(UINT *paFormatPriorityList, int cFormats) const;

   // Register a new format.
   UINT RegisterFormat(LPCSTR pszFormat);
};

//###############################################################################################
//###############################################################################################
//###
//###  CLASS:   ClipboardData
//###  PURPOSE: Class wrapper for a global memory block, compatible with the WIN32 clipboard functions
//###
//###############################################################################################
//###############################################################################################

class ClipboardData
{
private:
   HGLOBAL m_hData;
   LPSTR   m_pData;

public:
   ClipboardData(UINT uBytes=0);
   ~ClipboardData();

   // Cast operator for access to the held memory block.
   operator LPSTR();

   // Allocates a block of memory and locks it ready for access.
   BOOL Alloc(UINT uBytes);

   // Frees any held memory.
   void Free();

   // Releases and returns any held memory so that it does not get freed by the destructor.
   HANDLE Release();
};

#endif      // INC_CLIPBOARD_HPP

