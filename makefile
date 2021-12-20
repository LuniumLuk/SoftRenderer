CC = g++
CLANG = clang++
CFLAGS = -g -std=c++11 -Wall -Wextra
OBJCFLAGS  := -framework Cocoa

TARGET = viewer

SOURCEDIR  := src
INCLUDEDIR := src
BUILDDIR   := build
SOURCES    := $(wildcard $(addprefix $(SOURCEDIR)/, *.cpp))
OBJECTS    := $(addprefix $(BUILDDIR)/, $(notdir $(SOURCES:.cpp=.o)))
INCLUDES   := $(addprefix -I, $(wildcard $(addprefix $(INCLUDEDIR)/, *.hpp)))
# for MacOS
MACSOURCES := $(SOURCEDIR)/mac.mm
MACOBJECTS := $(filter-out build/main.o, $(OBJECTS))

RM         := rm -f
MD         := mkdir -p

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@$(MD) $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

debug: clean debug_compile
debug_compile: CFLAGS += -DDEBUG
debug_compile: $(TARGET);

info :
	@echo --- MAKEFILE -------------------
	@echo TARGET   : [ $(TARGET) ]
	@echo SOURCES  : [ $(SOURCES) ]
	@echo INCLUDES : [ $(INCLUDES) ]
	@echo OBJECTS  : [ $(OBJECTS) ]
	@echo --- MAC ------------------------
	@echo SOURCES  : [ $(MACSOURCES) ]
	@echo OBJECTS  : [ $(MACOBJECTS) ]
	@echo --------------------------------

.PHONY: clean
clean:
	@$(RM) $(TARGET)
	@$(RM) $(OBJECTS)
	@echo --- CLEAN COMPLETE -------------

mac : $(MACSOURCES) $(MACOBJECTS)
	@$(CLANG) $(OBJCFLAGS) $(CFLAGS) -o $(TARGET) $(MACSOURCES) $(MACOBJECTS) $(INCLUDES)