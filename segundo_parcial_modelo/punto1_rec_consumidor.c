#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/* consumidor */
int main(){
	 const char *name = "cat";
	 const int SIZE = 50261;
	 int shm_fd;
	 void *ptr;
	 int i;
	 /* open the shared memory segment */
	 shm_fd = shm_open(name, O_RDONLY, 0666);
	 if (shm_fd == -1) {
		 printf("shared memory failed\n");
		 exit(-1);
	 }
	 /* now map the shared memory segment in the address space of the process */
	 ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	 if (ptr == MAP_FAILED) {
		 printf("Map failed\n");
		 exit(-1);
	 }
	 //-----------
	 int temp=50260;
	 int width=259;
	 int height=194;
	 unsigned char *im=(unsigned char *)ptr;
	 unsigned char im2[SIZE];
	 //copiar la cabecera
	 for(int i=0;i<15;i++){
		 im2[i]=im[i];
	}
	printf("copiado la cabecera\n");
	 for(int i=0;i<height;i++){
		 for(int j=0;j<width;j++){
			im2[15+i*width+j]= im[temp];
			temp--;
		}
	 }
     //memcpy(imagen,ptr,SIZE);
	 //printf("%x\n",(char [])ptr);
	 int out_fd = open("cat_output.pgm", O_WRONLY | O_CREAT|O_TRUNC , 0644);
    if (out_fd < 0) {
        printf("Error en output\n");
    }
    int w=write(out_fd,&im2[0],SIZE);
    if(w==-1){
        printf("Error escribiendo\n");
    }
	 //
	 /* now read from the shared memory region */
	 //printf("%s",(char *)ptr);
	 /* remove the shared memory segment */
	/* if (shm_unlink(name) == -1) {
		 printf("Error removing %s\n",name);
		 exit(-1);
	 }*/
	 return 0;
}

