#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <cerrno>
#define secret_message "b"
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
		sem_wait(empty);
		sleep(std::rand() % 10);
		sem_wait(mutex);
		// writing the secret message
		int wr = write(fdShm, secret_message, strlen(secret_message));
		if(wr < 0){
                        std::cerr << "Couldn't write in the destination file" << std::endl;
                        exit(errno);
                }
		else{
			ptr += strlen(secret_message);
                	std::cout << "I have written the " << secret_message << std::endl;
		}
		sem_post(mutex);
		sem_post(full);
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
