//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  Median.hpp
// PURPOSE: Fast calculation of a median of a list.
// AUTHOR:  BHI  June 1999
//
// NOTES:   The testbed (below) showed the following:
//          (a) All three methods produced identical results.
//          (b) Method 1 (std::sort()) was significantly slower than the other two methods.
//
#include <algorithm>

template <class TYPE>
inline double _MedianOf(TYPE arr[], int n)
{
   int m = n/2;
   return (n % 2) ? arr[m] : (double(arr[m-1]) + arr[m])/2;
}

#define USE_STL_MEDIAN 0

#if USE_STL_MEDIAN==1

//===============================================================================================
// FUNCTION: Median (1)
// PURPOSE:  Returns the median of a distribution (sorts data).
//
template <class TYPE>
inline double Median(TYPE arr[], int n)
{
   std::sort(arr, arr+n);
   return _MedianOf(arr, n);
}

#elif USE_STL_MEDIAN==2

//===============================================================================================
// FUNCTION: Median (2)
// PURPOSE:  Returns the median of a distribution (sorts data).
//
template <class TYPE>
inline double Median(TYPE arr[], int n)
{
   std::nth_element(arr, arr+n/2, arr+n);
   return _MedianOf(arr, n);
}

#else

//===============================================================================================
// FUNCTION: Median (3)
// PURPOSE:  Returns the median of a distribution (partially reorders data).
// NOTES:    This Median routine is based on the QuickSelect algorithm described in
//           "Numerical recipies in C", Second Edition,
//           Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
//
template <class TYPE>
inline double Median(TYPE arr[], int n)
{
   #define ELEM_SWAP(a,b) { register TYPE t=(a);(a)=(b);(b)=t; }

   int low    = 0;
   int high   = n-1;
   const int median = n / 2;

   for (;;)
   {
      if (high <= low)        // One element only
         break;
      
      if (high == low + 1)    // Two elements only
      {
         if (arr[low] > arr[high])
            ELEM_SWAP(arr[low], arr[high]);
         break;
      }
      
      // Find median of low, middle and high items; swap into position low
      const int middle = (low + high) / 2;
      if (arr[middle] > arr[high])
         ELEM_SWAP(arr[middle], arr[high]);
      if (arr[low] > arr[high])
         ELEM_SWAP(arr[low], arr[high]);
      if (arr[middle] > arr[low])
         ELEM_SWAP(arr[middle], arr[low]);
      
      // Swap low item (now in position middle) into position (low+1)
      ELEM_SWAP(arr[middle], arr[low+1]);
      
      // Nibble from each end towards middle, swapping items when stuck
      int ll = low + 1;
      int hh = high;
      for (;;)
      {
         do ll++;
         while (arr[low] > arr[ll]);

         do hh--;
         while (arr[hh]  > arr[low]);
         
         if (hh < ll)
            break;
         
         ELEM_SWAP(arr[ll], arr[hh]);
      }
      
      // Swap middle item (in position low) back into correct position
      ELEM_SWAP(arr[low], arr[hh]);
      
      // Re-set active partition
      if (hh <= median)
         low = ll;
      if (hh >= median)
         high = hh - 1;
   }

   #undef ELEM_SWAP

   double medianval;

   if (n%2)
      medianval = arr[median];
   else
   {
      // otherwise we find the value to average with by maximising low partition
      TYPE max = arr[0];
      for (int i = 1; i < median; i++)
      {
         if (arr[i] > max)
            max = arr[i];
      }

      medianval = (double(arr[median])+max)/2;
   }
   return medianval;
}

#endif

#ifdef BUILD_MEDIAN_TESTBED

//##########################################################################################
//##########################################################################################
//###
//###  TESTBED for comparison of medians.
//###
//##########################################################################################
//##########################################################################################

#include "..\AxoDefn.h" // <windows.h>
#include <stdlib.h>
#include <algorithm>

