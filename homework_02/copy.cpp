#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

//DOESN'T WORK PROPERLY

int main(int argc, char** argv){
    // if there isn't enough arguments
    if(argc < 3){
        std::cout << "No source and/or destination file" << std::endl;
        exit (1);
    }
    
    // getting the source file's path
    const char* sfile = argv[1];
    
    // opening the source file
    int fd1 = open(sfile, O_RDONLY);
    
    // if there was an error
    if(fd1 < 0){
        std::cout << "Couldn't open the souce file" << std::endl;
        exit(errno);
    }
    
    // getting the destination file's path
    const char* dfile = argv[2];
    
    // 1) open the destination file if it exists
    // 2) creare if it doesn't exist; 
    // 3) trunc if ti exists and has dataso
    int fd2 = open(dfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
    
    // if there was an error
    if(fd2 < 0){
        std::cout << "Couldn't open the destination file" << std::endl;
        exit(errno);
    }
    
    // the logical size of the source file will be 
    off_t source_log = lseek(fd1, 0, SEEK_END);
    
    // if there was an error during iseek
    if(source_log < 0){
        std::cout << "Couldn't find the logical size of the source file" << std::endl;
        exit(errno);
    }
    
    // if the logical size was found 
    std::cout << "The logical size of our source file is: " << source_log << std::endl;
    
    off_t source_hole = 0;
    size_t source_phys = 0, hole_size = 0;
    
    // The physical size of the source file will be
    while(hole_size + source_phys != source_log){
        off_t source_data = lseek(fd1, source_hole, SEEK_DATA);
        
        if(source_data < 0){
            std::cout << "Error in lseek" << std::endl;
            exit(errno);
        }
        off_t hole_amount = source_data - source_hole;
        
        source_hole = lseek(fd1, source_data, SEEK_HOLE);
        if(source_hole < 0){
            std::cout << "Error in lseek" << std::endl;
            exit(errno);
        }
        if(hole_amount > 0){
            hole_size = hole_size + hole_amount;
        }
        off_t data_amount = source_hole - source_data;
        if(data_amount > 0){
            source_phys = source_phys + data_amount;
        }
    }
    
    // print the physical size of the source file
    std::cout << "The physical size of our source file is: " << source_phys << std::endl;
    
    off_t dest_hole = 0, dest_log = 0;
    size_t dest_phys = 0;
    
    // A loop for copy
    while(1 == 1){
        // The size from the begging to the first data's start
        off_t dest_data = lseek(fd1, dest_hole, SEEK_DATA);
        if(dest_data < 0){
            std::cout << "Error in lseek" << std::endl;
            exit(errno);
        }
        // The size of a hole 
        off_t hole_amount = dest_data - dest_hole;
        
        // Copy the hole
        if(hole_amount > 0){
            dest_log = lseek(fd2, hole_amount, SEEK_END);
        }
        
        //If the end is reached in source file
        if(source_log == dest_log){
            break;
        }
        
        dest_hole = lseek(fd1, dest_data, SEEK_HOLE);
        if(dest_hole < 0){
            std::cout << "Error in lseek" << std::endl;
            exit(errno);
        }
        
        off_t data_amount = dest_hole - dest_data;
        
        // Copy data
        if(data_amount > 0){
            char* buffer = new char [data_amount + 1];
            dest_data = lseek(fd1, dest_data, SEEK_SET);
            if(dest_data < 0){
                std::cout << "Error in lseek" << std::endl;
                exit(errno);
            }
            int readbytes = read(fd1, buffer, data_amount);
            
            // if couldn't read data
            if(readbytes < 0){
                std::cout << "Couldn't read bytes" << std::endl;
                exit(errno);
            }
            
            int writebytes = write(fd2, buffer, readbytes);
            
            // if couldn't write data
            if(writebytes < 0){
                std::cout << "Couldn't write bytes" << std::endl;
                exit(errno);
            }
            delete[] buffer;
            
            // The logical size of the destination file will be
            dest_log = lseek(fd2, 0, SEEK_END);
            if(dest_log < 0){
                std::cout << "Error in lseek" << std::endl;
                exit(errno);
            }
            
            // The physical size of the destination file will be
            dest_phys = dest_phys + data_amount;
            
            // If the source file has ended
            if(source_log == dest_log){
                break;
            }
            
        }
        
    }
    std::cout << "The logical size of our destination file is: " << dest_log << std::endl;
    std::cout << "The physical size of our destination file is:" << dest_phys << std::endl;
    
    int close1 = close(fd1);
    int close2 = close(fd2);
  
    
    return 0;
}
