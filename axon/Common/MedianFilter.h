//***********************************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  MedianFilter.H
// PURPOSE: Contains routines to filter an image using a median filter.
//

#ifndef INC_MEDIANFILTER_HPP
#define INC_MEDIANFILTER_HPP

#pragma once

BOOL MedianFilter3x3(WORD *pImage, int nWidth, int nHeight);

BOOL MeanFilter3x3(WORD *pImage, int nWidth, int nHeight);

BOOL MedianFilter5x5(WORD *pImage, int nWidth, int nHeight);

BOOL MeanFilter5x5(WORD *pImage, int nWidth, int nHeight);

BOOL DWMTM_Filter3x3_3x3(WORD *pImage, int nWidth, int nHeight, WORD wRange);

BOOL DWMTM_Filter3x3_5x5(WORD *pImage, int nWidth, int nHeight, WORD wRange);

#endif      // INC_MEDIANFILTER_HPP
