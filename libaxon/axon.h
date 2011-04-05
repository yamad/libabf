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
 *  \brief The main header file of Axon Library, contains ABF class
 */


#ifndef AXON_H_
#define AXON_H_

#include <vector>

#include "axon_defs.h"
#include "axon_structs.h"


// Definitions of the error and info values
#define AXON_ERROR_NO_NAME 		-1 		// the name has not been defined
#define AXON_ERROR_NO_HANDLE 	-2 		// there is no handle - file has not been open or there were some problem with it
#define AXON_ERROR_NO_FILEINFO 	-3		// there is no file section - have to read file section first
#define AXON_ERROR_NO_STRING 	-4		// there is no string for given index
#define AXON_INFO_OK			1		// everything OK
#define AXON_INFO_NO_SECTION	2 		// this section is not in the file (not an error)


/*! \class ABF
 * 	\brief Main class of the library
 */
class ABF {
	private:
		FILE *handle;
		char name[255];

	public:
		ABF_FileInfo 		FileInfo;
		ABF_ProtocolInfo 	ProtocolInfo;
		ABF_ADCInfo 		ADCInfo[ABF_ADCCOUNT];		// one for each ADC channel
		ABF_DACInfo 		DACInfo[ABF_DACCOUNT];      // one for each DAC channel
		ABF_EpochInfo 		EpochInfo[ABF_EPOCHCOUNT];  // one for each epoch
		// TODO Unknown structure - not defined by Axon
		//ABF_Section ADCPerDACSection;          		// one for each ADC for each DAC
		ABF_EpochInfoPerDAC EpochInfoPerDAC[ABF_DACCOUNT][ABF_EPOCHCOUNT];  	// one for each epoch for each DAC
		//ABF_Section UserListSection;           // one for each user list
		//ABF_Section StatsRegionSection;        // one for each stats region
		//ABF_Section MathSection;
		char Strings[ABF_BLOCKSIZE];
		std::vector<ABF_SynchArray> 		SynchArray;

	ABF(char fname[]);
	ABF(void);
	~ABF(void);
	int Open(char fname[]);
	int Open(void);
	int Close(void);
	int ReadFileInfo(void);
	int ReadProtocolInfo(void);
	int ReadADCInfo(void);
	int ReadDACInfo(void);
	int ReadEpochInfo(void);
	int ReadEpochInfoPerDAC(void);
	int ReadStrings(void);
	int GetString(char *str, int i);
	char* GetString(int i);
	int ReadSynchArray(void);
	int ReadAllSections(void);
	int ReadData(void *data);
	float *ReadFloatData(void);
	short *ReadIntData(void);
}; // class ABF

#endif /* AXON_H_ */
