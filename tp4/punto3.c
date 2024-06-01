/* Ejemplo con threads en Linux. Compilar con: gcc -o p p.c -lpthread */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale_threads.bmp"

#pragma pack(push, 1)
typedef struct {
	unsigned char magic[2];
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset;
} BMPHeader;

typedef struct {
	unsigned int headerSize;
	int width;
	int height;
	unsigned short planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int imageSize;
	int xPixelsPerM;
	int yPixelsPerM;
	unsigned int colorsUsed;
	unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

BMPHeader h;
BMPInfoHeader infoh;
int total = 0;
int width;
int height;
int padding;
unsigned char nueva_imagen[2000*2000*3];
/*Solo se modifica el arreglo nueva_imagen globalmente mientras que cada thread abre el archivo original para tener su propio descriptor*/
void *thread( void * i){

	int *n = (int *) i;
	int comienzoFila=n[0];
	int finFila=n[1];
	//el offset está después de los metadatos
	int or=abrirArchivoOr();

	unsigned char gray;
	unsigned char pixel[3];

    //Estando en los metadatos, posicionarse en la sección del arreglo
    int inicio_imagen_modificar=3*width*comienzoFila+comienzoFila*padding;
	lseek(or,inicio_imagen_modificar,SEEK_CUR);
    for(int i=comienzoFila;i<=finFila;i++){
        for(int j=0;j<width;j++){
            read(or,pixel, 3);
	        gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
	        nueva_imagen[i*(width*3+padding) + j*3]= gray;
	        nueva_imagen[i*(width*3+padding) + j*3 + 1] = gray;
	        nueva_imagen[i*(width*3+padding) + j*3 + 2] = gray;
        }
   		lseek(or, padding, SEEK_CUR); // Skipping padding
    }
    close(or);
	printf("fin thread\n");
}
int abrirArchivoOr(){
    int or = open(BMP_FILE, O_RDONLY);
	int offset=sizeof(BMPHeader)+sizeof(BMPInfoHeader);
    lseek(or,offset,SEEK_CUR);
	return or;
}

int abrirArchivoTmp(){
    int tmp = open(GRAYSCALE_FILE, O_WRONLY, 0644);
    int offset=sizeof(BMPHeader)+sizeof(BMPInfoHeader);
    lseek(tmp,offset,SEEK_CUR);
	return tmp;
}
void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

int main()
{
	int limites0[]={0,354};
    int limites1[]={355,708};
    int limites2[]={709,1063};
    pthread_t tid[3];
    int in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}

	BMPHeader h;
	read(in_fd, &h, sizeof(BMPHeader));

	if (h.magic[0] != 'B' || h.magic[1] != 'M') {
		something_wrong(in_fd, "No es BMP");
	}

	BMPInfoHeader infoh;
	read(in_fd, &infoh, sizeof(BMPInfoHeader));

	if (infoh.bpp != 24 || infoh.compression != 0) {
		something_wrong(in_fd, "No 24-bit BMP");
	}
    //Copiar el archivo en el archivo final

	int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT|O_TRUNC , 0644);
	if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}

	width=infoh.width;
	height=infoh.height;
	padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
    /* Create independent threads each of which will execute function */


    pthread_create( &tid[0], NULL, thread, (void *) &limites0);
    pthread_create( &tid[1], NULL, thread, (void *) &limites1);
    pthread_create( &tid[2], NULL, thread, (void *) &limites2);

	//Cada thread escribe en una sección del arreglo y se espera que finalicen
    for (int i = 0; i <= 2; i++){
        pthread_join(tid[i], NULL);
    }

    //Escribe los metadatos
	write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
    //Escribe la imagen
    write(out_fd, &nueva_imagen[0], infoh.width * height*3);
	//Cierra los archivos
    close(out_fd);
	close(in_fd);
	printf("Fin MAIN\n");
    return 0;
}
