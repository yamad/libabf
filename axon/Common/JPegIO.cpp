//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  JpegIO.CPP
// PURPOSE: Wrapper class for the LIBTIFF library.
// AUTHOR:  BHI  May 1998
//

#include "wincpp.hpp"
#include "JpegIO.hpp"
#include "\AxonDev\Comp\Common\version.hpp"
#include "\AxonDev\Comp\Common\ArrayPtr.hpp"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"

extern "C"
{
#define HAVE_BOOLEAN

#if _MSC_VER >= 1200
#define INT32_DEFINED
#endif // _MSC_VER >= 1200

#include "\AxonDev\3rdParty\JpegLib\jpeglib.h"    // Jpeg API
#include "\AxonDev\3rdParty\JpegLib\jerror.h"     // Jpeg error codes.
}

extern HINSTANCE g_hInstance;

//===============================================================================================
// CLASS:   CJpegException
// PURPOSE: Thrown in prefence to an exit(0).
//
class CJpegException
{
private:
   int m_nMsgCode;

public:
   CJpegException(int nMsgCode) { m_nMsgCode = nMsgCode; }
};

//===============================================================================================
// FUNCTION: ErrorExit
// PURPOSE:  To handle fatal errors.
// NOTES:    The original JPEG code will just exit(0).
//
static void ErrorExit(j_common_ptr pJpegInfo)
{
   // Throw an exception that can then be caught at a higher level.
   CJpegException Exception(pJpegInfo->err->msg_code);
   throw Exception;
}

//===============================================================================================
// FUNCTION: OutputMessage
// PURPOSE:  To catch message output during file I/O.
// NOTES:    The original JPEG does a fprintf to stderr.
//
static void OutputMessage(j_common_ptr pJpegInfo)
{
   char szBuffer[JMSG_LENGTH_MAX];

   // Create the message
   (*pJpegInfo->err->format_message) (pJpegInfo, szBuffer);

   TRACE1("JpegLib message: %s\n", szBuffer);
}

//===============================================================================================
// CLASS:   CJpegInfo
// PURPOSE: Class wrapper for the Jpeg info structures etc.
//
class CJpegInfo
{
private:
   union
   {
      jpeg_common_struct     m_CommonInfo;
      jpeg_compress_struct   m_CompressInfo;
      jpeg_decompress_struct m_DecompressInfo;
   };
   jpeg_error_mgr m_ErrorMgr;

private:
   CJpegInfo();

public:
   CJpegInfo(BOOL bDecompressor)
   {
      memset(this, 0, sizeof(*this));
      m_CommonInfo.err          = jpeg_std_error(&m_ErrorMgr);
      m_ErrorMgr.error_exit     = ErrorExit;
      m_ErrorMgr.output_message = OutputMessage;

      if (bDecompressor)
         jpeg_create_decompress(&m_DecompressInfo);
      else
         jpeg_create_compress(&m_CompressInfo);
   }

   ~CJpegInfo()
   {
      try
      {
         jpeg_destroy(&m_CommonInfo);
      }
      catch (CJpegException) { }

      memset(this, 0, sizeof(*this));
   }

   operator jpeg_common_struct *()
   {
      return &m_CommonInfo;
   }

   operator jpeg_compress_struct *()
   {
      ASSERT(!m_CompressInfo.is_decompressor);   
      return &m_CompressInfo;
   }

   operator jpeg_decompress_struct *()
   {
      ASSERT(m_CompressInfo.is_decompressor);   
      return &m_DecompressInfo;
   }

   operator jpeg_error_mgr *()
   {
      return &m_ErrorMgr;
   }

   // Make a sample array that will go away when done with image
   JSAMPARRAY AllocSampleArray(int nRowStride, int nRows)
   {
      return m_CommonInfo.mem->alloc_sarray(&m_CommonInfo, JPOOL_IMAGE, nRowStride, nRows);
   }

