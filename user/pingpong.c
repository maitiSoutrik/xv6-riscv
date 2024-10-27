#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ITERATIONS 10000

int main() {
    int p1[2], p2[2];
    char byte = 'P';
    int pid;
    int start_time, end_time;

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "Pipe creation failed\n");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        fprintf(2, "Fork failed\n");
        exit(1);
    }

    start_time = uptime();

    if (pid == 0) {  // Child process
        close(p1[1]);
        close(p2[0]);
        
        for (int i = 0; i < ITERATIONS; i++) {
            if (read(p1[0], &byte, 1) != 1) {
                fprintf(2, "Child read failed\n");
                exit(1);
            }
            if (write(p2[1], &byte, 1) != 1) {
                fprintf(2, "Child write failed\n");
                exit(1);
            }
        }
        exit(0);
    } else {  // Parent process
        close(p1[0]);
        close(p2[1]);
        
        for (int i = 0; i < ITERATIONS; i++) {
            if (write(p1[1], &byte, 1) != 1) {
                fprintf(2, "Parent write failed\n");
                exit(1);
            }
            if (read(p2[0], &byte, 1) != 1) {
                fprintf(2, "Parent read failed\n");
                exit(1);
            }
        }
        
        end_time = uptime();
        
        wait(0);  // Wait for child to finish
        
        int total_time = end_time - start_time;
        printf("Time taken: %d ticks\n", total_time);
        printf("Exchanges per second: %d\n", ITERATIONS * 100 / total_time);
        
        exit(0);
    }
}
