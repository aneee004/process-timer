#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    double* ptr; // shared memory pointer
    struct timeval current_time; // to store the current time
    pid_t pid; // to store the child process id
    int SIZE, FD, i;

    SIZE = sizeof(double); // size of the shared memory space
    FD = shm_open("start_time", O_CREAT | O_RDWR, 0666); // opening the shared memory
    ftruncate(FD, SIZE); // resizing the shared memory to the required size
    ptr = (double*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FD, 0); // mapping the shared memory locaiton to the pointer
    pid = fork();

    if (pid < 0) { // fork failure
        printf("Fork failure. Please try again.\n");
    }
    else if (pid == 0) { // child process
        // write cur time into shared memory space
        gettimeofday(&current_time, NULL);
        *ptr = current_time.tv_sec + 1e-6 * current_time.tv_usec;
        // running the command
        if (argc > 1) {
            for (int i = 1; i < argc; i++) {
                argv[i-1] = argv[i];
            }
            argv[argc - 1] = NULL;
            if (execvp(argv[0], argv) == -1) {
                printf("Error running your command.\n");
            }
        }
        else {
            printf("No command to execute\n");
        }
    }
    else { // parent process
        wait(NULL);
        gettimeofday(&current_time, NULL);
        printf("\n\nElapsed Time: %lf second(s)\n", current_time.tv_sec + 1e-6 * current_time.tv_usec - *ptr);
    }
    shm_unlink("start_time");
    return 0;
}