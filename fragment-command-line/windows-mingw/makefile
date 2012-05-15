PROJ = fragment
CC = mingw32-g++
LIBDIR = C:\MinGW\lib
INCDIR = C:\MinGW\include
LIBS = -l glfw \
	   -l opengl32 \
	   C:\MinGW\lib\DevIL.lib \
	   -l glew32 \
	   -l glu32 
INCS = fragment.h

$(PROJ).exe : main.o
	$(CC) main.o -o $(PROJ).exe -L $(LIBDIR) $(LIBS)

main.o : main.cpp 
	$(CC) -c main.cpp 

main.o : $(INCS)
main.o : makefile
