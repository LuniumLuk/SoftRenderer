OBJECTS = main.o lmath.o limage.o
INCLUDES = include/lmath.h include/limage.h include/lglobal.h
SRCS = src/main.cpp src/lmath.cpp src/limage.cpp
CC = g++
CFLAGS = -g -std=c++11
OBJCFLAGS = -framework Cocoa
# Windows
ifeq ($(OS), Windows_NT)
	SHELL = cmd.exe
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
endif

viewer : $(OBJECTS)
	$(CC) $(CFLAGS) -o viewer $(OBJECTS)

lmath.o : src/lmath.cpp include/lmath.h
	$(CC) $(CFLAGS) -c src/lmath.cpp
limage.o : src/limage.cpp include/limage.h
	$(CC) $(CFLAGS) -c src/limage.cpp
main.o : src/main.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $(SRCS)

.PHONY : clean
clean :
	rm *.o

macos : src/macos.mm limage.o
	clang++ $(OBJCFLAGS) $(CFLAGS) src/macos.mm limage.o -o viewer