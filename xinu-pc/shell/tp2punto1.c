#include <xinu.h>

int valores[3];

//mutex
int pid_mutex=-1;
sid32 mutex_init(){
    return semcreate(1);
} 
void lock(sid32 sem){
    wait(sem);
    pid_mutex=getpid();
}
void unlock(sid32 sem){
    if(getpid()==pid_mutex){
        signal(sem);
    }
}
//
int high_level_scheduler(
	int pid_a, int ms_a,
	int pid_b, int ms_b,
	int pid_c, int ms_c,
    sid32 mutex
                        ) {
	
	int prio_scheduler=getprio(getpid());
	int prio_a=getprio(pid_a);//Prioridad original del proceso a
	int prio_b=getprio(pid_b);//Prioridad original del proceso b
	int prio_c=getprio(pid_c);//Prioridad original del proceso c
	/*Obtener el PID del planificador (proceso actual) */	
	
	int aux;	        
	while(1){	            
        //PROCESO A
		//Setear la prioridad del proceso a a prio_scheduler-1	    
        //lock(mutex);
		chprio(pid_a,prio_scheduler-1);
        //unlock(mutex);
        //printf("\rScheduler: Prioridad para A\n");
		//Dormir mientras se ejecuta el proceso
		sleepms(ms_a);
		//Devolver al proceso a su prioridad original        
		chprio(pid_a,prio_a);	    
        
        //PROCESO B
		//Setear la prioridad del proceso  b prio_scheduler-1
		chprio(pid_b,prio_scheduler-1);
        //printf("\rScheduler: Prioridad para B\n");
		//Dormir mientras se ejecuta el proceso
		sleepms(ms_b);
		//Devolver al proceso a su prioridad original
		chprio(pid_b,prio_b);	    
        
        //PROCESO C
		//Setear la prioridad del proceso c prio_scheduler-1
		chprio(pid_c,prio_scheduler-1);
        //printf("\rScheduler: Prioridad para C\n");
		//Dormir mientras se ejecuta el proceso
		sleepms(ms_c);
		//Devolver al proceso a su prioridad original
		chprio(pid_c,prio_c);
				
	}
}
void proceso(sid32 mutex,int n){
    while(1){
        lock(mutex);        
        printf("\rProceso A: %d, Proceso B: %d, Proceso C: %d",valores[0],valores[1],valores[2]);
        valores[n]++;
        unlock(mutex);
        sleepms(1);
    }    
}

int tp2punto1(){
    control(CONSOLE, TC_MODER, 0, 0);
    sid32 mutex=mutex_init();
    int salir=0;
    int opcion=0;
    //Tiempos
	int ms_a=120;
	int ms_b=60;
	int ms_c=20;
	//Crear los procesos    
	pid32 pid_a=create(proceso,8000,1,"Proceso A",2,mutex,0);
	pid32 pid_b=create(proceso,8000,1,"Proceso B",2,mutex,1);
	pid32 pid_c=create(proceso,8000,1,"Proceso C",2,mutex,2);
	pid32 pid_main=create(high_level_scheduler,8000,19,"Planificador",7,pid_a,ms_a,pid_b,ms_b,pid_c,ms_c,mutex);
    
    printf("Prio MAIN: %d",getprio(getpid()));
    
	resume(pid_main);
	resume(pid_a);
	resume(pid_b);
	resume(pid_c);    
    
    while(1){
        printf("\r\nESPERANDO OPCIÓN\n");
        opcion=getchar();                
        if(opcion== 'q'){
            printf("\rSALIENDO\n");
            lock(mutex);            
            control(CONSOLE, TC_MODEC, 0, 0);            
            kill(pid_main);
            kill(pid_a);
            kill(pid_b);
            kill(pid_c);
            exit();
        }
    }    
	return 0;
}
