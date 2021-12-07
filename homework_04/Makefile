all:		output


output:		main.o matrix.o
		g++ main.o matrix.o -lpthread -o output

main.o:		main.cpp matrix.h
		g++ -c main.cpp

matrix.o:	matrix.cpp matrix.h
		g++ -c matrix.cpp

clean:
		rm *.o output
