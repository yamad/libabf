//************************************************************************************************
//
//    Copyright (c) 1993-2000 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  AXOUTILS32.H
// PURPOSE: Contains prototypes & #defines for standard Axon utility functions for
//          Windows programming.
// AUTHOR:  BHI  Oct 1993
//          CF   Mar 2000

#ifndef INC_AXOUTILS32_H
#define INC_AXOUTILS32_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#pragma pack(push, 4)

#define AXUAPI

// Version numbering of this API
#define AXU_APIVERSION      1,0,0,01
#define AXU_APIVERSION_STR  "1.0.0.01"

// Macros for rounding.
#define AXU_ROUNDUP(n, m)   (((long)(((n)+(m)-1)/(m)))*(m))
#define AXU_ROUNDDOWN(n, m) (((long)((n)/(m)))*(m))

//==========================================================================================
// #include "axoutils32.cpp"
//
// N.B. "#include" specified so that you can right click on the file name in DevStudio
//      and select "open" to see the implimentation file.
//
AXUAPI void WINAPI AXU_NormalizeRect(LPRECT lpRect);
AXUAPI void WINAPI AXU_ClipToRect(LPCRECT pRect, int &x, int &y);

AXUAPI void WINAPI AXU_ClientToScreen(HWND hWnd, LPRECT lpRect);
AXUAPI void WINAPI AXU_ScreenToClient(HWND hWnd, LPRECT lpRect);

AXUAPI HBRUSH WINAPI AXU_CreateDragBrush(void);

AXUAPI int   WINAPI AXU_MMToPixels( float fMM );
AXUAPI float WINAPI AXU_PixelsToMM( int nPixels );
AXUAPI int   WINAPI AXU_PointsToPixels( int nPoints );
AXUAPI int   WINAPI AXU_PixelsToPoints( int nPixels );

AXUAPI void WINAPI AXU_CenterWindow( HWND hWnd, HWND hwndParent );
AXUAPI void WINAPI AXU_CenterDialog( HWND hDlg );
AXUAPI void WINAPI AXU_SetWindowRect(HWND hWnd, const RECT *pRect);

// Look for a previous instance of the application, if found re-activate it.
// Refer to comment in AXOUTILS.CPP for code to set the class name of an MFC app.
AXUAPI BOOL WINAPI AXU_FindAndReactivateWindow(LPCSTR pszClassName);
AXUAPI HANDLE WINAPI AXU_GetStartupMutex(LPCSTR pszClassName);
AXUAPI void AXU_KillStartupMutex(HANDLE hMutex);

AXUAPI int WINAPI AXU_GetProgramName(HWND hWnd, LPSTR pszProgramName, UINT uMaxLen);

AXUAPI LPSTR WINAPI AXU_StripWhiteSpace(LPSTR psz);

AXUAPI BOOL WINAPI AXU_SIPrefixToMultiplier( char cSIPrefix, float *pfMultiplier );
AXUAPI BOOL WINAPI AXU_MultiplierToSIPrefix( float fMultiplier, char *pcSIPrefix );

AXUAPI BOOL WINAPI AXU_IsVxDInstalled(const UINT VxD_id);

AXUAPI int WINAPI AXU_FormatUnits(LPSTR pszBuf, LPCSTR pszUnits, UINT uMaxSize);

AXUAPI BOOL WINAPI AXU_SplitVersionNumber( LPCSTR pszVersString, BYTE *pbyVersItems );
AXUAPI int  WINAPI AXU_CompareVersionNumbers(LPCSTR pszVersion1, LPCSTR pszVersion2);

AXUAPI HINSTANCE WINAPI AXU_GetInstanceHandle(LPCVOID *pv);

AXUAPI int WINAPI AXU_CompareFloats(float fValue1, float fValue2);

typedef void (__cdecl *AXU_TerminateCallback)();
AXUAPI AXU_TerminateCallback WINAPI AXU_SetTerminateCallback(AXU_TerminateCallback pfnCallback);

//==========================================================================================
// #include "Sprintf.cpp"
//

AXUAPI LPSTR WINAPI AXU_strncpyz(LPSTR pszDest, LPCSTR pszSrce, UINT uBufSize);
AXUAPI LPSTR WINAPI AXU_strncatz(LPSTR pszDest, LPCSTR pszSrce, UINT uBufSize);

#define AXU_STRNCPYZ(d,s)  AXU_strncpyz((d), (s), ELEMENTS_IN(d))
#define AXU_STRNCATZ(d,s)  AXU_strncatz((d), (s), ELEMENTS_IN(d))

AXUAPI int cdecl AXU_sprintf(char *szBuffer, UINT uBufSize, LPCSTR szFormat, ...);
AXUAPI int   WINAPI AXU_vsprintf(char *szBuffer, UINT uBufSize, LPCSTR szFormat, void *argptr );
AXUAPI LPSTR WINAPI AXU_strstri(LPCSTR pszBuffer, LPCSTR pszStr);
AXUAPI LPSTR WINAPI AXU_W2A(LPSTR lpa, LPCWSTR lpw, UINT uMaxLen);
AXUAPI LPWSTR WINAPI AXU_A2W(LPWSTR lpw, LPCSTR lpa, UINT uMaxLen);

AXUAPI void WINAPI AXU_GUIDToString( const GUID &guid, LPSTR pszGuid, UINT uBufLen);
AXUAPI bool WINAPI AXU_StringToGUID( LPCSTR pszGuid, GUID &guid);

//==========================================================================================
// #include "Shutdown.cpp"
//

AXUAPI BOOL WINAPI AXU_IsMemberOfNTGroup(DWORD dwAlias);
AXUAPI BOOL WINAPI AXU_IsNTAdministrator();
AXUAPI BOOL WINAPI AXU_IsNTPowerUser();
AXUAPI BOOL WINAPI AXU_IsNTUser();
AXUAPI BOOL WINAPI AXU_IsNTGuest();
AXUAPI BOOL WINAPI AXU_SetPrivilege(LPCTSTR szPrivilegeName, BOOL bEnable);
AXUAPI BOOL WINAPI AXU_HasPrivilege(LPCTSTR szPrivilegeName);
AXUAPI BOOL WINAPI AXU_ExitWindows(UINT uFlags);

