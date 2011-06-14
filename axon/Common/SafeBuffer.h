//****************************************************************************
//
//    Copyright (c) 2001 Axon Instruments.
//    All rights reserved.
//
//****************************************************************************
// MODULE :  SafeBuffer.cpp
//
// CLASSES:  SafeBuffer
//
// RELATED MODULES:
//
// PURPOSE: Prevent Bugs.
//
// AUTHOR : AKC   2001
//
//////////////////////////////////////////////////////////////////////
//
// HOUSE RULES:
//
//  ASSERTS that specify "Safe" can be ignored but you DO have bug.
//          "Fatal" are likely to go bang real soon.
//          "Internal" roughly translate as Panic Now and it is sound advice.
//                     If one of these goes of the last check contained a bad error.       
//
//  The code in this module should adhere to the stl
//  function naming convention to facilitate the eventual deprecation
//  of this class in favour of the STL (or other like) string.
//
//  'Once and only once'. see canary() for an example:
//      the code fragment "m_szStr[m_nSize-1]" was written many times
//      it was refactored and replaced with canary()
//      Note how append reuses copy
//          Why ?
//          append (strncat) is the most dangerous function to write (or use).
//          remainder seemed safer to implement
//             and more importantly easier to test
//
// 'YAGNI'. You Arent Gunna Need It.
//      Actaully you do need this and only this functionality
//      and I have the list of bugs to prove it.
//
// #include <string>  // <<= HyperLink for reference purposes
//
#ifndef INC__H
#define INC__H
#pragma once



#include "\axondev\comp\common\axodebug.h"
#define UNIMPLEMENTED TRACE("UNIMPLEMENTED Feature");


////////////////////////////////////////////////////
// CLASS  : SafeBuffer
// PURPOSE: This is not an ALL singing all dancing string library. KISS
//          This class is designed to protect you from yourself!   KISS
//          Manipulating char * buffers is time consuming and
//          difficult. Difficult is defined by the fact that
//          frequently char * buffer manipulation code is buggy
//          and very fragile. The functional decomposition
//          approach of strncpy, strcpy, strcat. strncat
//          and even AXU_sprintf and AXU_strncpyz is not
//          sufficient. We (You and I) are better at writing bugs
//          than that! We are like users, capable of misusing any feature
//
//          The solution is a bug proof object.
//
//          Ok it is not bug PROOF but if we use it lots of times
//          and lots of people look at it eventually it will be
//          bug free and very hard to use incorrectly.
//
//          Ie. The intention of the contract that class provides
//          you is that almost no matter how you use it the class
//          will do what you asked it to, it may be that the result is a
//          misfeature, but it will not be a bug or a crash.
//
//          Misfeatures are easier to test for and less embarassing
//          when shipped by accident.
//
//          CAVEAT: This class is the wrong solution. We should use
//          a real string class like STL strings but we can't?
//
//          eg
//          DECLARE_SafeBuffer(Fred, 6);
//          // expands into
//          // char DoNotUseMe_Fred[6];
//          // SafeBuffer(Fred, ELEMENTS_IN(Fred));
//
//          Fred  = "123";
//          Fred += "45678";
//          printf("<%s> (%d)", Fred.c_str(), Fred.IsOk());
//          // note: printf(Fred.c_str()); is NOT safe robust code.
//          // result   <12345> (0)
//          // 0 => we truncated
//
//          Fred  = "123";  //Rests IsOk back to 1 (true)
//          Fred += "45";
//          printf("<%s> (%d)", Fred.c_str(), Fred.IsOk());
//          // result   <12345> (1)
//          // 1 => we truncated
//
//
class SafeBuffer_Test; // This documents usage, runnably and testably.

class SafeBuffer
{
   /////////////// Used for testing
   friend class SafeBuffer_Test;
   int  _GetErrorFlags() { return  m_ErrorFlag; }
   ///////////////

   char *m_szStr;
   int   m_nSize;
   int   m_ErrorFlag;

   void InvalidState()
   {  // You have asked for a nonsensical operation
      // We will now refuse to play.
      m_szStr = 0;
      m_nSize = 0;
      m_ErrorFlag  = eBadThings;
   }

   void _wipe()
   {
      InvalidState();
   }

public:

