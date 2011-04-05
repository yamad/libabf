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
 *  \brief MEX file containing ReadStrings wrapper
 */

#include <stdio.h>
#include <string.h>
#include "axon.h"
#include "mex.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	ABF abf = ABF();
	mxArray *a;
	char *fname, **str;
	int ret, ne;

	if (nrhs != 1)
	   mexErrMsgTxt("File name expected!");

	fname = mxArrayToString(prhs[0]);

	ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	ret = abf.ReadStrings();

	ne = abf.FileInfo.StringsSection.llNumEntries;
	str = (char **) mxMalloc(ne*sizeof(char *));
	for(int i = 0; i < ne; i++) {
		str[i] = (char *) mxMalloc(ABF_PATHLEN*sizeof(char));
		abf.GetString(str[i], i+1);
	}


	abf.Close();

	a = mxCreateCharMatrixFromStrings(ne, (const char **)str);

	for(int i = 0; i < ne; i++)
		mxFree(str[i]);
	mxFree(str);

	plhs[0] = a;
}
