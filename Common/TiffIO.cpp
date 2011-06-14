//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  TiffIO.CPP
// PURPOSE: Wrapper class for the LIBTIFF library.
// AUTHOR:  BHI  May 1998
//

#include "wincpp.hpp"
#include <malloc.h>

#include "TiffIO.hpp"
#include "\AxonDev\comp\axoutils32\axoutils32.h"
#include "\AxonDev\Comp\Common\version.hpp"
#include "\AxonDev\Comp\Common\ArrayPtr.hpp"

extern HINSTANCE g_hInstance;

// Default number of rows per strip to use if not set explicitly.
const int c_nROWS_PER_STRIP = 32;

static CTiffFile *s_pThis = NULL;

inline CTiffFile *GetThis()
{
   WPTRASSERT(s_pThis);
   return s_pThis;
}

#define METHOD_PROLOGUE(This)  { WPTRASSERT(This); s_pThis = This; }

// Reuse the hostcomputer string to save full-width & full-height.
#define TIFFTAG_FULLSIZE   TIFFTAG_HOSTCOMPUTER

const char s_szFullSize[] = "FullSize=";

// #define READ_RGB_IMAGE

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CTiffFile::CTiffFile()
{
   METHOD_PROLOGUE(this);

   m_pTiff            = NULL;
   m_hwndNotify       = NULL;
   m_uNotifyMsg       = 0;
   m_nLines           = 0;
   m_nTotalLines      = 0;
   m_szLastError[0]   = '\0';
   m_dMicronsPerPixel = 10.0;
   m_bOverride        = FALSE;
   m_dAxonFileVersion = FALSE;
   m_szModel[0]       = '\0';

   TIFFSetErrorHandler(CTiffFile::ErrorHandler);
   TIFFSetWarningHandler(CTiffFile::WarningHandler);
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CTiffFile::~CTiffFile()
{
   METHOD_PROLOGUE(this);
   Close();
}

//===============================================================================================
// FUNCTION: IsOpen
// PURPOSE:  Returns TRUE if the file is open.
//
BOOL CTiffFile::IsOpen() const
{
   MEMBERASSERT();
   return (m_pTiff!=NULL);
}

//===============================================================================================
// FUNCTION: OnError
// PURPOSE:  Callback function for LibTiff warnings & errors.
//
void CTiffFile::OnError(BOOL bWarning, LPCSTR pszModule, LPCSTR szFormat, va_list ap)
{
   LPSTR psz  = m_szLastError;
   UINT  uLen = sizeof(m_szLastError);

   AXU_sprintf(psz, uLen, "LibTiff %s%s: ", (bWarning ? "Warning" : "Error"), pszModule);
   int l = strlen(psz);
   psz  += l;
   uLen -= l;

   // Build the error message and pass it on to the active object.
   AXU_vsprintf(psz, uLen, szFormat, ap);
   TRACE1("%s\n", m_szLastError);
}

//===============================================================================================
// FUNCTION: WarningHandler
// PURPOSE:  Callback function for LibTiff warnings.
//
void CTiffFile::WarningHandler(LPCSTR pszModule, LPCSTR pszFormat, va_list ap)
{
   GetThis()->OnError(TRUE, pszModule, pszFormat, ap);
}

//===============================================================================================
// FUNCTION: ErrorHandler
// PURPOSE:  Callback function for LibTiff errors.
//
void CTiffFile::ErrorHandler(LPCSTR pszModule, LPCSTR pszFormat, va_list ap)
{
   GetThis()->OnError(FALSE, pszModule, pszFormat, ap);
}

//===============================================================================================
// FUNCTION: TIFFGetFieldString
// PURPOSE:  Reads a string tag from a TIFF file.
//
static BOOL TIFFGetFieldString( TIFF *pTiff, ttag_t nTag, LPSTR pszText, UINT uMaxLen)
{
   ARRAYASSERT(pszText, uMaxLen);
   if (pszText && uMaxLen)
      pszText[0] = '\0';

   LPSTR psz = NULL;
   if (!TIFFGetField( pTiff, nTag, &psz))
      return FALSE;

   AXU_strncpyz(pszText, psz, uMaxLen);
   return TRUE;
}

//===============================================================================================
// FUNCTION: TIFFGetFieldDouble
// PURPOSE:  Reads a float from a TIFF file and converts it to a double.
//
static BOOL TIFFGetFieldDouble( TIFF *pTiff, ttag_t nTag, double *pdValue)
{
   WPTRASSERT(pdValue);

   float fValue = 0.0F;
   if (!TIFFGetField( pTiff, nTag, &fValue))
      return FALSE;

   *pdValue = fValue;
   return TRUE;
}

//===============================================================================================
// FUNCTION: SetAppNameAndVersion
// PURPOSE:  Sets the name and version of the module writing the file.
//
static BOOL SetAppNameAndVersion(TIFF *pTiff)
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

   return TIFFSetField(pTiff, TIFFTAG_SOFTWARE, szAppNameAndVersion);
}

//===============================================================================================
// FUNCTION: SetDateAndTime
// PURPOSE:  Sets the current date and time.
//
static BOOL SetDateAndTime(TIFF *pTiff)
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
   return TIFFSetField(pTiff, TIFFTAG_DATETIME, szDateTime);
}

//===============================================================================================
// FUNCTION: SetMakeAndFileVersion
// PURPOSE:  Sets the manufacturers name (Axon) and our written file format version.
//
static BOOL SetMakeAndFileVersion(TIFF *pTiff)
{
   char szMakeAndVersion[64] = AXON_COMPANY_NAME;
   UINT uBufSize = sizeof(szMakeAndVersion);

   static char s_szNameTplt[] = "Axon Instruments (V%.2f)";
   AXU_sprintf(szMakeAndVersion, uBufSize, s_szNameTplt, AXON_TIFF_FILE_VERSION);

   return TIFFSetField(pTiff, TIFFTAG_MAKE, szMakeAndVersion);
}

