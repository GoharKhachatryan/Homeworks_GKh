#include "matrix.h"
#include <iostream>
#include <pthread.h>
#include <cerrno>

// function to allocate memory for matrix
void Matrix::allocateMem(){
	// creating an array which will contain all of the elements from matrix in array form
	elementarr = new int[rows * cols];
	m = new int*[rows];
	for(int i = 0; i < rows; ++i){
		m[i] = new int[cols];
	}
}

// Constructor that gives matrix' elements 0 values
Matrix::Matrix(int row, int col) : rows(row), cols(col){
	allocateMem();
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){
			m[i][j] = 0;
		}
	}
	// giving our array 0s
	for(int i = 0; i < rows * cols; ++i){
		elementarr[i] = 0;
	}
}

// Init function realization
void Matrix::Init(){
	int k = 0;
	for(int i = 0; i < rows; ++i){
                for(int j = 0; j < cols; ++j){
			// giving the elements random values from 1-20
			// and copying this values to array
                        m[i][j] = 1 + rand() % 20;
			elementarr[k] = m[i][j];
			k++;
                }
        }
}

// Destructor
Matrix::~Matrix(){
	for(int i = 0; i < rows; ++i){
		delete[] m[i];
	}
	delete[] m;
	delete[] elementarr;

}
// This function just counts the sum of matrix' elements with 2 loops
long int Matrix::Sum(){
	long int answer = 0;
	for(int i = 0; i < rows; ++i){
                for(int j = 0; j < cols; ++j){
                        answer+= m[i][j];
                }
        }
	return answer;
}

// because of copying the matrix elements into array, I used the algorithm
// to count the sum of an array with some changes

// creating struct args
struct args{
	Matrix* arr;
	int begin;
	int count;
};

// friend function to count the sum of a submatrix (or subarray)
void* subSum(void* arg){
	args* arguments = (args*) arg;
	long int result = 0;
	for(int i = arguments->begin; i < arguments->begin + arguments->count; ++i){
		result += arguments->arr->elementarr[i];
	}
	return (void*) (new long int(result));
}

// function to count the sums parallely using threads
long int Matrix::SumParallel(int thn){
	long int sum = 0;
	// creating an array to save thn arguments
	args* argumentsArr = new args[thn];
	// creating an array to save thn threads
	pthread_t* threadsArr = new pthread_t[thn];
	// creating threads
	for(int i = 0; i < thn; ++i){
		argumentsArr[i].begin = i*(rows * cols / thn);
		argumentsArr[i].count = (rows * cols) / thn;
		argumentsArr[i].arr = this;
		int t = pthread_create(&threadsArr[i], NULL, subSum, &argumentsArr[i]);
		// errors in creating a thread
		if(t != 0){
			std::cerr << "Something went wrong during the tread creation" << std::endl;
			exit(errno);
		}
	}

	void* subsum;

	// joining the threads
	for(int i = 0; i < thn; ++i){
		int j = pthread_join(threadsArr[i], &subsum);
		// error in joining
		if(j != 0){
			std::cerr << "Something went wrong during the joining" << std::endl;
			exit(errno);
		}
		long int* subsum_int = (long int*) subsum;
		sum += *subsum_int;
		delete subsum_int;
	}
	// releasing the memory for arrays
	delete[] threadsArr;
	delete[] argumentsArr;
	return sum;
}
