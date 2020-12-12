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
    char* ptr; // Shared memory pointer
    const int SIZE = 4096;
    int FD = shm_open("start_time", O_CREAT | O_RDWR, 0666), i;
    ptr = (char*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FD, 0);
    pid_t pid;
    pid = fork();

    if (pid < 0) { // fork failure
        printf("Fork failed\n");
    }
    else if (pid == 0) { // child process
        // write cur time into shared memory space
        struct timeval current;
        gettimeofday(&current, NULL);
        sprintf(ptr,"%lf", current.tv_sec + 1e-6 * current.tv_usec);
        char* argv_list[] = { "ls" };
        // execv("ls", argv_list);
        sleep(1);
    }
    else { // parent process
        wait(NULL);
        // char* null;
        // double start = strtod(ptr, &null);
        // printf("%s", ptr);
        struct timeval current;
        gettimeofday(&current, NULL);
        double start = 0.0;
        printf("Execution Complete\nTime taken: %lf second(s)\n", current.tv_sec + 1e-6 * current.tv_usec - start);
    }
    shm_unlink("start_time");
    return 0;
}