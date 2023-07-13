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
#define section_size 256
#define section_num chunk_size/section_size
#define pmw_pipe "./pipe_pmw"
#define uaw_pipe "./pipe_uaw"

//global variables
int pid_ua;
int fd_pmw, fd_uaw;
int addr_ua;

//signal handlers
void handle_palloc(){

    size_t sz;

    if(read(fd_uaw, &sz, sizeof(size_t)) < 0){
	perror("fail to read memory size from uaw pipe");
	exit(0);
    }

    if(read(fd_uaw, &addr_ua, sizeof(int)) < 0){
	perror("fail to read memory addr from uaw pipe");
	exit(0);
    }
}

void read_pid(){

    if(read(fd_uaw, &pid_ua, sizeof(int)) < 0){
	perror("fail to read pid from uaw pipe");
	exit(0);
    }
    printf("signal accepted!! pid of ua:%d\n", pid_ua);
    signal(SIGUSR1, handle_palloc);
}


int main(){
    
    void * chunk;
    int pid = getpid();
    int sh_id;

    signal(SIGUSR1, read_pid);
    //allocate and attach shared memory
    //need to move the location of memory to NUMA node
    if((sh_id = shmget(sh_key, chunk_size, IPC_CREAT | 0644)) < 0){
	perror("fail to allocate shared memory");
	return 1;
    }
    if((chunk = shmat(sh_id, NULL, 0)) == (void *) -1){
	perror("fail to attach shared memory to address space");
	return 1;
    }

    //delete existing pipe
    if(access(pmw_pipe,F_OK) == 0) unlink(pmw_pipe);
    if(access(uaw_pipe,F_OK) == 0) unlink(uaw_pipe);
    
    //make pipes
    if((mkfifo(pmw_pipe,0666)) < 0) {
	perror("fail to make pmw pipe");
	return 0;
    }
    if((mkfifo(uaw_pipe,0666)) < 0) {
	perror("fail to make uaw pipe");
	return 0;
    }

    //open pipe files
    if((fd_pmw = open(pmw_pipe, O_WRONLY)) < 0) {
	perror("fail to open pmw pipe");
	return 0;
    }
    if((fd_uaw = open(uaw_pipe, O_RDONLY)) < 0) {
	perror("fail to open uaw pipe");
	return 0;
    }

    //give user application a pid
    if(write(fd_pmw, &pid, sizeof(int)) < 0){
	perror("fail to write to pipe");
	return 0;
    }
    

    while(1){

    }

    close(fd_pmw);
    close(fd_uaw);
}
