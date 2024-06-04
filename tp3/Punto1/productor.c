#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <string.h>
/* productor */
int main()
{
 const int SIZE = 4096;
 const char *name = "OS";
 const char *mess0= "Studying "; 
 const char *mess1= "Operating Systems ";
 int shm_fd;
 void *ptr;
 /* create the shared memory segment */
 shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
 /* configure the size of the shared memory segment */
 ftruncate(shm_fd,SIZE);
 /* now map the shared memory segment in the 
 * address space of the process */
 ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
 if (ptr == MAP_FAILED) {
 printf("Map failed\n");
 return -1;
 }
 /* Now write to the shared memory region. */
 sprintf(ptr,"%s",mess0);
 ptr += strlen(mess0);
 sprintf(ptr,"%s",mess1);
 
 return 0;
}  