//==========================================================================================
// #include "RegistryUtils.cpp"
//

// Get the shell execute command for a registered extension.
AXUAPI BOOL WINAPI AXU_GetShellExecuteCommand(LPCSTR pszExt, LPSTR pszExeString, UINT uExeStringLen,
                                              LPSTR *ppsParameter);

// Run the internet browser and bring up a particular URL.
AXUAPI BOOL WINAPI AXU_ConnectToWWW(LPCSTR pszURLString);

//==========================================================================================
// #include "DiskUtils.cpp"
//

// Returns the path to the directory above that occupied by the calling app (EXE).
AXUAPI LPCSTR WINAPI AXU_GetAxonRoot();

// Adds the given directory to the location of the Axon Directory Tree.
AXUAPI void WINAPI AXU_BuildAxonPath(LPCSTR pszDirectory, LPSTR pszPath, UINT uMaxLen);

// Gets a fully qualified path to the default data directory for Axon apps.
AXUAPI void WINAPI AXU_GetDefaultDataDirectory(LPSTR pszDataDir, UINT uMaxLen);

// Gets a fully qualified path to the default template directory for Axon apps.
// This is for event detection templates.
AXUAPI void WINAPI AXU_GetDefaultTemplateDirectory(LPSTR pszTemplateDir, UINT uMaxLen);

// Gets a fully qualified path to the default protocol directory for Axon apps.
AXUAPI void WINAPI AXU_GetDefaultProtocolDirectory(LPSTR pszDataDir, UINT uMaxLen);

// Determines whether a specified path name corresponds to a directory.
AXUAPI BOOL WINAPI AXU_IsDirectory(LPCSTR szPath);

// Determines whether a specified path is a UNC path (e.g. \\Bruce\Drive_C\config.sys.)
AXUAPI BOOL WINAPI AXU_IsUNCPath(LPCSTR szPath);

// Returns TRUE if it is possible to write in the given directory.
AXUAPI BOOL WINAPI AXU_IsDirectoryWriteable(LPCSTR pszPath);

// Checks that the TEMP directory exists and is writeable.
AXUAPI BOOL WINAPI AXU_CheckTempDirectory();

// Gets a temporary path name.
AXUAPI BOOL WINAPI AXU_GetTempPath(UINT uMaxLen, LPSTR pszPath);

// Gets a temporary file name in the directory pointed to by the %TEMP% environment variable.
AXUAPI UINT WINAPI AXU_GetTempFileName(LPCSTR szPrefix, UINT uUnique, LPSTR lpTempName);

// Extracts the root directory of a full or partial path.
AXUAPI BOOL WINAPI AXU_GetRootDir(LPCSTR pszFileName, LPSTR pszRoot, UINT uRootLen);

// Retrieves information about the specified disk, including the amount of free space on the disk. 
// NOTE: This function copes with UNC names, GetDiskFreeSpace does not.
//
// BHI: This function retired due to incorrect return values under FAT32.
//
// AXUAPI BOOL WINAPI AXU_GetDiskFreeSpace(LPCSTR szRootDir, DWORD *pdwSectorsPerCluster, DWORD *pdwBytesPerSector, 
//                                         DWORD *pdwFreeClusters, DWORD *pdwClusters);
AXUAPI BOOL WINAPI AXU_GetDiskFreeSpaceEx(LPCTSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller,
                                          PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes);

// Wrapper around DeleteFile() that will also delete readonly files.
AXUAPI BOOL WINAPI AXU_DeleteFileEx(LPCTSTR pszFilename, BOOL bDeleteIfReadOnly);

// Wrapper function that brings the Win32 MoveFileEx functionality to Win95.
AXUAPI BOOL WINAPI AXU_MoveFileEx(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags);

// Wrapper around CreateDirectory that builds the whole path component by component.
AXUAPI BOOL WINAPI AXU_CreateDirectory(LPCTSTR pszFilename);

// Gets the amount of free space on a particular disk in kBytes.
AXUAPI DWORD WINAPI AXU_GetDiskFreeKB(LPCSTR pszFileName);

// Gets the total size of a disk.
AXUAPI DWORD WINAPI AXU_GetDiskSpaceKB(LPCSTR pszFileName);

// Allows the user to browse to select a directory (folder).
AXUAPI BOOL WINAPI AXU_ChooseDirectory(HWND hwndOwner, LPCSTR pszTitle, LPSTR pszPath, UINT uPathSize);
AXUAPI BOOL WINAPI AXU_ChooseDirectoryEx(HWND hwndOwner, LPCSTR pszTitle, LPCSTR pszCurrent, 
                                         LPSTR pszPath, UINT uPathSize);

// Get the long filename for all parts of a file path. (You can pass the same buffer for input and output)
AXUAPI DWORD WINAPI AXU_GetFullPathName(LPCTSTR lpFileName,  // address of name of file to find path for 
                                        DWORD nBufferLength, // size, in characters, of path buffer 
                                        LPTSTR lpBuffer,     // address of path buffer 
                                        LPTSTR *lpFilePart); // address of filename in path 

// Get the relative path from base_path to target_path
// base_path must end with a dirsep
AXUAPI BOOL WINAPI AXU_GetRelativePath(LPCTSTR base_path, LPCTSTR target_path, LPTSTR buffer, size_t buffer_length);

AXUAPI BOOL WINAPI AXU_RebuildShellIcons(void);
AXUAPI UINT WINAPI AXU_DerefShellLink(LPCTSTR lpszLinkName, LPTSTR lpszDest, UINT cchDest);

//==========================================================================================
// #include "textfont.cpp"
//
AXUAPI int WINAPI AXU_GetTextWidth( HDC hDC, LPCSTR psz );
AXUAPI int WINAPI AXU_GetTextHeight( HDC hDC, LPCSTR psz );
AXUAPI void WINAPI AXU_GetFontSizes( HDC hDC, HFONT hFont, int *pnHeight, int *pnWidth );
AXUAPI void WINAPI AXU_GetTextSizes( HDC hDC, HFONT hFont, LPCSTR psz, int *pnHeight, int *pnWidth );

