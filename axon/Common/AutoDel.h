//======================================================================
// FILE:
// AutoDel.h
//
// PURPOSE:
// Define the template class CAutoDeletePtr<Type>
//
// NOTE:
// As the current 16-bit version (VC++ 1.52, CL 8.00c) can't use templates,
// I have provided two macros which will create a custom class for each
// template type.
//
//======================================================================
// Make sure we only include this file once
#ifndef INCLUDE_AUTODEL_H
#define INCLUDE_AUTODEL_H

//======================================================================
// Check this is a C++ compilation
#ifndef  __cplusplus
#error   __cplusplus not defined! This is a C++ ONLY header file!
#endif

//======================================================================
// COMPILE OPTIONS
// Use real templates for Win32, macros for Win16
#ifdef   _WIN32
#define  USE_CPP_TEMPLATES  1
#else
#define  USE_CPP_TEMPLATES  0
#endif

//======================================================================
// USAGE:
// Here are several ways to use this class, and even more ways to mis-use it.
//
#if 0
{
   // ======================================================================
   // GOOD USAGE:
   // Allocate and delete an array
   // Declare the CAutoDeletePtr<char> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (char);
   void           YourFunction ()
   {
      // Allocate an array of chars
      AUTO_DELETE_PTR (char) CharPtr (300);
      // Use CharPtr as if it is a (char*)
      strcpy (CharPtr, pszYourBuffer);
      strcpy (pszYourBuffer, CharPtr);
      // CharPtr deletes the array when it goes out of scope
   }

   // ======================================================================
   // GOOD USAGE:
   // Delete an array allocated externally
   // Declare the CAutoDeletePtr<char> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (char);
   char          *YourOtherFunction ();// Returns allocated array which the CALLER must delete
   void           YourFunction ()
   {
      // Create CharPtr to hold pointer returned from YourOtherFunction()
      AUTO_DELETE_PTR (char) CharPtr (YourOtherFunction (), AUTO_DELETE_PTR (char)::USE_ARRAY_DELETE);
      // Use CharPtr as if it is a (char*)
      strcpy (CharPtr, pszYourBuffer);
      strcpy (pszYourBuffer, CharPtr);
      // CharPtr deletes the array when it goes out of scope
   }

   // ======================================================================
   // GOOD USAGE:
   // Delete single item allocated externally
   // Declare the CAutoDeletePtr<CObject> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (CObject);
   CObject       *YourOtherFunction ();// Returns allocated object which the CALLER must delete
   void           YourFunction ()
   {
      // Create CObjectPtr to hold pointer returned from YourOtherFunction()
      AUTO_DELETE_PTR (CObject) CObjectPtr (YourOtherFunction (), AUTO_DELETE_PTR (CObject)::USE_SINGLE_DELETE);
      // Use CObjectPtr as if it is a (CObject*)
      ASSERT_VALID (CObjectPtr);
      // CObjectPtr deletes the array when it goes out of scope
   }

   // ======================================================================
   // BAD USAGE:
   // Although you can store a copy of the pointer in another pointer,
   // DON'T DO IT! When the object goes out of scope, it will leave the copy
   // of the pointer with an invalid value.
   //
   char          *pChar;
   // Declare the CAutoDeletePtr<char> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (char);
   void           YourFunction ()
   {
      // Allocate an array of chars
      AUTO_DELETE_PTR (char) CharPtr (300);
      // Store a pointer to the array
      pChar = CharPtr;
      // pChar is valid at this point
      // CharPtr deletes the array when it goes out of scope
   }
   // pChar is INVALID at this point!

   // ======================================================================
   // BAD USAGE:
   // Although you can store a pointer to a temporary object, DON'T DO IT! 
   // When the temporary object goes out of scope, it will leave the pointer 
   // with an invalid value.
   //
   // Declare the CAutoDeletePtr<char> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (char);
   void           YourFunction ()
   {
      // Allocate an array of chars
      char          *pChar = AUTO_DELETE_PTR (char) CharPtr (300);
      // CharPtr deletes the array when it goes out of scope
      // pChar is INVALID at this point!
   }

   // ======================================================================
   // BAD USAGE:
   // Although you can return a copy of the pointer from a function,
   // DON'T DO IT! When the object goes out of scope, it will leave the
   // return value with an invalid value.
   //
   // Declare the CAutoDeletePtr<char> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (char);
   char          *YourFunction ()
   {
      // Allocate an array of chars
      AUTO_DELETE_PTR (char) CharPtr (300);
      // Return a pointer to the array
      return CharPtr;
      // CharPtr deletes the array when it goes out of scope
   }
   // return value is INVALID!

   // ======================================================================
   // BAD USAGE:
   // Although you can create more than one object refering to the same
   // pointer, DON'T DO IT!.  When the first object goes out of scope it
   // will delete the pointer, leaving the remaining objects with invalid
   // pointers. I have disabled the copy constructor and assignment operator
   // but it is still possible by converting the first object to a pointer 
   // and constructing the second object from the pointer.
   //
   // Possible alternatives to detect and/or prevent this are:
   // 1. Keep a static list to confirm each object has a unique pointer,
   // but I can't see an automatic way of creating a static list for each type 
   // in only one place.
   // FIXME - try deriving all template classes from a common base and have a 
   // single list for all types.
   //
   // 2. Over-write the data with garbage before deleting it, but this could 
   // prevent a class's destructor from functioning.
   //
   // Declare the CAutoDeletePtr<char> class.
   // This needs to be done once in EACH source module using the class.
   DECLARE_AUTO_DELETE_PTR (char);
   void           YourFunction ()
   {
      // Allocate an array of chars
      AUTO_DELETE_PTR (char) CharPtr1 (300);
      {
         // Create CharPtr2 to point to the same array
         AUTO_DELETE_PTR (char) CharPtr2 (CharPtr, AUTO_DELETE_PTR (char)::USE_ARRAY_DELETE);
         // CharPtr2 deletes the array when it goes out of scope
      }
      // CharPtr1 is INVALID at this point!
      // CharPtr1 tries to delete array AGAIN when it goes out of scope
   }
}
#endif          // 0