//===============================================================================================
// FUNCTION: GetFileVersion
// PURPOSE:  Parses out the company name and the file version number from MAKE field.
//
static double GetFileVersion( TIFF *pTiff )
{
   char szText[64];
   if (!TIFFGetFieldString( pTiff, TIFFTAG_MAKE, szText, sizeof(szText)))
      return 0.0;

   if (strnicmp(szText, AXON_COMPANY_NAME, strlen(AXON_COMPANY_NAME))!=0)
      return 0.0;

   LPSTR pszVersion = strstr(szText + strlen(AXON_COMPANY_NAME), "(V");
   if (!pszVersion)
      return 0.0;

   return atof(pszVersion+2);
}

//===============================================================================================
// FUNCTION: GetFullImageSize
// PURPOSE:  Parses the full image size out of the host computer name string.
//
static BOOL GetFullImageSize(TIFF *pTiff, int &nFullWidth, int &nFullHeight)
{
   nFullWidth = nFullHeight = 0;

   char szTxt[64];
   TIFFGetFieldString(pTiff, TIFFTAG_FULLSIZE, szTxt, sizeof(szTxt));

   // Look for prefix string.
   char *psz = strstr(szTxt, s_szFullSize);
   if (!psz)
      return FALSE;
   psz += strlen(s_szFullSize);

   // Parse out first number.
   psz =  strtok(psz, ", ");
   if (!psz)
      return FALSE;
   nFullWidth = atoi(psz);

   // Parse out second number.
   psz = strtok(NULL, ", ");
   if (psz && nFullWidth)
      nFullHeight = atoi(psz);

   // Both numbers must be positive to succeed.
   if (nFullWidth==0 || nFullHeight==0)
   {
      nFullWidth = nFullHeight = 0;
      return FALSE;
   }

   return TRUE;
}

//===============================================================================================
// FUNCTION: SetFullImageSize
// PURPOSE:  Sets the full image size in the host computer name string.
//
static BOOL SetFullImageSize(TIFF *pTiff, int nFullWidth, int nFullHeight)
{
   // Both numbers must be positive to succeed.
   if ((nFullWidth <= 0) || (nFullHeight <= 0))
      return FALSE;

   char szTxt[32];
   AXU_sprintf(szTxt, sizeof(szTxt), "%s%d,%d", s_szFullSize, nFullWidth, nFullHeight);
   return TIFFSetField(pTiff, TIFFTAG_FULLSIZE, szTxt);
}

//===============================================================================================
// FUNCTION: SetModel
// PURPOSE:  Sets the string to be stored in the model tag of the file.
//
void CTiffFile::SetModel(LPCSTR pszModel)
{
   MEMBERASSERT();
   AXU_strncpyz(m_szModel, pszModel, sizeof(m_szModel));
}

//===============================================================================================
// FUNCTION: WriteOpen
// PURPOSE:  Open an image file for writing.
//
BOOL CTiffFile::WriteOpen(LPCSTR pszFilename, LPCSTR pszComment)
{
   METHOD_PROLOGUE(this);

   // Close any existing file.
   Close();

   // Open the tiff file.
   m_pTiff = TIFFOpen(pszFilename, "w");
   if (!m_pTiff)
      return FALSE;

   // Set date and time for this DLL/EXE.
   BOOL bOK = SetAppNameAndVersion(m_pTiff);
   bOK     &= SetDateAndTime(m_pTiff);
   bOK     &= SetMakeAndFileVersion(m_pTiff);
   if (m_szModel[0])
      bOK &= TIFFSetField(m_pTiff, TIFFTAG_MODEL, m_szModel);

   if (pszComment && pszComment[0])
      bOK &= TIFFSetField(m_pTiff, TIFFTAG_DOCUMENTNAME, pszComment);

   return bOK;
}

//===============================================================================================
// FUNCTION: SetNotifyWnd
// PURPOSE:  Register a window to receive notifications.
//
void CTiffFile::SetNotifyWnd(HWND hwndNotify, UINT uNotifyMsg)
{
   METHOD_PROLOGUE(this);
   m_hwndNotify = hwndNotify;
   m_uNotifyMsg = uNotifyMsg;
}

//===============================================================================================
// FUNCTION: SetTotalLines
// PURPOSE:  If progress notifications are required, this function must be called to
//           set the target number of lines.
//
void CTiffFile::SetTotalLines(int nTotalLines)
{
   METHOD_PROLOGUE(this);
   m_nLines      = 0;
   m_nTotalLines = nTotalLines;
   Notify(0);
}

