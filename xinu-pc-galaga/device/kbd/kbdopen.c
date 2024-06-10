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
    devcall exito=0;
    if(kb_en_uso){
        //Si ya está en uso, retorna error
        exito=-1;
    }else{
        kb_en_uso=1;
    }
    
    return exito;
}
