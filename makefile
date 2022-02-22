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
# INCLUDES   := $(addprefix -I, $(wildcard $(addprefix $(INCLUDEDIR)/, *.hpp)))
INCLUDES   := -I$(INCLUDEDIR)
HEADERS    := $(wildcard $(addprefix $(INCLUDEDIR)/, *.hpp)) $(PLATDIR)/platform.hpp
# MacOS Compile
MACSOURCES := $(SOURCEDIR)/macos.mm
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
RMDIR      := rm -f
MD         := mkdir -p

# reference : https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
    CFLAGS 	+= -D WIN32
	MD 		:= mkdir
	RM 		:= del /s /q
	RMDIR 	:= rmdir /s /q
	TARGET 	:= viewer.exe
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

prepare:
	@echo $(RM) $(RMDIR) $(MD)
	@echo $(CFLAGS)
	@if not exist $(BUILDDIR) $(MD) $(BUILDDIR)

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(HEADERS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# Debug option
debug: CFLAGS += -DDEBUG

# help option
help:
	@echo --- MAKEFILE OPTIONS -----------
	@echo "macos : compile for MacOS App"
	@echo "win32 : compile for Win32 App"
	@echo "  dll : compile for DLL"
	@echo " test : compile for test script $(TESTSOURCE)"
	@echo " help : show makefile options"
	@echo "debug : add '#define DEBUG'"
	@echo "clean : clean target, bin/ and build/"

show:
	@echo $(OBJECTS)

.PHONY: clean
clean:
	@if exist $(TARGET) $(RM) $(TARGET)
	@if exist $(BUILDDIR) $(RMDIR) $(BUILDDIR)
	@echo --- CLEAN COMPLETE -------------

# MacOS compile options
macos: prepare macos_compile

macos_compile: $(OBJECTS)
	@$(CLANG) -o $(TARGET) $(OBJCFLAGS) $(CFLAGS) $(PLATDIR)/macos.mm $(OBJECTS)

# Win32 compile options
win32: prepare win32_compile

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
