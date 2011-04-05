cdef extern from "axon.h" namespace "axon":
     cdef cppclass ABF:
          ABF(void)
          ABF(char*)
          int Open(char*)
          int Open(void)
          int Close(void)
          int ReadFileInfo(void)
          int ReadProtocolInfo(void)
          int ReadADCInfo(void)
          int ReadDACInfo(void)
          int ReadEpochInfo(void)
          int ReadEpochInfoPerDAC(void)
          int ReadStrings(void)
          int GetString(char*, int)
          char* GetString(int)
          int ReadSynchArray(void)
          int ReadAllSections(void)
          int ReadData(void)
          float *ReadFloatData(void)
          short *ReadIntData(void)

import numpy as np

cdef class ABF2:
     cdef ABF *thisptr                # holds a C++ instance for wrapping
     def __cinit__(self, str fname):
          self.thisptr = new ABF(fname)
     def __dealloc__(self):
          def self.thisptr
     def Open(self):
          return self.thisptr.Open()
     def Close(self):
          return self.thisptr.Close()
     def ReadFileInfo(self):
          return self.thisptr.ReadFileInfo()
     def ReadProtocolInfo(self):
          return self.thisptr.ReadProtocolInfo()
     def ReadADCInfo(self):
          return self.thisptr.ReadADCInfo()
     def ReadDACInfo(self):
          return self.thisptr.ReadDACInfo()
     def ReadEpochInfo(self):
          return self.thisptr.ReadEpochInfo()
     def ReadEpochInfoPerDAC(self):
          return self.thisptr.ReadEpochInfoPerDAC()
     def ReadStrings(self):
          return self.thisptr.ReadStrings()
     def GetString(self, int i):
          return self.thisptr.GetString(i)
     def ReadSynchArray(self):
          return self.thisptr.ReadSynchArray()
     def ReadAllSections(self):
          return self.thisptr.ReadAllSections()
     def ReadData(self):
          if not self.thisptr.FileInfo: # int data
               self.thisptr.ReadFileInfo()
          if self.thisptr.FileInfo.nDataFormat == 0:
               data = np.array(self.thisptr.ReadIntData(), float)
          else if self.thisptr.FileInfo.nDataFormat == 1:
               data = np.array(self.thisptr.ReadFloatData(), float)
          else:
               print "Wrong data format, must be int or float data"
               self.thisptr.Close()
          return data
