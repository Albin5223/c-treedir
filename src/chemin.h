#include<stdio.h>
#include<stdlib.h>
#include<string.h>



char *couper(char *s1){
    char *res="";
    size_t i = 0;
    while(*(s1+i) != '/' && *(s1+i) != '\0'){
        res = strcat(res,s1+i);
        i++;

    }

    return res;
}