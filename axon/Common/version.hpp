//***********************************************************************************************
//
//    Copyright (c) 1994 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  version.hpp
// PURPOSE: Contains class definition and inline functions for the CVersion class.
// AUTHOR:  BHI  Mar 1994
//

#ifndef INC_VERSION_HPP
#define INC_VERSION_HPP
                      
#include <winver.h>
#include "..\common\axodefn.h"

struct VERTRANS;

//***********************************************************************************************
// Class definition
//

class CVersion
{
private:
   LPVOID            m_lpVerBuffer;
   UINT              m_uTrans;
   VERTRANS         *m_atrans;
   VS_FIXEDFILEINFO *m_lpvsffi;

private:
   VOID Initialize();
   CVersion(const CVersion &Ver);
   
public:
   CVersion();
   CVersion(LPCSTR lpszFileName);
   CVersion(HINSTANCE hInstance);
   ~CVersion();
   BOOL  IsValid() const;
   BOOL  ReadVersionInfo(LPCSTR lpszFileName);
   BOOL  GetFileVersion(DWORD *pdwMS, DWORD *pdwLS=NULL) const;
   UINT  GetFileVersion(LPSTR lpsBuffer, UINT uBufSize) const;
   BOOL  GetProductVersion(DWORD *pdwMS, DWORD *pdwLS=NULL) const;
   UINT  GetProductVersion(LPSTR lpsBuffer, UINT uBufSize) const;
   DWORD GetFileFlagsMask() const;
   DWORD GetFileFlags() const;
   DWORD GetFileOS() const;
   DWORD GetFileType() const;
   DWORD GetFileSubtype() const;
   BOOL  GetFileDate(DWORD *pdwMS, DWORD *pdwLS=NULL) const; 

   UINT  GetNumTranslation() const;
   BOOL  GetTranslation(UINT iTrans, VERTRANS *pvt) const;

   UINT  GetString(LPCSTR lpcszKey, VERTRANS FAR &rvt, LPSTR lpszValue, UINT cbValue) const;
   UINT  GetString(LPCSTR lpcszKey, LPSTR lpszValue, UINT cbValue) const;
};

#endif   // INC_VERSION_HPP
