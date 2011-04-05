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
 *  \brief Header containing all definitions for ABF
 */

#ifndef AXON_DEFS_H_
#define AXON_DEFS_H_

//
// Constants used in defining the ABF file header
//
#define ABF_ADCCOUNT           16    // number of ADC channels supported.
#define ABF_DACCOUNT           4     // number of DAC channels supported.
#define ABF_EPOCHCOUNT         10    // number of waveform epochs supported.
#define ABF_ADCUNITLEN         8     // length of ADC units strings
#define ABF_ADCNAMELEN_USER    8     // length of user-entered ADC channel name strings
#define ABF_ADCNAMELEN         10    // length of actual ADC channel name strings
#define ABF_DACUNITLEN         8     // length of DAC units strings
#define ABF_DACNAMELEN         10    // length of DAC channel name strings
#define ABF_USERLISTLEN        256   // length of the user list (V1.6)
#define ABF_USERLISTCOUNT      4     // number of independent user lists (V1.6)
#define ABF_OLDFILECOMMENTLEN  56    // length of file comment string (pre V1.6)
#define ABF_FILECOMMENTLEN     128   // length of file comment string (V1.6)
#define ABF_PATHLEN            256   // length of full path, used for DACFile and Protocol name.
#define ABF_CREATORINFOLEN     16    // length of file creator info string
#define ABF_ARITHMETICOPLEN    2     // length of the Arithmetic operator field
#define ABF_ARITHMETICUNITSLEN 8     // length of arithmetic units string
#define ABF_TAGCOMMENTLEN      56    // length of tag comment string
#define ABF_BLOCKSIZE          512   // Size of block alignment in ABF files.
#define PCLAMP6_MAXSWEEPLENGTH 16384   // Maximum multiplexed sweep length supported by pCLAMP6 apps.
#define PCLAMP7_MAXSWEEPLEN_PERCHAN    1032258  // Maximum per channel sweep length supported by pCLAMP7 apps.
#define ABF_MAX_SWEEPS_PER_AVERAGE 65500     // The maximum number of sweeps that can be combined into a
                                             // cumulative average (nAverageAlgorithm=ABF_INFINITEAVERAGE).
#define ABF_MAX_TRIAL_SAMPLES  0x7FFFFFFF    // Maximum length of acquisition supported (samples)
                                             // INT_MAX is used instead of UINT_MAX because of the signed
                                             // values in the ABF header.

//
// Constants for nDigitizerType
//
#define ABF_DIGI_UNKNOWN   0
#define ABF_DIGI_DEMO      1
#define ABF_DIGI_MINIDIGI  2
#define ABF_DIGI_DD132X    3
#define ABF_DIGI_OPUS      4
#define ABF_DIGI_PATCH     5
#define ABF_DIGI_DD1440    6

//
// Constants for nDrawingStrategy
//
#define ABF_DRAW_NONE            0
#define ABF_DRAW_REALTIME        1
#define ABF_DRAW_FULLSCREEN      2
#define ABF_DRAW_ENDOFRUN        3

//
// Constants for nTiledDisplay
//
#define ABF_DISPLAY_SUPERIMPOSED 0
#define ABF_DISPLAY_TILED        1

//
// Constants for nDataDisplayMode
//
#define ABF_DRAW_POINTS       0
#define ABF_DRAW_LINES        1

// Constants for the ABF_ReadOpen and ABF_WriteOpen functions
#define ABF_DATAFILE          0
#define ABF_PARAMFILE         1
#define ABF_ALLOWOVERLAP      2     // If this flag is not set, overlapping data in fixed-length
                                    // event-detected data will be edited out by adjustment of
                                    // the synch array. (ABF_ReadOpen only!)
#define ABF_DATAFILE_ABF1     4
#define ABF_PARAMFILE_ABF1    8

