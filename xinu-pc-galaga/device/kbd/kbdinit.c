/* kbdinit.c  -  kbdinit */

#include <xinu.h>
#include <keyboard.h>

//Crear estructura circular
int inicio=0;
int fin=0;
int tamanio_buffer_kbd=10;
unsigned char kblayout [10];  // { ... } Fill your layout yourself
int cant_entradas=0;
sid32 sem_kbd;

void keyboard_wait(byte a_type) //unsigned char
{
  int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}




/*------------------------------------------------------------------------
 *  kbdinit  -  Initialize the ps/2 keyboard
 *------------------------------------------------------------------------
 */
devcall	kbdinit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{

	int i;
	for (i=0; i<128; i++)
		kblayout[i] = 0;
	//keyboard_restart();

    sem_kbd=semcreate(0);
	byte _status;  //unsigned char

	//Enable the keyboard device   old: auxiliary mouse device
	keyboard_wait(1);
	outportb(0x64, 0xAE);
 
	//Enable the interrupts
	keyboard_wait(1);
	outportb(0x64, 0x20);
	keyboard_wait(0);
	_status=(inportb(0x60) | 1);
	keyboard_wait(1);
	outportb(0x64, 0x60);
	keyboard_wait(1);
	outportb(0x60, _status);
 
	//Tell the keyboard to use default settings
	/* while((inportb(0x64)&2)!=0){};*/
	// outportb(0x64, 0xF6);
	//keyboard_read();  //Acknowledge
 
	//Enable the keyboard
	while ((inportb(0x64) & 2) != 0) {
		inportb(0x60);
	}; 


	set_evec(1 + IRQBASE, (uint32)kbdhandlerirq);
	//outportb(0x64, 0xF4);
	//keyboard_read();  //Acknowledge

	return OK;
}
