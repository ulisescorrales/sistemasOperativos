#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */

/* productor */
int main()
{
    //Conseguir el texto
    FILE *fp = fopen("06mmap.txt", "r");
    char ch;
    char *txt="";
    fseek(fp,0,SEEK_END);
    long size=ftell(fp);    
    char texto[size];
    long i=0;
    fseek(fp,0,0);
    //while ((ch = fgetc(fp)) != EOF){
    while (i<size){
        //putchar(ch);
        ch = fgetc(fp);
        texto[i]=ch;
        i++;
    }       
    fclose(fp);
    //

    const int SIZE = size;
    printf("Size:%d \n",size);
    //const char *name = "OS";
    //const char *mess0= "Studying "; 
    //const char *mess1= "Operating Systems ";
    int shm_fd;
    void *ptr;
    /* create the shared memory segment */
    shm_fd = shm_open("06mmap.txt", O_CREAT | O_RDWR, 0666);
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
    sprintf(ptr,"%s",texto);
    //ptr += strlen(mess0);
    //sprintf(ptr,"%s",mess1);
    
    return 0;
}  

