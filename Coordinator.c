// Coordinator.h
// Created by Dominick Forlenza.

// The Coordinator program is responsible for launching 4 processes that it will
// then load using the Checker program. This program will fork() to launch
// processes, execlp() to replace the program driving the process and supply the
// arguments, and wait() to wait for process completion.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    
    int pid = 0; // Process ID
    int i = 0; // Loop variable
    int status; // Wait status
    char buf[80]; // Buffer
    int id[4]; // SHMID
    int fd[4][2]; // Piped File Descriptors
    int pids[4]; // PID array
    int n_bytes; // Write size
    int *data[4]; // Attached memory
    
    
    // Fork four processes
    for (i = 0; i < 4; i++) {
        
        // Create pipe
        pipe(fd[i]);
        
        // Create shared memory segment
        id[i] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0774);
        
        //Creates fork
        pid = fork();
        
        // Add pid to pids array
        pids[i] = pid;
        
        // Child process found - time to execlp()
        if (pid == 0) {
            
            // Close read end of pipe
            close(fd[i][1]);
            
            // Write to buffer
            sprintf(buf, "%d", fd[i][0]);
            
            // Exec to checker function
            execlp("./checker", "checker", argv[1], argv[i + 2], buf,  NULL);
        }
        
        // Parent processing
        if (pid > 0) {
            
            // Close write end of pipe
            close(fd[i][0]);
            
            // Attach shared memory segment
            data[i] = (int*)shmat(id[i], NULL, 0);
            
            printf("Coordinator: forked process with ID %d.\n", pids[i]); // Fork
            printf("Coordinator: wrote shm ID %d to pipe (%lu bytes)\n", id[i], sizeof(id[i])); // Write
            
            // Write shmid to pipe
            n_bytes = write(fd[i][1], &id[i], sizeof(id[i]));
            
        }
    }
    
    // Wait for four processes
    for (i = 0; i < 4; i++) {
        // Call wait
        waitpid(pids[i], &status, 0);
    }
    
    // Read return value and print results
    for (i = 0; i < 4; i++) {
        
        printf("Coordinator: waiting on child process %d...\n", pids[i]); // Wait
        
        // Is not divisible
        if (*(data[i]) != 1) {
            printf("Coordinator: result %d read from shared memory: %s is not divisible by %s\n", *(data[i]) , argv[i + 2] , argv[1]); // Result 1
        }
        // Is divisible
        else {
            printf("Coordinator: result %d read from shared memory: %s is divisible by %s\n", *(data[i]) , argv[i + 2] , argv[1]); // Result 2
        }
        
        // Detach and delete shared memory segment
        shmdt(data[i]);
        shmctl(id[i], IPC_RMID, 0);
    }
    
    // End the coordinator
    printf("Coordinator: exiting.\n");
    return 0;
    
}