AXUAPI void WINAPI AXU_HClipTextOut( HDC hDC, int x, int y, LPCSTR psz, LPCRECT lpR );
AXUAPI void WINAPI AXU_VClipTextOut( HDC hDC, int x, int y, LPCSTR psz, LPCRECT lpR );

AXUAPI HFONT WINAPI AXU_GetCurrentFont(HDC hDC);
AXUAPI BOOL WINAPI AXU_FillLogFont( HDC hDC, HFONT hFont, LPLOGFONT lplf );
AXUAPI void WINAPI AXU_MultiLineExtent(HDC hDC, HFONT hFont, LPCSTR psz, int *pnHeight, int *pnWidth );
AXUAPI HFONT WINAPI AXU_CreateDefaultDlgFont(void);

//==========================================================================================
// #include "winprops.cpp"
//
AXUAPI BOOL WINAPI AXU_SetDlgParam( HWND hWnd, LPARAM lParam );
AXUAPI LPARAM WINAPI AXU_GetDlgParam( HWND hWnd );
AXUAPI LPARAM WINAPI AXU_RemoveDlgParam( HWND hWnd );

AXUAPI BOOL WINAPI AXU_SetProp( HWND hWnd, LPCSTR pszPropName, LPARAM lParam );
AXUAPI LPARAM WINAPI AXU_GetProp( HWND hWnd, LPCSTR pszPropName );
AXUAPI LPARAM WINAPI AXU_RemoveProp( HWND hWnd, LPCSTR pszPropName );

//==========================================================================================
// #include "wversion.cpp"
//
#define AXU_ENV_UNKNOWN     0x00000000    // Unknown host
#define AXU_ENV_WIN32S      0x00000020    // Win32s           
#define AXU_ENV_WIN32C      0x00000040    // Windows 95       
#define AXU_ENV_WINNT       0x00000080    // Windows NT       
#define AXU_ENV_WIN98       0x00000240    // Windows 98       
#define AXU_ENV_WIN2000     0x00000480    // Windows 2000     
#define AXU_ENV_WINME       0x00000A40    // Windows Millenium
#define AXU_ENV_WINXP       0x00000C80    // Windows XP

AXUAPI DWORD WINAPI AXU_GetHostType(void);
AXUAPI BOOL WINAPI AXU_IsWin32s(void);
AXUAPI BOOL WINAPI AXU_IsWinNT(void);
AXUAPI BOOL WINAPI AXU_IsWin95(void);
AXUAPI BOOL WINAPI AXU_IsWin98(void);
AXUAPI BOOL WINAPI AXU_IsWinME(void);
AXUAPI BOOL WINAPI AXU_IsWin2000(void);
AXUAPI BOOL WINAPI AXU_IsWinXP(void);
AXUAPI BOOL WINAPI AXU_IsWow64Process(HANDLE hProcess, BOOL *pWow64Process);
AXUAPI BOOL WINAPI AXU_GetDCOMVersion(LPSTR pszVersion, UINT uMaxLen);
AXUAPI BOOL WINAPI AXU_GetIEVersion(LPSTR pszVersion, UINT uMaxLen);

//==========================================================================================
// #include "colors.cpp"
//
AXUAPI COLORREF WINAPI AXU_SelectSolidColor( HWND hWnd, COLORREF clrInit );
AXUAPI BOOL WINAPI AXU_SelectColor( HWND hWnd, COLORREF *pColor, BOOL bSolid, BOOL bExtended );
AXUAPI UINT WINAPI AXU_GetNumDisplayColors(void);

//==========================================================================================
// #include "memstat.cpp"
//
AXUAPI DWORD WINAPI AXU_GetPhysicalMemory(void);

//==========================================================================================
// #include "selticks.cpp"
AXUAPI BOOL WINAPI AXU_ChooseTicsDbl( int nScrMax, int nScrMin, double *pdDataMax, 
                                      double *pdDataMin, int nMinTicPixels, double *pdFirst, 
                                      int *pnFirstLabel, int *pnLabelFreq, double *pdTicStep, 
                                      int *pnNumTics, double *pdFactor, double *pdShift );
AXUAPI BOOL WINAPI AXU_ChooseTicsFlt( int nScrMax, int nScrMin, float *pfDataMax, 
                                      float *pfDataMin, int nMinTicPixels, float *pfFirst, 
                                      int *pnFirstLabel, int *pnLabelFreq, float *pfTicStep, 
                                      int *pnNumTics, float *pfFactor, float *pfShift );
struct AXU_TicParams
{
   BOOL     bLogScale;     // in       // TRUE if a log mapping is to be used.
   int      nScrMin;       // in       // pixel coordinate corresponding to dDataMax
   int      nScrMax;       // in       // pixel coordinate corresponding to dDataMin
   int      nMinTicPixels; // in       // number of pixels between the two closest tics.
   double   dDataMax;      // in/out   // Data value to put at the top of the axis
   double   dDataMin;      // in/out   // data value to put at the bottom of the axis
   double   dFirst;        // out      // value of the first tic on the axis
   double   dTicStep;      // out      // Step between minor tics.
   double   dFactor;       // out      // conversion factor.
   double   dShift;        // out      // conversion constant.
   int      nFirstLabel;   // out      // Count of the first tick to be labeled
   int      nLabelFreq;    // out      // Frequency at which to label tics
   int      nNumTics;      // out      // total number of tics.
   int      nDivisor;      // out      // number of intervals in each decade (not num tics!).
   double   dFirstMajor;   // out      // Value of the first major tic.
   BYTE     byUnused[64];  // ???      // Provision for future expansion

   AXU_TicParams() { memset(this, 0, sizeof(*this)); }
};

AXUAPI BOOL WINAPI AXU_ChooseTicsEx( AXU_TicParams *pParams );

struct AXU_Tic
{
   int    nPixel;
   double dValue;
   BOOL   bMajorTic;
};