template <class TYPE>
inline double MedianOf(TYPE arr[], int n)
{
   int m = n/2;
   return (n % 2) ? arr[m] : TYPE((double(arr[m-1]) + arr[m])/2);
}

template <class TYPE>
inline double MedianSort(TYPE arr[], int n)
{
//   ASSERT(n > 0);
   std::sort(arr, arr+n);
   return MedianOf(arr, n);
}

template <class TYPE>
inline double MedianNthElement(TYPE arr[], int n)
{
//   ASSERT(n > 0);
   std::nth_element(arr, arr+n/2, arr+n);
   return MedianOf(arr, n);
}

template <class TYPE>
inline double MedianSelect(TYPE arr[], int n)
{
   #define ELEM_SWAP(a,b) { register TYPE t=(a);(a)=(b);(b)=t; }

   int low  = 0;
   int high = n-1;
   int m    = n/2;

   for (;;)
   {
      if (high <= low)        // One element only
         break;
      
      if (high == low + 1)    // Two elements only
      {
         if (arr[low] > arr[high])
            ELEM_SWAP(arr[low], arr[high]);
         break;
      }
      
      // Find median of low, middle and high items; swap into position low
      const int middle = (low + high) / 2;
      if (arr[middle] > arr[high])
         ELEM_SWAP(arr[middle], arr[high]);
      if (arr[low] > arr[high])
         ELEM_SWAP(arr[low], arr[high]);
      if (arr[middle] > arr[low])
         ELEM_SWAP(arr[middle], arr[low]);
      
      // Swap low item (now in position middle) into position (low+1)
      ELEM_SWAP(arr[middle], arr[low+1]);
      
      // Nibble from each end towards middle, swapping items when stuck
      int ll = low + 1;
      int hh = high;
      for (;;)
      {
         do ll++;
         while (arr[low] > arr[ll]);

         do hh--;
         while (arr[hh]  > arr[low]);
         
         if (hh < ll)
            break;
         
         ELEM_SWAP(arr[ll], arr[hh]);
      }
      
      // Swap middle item (in position low) back into correct position
      ELEM_SWAP(arr[low], arr[hh]);
      
      // Re-set active partition
      if (hh <= m)
         low = ll;
      if (hh >= m)
         high = hh - 1;
   }

   #undef ELEM_SWAP
   // BUG BUG -- this routine works fine if the result is arr[n/2],
   //            but for even number of items this is incorrect.
   ASSERT(FALSE);
   return MedianOf(arr, n);
}

int main(int argc, char* argv[])
{
#ifdef TIMING_TEST
   static WORD awRand[1000001];
   for (int i=0; i<ELEMENTS_IN(awRand); i++)
      awRand[i] = rand();
#else
   WORD awRand[] = { 8,6,4,2 };
#endif

   WORD awMedians[3];
   static WORD awLists[ELEMENTS_IN(awMedians)][ELEMENTS_IN(awRand)];

#ifdef TIMING_TEST
   for (i=10001; i<ELEMENTS_IN(awRand); i+=10000)
#else
   int i=ELEMENTS_IN(awRand);
#endif
   {
      for (int j=0; j<ELEMENTS_IN(awMedians); j++)
         memcpy(awLists[j], awRand, i*sizeof(awRand[0]));

      UINT uTime0 = GetTickCount();
      awMedians[0] = MedianSelect(awLists[0], i);

      UINT uTime1 = GetTickCount();
      awMedians[1] = MedianNthElement(awLists[1], i);

      UINT uTime2 = GetTickCount();
      awMedians[2] = MedianSort(awLists[2], i);

      UINT uTime3 = GetTickCount();

      UINT uDelta1 = uTime1 - uTime0;
      UINT uDelta2 = uTime2 - uTime1;
      UINT uDelta3 = uTime3 - uTime2;

      printf("\n%d:\t%d\t%d\t%d", i, uDelta1, uDelta2, uDelta3);
      printf("\n\t%d\t%d\t%d", awMedians[0], awMedians[1], awMedians[2]);
   }

   printf("\n");
	return 0;
}

#endif