//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Write an image to an image file.
//
BOOL CTiffFile::Write(const CTiffImage *pImage, BOOL bCompress)
{
   METHOD_PROLOGUE(this);

   TiffImageProps Props;
   pImage->GetProps(&Props);

   int nPaletteEntries = pImage->GetPalette();

   // If the rows per strip have not been set, default to a reasonable number.
   if (Props.nRowsPerStrip==0)
      Props.nRowsPerStrip = c_nROWS_PER_STRIP;

   int nCompress    = (bCompress ? COMPRESSION_LZW : COMPRESSION_NONE);
   int nMaxSample   = (Props.nBitsPerSample==16 ? 65535 : 255);
   int nMinSample   = 0;

   int nPhotometric = PHOTOMETRIC_MINISBLACK;
   if (Props.nSamplesPerPixel > 1)
      nPhotometric = PHOTOMETRIC_RGB;
   else if (nPaletteEntries > 0)
      nPhotometric = PHOTOMETRIC_PALETTE;

   if (Props.nMaxSampleValue > 0)
      nMaxSample = min(nMaxSample, Props.nMaxSampleValue);
   if (Props.nMinSampleValue > 0)
      nMinSample = max(nMinSample, Props.nMinSampleValue);

   // Calculate the number of pixels per resolution unit.
   double dResolution = 10000.0 / Props.dMicronsPerPixel;

   // Convert the pixel origin coords to resolution units.
   double dOriginX    = Props.xOrigin / dResolution;
   double dOriginY    = Props.yOrigin / dResolution;

   BOOL bOK = FALSE;
   bOK  = TIFFSetField(m_pTiff, TIFFTAG_IMAGEWIDTH,      Props.nWidth);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_IMAGELENGTH,     Props.nHeight);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_SUBFILETYPE,     0);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_BITSPERSAMPLE,   Props.nBitsPerSample);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_SAMPLESPERPIXEL, Props.nSamplesPerPixel);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_ROWSPERSTRIP,    Props.nRowsPerStrip);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_COMPRESSION,     nCompress);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_PHOTOMETRIC,     nPhotometric);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_ORIENTATION,     ORIENTATION_TOPLEFT);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_PLANARCONFIG,    PLANARCONFIG_CONTIG);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_RESOLUTIONUNIT,  RESUNIT_CENTIMETER);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_XRESOLUTION,     dResolution);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_YRESOLUTION,     dResolution);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_XPOSITION,       dOriginX);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_YPOSITION,       dOriginY);

   SetFullImageSize(m_pTiff, Props.nFullWidth, Props.nFullHeight);

   bOK &= TIFFSetField(m_pTiff, TIFFTAG_MINSAMPLEVALUE,  nMinSample);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_MAXSAMPLEVALUE,  nMaxSample);
   bOK &= TIFFSetField(m_pTiff, TIFFTAG_SAMPLEFORMAT,    SAMPLEFORMAT_UINT);
   if (Props.szDescription[0])
      bOK &= TIFFSetField(m_pTiff, TIFFTAG_IMAGEDESCRIPTION, Props.szDescription);

   int nBitsPerPixel = Props.nBitsPerSample * Props.nSamplesPerPixel;
   UINT uBytesPerScanLine = Props.nWidth * nBitsPerPixel / 8;

   int nStrip = 0;
   int nStrips = Props.nHeight / Props.nRowsPerStrip;
   if (Props.nHeight % Props.nRowsPerStrip)
      nStrips++;

   // RGB strips need the RGB bytes reversed.
   BOOL bRGBImage = (Props.nSamplesPerPixel > 1);
   CArrayPtr<BYTE> StripBuffer;
   if (bRGBImage && !StripBuffer.Alloc(uBytesPerScanLine * Props.nRowsPerStrip))
      return FALSE;

   int  nLine = 0;
   for (int i=0; i<nStrips; i++)
   {
      UINT uStripHeight = min(Props.nRowsPerStrip, Props.nHeight - nLine);
      UINT uStripBytes  = uStripHeight * uBytesPerScanLine;

      BYTE *pbyImage = pImage->GetImagePtr(nLine, uStripHeight);
      if (!pbyImage)
      {
         pImage->ReleaseImagePtr();
         return FALSE;
      }

      if (bRGBImage)
      {
         ASSERT(Props.nSamplesPerPixel==3);
         ASSERT(Props.nBitsPerSample==8);

         BYTE *pDest = StripBuffer;
         BYTE *pSrce = pbyImage;
         for (int j=0; j<Props.nWidth * int(uStripHeight); j++)
         {
            pDest[0] = pSrce[2];
            pDest[1] = pSrce[1];
            pDest[2] = pSrce[0];
            pDest += 3;
            pSrce += 3;
         }
         pbyImage = StripBuffer;
      }

      if (TIFFWriteEncodedStrip(m_pTiff, nStrip++, pbyImage, uStripBytes) == -1)
      {
         pImage->ReleaseImagePtr();
         return FALSE;
      }

      nLine    += uStripHeight;
      m_nLines += uStripHeight;
      if (m_nTotalLines)
         Notify(UINT(m_nLines*100 / m_nTotalLines));
   }
   pImage->ReleaseImagePtr();

   if (nPaletteEntries > 0)
   {
      CArrayPtr<PALETTEENTRY> Palette(nPaletteEntries);

      int nTiffPaletteEntries = (Props.nBitsPerSample==16) ? 65535 : 256;
      CArrayPtr<WORD>         TiffPalette(nTiffPaletteEntries*3);
      if (!Palette || !TiffPalette)
         return FALSE;
      TiffPalette.Zero(nTiffPaletteEntries*3);

      pImage->GetPalette(Palette, nPaletteEntries);
      for (int i=0; i<nPaletteEntries; i++)
      {
         TiffPalette[i+0*nPaletteEntries] = MAKEWORD(0, Palette[i].peRed);
         TiffPalette[i+1*nPaletteEntries] = MAKEWORD(0, Palette[i].peGreen);
         TiffPalette[i+2*nPaletteEntries] = MAKEWORD(0, Palette[i].peBlue);
      }
      TIFFSetField(m_pTiff, TIFFTAG_COLORMAP, 
                   TiffPalette, TiffPalette+nPaletteEntries, TiffPalette+2*nPaletteEntries);
   }
   return TIFFWriteDirectory(m_pTiff);
}

