# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif
SRC_FILES=src/unity.c 
INC_DIRS=-Isrc
SYMBOLS=-DTEST

ifeq ($(OS),Windows_NT)
	CLEANUP = del /F /Q build\*
else
	CLEANUP = rm -f build/*.o
endif

all: clean default

default:
	mkdir build
	rake -vf unity/rakefile.rb
#	ruby auto/generate_test_runner.rb test/TestProductionCode.c  test/no_ruby/TestProductionCode_Runner.c
#	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
#	$(TARGET)

clean:
	$(CLEANUP)
	rm -rf build
	
