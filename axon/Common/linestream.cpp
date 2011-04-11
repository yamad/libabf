//***************************************************************************
//*                                                                         *
//*  Copyright (C) 1996 AXON Instruments Inc.                               *
//*                                                                         *
//*  This program is copyrighted and all modifications to this program      *
//*  retain the original copyright. All rights reserved.                    *
//*                                                                         *
//***************************************************************************
//
// HEADER:       LineStream.cpp
// PURPOSE:      The LineStream object.

#include "wincpp.hpp"

#include "LineStream.hpp"

#define  CR 13
#define  LF 10
#define  SP 32

//===========================================================================
// FUNCTION:  LineStream( char* pszFileName, BOOL bStripWS )
// PURPOSE:   Construct the LineStream object.
//

LineStream::LineStream( char* pszFileName, int nEOL, BOOL bStripWS )
:  m_hFile( INVALID_HANDLE_VALUE ),
   m_dwError( 0 ),
   m_eError( LS_NO_ERROR ),
   m_pc( m_sBuffer ),
   m_pe( m_sBuffer ),
   m_nEOL( nEOL ),
   m_bStripWS( bStripWS ),
   m_bEOF( FALSE )
{
   MEMBERASSERT();
   LPSZASSERT(pszFileName);

   // open the file
   m_hFile = CreateFile(pszFileName,      // pointer to name of the file 
                        GENERIC_READ,     // access (read-write) mode 
                        FILE_SHARE_READ,  // share mode 
                        NULL,             // pointer to security descriptor 
                        OPEN_EXISTING,    // how to create 
                        0,                // file attributes 
                        NULL);            // handle to file with attributes to copy
   if( INVALID_HANDLE_VALUE == m_hFile )
   {
      SetIO_Error( ::GetLastError() );
      return;
   }

   // do the initial fill ( will set error )
   Refill();
}

//===========================================================================
// FUNCTION: ~LineStream()
// PURPOSE:   Destruct the LineStream object.
//

LineStream::~LineStream()
{
   MEMBERASSERT();

   // close the file
   CloseFile();
}

//===========================================================================
// FUNCTION:  CloseFile()
// PURPOSE:   Close the open file.
//

void LineStream::CloseFile()
{
   MEMBERASSERT();

   // close the file
   if( INVALID_HANDLE_VALUE != m_hFile )
      CloseHandle( m_hFile );
   m_hFile = INVALID_HANDLE_VALUE;
}

//===========================================================================
// FUNCTION:  SetIO_Error( DWORD dwError )
// PURPOSE:   exit because of an i/o error
//

BOOL LineStream::SetIO_Error( DWORD dwError )
{
   MEMBERASSERT();

   m_dwError = dwError;
   m_eError  = LS_IO_ERROR;
   return FALSE;
}

//===========================================================================
// FUNCTION:  SetEOL_Error(void)
// PURPOSE:   exit because we can't find the eol marker in the buffersize
//

BOOL LineStream::SetEOL_Error()
{
   MEMBERASSERT();

   m_dwError = 0;
   m_eError  = LS_EOL_ERROR;
   return FALSE;
}

//===========================================================================
// FUNCTION:  ScanForEOL(void)
// PURPOSE:   Refill the buffer.
//

char* LineStream::ScanForEOL()
{
   MEMBERASSERT();

   return (char*)memchr( m_pc, m_nEOL, m_pe - m_pc );
}

//===========================================================================
// FUNCTION:  Refill(void)
// PURPOSE:   Refill the buffer.
//

BOOL LineStream::Refill()
{
   MEMBERASSERT();

   // if there is no file, return FALSE
   if( INVALID_HANDLE_VALUE == m_hFile )
      return FALSE;

   // if we're at EOF, return FALSE
   if( m_bEOF )
      return FALSE;

   // if there is unread chars in the buffer, return TRUE
   if( m_pc < m_pe )
      return TRUE;

   // read in the data
   DWORD dwRead;
   if( !ReadFile( m_hFile, m_sBuffer, LS_BUF_SIZE, &dwRead, NULL ) )
      return SetIO_Error( ::GetLastError() );

   // check for EOF
   if( 0 == dwRead )
   {
      m_bEOF = TRUE;
      return FALSE;
   }

   // setup the pointers
   m_pc = m_sBuffer;
   m_pe = &m_sBuffer[ dwRead ];

   return TRUE;
}

//===========================================================================
// FUNCTION:  Reset(void)
// PURPOSE:   reset the stream.
//

BOOL LineStream::Reset()
{
   MEMBERASSERT();

   // reset the state variables
   m_dwError = 0;
   m_eError  = LS_NO_ERROR;
   m_pc      = m_pe = m_sBuffer;
   m_bEOF    = FALSE;

   // see if the handle is valid
   if( INVALID_HANDLE_VALUE == m_hFile )
      return FALSE;

   // seek to the beginning of the file
   if( SetFilePointer( m_hFile, 0, NULL, FILE_BEGIN ) != 0 )
      return SetIO_Error( ::GetLastError() );

   // fill the buffer
   return Refill();
}

//===========================================================================
// FUNCTION:  NextLine(void)
// PURPOSE:   Get the next line.
//

BOOL LineStream::GetNextLine( char* pszBuffer, DWORD dwSize )
{
   MEMBERASSERT();
   WPTRASSERT( pszBuffer );
   ASSERT( dwSize > 1 );

   // see if we have had an error
   if( LS_NO_ERROR != m_eError )
      return FALSE;

   // see if we're at EOF
   if( m_bEOF )
      return FALSE;

   // the destination pointer
   char* pDst = pszBuffer;

   // leave room fro the trailing \0
   dwSize--;

   // fill the buffer
   while( dwSize )
   {
      // see if we need to refill the buffer
      if( m_pc >= m_pe )
      {
         if( ! Refill() )
            return FALSE;
      }
      // look for EOL
      char* pEOL = ScanForEOL();

         
      // get the number of bytes in the buffer
      size_t n = ((NULL == pEOL) ? m_pe : pEOL) - m_pc;
//      ASSERT( n > 0 );

      // see if we have exceeded the buffer size
      if( n >= dwSize )
         return SetEOL_Error();

      // move into the buffer
      memcpy( pDst, m_pc, n );

      // bump the pointers and count
      pDst   += n;
      m_pc   += n;
      dwSize -= n;

         // see if we're at EOL
      if( NULL != pEOL )
      {
         // go past the EOL marker
         m_pc++;

         // strip trailing WS?
         if( m_bStripWS )
         {
            char* pWS = pDst;

            while( --pWS >= pszBuffer
            && *pWS <= SP )
               ;
            // pWS now points to first non-ws char or pszBuffer[-1]
            // set pDst to where \0 should go.
            pDst = pWS+1;
         }

         // leave loop on EOL
         break;
      }
   }

   // terminate the line
   *pDst = 0;

   return TRUE;
}

//===========================================================================
// FUNCTION:  GetLastError( DWORD* pdwError )
// PURPOSE:   Get the last error.
//

LS_ERROR_E LineStream::GetLastError( DWORD* pdwError )
{
   MEMBERASSERT();

   // see if we have had an error
   if( LS_NO_ERROR != m_eError )
   {
      // see if we were passed a pointer
      if( NULL != pdwError )
      {
         WPTRASSERT( pdwError );
         *pdwError = m_dwError;
      }
   }

   // otherwise return the error code
   return m_eError;
}