//
// Constants for lParameterID in the ABFDelta structure.
//
// NOTE: If any changes are made to this list, the code in ABF_UpdateHeader must
//       be updated to include the new items.
#define ABF_DELTA_HOLDING0          0
#define ABF_DELTA_HOLDING1          1
#define ABF_DELTA_HOLDING2          2
#define ABF_DELTA_HOLDING3          3
#define ABF_DELTA_DIGITALOUTS       4
#define ABF_DELTA_THRESHOLD         5
#define ABF_DELTA_PRETRIGGER        6

// Because of lack of space, the Autosample Gain ID also contains the ADC number.
#define ABF_DELTA_AUTOSAMPLE_GAIN   100   // +ADC channel.

// Because of lack of space, the Signal Gain ID also contains the ADC number.
#define ABF_DELTA_SIGNAL_GAIN       200   // +ADC channel.


//
// Constants for nAveragingMode
//
#define ABF_NOAVERAGING       0
#define ABF_SAVEAVERAGEONLY   1
#define ABF_AVERAGESAVEALL    2

//
// Constants for nAverageAlgorithm
//
#define ABF_INFINITEAVERAGE   0
#define ABF_SLIDINGAVERAGE    1

//
// Constants for nUndoPromptStrategy
//
#define ABF_UNDOPROMPT_ONABORT   0
#define ABF_UNDOPROMPT_ALWAYS    1

//
// Constants for nTriggerAction
//
#define ABF_TRIGGER_STARTEPISODE 0
#define ABF_TRIGGER_STARTRUN     1
#define ABF_TRIGGER_STARTTRIAL   2    // N.B. Discontinued in favor of nTrialTriggerSource

//
// Constants for nTriggerPolarity.
//
#define ABF_TRIGGER_RISINGEDGE  0
#define ABF_TRIGGER_FALLINGEDGE 1

//
// Constants for nDACFileEpisodeNum
//
#define ABF_DACFILE_SKIPFIRSTSWEEP -1
#define ABF_DACFILE_USEALLSWEEPS    0
// >0 = The specific sweep number.

//
// Constants for nInterEpisodeLevel & nDigitalInterEpisode
//
#define ABF_INTEREPI_USEHOLDING    0
#define ABF_INTEREPI_USELASTEPOCH  1

//
// Constants for nArithmeticExpression
//
#define ABF_SIMPLE_EXPRESSION    0
#define ABF_RATIO_EXPRESSION     1

//
// Constants for nLowpassFilterType & nHighpassFilterType
//
#define ABF_FILTER_NONE          0
#define ABF_FILTER_EXTERNAL      1
#define ABF_FILTER_SIMPLE_RC     2
#define ABF_FILTER_BESSEL        3
#define ABF_FILTER_BUTTERWORTH   4

//
// Constants for post nPostprocessLowpassFilterType
//
#define ABF_POSTPROCESS_FILTER_NONE          0
#define ABF_POSTPROCESS_FILTER_ADAPTIVE      1
#define ABF_POSTPROCESS_FILTER_BESSEL        2
#define ABF_POSTPROCESS_FILTER_BOXCAR        3
#define ABF_POSTPROCESS_FILTER_BUTTERWORTH   4
#define ABF_POSTPROCESS_FILTER_CHEBYSHEV     5
#define ABF_POSTPROCESS_FILTER_GAUSSIAN      6
#define ABF_POSTPROCESS_FILTER_RC            7
#define ABF_POSTPROCESS_FILTER_RC8           8
#define ABF_POSTPROCESS_FILTER_NOTCH         9

//
// The output sampling sequence identifier for a separate digital out channel.
//
#define ABF_DIGITAL_OUT_CHANNEL -1
#define ABF_PADDING_OUT_CHANNEL -2

//
// Constants for nAutoAnalyseEnable
//
#define ABF_AUTOANALYSE_DISABLED   0
#define ABF_AUTOANALYSE_DEFAULT    1
#define ABF_AUTOANALYSE_RUNMACRO   2

