#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_ITERATIONS 1000  // Reduced iterations for xv6

int main(void) {
    int p1[2], p2[2];
    int pid;
    char buf[1];
    int start_time, end_time;

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) {  // Child
        close(p1[1]);
        close(p2[0]);
        
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            read(p1[0], buf, 1);
            write(p2[1], "c", 1);
        }
        exit(0);
    } else {  // Parent
        close(p1[0]);
        close(p2[1]);
        
        start_time = uptime();
        
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            write(p1[1], "p", 1);
            read(p2[0], buf, 1);
        }
        
        end_time = uptime();
        
        printf("Total ticks: %d\n", end_time - start_time);
        printf("Context switches: %d\n", NUM_ITERATIONS * 2);
        
        wait(0);
        exit(0);
    }
}