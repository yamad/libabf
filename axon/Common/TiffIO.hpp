//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  TiffIO.HPP
// PURPOSE: Wrapper class for the LIBTIFF library.
// AUTHOR:  BHI  May 1998
//

#ifndef INC_TIFFIO_HPP
#define INC_TIFFIO_HPP

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <stdarg.h>
#include "\AxonDev\3rdParty\LibTiff\tiffio.h"

#define AXON_TIFF_FILE_VERSION   1.0
#define AXON_COMPANY_NAME        "Axon Instruments"

struct TiffImageProps
{
   int    nWidth;              // Width of the image.
   int    nHeight;             // Height of the image.
   int    xOrigin;             // Offset of top-left from page origin (in pixels)
   int    yOrigin;             //    "        "   "    "    "    "
   int    nFullWidth;          // Full width of the surrounding page.
   int    nFullHeight;         // Full height of the surrounding page.
   int    nBitsPerSample;      // Number of bits in each sample (must be multiple of 8)
   int    nSamplesPerPixel;    // Number of samples that make up each pixel.
   int    nMaxSampleValue;     // Maximum value of a sample
   int    nMinSampleValue;     // Minimum value of a sample.
   int    nRowsPerStrip;       // Number of rows to group together for compression.
   int    nStrips;             // Number of strips in the file (read only).
   int    nStripSize;          // The maximum size of each strip (read only).
   BOOL   bCompress;           // TRUE if LZW compression is to be used.
   double dAxonFileVersion;    // Non-zero if the file was written by us.
   double dMicronsPerPixel;    // Scaling of image (um/pixel).
   char   szDescription[128];  // Description of this image.

   // Constructor.
   TiffImageProps() 
   {
      memset(this, 0, sizeof(*this));
      dMicronsPerPixel = 10.0;
   }
};

class PURE_INTERFACE CTiffImage
{
public:
   // Constructor/destructor
   CTiffImage() {};
   virtual ~CTiffImage() {};

   // Get set properties.
   virtual void GetProps(TiffImageProps *pProps) const = 0;
   virtual BOOL SetProps(const TiffImageProps *pProps) = 0;
   virtual BOOL SetHeight(int nHeight) = 0;

   // Get/release pointer to image pixels.
   virtual BYTE *GetImagePtr(int nStartLine, int nLines) const = 0;
   virtual void ReleaseImagePtr() const = 0;

   // Callback to return palette (can stub out with zero return if not interested in paletteized images).
   virtual int GetPalette(PALETTEENTRY *pColors=NULL, UINT uMaxEntries=0) const = 0;
};

class CTiffFile
{
private:   // Attributes
   TIFF  *m_pTiff;            // The LIBTIFF tiff file handle.
   HWND   m_hwndNotify;       // The window to notify of progress.
   UINT   m_uNotifyMsg;       // The message to send to notify of progress.
   int    m_nLines;           // The number of lines that have bben processed.
   int    m_nTotalLines;      // Total number of lines used to calculate % complete.
   char   m_szLastError[256]; // Text of last error condition flagged by the LIBTIFF.
   double m_dAxonFileVersion; // Set on reading if the file was written by us.
   double m_dMicronsPerPixel;
   char   m_szModel[128];
   BOOL   m_bOverride;

private:   // Unimplemented copy functions.
   CTiffFile(const CTiffFile &);
   const CTiffFile &operator=(const CTiffFile &);

private:   // Private functions
   static void WarningHandler(LPCSTR szModule, LPCSTR szFormat, va_list ap);
   static void ErrorHandler(LPCSTR szModule, LPCSTR szFormat, va_list ap);

   void OnError(BOOL bWarning, LPCSTR szModule, LPCSTR szFormat, va_list ap);
   void Notify(UINT uPercent);
   double GetMicronsPerPixel(double *pdResolution=NULL);
   BOOL SetLastError(LPCSTR pszErrorMsg);

public:    // Public interface
   CTiffFile();
   ~CTiffFile();

   // File open / close
   BOOL WriteOpen(LPCSTR pszFilename, LPCSTR pszComment=NULL);
   BOOL ReadOpen(LPCSTR pszFilename, UINT *puImages=NULL);
   void Close();
   BOOL IsOpen() const;

   // Setup functions.
   void SetTotalLines(int nTotalLines);
   BOOL GetInfo(char *pszInfo, UINT uInfoSize);
   void SetNotifyWnd(HWND hwndNotify, UINT uNotifyMsg);

   // Low level read functions.
   BOOL SetActiveImage(int nImage, TiffImageProps *pProps);
   BOOL ReadStrip(int nStrip, BYTE *pbyStrip, int nStripSize);
   double GetAxonFileVersion() const;

   // Read / write functions.
   BOOL Write(const CTiffImage *pImage, BOOL bCompress);
   BOOL Read(int nImage, CTiffImage *pImage);

   void SetDefaultScaling(double dMicronsPerPixel, BOOL bOverride);
   void SetModel(LPCSTR pszModel);

   // Error function.
   LPCSTR GetLastError() const;
};

const char TIFF_CANNOT_READ_FILE[]   = "Cannot read TIFF file.";
const char TIFF_CANNOT_READ_RGB[]    = "Cannot read RGB TIFF file.";
const char TIFF_UNSUPPORTED_FORMAT[] = "Unsupported TIFF format.";
const char TIFF_OUTOFMEMORY[]        = "Out of memory.";

#endif      // INC_TIFFIO_HPP

