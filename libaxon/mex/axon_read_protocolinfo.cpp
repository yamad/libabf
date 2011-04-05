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
 *  \brief MEX file containing ReadProtocolInfo wrapper
 */

#include <stdio.h>
#include <string.h>
#include "axon.h"
#include "mex.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	ABF abf = ABF();
	mxArray *a, *n;
	char *fname;
	int ret;
	// Keys of structure ABF_ProtocolInfo
	const char *k[] = { "nOperationMode",
						 "fADCSequenceInterval",
						 "bEnableFileCompression",
						 "uFileCompressionRatio",
						 //
						 "fSynchTimeUnit",
						 "fSecondsPerRun",
						 "lNumSamplesPerEpisode",
						 "lPreTriggerSamples",
						 "lEpisodesPerRun",
						 "lRunsPerTrial",
						 "lNumberOfTrials",
						 "nAveragingMode",
						 "nUndoRunCount",
						 "nFirstEpisodeInRun",
						 "fTriggerThreshold",
						 "nTriggerSource",
						 "nTriggerAction",
						 "nTriggerPolarity",
						 "fScopeOutputInterval",
						 "fEpisodeStartToStart",
						 "fRunStartToStart",
						 "lAverageCount",
						 "fTrialStartToStart",
						 "nAutoTriggerStrategy",
						 "fFirstRunDelayS",
						 //
						 "nChannelStatsStrategy",
						 "lSamplesPerTrace",
						 "lStartDisplayNum",
						 "lFinishDisplayNum",
						 "nShowPNRawData",
						 "fStatisticsPeriod",
						 "lStatisticsMeasurements",
						 "nStatisticsSaveStrategy",
						 //
						 "fADCRange",
						 "fDACRange",
						 "lADCResolution",
						 "lDACResolution",
						 //
						 "nExperimentType",
						 "nManualInfoStrategy",
						 "nCommentsEnable",
						 "lFileCommentIndex",
						 "nAutoAnalyseEnable",
						 "nSignalType",
						//
						 "nDigitalEnable",
						 "nActiveDACChannel",
						 "nDigitalHolding",
						 "nDigitalInterEpisode",
						 "nDigitalDACChannel",
						 "nDigitalTrainActiveLogic",
						 //
						 "nStatsEnable",
						 "nStatisticsClearStrategy",
						 //
						 "nLevelHysteresis",
						 "lTimeHysteresis",
						 "nAllowExternalTags",
						 "nAverageAlgorithm",
						 "fAverageWeighting",
						 "nUndoPromptStrategy",
						 "nTrialTriggerSource",
						 "nStatisticsDisplayStrategy",
						 "nExternalTagType",
						 "nScopeTriggerOut",
						 //
						 "nLTPType",
						 "nAlternateDACOutputState",
						 "nAlternateDigitalOutputState",
						 //
						 "fCellID",
						 //
						 "nDigitizerADCs",
						 "nDigitizerDACs",
						 "nDigitizerTotalDigitalOuts",
						 "nDigitizerSynchDigitalOuts",
						 "nDigitizerType"};

    if (nrhs != 1)
	   mexErrMsgTxt("File name expected!");

    fname = mxArrayToString(prhs[0]);

    // Reading from abf file
	ret = abf.Open(fname);
	if (ret != AXON_INFO_OK )
		mexErrMsgTxt("Cannot open the file!");

	ret = abf.ReadFileInfo();
	ret = abf.ReadProtocolInfo();
	abf.Close();

    a = mxCreateStructMatrix(1, 1, 70, k);

    // Packing values into structure matrix
	mxSetFieldByNumber(a, 0, 0, mxCreateDoubleScalar((double) abf.ProtocolInfo.nOperationMode));
	mxSetFieldByNumber(a, 0, 1, mxCreateDoubleScalar((double) abf.ProtocolInfo.fADCSequenceInterval));
	mxSetFieldByNumber(a, 0, 2, mxCreateDoubleScalar((double) abf.ProtocolInfo.bEnableFileCompression));
	mxSetFieldByNumber(a, 0, 3, mxCreateDoubleScalar((double) abf.ProtocolInfo.uFileCompressionRatio));
	//
	mxSetFieldByNumber(a, 0, 4, mxCreateDoubleScalar((double) abf.ProtocolInfo.fSynchTimeUnit));
	mxSetFieldByNumber(a, 0, 5, mxCreateDoubleScalar((double) abf.ProtocolInfo.fSecondsPerRun));
	mxSetFieldByNumber(a, 0, 6, mxCreateDoubleScalar((double) abf.ProtocolInfo.lNumSamplesPerEpisode));
	mxSetFieldByNumber(a, 0, 7, mxCreateDoubleScalar((double) abf.ProtocolInfo.lPreTriggerSamples));
	mxSetFieldByNumber(a, 0, 8, mxCreateDoubleScalar((double) abf.ProtocolInfo.lEpisodesPerRun));
	mxSetFieldByNumber(a, 0, 9, mxCreateDoubleScalar((double) abf.ProtocolInfo.lRunsPerTrial));
	mxSetFieldByNumber(a, 0, 10, mxCreateDoubleScalar((double) abf.ProtocolInfo.lNumberOfTrials));
	mxSetFieldByNumber(a, 0, 11, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAveragingMode));
	mxSetFieldByNumber(a, 0, 12, mxCreateDoubleScalar((double) abf.ProtocolInfo.nUndoRunCount));
	mxSetFieldByNumber(a, 0, 13, mxCreateDoubleScalar((double) abf.ProtocolInfo.nFirstEpisodeInRun));
	mxSetFieldByNumber(a, 0, 14, mxCreateDoubleScalar((double) abf.ProtocolInfo.fTriggerThreshold));
	mxSetFieldByNumber(a, 0, 15, mxCreateDoubleScalar((double) abf.ProtocolInfo.nTriggerSource));
	mxSetFieldByNumber(a, 0, 16, mxCreateDoubleScalar((double) abf.ProtocolInfo.nTriggerAction));
	mxSetFieldByNumber(a, 0, 17, mxCreateDoubleScalar((double) abf.ProtocolInfo.nTriggerPolarity));
	mxSetFieldByNumber(a, 0, 18, mxCreateDoubleScalar((double) abf.ProtocolInfo.fScopeOutputInterval));
	mxSetFieldByNumber(a, 0, 19, mxCreateDoubleScalar((double) abf.ProtocolInfo.fEpisodeStartToStart));
	mxSetFieldByNumber(a, 0, 20, mxCreateDoubleScalar((double) abf.ProtocolInfo.fRunStartToStart));
	mxSetFieldByNumber(a, 0, 21, mxCreateDoubleScalar((double) abf.ProtocolInfo.lAverageCount));
	mxSetFieldByNumber(a, 0, 22, mxCreateDoubleScalar((double) abf.ProtocolInfo.fTrialStartToStart));
	mxSetFieldByNumber(a, 0, 23, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAutoTriggerStrategy));
	mxSetFieldByNumber(a, 0, 24, mxCreateDoubleScalar((double) abf.ProtocolInfo.fFirstRunDelayS));
	//
	mxSetFieldByNumber(a, 0, 25, mxCreateDoubleScalar((double) abf.ProtocolInfo.nChannelStatsStrategy));
	mxSetFieldByNumber(a, 0, 26, mxCreateDoubleScalar((double) abf.ProtocolInfo.lSamplesPerTrace));
	mxSetFieldByNumber(a, 0, 27, mxCreateDoubleScalar((double) abf.ProtocolInfo.lStartDisplayNum));
	mxSetFieldByNumber(a, 0, 28, mxCreateDoubleScalar((double) abf.ProtocolInfo.lFinishDisplayNum));
	mxSetFieldByNumber(a, 0, 29, mxCreateDoubleScalar((double) abf.ProtocolInfo.nShowPNRawData));
	mxSetFieldByNumber(a, 0, 30, mxCreateDoubleScalar((double) abf.ProtocolInfo.fStatisticsPeriod));
	mxSetFieldByNumber(a, 0, 31, mxCreateDoubleScalar((double) abf.ProtocolInfo.lStatisticsMeasurements));
	mxSetFieldByNumber(a, 0, 32, mxCreateDoubleScalar((double) abf.ProtocolInfo.nStatisticsSaveStrategy));
	//
	mxSetFieldByNumber(a, 0, 33, mxCreateDoubleScalar((double) abf.ProtocolInfo.fADCRange));
	mxSetFieldByNumber(a, 0, 34, mxCreateDoubleScalar((double) abf.ProtocolInfo.fDACRange));
	mxSetFieldByNumber(a, 0, 35, mxCreateDoubleScalar((double) abf.ProtocolInfo.lADCResolution));
	mxSetFieldByNumber(a, 0, 36, mxCreateDoubleScalar((double) abf.ProtocolInfo.lDACResolution));
	//
	mxSetFieldByNumber(a, 0, 37, mxCreateDoubleScalar((double) abf.ProtocolInfo.nExperimentType));
	mxSetFieldByNumber(a, 0, 38, mxCreateDoubleScalar((double) abf.ProtocolInfo.nManualInfoStrategy));
	mxSetFieldByNumber(a, 0, 39, mxCreateDoubleScalar((double) abf.ProtocolInfo.nCommentsEnable));
	mxSetFieldByNumber(a, 0, 40, mxCreateDoubleScalar((double) abf.ProtocolInfo.lFileCommentIndex));
	mxSetFieldByNumber(a, 0, 41, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAutoAnalyseEnable));
	mxSetFieldByNumber(a, 0, 42, mxCreateDoubleScalar((double) abf.ProtocolInfo.nSignalType));
	//
	mxSetFieldByNumber(a, 0, 43, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitalEnable));
	mxSetFieldByNumber(a, 0, 44, mxCreateDoubleScalar((double) abf.ProtocolInfo.nActiveDACChannel));
	mxSetFieldByNumber(a, 0, 45, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitalHolding));
	mxSetFieldByNumber(a, 0, 46, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitalInterEpisode));
	mxSetFieldByNumber(a, 0, 47, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitalDACChannel));
	mxSetFieldByNumber(a, 0, 48, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitalTrainActiveLogic));
	//
	mxSetFieldByNumber(a, 0, 49, mxCreateDoubleScalar((double) abf.ProtocolInfo.nStatsEnable));
	mxSetFieldByNumber(a, 0, 50, mxCreateDoubleScalar((double) abf.ProtocolInfo.nStatisticsClearStrategy));
	//
	mxSetFieldByNumber(a, 0, 51, mxCreateDoubleScalar((double) abf.ProtocolInfo.nLevelHysteresis));
	mxSetFieldByNumber(a, 0, 52, mxCreateDoubleScalar((double) abf.ProtocolInfo.lTimeHysteresis));
	mxSetFieldByNumber(a, 0, 53, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAllowExternalTags));
	mxSetFieldByNumber(a, 0, 54, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAverageAlgorithm));
	mxSetFieldByNumber(a, 0, 55, mxCreateDoubleScalar((double) abf.ProtocolInfo.fAverageWeighting));
	mxSetFieldByNumber(a, 0, 56, mxCreateDoubleScalar((double) abf.ProtocolInfo.nUndoPromptStrategy));
	mxSetFieldByNumber(a, 0, 57, mxCreateDoubleScalar((double) abf.ProtocolInfo.nTrialTriggerSource));
	mxSetFieldByNumber(a, 0, 58, mxCreateDoubleScalar((double) abf.ProtocolInfo.nStatisticsDisplayStrategy));
	mxSetFieldByNumber(a, 0, 59, mxCreateDoubleScalar((double) abf.ProtocolInfo.nExternalTagType));
	mxSetFieldByNumber(a, 0, 60, mxCreateDoubleScalar((double) abf.ProtocolInfo.nScopeTriggerOut));
	//
	mxSetFieldByNumber(a, 0, 61, mxCreateDoubleScalar((double) abf.ProtocolInfo.nLTPType));
	mxSetFieldByNumber(a, 0, 62, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAlternateDACOutputState));
	mxSetFieldByNumber(a, 0, 63, mxCreateDoubleScalar((double) abf.ProtocolInfo.nAlternateDigitalOutputState));
	// fCellID
	n = mxCreateNumericMatrix(1, 3, mxSINGLE_CLASS, mxREAL);
	memcpy(mxGetPr(n), abf.ProtocolInfo.fCellID, 3*sizeof(float));
	mxSetFieldByNumber(a, 0, 64, n);
	//
	mxSetFieldByNumber(a, 0, 65, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitizerADCs));
	mxSetFieldByNumber(a, 0, 66, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitizerDACs));
	mxSetFieldByNumber(a, 0, 67, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitizerTotalDigitalOuts));
	mxSetFieldByNumber(a, 0, 68, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitizerSynchDigitalOuts));
	mxSetFieldByNumber(a, 0, 69, mxCreateDoubleScalar((double) abf.ProtocolInfo.nDigitizerType));

	plhs[0] = a;
}