AXUAPI void WINAPI AXU_GetTics( AXU_TicParams *pParams, int nFirstTic, AXU_Tic *pTics, int nNumTics);

AXUAPI int    WINAPI AXU_DataToPixel( const AXU_TicParams *pParams, double dDataValue );
AXUAPI double WINAPI AXU_PixelToData( const AXU_TicParams *pParams, int nPixel );

AXUAPI double WINAPI AXU_PixelToNiceNumberEx( const AXU_TicParams *pParams, int nPixel );

AXUAPI BOOL WINAPI AXU_PixelToNiceNumberDbl( int nPixel, double dUUtoPixelFactor, double dUUtoPixelShift,
                                             double *pdNiceNumber );
AXUAPI BOOL WINAPI AXU_PixelToNiceNumberFlt( int nPixel, float fUUtoPixelFactor, float fUUtoPixelShift,
                                             float *pfNiceNumber );
AXUAPI BOOL WINAPI AXU_PixelToLargerNiceNumber( int nPixel, double dUUtoPixelFactor, 
                                                double *pdNiceNumber );
AXUAPI BOOL WINAPI AXU_ConvertToNiceNumber(double dNumber, int nDigits, BOOL bAfterDecPlace, double *pdNiceNumber);
                
//==========================================================================================
// #include "3d_utils.cpp"

// DRAWING STYLES FOR AXU_3DRectangle
#define AXU_3DUP            0
#define AXU_3DDOWN          1
#define AXU_3DPUSHED        2
#define AXU_3DBORDER        4
#define AXU_3DTHICKBORDER   (AXU_3DBORDER|8)
#define AXU_3DRECESSED      16

AXUAPI void WINAPI AXU_3DRectangle(HDC hDC, RECT *pr, UINT uStyle, UINT uThickness);


// DRAWING STYLES FOR AXU_3DDrawButtonEx
#define AXU_BUTTONDOWN     1
#define AXU_BUTTONPUSHEDIN 2
#define AXU_BUTTONFOCUS    4
#define AXU_BUTTONDISABLED 8

AXUAPI void WINAPI AXU_3DDrawButtonEx(HDC hDC, RECT *prItem, LPCSTR szText, 
                                      DWORD dwFlags, HFONT hFont);
AXUAPI void WINAPI AXU_3DDrawButton(HDC hDC, RECT *prItem, LPCSTR szText, BOOL bPushed, 
                                    BOOL bFocus, HFONT hFont);

//==========================================================================================
// #include "slzutils.cpp"
//

// Set/Get the root from which all registry paths are derived.
AXUAPI BOOL   WINAPI AXU_SetRegistryRoot(LPCSTR pszRegistryRoot);
AXUAPI LPCSTR WINAPI AXU_GetRegistryRoot();

// Set/Get the tree which contains all registry paths are derived.
AXUAPI BOOL WINAPI AXU_SetRegistryTree(HKEY hkeyTree);
AXUAPI HKEY WINAPI AXU_GetRegistryTree();

AXUAPI void WINAPI AXU_UseINIFile(BOOL bUseINIFile);

// Write various types of data out to a .INI file
AXUAPI BOOL WINAPI AXU_WritePrivateProfileString( LPCSTR pszSection, LPCSTR pszEntry, 
                                                  LPCSTR pszVal, LPCSTR pszFilename );

AXUAPI BOOL WINAPI AXU_WritePrivateProfileBOOL( LPCSTR pszSection, LPCSTR pszEntry, BOOL bVal,
                                                LPCSTR pszFilename );

AXUAPI BOOL WINAPI AXU_WritePrivateProfileBool( LPCSTR pszSection, LPCSTR pszEntry, bool bVal,
                                                LPCSTR pszFilename );

AXUAPI BOOL WINAPI AXU_WritePrivateProfileUINT( LPCSTR pszSection, LPCSTR pszEntry, UINT uVal,
                                                LPCSTR pszFilename );

AXUAPI BOOL WINAPI AXU_WritePrivateProfileInt( LPCSTR pszSection, LPCSTR pszEntry, int nVal,
                                               LPCSTR pszFilename );
                                        
AXUAPI BOOL WINAPI AXU_WritePrivateProfileShort( LPCSTR pszSection, LPCSTR pszEntry, short nValue, 
                                                LPCSTR pszRegKey);

AXUAPI BOOL WINAPI AXU_WritePrivateProfileLong( LPCSTR pszSection, LPCSTR pszEntry, long lVal,
                                                LPCSTR pszFilename );
                                         
AXUAPI BOOL WINAPI AXU_WritePrivateProfileDWORD( LPCSTR pszSection, LPCSTR pszEntry, DWORD dwVal,
                                                 LPCSTR pszFilename );
                                         
AXUAPI BOOL WINAPI AXU_WritePrivateProfileFloat( LPCSTR pszSection, LPCSTR pszEntry, float fVal,
                                                 LPCSTR pszFilename );
                                            
AXUAPI BOOL WINAPI AXU_WritePrivateProfileDouble( LPCSTR pszSection, LPCSTR pszEntry, double dVal,
                                                  LPCSTR pszFilename );
                                           

// Get various types of data from a .INI file
AXUAPI BOOL WINAPI AXU_GetPrivateProfileString( LPCSTR pszSection, LPCSTR pszEntry, 
                                                LPCSTR pszDefVal, char *pszVal, int nSize, 
                                                LPCSTR pszFilename );
                                        
AXUAPI BOOL WINAPI AXU_GetPrivateProfileBOOL( LPCSTR pszSection, LPCSTR pszEntry, 
                                              BOOL bDefVal, BOOL *lpbVal, LPCSTR pszFilename );

AXUAPI BOOL WINAPI AXU_GetPrivateProfileBool( LPCSTR pszSection, LPCSTR pszEntry, 
                                              bool bDefVal, bool *lpbVal, LPCSTR pszFilename );

AXUAPI BOOL WINAPI AXU_GetPrivateProfileUINT( LPCSTR pszSection, LPCSTR pszEntry,
                                              UINT uDefVal, UINT *lpuVal, LPCSTR pszFilename );
                                       
