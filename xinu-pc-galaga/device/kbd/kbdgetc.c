
#include <xinu.h>
unsigned char kbdgetc(){
    extern int inicio;
    extern int kblayout[];
    unsigned char letra;
    extern int cant_entradas;
    extern sid32 sem_kbd;

    wait(sem_kbd);
    letra=kblayout[inicio];
    kblayout[inicio]=0;
    inicio++;
    if(inicio==128){
        inicio=0;
    }
    cant_entradas--;

    return letra;
}
