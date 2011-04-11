//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  JpegIO.HPP
// PURPOSE: Wrapper class for the JPEG library.
// AUTHOR:  BHI  May 1998
//

#ifndef INC_JPEGIO_HPP
#define INC_JPEGIO_HPP

#pragma once

#include <stdio.h>

#define AXON_JPEG_FILE_VERSION   1.0
#define AXON_COMPANY_NAME        "Axon Instruments"

struct JpegImageProps
{
   int    nWidth;              // Width of the image.
   int    nHeight;             // Height of the image.
   int    nBitsPerSample;      // Number of bits in each sample (must be multiple of 8)
   int    nSamplesPerPixel;    // Number of samples that make up each pixel.
   double dAxonFileVersion;    // Non-zero if the file was written by us.
   char   szDescription[128];  // Description of this image.

   // Constructor.
   JpegImageProps() 
   {
      memset(this, 0, sizeof(*this));
   }
};

class PURE_INTERFACE CJpegImage
{
public:
   // Constructor/destructor
   CJpegImage() {};
   virtual ~CJpegImage() {};

   // Get set properties.
   virtual void GetProps(JpegImageProps *pProps) const = 0;
   virtual BOOL SetProps(const JpegImageProps *pProps) = 0;
   virtual BOOL SetHeight(int nHeight) = 0;

   // Get/release pointer to image pixels.
   virtual BYTE *GetImagePtr(int nStartLine, int nLines) const = 0;
   virtual void ReleaseImagePtr() const = 0;
};

class CJpegInfo;

class CJpegFile
{
private:   // Attributes
   FILE           *m_pFile;
   CJpegInfo      *m_pJpegInfo;
   JpegImageProps  m_Props;   // Each Jpeg file only contains one image...

   HWND   m_hwndNotify;       // The window to notify of progress.
   UINT   m_uNotifyMsg;       // The message to send to notify of progress.
   double m_dAxonFileVersion; // Set on reading if the file was written by us.
   BOOL   m_bOverride;        // Override default scaling if TRUE.
   int    m_nLastError;       // Numeric last error value.
   char   m_szLastError[256]; // Text of last error condition flagged by the LIBJPEG.

private:   // Unimplemented copy functions.
   CJpegFile(const CJpegFile &);
   const CJpegFile &operator=(const CJpegFile &);

private:   // Private functions
   void Notify(UINT uPercent);
   double GetMicronsPerPixel();

public:    // Public interface
   CJpegFile();
   ~CJpegFile();

   BOOL IsOpen() const;
   void SetNotifyWnd(HWND hwndNotify, UINT uNotifyMsg);

   // File open / close
   BOOL WriteOpen(LPCSTR pszFilename, LPCSTR pszComment=NULL);
   BOOL ReadOpen(LPCSTR pszFilename);
   void Close();

   // Setup functions.
   BOOL GetInfo(char *pszInfo, UINT uInfoSize);

   // Read / write functions.
   BOOL Write(const CJpegImage *pImage, int nQuality);
   BOOL Read(CJpegImage *pImage);

   // Error functions.
   BOOL SetLastError(int nError);
   int GetLastError() const;
};

const int JERR_CANT_OPEN_FILE = -1;

#endif      // INC_JPEGIO_HPP

