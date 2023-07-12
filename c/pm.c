#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define chunk_size 1024
#define section_size 256
#define pipe_name "./pipe"

int main(){
    
    int * ptr;
    int fd;

    if ((fd = open(PIPENAME, O_WRONLY)) < 0) {
	perror("fail to open named pipe");
	return 0;
    }

}
