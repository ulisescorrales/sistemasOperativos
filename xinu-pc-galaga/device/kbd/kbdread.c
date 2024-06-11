/* kbdread.c  -  kbdread */

#include <xinu.h>
#include <mouse.h>

/*------------------------------------------------------------------------
 * kbdread  -  Read the status of the keyboard driver
 *------------------------------------------------------------------------
 */
devcall	kbdread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char          *buffer,        /* Address of buffer            */
          uint32        count           /* Length of buffer             */
	)
{
    //Consigue una letra y lo almacena en el buffer
    devcall exito=0;
    extern short int kb_en_uso;
    if(kb_en_uso){
        (*buffer)=kbdgetc();
        //printf("ingresaste una letra: %x \n",(*buffer));
    }else{
        exito=-1;
    }
	return exito;
}
