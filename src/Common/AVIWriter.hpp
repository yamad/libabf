//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  AVIWriter.hpp
// PURPOSE: Contains an object for writing AVI files using VideoForWindows.
// AUTHOR:  BHI  Apr 2001
//
#ifndef INC_AVIWRITER_HPP
#define INC_AVIWRITER_HPP

#pragma warning(disable: 4201)
#include <vfw.h>
#include <memory.h>

struct AVIProps
{
   bool  bCaptions;        // True if text captions are to be saved too.
   DWORD dwRate;           // Rate in frames per second.
   BITMAPINFOHEADER BIH;   // DIB header.

   AVIProps()
   {
      memset(this, 0, sizeof(*this));
      BIH.biSize   = sizeof(BIH);
      BIH.biPlanes = 1;
   }
};

class CAVIWriter
{
private:
   DWORD                m_dwVersion;
   AVIProps             m_Props;
   PAVIFILE             m_pFile;
   PAVISTREAM           m_psImages;
   PAVISTREAM           m_psCompressed;
   PAVISTREAM           m_psCaptions;
   AVICOMPRESSOPTIONS   m_CompressOptions;
   bool                 m_bFreeOptions;   

private:
   mutable HRESULT m_hrLastError;
   mutable char    m_strLastError[1024];
   bool SetLastError(HRESULT hr) const;
   void FreeOptions();

public:
   CAVIWriter();
   ~CAVIWriter();

   // Check to see if the version is 1.1 or later.
   bool IsOK() const { return HIWORD(m_dwVersion) >= 0x010a; }

   bool Open(LPCSTR pszFilename, const AVIProps *pProps, CLSID *pHandler=NULL);
   void Close();

   // Opens a dialog for the user to choose the codec and set compression options.
   bool PromptForOptions(HWND hwndParent);

   // Return the current compression options.
   const AVICOMPRESSOPTIONS *GetOptions() const { return &m_CompressOptions; }

   // Sets the compression options.
   void SetOptions(const AVICOMPRESSOPTIONS *pOptions);

   // Write an image to the file.
   bool Write(UINT uTime, BYTE *pbyData, LPCSTR pszCaption=NULL);

   // Get the last error code.
   HRESULT GetLastError(char * psErrorString = NULL, int nBufferSize = 0) const;
};

#endif   // INC_AVIWRITER_HPP
