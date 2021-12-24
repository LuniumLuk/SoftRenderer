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
HEADERS    := $(wildcard $(addprefix $(INCLUDEDIR)/, *.hpp))
# MacOS Compile
MACSOURCES := $(SOURCEDIR)/mac.mm
MACOBJECTS := $(filter-out build/main.o, $(OBJECTS))

RM         := rm -f
MD         := mkdir -p

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(BUILDDIR)/platform.o: $(SOURCEDIR)/mac.mm $(HEADERS)
	@echo $(CLANG) $(OBJCFLAGS) $(CFLAGS) $(INCLUDES) -o $@ -c $<
	@$(CC) $(OBJCFLAGS) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(HEADERS)
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

mac : mac_compile

mac_compile: $(OBJECTS)
	@$(CLANG) -o $(TARGET) $(OBJCFLAGS) $(CFLAGS) $(SOURCEDIR)/mac.mm $(OBJECTS)
# mac_compile: CC = clang++
# mac_compile: clean $(BUILDDIR)/platform.o $(OBJECTS) $(TARGET);