//
// Constants for nAutopeakSearchMode
//
#define ABF_PEAK_SEARCH_SPECIFIED       -2
#define ABF_PEAK_SEARCH_ALL             -1
// nAutopeakSearchMode 0..9   = epoch in waveform 0's epoch table
// nAutopeakSearchMode 10..19 = epoch in waveform 1's epoch table

//
// Constants for nAutopeakBaseline
//
#define ABF_PEAK_BASELINE_SPECIFIED    -3
#define ABF_PEAK_BASELINE_NONE 	      -2
#define ABF_PEAK_BASELINE_FIRSTHOLDING -1
#define ABF_PEAK_BASELINE_LASTHOLDING  -4

// Bit flag settings for nStatsSearchRegionFlags
//
#define ABF_PEAK_SEARCH_REGION0           0x01
#define ABF_PEAK_SEARCH_REGION1           0x02
#define ABF_PEAK_SEARCH_REGION2           0x04
#define ABF_PEAK_SEARCH_REGION3           0x08
#define ABF_PEAK_SEARCH_REGION4           0x10
#define ABF_PEAK_SEARCH_REGION5           0x20
#define ABF_PEAK_SEARCH_REGION6           0x40
#define ABF_PEAK_SEARCH_REGION7           0x80
#define ABF_PEAK_SEARCH_REGIONALL         0xFF        // All of the above OR'd together.

//
// Constants for nStatsActiveChannels
//
#define ABF_PEAK_SEARCH_CHANNEL0          0x0001
#define ABF_PEAK_SEARCH_CHANNEL1          0x0002
#define ABF_PEAK_SEARCH_CHANNEL2          0x0004
#define ABF_PEAK_SEARCH_CHANNEL3          0x0008
#define ABF_PEAK_SEARCH_CHANNEL4          0x0010
#define ABF_PEAK_SEARCH_CHANNEL5          0x0020
#define ABF_PEAK_SEARCH_CHANNEL6          0x0040
#define ABF_PEAK_SEARCH_CHANNEL7          0x0080
#define ABF_PEAK_SEARCH_CHANNEL8          0x0100
#define ABF_PEAK_SEARCH_CHANNEL9          0x0200
#define ABF_PEAK_SEARCH_CHANNEL10         0x0400
#define ABF_PEAK_SEARCH_CHANNEL11         0x0800
#define ABF_PEAK_SEARCH_CHANNEL12         0x1000
#define ABF_PEAK_SEARCH_CHANNEL13         0x2000
#define ABF_PEAK_SEARCH_CHANNEL14         0x4000
#define ABF_PEAK_SEARCH_CHANNEL15         0x8000
#define ABF_PEAK_SEARCH_CHANNELSALL       0xFFFF      // All of the above OR'd together.

//
// Constants for nLeakSubtractType
//
#define ABF_LEAKSUBTRACT_NONE       0
#define ABF_LEAKSUBTRACT_PN         1
#define ABF_LEAKSUBTRACT_RESISTIVE  2

//
// Constants for nPNPolarity
//
#define ABF_PN_OPPOSITE_POLARITY -1
#define ABF_PN_SAME_POLARITY     1

//
// Constants for nPNPosition
//
#define ABF_PN_BEFORE_EPISODE    0
#define ABF_PN_AFTER_EPISODE     1

//
// Constants for nAutosampleEnable
//
#define ABF_AUTOSAMPLEDISABLED   0
#define ABF_AUTOSAMPLEAUTOMATIC  1
#define ABF_AUTOSAMPLEMANUAL     2

