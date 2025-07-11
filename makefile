CC = g++
CLANG = clang++
CFLAGS = -g -std=c++11 -std=c++0x -Wall -Wextra
OBJCFLAGS  := -framework Cocoa

MAIN       := main

SOURCEDIR  := src
COREDIR    := $(SOURCEDIR)/core
PLATDIR    := $(SOURCEDIR)/platform
SAMPLEDIR  := $(SOURCEDIR)/sample
INCLUDEDIR := src
BUILDDIR   := build
DLLDIR     := bin

# Core sources (moved from src/ to src/core/)
CORESRCS   := $(wildcard $(addprefix $(COREDIR)/, *.cpp))
COREOBJS   := $(addprefix $(BUILDDIR)/core/, $(notdir $(CORESRCS:.cpp=.o)))

# Platform sources
PLATSRCS   := $(wildcard $(addprefix $(PLATDIR)/, *.cpp))
PLATOBJS   := $(addprefix $(BUILDDIR)/platform/, $(notdir $(PLATSRCS:.cpp=.o)))

# Sample sources
SAMPLESRCS := $(wildcard $(addprefix $(SAMPLEDIR)/, *.cpp))
SAMPLEOBJS := $(addprefix $(BUILDDIR)/sample/, $(notdir $(SAMPLESRCS:.cpp=.o)))

# All objects
OBJECTS    := $(COREOBJS) $(PLATOBJS) $(SAMPLEOBJS)

# Include directories
INCLUDES   := -I$(INCLUDEDIR) -I$(COREDIR) -I$(PLATDIR) -I$(SAMPLEDIR)

# Headers
HEADERS    := $(wildcard $(addprefix $(COREDIR)/, *.hpp)) $(wildcard $(addprefix $(PLATDIR)/, *.hpp)) $(wildcard $(addprefix $(SAMPLEDIR)/, *.hpp))

# MacOS Compile
MACSOURCES := $(PLATDIR)/macos.mm
MACOBJECTS := $(filter-out build/sample/$(MAIN).o, $(OBJECTS))

# DLL Compile
DLLOBJECTS := $(filter-out build/sample/$(MAIN).o, $(OBJECTS))

TARGET     = viewer
SAMPLE        = sample
DLLTARGET  = $(DLLDIR)/lurdr.dll

RM         := rm -f
RMDIR      := rm -f
MD         := mkdir -p

# reference : https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
    MD         := mkdir
    RM         := del /s /q
    RMDIR      := rmdir /s /q
    TARGET     := viewer.exe
    CFLAGS  += -fopenmp
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CFLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CFLAGS += -D ARM
    endif
endif

# all is set to default compile for MacOS
all: macos

win32_prepare:
    @if not exist $(BUILDDIR) $(MD) $(BUILDDIR)
    @if not exist $(BUILDDIR)\core $(MD) $(BUILDDIR)\core
    @if not exist $(BUILDDIR)\platform $(MD) $(BUILDDIR)\platform
    @if not exist $(BUILDDIR)\sample $(MD) $(BUILDDIR)\sample

macos_prepare:
    @$(MD) $(BUILDDIR)
    @$(MD) $(BUILDDIR)/core
    @$(MD) $(BUILDDIR)/platform
    @$(MD) $(BUILDDIR)/sample

$(TARGET): $(OBJECTS)
    @$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Core object files
$(BUILDDIR)/core/%.o: $(COREDIR)/%.cpp $(HEADERS)
    @$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# Platform object files
$(BUILDDIR)/platform/%.o: $(PLATDIR)/%.cpp $(HEADERS)
    @$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# Sample object files
$(BUILDDIR)/sample/%.o: $(SAMPLEDIR)/%.cpp $(HEADERS)
    @$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# Debug option
debug: CFLAGS += -DDEBUG

# help option
help:
    @echo --- MAKEFILE OPTIONS -----------
    @echo "macos : compile for MacOS App"
    @echo "win32 : compile for Win32 App"
    @echo "  dll : compile for DLL"
    @echo " sample : compile for sample script $(SAMPLESOURCE)"
    @echo "shadow : compile shadow mapping demo"
    @echo " help : show makefile options"
    @echo "debug : add '#define DEBUG'"
    @echo "clean : clean target, bin/ and build/"

show:
    @echo $(SAMPLESRCS)
    @echo $(SAMPLEOBJS)

.PHONY: clean
clean:
    @if exist $(TARGET) $(RM) $(TARGET)
    @if exist $(BUILDDIR) $(RMDIR) $(BUILDDIR)
    @echo --- CLEAN COMPLETE -------------

# Shadow mapping demo
shadow: win32_prepare shadow_compile

shadow_compile: $(OBJECTS)
    @$(CC) -o shadow_demo.exe $(CFLAGS) $(PLATDIR)/win32.cpp $(OBJECTS) -lgdi32

# MacOS compile options
macos: macos_prepare macos_compile

macos_compile: $(OBJECTS)
    @$(CLANG) -o $(TARGET) $(OBJCFLAGS) $(CFLAGS) $(PLATDIR)/macos.mm $(OBJECTS)

# Win32 compile options
win32: win32_prepare win32_compile

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

sample: dll_compile
    @$(CC) $(CFLAGS) -o $(SAMPLE) $(SAMPLESOURCE) $(DLLTARGET)
