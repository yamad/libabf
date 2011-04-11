//***************************************************************************
//*                                                                         *
//*  Copyright (C) 1994 - 1997 AXON Instruments Inc.                        *
//*                                                                         *
//*  This program is copyrighted and all modifications to this program      *
//*  retain the original copyright. All rights reserved.                    *
//*                                                                         *
//***************************************************************************
//
// HEADER:       AXOP.H
// PURPOSE:      Definition of constants types and prototypes for use in
//               interfacing to AXOP DLLs.
#ifndef __AXOP_H__
#define __AXOP_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// Definition of constants and types
//

// The version number of the current AXOP API specification.
#define AXOP_APIVERSION          0x01030000
#define AXOP_APIVERSION_STR      "1.3.0.00"

// The typedefs below are for Filter AXOPS only

struct AXOP_FILTER_CONVTOOWN_OPTIONS
{
   BOOL           bIncludeHeader;
};

// Filter Confidence enum

enum AXOP_eConfidence
{
   AXOP_NO_CONFIDENCE,                 // The filter cannot open the file
   AXOP_MAYBE_CONFIDENCE,              // The filter may be able to open the file
   // 970408 ajd
   // Temporary hack to force the text axop to be selected for general text files.
   // This will be removed once the selection dialog is in place.
   AXOP_MAYBE_TEXT_CONFIDENCE,         // The filter may be able to open the file
   AXOP_YES_CONFIDENCE                 // The filter can open the file
};

//
// Definition of functions for API layer 
//
// General axop functions

UINT WINAPI EXPORT AXOP_GetExeFunctionCount(void);
BOOL WINAPI EXPORT AXOP_GetFunctionName( UINT uFunction, char  *pszFnName, UINT uMaxName, DWORD  * pdwType );
BOOL WINAPI EXPORT AXOP_GetUIUpdateFnName( UINT uFunction, char  *pszFnName, UINT uMaxName );
BOOL WINAPI EXPORT AXOP_GetMenuEntry( UINT uFunction, int  *pnMenuName, int  *pnItemName, 
                                          int  *pnDescription, int  *pnToolTip);

// AXOP_Get_Version
        DWORD  WINAPI EXPORT AXOP_GetVersion(void);
typedef DWORD (WINAPI *AXOP_GETVERSIONFNPTR)(void);

// AXOP_GetType
        DWORD  WINAPI EXPORT  AXOP_GetType(void);
typedef DWORD (WINAPI       *AXOP_GETTYPE)(void);


// ConvertFromOwnToABF
#define sz_FILTER_CONVERTTOABF "FILTER_ConvertFromOwnToABF"

                   BOOL  WINAPI FILTER_ConvertFromOwnToABF( HWND hWnd, const char *pszFileName, char *pszNewName, UINT uNewNameSize, char *pszDescription, LONG lMaxDescriptionSize, int *pnError);

typedef            BOOL (WINAPI *AXOP_FILTER_CONVERTTOABF)( HWND hWnd, const char *pszFileName, char *pszNewName, UINT uNewNameSize, char *pszDescription, LONG lMaxDescriptionSize, int *pnError); 


// ConvertFromABFToOwn
#define sz_FILTER_CONVERTFROMABF "FILTER_ConvertFromABFToOwn"

                   BOOL  WINAPI   FILTER_ConvertFromABFToOwn( HWND hWnd, const char *pszFileName, char *pszNewName, UINT uNewNameSize, char *pszTypeString, AXOP_FILTER_CONVTOOWN_OPTIONS *pOptions, int *pnError );

typedef            BOOL (WINAPI *AXOP_FILTER_CONVERTFROMABF)( HWND hWnd, const char *pszFileName, char *pszNewName, UINT uNewNameSize, char *pszTypeString, AXOP_FILTER_CONVTOOWN_OPTIONS *pOptions, int *pnError );


// IsFileOpenable
#define sz_FILTER_ISFILEOPENABLE "FILTER_IsFileOpenable"

              BOOL  WINAPI        FILTER_IsFileOpenable( const char  *pszFileName, BOOL *pnCorrectType, int *pnError );

typedef       BOOL (WINAPI *AXOP_FILTER_ISFILEOPENABLE)( const char  *pszFileName, BOOL *pnCorrectType, int *pnError );


// QueryNumExportedFileTypes
#define sz_FILTER_QUERYNUMTYPES "FILTER_QueryNumExportedFileTypes"

                    BOOL  WINAPI FILTER_QueryNumExportedFileTypes( int *pnNumberTypes );