//======================================================================
// INCLUDE FILES
#include <stddef.h>

//======================================================================
// TEMPLATE:
// CAutoDeletePtr
//
// PURPOSE:
// Automatically deallocate memory when the "pointer" goes out of scope.
//======================================================================
#if   USE_CPP_TEMPLATES
// {
// Define the macros to use the real template
#define  AUTO_DELETE_PTR(Type)         CAutoDeletePtr<Type>
#define  DECLARE_AUTO_DELETE_PTR(Type) // empty definition
template <class Type>
class CAutoDeletePtr
{
public:
   enum eDELETE
   {
      USE_ARRAY_DELETE,
      USE_SINGLE_DELETE
   };
                  ~CAutoDeletePtr ();
                  CAutoDeletePtr (const size_t nElementCount);
                  CAutoDeletePtr (Type * pType, const eDELETE eDeleteMethod);
   operator Type *() const;
private:
   Type          *m_pType;             /* Pointer to the template type */
   eDELETE        m_eDeleteMethod;     /* Specify the method used to delete the pointer */
   /* Copy ctor, op= private & unimplemented to prevent use */
                  CAutoDeletePtr (const CAutoDeletePtr<Type>&other);
                  CAutoDeletePtr<Type>&operator = (const CAutoDeletePtr<Type>&other);
};

template <class Type>
inline CAutoDeletePtr<Type>::~CAutoDeletePtr ()
{
   /* Destructor - delete the pointer using the appropriate form of delete */
   if (m_pType != NULL)
   {
      switch (m_eDeleteMethod)
      {
      case USE_ARRAY_DELETE:
         delete[] m_pType;
         break;
      case USE_SINGLE_DELETE:
         delete m_pType;
         break;
      default:
         /* invalid enum value */
         ASSERT (FALSE);
         break;
      }
   }
}

