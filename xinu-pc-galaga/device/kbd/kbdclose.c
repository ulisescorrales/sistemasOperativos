/* kbdclose.c  -  kbdclose */

#include <xinu.h>

/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */
devcall	kbdclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
    extern kb_en_uso;
    devcall exito=0;
    extern int inicio;
    extern int fin;
    extern sid32 sem_kbd;
    extern int cant_entradas;

    if(kb_en_uso){
        kb_en_uso=0;
        semreset(sem_kbd,0);
        inicio=0;
        fin=0;
        cant_entradas=0;
    }else{
        exito=-1;
    }    
    return exito;
}
