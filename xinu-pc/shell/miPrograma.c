 #include <xinu.h>


void	sndA(void), sndB(void);

/*------------------------------------------------------------------------
 * main  --  example of creating processes in Xinu
 *------------------------------------------------------------------------
 */
void	miPrograma(void)
{
    int p1=create(sndA, 1024, 20, "process 1", 0);
    int p2=create(sndB, 1024, 20, "process 2", 0);
	resume(p1);
	resume(p2);
    
    sleep(2);
    
    kill(p1);
    kill(p2);
}

/*------------------------------------------------------------------------
 * sndA  --  repeatedly emit 'A' on the console without terminating
 *------------------------------------------------------------------------
 */
void	sndA(void)
{
	while( 1 )
		putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 * sndB  --  repeatedly emit 'B' on the console without terminating
 *------------------------------------------------------------------------
 */
void	sndB(void)
{
	while( 1 )
		putc(CONSOLE, 'B');
}
