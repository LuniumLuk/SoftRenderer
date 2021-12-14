OBJECTS = main.o maths.o image.o mesh.o darray.o
INCLUDES = include/maths.h include/image.h include/global.h include/mesh.h include/darray.h
SRCS = src/main.cpp src/maths.cpp src/image.cpp src/mesh.cpp src/darray.cpp
CC = g++
CFLAGS = -g -std=c++11
OBJCFLAGS = -framework Cocoa
# Windows
ifeq ($(OS), Windows_NT)
	SHELL = cmd.exe
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        @echo "ARCHITECTURE AMD64"
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            @echo "ARCHITECTURE AMD64"
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            @echo "ARCHITECTURE x86"
            CCFLAGS += -D IA32
        endif
    endif
endif

viewer : $(OBJECTS)
	$(CC) $(CFLAGS) -o viewer $(OBJECTS)

mesh.o : src/mesh.cpp include/mesh.h
	$(CC) $(CFLAGS) -c src/mesh.cpp
maths.o : src/maths.cpp include/maths.h
	$(CC) $(CFLAGS) -c src/maths.cpp
image.o : src/image.cpp include/image.h
	$(CC) $(CFLAGS) -c src/image.cpp
darray.o : src/darray.cpp include/darray.h
	$(CC) $(CFLAGS) -c src/darray.cpp
main.o : src/main.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $(SRCS)

.PHONY : clean
clean :
	rm *.o

macos : src/macos.mm limage.o
	clang++ $(OBJCFLAGS) $(CFLAGS) src/macos.mm limage.o -o viewer