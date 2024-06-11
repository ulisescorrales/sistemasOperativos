/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* kbdhandler.c - kbdhandler */

#include <xinu.h>
#include <keyboard.h>


unsigned char get_scancode()
{
    unsigned char inputdata;
    inputdata = inportb(KEYBOARD_DATAREG);
    return inputdata;
}


/*------------------------------------------------------------------------
 *  kbdhandler  -  Handle an interrupt for the keyboard device
 *------------------------------------------------------------------------
 */
void kbdhandler(void)
{
	char t[80];
	unsigned char scancode; 
	unsigned int shift_key = 0;
	int i = 10;

    extern unsigned char kblayout[];
    extern sid32 sem_kbd;
    extern int fin;
    extern int cant_entradas;
    extern int tamanio_buffer_kbd;

	scancode = get_scancode();

    //imprimir en vga scancode
	//sprintf(t, "kbd: 0x%x     ", scancode);
	//print_text_on_vga(10, 300, t);

	if(scancode == 0x2A) {
		shift_key = 1;//Shift key is pressed
	} else if(scancode & 0xAA) {
		shift_key = 0;//Shift Key is not pressed
	} else {          
		if (scancode & 0x80) {
			int shiftaltctrl = 1;//Put anything to see what special keys were pressed
		} else {  
		}     
	}

	//Si el buffer no está lleno
	if(cant_entradas<tamanio_buffer_kbd){	
		kblayout[fin]=scancode;
		//printf("kablayout[%d]: %x\n",fin,kblayout[fin]);
		cant_entradas++;
		fin++;
		if(fin==tamanio_buffer_kbd){
			fin=0;
		}
		signal(sem_kbd);
	}
}

