all: output

output: copy.o
	g++ copy.o -o output
copy.o: copy.cpp
	g++ -c copy.cpp
clean:
	rm *.o output