//===============================================================================================
// FUNCTION: GetCompression
// PURPOSE:  Helper function for returning a textual description of a TIFF file.
//
static LPCSTR GetCompression(int nCompression)
{
   switch (nCompression)
   {
      case COMPRESSION_NONE:
         return "No compression";
      case COMPRESSION_CCITTRLE:
         return "CCITT modified Huffman RLE";
      case COMPRESSION_CCITTFAX3:
         return "CCITT Group 3 fax encoding";
      case COMPRESSION_CCITTFAX4:
         return "CCITT Group 4 fax encoding";
      case COMPRESSION_LZW:
         return "Lempel-Ziv & Welch";
      case COMPRESSION_OJPEG:
         return "TIFF 6.0 JPEG";
      case COMPRESSION_JPEG:
         return "JPEG DCT compression";
      case COMPRESSION_NEXT:
         return "NeXT 2-bit RLE";
      case COMPRESSION_CCITTRLEW:
         return "#1 w/ word alignment";
      case COMPRESSION_PACKBITS:
         return "Macintosh RLE";
      case COMPRESSION_THUNDERSCAN:
         return "ThunderScan RLE";
      case COMPRESSION_IT8CTPAD:
         return "IT8 CT w/padding";
      case COMPRESSION_IT8LW:
         return "IT8 Linework RLE";
      case COMPRESSION_IT8MP:
         return "IT8 Monochrome picture";
      case COMPRESSION_IT8BL:
         return "IT8 Binary line art";
      case COMPRESSION_PIXARFILM:
         return "Pixar companded 10bit LZW";
      case COMPRESSION_PIXARLOG:
         return "Pixar companded 11bit ZIP";
      case COMPRESSION_DEFLATE:
         return "Deflate compression";
      case COMPRESSION_DCS:
         return "Kodak DCS encoding";
      case COMPRESSION_JBIG:
         return "ISO JBIG";
      case COMPRESSION_SGILOG:
         return "SGI Log Luminance RLE";
      case COMPRESSION_SGILOG24:
         return "SGI Log 24-bit packed";
      default:
         return "Unknown compression scheme";
   }
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
// FUNCTION: ReadOpen
// PURPOSE:  Open an image file.
//
BOOL CTiffFile::ReadOpen(LPCSTR pszFilename, UINT *puImages)
{
   METHOD_PROLOGUE(this);
   Close();

   m_pTiff = TIFFOpen(pszFilename, "r");  // "u" means do not use MMF.
   if (!m_pTiff)
      return FALSE;

   m_nLines      = 0;
   m_nTotalLines = 0;
   int nSamplesPerPixel=0, nBitsPerSample=0, nCompression=0, nRowsPerStrip=0; 

   BOOL bOK = FALSE;
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_BITSPERSAMPLE,   &nBitsPerSample);
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_SAMPLESPERPIXEL, &nSamplesPerPixel);
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_COMPRESSION,     &nCompression);
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_ROWSPERSTRIP,    &nRowsPerStrip);

   m_dAxonFileVersion = GetFileVersion(m_pTiff);

   for (tdir_t nDir = 0; TIFFSetDirectory(m_pTiff, nDir); nDir++)
   {
      int nWidth = 0, nHeight = 0;
      if (!TIFFGetField(m_pTiff, TIFFTAG_IMAGEWIDTH,  &nWidth) ||
          !TIFFGetField(m_pTiff, TIFFTAG_IMAGELENGTH, &nHeight))
         break;
      m_nTotalLines += nHeight;
      bOK = TRUE;
   }
   TIFFSetDirectory(m_pTiff, 0);

   if (bOK)
      Notify(0);

   if (puImages)
      *puImages = nDir;

   return bOK;
}

//===============================================================================================
// FUNCTION: GetInfo
// PURPOSE:  Gets a textual description of the file contents.
//
BOOL CTiffFile::GetInfo(char *pszInfo, UINT uInfoSize)
{
   METHOD_PROLOGUE(this);
   ASSERT(m_pTiff!=NULL);
   if (!m_pTiff)
      return FALSE;

   tdir_t nCurrentDirectory = TIFFCurrentDirectory(m_pTiff);

   TiffImageProps Props;
   SetActiveImage(0, &Props);

   char szComment[256] = "";
   TIFFGetFieldString(m_pTiff, TIFFTAG_DOCUMENTNAME, szComment, sizeof(szComment));

   int nCompression = COMPRESSION_NONE;
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_COMPRESSION, &nCompression);

   char szImageSizes[256];
   UINT uSizeFree = 0;
   char szScaling[256];
   UINT uScalingFree = 0;

   for (tdir_t nDir = 0; TIFFSetDirectory(m_pTiff, nDir); nDir++)
   {
      int nWidth = 0, nHeight = 0;
      if (!TIFFGetField(m_pTiff, TIFFTAG_IMAGEWIDTH,  &nWidth) ||
          !TIFFGetField(m_pTiff, TIFFTAG_IMAGELENGTH, &nHeight))
         break;

      double dMicronsPerPixel = GetMicronsPerPixel();

      if (nDir > 0)
         uSizeFree += AXU_sprintf(szImageSizes+uSizeFree, sizeof(szImageSizes)-uSizeFree, ", ");
      uSizeFree += AXU_sprintf(szImageSizes+uSizeFree, sizeof(szImageSizes)-uSizeFree, "%dx%d", nWidth, nHeight);

      if (nDir > 0)
         uScalingFree += AXU_sprintf(szScaling+uScalingFree, sizeof(szScaling)-uScalingFree, ", ");
      uScalingFree += AXU_sprintf(szScaling+uScalingFree, sizeof(szScaling)-uScalingFree, "%d", int(dMicronsPerPixel));
   }
   VERIFY( TIFFSetDirectory(m_pTiff, nCurrentDirectory) );

   int nBitsPerPixel = Props.nBitsPerSample * Props.nSamplesPerPixel;

   int l = AXU_sprintf(pszInfo, uInfoSize, "%d Image%s, %d bit%s per pixel\r\n"
                                           "Compression=%s\r\n"
                                           "Size=%s pixels\r\n"
                                           "Scaling=%s µm per pixel\r\n",
                                           int(nDir), Plural(nDir),
                                           nBitsPerPixel, Plural(nBitsPerPixel), 
                                           GetCompression(nCompression),
                                           szImageSizes,
                                           szScaling);
   if (szComment[0])
      AXU_sprintf(pszInfo+l, uInfoSize-l, "\r\n%s", szComment);

   return TRUE;
}

