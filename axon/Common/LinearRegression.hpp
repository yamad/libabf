//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  LinearRegression.HPP
// PURPOSE: Contains templated function for linear regression of a set of points.
// AUTHOR:  BHI  Aug 1998
//

#ifndef INC_LINEARREGRESSION_HPP
#define INC_LINEARREGRESSION_HPP

#pragma once
#include <math.h>

//##########################################################################################
//##########################################################################################
//###
//###  Simple Linear Regression
//###
//##########################################################################################
//##########################################################################################

#if 1
//===============================================================================================
// FUNCTION: LinearRegression
// PURPOSE:  Given a set of data points x[0..ndata-1],y[0..ndata-1], 
//           fit them to a straight line "y = ax + b" by minimizing the sum of the squares.
//
// RETURNS:  a,b and the correlation coefficient R²
//
// SOURCE:   C/C++ Mathematical Algorithms for Scientists and Engineers.
//           by Namir C. Shammas  (McGraw Hill)
//
template <class T>
bool LinearRegression(T x[], T y[], int nData, double *pdA, double *pdB, double *pdR2)
{
   ASSERT(nData > 1);
   if (nData < 2)
      return false;

   double dSumX=0, dSumY=0, dSumXX=0, dSumYY=0, dSumXY=0;
   for (int i=0; i<nData; i++)
   {
      double dX = x[i];
      double dY = y[i];
      dSumX    += dX;
      dSumY    += dY;
      dSumXX   += dX*dX;
      dSumYY   += dY*dY;
      dSumXY   += dX*dY;
   }
   double dMeanX = dSumX / nData;
   double dMeanY = dSumY / nData;

   double dSdevX = sqrt((dSumXX - (dSumX*dSumX) / nData) / (nData - 1));
   double dSdevY = sqrt((dSumYY - (dSumY*dSumY) / nData) / (nData - 1));

   double dSlope = 0, dIntercept = 0, dR = 0;
   if (dSdevY == 0)
   {
      dSlope = dR = 0;
      dIntercept  = dMeanY;
   }
   else if (dSdevX == 0)
   {
      dSlope = dIntercept = 1/dSdevX;  // return infinity!
      dR     = 0;
   }
   else
   {
//    dSlope     = (dSumXY - dMeanX * dMeanY * nData) / (dSdevX * dSdevX) / (nData - 1);
      dSlope     = (dSumXY * nData - dSumX * dSumY) / (dSumXX * nData - dSumX*dSumX);
      dIntercept = dMeanY - dSlope * dMeanX;
      dR         = dSlope * dSdevX / dSdevY;
   }
   double dR2 = dR * dR;

   *pdA  = dSlope;
   *pdB  = dIntercept;
   *pdR2 = dR2;
   return true;
}

#else
//===============================================================================================
// FUNCTION: LinearRegression
// PURPOSE:  Given a set of data points x[0..ndata-1],y[0..ndata-1], 
//           fit them to a straight line "y = ax + b" by minimizing Chi2.
//
// RETURNS:  a,b and their respective probable uncertainties siga and sigb, and
//           the chi-square chi2. 
//
// SOURCE:   Numerical Recipes Chapter 14.2 - Fitting data to a straight line.
//
template <class T>
void LinearRegression(T x[], T y[], int nData, double *pdA, double *pdB, double *pdChi2)
{
   int i;

   double sx=0.0, sy=0.0;
   for (i=0; i<nData; i++)
   {
      sx += x[i];
      sy += y[i];
   }
   double sxoss = sx / nData;
   double st2   = 0.0;
   double dA    = 0.0;
   for (i=0; i<nData; i++)
   {
      double t = x[i]-sxoss;
      st2     += t * t;
      dA      += t * y[i];
   }
   dA /= st2; // Solve for a, b, siga, and sigb.
   
   double dB = (sy - sx * dA)/nData;

   double dSigB = sqrt((1.0+sx*sx/(nData*st2))/nData);
   double dSigA = sqrt(1.0/st2);
   
   double dChi2 = 0.0; // Calculate Chi-square.
   for (i=0; i<nData; i++)
   {
      double dDiff = y[i] - (x[i] * dA + dB);
      dChi2 += dDiff * dDiff;
   }

   // Evaluate typical sig using chi2, and adjust the standard deviations.
   if (nData > 2)
   {
      double dSigdat = sqrt(dChi2/(nData-2));
      dSigA *= dSigdat;
      dSigB *= dSigdat;
   }

   *pdA    = dA;
   *pdB    = dB;
   *pdChi2 = dChi2;
}
#endif

//##########################################################################################
//##########################################################################################
//###
//###  Symmetric Linear Regression
//###
//##########################################################################################
//##########################################################################################

// [Bruce Ikin]
// One of the columns of output data the we produce from GenePix is a 
// "regression ratio" generated from a linear regression of the pixel 
// distribution created by plotting the response from the 523 nm laser 
// against the 635 nm laser.
// 
// Recently it was brought to my attention that the standard linear 
// regression algorithm that I had been using was not symmetric when 
// the X & Y axes were swapped. i.e. the ratio calculated for 532/635 
// was not the reciprocal of that calculated for 635/532.
// 
// This is because the standard algorithm only minimises the errors in 
// the Y measurements (assuming that X is "known" and Y is "sampled"). 
// In our case both axes consist of sampled data, so there is no reason 
// to measure the errors in one axis in preference to the other.

