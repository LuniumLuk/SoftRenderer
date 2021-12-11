OBJECTS = main.o lmath.o
CC = g++
CFLAGS = -g -std=c++11
SHELL = cmd.exe

viewer : $(OBJECTS)
	$(CC) $(CFLAGS) -o viewer $(OBJECTS)

lmath.o : src/lmath.cpp include/lmath.h
	$(CC) $(CFLAGS) -c src/lmath.cpp
main.o : src/main.cpp include/lmath.h
	$(CC) $(CFLAGS) -c src/main.cpp

.PHONY : clean
clean :
	rm *.o
