
#include <xinu.h>
unsigned char kbdgetc(){
    extern int inicio;
    extern int kbdlayout[];
    unsigned char byte=kbdlayout[inicio];
    extern int cant_entradas;
    extern sid32 kbd_sem;

    wait(kbd_sem);
    kbdlayout[inicio]=0;
    inicio++;
    cant_entradas--;

    return byte;
}