   enum enumErrorBitFlags { eNoError = 0, eOverflow = 1, eNullParam = 2, eBadThings = 3 };

   SafeBuffer()
   {
      _wipe();
   }

   SafeBuffer(char *pStr, int sz)
   {
      _wipe();
      if (pStr && sz > 0 )
      {
         ARRAYASSERT(m_szStr,m_nSize);
         m_szStr  = pStr;
         m_nSize = (UINT)sz;
         m_ErrorFlag  = eNoError;
         m_szStr[m_nSize-1] = 0;
      }
      else
      {
         //cant do this check    WRITEPTR_ASSERT(m_szStr,m_nSize);
         // If you dont like these constraints on our contract define correct
         // behaviour and implement it.
         ASSERT(!(sz==0)  && "Safe:Zero     sized buffers are Not Nice!!");
         ASSERT(!(sz<0)   && "Safe:Negative sized buffers are even Not Nicer!!");
         ASSERT((pStr!=0) && "Safe:Null Ptrs are BAD");
         InvalidState();
         m_ErrorFlag  = eNullParam;
      }
   }

   void copy(const char *sz);
   void copy(const SafeBuffer &SB) 
   { 
      copy(SB.c_str()); 
      m_ErrorFlag |= SB.m_ErrorFlag; 
   }
   SafeBuffer &operator = (const SafeBuffer &SB)
   {
      copy(SB);
      return *this;
   }
   SafeBuffer &operator =(const char *pStr)
   {
      copy(pStr);
      return *this;
   }

   void append(const char *sz);
   void append(const SafeBuffer &SB) 
   { 
      append(SB.c_str()); 
      m_ErrorFlag |= SB.m_ErrorFlag; 
   }
   SafeBuffer &operator +=(const SafeBuffer &SB)
   {
      append(SB);
      return *this;
   }
   SafeBuffer &operator +=(const char *sz)
   {
      append(sz);
      return *this;
   }

   const char *c_str() const { return m_szStr; }
         char *c_str()       { return m_szStr; }

   int const size() const { return m_nSize; }

   void remainder(SafeBuffer &);
   
   //////////////////////////////////////////////////
   // Extensions to string required by the LIMITATIONS of this
   // solution to a well known standard already fixed problem.
   // ie These are NOT enhancements they are work arounds
   // ie do NOT rape and paste these as good ideas.
   const char *canary() const { return &m_szStr[m_nSize-1]; }
         char *canary()       { return &m_szStr[m_nSize-1]; }

   bool IsOk()          { return m_ErrorFlag == eNoError; }
   bool IsOverflow()    { return (m_ErrorFlag & eOverflow) != 0; }
   bool IsNullParam()   { return (m_ErrorFlag & eNullParam) != 0; }

   static bool OurEquals(const char *sz1,const char *sz2)
   {
      if (sz1 == 0 || sz2 == 0)
      {
         return sz1 == sz2; //Umlike NAN   NULLptr == NULLptr
      }
      return strcmp(sz1,sz2) == 0;
   }
   static bool OurLessThan(const char *sz1,const char *sz2)
   {
      if (sz1 == 0 || sz2 == 0)
      {
         return sz1 < sz2; //Umlike NAN   NULLptr == NULLptr
      }
      return strcmp(sz1,sz2) < 0;
   }
};



inline bool operator ==(const SafeBuffer &SB , const char *sz)
{
   return SafeBuffer::OurEquals(SB.c_str(),sz);
}

inline operator ==(const SafeBuffer &SB1,const SafeBuffer &SB2)
{
   return SafeBuffer::OurEquals(SB1.c_str(),SB2.c_str());
}

inline operator ==(const char *sz,const SafeBuffer &SB2)
{
   return SafeBuffer::OurEquals(sz,SB2.c_str());
}



