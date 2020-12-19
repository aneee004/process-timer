#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    double* ptr; // shared memory pointer
    struct timeval current_time; // to store the current time
    pid_t pid; // to store the child process id
    int SIZE, FD, i; // shared region size, shm descriptor, iterator

    SIZE = sizeof(double); // size of the shared memory space
    FD = shm_open("start_time", O_CREAT | O_RDWR, 0666); // opening the shared memory
    ftruncate(FD, SIZE); // resizing the shared memory to the required size
    ptr = (double*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FD, 0); // mapping the shared memory locaiton to the pointer
    pid = fork();

    if (pid < 0) { // fork failure
        printf("Fork failure. Please try again.\n");
    }
    else if (pid == 0) { // child process
        if (argc > 1) { // if a command is passed
            // prepare the argv list to pass
            for (int i = 1; i < argc; i++) {
                argv[i-1] = argv[i];
            }
            argv[argc - 1] = NULL;
            // write cur time into shared memory space
            gettimeofday(&current_time, NULL); // get the current time
            *ptr = current_time.tv_sec + 1e-6 * current_time.tv_usec;
            //  replace the child process with the command line instruction
            if (execvp(argv[0], argv) == -1) {
                printf("Error running your command.\n"); // in case an error occurs while replacing
            }
        }
        else { // no command is passed as argument
            printf("No command to execute\n");
        }
    }
    else { // parent process
        wait(NULL); // wait for the child to complete
        gettimeofday(&current_time, NULL); // get the current time
        printf("\n\nElapsed Time: %lf second(s)\n", current_time.tv_sec + 1e-6 * current_time.tv_usec - *ptr); // log the time
    }
    shm_unlink("start_time");
    return 0;
}