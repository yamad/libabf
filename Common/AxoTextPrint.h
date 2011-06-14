//****************************************************************************
//
//    Copyright (c) 1996-1997 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************
// MODULE:  AxoTextPrint.h
// PURPOSE: This file contains the prototypes for the text printing abstract class
// AUTHOR:  JL  July 1997
//

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@                                                                        @@
//@@ Axon Generic Application Framework Component                           @@
//@@                                                                        @@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#ifndef INC_AXOTEXTPRINT_H
#define INC_AXOTEXTPRINT_H

//============================================================================
// CLASS:         CAxoTextPrint
// PURPOSE:       That abstract base class that encapsulates the general
//                purpose text printing fuctionality.
//
class CAxoTextPrint
{
protected:
                     // Window handle of the owner.
   HWND              m_hOwnerWnd;

                     // Maximal number of pages
   UINT              m_uMaxPage;

                     // Left page margin in inches
   float             m_fMarginLeft;

                     // Top page margin in inches
   float             m_fMarginTop;

                     // Right page margin in inches
   float             m_fMarginRight;

                     // Bottom page margin in inches
   float             m_fMarginBottom;

                     // Flag that displays the 'Print' dialog box when start printing
   BOOL              m_bShowPrintDialog;

   enum              { MAX_DOC_NAME = 255 };

                     // A null-terminated string that specifies the name of the document
   char              m_szDocName[ MAX_DOC_NAME ];

                     // default based on app name
   LPCTSTR           m_pszProfileName;
private:
   // Prevent the use of the default constructor.
   // Use public version only for instantiation.
   CAxoTextPrint( void );

   // Prevent the use of the copy constructor and assignement operator.
   CAxoTextPrint( const CAxoTextPrint & );
   const CAxoTextPrint & operator=( const CAxoTextPrint & );
   
public:
   CAxoTextPrint( HWND hOwnerWnd, BOOL bShowPrintDialog, LPCSTR lpszDocName, LPCTSTR pszProfileName );
   virtual BOOL      DoPrint( void );

   // Overridables
protected:
   virtual BOOL      OnPrintPage( HDC hDC, UINT uPageNumber, RECT rectPage ) = 0;
   virtual void      SetPrintDC( HDC hDC );
   virtual void      GetDefaults( HDC hDC, RECT &rectPage, RECT &rectMargins );
   virtual void      CreateDeviceHandles( HANDLE *phDevNames, HANDLE *phDevMode );
};

#endif // INC_AXOTEXTPRINT_H
