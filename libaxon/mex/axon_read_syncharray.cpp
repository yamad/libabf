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
 *  \brief MEX file containing ReadData wrapper
 */

#include <stdio.h>
#include "axon.h"
#include "mex.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	mwSize ne;
	mxArray *a;
	double *p;
	ABF abf = ABF();
	char *fname;
	int ret;
	char as_struct = 0;
	// Keys of structure ABF_SynchArray
	const char *k[] = { "lStart",
						"lLength"};

	if (nrhs > 2)
		   mexErrMsgTxt("Too many parameters!");

	if (nrhs < 1)
	   mexErrMsgTxt("File name expected!");


	fname = mxArrayToString(prhs[0]);

	if (nrhs == 2)
		if(mxGetScalar(prhs[1]) != 0)
			as_struct = 1;


	ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	ret = abf.ReadSynchArray();

	ne = abf.FileInfo.SynchArraySection.llNumEntries;

	// Returning SynchArray as an array of struct
	if (as_struct)
	{
		a = mxCreateStructMatrix(ne, 1, 2, k);
		for (int i = 0; i < ne; i++) {
			mxSetFieldByNumber(a, i, 0, mxCreateDoubleScalar((double) abf.SynchArray[i].lStart));
			mxSetFieldByNumber(a, i, 1, mxCreateDoubleScalar((double) abf.SynchArray[i].lLength));
		}
	}
	// Returning SynchArray as a numeric array
	else
	{
		a = mxCreateDoubleMatrix(ne, 2 , mxREAL);
		p = mxGetPr(a);
		for (int i = 0; i < ne; i++) {
			p[i] = (double) abf.SynchArray[i].lStart;
			p[i+ne] = (double) abf.SynchArray[i].lLength;
		}

	}

	abf.Close();

	plhs[0] = a;
}
