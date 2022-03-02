#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

// my function to handle signal
void sigHandler(int sigNumber, siginfo_t* info, void* myContext){
	// works for SIGUSR1 as requested
	if(sigNumber == SIGUSR1){
		// casting
		ucontext_t *context = (ucontext_t*) myContext;

		// getting the values of registers
        	unsigned long eaxValue = context->uc_mcontext.gregs[REG_RAX];
        	unsigned long ebxValue = context->uc_mcontext.gregs[REG_RBX];
        	unsigned long eipValue = context->uc_mcontext.gregs[REG_RIP];

		// struct for getting the username
		struct passwd *usrnm;
		usrnm = getpwuid(info->si_uid);

		// printing stuff
        	std::cout << "The PID is " << info->si_pid << std::endl
                	  << "The UID is " << info->si_uid << std::endl
			  << "The username is " << usrnm->pw_name << std::endl
                  	  << "The EAX register value " << eaxValue << std::endl
                  	  << "The EBX register value " << ebxValue << std::endl
                  	  << "The EIP register value " << eipValue << std::endl;
	}

	return;

}

int main(){

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sigHandler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	while(true){
		sleep(10);
	}

	return 0;
}
