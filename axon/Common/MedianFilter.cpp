//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  MedianFilter.CPP
// PURPOSE: Contains routines to filter an image using a median filter.

#include "wincpp.hpp"
#include "MedianFilter.h"
#include "CHighResTimer.hpp"

//===============================================================================================
// Median Finding on a 3-by-3 Grid by Alan Paeth from "Graphics Gems", Academic Press, 1990
//

#define s2(a,b)            { int t=int(b)-a; if (t<0) { a=WORD(a+t); b=WORD(b-t); }}
#define mn3(a,b,c)         s2(a,b); s2(a,c);
#define mx3(a,b,c)         s2(b,c); s2(a,c);
#define mnmx3(a,b,c)       mx3(a,b,c); s2(a,b);
#define mnmx4(a,b,c,d)     s2(a,b); s2(c,d); s2(a,c); s2(b,d);
#define mnmx5(a,b,c,d,e)   s2(a,b); s2(c,d); mn3(a,c,e); mx3(b,d,e);
#define mnmx6(a,b,c,d,e,f) s2(a,d); s2(b,e); s2(c,f); mn3(a,b,c); mx3(d,e,f);

//===============================================================================================
// FUNCTION: Median3x3
// PURPOSE:  Find median on a 3x3 input box of integers.
// NOTES:    b1, b2, b3 are pointers to the left-hand edge of three parallel scan-lines 
//           to form a 3x3 spatial median.
//           Rewriting b2 and b3 as b1 yields code which forms median on input presented 
//           as a linear array of nine elements.
//
inline WORD Median3x3(WORD *b1, WORD *b2, WORD *b3)
{
   WORD r1 = b1[0]; 
   WORD r2 = b1[1]; 
   WORD r3 = b1[2];
   WORD r4 = b2[0];
   WORD r5 = b2[1];
   WORD r6 = b2[2];
   mnmx6(r1, r2, r3, r4, r5, r6);
   r1 = b3[0];
   mnmx5(r1, r2, r3, r4, r5);
   r1 = b3[1];
   mnmx4(r1, r2, r3, r4);
   r1 = b3[2];
   mnmx3(r1, r2, r3);
   return r2;
}

//===============================================================================================
// FUNCTION: Mean3x3
// PURPOSE:  Find mean on a 3x3 input box of integers.
// NOTES:    b1, b2, b3 are pointers to the left-hand edge of three parallel scan-lines 
//           to form a 3x3 spatial median.
//
inline WORD Mean3x3(WORD *b1, WORD *b2, WORD *b3)
{
   return WORD((UINT(b1[0])+b2[0]+b3[0]+b1[1]+b2[1]+b3[1]+b1[2]+b2[2]+b3[2])/9);
}

//===============================================================================================
// FUNCTION: Mean5x5
// PURPOSE:  Find mean on a 5x5 input box of integers.
// NOTES:    b1, b2, b3 are pointers to the left-hand edge of three parallel scan-lines 
//           to form a 3x3 spatial median.
//
inline WORD Mean5x5(WORD *b0, WORD *b1, WORD *b2, WORD *b3, WORD *b4)
{
   UINT uSum = 0;
   for (int i=0; i<5; i++)
      uSum += UINT(b0[i])+b1[i]+b2[i]+b3[i]+b4[i];
   return WORD(uSum/25);
}

