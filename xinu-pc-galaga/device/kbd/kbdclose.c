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
    if(kb_en_uso){
        kb_en_uso=0;
    }else{
        exito=-1;
    }    
    return exito;
}
