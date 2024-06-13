#include <xinu.h>
int teclado(){
    paint_screen();
    char a;
    int cant=20;
    unsigned char tecla;
    unsigned char scancode[80];
    int altura=0;
    int k=open(KEYBOARD,"","");
    if(k==-1){
        printf("Error abriendo KEYBOARD");
    }else{
        for(int i=0;i<20;i++){
            printf("\rIngrese 20 teclas, faltan: %d",cant);
            read(KEYBOARD,&tecla,1);
            sprintf(scancode,"Tecla presionada: 0x%x",tecla);
            print_text_on_vga(10, altura, scancode);
            altura+=10;
            cant--;
        }
        int c=close(KEYBOARD);
        if(c==-1){
            printf("Error en close\n");
        }
    }
    printf("\n");
    return 0;
}