//===============================================================================================
// FUNCTION: Mean5x5_InRange
// PURPOSE:  Find mean on a 5x5 input box of integers.
// NOTES:    b0, b1, b2, b3, b4 are pointers to the left-hand edge of five parallel scan-lines 
//           to form a 5x5 spatial median.
//
inline WORD Mean5x5_InRange(WORD *b0, WORD *b1, WORD *b2, WORD *b3, WORD *b4, WORD wMedian, WORD wRange)
{
   UINT uSum   = 0;
   UINT uCount = 0;
   int nVal;
   for (int i=0; i<5; i++)
   {
      nVal = b0[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
      nVal = b1[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
      nVal = b2[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
      nVal = b3[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
      nVal = b4[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
   }
   ASSERT(uCount > 0);
   return WORD(uSum/uCount);
}

//===============================================================================================
// FUNCTION: Mean3x3_InRange
// PURPOSE:  Find mean on a 3x3 input box of integers.
// NOTES:    b1, b2, b3 are pointers to the left-hand edge of five parallel scan-lines 
//           to form a 3x3 spatial median.
//
inline WORD Mean3x3_InRange(WORD *b1, WORD *b2, WORD *b3, WORD wMedian, WORD wRange)
{
   UINT uSum   = 0;
   UINT uCount = 0;
   int nVal;
   for (int i=0; i<3; i++)
   {
      nVal = b1[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
      nVal = b2[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
      nVal = b3[i];
      if (abs(nVal-wMedian) <= wRange)
      {
         uSum += nVal;
         uCount++;
      }
   }
   ASSERT(uCount > 0);
   return WORD(uSum/uCount);
}

/* t2(i,j) transposes elements in A[] such that A[i] <= A[j] */
#define t2(i, j) s2(A[i-1], A[j-1])

//===============================================================================================
// FUNCTION: Median25
// PURPOSE:  median25(A) partitions the array A[0..24] such that element A[12] is the median and 
//           subarrays A[0..11] and A[13..24] are partitions containing elements of smaller and 
//           larger value (rank), respectively.
// NOTES:    The exchange table lists element indices on the range 1..25, this accounts for the 
//           "-1" offsets in the macro t2 and in the final return value used to adjust subscripts 
//           to C-code conventions (array indices begin at zero).
//
inline WORD Median25(WORD A[25])
{
   t2( 1, 2); t2( 4, 5); t2( 3, 5); t2( 3, 4); t2( 7, 8);
   t2( 6, 8); t2( 6, 7); t2(10,11); t2( 9,11); t2( 9,10);
   t2(13,14); t2(12,14); t2(12,13); t2(16,17); t2(15,17);
   t2(15,16); t2(19,20); t2(18,20); t2(18,19); t2(22,23);
   t2(21,23); t2(21,22); t2(24,25); t2( 3, 6); t2( 4, 7);
   t2( 1, 7); t2( 1, 4); t2( 5, 8); t2( 2, 8); t2( 2, 5);
   t2(12,15); t2( 9,15); t2( 9,12); t2(13,16); t2(10,16);
   t2(10,13); t2(14,17); t2(11,17); t2(11,14); t2(21,24);
   t2(18,24); t2(18,21); t2(22,25); t2(19,25); t2(19,22);
   t2(20,23); t2( 9,18); t2(10,19); t2( 1,19); t2( 1,10);
   t2(11,20); t2( 2,20); t2( 2,11); t2(12,21); t2( 3,21);
   t2( 3,12); t2(13,22); t2( 4,22); t2( 4,13); t2(14,23);
   t2( 5,23); t2( 5,14); t2(15,24); t2( 6,24); t2( 6,15);
   t2(16,25); t2( 7,25); t2( 7,16); t2( 8,17); t2( 8,20);
   t2(14,22); t2(16,24); t2( 8,14); t2( 8,16); t2( 2,10);
   t2( 4,12); t2( 6,18); t2(12,18); t2(10,18); t2( 5,11);
   t2( 7,13); t2( 8,15); t2( 5, 7); t2( 5, 8); t2(13,15);
   t2(11,15); t2( 7, 8); t2(11,13); t2( 7,11); t2( 7,18);
   t2(13,18); t2( 8,18); t2( 8,11); t2(13,19); t2( 8,13);
   t2(11,19); t2(13,21); t2(11,21); t2(11,13);
   return A[13-1];
}

//===============================================================================================
// FUNCTION: MedianFilter3x3
// PURPOSE:  Run a median filter over the image.
//
BOOL MedianFilter3x3(WORD *pImage, int nWidth, int nHeight)
{
   // The image must be at least three pixels high to be filterable.
   if ((nHeight < 3) || (nWidth < 3))
      return TRUE;

   // Create a working region of three image lines.
   CArrayPtr<WORD> TempBuffer;
   if (!TempBuffer.Alloc(nWidth * 3))
      return FALSE;

   WORD *pLine[3];
   for (int i=0; i<3; i++)
      pLine[i] = TempBuffer + i * nWidth;

CHighResTimer Timer;

   // Preload the first two lines.
   memcpy(TempBuffer.Get(), pImage, 2*nWidth*sizeof(WORD));

   // Start the destination at the second image line.
   WORD *pDest = pImage + nWidth;
   WORD *pNext = pDest  + nWidth;
   for (int j=1; j<nHeight-1; j++)
   {
      // Load the next line
      memcpy(pLine[(j+1)%3], pNext, nWidth*sizeof(WORD));
      pNext += nWidth;

      // Filter this line into the source image.
      WORD *p0 = pLine[0];
      WORD *p1 = pLine[1];
      WORD *p2 = pLine[2];
      for (int i=1; i<nWidth-1; i++)
         pDest[i] = Median3x3(p0++, p1++, p2++);

      // Step on the destination pointer.
      pDest += nWidth;
   }

AXODBG_printf("Median filtered in %d ms\n", Timer.GetTimeMS());
   return TRUE;
}

//===============================================================================================
// FUNCTION: MeanFilter3x3
// PURPOSE:  Run a median filter over the image.
//
BOOL MeanFilter3x3(WORD *pImage, int nWidth, int nHeight)
{
   // The image must be at least three pixels high to be filterable.
   if ((nHeight < 3) || (nWidth < 3))
      return TRUE;

   // Create a working region of three image lines.
   CArrayPtr<WORD> TempBuffer;
   if (!TempBuffer.Alloc(nWidth * 3))
      return FALSE;

   WORD *pLine[3];
   for (int i=0; i<3; i++)
      pLine[i] = TempBuffer + i * nWidth;

CHighResTimer Timer;

   // Preload the first two lines.
   memcpy(TempBuffer.Get(), pImage, 2*nWidth*sizeof(WORD));

   // Start the destination at the second image line.
   WORD *pDest = pImage + nWidth;
   WORD *pNext = pDest  + nWidth;
   for (int j=1; j<nHeight-1; j++)
   {
      // Load the next line
      memcpy(pLine[(j+1)%3], pNext, nWidth*sizeof(WORD));
      pNext += nWidth;

      // Filter this line into the source image.
      WORD *p0 = pLine[0];
      WORD *p1 = pLine[1];
      WORD *p2 = pLine[2];
      for (int i=1; i<nWidth-1; i++)
         pDest[i] = Mean3x3(p0++, p1++, p2++);

      // Step on the destination pointer.
      pDest += nWidth;
   }

AXODBG_printf("Mean filtered in %d ms\n", Timer.GetTimeMS());
   return TRUE;
}

//===============================================================================================
// FUNCTION: MedianFilter5x5
// PURPOSE:  Run a median filter over the image.
//
BOOL MedianFilter5x5(WORD *pImage, int nWidth, int nHeight)
{
   // The image must be at least three pixels high to be filterable.
   if ((nHeight < 5) || (nWidth < 5))
      return TRUE;

   // Create a working region of five image lines.
   CArrayPtr<WORD> TempBuffer;
   if (!TempBuffer.Alloc(nWidth * 5))
      return FALSE;

   WORD *pLine[5];
   for (int i=0; i<5; i++)
      pLine[i] = TempBuffer + i * nWidth;

CHighResTimer Timer;

   // Preload the first four lines.
   memcpy(TempBuffer.Get(), pImage, 4*nWidth*sizeof(WORD));

   // Start the destination at the third image line.
   WORD *pDest = pImage + 2*nWidth;
   WORD *pNext = pDest  + 2*nWidth;
   for (int j=2; j<nHeight-2; j++)
   {
      // Load the next line
      memcpy(pLine[(j+2)%5], pNext, nWidth*sizeof(WORD));
      pNext += nWidth;

      // Filter this line into the source image.
      for (int i=2; i<nWidth-2; i++)
      {
         WORD Buffer[25];
         for (int k=0; k<5; k++)
            memcpy(Buffer+k*5, pLine[k]+i, 5*sizeof(WORD));
         pDest[i] = Median25(Buffer);
      }

      // Step on the destination pointer.
      pDest += nWidth;
   }

AXODBG_printf("Median filtered in %d ms\n", Timer.GetTimeMS());
   return TRUE;
}

//===============================================================================================
// FUNCTION: MeanFilter5x5
// PURPOSE:  Run a median filter over the image.
//
BOOL MeanFilter5x5(WORD *pImage, int nWidth, int nHeight)
{
   // The image must be at least three pixels high to be filterable.
   if ((nHeight < 5) || (nWidth < 5))
      return TRUE;

   // Create a working region of five image lines.
   CArrayPtr<WORD> TempBuffer;
   if (!TempBuffer.Alloc(nWidth * 5))
      return FALSE;

   WORD *pLine[5];
   for (int i=0; i<5; i++)
      pLine[i] = TempBuffer + i * nWidth;

CHighResTimer Timer;

   // Preload the first four lines.
   memcpy(TempBuffer.Get(), pImage, 4*nWidth*sizeof(WORD));

   // Start the destination at the third image line.
   WORD *pDest = pImage + 2*nWidth;
   WORD *pNext = pDest  + 2*nWidth;
   for (int j=2; j<nHeight-2; j++)
   {
      // Load the next line
      memcpy(pLine[(j+2)%5], pNext, nWidth*sizeof(WORD));
      pNext += nWidth;

      // Filter this line into the source image.
      WORD *p0 = pLine[0];
      WORD *p1 = pLine[1];
      WORD *p2 = pLine[2];
      WORD *p3 = pLine[3];
      WORD *p4 = pLine[4];
      for (int i=2; i<nWidth-2; i++, p0++, p1++, p2++, p3++, p4++)
         pDest[i] = Mean5x5(p0, p1, p2, p3, p4);

      // Step on the destination pointer.
      pDest += nWidth;
   }

AXODBG_printf("Mean filtered in %d ms\n", Timer.GetTimeMS());
   return TRUE;
}

//===============================================================================================
// FUNCTION: DWMTM_Filter3x3_3x3
// PURPOSE:  Run a "Double Window Modified Trimmed Mean" filter over the image.
// NOTES:    The median of a 3x3 region is used as an estimator of the mean and only points
//           within the given range of the mean are included from the 3x3 window.
//
BOOL DWMTM_Filter3x3_3x3(WORD *pImage, int nWidth, int nHeight, WORD wRange)
{
   ASSERT(wRange > 0);

   // The image must be at least three pixels high to be filterable.
   if ((nHeight < 3) || (nWidth < 3))
      return TRUE;

   // Create a working region of three image lines.
   CArrayPtr<WORD> TempBuffer;
   if (!TempBuffer.Alloc(nWidth * 3))
      return FALSE;

   WORD *pLine[3];
   for (int i=0; i<3; i++)
      pLine[i] = TempBuffer + i * nWidth;

CHighResTimer Timer;

   // Preload the first two lines.
   memcpy(TempBuffer.Get(), pImage, 2*nWidth*sizeof(WORD));

   // Start the destination at the second image line.
   WORD *pDest = pImage + nWidth;
   WORD *pNext = pDest  + nWidth;
   for (int j=1; j<nHeight-1; j++)
   {
      // Load the next line
      memcpy(pLine[(j+1)%3], pNext, nWidth*sizeof(WORD));
      pNext += nWidth;

      // Filter this line into the source image.
      WORD *p0 = pLine[0];
      WORD *p1 = pLine[1];
      WORD *p2 = pLine[2];
      for (int i=1; i<nWidth-1; i++, p0++, p1++, p2++)
      {
         WORD wMedian = Median3x3(p0, p1, p2);
         pDest[i] = Mean3x3_InRange(p0, p1, p2, wMedian, wRange);
      }

      // Step on the destination pointer.
      pDest += nWidth;
   }

AXODBG_printf("DWMTM filtered in %d ms\n", Timer.GetTimeMS());
   return TRUE;
}

//===============================================================================================
// FUNCTION: DWMTM_Filter3x3_5x5
// PURPOSE:  Run a "Double Window Modified Trimmed Mean" filter over the image.
// NOTES:    The median of a 3x3 region is used as an estimator of the mean and only points
//           within the given range of the mean are included from the 5x5 window.
//
BOOL DWMTM_Filter3x3_5x5(WORD *pImage, int nWidth, int nHeight, WORD wRange)
{
   ASSERT(wRange > 0);

   // The image must be at least five pixels high to be filterable.
   if ((nHeight < 5) || (nWidth < 5))
      return TRUE;

   // Create a working region of five image lines.
   CArrayPtr<WORD> TempBuffer;
   if (!TempBuffer.Alloc(nWidth * 5))
      return FALSE;

   WORD *pLine[5];
   for (int i=0; i<5; i++)
      pLine[i] = TempBuffer + i * nWidth;

CHighResTimer Timer;

   // Preload the first four lines.
   memcpy(TempBuffer.Get(), pImage, 4*nWidth*sizeof(WORD));

   // Start the destination at the third image line.
   WORD *pDest = pImage + 2*nWidth;
   WORD *pNext = pDest  + 2*nWidth;
   for (int j=2; j<nHeight-2; j++)
   {
      // Load the next line
      memcpy(pLine[(j+2)%5], pNext, nWidth*sizeof(WORD));
      pNext += nWidth;

      // Filter this line into the source image.
      WORD *p0 = pLine[0];
      WORD *p1 = pLine[1];
      WORD *p2 = pLine[2];
      WORD *p3 = pLine[3];
      WORD *p4 = pLine[4];
      for (int i=2; i<nWidth-2; i++, p0++, p1++, p2++, p3++, p4++)
      {
         WORD wMedian = Median3x3(p1+1, p2+1, p3+1);
         pDest[i] = Mean5x5_InRange(p0, p1, p2, p3, p4, wMedian, wRange);
      }

      // Step on the destination pointer.
      pDest += nWidth;
   }

AXODBG_printf("DWMTM filtered in %d ms\n", Timer.GetTimeMS());
   return TRUE;
}

