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
 *  \brief The main source file of Axon Library, contains ABF class
 */

#include <stdio.h>
#include <string.h>
#include "axon.h"

// ABF class constructor.
ABF::ABF()
{
	// Initializing the internal variables to allow the check if they are empty
		strncpy(this->name, "",0);
		this->handle = NULL;
		int i, j;
		for(i = 0; i < ABF_ADCCOUNT; i++)
			this->ADCInfo[i].nADCNum = -1;
		for(i = 0; i < ABF_DACCOUNT; i++) {
			this->DACInfo[i].nDACNum = -1;
			for(j = 0; j < ABF_EPOCHCOUNT; j++) {
				this->EpochInfoPerDAC[i][j].nDACNum = -1;
				this->EpochInfoPerDAC[i][j].nEpochNum = -1;
			}
		}
		for(i = 0; i < ABF_EPOCHCOUNT; i++)
			this->EpochInfo[i].nEpochNum = -1;
}

ABF::ABF(char *fname) {
	// Initializing the internal variables to allow the check if they are empty
	strncpy(this->name, "",0);
	this->handle = NULL;
	int i, j;
	for(i = 0; i < ABF_ADCCOUNT; i++)
		this->ADCInfo[i].nADCNum = -1;
	for(i = 0; i < ABF_DACCOUNT; i++) {
		this->DACInfo[i].nDACNum = -1;
		for(j = 0; j < ABF_EPOCHCOUNT; j++) {
			this->EpochInfoPerDAC[i][j].nDACNum = -1;
			this->EpochInfoPerDAC[i][j].nEpochNum = -1;
		}
	}
	for(i = 0; i < ABF_EPOCHCOUNT; i++)
		this->EpochInfo[i].nEpochNum = -1;
	// If no name is specified exit.
	if (!strlen(fname))
		return;
	// Otherwise...
	this->Open(fname); 	// Checking if the file is possible to open.
	this->Close();

}

// ABF class destructor.
ABF::~ABF(void) {
	if (this->handle)
	{
		fclose(this->handle);
		this->handle = NULL;
	}
}

// Method opens a file.
int ABF::Open(char *fname) {
	// If a name have been specified, save it.
	if (strlen(fname))
		strcpy(this->name, fname);

	return this->Open();
}

int ABF::Open()
{
	// If no name is specified exit.
	if (!strlen(this->name))
		return AXON_ERROR_NO_NAME;

	this->handle = fopen(this->name,"rb");  // Open the file
	if (!this->handle) {					// If not delete the name.
		strcpy(this->name, "");
		this->handle = NULL;
		return AXON_ERROR_NO_HANDLE;
	}

	return AXON_INFO_OK;
}

// Method closes a file.
int ABF::Close(void) {
	// If there's no name, reset the handle and exit.
	if(!this->name) {
		this->handle = NULL;
		return AXON_ERROR_NO_NAME;
	}
	// If there's no handle (no file was opened) exit.
	if(!this->handle)
		return AXON_ERROR_NO_HANDLE;
	// Otherwise close file and reset the handle.
	fclose(this->handle);
	this->handle = NULL;
	return AXON_INFO_OK;
}

// Method reads File Info from the ABF file
int ABF::ReadFileInfo() {
	// The file have to be opened first - use Open()
	if (!this->handle)
		return AXON_ERROR_NO_HANDLE;

	// File Info section
	fseek(this->handle, 0, SEEK_SET);
	fread(&this->FileInfo, sizeof this->FileInfo, 1, this->handle);
	return AXON_INFO_OK;
}

