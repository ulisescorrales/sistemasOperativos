
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BMP_FILE "input2.bmp"
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

	//Abrir archivo original y en gris(en construcción)
	//el offset está después de los metadatos en ambos casos
	int or=abrirArchivoOr();
    int tmpGray=abrirArchivoTmp();


    //Estando en los metadatos, posicionarse en la sección del arreglo
    int inicio_imagen_modificar=3*width*comienzoFila+(comienzoFila*(padding));
	lseek(or,inicio_imagen_modificar,SEEK_CUR);
	int a=0;
    for(int i=0;i<finFila-comienzoFila;i++){
        for(int j=0;j<width;j++){
            read(or,pixel, 3);
	        gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
	        nueva_imagen[i*(width*3+padding) + j*3]= gray;
	        nueva_imagen[i*(width*3+padding) + j*3 + 1] = gray;
	        nueva_imagen[i*(width*3+padding) + j*3 + 2] = gray;
        }
   		lseek(or, padding, SEEK_CUR); // Skipping padding
    }
    //54 bytes ocupan los metadatos
    int metadatos=sizeof(BMPHeader)+sizeof(BMPInfoHeader);
    lseek(tmpGray,inicio_imagen_modificar,SEEK_CUR);

    write(tmpGray, &nueva_imagen[0], width * (finFila-comienzoFila) * 3+(finFila-comienzoFila)*padding);

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
    //Copiar el archivo en el archivo final

	int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT|O_TRUNC , 0644);
	if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}

	unsigned char pixel[3];
	int height=infoh.height;
	int width=infoh.width;
	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	//unsigned char nueva_imagen[2100*2000];
	for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            read(in_fd, pixel, 3);
	        nueva_imagen[i*(width*3+padding) + j*3]= pixel[0];
	        nueva_imagen[i*(width*3+padding) + j*3 + 1] = pixel[1];
	        nueva_imagen[i*(width*3+padding) + j*3 + 2] = pixel[2];	        
        }
        lseek(in_fd, padding, SEEK_CUR); // Skipping padding
    }
    //3393092
    //printf("%d\n",(height-1)*(width*3+padding) + (width-1)*3+2);
    //Escribe los metadatos
	write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
    //Escribe la imagen
    write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);
    close(out_fd);
	close(in_fd);

	//Dividir conversión a gris en 3 procesos (1063 filas en total)
	int proceso_1=fork();
	if(proceso_1==0){
    	convertir(h, infoh,0,354);
	}else{
	    int proceso_2=fork();
	    if(proceso_2==0){
	    	convertir(h, infoh,354,708);
	    }else{
	        int proceso_3=fork();
	        if(proceso_3==0){
    	        convertir(h, infoh,708,1064);
	        }else{
	            wait(proceso_1);
   	            wait(proceso_2);
   	            wait(proceso_3);
                printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
	        }
	    }
	}
}