AXUAPI BOOL WINAPI AXU_GetPrivateProfileInt( LPCSTR pszSection, LPCSTR pszEntry,
                                             int nDefVal, int *lpnVal, LPCSTR pszFilename );
                                      
AXUAPI BOOL WINAPI AXU_GetPrivateProfileShort( LPCSTR pszSection, LPCSTR pszEntry, short nDefVal, 
                                              short *pnVal, LPCSTR pszFilename );
                                       
AXUAPI BOOL WINAPI AXU_GetPrivateProfileLong( LPCSTR pszSection, LPCSTR pszEntry, long lDefVal, 
                                              long *lplVal, LPCSTR pszFilename );
                                       
AXUAPI BOOL WINAPI AXU_GetPrivateProfileDWORD( LPCSTR pszSection, LPCSTR pszEntry, DWORD dwDefVal, 
                                               DWORD *lpdwVal, LPCSTR pszFilename );
                                       
AXUAPI BOOL WINAPI AXU_GetPrivateProfileFloat( LPCSTR pszSection, LPCSTR pszEntry, float fDefVal, 
                                               float *lpfVal, LPCSTR pszFilename );
                                        
AXUAPI BOOL WINAPI AXU_GetPrivateProfileDouble( LPCSTR pszSection, LPCSTR pszEntry,
                                                double dDefVal, double *lpdVal, 
                                                LPCSTR pszFilename );
                                         
//
// Get various types of data from a .INI file, but use a list of entries for
// validation of the data, and return to the caller an index into the list.
//
AXUAPI BOOL WINAPI AXU_GetPrivateProfileListInt( LPCSTR pszSection, LPCSTR pszEntry,
                                                 int *lpnListVals, UINT uDefInx, UINT *lpuInx, 
                                                 LPCSTR pszFilename );
                                          
AXUAPI BOOL WINAPI AXU_GetPrivateProfileListString( LPCSTR pszSection, LPCSTR pszEntry,
                                                    char *lpnListVals[], UINT uDefInx, 
                                                    UINT *lpuInx, LPCSTR pszFilename );
                                             
// Write/read a RECT/POINT structures to/from a .INI file.
AXUAPI BOOL WINAPI AXU_GetPrivateProfileRect(LPCSTR pszSection, LPCSTR pszEntry, const RECT *prDefault,
                                             RECT *pRect, LPCSTR pszFilename);

AXUAPI BOOL WINAPI AXU_WritePrivateProfileRect( LPCSTR pszSection, LPCSTR pszEntry, 
                                                const RECT *pRect, LPCSTR pszFilename);
                                                
AXUAPI BOOL WINAPI AXU_GetPrivateProfilePoint(LPCSTR pszSection, LPCSTR pszEntry, const POINT *pDefault,
                                              POINT *pPoint, LPCSTR pszFilename);

AXUAPI BOOL WINAPI AXU_WritePrivateProfilePoint( LPCSTR pszSection, LPCSTR pszEntry, 
                                                 const POINT *pPoint, LPCSTR pszFilename);

// Write/read a LOGFONT structure to/from a .INI file.
AXUAPI BOOL WINAPI AXU_GetPrivateProfileLogFont( LPCSTR pszSection, LPCSTR pszEntry, const LOGFONT *plfDefault, 
                                                 LOGFONT *pLogFont, LPCSTR pszFilename);
AXUAPI BOOL WINAPI AXU_WritePrivateProfileLogFont( LPCSTR pszSection, LPCSTR pszEntry, 
                                                   const LOGFONT *pLogFont, LPCSTR pszFilename);

// Write/read a BLOB to/from a .INI file.
AXUAPI UINT WINAPI AXU_GetPrivateProfileBlobSize( LPCSTR pszSection, LPCSTR pszEntry, LPCSTR pszFilename);
AXUAPI BOOL WINAPI AXU_GetPrivateProfileBlob( LPCSTR pszSection, LPCSTR pszEntry, 
                                              void *pBlob, UINT uMaxBytes, LPCSTR pszFilename);
AXUAPI BOOL WINAPI AXU_WritePrivateProfileBlob( LPCSTR pszSection, LPCSTR pszEntry, 
                                                const void *pBlob, UINT uSize, LPCSTR pszFilename);

AXUAPI BOOL WINAPI AXU_WritePrivateProfileVersion( LPCSTR pszSection, HINSTANCE hInstance, LPCSTR pszIniFile );

//==========================================================================================
// #include "scrlbar.cpp"
AXUAPI BOOL WINAPI AXU_ComputeScrollUnits( double dMin, double dMax, double dGranularity,
                                           int *pnPositions, double *pdScrollUnits, double *pdOffset );

//==========================================================================================
// #include "dialog.cpp"
AXUAPI DWORD WINAPI AXU_GetBaseUnits(HFONT hfont);
AXUAPI void WINAPI AXU_MapDialogRect(HWND hWnd, RECT *pRect);
AXUAPI void WINAPI AXU_UnmapDialogRect(HWND hWnd, RECT *pRect);
AXUAPI BOOL WINAPI AXU_ReplaceControl(HWND hwndDialog, int nControlID, HWND hwndControl);
//AXUAPI HWND WINAPI AXU_CreateDialogInWin( HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hParent,
//                                          LPRECT lpDlgRect, HFONT *lphfont);
//AXUAPI BOOL WINAPI AXU_ClearDialog (HWND hwnd);

//==========================================================================================
// #include "appmsg.cpp"
AXUAPI BOOL WINAPI AXU_ResetStopwatch( HWND hWnd );
AXUAPI BOOL WINAPI AXU_GetContextHelp( HWND hWnd, LPCSTR pszModule, UINT uContext);
AXUAPI BOOL cdecl  AXU_SetStatbarString( HWND hWnd, LPCSTR pszFormat, ...);
AXUAPI BOOL WINAPI AXU_IsLogAvailable(HWND hWnd);
AXUAPI BOOL cdecl  AXU_SendDebugMessage( HWND hWnd, LPCSTR pszFormat, ...);
AXUAPI BOOL WINAPI AXU_IsHelpAvailable(HWND hWnd);
AXUAPI BOOL WINAPI AXU_GetStopwatchTime(HWND hWnd, long *plStopwatchSeconds);
AXUAPI BOOL WINAPI AXU_CheckDongle(HWND hWnd, BOOL *pbDonglePresent);
AXUAPI BOOL WINAPI AXU_PumpMessages(HWND hWnd, long lFlags);