// Method reads Protocol Info from the ABF file
int ABF::ReadProtocolInfo() {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.ProtocolSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// Protocol Info section
	fseek(this->handle, this->FileInfo.ProtocolSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(&this->ProtocolInfo, sizeof this->ProtocolInfo, 1, this->handle);
	return AXON_INFO_OK;
}

// Method reads ADC Info from the ABF file
int ABF::ReadADCInfo() {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.ADCSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// ADC Info section
	fseek(this->handle, this->FileInfo.ADCSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(&this->ADCInfo,this->FileInfo.ADCSection.uBytes, this->FileInfo.ADCSection.llNumEntries, this->handle);

	return AXON_INFO_OK;
}

// Method reads DAC Info from the ABF file
int ABF::ReadDACInfo() {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.DACSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// ADC Info section
	fseek(this->handle, this->FileInfo.DACSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(&this->DACInfo,this->FileInfo.DACSection.uBytes, FileInfo.DACSection.llNumEntries, this->handle);

	return AXON_INFO_OK;
}

// Method reads Epoch Info from the ABF file
int ABF::ReadEpochInfo() {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.EpochSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// ADC Info section
	fseek(this->handle, this->FileInfo.EpochSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(&this->EpochInfo,this->FileInfo.EpochSection.uBytes, this->FileInfo.EpochSection.llNumEntries, this->handle);

	return AXON_INFO_OK;
}

// Method reads Epoch Info per DAC from the ABF file
int ABF::ReadEpochInfoPerDAC() {
	ABF_EpochInfoPerDAC tmp;

	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.EpochPerDACSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// ADC Info section
	fseek(this->handle, this->FileInfo.EpochPerDACSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	for(int i = 0; i < (int)this->FileInfo.EpochPerDACSection.llNumEntries; i++)
	{
		fread(&tmp,this->FileInfo.EpochPerDACSection.uBytes, 1, this->handle);
		this->EpochInfoPerDAC[tmp.nDACNum][tmp.nEpochNum] = tmp;
	}

	return AXON_INFO_OK;
}

// Method reads Strings from the ABF file
int ABF::ReadStrings() {
	// The file have to be opened first - use Open()
	if (!this->handle)
		return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.StringsSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// Strings section
	fseek(this->handle, this->FileInfo.StringsSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(&this->Strings,ABF_BLOCKSIZE, 1, this->handle);

	return AXON_INFO_OK;
}

// Method gets string with index i from Strings from the ABF file
int ABF::GetString(char *str, int index) {
	char tmp[ABF_BLOCKSIZE], *ptmp;
	int i=1, n;

	// The file have to be opened first - use Open()
	if (!this->handle)
		return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.StringsSection.llNumEntries < index)
		return AXON_ERROR_NO_STRING;

	// Copy useful Strings part
	ptmp = this->Strings+44;
	while (index - i >= 0){
		// If the index point to this string, read it to given variable
		if (index == i) {
			strcpy(str, ptmp);
			n = strlen(str);
		}
		else {
			strcpy(tmp, ptmp);
			n = strlen(tmp);
		}
		ptmp += n+1;
		i++;
	}


	return AXON_INFO_OK;
}

// The same function as before, just returns string (more useful for wrapping)
char* ABF::GetString(int i) {
	char *str = new char[255];

	this->GetString(str,i);
	return str;
}

// Method reads Sych Array from the ABF file
int ABF::ReadSynchArray() {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.SynchArraySection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// Create SynchArray variable

	// If SynchArray have non-zero length, create it
	if (this->FileInfo.SynchArraySection.llNumEntries > 0)
		this->SynchArray.reserve(this->FileInfo.SynchArraySection.llNumEntries);
	else
		return AXON_INFO_NO_SECTION;

	// Synch Array section
	fseek(this->handle, this->FileInfo.SynchArraySection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(&this->SynchArray[0],this->FileInfo.SynchArraySection.uBytes, this->FileInfo.SynchArraySection.llNumEntries, this->handle);

	return AXON_INFO_OK;
}

// Method reads the content of the ABF file
int ABF::ReadAllSections() {
	int ret;

	ret = this->ReadFileInfo();
	if(ret < 0)
		return ret;

	ret = this->ReadProtocolInfo();
	if(ret < 0)
		return ret;

	ret = this->ReadADCInfo();
	if(ret < 0)
		return ret;

	ret = this->ReadDACInfo();
	if(ret < 0)
		return ret;

	ret = this->ReadEpochInfo();
	if(ret < 0)
		return ret;

	ret = this->ReadEpochInfoPerDAC();
		if(ret < 0)
			return ret;

	ret = this->ReadStrings();
	if(ret < 0)
		return ret;

	ret = this->ReadSynchArray();
		if(ret < 0)
			return ret;

	return ret;
}

// Method reads Data from the ABF file
int ABF::ReadData(void *data) {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return AXON_ERROR_NO_HANDLE;

	// Check if the section is present in the file
	if (this->FileInfo.DataSection.uBlockIndex == 0)
		return AXON_INFO_NO_SECTION;

	// Data section
	fseek(this->handle, this->FileInfo.DataSection.uBlockIndex * ABF_BLOCKSIZE, SEEK_SET);
	fread(data,this->FileInfo.DataSection.uBytes,
			this->FileInfo.DataSection.llNumEntries, this->handle);

	return AXON_INFO_OK;
}

// Method reads float data from the ABF file
float *ABF::ReadFloatData(void) {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return NULL;

	// Check if the section is present in the file
	if (this->FileInfo.DataSection.uBlockIndex == 0)
		return NULL;

	float *ret = new float[this->FileInfo.DataSection.llNumEntries];

	if( ReadData(ret) != AXON_INFO_OK)
		return NULL;
	return ret;
}

// Method reads integer data from the ABF file
short *ABF::ReadIntData(void) {
	// The file have to be opened first - use Open()
	if (!this->handle)
			return NULL;

	// Check if the section is present in the file
	if (this->FileInfo.DataSection.uBlockIndex == 0)
		return NULL;

	short *ret = new short[this->FileInfo.DataSection.llNumEntries];

	if( ReadData(ret) != AXON_INFO_OK)
		return NULL;
	return ret;
}

