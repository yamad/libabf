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
 *  \brief MEX file containing ReadFileInfo wrapper
 */

#include <stdio.h>
#include <string.h>
#include "axon.h"
#include "mex.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	ABF abf = ABF();
	mxArray *a, *s, *n;
	char *fname;
	int ret;
	// Keys of structure MYGUID keys
	const char *kg[] = {"Data1", "Data2", "Data3", "Data4"};
	// Keys of structure ABF_Section keys
	const char *ks[] = { "BlockIndex", "uBytes", "llNumEntries"};
	// Keys of structure ABF_FileInfo
	const char *kf[] = { "uFileSignature",
						 "uFileVersionNumber",
						 //
						 "uFileInfoSize",
						 //
						 "uActualEpisodes",
						 "uFileStartDate",
						 "uFileStartTimeMS",
						 "uStopwatchTime",
						 "nFileType",
						 "nDataFormat",
						 "nSimultaneousScan",
						 "nCRCEnable",
						 "uFileCRC",
						 //
						 "FileGUID",
						 //
						 "uCreatorVersion",
						 "uCreatorNameIndex",
						 "uModifierVersion",
						 "uModifierNameIndex",
						 "uProtocolPathIndex",
						 //
						 "ProtocolSection",
						 "ADCSection",
						 "DACSection",
						 "EpochSection",
						 "ADCPerDACSection",
						 "EpochPerDACSection",
						 "UserListSection",
						 "StatsRegionSection",
						 "MathSection",
						 "StringsSection",
						 //
						 "DataSection",
						 "TagSection",
						 "ScopeSection",
						 "DeltaSection",
						 "VoiceTagSection",
						 "SynchArraySection",
						 "AnnotationSection",
						 "StatsSection"};

    if (nrhs != 1)
	   mexErrMsgTxt("File name expected!");

    fname = mxArrayToString(prhs[0]);

    // Reading from abf file
	ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	abf.Close();

    a = mxCreateStructMatrix(1, 1, 36, kf);

    // Packing values into structure matrix
	mxSetFieldByNumber(a, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.uFileSignature));
	mxSetFieldByNumber(a, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.uFileVersionNumber));
	//
	mxSetFieldByNumber(a, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.uFileInfoSize));
	//
	mxSetFieldByNumber(a, 0, 3, mxCreateDoubleScalar((double) abf.FileInfo.uActualEpisodes));
	mxSetFieldByNumber(a, 0, 4, mxCreateDoubleScalar((double) abf.FileInfo.uFileStartDate));
	mxSetFieldByNumber(a, 0, 5, mxCreateDoubleScalar((double) abf.FileInfo.uFileStartTimeMS));
	mxSetFieldByNumber(a, 0, 6, mxCreateDoubleScalar((double) abf.FileInfo.uStopwatchTime));
	mxSetFieldByNumber(a, 0, 7, mxCreateDoubleScalar((double) abf.FileInfo.nFileType));
	mxSetFieldByNumber(a, 0, 8, mxCreateDoubleScalar((double) abf.FileInfo.nDataFormat));
	mxSetFieldByNumber(a, 0, 9, mxCreateDoubleScalar((double) abf.FileInfo.nSimultaneousScan));
	mxSetFieldByNumber(a, 0, 10, mxCreateDoubleScalar((double) abf.FileInfo.nCRCEnable));
	mxSetFieldByNumber(a, 0, 11, mxCreateDoubleScalar((double) abf.FileInfo.uFileCRC));
	// struct MYGUID
	s = mxCreateStructMatrix(1, 1, 4, kg);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.FileGUID.Data1));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.FileGUID.Data2));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.FileGUID.Data3));
	n = mxCreateNumericMatrix(1, 8, mxUINT8_CLASS, mxREAL);
	memcpy(mxGetPr(n), abf.FileInfo.FileGUID.Data4, 8*sizeof(char));
	mxSetFieldByNumber(s, 0, 3, n);
	mxSetFieldByNumber(a, 0, 12, s);
	//
	mxSetFieldByNumber(a, 0, 13, mxCreateDoubleScalar((double) abf.FileInfo.uCreatorVersion));
	mxSetFieldByNumber(a, 0, 14, mxCreateDoubleScalar((double) abf.FileInfo.uCreatorNameIndex));
	mxSetFieldByNumber(a, 0, 15, mxCreateDoubleScalar((double) abf.FileInfo.uModifierVersion));
	mxSetFieldByNumber(a, 0, 16, mxCreateDoubleScalar((double) abf.FileInfo.uModifierNameIndex));
	mxSetFieldByNumber(a, 0, 17, mxCreateDoubleScalar((double) abf.FileInfo.uProtocolPathIndex));
	// struct ABF_Section (ProtocolSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.ProtocolSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.ProtocolSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.ProtocolSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 18, s);
	// struct ABF_Section (ADCSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.ADCSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.ADCSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.ADCSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 19, s);
	// struct ABF_Section (DACSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.DACSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.DACSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.DACSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 20, s);
	// struct ABF_Section (EpochSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.EpochSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.EpochSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.EpochSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 21, s);
	// struct ABF_Section (ADCPerDACSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.ADCPerDACSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.ADCPerDACSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.ADCPerDACSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 22, s);
	// struct ABF_Section (EpochPerDACSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.EpochPerDACSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.EpochPerDACSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.EpochPerDACSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 23, s);
	// struct ABF_Section (UserListSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.UserListSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.UserListSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.UserListSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 24, s);
	// struct ABF_Section (StatsRegionSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.StatsRegionSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.StatsRegionSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.StatsRegionSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 25, s);
	// struct ABF_Section (MathSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.MathSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.MathSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.MathSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 26, s);
	// struct ABF_Section (StringsSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.StringsSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.StringsSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.StringsSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 27, s);
	// struct ABF_Section (DataSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.DataSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.DataSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.DataSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 28, s);
	// struct ABF_Section (TagSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.TagSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.TagSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.TagSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 29, s);
	// struct ABF_Section (ScopeSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.ScopeSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.ScopeSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.ScopeSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 30, s);
	// struct ABF_Section (DeltaSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.DeltaSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.DeltaSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.DeltaSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 31, s);
	// struct ABF_Section (VoiceTagSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.VoiceTagSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.VoiceTagSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.VoiceTagSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 32, s);
	// struct ABF_Section (SynchArraySection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.SynchArraySection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.SynchArraySection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.SynchArraySection.llNumEntries));
	mxSetFieldByNumber(a, 0, 33, s);
	// struct ABF_Section (AnnotationSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.AnnotationSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.AnnotationSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.AnnotationSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 34, s);
	// struct ABF_Section (StatsSection)
	s = mxCreateStructMatrix(1, 1, 3, ks);
	mxSetFieldByNumber(s, 0, 0, mxCreateDoubleScalar((double) abf.FileInfo.StatsSection.uBlockIndex));
	mxSetFieldByNumber(s, 0, 1, mxCreateDoubleScalar((double) abf.FileInfo.StatsSection.uBytes));
	mxSetFieldByNumber(s, 0, 2, mxCreateDoubleScalar((double) abf.FileInfo.StatsSection.llNumEntries));
	mxSetFieldByNumber(a, 0, 35, s);

	plhs[0] = a;
}
