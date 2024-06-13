#include <stdio.h>
int main(){
    void *ptr;
    char imagen[]={'H','o','l','a','\0'};

    for(int i=0;i<4;i++){
        ptr=imagen[i];
    }
    printf("%s\n",ptr);
    //printf("%s\n",ptr);
    return 0;
}