//==========================================================================================
// The LabBook log entry message functions
// After we have changed all the current modules to use the new function I will remove the original function from this header.
// This will prevent any current modules from accessing the original function.
// The function will still be implemented in the DLL so existing released products still work.
AXUAPI BOOL cdecl  AXU_SendLogMessage( HWND hWnd, long lFlags, UINT uPriority, LPCSTR pszFormat, ...);
AXUAPI BOOL cdecl  AXU_SendLogMessage01( HWND hWnd, LPCTSTR szFileName, LPCTSTR szEventType, LPCTSTR szEventSubType, long lFlags, UINT uPriority, LPCSTR pszFormat, ...);

//==========================================================================================
// #include "bitmap.cpp"
AXUAPI BOOL WINAPI AXU_MaskBlt(HDC hdcDest, HBITMAP hbm, int xStart, int yStart, COLORREF cTransparentColor);

//==========================================================================================
// #include "RefCount.cpp"
AXUAPI int WINAPI AXU_AddRef(void);
AXUAPI int WINAPI AXU_GetRefCount(void);

//==========================================================================================
// #include "axformat.cpp"
//
// Standard string formatting routines that takes position markers.
//
// These routines are designed to take a format string with embedded position markers in them
// and place in the positions the passed in strings.  For example, with a format like this;
//
// This %0 %1 test.
//
// You could call AxFormatString like this;
//
// AxFormatString( szOutput, "This %0 %1 test.", "is", "a" );
//
// And the result in szOutput would be "This is a test."  The '%0' zero tells the routine to
// insert the first item in the spot.  The '%1' indicates that it should insert the second,
// and so on.  You can order these any way you like, for example "This %1 %0 test.", is valid
// and would return "This a is test."  And you can duplicate entries, for example, using
// a format string like this "This %0 is %1 %1 test." would return "This is a a test."
//
// To add a percentile sign, use '%%'.
//
// To use the instance handle versions pass the instance handle of your application or DLL then
// call MAKEINTRESOURCE() with the ID of the string as the argument.  For example;
// 
// AxFormatString( hInstance,
//                 szOutput,
//                 MAKEINTRESOURCE( IDS_FMT_STRING ),
//                 MAKEINTRESOURCE( IDS_IS_STRING ),
//                 MAKEINTRESOURCE( IDS_A_STRING ) );
//
// Will work if your string table has definitions like this:
//
// IDS_FMT_STRING "This is %0 %1 test."
// IDS_IS_STRING "is"
// IDS_A_STRING "a"
//
// There are six routines;
//
// AxFormatString - The most simple routine, this take the output pointer, the format and
//                  the arguments.
// 
// AxFormatStringEx - The same as AxFormatString but it also takes an instance handle and
//                    if any of the strings are made with MAKEINTRESOURCE it will get the
//                    string from the instance handle provided.
//
// AxVFormatString - Takes a va_list, and can be used by a wrapping function.
// 
// AxVFormatStringEx - The same as AxVFormatString but it also takes an instance handle and
//                     if any of the strings are made with MAKEINTRESOURCE it will get the
//                     string from the instance handle provided.
//
// AxAFormatString - Takes an array of up to 10 strings, one for each position index.
// 
// AxAFormatStringEx - The same as AxAFormatString but it also takes an instance handle and
//                     if any of the strings are made with MAKEINTRESOURCE it will get the
//                     string from the instance handle provided.
//
AXUAPI void cdecl  AXU_FormatString( char *pszOutput, LPCSTR pszFormat, ... );
AXUAPI void WINAPI AXU_VFormatString( char *pszOutput, LPCSTR pszFormat, void *pArgs );
AXUAPI void WINAPI AXU_AFormatString( char *pszOutput, LPCSTR pszFormat, char *apszFormatItems[] );
AXUAPI void cdecl  AXU_FormatStringEx( HINSTANCE hInstance, char *pszOutput, LPCSTR pszFormat, ... );
AXUAPI void WINAPI AXU_VFormatStringEx( HINSTANCE hInstance, char *pszOutput, LPCSTR pszFormat, void *pArgs );
AXUAPI void WINAPI AXU_AFormatStringEx( HINSTANCE hInstance, char *pszOutput, LPCSTR pszFormat, char *apszFormatItems[] );

//=============================================================================
// FUNCTION:   AXU_FormatIntegerList
// PURPOSE:    Convert selected items to trace list e.g. 1..5, 7, 9..11
// PARAMETERS:
//    pnValueList       -- points to an array of integers.
//    nEntries          -- Length of list pointed to by pnValueList
//    nOffset           -- Offset added to the list entries prior to formatting (typically 0 or 1).
//    pszFormattedList  -- Pointer to a buffer to receive the formatted list.
//    nListSize         -- Length of buffer pointed to by pszFormattedList
// RETURNS:
//    TRUE  if the list fits in the buffer.
//    FALSE if the formatted list is clipped by the buffer size.
//
AXUAPI BOOL WINAPI AXU_FormatIntegerList( const int *pnValueList, int nEntries, int nOffset, 
                                          LPSTR pszFormattedList, int nListSize);

//==========================================================================================
// #include "drawutils.cpp"

//===============================================================================================
// MACRO:   AXU_DRAWFASTRECT
// PURPOSE: Fast drawing of solid color rectangles.
// NOTES:
//    ExtTextOut() is the fastest way to draw a filled rectangle in Windows 
//    (if you don't want dithered colors or borders).
//
//    The following macro (DRAWFASTRECT) draws a filled rectangle with no border and a 
//    solid color.  It uses the current background color as the fill color.
//
#define AXU_DRAWFASTRECT(hdc,lprc) ExtTextOut(hdc,0,0,ETO_OPAQUE,lprc,NULL,0,NULL)

