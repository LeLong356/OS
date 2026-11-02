#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


#define FILE_PATH "problem4.txt"   
#define FILE_SIZE 1000          

int main(int argc, char *argv[]) {
    int fd = -1;
    char *map = NULL;

   fd = open(FILE_PATH, O_RDWR | O_CREAT, 0644);
   if ( fd < 0 ) {
    perror("open");
    return 1;
   }
   
   ftruncate(fd, FILE_SIZE);

   map = mmap(NULL,FILE_SIZE, PROT_WRITE , MAP_SHARED, fd,0);
   if(map == MAP_FAILED){
    perror("mmap");
    return 1;
   }

    sprintf(map, "Hello world\n");

    sleep(10);  

    munmap(map,FILE_SIZE);
    close(fd);

    return 0;
}