//
// Constants for nAutosampleInstrument
//
#define ABF_INST_UNKNOWN         0   // Unknown instrument (manual or user defined telegraph table).
#define ABF_INST_AXOPATCH1       1   // Axopatch-1 with CV-4-1/100
#define ABF_INST_AXOPATCH1_1     2   // Axopatch-1 with CV-4-0.1/100
#define ABF_INST_AXOPATCH1B      3   // Axopatch-1B(inv.) CV-4-1/100
#define ABF_INST_AXOPATCH1B_1    4   // Axopatch-1B(inv) CV-4-0.1/100
#define ABF_INST_AXOPATCH201     5   // Axopatch 200 with CV 201
#define ABF_INST_AXOPATCH202     6   // Axopatch 200 with CV 202
#define ABF_INST_GENECLAMP       7   // GeneClamp
#define ABF_INST_DAGAN3900       8   // Dagan 3900
#define ABF_INST_DAGAN3900A      9   // Dagan 3900A
#define ABF_INST_DAGANCA1_1      10  // Dagan CA-1  Im=0.1
#define ABF_INST_DAGANCA1        11  // Dagan CA-1  Im=1.0
#define ABF_INST_DAGANCA10       12  // Dagan CA-1  Im=10
#define ABF_INST_WARNER_OC725    13  // Warner OC-725
#define ABF_INST_WARNER_OC725C   14  // Warner OC-725
#define ABF_INST_AXOPATCH200B    15  // Axopatch 200B
#define ABF_INST_DAGANPCONE0_1   16  // Dagan PC-ONE  Im=0.1
#define ABF_INST_DAGANPCONE1     17  // Dagan PC-ONE  Im=1.0
#define ABF_INST_DAGANPCONE10    18  // Dagan PC-ONE  Im=10
#define ABF_INST_DAGANPCONE100   19  // Dagan PC-ONE  Im=100
#define ABF_INST_WARNER_BC525C   20  // Warner BC-525C
#define ABF_INST_WARNER_PC505    21  // Warner PC-505
#define ABF_INST_WARNER_PC501    22  // Warner PC-501
#define ABF_INST_DAGANCA1_05     23  // Dagan CA-1  Im=0.05
#define ABF_INST_MULTICLAMP700   24  // MultiClamp 700
#define ABF_INST_TURBO_TEC       25  // Turbo Tec
#define ABF_INST_OPUSXPRESS6000  26  // OpusXpress 6000A
#define ABF_INST_AXOCLAMP900     27  // Axoclamp 900

//
// Constants for nTagType in the ABFTag structure.
//
#define ABF_TIMETAG              0
#define ABF_COMMENTTAG           1
#define ABF_EXTERNALTAG          2
#define ABF_VOICETAG             3
#define ABF_NEWFILETAG           4
#define ABF_ANNOTATIONTAG        5        // Same as a comment tag except that nAnnotationIndex holds
                                          // the index of the annotation that holds extra information.

// Comment inserted for externally acquired tags (expanded with spaces to ABF_TAGCOMMENTLEN).
#define ABF_EXTERNALTAGCOMMENT   "<External>"
#define ABF_VOICETAGCOMMENT      "<Voice Tag>"

//
// Constants for nManualInfoStrategy
//
#define ABF_ENV_DONOTWRITE      0
#define ABF_ENV_WRITEEACHTRIAL  1
#define ABF_ENV_PROMPTEACHTRIAL 2

//
// Constants for nAutopeakPolarity
//
#define ABF_PEAK_NEGATIVE       -1
#define ABF_PEAK_ABSOLUTE        0
#define ABF_PEAK_POSITIVE        1

//
// LTP Types - Reflects whether the header is used for LTP as baseline or induction.
//
#define ABF_LTP_TYPE_NONE              0
#define ABF_LTP_TYPE_BASELINE          1
#define ABF_LTP_TYPE_INDUCTION         2

//
// LTP Usage of DAC - Reflects whether the analog output will be used presynaptically or postsynaptically.
//
#define ABF_LTP_DAC_USAGE_NONE         0
#define ABF_LTP_DAC_USAGE_PRESYNAPTIC  1
#define ABF_LTP_DAC_USAGE_POSTSYNAPTIC 2

