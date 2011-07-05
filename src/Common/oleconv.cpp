// This is a part of the Active Template Library.
// Copyright (C) 1996-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#include "wincpp.hpp"
#include "oleconv.h"

#ifndef _ATL_NO_CONVERSIONS
/////////////////////////////////////////////////////////////////////////////
// Global UNICODE<>ANSI translation helpers
LPWSTR WINAPI AtlA2WHelper(LPWSTR lpw, LPCSTR lpa, int nChars)
{
	ARRAYASSERT(lpa,  nChars);
	WPTRASSERT(lpw);
	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	lpw[0] = '\0';
	MultiByteToWideChar(CP_ACP, 0, lpa, -1, lpw, nChars);
	return lpw;
}

LPSTR WINAPI AtlW2AHelper(LPSTR lpa, LPCWSTR lpw, int nChars)
{
	ARRAYASSERT(lpw, nChars);
	WPTRASSERT(lpa);
	// verify that no illegal character present
	// since lpa was allocated based on the size of lpw
	// don't worry about the number of chars
	lpa[0] = '\0';
	WideCharToMultiByte(CP_ACP, 0, lpw, -1, lpa, nChars, NULL, NULL);
	return lpa;
}

#endif //!_ATL_NO_CONVERSIONS
