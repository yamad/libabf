//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  DACFile.HPP
// PURPOSE: Contains class definition for CDACFile, a temp file for DAC file data.
// AUTHOR:  BHI  May 1994
//

#ifndef INC_CDACFILE_HPP
#define INC_CDACFILE_HPP

#include "..\Common\FileIO.hpp"
//-----------------------------------------------------------------------------------------------
// CDACFile class definition

class CDACFile
{
private:    // Member variables.
   char     m_szFileName[_MAX_PATH];     // Filename for array virtualization
   CFileIO  m_File;                      // Handle to temporary file.

   UINT     m_uSweepCount;               // Total count of entries in the DACFile
   UINT     m_uSweepLength;              // Length of all sweeps

private:    // Declare but don't define copy constructor to prevent use of default
   CDACFile(const CDACFile &CS);
   const CDACFile &operator=(const CDACFile &CS);

private:    // Private member functions.
   void _Initialize();
   
public:     // Public member functions
   CDACFile();
   ~CDACFile();

   BOOL  OpenFile();
   void  CloseFile();

   BOOL  PutSweep( UINT uSweep, const DAC_VALUE *pnData, UINT uLength );
   BOOL  GetSweep( UINT uSweep, DAC_VALUE *pnData, UINT uMaxLength );
   UINT  GetCount() const;
   
   BOOL  Write( HANDLE hDataFile );
};


//===============================================================================================
// PROCEDURE: GetCount
// PURPOSE:   Returns the current count of synch elements.
//
inline UINT CDACFile::GetCount() const
{
   MEMBERASSERT();
   return m_uSweepCount;
}

#endif      // INC_CDACFILE_HPP
