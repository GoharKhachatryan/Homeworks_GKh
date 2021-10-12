#include <iostream>
#include <cmath>
#include "complex.h"

Complex::Complex() : real(0), imaginary(0) {}			//Default constructor realization

Complex::Complex(int a, int b) : real(a), imaginary(b) {}	//Constructor realization

Complex::Complex(const Complex& other){				//Copy constructor realization
	real = other.real;
	imaginary = other.imaginary;
}

const Complex& Complex::operator=(const Complex& other){	//Operator= realization
	real = other.real;
	imaginary = other.imaginary;
	return *this;
}

Complex Complex::operator+(const Complex& object){		//Operator+ realization
	Complex result;
	result.real = real + object.real;
	result.imaginary = imaginary + object.imaginary;
	return result;
}

Complex Complex::operator-(const Complex& object){		//Operator- realization
	Complex result;
	result.real = real - object.real;
	result.imaginary = imaginary - object.imaginary;
	return result;
}

Complex Complex::operator*(int number){				//Operator* realization for complex number and int
	real = real * number;
	imaginary = imaginary * number;
	return *this;
}

Complex operator*(int number, const Complex& object){		//Operator* realization for int and complex number
	Complex result;
	result.real = number * object.real;
	result.imaginary = number * result.imaginary;
	return result;
}
double Complex::abs_value(){					//returns the absolute value of a complex number
	return sqrt(pow(real, 2 ) + pow(imaginary, 2));
}

void Complex::print_num() const{					//prints the real and imaginary parst of a number
	std::cout <<  real << " + "
		<< imaginary << "i"<< std::endl;
}

void sort_array(Complex arr[],const int n){			//Sorts the array of complex numbers by absolute value
	int i, j;
	double val1, val2;
	Complex temp;
	for(i = 0; i < n - 1; i++){
		for(j = 0; j < n - i -1; j++){
			val1 = arr[j].abs_value();
			val2 = arr[j + 1].abs_value();
			if(val1 > val2){
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void print_array(Complex arr[],const int n){				//Prints the array of complex numbers
	for(int i = 0; i < n; i++){
		arr[i].print_num();
	}
}
