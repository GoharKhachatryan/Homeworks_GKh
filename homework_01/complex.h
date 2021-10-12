#pragma once
#include <iostream>

class Complex{
private:
	int real;							//creating the real part of a complex number
	int imaginary;							//creating the imaginary part of a complex number
	friend Complex operator*(int number,const Complex& object);	//operator* for constant and complex number
public:
	//Defining the constructors and methods, which will be realised in complex.cpp
	Complex();	//Default constructor
	Complex(int a, int b); //Constructor
	Complex(const Complex &other);	//Copy constructor
	const Complex& operator=(const Complex &other); //operator=
	Complex operator+(const Complex &object);	//operator+
	Complex operator-(const Complex &object);	//operator-
	Complex operator*(int number);			//operator* for complex number and constant
	double abs_value();				//returns the absolute value of a complex number
	void print_num() const;				//prints the real and imaginary parts of a complex number

};
void sort_array(Complex arr[],const int n);		//Sorts the array of complex numbers by their absolute value
void print_array(Complex arr[],const int n);			//Prints the array of complex numbers