typedef             BOOL (WINAPI  *AXOP_FILTER_QUERYNUMFILETYPES)( int *pnNumberTypes );


// QueryFileType
#define sz_FILTER_QUERYTYPE "FILTER_QueryFileType"

         BOOL  WINAPI        FILTER_QueryFileType( int nTypeIndex, char  *pszTypeString, int nMaxLenType, char *pszFileNameWildCard, int nMaxLenWild );

typedef  BOOL (WINAPI *AXOP_FILTER_QUERYFILETYPE)( int nTypeIndex, char  *pszTypeString, int nMaxLenType, char *pszFileNameWildCard, int nMaxLenWild );


// GetFileOpenDialogStrings
#define sz_FILTER_GetFileOpenDialogStrings   "FILTER_GetFileOpenDialogStrings"

         BOOL  WINAPI        FILTER_GetFileOpenDialogStrings ( int nTypeIndex, char  *szDescription, int nDescriptionLength, char *szWildcard, int nWildcardLength );

typedef  BOOL (WINAPI * AXOP_FILTER_GetFileOpenDialogStrings)( int nTypeIndex, char  *szDescription, int nDescriptionLength, char *szWildcard, int nWildcardLength );


// GetFileSaveAsDialogStrings
#define sz_FILTER_GetFileSaveAsDialogStrings "FILTER_GetFileSaveAsDialogStrings"

         BOOL  WINAPI        FILTER_GetFileSaveAsDialogStrings ( int nTypeIndex, char  *szDescription, int nDescriptionLength, char *szWildcard, int nWildcardLength );

typedef  BOOL (WINAPI * AXOP_FILTER_GetFileSaveAsDialogStrings)( int nTypeIndex, char  *szDescription, int nDescriptionLength, char *szWildcard, int nWildcardLength );


// GetFileRegistrationStrings
#define sz_FILTER_GetFileRegistrationStrings "FILTER_GetFileRegistrationStrings"

         BOOL  WINAPI        FILTER_GetFileRegistrationStrings ( int nTypeIndex, char  *szDescription, int nDescriptionLength, char *szWildcard, int nWildcardLength );

typedef  BOOL (WINAPI * AXOP_FILTER_GetFileRegistrationStrings)( int nTypeIndex, char  *szDescription, int nDescriptionLength, char *szWildcard, int nWildcardLength );


// BuildErrorText
#define sz_FILTER_BUILDERRORTEXT "FILTER_BuildErrorText"

              BOOL  WINAPI        FILTER_BuildErrorText( int nError, const char *pszFileName, char *pszErrorText, int nMaxErrorLen );

typedef       BOOL (WINAPI *AXOP_FILTER_BUILDERRORTEXT)( int nError, const char *pszFileName, char *pszErrorText, int nMaxErrorLen );


// GetFileOpenConfidence
#define sz_FILTER_GETFILEOPENCONFIDENCE "FILTER_GetFileOpenConfidence"

BOOL WINAPI                  FILTER_GetFileOpenConfidence( const char *pszFileName,
                                                           AXOP_eConfidence *peConfidence,
                                                           int *pnError );

typedef BOOL  (WINAPI *AXOP_FILTER_GETFILEOPENCONFIDENCE)( const char  *pszFileName,
                                                           AXOP_eConfidence *peConfidence,
                                                           int *pnError );

// GetFilterName
#define sz_FILTER_GETFILTERNAME "FILTER_GetFilterName" 

             BOOL  WINAPI        FILTER_GetFilterName( char *pszName, int nMaxNameSize, int *pnError );

typedef      BOOL (WINAPI *AXOP_FILTER_GETFILTERNAME)( char *pszName, int nMaxNameSize, int *pnError );


// GetFilterDescription
#define sz_FILTER_GETFILTERDESCRIPTION "FILTER_GetFilterDescription"

                    BOOL  WINAPI        FILTER_GetFilterDescription( char *pszDescription, int nMaxDescriptionSize, int *pnError );

typedef             BOOL (WINAPI *AXOP_FILTER_GETFILTERDESCRIPTION)( char *pszDescription, int nMaxDescriptionSize, int *pnError );


// Type of AXOPS supported (these values should be able to be OR'ed)
#define AXOP_TYPE_ABFFILTERFUNCTION    2
#define AXOP_TYPE_ABFIMAGEFUNCTION     4

// error internal to the AXOP, AXOP error is added to this value
#define AXOP_ERR_AXOPERROR             100      

// no error - user cancelled operation - do not display error dialog
#define AXOP_ERR_CANCEL                0        

#ifdef __cplusplus
}
#endif

#endif    // __AXOP_H__
