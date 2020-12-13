#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    double* ptr; // Shared memory pointer
    const int SIZE = sizeof(double);
    int FD = shm_open("start_time", O_CREAT | O_RDWR, 0666);
    ftruncate(FD, SIZE);
    ptr = (double*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FD, 0);
    pid_t pid;
    pid = fork();

    if (pid < 0) { // fork failure
        printf("Fork failed\n");
    }
    else if (pid == 0) { // child process
        // write cur time into shared memory space
        struct timeval current;
        gettimeofday(&current, NULL);
        *ptr = current.tv_sec + 1e-6 * current.tv_usec;
        // running the command
        if (argc > 1) {
            const char* command = argv[1];
            char** argv_list = (char**) malloc(argc * sizeof(char*));
            for (int i = 0; i < argc - 1; i++) {
                argv_list[i] = argv[i+1];
            }
            argv_list[argc - 1] = NULL;
            execvp(command, argv_list);
            // sleep(1);
        }
    }
    else { // parent process
        wait(NULL);
        double start = *ptr;
        struct timeval current;
        gettimeofday(&current, NULL);
        printf("Execution Complete\nTime taken: %lf second(s)\n", current.tv_sec + 1e-6 * current.tv_usec - start);
    }
    shm_unlink("start_time");
    return 0;
}
