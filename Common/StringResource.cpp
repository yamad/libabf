//***********************************************************************************************
//
//    Copyright (c) 1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  StringResource.CPP
// PURPOSE: Implimentation of the class CStringResource, an auto pointer for resource strings.
// AUTHOR:  BHI Dec 1997
//
#include "wincpp.hpp"
#include "StringResource.hpp"

//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  Create a new object that loads a string resource.
// NOTES:    The definition of this constructor contains default parameters of NULL,0 
//           so that CStringResource objects can be created that do not hold anything.
//
CStringResource::CStringResource(HINSTANCE hInstance, UINT uStringID)
{
   m_pszRval = m_pszString = NULL;
   if ( uStringID != INVALID_STRING_ID )
      LoadString(hInstance, uStringID);
}

//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  This alternate constructor allows the holding of a string pointer or a load of a
//           resource string if the MAKEINTRESOURCE() macro is used on the arguement.
//
CStringResource::CStringResource(HINSTANCE hInstance, LPCSTR pszPseudoPointer)
{
   MEMBERASSERT();

   // Start in a known state.
   m_pszString = NULL;
   m_pszRval   = pszPseudoPointer;

   UINT uStringID = GetStringID(pszPseudoPointer);
   if ( uStringID != INVALID_STRING_ID )
      LoadString(hInstance, uStringID);
}

//================================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleans up the held buffer.
//
CStringResource::~CStringResource()
{
   MEMBERASSERT();
   delete[] m_pszString;
   m_pszString = NULL;
}

//================================================================================================
// FUNCTION: Enlarge
// PURPOSE:  Enlarges the held buffer by a chunk.
//
BOOL CStringResource::Enlarge(UINT &uLength)
{
   const UINT uChunkSize = 256;

   // Cleanup the current allocation.
   delete[] m_pszString;

   // Try to allocate the new length.
   m_pszString = new char[ uLength+uChunkSize ];
   if ( m_pszString == NULL )
   {
      uLength = 0;
      return FALSE;
   }

   uLength += uChunkSize;
   return TRUE;
}

//================================================================================================
// FUNCTION: GetStringID
// PURPOSE:  Returns the string ID if the pointer is a Pseudo-pointer.
//           Returns 0 otherwise.
//
UINT CStringResource::GetStringID(LPCSTR pszPseudoPointer)
{
   // No MEMBERASSERT() -- static function;

   // If the hi-word is not zero, it is not a pseudo-pointer
   if ( HIWORD( pszPseudoPointer ) != 0 )
      return INVALID_STRING_ID;

   return LOWORD( (DWORD)pszPseudoPointer );
}

//================================================================================================
// FUNCTION: LoadString
// PURPOSE:  Attempts to load the string, increasing the buffer length until it is big enough.
//
BOOL CStringResource::LoadString(HINSTANCE hInstance, UINT uStringID)
{
   MEMBERASSERT();
   ASSERT( hInstance != NULL );
   UINT uLength = 0;
   for (;;)
   {
      // Add a chunk to the buffer.
      if (!Enlarge(uLength))
         break;

      // Load the string into our buffer.
      UINT uCopied = ::LoadString( hInstance, uStringID, m_pszString, uLength );
      if (uCopied == 0)
      {
         TRACE1("String Resource %u was not found.\n", uStringID);
         ASSERT(FALSE);

         delete[] m_pszString;
         m_pszString = NULL;
         break;
      }

      // If the LoadString was not buffer-length limited, break the loop.
      if (uCopied < uLength-1)
         break;
   }
   
   //  zero terminate the string just to be sure.
   if (m_pszString)
      m_pszString[uLength-1] = '\0';

   m_pszRval = m_pszString;
   return (m_pszRval!=NULL);
}

//================================================================================================
// FUNCTION: Format
// PURPOSE:  Loads the string and performs a sprintf through it.
//
BOOL CStringResource::Format(HINSTANCE hInstance, UINT uFormatStringID, ...)
{
   MEMBERASSERT();

   // Get a pointer to the arguements.
   va_list args;
   va_start(args, uFormatStringID);

   // Do the formatting.
   BOOL bRval = vFormat(hInstance, uFormatStringID, args);

   // End the va_list
   va_end( args );
   return bRval;
}

//================================================================================================
// FUNCTION: vFormat
// PURPOSE:  Loads the string and performs a vsprintf through it.
//
BOOL CStringResource::vFormat(HINSTANCE hInstance, UINT uFormatStringID, va_list args)
{
   MEMBERASSERT();

   CStringResource szFormat(hInstance, MAKEINTRESOURCE(uFormatStringID));
   if (!szFormat)
      return FALSE;

   UINT uLength = 0;
   for (;;)
   {
      // Add a chunk to the buffer.
      if (!Enlarge(uLength))
         break;

      // Load the string into our buffer.
      int nCopied = _vsnprintf(m_pszString, uLength, szFormat, args);

      // If the LoadString was not buffer-length limited, break the loop.
      if ((nCopied >= 0) && (nCopied < int(uLength-1)))
         break;
   }

   //  zero terminate the string just to be sure.
   if (m_pszString)
      m_pszString[uLength-1] = '\0';

   m_pszRval = m_pszString;
   return (m_pszRval!=NULL);
}