// Values for the wScopeMode field in ABFScopeConfig.
#define ABF_EPISODICMODE    0
#define ABF_CONTINUOUSMODE  1
//#define ABF_XYMODE          2

//
// Constants for nExperimentType
//
#define ABF_VOLTAGECLAMP         0
#define ABF_CURRENTCLAMP         1
#define ABF_SIMPLEACQUISITION    2

//
// Miscellaneous constants
//
#define ABF_FILTERDISABLED  100000.0F     // Large frequency to disable lowpass filters
#define ABF_UNUSED_CHANNEL  -1            // Unused ADC and DAC channels.
#define ABF_ANY_CHANNEL     (UINT)-1      // Any ADC or DAC channel.

//
// Constant definitions for nDataFormat
//
#define ABF_INTEGERDATA      0
#define ABF_FLOATDATA        1

//
// Constant definitions for nOperationMode
//
#define ABF_VARLENEVENTS     1
#define ABF_FIXLENEVENTS     2     // (ABF_FIXLENEVENTS == ABF_LOSSFREEOSC)
#define ABF_LOSSFREEOSC      2
#define ABF_GAPFREEFILE      3
#define ABF_HIGHSPEEDOSC     4
#define ABF_WAVEFORMFILE     5

//
// Constants for nEpochType
//
#define ABF_EPOCHDISABLED           0     // disabled epoch
#define ABF_EPOCHSTEPPED            1     // stepped waveform
#define ABF_EPOCHRAMPED             2     // ramp waveform
#define ABF_EPOCH_TYPE_RECTANGLE    3     // rectangular pulse train
#define ABF_EPOCH_TYPE_TRIANGLE     4     // triangular waveform
#define ABF_EPOCH_TYPE_COSINE       5     // cosinusoidal waveform
#define ABF_EPOCH_TYPE_UNUSED       6     // was ABF_EPOCH_TYPE_RESISTANCE
#define ABF_EPOCH_TYPE_BIPHASIC     7     // biphasic pulse train

//
// Constants for epoch resistance
//
#define ABF_MIN_EPOCH_RESISTANCE_DURATION 8

//
// Constants for nWaveformSource
//
#define ABF_WAVEFORMDISABLED     0               // disabled waveform
#define ABF_EPOCHTABLEWAVEFORM   1
#define ABF_DACFILEWAVEFORM      2

//
// Constant definitions for nFileType
//
#define ABF_ABFFILE          1
#define ABF_FETCHEX          2
#define ABF_CLAMPEX          3

//
// maximum values for various parameters (used by ABFH1_CheckUserList).
//
#define ABF_CTPULSECOUNT_MAX           10000
#define ABF_CTBASELINEDURATION_MAX     1000000.0F
#define ABF_CTSTEPDURATION_MAX         1000000.0F
#define ABF_CTPOSTTRAINDURATION_MAX    1000000.0F
#define ABF_SWEEPSTARTTOSTARTTIME_MAX  1000000.0F
#define ABF_PNPULSECOUNT_MAX           8
#define ABF_DIGITALVALUE_MAX           0xFF
#define ABF_EPOCHDIGITALVALUE_MAX      0xFF

//
// Constants for nTriggerSource
//
#define ABF_TRIGGERLINEINPUT           -5   // Start on line trigger (DD1320 only)
#define ABF_TRIGGERTAGINPUT            -4
#define ABF_TRIGGERFIRSTCHANNEL        -3
#define ABF_TRIGGEREXTERNAL            -2
#define ABF_TRIGGERSPACEBAR            -1
// >=0 = ADC channel to trigger off.

