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
#include "wincpp.hpp"
#include "SafeBuffer.h"


#ifdef TEST_ME
////////////////////////////////////////////////////
// Below here is test code and example code of how this class may be used.
//

#include <stdio.h>
#include "\AxonDev\Comp\AxonObjects\Test\AxonObjectTest.h"

using namespace AxonObjectTest;
   
//////////////////////////////////////////////////////////
// Tests
//
class SafeBuffer_Test : public Test
{
public:
   bool TestMe()
   {
      DECLARE_SafeBuffer(SB, 6);
      DECLARE_SafeBuffer(SB2, 16);


      ///////////////////////////////////////////////
      // Things that DO NOT WORK (unless you are lucky)
      // 
      //  SB = "123456";
      //  SB = SB.cstr() + 2;   <<<<< In place copying is a no no.
      //
      //  SB = SB.cstr()  is very likely to work but dont do it
      //  SB = SB;        is very likely to work but dont do it
      //
      //  If you need to do these write the test and the code
      //

      //////////////////////////////////////
      // Phase 1
      //       Usual Cases
      //////////////////////////////////////

      SB  = "ABC";
   //////////////////////////////////////
   // TO DO   
      // MAKE THIS Loook like
      // ASSERT( SB                  == "ABC" );    
      // ASSERT( SB._GetErrorFlags() == 0     );    

      m_ErrorLog.Assert ( SB              == "ABC" );    
      m_ErrorLog.Assert ( SB.IsOk()       == true  );
      m_ErrorLog.Assert ( SB.IsOverflow() == false );
      m_ErrorLog.Assert ( SB.IsNullParam()== false );

      m_ErrorLog.Assert ( SB._GetErrorFlags() == 0     );


      printf("Assignment && Equality <%s> (%d) == <ABC> (0)\n",SB.c_str(),SB._GetErrorFlags() );



      SB  = "123";
      SB += "45";

      m_ErrorLog.Assert ( SB              == "12345" );
      m_ErrorLog.Assert ( SB.IsOk()       == true  );
      m_ErrorLog.Assert ( SB.IsOverflow() == false );
      m_ErrorLog.Assert ( SB.IsNullParam()== false );

      printf("No Overflow <%s> (%d) == <12345> (0)\n",SB.c_str(),SB._GetErrorFlags() );

      // Dealing with trucation elegantly may be hard!!!!
      //////////////////////////////////////////////////////////////
      // If you think that you want to write code like this.
      //    Please try and find another way to do it.
      //    This code works, but it will be hard to maintain
      //    That may be the best you can do. Sometimes string code is HARD.
      //
      SafeBuffer Tmp;
      SB = "";
      Tmp.remainder(SB);
                      // Make a temporary 
      Tmp  = "12";
      Tmp.remainder(Tmp);      // advance the temporary  
                               //The next line assigns to Tmp  which is a cat to SB !!!
      Tmp  = "3456";           // Causes overflow error but it is not propogated to SB!!!!!
      if (!Tmp.IsOk())
            Tmp  = "...";      // Overwirtes the error flag


      m_ErrorLog.Assert ( SB              == "12..." );
      m_ErrorLog.Assert ( SB.IsOk()       == true  );
      m_ErrorLog.Assert ( SB.IsOverflow() == false );
      m_ErrorLog.Assert ( SB.IsNullParam()== false );
      //Note how the error flag is nowhere to be seen
      m_ErrorLog.Assert ( Tmp               == "..." );
      m_ErrorLog.Assert ( Tmp.IsOk()        == true  );
      m_ErrorLog.Assert ( Tmp.IsOverflow()  == false );
      m_ErrorLog.Assert ( Tmp.IsNullParam() == false );

      printf("remainder() of Self <%s> (%d) == <12...> (0)\n",SB.c_str(),SB._GetErrorFlags() );

      // Test that the macro intialises its internal char array
      DECLARE_SafeBuffer(SBempty, 6);

      m_ErrorLog.Assert ( SBempty == "" );

      printf("Default initialisation <%s> (%d) == <> (0)\n",SBempty.c_str(),SBempty._GetErrorFlags() );


      // The following commented out CODE IS INCORRECT
      //char f[13];
      //SafeBuffer tt(f, sizeof(f));   
      //tt += "foo";
      //  End of INCORRECT CODE

      // How to do the same thing but correctly
      char pStr[13];
      size_t uSize = sizeof(pStr);
      // Assume we were passed a char * and a sizeof 
      SafeBuffer SB_UnInit_1(pStr, uSize);   
      SB_UnInit_1 = "foo";                 // <<<<< This initalises the memory 
      SB_UnInit_1 += " bar";

      m_ErrorLog.Assert ( SB_UnInit_1               == "foo bar" );

      printf("Manual initialisation <%s> (%d) == <foo bar> (0)\n",SB_UnInit_1.c_str(),SB_UnInit_1._GetErrorFlags() );

      //////////////////////////////////////
      // Phase 2
      //    Usual Error Cases
      //////////////////////////////////////

      SB  = "123";
      SB += "45678";

      m_ErrorLog.Assert ( SB              == "12345" );
      m_ErrorLog.Assert ( SB.IsOk()       == false   );
      m_ErrorLog.Assert ( SB.IsOverflow() == true    );
      m_ErrorLog.Assert ( SB.IsNullParam()== false   );

      printf("Overflow <%s> (%d) == <12345> (1)\n",SB.c_str(),SB._GetErrorFlags() );



      SB  = "12345678";

      m_ErrorLog.Assert ( SB              == "12345" );
      m_ErrorLog.Assert ( SB.IsOk()       == false   );
      m_ErrorLog.Assert ( SB.IsOverflow() == true    );
      m_ErrorLog.Assert ( SB.IsNullParam()== false   );

      printf("Overflow2 <%s> (%d) == <12345> (1)\n",SB.c_str(),SB._GetErrorFlags() );



      SB  = "123";
      SB += (char *)0;

      m_ErrorLog.Assert ( SB              == "123" );
      m_ErrorLog.Assert ( SB.IsOk()       == false   );
      m_ErrorLog.Assert ( SB.IsOverflow() == false   );
      m_ErrorLog.Assert ( SB.IsNullParam()== true    );

      printf("Null Ptr <%s> (%d) == <123> (2)\n",SB.c_str(),SB._GetErrorFlags() );



      //////////////////////////////////////
      // Phase 3
      //    Less common Error propogation Cases
      //////////////////////////////////////

      SB  = "12345678";    // This has a truncation error
      SB2 = SB;            // This shows the error is propagated into SB2 using =

      m_ErrorLog.Assert ( SB2              == "12345" );
      m_ErrorLog.Assert ( SB2.IsOk()       == false   );
      m_ErrorLog.Assert ( SB2.IsOverflow() == true    );
      m_ErrorLog.Assert ( SB2.IsNullParam()== false   );

      printf("Error Propogation = <%s> (%d) == <123> (1)\n",SB.c_str(),SB2._GetErrorFlags() );



      SB   = "12345678";      // This has a truncation error
      SB2  = "Fred";
      SB2 += SB;              // This shows the error is propagated into SB2 using +=

      m_ErrorLog.Assert ( SB2              == "Fred12345" );
      m_ErrorLog.Assert ( SB2.IsOk()       == false   );
      m_ErrorLog.Assert ( SB2.IsOverflow() == true    );
      m_ErrorLog.Assert ( SB2.IsNullParam()== false   );

      printf("Error Propogation += <%s> (%d) == <123> (1)\n",SB.c_str(),SB2._GetErrorFlags() );



      SB  = (char *)0;
      SB2 = SB;

      m_ErrorLog.Assert ( SB2              == ""  );
      m_ErrorLog.Assert ( SB2.IsOk()       == false   );
      m_ErrorLog.Assert ( SB2.IsOverflow() == false   );
      m_ErrorLog.Assert ( SB2.IsNullParam()== true    );

      printf("Error Propogation  NULL = <%s> (%d) == <123> (2)\n",SB.c_str(),SB2._GetErrorFlags() );


      return m_ErrorLog.hasErrors();
   }

}; // end of class SafeBuffer_Test

namespace UnitTest {

SafeBuffer_Test g_SafeBuffer_Test;
AxonObjectTest::RegisterMe RegMe(&g_SafeBuffer_Test);

};

#endif

