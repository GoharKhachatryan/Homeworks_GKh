all: output

output: Main.o complex.o
	g++ Main.o complex.o -o output

Main.o: Main.cpp
	g++ -c Main.cpp

complex.o: complex.cpp complex.h
	g++ -c complex.cpp

clean:
	rm *.o output
