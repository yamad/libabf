//**********************************************************************
// RecursionCounter.hpp
//
// PURPOSE:
// Implement a thread-safe recursion counter.
//
// NOTE:
// This code is ok to use in release builds.
//
// The InterlockedXXX functions are quite fast.
// For the release version they are intrinsic, and usually 
// around a dozen bytes of machine code.
//
// AUTHOR:
// Andrew Dalgleish
//
// Copyright (c) 2001 Axon Instruments, Inc.
// All rights reserved.

#ifndef  INC_RECURSIONCOUNTER_HPP
#define  INC_RECURSIONCOUNTER_HPP

//**********************************************************************
// atomic_read()
//
// PURPOSE:
// Read a LONG value from the pointer in a thread-safe manner.
//
inline LONG  atomic_read(LPLONG pointer)
{
   return InterlockedExchangeAdd(pointer, 0);
}

//**********************************************************************
// IF_RECURSING()
//
// PURPOSE:
// Increment and test the value, and decrement it at the end of scope.
//
// NOTE:
// This is a macro because we instantiate a (hidden) temporary object 
// and use its destructor to decrement the pointer.
//
#define  IF_RECURSING(pointer)   IF_RECURSING2(pointer, __LINE__)
#define  IF_RECURSING2(pointer, line)  \
   CHiddenRecursionCounter    ___hidden_recursion_counter___##line;  \
   if (___hidden_recursion_counter___##line.hook_and_increment(pointer) != 1)


//**********************************************************************
// For some reason the function InterlockedExchangePointer() is 
// documented, but not defined in the MSVC 6 header files.
// This is a work-around which works for Win32.
#ifndef  InterlockedExchangePointer
STATIC_ASSERT(sizeof(LONG) == sizeof(PVOID));
#define  InterlockedExchangePointer(ppvDestination, pvExchange)  PVOID(InterlockedExchange(LPLONG(ppvDestination), LONG(pvExchange)))
#endif

//**********************************************************************
// CHiddenRecursionCounter
//
// PURPOSE:
// Atomically increment a pointer, and hook the pointer so we can
// decrement it when we go out of scope.
//
// We want the "increment and return the value" to be atomic we can't 
// do the increment in the constructor.
//
// We have to hook the pointer when we do the increment so we don't
// decrement in the dtor if we did not do the increment.
//
class CHiddenRecursionCounter
{
public:
   ~CHiddenRecursionCounter()
   {
      // Unhook and decrement our pointer if we had one
      hook_and_increment(NULL);
   }
   CHiddenRecursionCounter()
      :current_pointer(NULL)
   {
   }
   LONG  hook_and_increment(LPLONG new_pointer)
   {
      // Hook the pointer atomically
      LPLONG   old_pointer = LPLONG(InterlockedExchangePointer(&current_pointer, new_pointer));

      // Decrement the old pointer
      if (old_pointer)
      {
         WPTRASSERT(old_pointer);
         InterlockedDecrement(old_pointer);
      }

      // Increment the new pointer and return its NEW value
      if (new_pointer)
      {
         WPTRASSERT(new_pointer);
         return InterlockedIncrement(new_pointer);
      }

      return 0;
   }
private:
   LONG*  current_pointer;

   // private and unimplemented to prevent use
   CHiddenRecursionCounter(CHiddenRecursionCounter const &);
   CHiddenRecursionCounter &           operator =(CHiddenRecursionCounter const &);
   void*                               operator new(size_t);
   CHiddenRecursionCounter*            operator &();
   CHiddenRecursionCounter const *     operator &() const;
   CHiddenRecursionCounter*            operator ->();
   CHiddenRecursionCounter const *     operator ->() const;
};


//**********************************************************************
// USAGE:
#if 0

//----------------------------------------------------------------------
// The most common usage will be to stop simple recursion.

void  function_X()
{
   static LONG  in_function_X = 0;
   IF_RECURSING(&in_function_X)
      return;
}

//----------------------------------------------------------------------
// If you are using an object the variable should be an object member 
// variable.
//
// Don't forget to initialize it in the class constructor, and you 
// probably need to *clear* it in any copy constructor or assignment 
// operator.
//
// If you are protecting a const function the variable must be "mutable".

class ClassA
{
public:
   ClassA();
   void  function_X();
   void  function_Y() const;
private:
   LONG           in_function_X;
   mutable  LONG  in_function_Y;
};

ClassA::ClassA()
:in_function_Y(0)
{
}

void  ClassA::function_X()
{
   MEMBERASSERT();

   IF_RECURSING(&in_function_X)
   {
      // If you expect the recursion, don't assert
      return;
   }
}

void  ClassA::function_Y() const
{
   MEMBERASSERT();

   IF_RECURSING(&in_function_Y)
   {
      // If you don't expect the recursion, assert!
      ASSERT(false);
      return;
   }
}

//----------------------------------------------------------------------
// A more complex example is to use the recursion counter to protect 
// yourself during "unsafe" operations.
//
// This is often not apparent from your own code, but can occur if you 
// call other modules which then call back into you.
//
// Note that "in_function_A" must be mutable because we use atomic_read() 
// from function_B() which is const.

class ClassB
{
public:
   ClassB();
   void  function_A();
   void  function_B() const;
private:
   mutable  LONG  in_function_A;
};

// other_function is called from ClassB::function_A, and calls back into ClassB::function_B
void  other_function(ClassB *my_class)
{
   my_class->function_B();
}

ClassB::ClassB()
:in_function_A(0)
{
}

// This function modifies ClassB in such a way that it is temporarily invalid.
// To prevent accidents I use a recursion counter to check if I am in this function.
void  ClassB::function_A()
{
   IF_RECURSING(&in_function_A)
   {
      ASSERT(false);
      return;
   }

   // do stuff which modifies ClassB...

   // Call other_function (which calls ClassB::function_B)
   other_function(this);

   // do more stuff...
}

void  ClassB::function_B() const
{
   // This function must not be called if I am in the middle of function_A.
   if (atomic_read(&in_function_A))
   {
      ASSERT(false);
      return;
   }

}

#endif

#endif
