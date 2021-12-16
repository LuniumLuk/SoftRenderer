OBJECTS = main.o maths.o image.o mesh.o darray.o graphics.o
MACOBJS = maths.o image.o mesh.o darray.o graphics.o
INCLUDES = include/maths.hpp include/image.hpp include/global.hpp include/mesh.hpp include/darray.hpp include/graphics.hpp include/buffer.hpp 
SRCS = src/main.cpp src/maths.cpp src/image.cpp src/mesh.cpp src/graphics.cpp
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

mesh.o : src/mesh.cpp include/mesh.hpp
	$(CC) $(CFLAGS) -c src/mesh.cpp
maths.o : src/maths.cpp include/maths.hpp
	$(CC) $(CFLAGS) -c src/maths.cpp
image.o : src/image.cpp include/image.hpp
	$(CC) $(CFLAGS) -c src/image.cpp
graphics.o : src/graphics.cpp include/graphics.hpp
	$(CC) $(CFLAGS) -c src/graphics.cpp
main.o : src/main.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $(SRCS)

.PHONY : clean
clean :
	rm *.o

macos : src/macos.mm $(MACOBJS) $(INCLUDES)
	clang++ $(OBJCFLAGS) $(CFLAGS) src/macos.mm -o viewer $(MACOBJS)