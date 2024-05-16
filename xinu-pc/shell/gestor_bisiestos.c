/* bisiestos.c en XINU */

#include <xinu.h>

#define ASCII_ESC 27
long int calculo[2];
int suspender[2];
/* completar el codigo del programa bisiesto */
bisiesto(int base, int limite,int n,sid32 modArreglo,sid32 mostrArreglo)
{
	int i;    
	/* base, limite y n vienen como argumentos */
	for (i=base; i<=limite; i++) {
	    if(suspender[n]==1){
	        suspend(getpid());
	    }	   
	    wait(modArreglo);	    
		  /* anio i es bisiesto */ 
        if(i%4 == 0 && i%100>0){
			calculo[n] = i;	
			sleepms(300);
    	}
    	signal(mostrArreglo);
   	}
}

muestra_bisiestos(sid32 modArreglo,sid32 mostrArreglo)
{
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC );

	while(1) {
	    wait(mostrArreglo);
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC );
    		printf( "proceso a: %d \n", calculo[0]);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[7;10f", ASCII_ESC );
    		printf( "proceso b: %d \n", calculo[1]);

		sleepms(50);
		signal(modArreglo);
	}
}


/* completar el gestor de procesos bisiestos */
gestor_bisiestos()
{
    int tecla;
    int salir=0;
    sid32 modificarArreglo=semcreate(1);
    sid32 mostrarArreglo=semcreate(0);
    int pid1=create(bisiesto,8000,20,"Proceso 1",6,0,2000,0,modificarArreglo,mostrarArreglo);
    int pid2=create(bisiesto,8000,20,"Proceso 2",6,2001,4000,1,modificarArreglo,mostrarArreglo);
    int pidMostrar=create(muestra_bisiestos,8000,20,"Mostrar",2,modificarArreglo,mostrarArreglo);
    resume(pid1);
    resume(pid2);
    resume(pidMostrar);
	while(1) {
		/* gestionar las pulsaciones del teclado, procesar, informar al usuario */
		if(salir){
		    break;
		}
        tecla=getchar();
        /*Como la terminal escribe sobre una seccion diferente de la pantalla a muestra_bisiestos (y es el único hilo sobre esa sección) entonces no es necesario usar un lock
        para resguardar esta región. muestra_bisiestos solo escribe cuando se modifica el arreglo y es el único hilo que escribe sobre su región*/
        switch(tecla){
            case '1':

                if(suspender[0]){
                    //reanudar proceso 1
                    suspender[0]=0;
                    resume(pid1);
                    printf( "%c[9;10f", ASCII_ESC );
                    printf( "primer proceso reanudado\n");
                }else{
                    //Suspender proceso 1
                    suspender[0]=1;
                    printf( "%c[9;10f", ASCII_ESC );
                    printf( "primer proceso suspendido\n");
                }
                break;
            case '3':
               //finalizar proceso 1
                kill(pid1);               
                printf( "%c[9;10f", ASCII_ESC );
                printf( "primer proceso finalizado\n");
               break;
            case '7':
                 if(suspender[1]==1){
                    //reanudar proceso 2
                    suspender[1]=0;
                    resume(pid2);
                    printf( "%c[9;10f", ASCII_ESC );
                    printf( "segundo proceso reanudado\n");
                }else{
                    //Suspender proceso 2
                    suspender[1]=1;
                    suspend(pid2);
                    printf( "%c[9;10f", ASCII_ESC );
                    printf( "segundo proceso suspendido\n");
                }
                break;
            case '9':
                //finalizar proceso 2
                kill(pid2);
                printf( "%c[9;10f", ASCII_ESC );
                printf( "segundo proceso finalizado\n");
                break;
            case 'q':
                //Finalizar y volver al shell
                kill(pid1);
                kill(pid2);
                kill(pidMostrar);     
                salir=1;           
                break;
        }
		sleepms(5);
	}

}

