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
 *  \brief MEX file containing ReadEpochInfoPerDAC wrapper
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
	// Keys of structure ABF_EpochInfoPerDAC
	const char *k[] = { "nEpochNum",
						"nDACNum",
						//
						"nEpochType",
						"fEpochInitLevel",
						"fEpochLevelInc",
						"lEpochInitDuration",
						"lEpochDurationInc",
						"lEpochPulsePeriod",
						"lEpochPulseWidth"};

    if (nrhs != 1)
	   mexErrMsgTxt("File name expected!");

    fname = mxArrayToString(prhs[0]);

    ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	ret = abf.ReadEpochInfoPerDAC();
	abf.Close();

	//ne = abf.FileInfo.EpochSection.llNumEntries;
	a = mxCreateStructMatrix(ABF_EPOCHCOUNT, ABF_DACCOUNT, 9, k);

	for(int j = 0; j < ABF_DACCOUNT; j++) {
		for(int i = 0; i < ABF_EPOCHCOUNT; i++){
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 0, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].nEpochNum));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 1, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].nDACNum));
			//
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 2, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].nEpochType));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 3, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].fEpochInitLevel));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 4, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].fEpochLevelInc));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 5, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].lEpochInitDuration));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 6, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].lEpochDurationInc));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 7, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].lEpochPulsePeriod));
			mxSetFieldByNumber(a, i+j*ABF_EPOCHCOUNT, 8, mxCreateDoubleScalar((double) abf.EpochInfoPerDAC[j][i].lEpochPulseWidth));
		}
	}

	plhs[0] = a;
}
