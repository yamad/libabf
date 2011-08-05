#---------
#
# CppUTest Examples Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = axon
CPPUTEST_HOME = ~/src/upstream/cpputest

CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_WARNINGFLAGS = -Wall

CPP_PLATFORM = Gcc
PLATFORM = gcc

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.
#CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h
SRC_DIRS = \
  src\
  src/File
  #src/ABFFIO\
  #src/AxAbfFio32\
  #src/AxAtfFio32\
  #src/AxoUtils32\
  #src/AxonValidation\
  #src/Common

TEST_SRC_DIRS = \
  test

INCLUDE_DIRS =\
  .\
  src\
  src/File\
  src/ABFFIO\
  src/AxAbfFio32\
  src/AxAtfFio32\
  src/AxoUtils32\
  src/AxonValidation\
  src/Common\
  src/platforms/$(PLATFORM)\
  $(CPPUTEST_HOME)/include

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