//===============================================================================================
// FUNCTION: UnpackStrip
// PURPOSE:  Unpacks a packed strip into samples that are even byte multiples.
//
static void UnpackStrip(BYTE *pImage, UINT uStripBytes, UINT uSamples, int nBitsPerPixel)
{
   if (nBitsPerPixel==12)
   {
      WORD *pwDest = (WORD *)pImage;
      for (int i=int(uSamples-1); i>=0; i--)
      {
         UINT uBits  = i * 12;
         UINT uBytes = uBits / 8;
         WORD wValue = *(WORD *)(pImage+uBytes);
         if ((i & 1)==0)
            wValue <<= 4;
         pwDest[i] = WORD(wValue & 0xFFF0);
      }
   }
   else if (nBitsPerPixel==1)
   {
      for (int i=int(uSamples-1); i>=0; i--)
      {
         UINT uBits  = i;
         UINT uBytes = uBits / 8;
         pImage[i]   = BYTE((pImage[uBytes] & (1<<(uBits%8))) ? 255 : 0);
      }
   }
}

//===============================================================================================
// FUNCTION: SetDefaultScaling
// PURPOSE:  Sets the default scaling of microns to pixels.
//
void CTiffFile::SetDefaultScaling(double dMicronsPerPixel, BOOL bOverride)
{
   MEMBERASSERT();
   m_dMicronsPerPixel = dMicronsPerPixel;
   m_bOverride        = bOverride;
}

//===============================================================================================
// FUNCTION: GetMicronsPerPixel
// PURPOSE:  Convert resolution unit and pixels per resolution unit to microns-per-pixel.
//
double CTiffFile::GetMicronsPerPixel(double *pdResolution)
{
   int   nResolutionUnit = RESUNIT_NONE;
   double dXResolution = 0.0, dYResolution = 0.0;
   BOOL bOK = TIFFGetField(m_pTiff, TIFFTAG_RESOLUTIONUNIT,    &nResolutionUnit);
   bOK     &= TIFFGetFieldDouble(m_pTiff, TIFFTAG_XRESOLUTION, &dXResolution);
   bOK     &= TIFFGetFieldDouble(m_pTiff, TIFFTAG_YRESOLUTION, &dYResolution);

   if (!bOK)
      return m_dMicronsPerPixel;

   double dResolution = min(dXResolution, dYResolution);
   if ((dXResolution<=0.0) || (dYResolution<=0.0))
      dResolution = max(dXResolution, dYResolution);
   if (pdResolution)
      *pdResolution = dResolution;

   if ((nResolutionUnit==RESUNIT_NONE) || (dResolution <= 0.0))
      return m_dMicronsPerPixel;

   if (m_bOverride && (m_dAxonFileVersion==0.0))
      return m_dMicronsPerPixel;

   switch (nResolutionUnit)
   {
      case RESUNIT_INCH:      // english
         return 10000.0 / (dResolution/2.54);
         break;
      default:
         ERRORMSG1("Unexpected resolution unit (%d)", nResolutionUnit);
         return 1.0;
      case RESUNIT_CENTIMETER:// metric
         return 10000.0 / dResolution;
   }
}

//===============================================================================================
// FUNCTION: SetActiveImage
// PURPOSE:  Sets a given image as active and gets its properties.
//
BOOL CTiffFile::SetActiveImage(int nImage, TiffImageProps *pProps)
{
   METHOD_PROLOGUE(this);
   if (!TIFFSetDirectory(m_pTiff, tdir_t(nImage)))
      return FALSE;

   // Clear all to start with.
   memset(pProps, 0, sizeof(*pProps));

   VERIFY( TIFFGetField(m_pTiff, TIFFTAG_IMAGEWIDTH,  &pProps->nWidth) );
   VERIFY( TIFFGetField(m_pTiff, TIFFTAG_IMAGELENGTH, &pProps->nHeight) );

   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_BITSPERSAMPLE,   &pProps->nBitsPerSample);
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_SAMPLESPERPIXEL, &pProps->nSamplesPerPixel);
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_ROWSPERSTRIP,    &pProps->nRowsPerStrip);

   pProps->nMaxSampleValue = (pProps->nBitsPerSample==16 ? 65535 : 255);
   pProps->nMinSampleValue = 0;
   TIFFGetField(m_pTiff, TIFFTAG_MINSAMPLEVALUE, &pProps->nMinSampleValue);
   TIFFGetField(m_pTiff, TIFFTAG_MAXSAMPLEVALUE, &pProps->nMaxSampleValue);

   double dResolution = 0.0;
   pProps->dMicronsPerPixel = GetMicronsPerPixel(&dResolution);
   pProps->dAxonFileVersion = m_dAxonFileVersion;

   if (dResolution > 0.0)
   {
      double dOriginX = 0.0, dOriginY = 0.0;
      TIFFGetFieldDouble(m_pTiff, TIFFTAG_XPOSITION, &dOriginX);
      TIFFGetFieldDouble(m_pTiff, TIFFTAG_YPOSITION, &dOriginY);
      pProps->xOrigin = int(dOriginX * dResolution + 0.5);
      pProps->yOrigin = int(dOriginY * dResolution + 0.5);

      GetFullImageSize(m_pTiff, pProps->nFullWidth, pProps->nFullHeight);
   }

   int nCompression = COMPRESSION_NONE;
   TIFFGetFieldDefaulted(m_pTiff, TIFFTAG_COMPRESSION, &nCompression);
   pProps->bCompress = (nCompression != COMPRESSION_NONE);

   TIFFGetFieldString(m_pTiff, TIFFTAG_IMAGEDESCRIPTION, pProps->szDescription, sizeof(pProps->szDescription));

   pProps->nStrips    = TIFFNumberOfStrips(m_pTiff);
   pProps->nStripSize = TIFFStripSize(m_pTiff);

   return TRUE;
}