//
// Constants for nTrialTriggerSource
//
#define ABF_TRIALTRIGGER_SWSTARTONLY   -6   // Start on software message, end when protocol ends.
#define ABF_TRIALTRIGGER_SWSTARTSTOP   -5   // Start and end on software messages.
#define ABF_TRIALTRIGGER_LINEINPUT     -4   // Start on line trigger (DD1320 only)
#define ABF_TRIALTRIGGER_SPACEBAR      -3   // Start on spacebar press.
#define ABF_TRIALTRIGGER_EXTERNAL      -2   // Start on external trigger high
#define ABF_TRIALTRIGGER_NONE          -1   // Start immediately (default).
// >=0 = ADC channel to trigger off.    // Not implemented as yet...

//
// Constants for lStatisticsMeasurements
//
#define ABF_STATISTICS_ABOVETHRESHOLD     0x00000001
#define ABF_STATISTICS_EVENTFREQUENCY     0x00000002
#define ABF_STATISTICS_MEANOPENTIME       0x00000004
#define ABF_STATISTICS_MEANCLOSEDTIME     0x00000008
#define ABF_STATISTICS_ALL                0x0000000F     // All the above OR'd together.

//
// Constants for nStatisticsSaveStrategy
//
#define ABF_STATISTICS_NOAUTOSAVE            0
#define ABF_STATISTICS_AUTOSAVE              1
#define ABF_STATISTICS_AUTOSAVE_AUTOCLEAR    2

//
// Constants for nStatisticsDisplayStrategy
//
#define ABF_STATISTICS_DISPLAY      0
#define ABF_STATISTICS_NODISPLAY    1

//
// Constants for nStatisticsClearStrategy
// determines whether to clear statistics after saving.
//
#define ABF_STATISTICS_NOCLEAR      0
#define ABF_STATISTICS_CLEAR        1

#define ABF_STATS_REGIONS     8              // The number of independent statistics regions.
#define ABF_BASELINE_REGIONS  1              // The number of independent baseline regions.
#define ABF_STATS_NUM_MEASUREMENTS 18        // The total number of supported statistcs measurements.

//
// Constants for lAutopeakMeasurements
//
#define ABF_PEAK_MEASURE_PEAK                0x00000001
#define ABF_PEAK_MEASURE_PEAKTIME            0x00000002
#define ABF_PEAK_MEASURE_ANTIPEAK            0x00000004
#define ABF_PEAK_MEASURE_ANTIPEAKTIME        0x00000008
#define ABF_PEAK_MEASURE_MEAN                0x00000010
#define ABF_PEAK_MEASURE_STDDEV              0x00000020
#define ABF_PEAK_MEASURE_INTEGRAL            0x00000040
#define ABF_PEAK_MEASURE_MAXRISESLOPE        0x00000080
#define ABF_PEAK_MEASURE_MAXRISESLOPETIME    0x00000100
#define ABF_PEAK_MEASURE_MAXDECAYSLOPE       0x00000200
#define ABF_PEAK_MEASURE_MAXDECAYSLOPETIME   0x00000400
#define ABF_PEAK_MEASURE_RISETIME            0x00000800
#define ABF_PEAK_MEASURE_DECAYTIME           0x00001000
#define ABF_PEAK_MEASURE_HALFWIDTH           0x00002000
#define ABF_PEAK_MEASURE_BASELINE            0x00004000
#define ABF_PEAK_MEASURE_RISESLOPE           0x00008000
#define ABF_PEAK_MEASURE_DECAYSLOPE          0x00010000
#define ABF_PEAK_MEASURE_REGIONSLOPE         0x00020000

#define ABF_PEAK_NORMAL_PEAK                 0x00100000
#define ABF_PEAK_NORMAL_ANTIPEAK             0x00400000
#define ABF_PEAK_NORMAL_MEAN                 0x01000000
#define ABF_PEAK_NORMAL_STDDEV               0x02000000
#define ABF_PEAK_NORMAL_INTEGRAL             0x04000000

#define ABF_PEAK_NORMALISABLE                0x00000075
#define ABF_PEAK_NORMALISED                  0x07500000

