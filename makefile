OBJECTS = main.o lmath.o
SRCS = src/main.cpp src/lmath.cpp
CC = g++
CFLAGS = -g -std=c++11
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
main.o : src/main.cpp include/lmath.h
	$(CC) $(CFLAGS) -c $(SRCS)

.PHONY : clean
clean :
	rm *.o
