#include <xinu.h>
void produce(sid32,sid32),consume(sid32,sid32);
int32 n=0;

void tp2Punto2(void){
    sid32 produced,consumed;
    consumed=semcreate(0);
    produced=semcreate(1);
    resume(create(consume,1024,20,"cons",2,consumed,produced));
    resume(create(produce,1024,20,"prod",2,consumed,produced));    
}
void produce(sid32 produced,sid32 consumed){
    int32 i;
    for(i=1;i<=2000;i++){
        wait(consumed);
        n++;
        signal(produced);
    }
}
void consume(sid32 produced,sid32 consumed){
    int32 i;
    for(i=1;i<=2000;i++){
        wait(produced);
        printf("The value of n id %d \n",n);
        signal(consumed);
    }
}
