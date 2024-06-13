
/* Ejemplo con threads en Linux. Compilar con: gcc -o p p.c -lpthread */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <string.h>

#define BMP_FILE "cat.pgm"
#define GRAYSCALE_FILE "output_cat.pgm"
#pragma pack(push, 1)
#pragma pack(pop)
/*Solo se modifica el arreglo nueva_imagen globalmente mientras que cada thread abre el archivo original para tener su propio descriptor*/

void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}
int crearMemomoriaCompartida(unsigned char imagen[],int TAMANIO){
    int shm_fd;
    void *ptr;
    shm_fd = shm_open("cat", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd,TAMANIO);
    ptr = mmap(0,TAMANIO, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }
    memcpy(ptr, imagen, TAMANIO);
    return 0;
}
int main()
{
    int width=259;
    int height=194;
    int tamanio_cabecera=15;
    int tamanio=width*height+tamanio_cabecera;
    unsigned char imagen[width*height+15];
    unsigned char byte;
    unsigned char cabecera[tamanio_cabecera];
    int in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}
    for(int i=0;i<tamanio_cabecera;i++){
        read(in_fd,&byte,1);
        imagen[i]=byte;
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            read(in_fd,&byte, 1);
	        imagen[15+i*width+j]= byte;
        }
    }

    int mem=crearMemomoriaCompartida(imagen,width*height+15);
    if(mem==-1){
        printf("Error creando memoria compartida\n");
    };
    int out_fd = open("cat_output.pgm", O_WRONLY | O_CREAT|O_TRUNC , 0644);
    if (out_fd < 0) {
        printf("Error en output\n");
    }
    int w=write(out_fd,&imagen[0],tamanio);
    if(w==-1){
        printf("Error escribiendo\n");
    }

	close(in_fd);
	printf("Fin MAIN\n");
    return 0;
}