//===============================================================================================
// FUNCTION: Read
// PURPOSE:  Read an image from the file.
//
BOOL CTiffFile::ReadStrip(int nStrip, BYTE *pbyStrip, int nStripSize)
{
   METHOD_PROLOGUE(this);
   return (TIFFReadEncodedStrip(m_pTiff, nStrip, pbyStrip, nStripSize)!=-1);
}

//===============================================================================================
// FUNCTION: Read
// PURPOSE:  Read an image from the file.
//
BOOL CTiffFile::Read(int nImage, CTiffImage *pImage)
{
   METHOD_PROLOGUE(this);

   TiffImageProps Props;
   if (!SetActiveImage(nImage, &Props))
      return SetLastError(TIFF_CANNOT_READ_FILE);

   // Assume that 12 bit TIFF files are just like 16 bit TIFF files (?).
   int nBitsPerPixel  = Props.nBitsPerSample * Props.nSamplesPerPixel;
   int nBytesPerPixel = nBitsPerPixel / 8;

   // Must be at least 8 bits per pixel.
   if (nBytesPerPixel < 1)
      return SetLastError(TIFF_UNSUPPORTED_FORMAT);

#ifdef READ_RGB_IMAGE

   // TIFFReadRGBAImage only returns 32bit color images.
   nBytesPerPixel         = 32;
   Props.nSamplesPerPixel = 4;
   if (!pImage->SetProps(&Props))
      return SetLastError(TIFF_OUTOFMEMORY);

   // This code will read RGB TIFF images, but they come out upsidedown!
   char szMsg[1024] = "";
   if (!TIFFRGBAImageOK(m_pTiff, szMsg))
   {
      TRACE1("TIFF error: %s\n", szMsg);
      AXU_strncpyz(m_szLastError, szMsg, sizeof(m_szLastError));
      return FALSE;
   }
   ULONG *pRGBImage = (ULONG *)pImage->GetImagePtr(0, Props.nHeight);
   if (!TIFFReadRGBAImage(m_pTiff, Props.nWidth, Props.nHeight, pRGBImage, 0))
   {
      pImage->ReleaseImagePtr();
      return FALSE;
   }
   int nLine = Props.nHeight;
   int nStrip = 1;

#else

   if (Props.nSamplesPerPixel > 1)
      return SetLastError(TIFF_CANNOT_READ_RGB);

   if (!pImage->SetProps(&Props))
      return SetLastError(TIFF_OUTOFMEMORY);

   int nLine = 0;
   for (int nStrip=0; nStrip<Props.nStrips; nStrip++)
   {
      UINT uStripHeight = min(Props.nRowsPerStrip, Props.nHeight - nLine);
      BYTE *pbyImage = pImage->GetImagePtr(nLine, uStripHeight);
      if (!pbyImage || !ReadStrip(nStrip, pbyImage, Props.nStripSize))
         break;

      if (nBitsPerPixel % 8)
         UnpackStrip(pbyImage, Props.nStripSize, uStripHeight*Props.nWidth, nBitsPerPixel);

      nLine    += uStripHeight;
      m_nLines += uStripHeight;
      Notify(UINT(m_nLines*100 / m_nTotalLines));
   }

#endif

   pImage->ReleaseImagePtr();
   pImage->SetHeight(nLine);

   if (nStrip==0)
      return SetLastError(TIFF_CANNOT_READ_FILE);

   return TRUE;
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Close an image file.
//
void CTiffFile::Close()
{
   METHOD_PROLOGUE(this);
   if (m_pTiff)
   {
      TIFFClose(m_pTiff);
      m_pTiff = NULL;
      Notify(100);
   }
}

//===============================================================================================
// FUNCTION: Notify
// PURPOSE:  Send a notification to the notify window.
//
void CTiffFile::Notify(UINT uPercent)
{
   MEMBERASSERT();
   if (m_hwndNotify)
      SendMessage(m_hwndNotify, m_uNotifyMsg, uPercent, 0);
}

//===============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  Returns a pointer to the last error string.
//
LPCSTR CTiffFile::GetLastError() const
{
   MEMBERASSERT();
   return m_szLastError;
}

//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets an error string and returns FALSE as a convenience.
//
BOOL CTiffFile::SetLastError(LPCSTR pszErrorMsg)
{
   MEMBERASSERT();
   AXU_strncpyz(m_szLastError, pszErrorMsg, sizeof(m_szLastError));
   return FALSE;
}

//===============================================================================================
// FUNCTION: GetAxonFileVersion
// PURPOSE:  Returns the version of tiff format that this file was written to.
//
double CTiffFile::GetAxonFileVersion() const
{
   MEMBERASSERT();
   return m_dAxonFileVersion;
}

#if 0

/*
 * Private Extended TIFF library interface.
 *
 *  uses private LIBTIFF interface.
 *
 *  The portions of this module marked "XXX" should be
 *  modified to support your tags instead.
 *
 *  written by: Niles D. Ritter
 *
 */

/* These tags are not valid, but are provided for example */
#define TIFFTAG_EXAMPLE_MULTI      61234
#define TIFFTAG_EXAMPLE_SINGLE     61235
#define TIFFTAG_EXAMPLE_ASCII      61236

#include "\AxonDev\3rdParty\LibTiff\tiffiop.h"
#include "xtiffio.h"

/**********************************************************************
 *               User Configuration
 **********************************************************************/

// XXX - Define number of your extended tags here
#define NUM_XFIELD 3
#define XFIELD_BASE             (FIELD_LAST-NUM_XFIELD)

//  XXX - Define your Tag Fields here
#define FIELD_EXAMPLE_MULTI     (XFIELD_BASE+0)
#define FIELD_EXAMPLE_SINGLE    (XFIELD_BASE+1)
#define FIELD_EXAMPLE_ASCII     (XFIELD_BASE+2)

// XXX - Define Private directory tag structure here
struct XTIFFDirectory
{
   uint16   xd_num_multi;       // dir-count for the multi tag
   double*  xd_example_multi;
   uint32   xd_example_single; 
   char*    xd_example_ascii;
};

/**********************************************************************
 *    Nothing below this line should need to be changed by the user.
 **********************************************************************/

struct xtiff
{
   TIFF           *xtif_tif;            // parent TIFF pointer
   uint32          xtif_flags;
   #define         XTIFFP_PRINT   0x00000001
   XTIFFDirectory  xtif_dir;            // internal rep of current directory
   TIFFVSetMethod  xtif_vsetfield;      // inherited tag set routine
   TIFFVGetMethod  xtif_vgetfield;      // inherited tag get routine
   TIFFPrintMethod xtif_printdir;       // inherited dir print method
};

#define PARENT(xt,pmember)     ((xt)->xtif_ ## pmember) 
#define TIFFMEMBER(tf,pmember) ((tf)->tif_ ## pmember) 
#define XTIFFDIR(tif)          ((xtiff *)TIFFMEMBER(tif,clientdir))
   
/* Extended TIFF flags */
#define XTIFF_INITIALIZED 0x80000000
   
/*
 * xtif_dir.c
 *
 * Extended TIFF Directory Tag Support.
 *
 *  You may use this file as a template to add your own
 *  extended tags to the library. Only the parts of the code
 *  marked with "XXX" require modification. Three example tags
 *  are shown; you should replace them with your own.
 *
 *  Author: Niles D. Ritter
 *
 *  Tiff info structure.
 *
 *     Entry format:
 *        { TAGNUMBER, ReadCount, WriteCount, DataType, FIELDNUM, 
 *          OkToChange, PassDirCountOnSet, AsciiName }
 *
 *     For ReadCount, WriteCount, -1 = unknown; used for mult-valued
 *         tags and ASCII.
 */

static const TIFFFieldInfo xtiffFieldInfo[] =
{
  // XXX Replace these example tags with your own extended tags
   { TIFFTAG_EXAMPLE_MULTI,   -1,-1, TIFF_DOUBLE,  FIELD_EXAMPLE_MULTI, TRUE, TRUE,  "MyMultivaluedTag" },
   { TIFFTAG_EXAMPLE_SINGLE,   1, 1, TIFF_LONG, FIELD_EXAMPLE_SINGLE,   TRUE, FALSE, "MySingleLongTag" },
   { TIFFTAG_EXAMPLE_ASCII,   -1,-1, TIFF_ASCII,   FIELD_EXAMPLE_ASCII, TRUE, FALSE, "MyAsciiTag" },
};

static void _XTIFFPrintDirectory(TIFF* tif, FILE* fd, long flags)
{
   xtiff          *xt = XTIFFDIR(tif);
   XTIFFDirectory *xd = &xt->xtif_dir;

   // call the inherited method
   if (PARENT(xt,printdir))
      (PARENT(xt,printdir))(tif,fd,flags);

   // XXX Add field printing here. Replace the three example tags implemented below with your own.
   fprintf(fd,"--Axon Tags--\n");

   if (TIFFFieldSet(tif,FIELD_EXAMPLE_MULTI))
   {
      fprintf(fd, "  My Multi-Valued Doubles:");
      double *value = xd->xd_example_multi;
      int     num   = xd->xd_num_multi;

      fprintf(fd,"(");
      for (int i=0; i<num; i++)
         fprintf(fd, " %lg", *value++);
      fprintf(fd,")\n");
   }

   if (TIFFFieldSet(tif,FIELD_EXAMPLE_SINGLE))
      fprintf(fd, "  My Single Long Tag:  %lu\n", xd->xd_example_single);

   if (TIFFFieldSet(tif,FIELD_EXAMPLE_ASCII))
      _TIFFprintAsciiTag(fd,"My ASCII Tag",
          xd->xd_example_ascii);
}

static int _XTIFFVSetField(TIFF* tif, ttag_t tag, va_list ap)
{
   xtiff          *xt = XTIFFDIR(tif);
   XTIFFDirectory *xd = &xt->xtif_dir;

   // va_start is called by the calling routine
   switch (tag)
   {
      // XXX put your extended tags here; replace the implemented example tags with your own. 
      case TIFFTAG_EXAMPLE_MULTI:
         // multi-valued tags need to store the count as well
         xd->xd_num_multi = (uint16) va_arg(ap, int);
         _TIFFsetDoubleArray(&xd->xd_example_multi, va_arg(ap, double*), (long) xd->xd_num_multi);
         break;
      case TIFFTAG_EXAMPLE_SINGLE:
         xd->xd_example_single = va_arg(ap, uint32);
         break;
      case TIFFTAG_EXAMPLE_ASCII:
         _TIFFsetString(&xd->xd_example_ascii, va_arg(ap, char*));
         break;
      default:
         // call the inherited method
         return (PARENT(xt,vsetfield))(tif,tag,ap);
         break;
   }

   // we have to override the print method here, after the compression tags have gotten to it.
   // This makes sense because the only time we would need the extended print method is if an 
   // extended tag is set by the reader.
   if (!(xt->xtif_flags & XTIFFP_PRINT))
   {
      PARENT(xt,printdir) = TIFFMEMBER(tif,printdir);
      TIFFMEMBER(tif,printdir) = _XTIFFPrintDirectory;
      xt->xtif_flags |= XTIFFP_PRINT;
   }
   TIFFSetFieldBit(tif, _TIFFFieldWithTag(tif, tag)->field_bit);
   tif->tif_flags |= TIFF_DIRTYDIRECT;

   va_end(ap);
   return 1;
}

static int _XTIFFVGetField(TIFF* tif, ttag_t tag, va_list ap)
{
   xtiff *xt = XTIFFDIR(tif);
   XTIFFDirectory* xd = &xt->xtif_dir;

   switch (tag)
   {
      // XXX put your extended tags here; replace the implemented example tags with your own.

      case TIFFTAG_EXAMPLE_MULTI:
         *va_arg(ap, uint16*)  = xd->xd_num_multi;
         *va_arg(ap, double**) = xd->xd_example_multi;
         break;
      case TIFFTAG_EXAMPLE_ASCII:
         *va_arg(ap, char**) = xd->xd_example_ascii;
         break;
      case TIFFTAG_EXAMPLE_SINGLE:
         *va_arg(ap, uint32*) = xd->xd_example_single;
         break;
      default:
         // return inherited method
         return (PARENT(xt,vgetfield))(tif,tag,ap);
         break;
   }
   return (1);
}

#define  CleanupField(member) {     \
   if (xd->member)                  \
   {                                \
      _TIFFfree(xd->member);        \
      xd->member = 0;               \
   }                                \
}

