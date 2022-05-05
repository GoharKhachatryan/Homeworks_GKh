all:		output

output:		http.o
		g++ http.o -o output

http.o:		http.cpp
		g++ -c http.cpp

clean:
		rm *.o output
