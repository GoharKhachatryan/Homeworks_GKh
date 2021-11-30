all:		output

output:		GoShell.o
		g++ GoShell.o -o output
GoShell.o:	GoShell.cpp
		g++ -c GoShell.cpp
clean:
		rm *.o output
