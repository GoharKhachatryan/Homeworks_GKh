#pragma once
#include <iostream>

class Matrix{
private:
	int rows;
	int cols;
	int** m;
	int* elementarr;
	void allocateMem();
	friend void* subSum(void* arg);
public:
	Matrix(int row, int col);
	~Matrix();
	void Init();
	long int Sum();
	long int SumParallel(int thn);
};
