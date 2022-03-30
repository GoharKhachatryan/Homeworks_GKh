all:		output


output:		parallel.o
		g++ parallel.o -o output

parallel.o:	parallel.cpp
		g++ -c parallel.cpp

clean:
		rm *.o output
