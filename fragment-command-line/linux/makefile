PROJ = fragment
CC = g++
LIBS = -l glfw \
	   -l GL \
	   -l IL \
	   -l GLEW \
	   -l GLU 
INCS = fragment.h

$(PROJ).exe : main.o
	$(CC) main.o -o $(PROJ) $(LIBS)

main.o : main.cpp 
	$(CC) -c main.cpp 

main.o : $(INCS)
main.o : makefile
