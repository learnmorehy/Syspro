#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>

#define SHM_KEY 0x1234

int* get_sh(size_t size){

   int shmid;
   int * sh; 

   shmid = shmget(SHM_KEY, size, 0644|IPC_CREAT);

    if (shmid == -1) {
	perror("Failed to get shared memory");
	exit(0);
    }   

    sh = shmat(shmid, NULL, 0);

    if (sh == (int *)-1) {
	perror("Failed to attach shared memory");
	exit(0);
    }

    return sh;
}

