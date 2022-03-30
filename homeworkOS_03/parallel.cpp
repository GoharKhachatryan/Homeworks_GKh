// works when n is divided by m
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cerrno>

int main(){
	// defining some arguments
	unsigned int n, m;
	long answerParent = 0;
	long answerChildren = 0;
	unsigned int start, finish;
	unsigned int div = 0;

	std::cin >> n >> m;

	// main array
	int* arr = new int [n];
	for(unsigned int i = 0; i < n; ++i){
		arr[i] = rand() % 30 + 123;
		answerParent += arr[i];
	}

	// matrix for pipes (2*m pipes)
	int** pipeFd = new int* [2 * m];
	for(unsigned int i = 0; i < 2 * m; ++i){
		pipeFd[i] = new int[2];
	}

	// creating pipes
	for(unsigned int i = 0; i < 2 * m; ++i){
		if(pipe(pipeFd[i]) == -1){
			std::cerr << "Error in pipe() function" << std::endl;
			exit(errno);
		}
	}

	// argument to divide array into equal parts
	div = n / m;

	// creating m processes
	for(unsigned int childNumber = 0; childNumber < m; ++childNumber){

		// defining indexes
		start = childNumber * div;
		finish = start + div;

		// writing indexes into pipe
		// for process with childNumber, pipeFd[2 * childNumber] is for indexes
		// and pipeFd[2 * childNumber + 1] is for result
		if(write(pipeFd[2 * childNumber][1], &start, sizeof(start)) == -1){
                                std::cerr << "Error in writing start index from parent process" << std::endl;
                                exit(errno);
                        }
		if(write(pipeFd[2 * childNumber][1], &finish, sizeof(finish)) == -1){
                                std::cerr << "Error in writing finish index from parent process" << std::endl;
                                exit(errno);
                        }

		// forking
		int child = fork();

		// handling errors during fork()
		if(child < 0){
			std::cerr << "Error in fork()" << std::endl;
			exit(errno);
		}

		// if we are in child process:
		else if(child == 0){
			// child process won't write in its index pipe
			close(pipeFd[2 * childNumber][1]);
			// child process won't read from its result pipe
			close(pipeFd[2 * childNumber + 1][0]);

			// reading indexes from index pipe
			if(read(pipeFd[2 * childNumber][0], &start, sizeof(start)) == -1){
                                std::cerr << "Error in reading start index from pipe in child process" << std::endl;
                                exit(errno);
                        }
			if(read(pipeFd[2 * childNumber][0], &finish, sizeof(finish)) == -1){
                                std::cerr << "Error in reading finish index from pipe in child process" << std::endl;
                                exit(errno);
                        }

			// now child process won't read from index pipe
			close(pipeFd[2 * childNumber][0]);

			// argument for child's partial sum(long 'cuz we don't know the subarray's length)
			long partialSum = 0;

			// calculating the partial sum
			for(unsigned int i = start; i < finish; ++i){
				partialSum += arr[i];
			}
			std::cout << "Partial Sum by process with pid: " << getpid() << " "
				  << "is: " << partialSum << std::endl;

			// child process will prite the result in its result pipe
			if(write(pipeFd[2 * childNumber + 1][1], &partialSum, sizeof(partialSum)) == -1){
				std::cerr << "Error in writing in pipe" << std::endl;
				exit(errno);
			}
			// closing the result pipe for writing
			close(pipeFd[childNumber * 2 + 1][1]);
			// exiting the loop for child process
			exit(0);
		}

		// if we are in paren process
		else if(child > 0){
			// the parent process won't need to read or write in index pipe anymore
			close(pipeFd[childNumber * 2][0]);
			close(pipeFd[childNumber * 2][1]);
		}

	}


	// waiting for all the children
	wait(NULL);

	// calculating the answer from the children
	// therefore we will need only the result pipes
	for(unsigned int i = 1; i < 2 * m; i += 2){
		long sumfromChild = 0;
		// no writing from parent
		close(pipeFd[i][1]);
		// reading the child's answer
		if(read(pipeFd[i][0], &sumfromChild, sizeof(long)) == -1){
			std::cerr << "Error in reading from pipe in parent" << std::endl;
			exit(errno);
		}
		// no need to read something now
		close(pipeFd[i][0]);

		answerChildren += sumfromChild;
	}
	// The sum that was calculated by parent only to check the answer
	std::cout << "The sum from Parent: " << answerParent << std::endl;
	// The answer from parallel children
	std::cout << "The sum from Children: " << answerChildren << std::endl;

	// free used memory
	delete[] arr;
	for(unsigned int i = 0; i < 2 * m; i++){
		delete[] pipeFd[i];
	}
	delete[] pipeFd;

	return 0;

}
