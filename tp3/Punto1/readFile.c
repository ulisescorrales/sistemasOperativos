#include <stdio.h>

int main(){
    FILE *fp = fopen("06mmap.txt", "r");
    char ch;
    char *txt="";
    fseek(fp,0,SEEK_END);
    long size=ftell(fp);    
    char texto[size];
    long i=0;
    fseek(fp,0,0);
    //while ((ch = fgetc(fp)) != EOF){
    while (i<size){
        //putchar(ch);
        ch = fgetc(fp);
        texto[i]=ch;
        i++;
    }       
    printf("%s",texto);
    // close the file
    fclose(fp);
}
