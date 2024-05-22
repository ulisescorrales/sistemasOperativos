
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"

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
unsigned char nueva_imagen [2000*2000*3];
void convertir( BMPHeader h, BMPInfoHeader infoh,int comienzoFila,int finFila){
    int width = infoh.width;
	int height = infoh.height;
	unsigned char gray;
	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	unsigned char pixel[3];
	
	int or=abrirArchivoOr();
    int tmpGray=abrirArchivoTmp();
    for(int i=comienzoFila;i<finFila;i++){
        for(int j=0;j<width;j++){
            read(or,pixel, 3);
	        gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
	        nueva_imagen[i*(width*3+padding) + j*3]= gray;
	        nueva_imagen[i*(width*3+padding) + j*3 + 1] = gray;
	        nueva_imagen[i*(width*3+padding) + j*3 + 2] = gray;
        }
   		lseek(nueva_imagen, padding, SEEK_CUR); // Skipping padding
    }
    write(tmpGray, &h, sizeof(BMPHeader));
    write(tmpGray, &infoh, sizeof(BMPInfoHeader));
    write(tmpGray, &nueva_imagen[0], infoh.width * infoh.height * 3);
    close(tmpGray);
    close(or);
}
void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}
int abrirArchivoOr(){
    int in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}
	return in_fd;
}
int abrirArchivoTmp(){
    int in_fd = open(GRAYSCALE_FILE, O_RDWR | O_CREAT, 0644);
    
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
    //cargar píxeles en el arreglo    
    unsigned char pixel[3];
    int width=infoh.width;
    int height=infoh.height;
  	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            read(in_fd, pixel, 3);
	        nueva_imagen[i*(width*3+padding) + j*3]= pixel[0];
	        nueva_imagen[i*(width*3+padding) + j*3 + 1] = pixel[1];
	        nueva_imagen[i*(width*3+padding) + j*3 + 2] = pixel[2];
        }
    }
    	
	return in_fd;
}
int main()
{	
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

    //Guardar una primera imagen temporal

	int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT , 0644);
	write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
    write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);
    close(in_fd);
    close(out_fd);
    
	if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}
    //Separar convertir en 3 procesos
  	int a=fork();
  	if(a==0){
  	    convertir(h, infoh,0,354);
  	    exit(0);
  	}/*else{
 	    wait(a);
  	    int b=fork();
  	    if(b==0){
      	    convertir(h, infoh,354,708);
      	    exit(0);
  	    }else{
  	        wait(b);
  	        int c=fork();
  	        if(c==0){
  	            convertir(h, infoh,708,1063);
          	    exit(0);
  	        }else{
  	            wait(c);
                printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
  	        }
  	    }
  	}*/
}

