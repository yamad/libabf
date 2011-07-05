//***********************************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  AVIWriter.cpp
// PURPOSE: Contains an object for writing AVI files using VideoForWindows.
// AUTHOR:  BHI  Apr 2001
//
#include "wincpp.hpp"
#include "AVIWriter.hpp"

#pragma comment(lib, "VFW32.lib")

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CAVIWriter::CAVIWriter()
{
   m_pFile        = NULL;
   m_psImages     = NULL;
   m_psCompressed = NULL;
   m_psCaptions   = NULL;
   m_hrLastError  = 0;
   m_strLastError[0] = 0;
   m_dwVersion    = VideoForWindowsVersion();

   memset(&m_CompressOptions, 0, sizeof(m_CompressOptions));
   m_CompressOptions.fccType         = streamtypeVIDEO;
   m_CompressOptions.fccHandler      = mmioFOURCC('m','s','v','c');
   m_CompressOptions.dwFlags         = AVICOMPRESSF_VALID | AVICOMPRESSF_KEYFRAMES;
   m_CompressOptions.dwKeyFrameEvery = 3;
   m_CompressOptions.dwQuality       = 8500;             // Quality in percent * 100.
   m_bFreeOptions = false;
   if (!IsOK())
   {
      SetLastError(AVIERR_UNSUPPORTED);
      return;
   }
   AVIFileInit();
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup -- calls Close so explicit closing is not strictly required.
//
CAVIWriter::~CAVIWriter()
{
   if (!IsOK())
      return;

   Close();

   // Free any options.
   FreeOptions();

   // Close the library.
   AVIFileExit();
}

//===============================================================================================
// FUNCTION: PromptForOptions
// PURPOSE:  Opens a dialog for the user to choose the codec and set compression options.
//
bool CAVIWriter::PromptForOptions(HWND hwndParent)
{
   AVICOMPRESSOPTIONS *aopts = &m_CompressOptions;
   if (!AVISaveOptions(hwndParent, 0, 1, &m_psImages, &aopts))
      return SetLastError(AVIERR_USERABORT);

   m_bFreeOptions = true;
   return true;
}

//===============================================================================================
// FUNCTION: FreeOptions
// PURPOSE:  Frees any options allocated by the codec.
//
void CAVIWriter::FreeOptions()
{
   MEMBERASSERT();
   
   // Free any options.
   if (m_bFreeOptions)
   {
      AVICOMPRESSOPTIONS *aopts = &m_CompressOptions;
      AVISaveOptionsFree(1, &aopts);
   }
   m_CompressOptions.lpParms = NULL;
   m_CompressOptions.cbParms = 0;
   m_bFreeOptions = false;
}

//===============================================================================================
// FUNCTION: SetOptions
// PURPOSE:  Sets the compression options.
//
void CAVIWriter::SetOptions(const AVICOMPRESSOPTIONS *pOptions)
{
   MEMBERASSERT();

   // Must be called before the compressor is opened.
   ASSERT(m_psCompressed==NULL);

   // Free any currently held options if the new ones are different
   if ((pOptions->cbParms != m_CompressOptions.cbParms) ||
       (pOptions->lpParms != m_CompressOptions.lpParms))
      FreeOptions();

   // Save a copy of the options.
   m_CompressOptions = *pOptions;
}

//===============================================================================================
// FUNCTION: Open
// PURPOSE:  Opens an AVI file for writing and sets up the streams.
//
bool CAVIWriter::Open(LPCSTR pszFilename, const AVIProps *pProps, CLSID *pHandler)
{
   if (!IsOK())
      return false;

   Close();
   m_Props = *pProps;

   // Open the movie file for writing....
   HRESULT hr = AVIFileOpen(&m_pFile, pszFilename, OF_WRITE | OF_CREATE, pHandler);
   if (hr != AVIERR_OK)
      return SetLastError(hr);

   // Open the video stream...
   {
      AVISTREAMINFO strhdr = { 0 };
      strhdr.fccType                = streamtypeVIDEO;   // stream type
      strhdr.fccHandler             = 0;                 // handler key code.
      strhdr.dwScale                = 1;                 // rate/scale = FPS.
      strhdr.dwRate                 = m_Props.dwRate;
      strhdr.dwSuggestedBufferSize  = m_Props.BIH.biSizeImage;
      SetRect(&strhdr.rcFrame, 0, 0, m_Props.BIH.biWidth, m_Props.BIH.biHeight);

      // And create the stream;
      hr = AVIFileCreateStream(m_pFile,       // file pointer
                               &m_psImages,   // returned stream pointer
                               &strhdr);      // stream header
      if (hr != AVIERR_OK)
         return SetLastError(hr);
   }

   // Create the stream for the captions if necessary.
   if (m_Props.bCaptions)
   {
      AVISTREAMINFO strhdr = { 0 };
      strhdr.fccType                = streamtypeTEXT;
      strhdr.fccHandler             = mmioFOURCC('D','R','A','W');
      strhdr.dwScale                = 1;
      strhdr.dwRate                 = m_Props.dwRate;
      strhdr.dwSuggestedBufferSize  = 0;
      SetRect(&strhdr.rcFrame, 0, m_Props.BIH.biHeight, m_Props.BIH.biWidth, m_Props.BIH.biHeight+16);

      hr = AVIFileCreateStream(m_pFile, &m_psCaptions, &strhdr);
      if (hr != AVIERR_OK)
         return SetLastError(hr);

      // The following format came from Microsoft sample code (???)
      DWORD dwTextFormat = sizeof(dwTextFormat);
      hr = AVIStreamSetFormat(m_psCaptions, 0, &dwTextFormat, sizeof(dwTextFormat));
      if (hr != AVIERR_OK)
         return SetLastError(hr);
   }
   return true;
}

//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Writes a DIB to the file,  with an optionsl caption.
//
bool CAVIWriter::Write(UINT uTime, BYTE *pbyData, LPCSTR pszCaption)
{
   if (!m_psCompressed)
   {
      HRESULT hr = AVIMakeCompressedStream(&m_psCompressed, m_psImages, &m_CompressOptions, NULL);
      if (hr != AVIERR_OK)
         return SetLastError(hr);

      // If the number of colors is not zero, then we need the colortable in the Props structure.
      ASSERT(m_Props.BIH.biClrUsed==0);

      hr = AVIStreamSetFormat(m_psCompressed, 0,
                              &m_Props.BIH,           // stream format
                              m_Props.BIH.biSize +    // format size
                              m_Props.BIH.biClrUsed * sizeof(RGBQUAD));
      if (hr != AVIERR_OK)
      {
         // Close the image stream if open.
         AVIStreamClose(m_psCompressed);
         m_psCompressed = NULL;
         return SetLastError(hr);
      }
   }

   HRESULT hr = AVIStreamWrite(m_psCompressed,           // stream pointer
                               uTime,                    // time of this frame
                               1,                        // number to write
                               pbyData,                  // pointer to data
                               m_Props.BIH.biSizeImage,  // size of this frame
                               AVIIF_KEYFRAME,           // flags....
                               NULL, NULL);
   if (hr != AVIERR_OK)
      return SetLastError(hr);

   ASSERT(!pszCaption || m_Props.bCaptions);
   if (pszCaption)
   {
      WPTRASSERT(m_psCaptions);
      hr = AVIStreamWrite(m_psCaptions, uTime, 1, (void *)pszCaption, strlen(pszCaption)+1,
                          AVIIF_KEYFRAME, NULL, NULL);
      if (hr != AVIERR_OK)
         return SetLastError(hr);
   }
   return true;
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Closes an open AVI file.
//
void CAVIWriter::Close()
{
   // If no file held, nothing to close.
   if (!IsOK() || !m_pFile)
      return;

   // Close the caption stream if open.
   if (m_psCaptions)
      AVIStreamClose(m_psCaptions);
   m_psCaptions = NULL;

   // Close the compressed stream if open.
   if (m_psCompressed)
      AVIStreamClose(m_psCompressed);
   m_psCompressed = NULL;

   // Close the image stream if open.
   if (m_psImages)
      AVIStreamClose(m_psImages);
   m_psImages = NULL;

   // Release the file (superceeds AVIFileClose)
   AVIFileRelease(m_pFile);
   m_pFile = NULL;
}

//===============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  
//
HRESULT CAVIWriter::GetLastError(char * psErrorString, int nBufferSize) const 
{ 
   if (psErrorString)  
   {
      WARRAYASSERT(psErrorString, nBufferSize);
      AXU_strncpyz(psErrorString, m_strLastError, nBufferSize);
   }

   return m_hrLastError;
}


//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Convenience function to display errors and return false.
//
bool CAVIWriter::SetLastError(HRESULT hr) const
{
   if (hr == AVIERR_UNSUPPORTED)
      AXU_strncpyz(m_strLastError, "The current operation has been cancelled.", sizeof(m_strLastError));            
   else if (hr == AVIERR_BADFORMAT)
      AXU_strncpyz(m_strLastError, "The file has a bad format.", sizeof(m_strLastError));                  
   else if (hr == AVIERR_MEMORY)
      AXU_strncpyz(m_strLastError, "Insufficient memory to complete the operation.", sizeof(m_strLastError));                        
   else if (hr == AVIERR_INTERNAL)
      AXU_strncpyz(m_strLastError, "Internal unknown error.", sizeof(m_strLastError));                              
   else if (hr == AVIERR_BADFLAGS)
      AXU_strncpyz(m_strLastError, "The flag options are invalid.", sizeof(m_strLastError));                              
   else if (hr == AVIERR_BADPARAM)
      AXU_strncpyz(m_strLastError, "The parameter options are invalid.", sizeof(m_strLastError));                                    
   else if (hr == AVIERR_BADSIZE)
      AXU_strncpyz(m_strLastError, "The file has an invalid size.", sizeof(m_strLastError));                                          
   else if (hr == AVIERR_BADHANDLE)
      AXU_strncpyz(m_strLastError, "The file handle is invalid.", sizeof(m_strLastError));      
   else if (hr == AVIERR_FILEREAD)
      AXU_strncpyz(m_strLastError, "The file can not be read. Please make sure that the file is a supported AVI file or is not corrupt", sizeof(m_strLastError));            
   else if (hr == AVIERR_FILEWRITE)
      AXU_strncpyz(m_strLastError, "The file can not be written. Please make sure that the file is not already in use or that the disk is full.", sizeof(m_strLastError));            
   else if (hr == AVIERR_FILEOPEN)
      AXU_strncpyz(m_strLastError, "The file can not be opened. Please make sure that the file is not already in use.", sizeof(m_strLastError));            
   else if (hr == AVIERR_COMPRESSOR)
      AXU_strncpyz(m_strLastError, "The compressor used is invalid or is not supported.", sizeof(m_strLastError));            
   else if (hr == AVIERR_NOCOMPRESSOR)
      AXU_strncpyz(m_strLastError, "No compressor is available to read the image correctly.", sizeof(m_strLastError));                  
   else if (hr == AVIERR_READONLY)
      AXU_strncpyz(m_strLastError, "The file can not be accessed. It is marked as read only.", sizeof(m_strLastError));                        
   else if (hr == AVIERR_NODATA)
      AXU_strncpyz(m_strLastError, "The file contains no data.", sizeof(m_strLastError));                        
   else if (hr == AVIERR_BUFFERTOOSMALL)
      AXU_strncpyz(m_strLastError, "The file is too big to open with the available buffer sizes.", sizeof(m_strLastError));                              
   else if (hr == AVIERR_CANTCOMPRESS)
      AXU_strncpyz(m_strLastError, "The file can not be compressed with the available options.", sizeof(m_strLastError));                              
   else if (hr == AVIERR_USERABORT) 
      AXU_strncpyz(m_strLastError, "The current operation has been aborted.", sizeof(m_strLastError));                                    
   else if (hr == REGDB_E_CLASSNOTREG)
      AXU_strncpyz(m_strLastError, "The type of file specified does not have a supported handler to process it.", sizeof(m_strLastError)); // in AVIFileOpen                                     
   else // if (hr == AVIERR_ERROR)
      AXU_strncpyz(m_strLastError, "An unknown error has occurred.", sizeof(m_strLastError));                                    

   m_hrLastError = hr;
   return (hr == S_OK);
}