// Draws a solid rectangle in a specified color.
AXUAPI void WINAPI AXU_Rectangle(HDC hDC, const RECT *pRect, COLORREF rgb);

// Fills in a region with a solid specified color.
AXUAPI void WINAPI AXU_FillRgn(HDC hDC, HRGN hrgn, COLORREF rgb);

// Draws a polygon in a solid specified color.
AXUAPI void WINAPI AXU_Polygon(HDC hDC, POINT apt[], UINT uPoints, COLORREF rgb);

// Draws a hollow rectangle with the given pen.
AXUAPI void WINAPI AXU_HollowRectangle(HDC hDC, const RECT *prRect, HPEN hpen);

// PatBlts a rectangle filled with the given brush, which should 
// have been created with AXU_CreateDragBrush().
AXUAPI void WINAPI AXU_DrawDragRectangle(HDC hDC, const RECT *prRect, HBRUSH hbrush);

//==========================================================================================
// #include "AbbrevPath.cpp"

// A path is abbreviated based on the length of the buffer to hold it.
AXUAPI UINT WINAPI AXU_AbbreviatePath(LPCSTR pszPath, LPSTR pszAbbrevPath, UINT uMaxChars);

// A path is abbreviated if necessary based on whether it fits in a given number of pixels.
AXUAPI UINT WINAPI AXU_AbbreviatePathDC(HDC hDC, LPCSTR pszPath, LPSTR pszAbbrevPath, 
                                        UINT uMaxChars, int nMaxWidth);

//==========================================================================================
// #include "ResourceUtils.cpp"
//

// Find the index of a given resource in an EXE file.
AXUAPI int WINAPI AXU_GetResourceIndex(HINSTANCE hInstance, LPCTSTR pszResourceType, UINT uResourceId);

//==========================================================================================
// #include "TimeUtils.cpp"
//

// Converts time in hours,minutes,seconds to seconds.
AXUAPI double WINAPI AXU_HMStoSeconds( UINT uHours, UINT uMinutes, double dSeconds );

// Converts seconds to hours, minutes and seconds.
AXUAPI void WINAPI AXU_SecondsToHMS( double dTimeInSeconds, UINT *puHours, UINT *puMinutes, double *pdSeconds);

// Formats the time in HH:MM:SS.
#define AXU_MAXHMSLENGTH  9  // maximum length of an HH:MM:SS string.
AXUAPI int WINAPI AXU_FormatHMS( UINT uSeconds, char *pszBuffer, UINT uMaxLen );

// Formats the time in HHH:MM:SS.
AXUAPI int WINAPI AXU_FormatHHHMMSS( UINT uSeconds, char *pszBuffer, UINT uMaxLen );

// Formats the time in HHHH:MM:SS.ss
#define AXU_MAXHHHHMMSSssLENGTH  32  // maximum length of an HHHH:MM:SS.ss string.
AXUAPI int WINAPI AXU_FormatHHHHMMSSss( double dTimeInSeconds, int nDecimalPlaces, char *pszBuffer, UINT uMaxLen );

// Parses a time text string into seconds.
AXUAPI double WINAPI AXU_ParseTime( LPCSTR pszText );

// Turn the creation date of a file into two longs (as used by ABF).
AXUAPI BOOL WINAPI AXU_GetFileDateTime( HANDLE hFile, long *plDate, long *plTime );

// Formats the date from a packed integer (yymmss) according to the locale setting.
#define AXU_MAXYMDLENGTH  11  // maximum length of a DD:MM:YYYY string.
AXUAPI void WINAPI AXU_FormatDate( int nDate, char *pszDate, UINT uMaxLen );

//==========================================================================================
// #include "DebugBreak.cpp"
//

// Checks whether the app is currently being debugged.
AXUAPI BOOL WINAPI AXU_IsDebuggerPresent(void);

// Forces a debugger break by loading the registered JIT debugger and then executing a debug breakpoint.
AXUAPI void WINAPI AXU_ForceDebugBreak(void);

//==========================================================================================
// #include "Process.cpp"
//
typedef BOOL (CALLBACK *AXU_PROCENUMPROC)( DWORD dwPID, LPSTR pszFilename, LPARAM lParam );
AXUAPI BOOL WINAPI AXU_EnumProcs( AXU_PROCENUMPROC lpProc, LPARAM lParam );

//==========================================================================================
// #include "EnumComputers.cpp"
//
struct AXU_ComputerInfo
{
   DWORD  dwMajorVersion;
   DWORD  dwMinorVersion;
   DWORD  dwType;
   bool   bIsWin9x;
   bool   bResponding;
};

BOOL WINAPI AXU_GetComputerInfo(LPCSTR pszName, AXU_ComputerInfo *pComputerInfo);

//==========================================================================================
// #include "WinSock2.cpp"
//
AXUAPI BOOL WINAPI AXU_PingTarget(LPCSTR pszName, UINT uTimeout, bool *pbTimedout);

//==========================================================================================
// #include "CPUID.cpp"
//
// CPU Vendors                              // Known Vendor Strings                
const UINT AXU_CPU_VENDOR_UNKNOWN   = 0;    // ====================                
const UINT AXU_CPU_VENDOR_INTEL     = 1;    // "GenuineIntel"      Intel           
const UINT AXU_CPU_VENDOR_TRANSMETA = 2;    // "GeniuneTMx86"      Transmeta       
const UINT AXU_CPU_VENDOR_UMC       = 3;    // "UMC UMC UMC "      UMC             
const UINT AXU_CPU_VENDOR_AMD       = 4;    // "AuthenticAMD"      AMD        
const UINT AXU_CPU_VENDOR_CYRIX     = 5;    // "CyrixInstead"      Cyrix / IBM     
const UINT AXU_CPU_VENDOR_NEXGEN    = 6;    // "NexGenDriven"      NexGen          
const UINT AXU_CPU_VENDOR_IDT       = 7;    // "CentaurHauls"      IDT             
const UINT AXU_CPU_VENDOR_RISE      = 8;    // "RiseRiseRise"      Rise Technology 

