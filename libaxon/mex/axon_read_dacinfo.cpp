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
 *  \brief MEX file containing ReadDACInfo wrapper
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
	const char *k[] = { "nDACNum",
						//
						"nTelegraphDACScaleFactorEnable",
						"fInstrumentHoldingLevel",
						//
						"fDACScaleFactor",
						"fDACHoldingLevel",
						"fDACCalibrationFactor",
						"fDACCalibrationOffset",
						//
						"lDACChannelNameIndex",
						"lDACChannelUnitsIndex",
						//
						"lDACFilePtr",
						"lDACFileNumEpisodes",
						//
						"nWaveformEnable",
						"nWaveformSource",
						"nInterEpisodeLevel",
						//
						"fDACFileScale",
						"fDACFileOffset",
						"lDACFileEpisodeNum",
						"nDACFileADCNum",
						//
						"nConditEnable",
						"lConditNumPulses",
						"fBaselineDuration",
						"fBaselineLevel",
						"fStepDuration",
						"fStepLevel",
						"fPostTrainPeriod",
						"fPostTrainLevel",
						"nMembTestEnable",
						//
						"nLeakSubtractType",
						"nPNPolarity",
						"fPNHoldingLevel",
						"nPNNumADCChannels",
						"nPNPosition",
						"nPNNumPulses",
						"fPNSettlingTime",
						"fPNInterpulse",
						//
						"nLTPUsageOfDAC",
						"nLTPPresynapticPulses",
						//
						"lDACFilePathIndex",
						//
						"fMembTestPreSettlingTimeMS",
						"fMembTestPostSettlingTimeMS",
						//
						"nLeakSubtractADCIndex"};

    if (nrhs != 1)
	   mexErrMsgTxt("File name expected!");

    fname = mxArrayToString(prhs[0]);

    ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	ret = abf.ReadDACInfo();
	abf.Close();

	ne = abf.FileInfo.DACSection.llNumEntries;
	a = mxCreateStructMatrix(ne, 1, 41, k);

	for(int i = 0; i < (int)ne; i++) {
		mxSetFieldByNumber(a, i, 0, mxCreateDoubleScalar((double) abf.DACInfo[i].nDACNum));
		//
		mxSetFieldByNumber(a, i, 1, mxCreateDoubleScalar((double) abf.DACInfo[i].nTelegraphDACScaleFactorEnable));
		mxSetFieldByNumber(a, i, 2, mxCreateDoubleScalar((double) abf.DACInfo[i].fInstrumentHoldingLevel));
		//
		mxSetFieldByNumber(a, i, 3, mxCreateDoubleScalar((double) abf.DACInfo[i].fDACScaleFactor));
		mxSetFieldByNumber(a, i, 4, mxCreateDoubleScalar((double) abf.DACInfo[i].fDACHoldingLevel));
		mxSetFieldByNumber(a, i, 5, mxCreateDoubleScalar((double) abf.DACInfo[i].fDACCalibrationFactor));
		mxSetFieldByNumber(a, i, 6, mxCreateDoubleScalar((double) abf.DACInfo[i].fDACCalibrationOffset));
		//
		mxSetFieldByNumber(a, i, 7, mxCreateDoubleScalar((double) abf.DACInfo[i].lDACChannelNameIndex));
		mxSetFieldByNumber(a, i, 8, mxCreateDoubleScalar((double) abf.DACInfo[i].lDACChannelUnitsIndex));
		//
		mxSetFieldByNumber(a, i, 9, mxCreateDoubleScalar((double) abf.DACInfo[i].lDACFilePtr));
		mxSetFieldByNumber(a, i, 10, mxCreateDoubleScalar((double) abf.DACInfo[i].lDACFileNumEpisodes));
		//
		mxSetFieldByNumber(a, i, 11, mxCreateDoubleScalar((double) abf.DACInfo[i].nWaveformEnable));
		mxSetFieldByNumber(a, i, 12, mxCreateDoubleScalar((double) abf.DACInfo[i].nWaveformSource));
		mxSetFieldByNumber(a, i, 13, mxCreateDoubleScalar((double) abf.DACInfo[i].nInterEpisodeLevel));
		//
		mxSetFieldByNumber(a, i, 14, mxCreateDoubleScalar((double) abf.DACInfo[i].fDACFileScale));
		mxSetFieldByNumber(a, i, 15, mxCreateDoubleScalar((double) abf.DACInfo[i].fDACFileOffset));
		mxSetFieldByNumber(a, i, 16, mxCreateDoubleScalar((double) abf.DACInfo[i].lDACFileEpisodeNum));
		mxSetFieldByNumber(a, i, 17, mxCreateDoubleScalar((double) abf.DACInfo[i].nDACFileADCNum));
		//
		mxSetFieldByNumber(a, i, 18, mxCreateDoubleScalar((double) abf.DACInfo[i].nConditEnable));
		mxSetFieldByNumber(a, i, 19, mxCreateDoubleScalar((double) abf.DACInfo[i].lConditNumPulses));
		mxSetFieldByNumber(a, i, 20, mxCreateDoubleScalar((double) abf.DACInfo[i].fBaselineDuration));
		mxSetFieldByNumber(a, i, 21, mxCreateDoubleScalar((double) abf.DACInfo[i].fBaselineLevel));
		mxSetFieldByNumber(a, i, 22, mxCreateDoubleScalar((double) abf.DACInfo[i].fStepDuration));
		mxSetFieldByNumber(a, i, 23, mxCreateDoubleScalar((double) abf.DACInfo[i].fStepLevel));
		mxSetFieldByNumber(a, i, 24, mxCreateDoubleScalar((double) abf.DACInfo[i].fPostTrainPeriod));
		mxSetFieldByNumber(a, i, 25, mxCreateDoubleScalar((double) abf.DACInfo[i].fPostTrainLevel));
		mxSetFieldByNumber(a, i, 26, mxCreateDoubleScalar((double) abf.DACInfo[i].nMembTestEnable));
		//
		mxSetFieldByNumber(a, i, 27, mxCreateDoubleScalar((double) abf.DACInfo[i].nLeakSubtractType));
		mxSetFieldByNumber(a, i, 28, mxCreateDoubleScalar((double) abf.DACInfo[i].nPNPolarity));
		mxSetFieldByNumber(a, i, 29, mxCreateDoubleScalar((double) abf.DACInfo[i].fPNHoldingLevel));
		mxSetFieldByNumber(a, i, 30, mxCreateDoubleScalar((double) abf.DACInfo[i].nPNNumADCChannels));
		mxSetFieldByNumber(a, i, 31, mxCreateDoubleScalar((double) abf.DACInfo[i].nPNPosition));
		mxSetFieldByNumber(a, i, 32, mxCreateDoubleScalar((double) abf.DACInfo[i].nPNNumPulses));
		mxSetFieldByNumber(a, i, 33, mxCreateDoubleScalar((double) abf.DACInfo[i].fPNSettlingTime));
		mxSetFieldByNumber(a, i, 34, mxCreateDoubleScalar((double) abf.DACInfo[i].fPNInterpulse));
		//
		mxSetFieldByNumber(a, i, 35, mxCreateDoubleScalar((double) abf.DACInfo[i].nLTPUsageOfDAC));
		mxSetFieldByNumber(a, i, 36, mxCreateDoubleScalar((double) abf.DACInfo[i].nLTPPresynapticPulses));
		//
		mxSetFieldByNumber(a, i, 37, mxCreateDoubleScalar((double) abf.DACInfo[i].lDACFilePathIndex));
		//
		mxSetFieldByNumber(a, i, 38, mxCreateDoubleScalar((double) abf.DACInfo[i].fMembTestPreSettlingTimeMS));
		mxSetFieldByNumber(a, i, 39, mxCreateDoubleScalar((double) abf.DACInfo[i].fMembTestPostSettlingTimeMS));
		//
		mxSetFieldByNumber(a, i, 40, mxCreateDoubleScalar((double) abf.DACInfo[i].nLeakSubtractADCIndex));
	}

	plhs[0] = a;
}