template <class Type>
inline CAutoDeletePtr<Type>::CAutoDeletePtr (const size_t nElementCount)
{
   /* Construct the object, allocating the array */
   m_pType = new Type[nElementCount];  /* Allocate the array of elements */
   ASSERT (m_pType != NULL);           /* Check we allocated the array ok */
   m_eDeleteMethod = USE_ARRAY_DELETE; /* Remember to use the array form of delete */
}

template <class Type>
inline CAutoDeletePtr<Type>::CAutoDeletePtr (Type * pType, const eDELETE eDeleteMethod)
{
   /* Construct the object from the given pointer & flag */
   m_pType = pType;                    /* Copy the given pointer */
   switch (eDeleteMethod)
   {
   case USE_ARRAY_DELETE:
   case USE_SINGLE_DELETE:
      m_eDeleteMethod = eDeleteMethod; /* Copy the deletion method flag */
      break;
   default:
      /* invalid enum value */
      ASSERT (FALSE);
      break;
   }
}

template <class Type>
inline CAutoDeletePtr<Type>::operator Type * () const
{
   /* COnversion operator - return the pointer */
   return m_pType;
}
// }
//======================================================================
#else           // USE_CPP_TEMPLATES
// {
// Define the macros to simulate the template
#define  AUTO_DELETE_PTR(Type)         CAutoDeletePtr##Type
#define  DECLARE_AUTO_DELETE_PTR(Type) \
class CAutoDeletePtr##Type\
{\
public:\
   enum eDELETE\
   {\
      USE_ARRAY_DELETE,\
      USE_SINGLE_DELETE\
   };\
                  ~CAutoDeletePtr##Type ();\
                  CAutoDeletePtr##Type (const size_t nElementCount);\
                  CAutoDeletePtr##Type (Type * pType, const eDELETE eDeleteMethod);\
   operator Type *() const;\
private:\
   Type          *m_pType;             /* Pointer to the template type */\
   eDELETE        m_eDeleteMethod;     /* Specify the method used to delete the pointer */\
   /* Copy ctor, op= private & unimplemented to prevent use */\
                  CAutoDeletePtr##Type (const CAutoDeletePtr##Type&other);\
                  CAutoDeletePtr##Type&operator = (const CAutoDeletePtr##Type&other);\
};\
\
inline CAutoDeletePtr##Type::~CAutoDeletePtr##Type ()\
{\
   /* Destructor - delete the pointer using the appropriate form of delete */\
   if (m_pType != NULL)\
   {\
      switch (m_eDeleteMethod)\
      {\
      case USE_ARRAY_DELETE:\
         delete[] m_pType;\
         break;\
      case USE_SINGLE_DELETE:\
         delete m_pType;\
         break;\
      default:\
         /* invalid enum value */\
         ASSERT (FALSE);\
         break;\
      }\
   }\
}\
\
inline CAutoDeletePtr##Type::CAutoDeletePtr##Type (const size_t nElementCount)\
{\
   /* Construct the object, allocating the array */\
   m_pType = new Type[nElementCount];  /* Allocate the array of elements */\
   ASSERT (m_pType != NULL);           /* Check we allocated the array ok */\
   m_eDeleteMethod = USE_ARRAY_DELETE; /* Remember to use the array form of delete */\
}\
\
inline CAutoDeletePtr##Type::CAutoDeletePtr##Type (Type * pType, const eDELETE eDeleteMethod)\
{\
   /* Construct the object from the given pointer & flag */\
   m_pType = pType;                    /* Copy the given pointer */\
   switch (eDeleteMethod)\
   {\
   case USE_ARRAY_DELETE:\
   case USE_SINGLE_DELETE:\
      m_eDeleteMethod = eDeleteMethod; /* Copy the deletion method flag */\
      break;\
   default:\
      /* invalid enum value */\
      ASSERT (FALSE);\
      break;\
   }\
}\
\
inline CAutoDeletePtr##Type::operator Type * () const\
{\
   /* COnversion operator - return the pointer */\
   return m_pType;\
}\
// end of macro
// }

//======================================================================
#endif          // USE_CPP_TEMPLATES
#undef   USE_CPP_TEMPLATES

//======================================================================
#endif          // INCLUDE_AUTODEL_H
// end of file