// Release storage associated with a directory.
static void _XTIFFFreeDirectory(xtiff* xt)
{
   XTIFFDirectory* xd = &xt->xtif_dir;

   // XXX - Purge all Your allocated memory except for the xtiff directory itself. 
   // This includes all fields that require a _TIFFsetXXX call in _XTIFFVSetField().
   CleanupField(xd_example_multi);
   CleanupField(xd_example_ascii);
}
#undef CleanupField

static void _XTIFFLocalDefaultDirectory(TIFF *tif)
{
   xtiff *xt = XTIFFDIR(tif);
   XTIFFDirectory* xd = &xt->xtif_dir;

   // Install the extended Tag field info
   _TIFFMergeFieldInfo(tif, xtiffFieldInfo, ELEMENTS_IN(xtiffFieldInfo));

   // free up any dynamically allocated arrays before the new directory is read in.
   _XTIFFFreeDirectory(xt);   
   _TIFFmemset(xt,0,sizeof(xtiff));

   // Override the tag access methods
   PARENT(xt,vsetfield) =  TIFFMEMBER(tif,vsetfield);
   TIFFMEMBER(tif,vsetfield) = _XTIFFVSetField;
   PARENT(xt,vgetfield) =  TIFFMEMBER(tif,vgetfield);
   TIFFMEMBER(tif,vgetfield) = _XTIFFVGetField;

   // XXX Set up any default values here.
   xd->xd_example_single = 234;
}

