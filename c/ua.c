#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define sh_key 1234
#define chunk_size 1024
#define pmw_pipe "./pipe_pmw" 
#define uaw_pipe "./pipe_uaw"

//global variables
int pid_pm;
int fd_pmw, fd_uaw;

//interface functions
void * palloc(size_t size){

    size_t sz = size;

    if(write(fd_uaw, &sz, sizeof(size_t)) < 0){
	perror("fail to give the size of required memory to shared memory");
	exit(0);
    }
    //send signal to let sm know the size is written in pipe
    kill(pid_pm, SIGUSR1);
    //wait until sm give back the address
    kill(getpid(), SIGSTOP);
}

void NDP(){

}

int main(){

    int pid = getpid();
    int sh_id;
    void * chunk;

    //allocate and attach shared memory
    if(sh_id = shmget(sh_key, chunk_size, IPC_CREAT | 0644) < 0){
	perror("fail to allocate shared memory");
	return 1;
    }
    if((chunk = shmat(sh_id, NULL, 0)) == (void *) -1){
	perror("fail to attach shared memory to address space");
	return 1;
    }

    //open pipe files
    if((fd_pmw = open(pmw_pipe, O_RDONLY)) < 0) {
	perror("fail to open pmw pipe");
	return 0;
    }                
    if((fd_uaw = open(uaw_pipe, O_WRONLY)) < 0) {
	perror("fail to open uaw pipe");
	return 0;       
    } 

    if(read(fd_pmw, &pid_pm, sizeof(int)) < 0){
	perror("fail to read pm pid");
	return 1;
    }

    if(write(fd_uaw, &pid, sizeof(int)) < 0){
	perror("fail to write pid");
	return 1;
    }

    //let pm know that pid of ua is written in pipe
    kill(pid_pm, SIGUSR1);
	    
    while(1){

    }


}
