#include <xinu.h>
unsigned char x=0;

struct Lock{
    int pidOwner;
    sid32 sem;
};
struct Lock mutex_init(){
    struct Lock lock;    
    lock.sem=semcreate(1);
    lock.pidOwner=-1;
    return lock;
};
void mutex_lock(struct Lock *lock){
    wait(lock->sem);
    lock->pidOwner=getpid();
}
void mutex_unlock(struct Lock *lock){    
    if(getpid()==lock->pidOwner){
        signal(lock->sem);
    }else{
        return 1;
    }  
}
void operar(struct Lock *lock), incrementar(struct Lock *lock);
void mut(){
	int i;          
    struct Lock lock=mutex_init();
    int p1=create(operar,8000,20,"process 1",1,lock);            
    int p2=create(incrementar,8000,20,"process 2",1,lock);
    resume(p1);
    resume(p2);
    
    sleep(10);	
}
/* Operar x e y */

void operar(struct Lock *lock){
    int y=0;
    printf("Si no existen mensajes de ERROR entonces todo va OK!\n");
    while(1){
        //Si x es módulo de 10
        mutex_lock(lock);
        if ((x%10)==0){
            y=x*2;
            //como y es el doble de x entonces y es múltiplo de 10 también
            if((y%10)!=0){
                printf("\r ERROR!! y=%d, x=%d \r",y,x);
            }else{
                printf("\r OK: y=%d, x=%d \r",y,x);
            }
        }
        mutex_unlock(lock);
    }
}
//Incrementar X
void incrementar(struct Lock *lock){
    while(1){
        mutex_lock(lock);
        x=x+1;
        mutex_unlock(lock);
    }
}    