/**********************************************************************
 *    Nothing below this line should need to be changed.
 **********************************************************************/

static TIFFExtendProc _ParentExtender;

// This is the callback procedure, and is called by the DefaultDirectory method
// every time a new TIFF directory is opened.
static void _XTIFFDefaultDirectory(TIFF *tif)
{
   // Allocate Directory Structure if first time, and install it
   if (!(tif->tif_flags & XTIFF_INITIALIZED))
   {
      xtiff *xt = _TIFFmalloc(sizeof(xtiff));
      if (!xt) // handle memory allocation failure here !
         return;

      _TIFFmemset(xt, 0, sizeof(xtiff));

      // Install into TIFF structure.
      TIFFMEMBER(tif, clientdir) = (tidata_t)xt;
      tif->tif_flags |= XTIFF_INITIALIZED; // dont do this again!
   }
   
   // set up our own defaults
   _XTIFFLocalDefaultDirectory(tif);

   // Since an XTIFF client module may have overridden the default directory method,
   // we call it now to allow it to set up the rest of its own methods.
   if (_ParentExtender) 
      (*_ParentExtender)(tif);
}

/*
 *  XTIFF Initializer -- sets up the callback
 *   procedure for the TIFF module.
 */

static void _XTIFFInitialize(void)
{
   static BOOL s_bInitialized = FALSE;
   if (s_bInitialized)
      return;

   // Grab the inherited method and install
   _ParentExtender = TIFFSetTagExtender(_XTIFFDefaultDirectory);
   s_bInitialized = TRUE;
}

#endif
