//***********************************************************************************************
//
//    Copyright (c) 1993-2002 Axon Instruments, Inc.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// MODULE:  DACFile.CPP
// PURPOSE: Contains member function code for the CDACFile class.
// AUTHOR:  BHI  May 1994
// NOTES:
//

#include "wincpp.hpp"
#include "..\AxABFFIO32\DACFile.hpp"
#include "..\AxABFFIO32\abfutil.h"
#include "..\Common\ArrayPtr.hpp"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"     // for AXU_* functions

//===============================================================================================
// PROCEDURE: _Initialize
// PURPOSE:   Internal initialization routine.
//
void CDACFile::_Initialize()
{
   m_szFileName[0] = '\0';                // Filename for array virtualization
   m_uSweepCount   = 0;                   // Total count of entries in the synch array
   m_uSweepLength  = 0;                   // Length of all sweeps
}

//===============================================================================================
// PROCEDURE: CDACFile
// PURPOSE:   Constructor.
//
CDACFile::CDACFile()
{
   MEMBERASSERT();
   _Initialize();
}

//===============================================================================================
// PROCEDURE: ~CDACFile
// PURPOSE:   Destructor. Closes the temporary file and deletes it.
//
CDACFile::~CDACFile()
{
   MEMBERASSERT();
   CloseFile();
}

//===============================================================================================
// PROCEDURE: CreateFile
// PURPOSE:   Gets a unique filename and opens it as a temporary file.
//
BOOL CDACFile::OpenFile()
{
   MEMBERASSERT();
   CloseFile();

   // Get a unique temporary file name.
   AXU_GetTempFileName("DAC", 0, m_szFileName);
   
   // Create the temporary file.
   return m_File.Create(m_szFileName, FALSE, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE);
}

//===============================================================================================
// PROCEDURE: CloseFile
// PURPOSE:   Closes the file if it was opened previously.
//
void CDACFile::CloseFile()
{
   MEMBERASSERT();
   m_File.Close();
   _Initialize();
}

//===============================================================================================
// PROCEDURE: GetSweep
// PURPOSE:   Retrieves a sweep from the virtualized array.
//
BOOL CDACFile::GetSweep( UINT uSweep, DAC_VALUE *pnData, UINT uMaxLength )
{
   MEMBERASSERT();
   ARRAYASSERT(pnData, uMaxLength);
   ASSERT(uSweep < m_uSweepCount);

   m_File.Seek(uSweep * m_uSweepLength * sizeof(DAC_VALUE), FILE_BEGIN);
   return m_File.Read(pnData, uMaxLength * sizeof(DAC_VALUE));
}

//===============================================================================================
// PROCEDURE: PutSweep
// PURPOSE:   Puts a new sweep into the DAC file.
//
BOOL CDACFile::PutSweep( UINT uSweep, const DAC_VALUE *pnData, UINT uLength )
{
   MEMBERASSERT();
   ARRAYASSERT(pnData, uLength);
   ASSERT(uSweep <= m_uSweepCount);

   if (m_uSweepLength == 0)
      m_uSweepLength = uLength;

   VERIFY(m_File.Seek(uSweep * m_uSweepLength * sizeof(DAC_VALUE), FILE_BEGIN));
   if (!m_File.Write(pnData, uLength * sizeof(DAC_VALUE)))
      return FALSE;

   m_uSweepCount++;
   return TRUE;
}

//===============================================================================================
// PROCEDURE: Write
// PURPOSE:   Copies the complete synch array to another file, packing out the file-offset entry.
//
BOOL CDACFile::Write( HANDLE hDataFile )
{
   MEMBERASSERT();
   ASSERT( hDataFile != INVALID_HANDLE_VALUE );
   if (m_uSweepCount == 0)
      return TRUE;

   // Seek to the end of the passed file. This will only fail for invalid file handles.
   CFileIO_NoClose OutFile(hDataFile);
   LONGLONG llCurrentPos = 0;
   if (!OutFile.Seek(0, FILE_END, &llCurrentPos))
      return FALSE;

   // Seek to the start of the temporary file.
   m_File.Seek(0, FILE_BEGIN);

   CArrayPtr<DAC_VALUE> anBuffer(m_uSweepLength);
   if (!anBuffer)
      return FALSE;

   // Read the Synch data in a buffer at a time and write it out to the passed file.
   for (UINT i=0; i<m_uSweepCount; i++)
   {
      // Read in a buffer from the temp file.
      VERIFY(m_File.Read( anBuffer, m_uSweepLength * sizeof(DAC_VALUE)));      

      // Write the packed buffer out to the temp file.
      if ( !OutFile.Write(anBuffer, m_uSweepLength * sizeof(DAC_VALUE)))
      {
         // If an error occurs, go back to the start of the block and truncate the file
         // ready for the next attempt after the user has freed up some disk space.
         OutFile.Seek(llCurrentPos, FILE_BEGIN);
         OutFile.SetEndOfFile();
         return FALSE;
      }
   }
   
   // Seek back to end of the temporary file.
   m_File.Seek(0, FILE_END);
   return TRUE;
}