#define DECLARE_SafeBuffer(a,s)                                \
      char DoNotUseMe_ ## a [ s ] = "";                        \
      SafeBuffer a( DoNotUseMe_ ## a, sizeof(DoNotUseMe_ ## a))

namespace AxonObjects {


/////////////////////////////////////////////////////////////////////////
//TO DO: we need inline wrappers on AXU_sprintf, AXU_strncpyz, LoadString
//     to all take SafeBuffer parameters.
// AXU_sprintf( SafeBuffer &SB, const char *Fmt, ...)
// {
//    AXU_vsprintf( SB.c_str(), SB.size(),Fmt, xxx);
// }
//


inline int LoadString( HINSTANCE h, UINT uID, SafeBuffer &SB   )
{
   return ::LoadString( h,  uID, SB.c_str(), SB.size() );
}
//
// AXU_LoadString( HINSTANCE h, UINT uID, SafeBuffer &SB   )
// {
//    LoadString( HINSTANCE h, UINT uID, SB.c_str(), SB.size() );
// }
//
//

};



/////////////////////////////////////////////////////////////////////////
// PURPOSE: Set up this Buffer to be the remainder of that buffer
// This function is private by DESIGN for the purpose of KISS.
// If it is made public it will be to easy for clients to write
// code that does not do what they mean.                 KISS.
//
// PURPOSE: Turn us into a SafeBuffer to the unused portion of WholeBuffer
//          See append for the example
//
//
inline void SafeBuffer::remainder(SafeBuffer &WholeBuffer)
{

   if (m_szStr)
   {  // This function is odd in that it is a valid operation even on
      // an invalid object !!!!!
      // unit tests on us.
      ASSERT(m_nSize >= 0 && "Fatal: Bad things might happen real soon.");
      ASSERT(*canary() == 0 && "Internal: The Canary is Dead");    // Check the canary is still there
   }

   char *pStr = WholeBuffer.c_str();
   if (!pStr)
   {
      InvalidState();
      m_ErrorFlag |= eNullParam;
      return;
   }

   int nCurrLen = strlen(pStr);
   m_szStr = pStr + nCurrLen;                // start us at their strings end point
   m_nSize = WholeBuffer.m_nSize - nCurrLen; // compute our size: the remainder of them
   // unit tests on us. (the previous line)
   ASSERT( canary() == WholeBuffer.canary() && "Internal: The Canary is both Lost && Dead");
   // unit tests on us. (the previous 2 lines to that line)
   ASSERT( m_szStr[0] == 0 && "Internal: We just computed that 1 + 1 == 3");
   m_ErrorFlag = eNoError;
}

/////////////////////////////////////////////////////////////////////////
// PURPOSE: Self Evident
//
inline void SafeBuffer::copy(const char *sz)
{
   ASSERT(m_szStr!=0 && "It appears you are copying to an unintialised (default constructed) object");
   m_szStr[0] = 0;            ///Copy deletes our Error State
   m_ErrorFlag = eNoError; 

   if (!sz)
   {
      m_ErrorFlag |= eNullParam;
      return;
   }
   if (!m_szStr)
   {
      m_ErrorFlag |= eNullParam;
      return;
   }

   // unit tests
   ASSERT(m_nSize >= 0 && "Fatal: Bad things will happen real soon.");
   ASSERT(*canary() == 0 && "Internal: The Canary is Dead");    // Check the canary is still there
   // roughly (as best we can) test adherance of caller to contract.
   // many callers who break the contract will pass this test.

   *canary() = 0;                      // put   the canary in place

   strncpy(m_szStr,sz,m_nSize); // copies at MOST n chars may fail to terminate
   // Logical ASSERTION if strlen(sz)>= m_nSize Canary is now dead
   if (m_szStr[m_nSize-1] != 0)
   {
      // check canary is still alive
      m_ErrorFlag |= eOverflow;
   }
   m_szStr[m_nSize-1] = 0;             // Guarantee termination => canary is alive
}

/////////////////////////////////////////////////////////////////////////
// PURPOSE: Self Evident
//
inline void SafeBuffer::append(const char *sz)
{
   if (!sz)
   {
      m_ErrorFlag |= eNullParam;
      return;
   }
   if (!m_szStr)
   {
      m_ErrorFlag |= eNullParam;
      return;
   }
   // unit tests
   ASSERT(m_nSize >= 0 && "Fatal: Bad things will happen real soon.");
   ASSERT(m_szStr[m_nSize-1] == 0 && "Internal: The Canary is Dead");    // Check the canary is still there

   SafeBuffer tmp;
   tmp.remainder(*this);

   tmp.copy(sz);                       // reuse copy
   // This is the required line of non obvious code that
   //       prevents remainder from being a public member function
   m_ErrorFlag |= tmp.m_ErrorFlag;
}






#endif
