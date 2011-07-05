//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  WindowPlacement.CPP
// PURPOSE: Reads and writes WINDOWPLACEMENT to/from the registry. 
// AUTHOR:  BHI  Aug 1998
//
#ifndef  INC_WINDOWPLACEMENT_HPP
#define  INC_WINDOWPLACEMENT_HPP

#pragma once

struct CWindowPlacement : private tagWINDOWPLACEMENT
{
public:
	CWindowPlacement();
	~CWindowPlacement();

	// Save/restore window pos (from app profile)
	BOOL     Restore(HWND hWnd, LPCSTR pszSection, LPCSTR pszRegKey);
	void     Save(HWND hWnd, LPCSTR pszSection, LPCSTR pszRegKey);

   void     SetShow(UINT uShowCmd, LPCSTR pszSection, LPCSTR pszRegKey);

private:	
	// Read/write to app profile
	BOOL     LoadFromRegistry(LPCSTR pszSection, LPCSTR pszRegKey);
	void     SaveToRegistry(LPCSTR pszSection, LPCSTR pszRegKey);
};

#endif   // INC_WINDOWPLACEMENT_HPP
