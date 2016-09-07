//  Checker.c
//  Created by Dominick Forlenza.

//  Checker is responsible for checking whether divisor and dividend supplied to it by
//  the Coordinator are divisible. The checker will print whether the numbers are divisible
//  and will return 1 if they are divisible and 0 if they are not.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int main(int argc, char** argv) {
    
    int pid = getpid(); // PID
    int *data; // Attached memory
    int id = 0; // Read in SHMID
    
    printf("Checker process[%d]: starting.\n", pid); // Start
    
    // Get arguments from command prompt that came from exec call
    int divisor = atoi(argv[1]);
    int dividend = atoi(argv[2]);
    int fdRead = atoi(argv[3]);
    
    // Read id from pipe
    read(fdRead, &id, sizeof(id));
    
    // Attach shared memory segment
    data = (int*)shmat(id, NULL, 0);
    
    printf("Checker process[%d]: read %lu bytes containing shm ID %d\n", pid, sizeof(id), id); // Read
    
    // IS DIVISIBLE
    if ((dividend % divisor) == 0) {
        
        // Print to console
        printf("Checker process[%d]: %d *IS* divisible by %d.\n", pid, dividend, divisor); // Is divisible
        printf("Checker process[%d]: wrote result (1) to shared memory.\n", pid); // Wrote result
        
        // Shared segment writes 1
        *data = 1;
        return 1;
    }
    
    // NOT DIVISIBLE
    else {
        
        // Print to console
        printf("Checker process[%d]: %d *IS NOT* divisible by %d.\n", pid, dividend, divisor); // IS NOT divisible
        printf("Checker process[%d]: wrote result (0) to shared memory.\n", pid); // Wrote result
        
        // Shared segment writes 0
        *data = 0;
        return 0;
    }
    
    // Clear out attached memory data
    shmdt(data);
    shmctl(id, IPC_RMID, 0);
}
