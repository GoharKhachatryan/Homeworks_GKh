
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <cerrno>
int main(){

        // defining some arguments
        const size_t size = getpagesize();
        const char* name = "prod-cons-buffer";
        const char* sem1 = "full";
        const char* sem2 = "empty";
        // one mutex for critical section
        const char* sem3 = "mutex";
        // file descriptor for shared memory file
        int fdShm;
        // pointer for shared memory file
        char* ptr;
        // semaphores
        sem_t *full, *empty, *mutex;

        // creating shared memory with shm_open
        fdShm = shm_open(name, O_CREAT | O_RDWR, 0666);
        // the size of the shared memory segment must be 4096
        ftruncate(fdShm, size);
        // memory mapping
        ptr = (char*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdShm, 0);

        // creating and then opening semaphores
        full = sem_open(sem1, O_CREAT, 0666, 0);
        empty = sem_open(sem2, O_CREAT, 0666, size);
        mutex = sem_open(sem3, O_CREAT, 0666, 1);
        while(true){
                sem_wait(full);
                sleep(std::rand() % 10);
                sem_wait(mutex);
                // reading the secret message
                char buff;
		int rb = read(fdShm, &buff, 1);
		if(rb < 0){
                        std::cerr << "Error in reading from source" << std::endl;
                        exit(errno);
                }
		else{
			ptr -= 2;
	                std::cout << "I have read the " << buff << std::endl;
		}
                sem_post(mutex);
                sem_post(empty);
	}

	// closing semaphores
        sem_close(full);
        sem_close(empty);
        sem_close(mutex);
        // unlinking semaphores
        sem_unlink(sem1);
        sem_unlink(sem2);
        sem_unlink(sem3);
        // unmapping shared memory
        munmap(ptr, size);
        // closing
        close(fdShm);
        // unlinking
        shm_unlink(name);

        return 0;
}