#define ABF_PEAK_MEASURE_ALL                 0x0752FFFF    // All of the above OR'd together.

//
// Constant definitions for nParamToVary
//
#define ABF_CONDITNUMPULSES         0
#define ABF_CONDITBASELINEDURATION  1
#define ABF_CONDITBASELINELEVEL     2
#define ABF_CONDITSTEPDURATION      3
#define ABF_CONDITSTEPLEVEL         4
#define ABF_CONDITPOSTTRAINDURATION 5
#define ABF_CONDITPOSTTRAINLEVEL    6
#define ABF_EPISODESTARTTOSTART     7
#define ABF_INACTIVEHOLDING         8
#define ABF_DIGITALHOLDING          9
#define ABF_PNNUMPULSES             10
#define ABF_PARALLELVALUE           11
#define ABF_EPOCHINITLEVEL          (ABF_PARALLELVALUE + ABF_EPOCHCOUNT)
#define ABF_EPOCHINITDURATION       (ABF_EPOCHINITLEVEL + ABF_EPOCHCOUNT)
#define ABF_EPOCHTRAINPERIOD        (ABF_EPOCHINITDURATION + ABF_EPOCHCOUNT)
#define ABF_EPOCHTRAINPULSEWIDTH    (ABF_EPOCHTRAINPERIOD + ABF_EPOCHCOUNT)
// Next value is (ABF_EPOCHINITDURATION + ABF_EPOCHCOUNT)

// Values for the nEraseStrategy field in ABFScopeConfig.
#define ABF_ERASE_EACHSWEEP   0
#define ABF_ERASE_EACHRUN     1
#define ABF_ERASE_EACHTRIAL   2
#define ABF_ERASE_DONTERASE   3

// Indexes into the rgbColor field of ABFScopeConfig.
#define ABF_BACKGROUNDCOLOR   0
#define ABF_GRIDCOLOR         1
#define ABF_THRESHOLDCOLOR    2
#define ABF_EVENTMARKERCOLOR  3
#define ABF_SEPARATORCOLOR    4
#define ABF_AVERAGECOLOR      5
#define ABF_OLDDATACOLOR      6
#define ABF_TEXTCOLOR         7
#define ABF_AXISCOLOR         8
#define ABF_ACTIVEAXISCOLOR   9
#define ABF_LASTCOLOR         ABF_ACTIVEAXISCOLOR
#define ABF_SCOPECOLORS       (ABF_LASTCOLOR+1)

// Extended colors for rgbColorEx field in ABFScopeConfig
#define ABF_STATISTICS_REGION0 0
#define ABF_STATISTICS_REGION1 1
#define ABF_STATISTICS_REGION2 2
#define ABF_STATISTICS_REGION3 3
#define ABF_STATISTICS_REGION4 4
#define ABF_STATISTICS_REGION5 5
#define ABF_STATISTICS_REGION6 6
#define ABF_STATISTICS_REGION7 7
#define ABF_BASELINE_REGION    8
#define ABF_STOREDSWEEPCOLOR   9
#define ABF_LASTCOLOR_EX       ABF_STOREDSWEEPCOLOR
#define ABF_SCOPECOLORS_EX     (ABF_LASTCOLOR+1)

//
// Constants for nCompressionType in the ABFVoiceTagInfo structure.
//
#define ABF_COMPRESSION_NONE     0
#define ABF_COMPRESSION_PKWARE   1

#define ABF_CURRENTVERSION    ABF_V200        // Current file format version number
//
// Header Version Numbers
//
#define ABF_V200  2.00F                       // Alpha versions of pCLAMP 10 and DataXpress 2
#define ABF_V201  2.01F                       // DataXpress 2.0.0.16 and later
                                              // pCLAMP 10.0.0.6 and later

// Retired constants.
#undef ABF_AUTOANALYSE_RUNMACRO
#undef ABF_MACRONAMELEN

#endif /* AXON_DEFS_H_ */
