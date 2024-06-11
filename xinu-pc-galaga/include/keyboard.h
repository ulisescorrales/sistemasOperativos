
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)

//extern int tamanio_buffer_kbd;
//extern unsigned char kblayout [];  // { ... } Fill your layout yourself 

