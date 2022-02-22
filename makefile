CC = g++
CLANG = clang++
CFLAGS = -g -std=c++11 -std=c++0x -Wall -Wextra
OBJCFLAGS  := -framework Cocoa

MAIN	   := main

SOURCEDIR  := src
INCLUDEDIR := src
BUILDDIR   := build
DLLDIR	   := bin
TESTDIR	   := $(SOURCEDIR)/test
PLATDIR    := src/platform
SOURCES    := $(wildcard $(addprefix $(SOURCEDIR)/, *.cpp))
OBJECTS    := $(addprefix $(BUILDDIR)/, $(notdir $(SOURCES:.cpp=.o)))
INCLUDES   := $(addprefix -I, $(wildcard $(addprefix $(INCLUDEDIR)/, *.hpp)))
HEADERS    := $(wildcard $(addprefix $(INCLUDEDIR)/, *.hpp)) $(PLATDIR)/platform.hpp
# MacOS Compile
MACSOURCES := $(SOURCEDIR)/mac.mm
MACOBJECTS := $(filter-out build/$(MAIN).o, $(OBJECTS))
# DLL Compile
DLLOBJECTS := $(filter-out build/$(MAIN).o, $(OBJECTS))
# Test Compile
# modify here if to compile other file
TESTSOURCE := $(TESTDIR)/test.cpp

TARGET     = viewer
TEST 	   = test
DLLTARGET  = $(DLLDIR)/lurdr.dll

RM         := rm -f
MD         := mkdir -p

# all is set to default compile for MacOS
all: mac

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(HEADERS)
# @$(MD) $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# Debug option
debug: CFLAGS += -DDEBUG

# help option
help:
	@echo --- MAKEFILE OPTIONS -----------
	@echo "  mac : compile for MacOS"
	@echo "  dll : compile for DLL"
	@echo " test : compile for test script $(TESTSOURCE)"
	@echo " help : show makefile options"
	@echo "debug : add '#define DEBUG'"
	@echo "clean : clean target, bin/ and build/"

show:
	@echo $(OBJECTS)

.PHONY: clean
clean:
	@$(RM) $(TARGET)
	@$(RM) $(TEST)
	@$(RM) build/*
	@$(RM) bin/*
	@echo --- CLEAN COMPLETE -------------

# MacOS compile options
mac: CFLAGS += -DMACOS
mac: mac_compile

mac_compile: $(OBJECTS)
	@$(CLANG) -o $(TARGET) $(OBJCFLAGS) $(CFLAGS) $(PLATDIR)/mac.mm $(OBJECTS)

# Win32 compile options
win32: CFLAGS += -DWIN32
win32: win32_compile
 
win32_compile: $(OBJECTS)
	@$(CC) -o $(TARGET) $(CFLAGS) $(PLATDIR)/win32.cpp $(OBJECTS) -lgdi32

run:
	@$(TARGET)

# DLL compile options
dll: clean dll_compile

dll_compile: CFLAGS += -DBDLL
dll_compile: $(DLLTARGET)

$(DLLTARGET): $(DLLOBJECTS)
	@$(MD) $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -shared -o $@ $^

test: dll_compile
	@$(CC) $(CFLAGS) -o $(TEST) $(TESTSOURCE) $(DLLTARGET)