// CPU Families
const UINT AXU_CPU_FAMILY_386 = 3;  // 80386s
const UINT AXU_CPU_FAMILY_486 = 4;  // most 80486s, AMD 5x86, Cyrix 5x86
const UINT AXU_CPU_FAMILY_586 = 5;  // Intel P5, P54C, P55C, P24T
                                    // NexGen Nx586, Cyrix M1, AMD K5, K6
                                    // Centaur C6, C2, C3, Rise mP6
const UINT AXU_CPU_FAMILY_PENTIUM = AXU_CPU_FAMILY_586;

const UINT AXU_CPU_FAMILY_686 = 6;  // Intel P6, P2
                                    // AMD K7, Cyrix M2, VIA Cyrix III
const UINT AXU_CPU_FAMILY_PENTIUMII  = AXU_CPU_FAMILY_686;

const UINT AXU_CPU_FAMILY_786 = 7;  // Intel P3, Cyrix III
const UINT AXU_CPU_FAMILY_PENTIUMIII = AXU_CPU_FAMILY_786;

const UINT AXU_CPU_FAMILY_PENTIUM4 = 0x0F;  // Intel P4
                                                 
struct AXU_CPUINFO
{
   char     szVendor[16];
   char     szFullName[64];

   UINT     uVendor;
   UINT     uFamily;
   UINT     uModel;
   UINT     uStepping;
   UINT     uBrandID;
   UINT     uFeatureFlags;
   UINT     uFrequency;
   UINT     uCPUCount;
   LONGLONG uSerialNumber;

   AXU_CPUINFO() { memset(this, 0,  sizeof(*this)); }
};

AXUAPI LPCSTR WINAPI AXU_GetCPUVendor(UINT uVendor);
AXUAPI LPCSTR WINAPI AXU_GetCPUFamily(UINT uFamily);
AXUAPI void   WINAPI AXU_GetCPUInfo(AXU_CPUINFO *pInfo);

//==========================================================================================
// #include "MultiMon.cpp"
//
// If we are building with Win95/NT4 headers, we need to declare
// the multimonitor-related metrics and APIs ourselves.
//
#ifndef SM_CMONITORS

#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#define SM_CMONITORS            80
#define SM_SAMEDISPLAYFORMAT    81

// HMONITOR is already declared if _WIN32_WINNT >= 0x0500 in windef.h
// This is for components built with an older version number.
#if !defined(HMONITOR_DECLARED) && (_WIN32_WINNT < 0x0500)
DECLARE_HANDLE(HMONITOR);
#define HMONITOR_DECLARED
#endif

#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002

#define MONITORINFOF_PRIMARY        0x00000001

typedef struct tagMONITORINFO
{
    DWORD   cbSize;
    RECT    rcMonitor;
    RECT    rcWork;
    DWORD   dwFlags;
} MONITORINFO, *LPMONITORINFO;

#ifndef CCHDEVICENAME
#define CCHDEVICENAME 32
#endif

typedef struct tagMONITORINFOEX : public tagMONITORINFO
{
    TCHAR       szDevice[CCHDEVICENAME];
} MONITORINFOEX, *LPMONITORINFOEX;

typedef BOOL (CALLBACK* MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);

#endif  // SM_CMONITORS

int      WINAPI AXU_GetSystemMetrics(int nIndex);
HMONITOR WINAPI AXU_MonitorFromWindow(HWND hWnd, DWORD dwFlags);
HMONITOR WINAPI AXU_MonitorFromRect(LPCRECT pr, DWORD dwFlags);
HMONITOR WINAPI AXU_MonitorFromPoint(POINT pt, DWORD dwFlags);
BOOL     WINAPI AXU_GetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO pMonitorInfo);
BOOL     WINAPI AXU_EnumDisplayMonitors(HDC hDC, LPCRECT pr, MONITORENUMPROC pProc, LPARAM lParam);

//##########################################################################################
//##########################################################################################
//###
//###   Inline functions.
//###
//##########################################################################################
//##########################################################################################

const char AXU_APP_OPERATION_MODE[] = "AxonAppOperationMode";
//==========================================================================================
// FUNCTION: AXU_SetDemoMode
// PURPOSE:  Sets the internal flag that says whether the app is demo or real.
//
inline void AXU_SetDemoMode(BOOL bDemo)
{
   // This function should only be called once, at app startup.
#ifdef _DEBUG
   char szBuf[32];
   ASSERT(GetEnvironmentVariable(AXU_APP_OPERATION_MODE, szBuf, ELEMENTS_IN(szBuf))==0);
#endif

   SetEnvironmentVariable(AXU_APP_OPERATION_MODE, bDemo ? "Demo" : "Real");
}

//==========================================================================================
// FUNCTION: AXU_IsDemoMode
// PURPOSE:  Gets the internal flag that says whether the app is demo or real.
//
inline BOOL AXU_IsDemoMode()
{
   // Default to "Real" mode if AXU_SetDemoMode() has not been called.
   char szBuf[32];
   if (GetEnvironmentVariable(AXU_APP_OPERATION_MODE, szBuf, ELEMENTS_IN(szBuf))==0)
      return FALSE;

   // Test the environment variable's contents.
   return strcmp(szBuf, "Demo")==0;
}

//==========================================================================================
// FUNCTION: AXU_IsAxonKeyDLLInstalled
// PURPOSE:  Checks whether the DLL is present that indicates that this was a bought product.
//
inline BOOL AXU_IsAxonKeyDLLInstalled(void)
{
   return !AXU_IsDemoMode();

//   // Previously a key DLL was used...
//   const char c_szAxonKeyDLL[] = "MSQTVRDC32.DLL";
//   HINSTANCE hDLLInstance = LoadLibrary( c_szAxonKeyDLL );
//   if (hDLLInstance==NULL)
//      return FALSE;
//
//   FreeLibrary( hDLLInstance );
//   return TRUE;
}

#pragma pack(pop)

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // INC_AXOUTILS32_H
