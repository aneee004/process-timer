#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char* argv[]) {
    char* ptr; ///> Shared memory pointer
    const int SIZE = 4096;
    int FD = shm_open("start_time", O_CREAT | O_RDWR, 0666);
    ftruncate(FD, SIZE);
    ptr = (char*) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FD, 0);
    pid_t pid;
    pid = fork();

    if (pid < 0) { // fork failure
        printf("Fork failed\n");
    }
    else if (pid == 0) { // child process
        sleep(1000);
    }
    else { // parent process
        wait(NULL);
        printf("Child Complete");
    }
    shm_unlink("start_time");
    return 0;
}