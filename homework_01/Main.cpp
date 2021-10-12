#include <iostream>
#include "complex.h"
int main(){
	const int n = 4; 
	Complex array[n] = {Complex(2, 2), Complex(10, 10), Complex(1, 1), Complex(3, 4)};	//array to test sort
	sort_array(array, n);
	print_array(array, n);
	return 0;
}

