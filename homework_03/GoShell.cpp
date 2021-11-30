#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cerrno>
#include <sys/wait.h>
#include <string>
#include <string.h>

// Function to execute the command
void execute(char** arguments){
	pid_t pid = fork();
	// The cases for pid' s value
	if(pid == -1){
		std::cerr << "The forking was failed!" << std::endl;
		exit(errno);
	}
	else if(pid == 0){
		if(execvp(arguments[0], arguments) < 0){
			std::cerr << "The command's execution was failed" << std::endl;
		}
		exit(0);
	}
	else{
		// Waiting for child's termination and returning
		wait(NULL);
		return;
	}
}

// function to parse space symbols in inputStr

void parse(char* str, char** result){
	int i = 0;
	for(i = 0; i < 100; i++){
		result[i] = strsep(&str, " ");
		// if the strsep has ended
		if(result[i] == NULL){
			break;
		}
		if(strlen(result[i]) == 0){
			i--;
		}
	}

}


int main(){

	// Creating the necessary arguments
	char* arguments[100];
	char inputStr[1000];
	std::cout << "Welcome to GoShell! Insert the command only or command with arguments" << std::endl
	       	<< "Press ctrl+c to exit, because EXIT wasn't implemented" << std::endl;

	// Starting a loop
	while(1 == 1){
		std::cout << "Insert in GoShell>>";
		std::cin.getline(inputStr, 100);
		// Handling the "space" symbols by parsing inputStr
		parse(inputStr, arguments);
		// Execute the command
		execute(arguments);
	}
	return 0;
}
