#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int a=open("ejemplo.txt",O_CREAT|O_RDWR,0644);

    write(a,"HOLaKJLaaA\n",5);
    close(a);
}
