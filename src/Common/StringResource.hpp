//***********************************************************************************************
//
//    Copyright (c) 1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  StringResource.HPP
// PURPOSE: Define the class CStringResource an auto pointer for resource strings.
// AUTHOR:  BHI Dec 1997
//
#ifndef INC_STRINGRESOURCE_HPP
#define INC_STRINGRESOURCE_HPP

#include <stdio.h>
#include <stdarg.h>

//***********************************************************************************************
// CLASS:   CStringResource
// PURPOSE: A simple smart pointer for resource strings.
//
class CStringResource
{
private:    // Private data.
   enum { INVALID_STRING_ID = 0 };
   LPSTR  m_pszString;
   LPCSTR m_pszRval;

private:    // Prevent copy constructor and operator=().
   CStringResource(const CStringResource &);
   const CStringResource &operator=(const CStringResource &);

private:    // Implimentation internal functions.
   static UINT GetStringID(LPCSTR pszPseudoPointer);
   BOOL Enlarge(UINT &uLength);

public:     // Public member functions.

   // Constructors & destructor. See notes below.
   CStringResource(HINSTANCE hInstance=NULL, UINT uStringID=0);
   CStringResource(HINSTANCE hInstance, LPCSTR pszPseudoPointer);
   ~CStringResource();

   // Load a string from the given string resource.
   BOOL LoadString(HINSTANCE hInstance, UINT uStringID);

   // Load a string and sprintf into the buffer from it.
   BOOL Format(HINSTANCE hInstance, UINT uFormatStringID, ...);

   // Load a string and sprintf into the buffer from it.
   BOOL vFormat(HINSTANCE hInstance, UINT uFormatStringID, va_list args);

   // Accessor functions to get at the wrapped array.
   operator LPCSTR () const;
   LPCSTR Get() const;
};

//================================================================================================
// FUNCTION: Get
// PURPOSE:  Returns a pointer to the string buffer.
//
inline LPCSTR CStringResource::Get() const
{
   MEMBERASSERT();
   return m_pszRval; 
}

//================================================================================================
// FUNCTION: Overloaded cast operator
// PURPOSE:  Returns a pointer to the string buffer.
//
inline CStringResource::operator LPCSTR() const
{
   MEMBERASSERT();
   return Get();
}


#endif          // INC_STRINGRESOURCE_HPP
