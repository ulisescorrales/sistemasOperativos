/* kbdopen.c  -  kbdopen */

#include <xinu.h>

/*------------------------------------------------------------------------
 * kbdopen  -  Open the ps/2 keyboard device
 *------------------------------------------------------------------------
 */
short int kb_en_uso=0;
devcall	kbdopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a kbd */
	 char	*mode			/* Unused for a kbd */
	)
{
    extern sid32 sem_kbd;
    extern int inicio;
    extern int fin;
    extern int cant_entradas;
    devcall exito=0;
    if(kb_en_uso){
        //Si ya está en uso, retorna error
        exito=-1;
    }else{
        kb_en_uso=1;
        //Limpiar todo lo que se haya acumulado
        semreset(sem_kbd,0);
        inicio=0;
        fin=0;
        cant_entradas=0;
    }
    
    return exito;
}
