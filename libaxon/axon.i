%module axon
 %{
#include "axon_defs.h"
#include "axon_structs.h"
#include "axon.h"
 %}

%typemap(out) float * {
  $result = PyList_New(arg1->FileInfo.DataSection.llNumEntries);
  for (int i = 0; i < arg1->FileInfo.DataSection.llNumEntries; i++) {
    PyObject *o = PyFloat_FromDouble((double) $1[i]);
    PyList_SetItem($result,i,o);
  }
}

%typemap(out) short * {
  $result = PyList_New(arg1->FileInfo.DataSection.llNumEntries);
  for (int i = 0; i < arg1->FileInfo.DataSection.llNumEntries; i++) {
    PyObject *o = PyInt_FromLong((long) $1[i]);
    PyList_SetItem($result,i,o);
  }
}

%include "std_vector.i"
%include "axon_defs.h"
%include "axon_structs.h" 

namespace std {
   %template(VectSynchArray) vector<ABF_SynchArray>;
};


// Definitions of the error and info values
#define AXON_ERROR_NO_NAME 		-1 		// the name has not been defined
#define AXON_ERROR_NO_HANDLE 	-2 		// there is no handle - file has not been open or there were some problem with it
#define AXON_ERROR_NO_FILEINFO 	-3		// there is no file section - have to read file section first
#define AXON_ERROR_WRONG_TYPE 	-4		// wrong data type passed to method as the argument
#define AXON_INFO_OK			1		// everything OK
#define AXON_INFO_NO_SECTION	2 		// this section is not in the file (not an error)

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
	//int GetString(char *str, int i);
	char* GetString(int i);
	int ReadSynchArray(void);
	int ReadAllSections(void);
	//int ReadData(void *data);
	float *ReadFloatData(void);
	short *ReadIntData(void);
}; // class ABF

%extend ABF {
	ABF_ADCInfo GetADCInfo(int i) {
		return $self->ADCInfo[i];
	}

	ABF_DACInfo GetDACInfo(int i) {
		return $self->DACInfo[i];
	}

	ABF_EpochInfo GetEpochInfo(int i) {
		return $self->EpochInfo[i];
	}

	ABF_EpochInfoPerDAC GetEpochInfoPerDAC(int i, int j) {
		return $self->EpochInfoPerDAC[i][j];
	}
	
	ABF_SynchArray GetSynchArray(int i) {
		return $self->SynchArray[i];
	}
};