   int GetLastError(char *pszError=NULL)
   {
      if (pszError)
         m_ErrorMgr.format_message(&m_CommonInfo, pszError);
      return m_ErrorMgr.msg_code;
   }
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CJpegFile::CJpegFile()
{
   MEMBERASSERT();

   // Assume that we are only dealing with 8-bit samples.
   ASSERT(BITS_IN_JSAMPLE==8);

   m_pFile            = NULL;
   m_pJpegInfo        = NULL;
   m_hwndNotify       = NULL;
   m_uNotifyMsg       = 0;
   m_szLastError[0]   = '\0';
   m_bOverride        = FALSE;
   m_dAxonFileVersion = FALSE;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CJpegFile::~CJpegFile()
{
   MEMBERASSERT();
   Close();
}

//===============================================================================================
// FUNCTION: IsOpen
// PURPOSE:  Returns TRUE if the file is open.
//
BOOL CJpegFile::IsOpen() const
{
   MEMBERASSERT();
   return (m_pFile!=NULL);
}

//===============================================================================================
// FUNCTION: SetNotifyWnd
// PURPOSE:  Register a window to receive notifications.
//
void CJpegFile::SetNotifyWnd(HWND hwndNotify, UINT uNotifyMsg)
{
   MEMBERASSERT();
   m_hwndNotify = hwndNotify;
   m_uNotifyMsg = uNotifyMsg;
}

//===============================================================================================
// FUNCTION: Notify
// PURPOSE:  Send a notification to the notify window.
//
void CJpegFile::Notify(UINT uPercent)
{
   MEMBERASSERT();
   if (m_hwndNotify)
      SendMessage(m_hwndNotify, m_uNotifyMsg, uPercent, 0);
}

//===============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  Returns a pointer to the last error string.
//
int CJpegFile::GetLastError() const
{
   MEMBERASSERT();
   return m_nLastError;
}

//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error code and returns FALSE as a convenience.
//
BOOL CJpegFile::SetLastError(int nError)
{
   MEMBERASSERT();
   m_nLastError = nError;
   return FALSE;
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Close an image file.
//
void CJpegFile::Close()
{
   MEMBERASSERT();
   if (m_pJpegInfo)
   {
      delete m_pJpegInfo;
      m_pJpegInfo = NULL;
      Notify(100);
   }
   if (m_pFile)
   {
      fclose(m_pFile);
      m_pFile = NULL;
   }
}

//===============================================================================================
// Compiler warns of unreachable code in the catch block in a release build.
#pragma warning(disable:4702)

//===============================================================================================
// FUNCTION: ReadOpen
// PURPOSE:  Open an image file.
//
BOOL CJpegFile::ReadOpen(LPCSTR pszFilename)
{
   MEMBERASSERT();
   Close();

   // In this example we want to open the input file before doing anything else,
   // so that the setjmp() error recovery below can assume the file is open.
   // VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   // requires it in order to read binary files.
   
   m_pFile = fopen(pszFilename, "rb");
   if (m_pFile == NULL)
      return SetLastError(JERR_CANT_OPEN_FILE);
   
   // Step 1: allocate and initialize JPEG decompression object
   
   // This struct contains the JPEG decompression parameters and pointers to
   // working space (which is allocated as needed by the JPEG library).
   BOOL bDecompressor = TRUE;
   m_pJpegInfo = new CJpegInfo(bDecompressor);
   if (!m_pJpegInfo)
   {
      Close();
      return SetLastError(JERR_OUT_OF_MEMORY);
   }
   
   try
   {
      j_decompress_ptr pInfo = *m_pJpegInfo;

      // specify data source (eg, a file)
      jpeg_stdio_src(pInfo, m_pFile);
   
      // read file parameters with jpeg_read_header()
      jpeg_read_header(pInfo, TRUE);
   
      // We can ignore the return value from jpeg_read_header since
      //   (a) suspension is not possible with the stdio data source, and
      //   (b) we passed TRUE to reject a tables-only JPEG file as an error.
      // See libjpeg.doc for more info.
   
      // how big is this thing ?
      m_Props.nWidth           = pInfo->image_width;     // output_width
      m_Props.nHeight          = pInfo->image_height;    // output_height
      m_Props.nSamplesPerPixel = pInfo->num_components;  // pInfo->out_color_components;
      m_Props.nBitsPerSample   = BITS_IN_JSAMPLE;        // ONLY 8 bit samples supported.

// FIX FIX FIX
//      m_dAxonFileVersion = GetFileVersion(m_pJpeg);
   }
   catch (CJpegException)
   {
      m_nLastError = m_pJpegInfo->GetLastError(m_szLastError);
      Close();
      return FALSE;
   }

   Notify(0);
   return TRUE;
}

#pragma warning(default:4702)

//===============================================================================================
// FUNCTION: Read
// PURPOSE:  Read an image from the file.
//
BOOL CJpegFile::Read(CJpegImage *pImage)
{
   MEMBERASSERT();

   // Assume that 12 bit TIFF files are just like 16 bit TIFF files (?).
   int nBitsPerPixel  = m_Props.nBitsPerSample * m_Props.nSamplesPerPixel;
   int nBytesPerPixel = nBitsPerPixel / 8;
   int nBytesPerRow   = nBytesPerPixel * m_Props.nWidth;

   // Must be at least 8 bits per pixel.
   if (nBytesPerPixel < 1)
      return FALSE;

   if (!pImage->SetProps(&m_Props))
      return FALSE;
   int nLine = 0;

   try
   {
      WPTRASSERT(m_pJpegInfo);
      j_decompress_ptr pInfo = *m_pJpegInfo;

      // Step 4: set parameters for decompression
   
      // In this example, we don't need to change any of the defaults set by
      // jpeg_read_header(), so we do nothing here.
   
      // Step 5: Start decompressor
      jpeg_start_decompress(pInfo);
   
      // We can ignore the return value since suspension is not possible
      // with the stdio data source.
   
      // We may need to do some setup of our own at this point before reading
      // the data.  After jpeg_start_decompress() we have the correct scaled
      // output image dimensions available, as well as the output colormap
      // if we asked for color quantization.
      // In this example, we need to make an output work buffer of the right size.
   
      ////////////////////////////////////////////////////////////
      // alloc and open our new buffer
      BYTE *pImageBuf = pImage->GetImagePtr(0, m_Props.nHeight);
      WPTRASSERT(pImageBuf);
   
      // JSAMPLEs per row in output buffer
      int nRowStride = m_Props.nWidth * m_Props.nSamplesPerPixel;
   
      // Make a one-row-high sample array that will go away when done with image
      JSAMPARRAY buffer = m_pJpegInfo->AllocSampleArray(nRowStride, 1);
      ASSERT(sizeof(JSAMPLE)==1);
   
      // Step 6: while (scan lines remain to be read)
      //           jpeg_read_scanlines(...);
   
      // Here we use the library's state variable pInfo->output_scanline as the
      // loop counter, so that we don't have to keep track ourselves.
      while (pInfo->output_scanline < pInfo->output_height)
      {
         // jpeg_read_scanlines expects an array of pointers to scanlines.
         // Here the array is only one element long, but you could ask for
         // more than one scanline at a time if that's more convenient.
         jpeg_read_scanlines(pInfo, buffer, 1);

         memcpy(pImageBuf, buffer[0], nBytesPerRow);
         pImageBuf += nBytesPerRow;
         nLine++;
      }
   
      // Step 7: Finish decompression
      jpeg_finish_decompress(pInfo);

      pImage->ReleaseImagePtr();
      pImage->SetHeight(nLine);
   }
   catch (CJpegException)
   {
      pImage->ReleaseImagePtr();
      m_nLastError = m_pJpegInfo->GetLastError(m_szLastError);
      Close();
      return FALSE;
   }

   return TRUE;
}

//===============================================================================================
// FUNCTION: WriteOpen
// PURPOSE:  Open an image file for writing.
//
BOOL CJpegFile::WriteOpen(LPCSTR pszFilename, LPCSTR pszComment)
{
   MEMBERASSERT();

   // Close any existing file.
   Close();

   // In this example we want to open the input file before doing anything else,
   // so that the setjmp() error recovery below can assume the file is open.
   // VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   // requires it in order to read binary files.
   
   m_pFile = fopen(pszFilename, "wb");
   if (m_pFile == NULL)
      return SetLastError(JERR_CANT_OPEN_FILE);
   
   // Step 1: allocate and initialize JPEG decompression object
   
   // This struct contains the JPEG decompression parameters and pointers to
   // working space (which is allocated as needed by the JPEG library).
   BOOL bDecompressor = FALSE;
   m_pJpegInfo = new CJpegInfo(bDecompressor);
   if (!m_pJpegInfo)
   {
      Close();
      return SetLastError(JERR_OUT_OF_MEMORY);
   }
   
   j_compress_ptr pInfo = *m_pJpegInfo;

   // specify data source (eg, a file)
   jpeg_stdio_dest(pInfo, m_pFile);

// FIX FIX FIX
   
      // Set date and time for this DLL/EXE.
//      BOOL bOK = SetAppNameAndVersion(m_pJpeg);
//      bOK     &= SetDateAndTime(m_pJpeg);
//      bOK     &= SetMakeAndFileVersion(m_pJpeg);
//
//      if (pszComment && pszComment[0])
//         bOK &= TIFFSetField(m_pJpeg, TIFFTAG_DOCUMENTNAME, pszComment);

   Notify(0);
   return TRUE;
}

//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Write an image to an image file.
// NOTES:    nQuality is in the range 1..100.
//
BOOL CJpegFile::Write(const CJpegImage *pImage, int nQuality)
{
   MEMBERASSERT();

   pImage->GetProps(&m_Props);
   int nBitsPerPixel  = m_Props.nBitsPerSample * m_Props.nSamplesPerPixel;
   int nBytesPerPixel = nBitsPerPixel / 8;
   int nBytesPerRow   = nBytesPerPixel * m_Props.nWidth;

   // First we supply a description of the input image.
   // Four fields of the cinfo struct must be filled in:
   j_compress_ptr pInfo = *m_pJpegInfo;
   pInfo->image_width  = m_Props.nWidth;    // image widthPix and height, in pixels
   pInfo->image_height = m_Props.nHeight;
   if (m_Props.nSamplesPerPixel == 3)
   {
      pInfo->input_components = 3;      // # of color components per pixel
      pInfo->in_color_space = JCS_RGB;  // colorspace of input image
   }
   else
   {
      pInfo->input_components = 1;      // # of color components per pixel
      pInfo->in_color_space = JCS_GRAYSCALE;  // colorspace of input image
   }

   try
   {
      // Now use the library's routine to set default compression parameters.
      // (You must set at least pInfo->in_color_space before calling this,
      // since the defaults depend on the source color space.)
      jpeg_set_defaults(pInfo);
   
      // Now you can set any non-default parameters you wish to.
      // Here we just illustrate the use of quality (quantization table) scaling:
      jpeg_set_quality(pInfo, nQuality, TRUE /* limit to baseline-JPEG values */);
   
      // Start compressor
   
      // TRUE ensures that we will write a complete interchange-JPEG file.
      // Pass TRUE unless you are very sure of what you're doing.
      jpeg_start_compress(pInfo, TRUE);
   
      // while (scan lines remain to be written)
      //    jpeg_write_scanlines(...);
   
      // Here we use the library's state variable pInfo->next_scanline as the
      // loop counter, so that we don't have to keep track ourselves.
      // To keep things simple, we pass one scanline per call; you can pass
      // more if you wish, though.

      // JSAMPLEs per row in image_buffer
      int nRowStride = m_Props.nWidth * m_Props.nSamplesPerPixel;
   
      BYTE *pbyImage = pImage->GetImagePtr(0, m_Props.nHeight);
      while (pInfo->next_scanline < pInfo->image_height)
      {
         // jpeg_write_scanlines expects an array of pointers to scanlines.
         // Here the array is only one element long, but you could pass
         // more than one scanline at a time if that's more convenient.
         jpeg_write_scanlines(pInfo, &pbyImage, 1);
         pbyImage += nBytesPerRow;
      }
   
      // Finish compression
      jpeg_finish_compress(pInfo);

      pImage->ReleaseImagePtr();
   }
   catch (CJpegException)
   {
      pImage->ReleaseImagePtr();
      m_nLastError = m_pJpegInfo->GetLastError(m_szLastError);
      Close();
      return FALSE;
   }

//   bOK &= TIFFSetField(m_pJpeg, TIFFTAG_RESOLUTIONUNIT,  RESUNIT_CENTIMETER);
//   bOK &= TIFFSetField(m_pJpeg, TIFFTAG_XPOSITION,       0.0F);
//   bOK &= TIFFSetField(m_pJpeg, TIFFTAG_YPOSITION,       0.0F);
//   if (Props.szDescription[0])
//      bOK &= TIFFSetField(m_pJpeg, TIFFTAG_IMAGEDESCRIPTION, Props.szDescription);
   return TRUE;
}

//===============================================================================================
// FUNCTION: Plural
// PURPOSE:  Helper function for returning a textual description of a TIFF file.
//
static LPCSTR Plural(int nValue)
{
   return (nValue==1) ? "" : "s";
}

//===============================================================================================
// FUNCTION: GetInfo
// PURPOSE:  Gets a textual description of the file contents.
//
BOOL CJpegFile::GetInfo(char *pszInfo, UINT uInfoSize)
{
   MEMBERASSERT();
   ASSERT(IsOpen());
   if (!IsOpen())
      return FALSE;

   char szComment[256] = "";
//   TIFFGetFieldString(m_pJpeg, TIFFTAG_DOCUMENTNAME, szComment, sizeof(szComment));

   int  nQuality = 75;
   int nBitsPerPixel = m_Props.nBitsPerSample * m_Props.nSamplesPerPixel;
   int l = AXU_sprintf(pszInfo, uInfoSize, "1 Image, %d bit%s per pixel\r\n"
                                           "Quality=%d\r\n"
                                           "Size=%dx%d pixels\r\n",
                                           nBitsPerPixel, Plural(nBitsPerPixel), 
                                           nQuality,
                                           m_Props.nWidth, m_Props.nHeight);
   if (szComment[0])
      AXU_sprintf(pszInfo+l, uInfoSize-l, "\r\n%s", szComment);

   return TRUE;
}

#if 0
//===============================================================================================
// FUNCTION: SetAppNameAndVersion
// PURPOSE:  Sets the name and version of the module writing the file.
//
static BOOL SetAppNameAndVersion(TIFF *pJpeg)
{
   CVersion Version(g_hInstance);
   if (!Version.IsValid())
   {
      ERRORMSG("Cannot find version resource.");
      return FALSE;
   }

   char szAppNameAndVersion[64] = "";
   UINT uBufSize = sizeof(szAppNameAndVersion);
   LPSTR psz = szAppNameAndVersion;

   UINT uLen = Version.GetString("InternalName", psz, uBufSize-1);
   ASSERT(uLen > 0);
   strcat(psz, " ");
   psz      += uLen+1;
   uBufSize -= uLen+1;

   uLen = Version.GetFileVersion(psz, uBufSize);
   ASSERT(uLen > 0);

   BOOL bOK = TIFFSetField(pJpeg, TIFFTAG_SOFTWARE, szAppNameAndVersion);
   bOK     &= TIFFSetField(pJpeg, TIFFTAG_MODEL,    szAppNameAndVersion);

   return bOK;
}

//===============================================================================================
// FUNCTION: SetDateAndTime
// PURPOSE:  Sets the current date and time.
//
static BOOL SetDateAndTime(TIFF *pJpeg)
{
   char szDateTime[64] = "";
   UINT uBufSize = sizeof(szDateTime);

   SYSTEMTIME SystemTime = { 0 };
   GetLocalTime(&SystemTime);
   
   int nLen = GetDateFormat(LOCALE_USER_DEFAULT, 0, &SystemTime, "yyyy/MM/dd", szDateTime, uBufSize-1);
   ASSERT(nLen > 0);

   strcat(szDateTime, " ");
   LPSTR psz = szDateTime + nLen;
   uBufSize -= nLen;

   nLen = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &SystemTime, "HH:mm:ss", psz, uBufSize);
   ASSERT(nLen > 0);
   return TIFFSetField(pJpeg, TIFFTAG_DATETIME, szDateTime);
}

//===============================================================================================
// FUNCTION: SetMakeAndFileVersion
// PURPOSE:  Sets the manufacturers name (Axon) and our written file format version.
//
static BOOL SetMakeAndFileVersion(TIFF *pJpeg)
{
   char szMakeAndVersion[64] = AXON_COMPANY_NAME;
   UINT uBufSize = sizeof(szMakeAndVersion);

   static char s_szNameTplt[] = "Axon Instruments (V%.2f)";
   AXU_sprintf(szMakeAndVersion, uBufSize, s_szNameTplt, AXON_TIFF_FILE_VERSION);

   return TIFFSetField(pJpeg, TIFFTAG_MAKE, szMakeAndVersion);
}

//===============================================================================================
// FUNCTION: GetFileVersion
// PURPOSE:  Parses out the company name and the file version number from MAKE field.
//
static double GetFileVersion( TIFF *pJpeg )
{
   char szText[64];
   if (!TIFFGetFieldString( pJpeg, TIFFTAG_MAKE, szText, sizeof(szText)))
      return 0.0;

   if (strnicmp(szText, AXON_COMPANY_NAME, strlen(AXON_COMPANY_NAME))!=0)
      return 0.0;

   LPSTR pszVersion = strstr(szText + strlen(AXON_COMPANY_NAME), "(V");
   if (!pszVersion)
      return 0.0;

   return atof(pszVersion+2);
}

#endif
