import numpy as np
from numpy cimport ndarray, NPY_DOUBLE, npy_intp, import_array

import_array()

cdef extern from "numpy/arrayobject.h":
     cdef object PyArray_SimpleNewFromData(int nd, npy_intp *dims,
                                           int typenum, void *data)

cdef extern from "axon_structs.h":
     ctypedef struct ABF_Section:
          long long llNumEntries
          unsigned int uBytes
          unsigned int uBlockIndex
     ctypedef struct ABF_FileInfo:
          # ABF 1 sections
          ABF_Section DataSection
          ABF_Section TagSection
          ABF_Section ScopeSection
          ABF_Section DeltaSection
          ABF_Section VoiceTagSection
          ABF_Section SynchArraySection
          ABF_Section AnnotationSection
          ABF_Section StatsSection

cdef extern from "axon.h":
     cdef cppclass ABF:
          ABF_FileInfo FileInfo
          ABF()
          ABF(char*)
          int Open(char*)
          int Open()
          int Close()
          int ReadFileInfo()
          int ReadProtocolInfo()
          int ReadADCInfo()
          int ReadDACInfo()
          int ReadEpochInfo()
          int ReadEpochInfoPerDAC()
          int ReadStrings()
          int GetString(char*, int)
          char* GetString(int)
          int ReadSynchArray()
          int ReadAllSections()
          int ReadData()
          float *ReadFloatData()
          short *ReadIntData()

cdef class ABF2:
     cdef ABF *thisptr                # holds a C++ instance for wrapping
     def __cinit__(self, bytes fname):
          self.thisptr = new ABF(fname)
     def __dealloc__(self):
          del self.thisptr
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
          self.ReadFileInfo()
          cdef npy_intp dims = self.thisptr.FileInfo.DataSection.llNumEntries

          if self.thisptr.FileInfo.nDataFormat == 0:
               data = PyArray_SimpleNewFromData(1, &dims, NPY_DOUBLE,
                                                self.thisptr.ReadIntData())
          elif self.thisptr.FileInfo.nDataFormat == 1:
               data = PyArray_SimpleNewFromData(1, &dims, NPY_DOUBLE,
                                                self.thisptr.ReadFloatData())
          else:
               print "Wrong data format, must be int or float data"
               self.Close()
          return data

     property FileInfo:
          def __get__(self): return self.thisptr.FileInfo
