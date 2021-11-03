#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#define buffersize      1024
int main(int argc, char** argv){
        // if the arguments were not enough
        if(argc < 3){
                std::cerr << "The arguments are not enough" << std::endl;
                exit(1);
        }

        // getting the source file's path
        const char* sfile = argv[1];

        // open the source file
        int fd1 = open(sfile, O_RDONLY);

        // couldn't open the file
        if(fd1 < 0){
                std::cerr << "Couldn't open the source file" << std::endl;
                exit(errno);
        }

        // getting the destination file's path
        const char* dfile = argv[2];

        // open the destination file
        int fd2 = open(dfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

        // couldn't open the destination file
        if(fd2 < 0){
                std::cerr << "Couldn't open the destination file" << std::endl;
                exit(errno);
        }

        // the logical size of the source file
        off_t source_logical = lseek(fd1, 0, SEEK_END);

        // error in lseek
        if(source_logical < 0){
                std::cerr << "Error in lseek" << std::endl;
                exit(errno);
        }

        std::cout << "The logical size of our source file is: " << source_logical << std::endl;

        // bring the cursor to the beginning
        lseek(fd1, 0, SEEK_SET);

        // allocating buffer
        char* buffer = new char[buffersize + 1];

        // starting a loop for copy
        while(1 == 1){
                int readbytes = read(fd1, buffer, buffersize);

                // if couldn't read bytes
                if(readbytes < 0){
                        std::cerr << "Error in reading from source" << std::endl;
                        exit(errno);
                }

                // if the source file ended
                else if(readbytes == 0){
                        break;
                }

                // writing the readbytes in destination
                int writebytes = write(fd2, buffer, readbytes);

                // if couldn't write
                if(writebytes < 0){
                        std::cerr << "Couldn't write in the destination file" << std::endl;
                        exit(errno);
                }
        }

        //release memory
        delete[] buffer;

        // close the source and the destination
        int cl1 = close(fd1);
        int cl2 = close(fd2);


        return 0;
}

