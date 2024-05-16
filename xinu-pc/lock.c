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
