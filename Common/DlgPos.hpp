//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  DlgPos.HPP
// PURPOSE: Saves and restores a dialog position on screen
// AUTHOR:  RHW  Jun 1998
//

#ifndef INC_DlgPos_HPP
#define INC_DlgPos_HPP

class CDlgPos
{
private:   // Attributes

private:   // Private functions

private:   // Unimplemented copy functions.
   CDlgPos(const CDlgPos &);
   const CDlgPos &operator=(const CDlgPos &);

public:    // Public interface
   CDlgPos();
   ~CDlgPos();

   // Restores the window size.
   void RestoreSize( HWND hWnd, const char * pszIniFile, const char * pSectionName );

   // Saves the window size.
   void SaveSize( HWND hWnd, const char * pszIniFile, const char * pSectionName ) const;
};


#endif      // INC_DlgPos_HPP

