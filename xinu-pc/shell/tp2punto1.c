#include <xinu.h>
int high_level_scheduler(
	int pid_a, int ms_a,
	int pid_b, int ms_b,
	int pid_c, int ms_c) {
	
	int prio_scheduler=getprio(getpid());
	int prio_a=getprio(pid_a);//Prioridad original del proceso a
	int prio_b=getprio(pid_b);//Prioridad original del proceso b
	int prio_c=getprio(pid_c);//Prioridad original del proceso c
	/*Obtener el PID del planificador (proceso actual) */
	int cont=0;
	
	int aux;	
	while(1){
	    printf("Main iteración: %d\n",cont);
		//Setear la prioridad del proceso a a prio_scheduler-1	    
		chprio(pid_a,prio_scheduler-1);
        printf("Prioridad para A\n");
		//Dormir mientras se ejecuta el proceso
		sleepms(ms_a);
		//Devolver al proceso a su prioridad original
		chprio(pid_a,prio_a);	    
		//Setear la prioridad del proceso  b prio_scheduler-1
		chprio(pid_b,prio_scheduler-1);
        printf("Prioridad para B\n");
		//Dormir mientras se ejecuta el proceso
		sleepms(ms_b);
		//Devolver al proceso a su prioridad original
		chprio(pid_b,prio_b);	    
		//Setear la prioridad del proceso c prio_scheduler-1
		chprio(pid_c,prio_scheduler-1);
        printf("Prioridad para C\n");
		//Dormir mientras se ejecuta el proceso
		sleepms(ms_c);
		//Devolver al proceso a su prioridad original
		chprio(pid_c,prio_c);
		
		cont++;
	}
}

void proceso_a(){
	int var_a=0;
	while(1){
		printf("Proceso A: %d\n",var_a);
        //putc(CONSOLE, 'A');
		var_a++;
	}
}
void proceso_b(){
	int var_b=0;
	while(1){
		printf("Proceso B: %d\n",var_b);
		//putc(CONSOLE, 'B');
		var_b++;
	}
}
void proceso_c(){
	int var_c=0;
	while(1){
//    	putc(CONSOLE, 'C');
		printf("Proceso C: %d\n",var_c);
		var_c++;
	}
}
int tp2punto1(){
    //Tiempos
	int ms_a=120;
	int ms_b=60;
	int ms_c=20;
	//Crear los procesos
	int pid_a=create(proceso_a,8000,1,"Proceso A",0);
	int pid_b=create(proceso_b,8000,1,"Proceso B",0);
	int pid_c=create(proceso_c,8000,1,"Proceso C",0);
	int pid_main=create(high_level_scheduler,8000,99,"Planificador",6,pid_a,ms_a,pid_b,ms_b,pid_c,ms_c);

	resume(pid_main);
	resume(pid_a);
	resume(pid_b);
	resume(pid_c);
	return 0;
}
