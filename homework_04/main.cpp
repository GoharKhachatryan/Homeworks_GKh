#include <iostream>
#include "matrix.h"


int main(){
	int a = 100;
	int b = 100;
	Matrix mat(a, b);
	mat.Init();
	long int res = mat.Sum();
	long int res4 = mat.SumParallel(4);
	long int res8 = mat.SumParallel(8);
	long int res16 = mat.SumParallel(16);
	long int res100 = mat.SumParallel(100);
	std::cout << "Function Sum =>"  << res << std::endl;
	std::cout << "Number of threads = 4 => "  << res4 << std::endl;
	std::cout << "Number of threads = 8 => "  << res8 << std::endl;
	std::cout << "Number of threads = 16 => "  << res16 << std::endl;
	std::cout << "Number of threads = 100 => "  << res100 << std::endl;

	return 0;
}
