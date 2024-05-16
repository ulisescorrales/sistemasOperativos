#include<xinu.h>

void print(sid32 mutex){
    int a=0;
    while(1){
        lock(mutex);
        printf("\r%d",a);        
        a++;
        unlock(mutex);
        sleepms(100);
    }
}

int killSleep (){
    
    sid32 m=mutex_init();
    //control(CONSOLE, TC_MODER, 0, 0);
    printf("Priority MAIN: %d\n",getprio(getpid()));
    int opcion=0;
    pid32 pida=create(print,8000,1,"Prueba",1,m);    
    
    resume(pida);
    
    /*while(1){
        opcion=getchar();
        switch(opcion){
            case '1':
                printf("Presionaste 1\n");
                break;
            case 'q':
                kill(pida);
                control(CONSOLE, TC_MODEC, 0, 0);
                exit();
                break;
        }
    }*/
}