// [Michael Buckley]
// The problem is essentially one of Principal Components in two
// dimensions. The first principal component corresponds to a direction
// in which variation (as measured by variance) is maximised, while the
// second PC is the direction in which variation is minimised. This is
// exactly the "orthogonal regression" problem which is the problem of
// finding a line through the data points minimising the sum of squares
// of distances of the points from the line.
// 
// Principal components in general is an eigen-decomposition problem in
// terms of computation of the solution, but in the two dimensional case
// the solution can be written explicitly. Let R and G be the given spot
// values, then compute sums, sums of squares and sum of R times G:
// 
//     n = length(R)
//     sR = sum(R)
//     sRR = sum(R*R)
//     sG = sum(G)
//     sGG = sum(G*G)
//     sRG = sum(R*G)
// 
// Correct to get variances and covariances:
// 
//     vRR = sRR - sR*sR/n
//     vGG = sGG - sG*sG/n
//     vRG = sRG - sR*sG/n
// 
// First check vRG != 0. If this is zero there is zero correlation
// between R and G and all lines through the centroid of the data cloud
// are equally good.
// 
// The slope of the line is now
// 
//     v = sqrt(1 + T*T) - T                            (1)
// 
// where
// 
//     T = (vRR - vGG)/(2*vRG).
// 
// It may be verified that swapping R and G inverts v ... If R and G are
// swapped, vRG is unaffected while vRR and vGG are swapped. This changes
// the sign of T. If v' is the new slope we have
// 
//     v' = sqrt(1 + T*T) + T.
// 
// We would expect v' = 1/v; i.e. v * v' = 1. But
// 
//     v * v' = [sqrt(1 + T*T) - T] * [sqrt(1 + T*T) + T]
//            = [A - T] * [A + T]               where  A = sqrt(1 + T*T)
//            = A*A - T*T
//            = 1 + T*T - T*T
//            = 1.
// 
// With Mark's help I found a solution to this problem in Kendall and
// Stuart, an old but very good three-volume stats book ("The Advanced
// Theory of Statistics", Griffin, Second edition, 1967). Equation (29.29)
// on page 381 of volume 2  is equivalent to (1) above, although in
// different notation.
// 
// R^2 (coefficient of determination)
// ==================================
// The formula you should use, I think, is
// 
//     R^2 = (vRG)^2 / (vRR * vGG)
// 
// where the terms are defined as in my earlier email -- i.e. corrected sums
// of squares and products.
// 
// This is the standard value for regression of G on R, but you can see that
// you get the same if you exchange G and R. In other words, as I said on the
// phone, the two versions of standard regression -- i.e. of G on R, or of R
// on G -- give the same value for R^2 even though they give different slopes
// (vRG/vRR and vRG/vGG).
// 
// The formula I sent last week for a symmetric slope estimate doesn't
// directly link to this standard R^2. I did some calculations to work out a
// number for this -- i.e.  corresponding R^2 for the symmetric slope estimate
// -- but it didn't turn out to have a simple formula. In any case it would
// seem sensible to use the standard form, particularly as it is symmetric in
// R ang G.
// 
//  Michael Buckley
//  CSIRO Mathematical and Information Sciences
//  Locked Bag 17, North Ryde, NSW 2113
//  Phone:+61 2 9325 3209, Fax:+61 2 9325 3200
//  Michael.Buckley@cmis.csiro.au


//===============================================================================================
// FUNCTION: SymmetricLinearRegression
// PURPOSE:  Given a set of data points x[0..ndata-1],y[0..ndata-1], 
//           fit them to a straight line "y = ax + b" by minimizing the sum of the squares.
//
// RETURNS:  a,b and the correlation coefficient R²
//
// SOURCE:   Michael Buckley
//           CSIRO Mathematical and Information Sciences
//           Locked Bag 17, North Ryde, NSW 2113
//           Phone:+61 2 9325 3209, Fax:+61 2 9325 3200
//           Michael.Buckley@cmis.csiro.au
//
template <class T>
bool SymmetricLinearRegression(T x[], T y[], int nData, double *pdA, double *pdB, double *pdR2)
{
   ASSERT(nData > 1);
   if (nData < 2)
      return false;

   // Compute sums, sums of squares and sum of X times Y
   double dSumX=0, dSumY=0, dSumXX=0, dSumYY=0, dSumXY=0;
   for (int i=0; i<nData; i++)
   {
      double dX = x[i];
      double dY = y[i];
      dSumX    += dX;
      dSumY    += dY;
      dSumXX   += dX*dX;
      dSumYY   += dY*dY;
      dSumXY   += dX*dY;
   }

   // Compute means.
   double dMeanX = dSumX / nData;
   double dMeanY = dSumY / nData;

   // Compute variances and covariances:
   double dVarianceX  = dSumXX - dSumX*dSumX/nData;
   double dVarianceY  = dSumYY - dSumY*dSumY/nData;
   double dCovariance = dSumXY - dSumX*dSumY/nData;

   // Check for special cases (vertical, horizontal, symmetric cloud)
   double dSlope = 0, dIntercept = 0, dR2 = 0;
   if ((dVarianceY == 0) || (dCovariance == 0))
   {
      dSlope = dR2 = 0;
      dIntercept   = dMeanY;
   }
   else if (dVarianceX == 0)
   {
      dSlope = dIntercept = 1/dVarianceX;  // return infinity!
      dR2    = 0;
   }
   else
   {
      ASSERT(dCovariance != 0);
      double T   = (dVarianceX - dVarianceY)/(2*dCovariance);
      dSlope     = sqrt(1 + T*T) - T;

      dIntercept = dMeanY - dSlope * dMeanX;
      dR2        = dCovariance*dCovariance / (dVarianceX * dVarianceY);
   }

   *pdA  = dSlope;
   *pdB  = dIntercept;
   *pdR2 = dR2;
   return true;
}

#endif      // INC_LINEARREGRESSION_HPP
