//
// This file is part of the Axon Library.
//
// Copyright (c) 2008-2009 Jakub Nowacki
//
// The Axon Binary Format is property of Molecular Devices.
// All rights to the Axon Binary Format are reserved to Molecular Devices.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA
//

/*! \file
 *  \brief MEX file containing ReadADCInfo wrapper
 */


#include <stdio.h>
#include "axon.h"
#include "mex.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	ABF abf = ABF();
	mxArray *a;
	mwSize ne;
	char *fname;
	int ret;
	// Keys of structure ABF_ADCInfo
	const char *k[] = { "nADCNum",
						//
						"nTelegraphEnable",
						"nTelegraphInstrument",
						"fTelegraphAdditGain",
						"fTelegraphFilter",
						"fTelegraphMembraneCap",
						"nTelegraphMode",
						"fTelegraphAccessResistance",
						//
						"nADCPtoLChannelMap",
						"nADCSamplingSeq",
						//
						"fADCProgrammableGain",
						"fADCDisplayAmplification",
						"fADCDisplayOffset",
						"fInstrumentScaleFactor",
						"fInstrumentOffset",
						"fSignalGain",
						"fSignalOffset",
						"fSignalLowpassFilter",
						"fSignalHighpassFilter",
						//
						"nLowpassFilterType",
						"nHighpassFilterType",
						"fPostProcessLowpassFilter",
						"nPostProcessLowpassFilterType",
						"bEnabledDuringPN",
						//
						"nStatsChannelPolarity",
						//
						"lADCChannelNameIndex",
						"lADCUnitsIndex"};

    if (nrhs != 1)
	   mexErrMsgTxt("File name expected!");

    fname = mxArrayToString(prhs[0]);

    ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	ret = abf.ReadADCInfo();
	abf.Close();

	ne = abf.FileInfo.ADCSection.llNumEntries;
	a = mxCreateStructMatrix(ne, 1, 27, k);

	for(int i = 0; i < (int)ne; i++) {
		mxSetFieldByNumber(a, i, 0, mxCreateDoubleScalar((double) abf.ADCInfo[i].nADCNum));
		//
		mxSetFieldByNumber(a, i, 1, mxCreateDoubleScalar((double) abf.ADCInfo[i].nTelegraphEnable));
		mxSetFieldByNumber(a, i, 2, mxCreateDoubleScalar((double) abf.ADCInfo[i].nTelegraphInstrument));
		mxSetFieldByNumber(a, i, 3, mxCreateDoubleScalar((double) abf.ADCInfo[i].fTelegraphAdditGain));
		mxSetFieldByNumber(a, i, 4, mxCreateDoubleScalar((double) abf.ADCInfo[i].fTelegraphFilter));
		mxSetFieldByNumber(a, i, 5, mxCreateDoubleScalar((double) abf.ADCInfo[i].fTelegraphMembraneCap));
		mxSetFieldByNumber(a, i, 6, mxCreateDoubleScalar((double) abf.ADCInfo[i].nTelegraphMode));
		mxSetFieldByNumber(a, i, 7, mxCreateDoubleScalar((double) abf.ADCInfo[i].fTelegraphAccessResistance));
		//
		mxSetFieldByNumber(a, i, 8, mxCreateDoubleScalar((double) abf.ADCInfo[i].nADCPtoLChannelMap));
		mxSetFieldByNumber(a, i, 9, mxCreateDoubleScalar((double) abf.ADCInfo[i].nADCSamplingSeq));
		//
		mxSetFieldByNumber(a, i, 10, mxCreateDoubleScalar((double) abf.ADCInfo[i].fADCProgrammableGain));
		mxSetFieldByNumber(a, i, 11, mxCreateDoubleScalar((double) abf.ADCInfo[i].fADCDisplayAmplification));
		mxSetFieldByNumber(a, i, 12, mxCreateDoubleScalar((double) abf.ADCInfo[i].fADCDisplayOffset));
		mxSetFieldByNumber(a, i, 13, mxCreateDoubleScalar((double) abf.ADCInfo[i].fInstrumentScaleFactor));
		mxSetFieldByNumber(a, i, 14, mxCreateDoubleScalar((double) abf.ADCInfo[i].fInstrumentOffset));
		mxSetFieldByNumber(a, i, 15, mxCreateDoubleScalar((double) abf.ADCInfo[i].fSignalGain));
		mxSetFieldByNumber(a, i, 16, mxCreateDoubleScalar((double) abf.ADCInfo[i].fSignalOffset));
		mxSetFieldByNumber(a, i, 17, mxCreateDoubleScalar((double) abf.ADCInfo[i].fSignalLowpassFilter));
		mxSetFieldByNumber(a, i, 18, mxCreateDoubleScalar((double) abf.ADCInfo[i].fSignalHighpassFilter));
		//
		mxSetFieldByNumber(a, i, 19, mxCreateDoubleScalar((double) abf.ADCInfo[i].nLowpassFilterType));
		mxSetFieldByNumber(a, i, 20, mxCreateDoubleScalar((double) abf.ADCInfo[i].nHighpassFilterType));
		mxSetFieldByNumber(a, i, 21, mxCreateDoubleScalar((double) abf.ADCInfo[i].fPostProcessLowpassFilter));
		mxSetFieldByNumber(a, i, 22, mxCreateDoubleScalar((double) abf.ADCInfo[i].nPostProcessLowpassFilterType));
		mxSetFieldByNumber(a, i, 23, mxCreateDoubleScalar((double) abf.ADCInfo[i].bEnabledDuringPN));
		//
		mxSetFieldByNumber(a, i, 24, mxCreateDoubleScalar((double) abf.ADCInfo[i].nStatsChannelPolarity));
		//
		mxSetFieldByNumber(a, i, 25, mxCreateDoubleScalar((double) abf.ADCInfo[i].lADCChannelNameIndex));
		mxSetFieldByNumber(a, i, 26, mxCreateDoubleScalar((double) abf.ADCInfo[i].lADCUnitsIndex));
	}

	plhs[0] = a;
}
