//***************************************************************************
//*                                                                         *
//*  Copyright (C) 1996 AXON Instruments Inc.                               *
//*                                                                         *
//*  This program is copyrighted and all modifications to this program      *
//*  retain the original copyright. All rights reserved.                    *
//*                                                                         *
//***************************************************************************
//
// HEADER:       LineStream.hpp
// PURPOSE:      Declaration of the LineStream object.

#ifndef LINE_STREAM_HPP
#define LINE_STREAM_HPP

#define LS_BUF_SIZE     4096

enum LS_ERROR_E
{
   LS_NO_ERROR,         // no error or EOF
   LS_EOL_ERROR,        // no end-of-line character found in buffersize bytes
   LS_IO_ERROR          // i/o error occured during read
};

class LineStream
{
   // the file handle
   HANDLE      m_hFile;

   // i/o error code (from GetLastError())
   DWORD       m_dwError;

   // the error state
   LS_ERROR_E  m_eError;

   // the file buffer
   char        m_sBuffer[ LS_BUF_SIZE ];

   // pointer to the current input character
   char *      m_pc;

   // pointer to one char past the end of the chars in m_sBuffer
   char *      m_pe;

   // The end-of-line character
   int         m_nEOL;

   // Have we seen EOF
   BOOL        m_bEOF;

   // should we strip leading and trailing white space
   // including trailing \t
   BOOL        m_bStripWS;

   // set an i/o error
   BOOL        SetIO_Error( DWORD dwError );

   // set an eol error
   BOOL        SetEOL_Error( void );

   char*       ScanForEOL( void );

   // Refill the buffer
   BOOL        Refill( void );

public:
               LineStream( char* pszFileName, int nEOL, BOOL bStripWS = FALSE );
              ~LineStream();

   // reset the stream
   BOOL        Reset( void );

   // Get the next line
   BOOL        GetNextLine( char* pszBuffer, DWORD dwSize );

   // return TRUE is at EOF
   BOOL        IsEOF( void ) { return m_bEOF; }

   // close the file
   void        CloseFile( void );

   // Get the last error
   LS_ERROR_E  GetLastError( DWORD * pdwError = NULL );
};

#endif//LINE_STREAM_HPP

