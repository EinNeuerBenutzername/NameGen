#include <stdio.h>
#include "namegen.h"
int main(){
    printf("Name Generater\n\n\n");
    namegen.init();
    namegen.load();
    printf("%d names loaded.\n",namegen.namescount);
    namegen.calc();
    FILE *out=fopen("out.txt","w");
    for(int i=0;i<333;i++){
        for(int j=0;j<3;j++)namegen.gen(out);
        fprintf(out,"\n");
    }
    fclose(out);
    return 0;
}
