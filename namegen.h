#ifndef NAMEGEN_H
#define NAMEGEN_H
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mtwister.h"
#define MAXNAMES 1024
#define NAMELEN 20
#define NAMEPLEN 5
void NameGen_Init(void);
void NameGen_Load(void);
void NameGen_Calc(void);
void NameGen_Gen(FILE *fp);
int NameGen__IsVowel(char a);

struct name_tag{
    char str[NAMELEN];
} name_tag;
struct name_tag *names=NULL;
struct {
    int namescount;
    char length[MAXNAMES];
    MTRand mtr;
    void (*init)(void);
    void (*load)(void);
    void (*calc)(void);
    void (*gen)(FILE *fp);
    int icount;
    char initiater[MAXNAMES][NAMEPLEN];
    int vcount;
    char vowel[MAXNAMES][NAMEPLEN];
    int ncount;
    char nvowel[MAXNAMES][NAMEPLEN];
    int ecount;
    char ender[MAXNAMES][NAMEPLEN];
} namegen={
    0,{0},{0},
    NameGen_Init,
    NameGen_Load,
    NameGen_Calc,
    NameGen_Gen,
    0,{0},0,{0},0,{0},0,{0},
};


void NameGen_Init(void){
    names=malloc(MAXNAMES*sizeof(name_tag));
    namegen.mtr=seedRand(time(NULL));
}
void NameGen_Load(void){
    FILE *fp=fopen("data.txt","r");
    if(fp==NULL){
        exit(-1);
    }
    char *tmpbuf=malloc(1024*sizeof(char));
    for(;(!feof(fp))&&namegen.namescount<MAXNAMES;){
        memset(names[namegen.namescount].str,0,NAMELEN);
        fgets(tmpbuf,1024,fp);
        if(tmpbuf[0]=='#')continue;
        if(strlen(tmpbuf)>NAMELEN)continue;
        if(strlen(tmpbuf)<=2)continue;
        while(tmpbuf[strlen(tmpbuf)-1]=='\n'||
              tmpbuf[strlen(tmpbuf)-1]=='\r')
            tmpbuf[strlen(tmpbuf)-1]=0;
        if(strlen(tmpbuf)<2)continue;
        strncpy(names[namegen.namescount].str,tmpbuf,NAMELEN-1);
//        fscanf(fp,"%s",names[namegen.namescount].str);
        namegen.namescount++;
    }
    fclose(fp);
}
void NameGen_Calc(void){
    char name[NAMELEN];
    for(int i=0;i<namegen.namescount;i++){
        memset(name,0,NAMELEN);
        strcpy(name,names[i].str);
        int vowel=NameGen__IsVowel(name[0]);
        char namepart[NAMEPLEN];
        memset(namepart,0,NAMEPLEN);
        for(int j=0,init=1,save=0,namelen=0;j<strlen(name);j++){
            if(NameGen__IsVowel(name[j])){
                if(vowel&&strlen(namepart)<NAMEPLEN-1){
                    namepart[strlen(namepart)]=name[j];
                    if(j<strlen(name)-1)continue;
                    else save=3;
                }else{
                    save=1;
                    j--;
                }
            }else{
                if((!vowel)&&strlen(namepart)<NAMEPLEN-1){
                    namepart[strlen(namepart)]=name[j];
                    if(j<strlen(name)-1)continue;
                    else save=3;
                }else{
                    save=2;
                    j--;
                }
            }
            if(save&&strlen(namepart)){
                if(init==1){
                    memset(namegen.initiater[namegen.icount],0,NAMEPLEN);
                    strcpy(namegen.initiater[namegen.icount],namepart);
                    namegen.icount++;
                    init=0;
                }else if(j==strlen(name)-1){
                    memset(namegen.ender[namegen.ecount],0,NAMEPLEN);
                    strcpy(namegen.ender[namegen.ecount],namepart);
                    namegen.ecount++;
                }else if(save==1){ // save namepart as a non-vowel
                    memset(namegen.nvowel[namegen.ncount],0,NAMEPLEN);
                    strcpy(namegen.nvowel[namegen.ncount],namepart);
                    namegen.ncount++;
                }else if(save==2){
                    memset(namegen.vowel[namegen.vcount],0,NAMEPLEN);
                    strcpy(namegen.vowel[namegen.vcount],namepart);
                    namegen.vcount++;
                }
                memset(namepart,0,NAMEPLEN);
                namelen++;
                if(j==strlen(name)-1){
                    namegen.length[i]=namelen;
                }else{
                    vowel=NameGen__IsVowel(name[j+1]);
                }
                save=0;
            }
        }
    }
}
void NameGen_Gen(FILE *fp){
    char result[NAMELEN*2];
    memset(result,0,NAMELEN*2);

    char *init=namegen.initiater[genRandLong(&namegen.mtr)%namegen.icount];
    int vowel=NameGen__IsVowel(init[0]);
    strcat(result,init);

    int lenid=genRandLong(&namegen.mtr)%namegen.namescount;
    int rem=namegen.length[lenid]-2;
    for(int i=0;i<rem;i++){
        vowel=1-vowel;
        if(vowel){
            strcat(result,namegen.vowel[genRandLong(&namegen.mtr)%namegen.vcount]);
        }else{
            strcat(result,namegen.nvowel[genRandLong(&namegen.mtr)%namegen.ncount]);
        }
    }

    vowel=1-vowel;
    for(int i=genRandLong(&namegen.mtr)%namegen.ecount;;i++){
        if(i==namegen.ecount)i=0;
        if(NameGen__IsVowel(namegen.ender[i][0])==vowel){
            strcat(result,namegen.ender[i]);
            break;
        }
    }

    if(fp==NULL)printf("%-25s",result);
    else fprintf(fp,"%-25s",result);
}
int NameGen__IsVowel(char a){
    switch(a){
    case 'a':return 1;
    case 'A':return 1;
    case 'e':return 1;
    case 'E':return 1;
    case 'i':return 1;
    case 'I':return 1;
    case 'o':return 1;
    case 'O':return 1;
    case 'u':return 1;
    case 'U':return 1;
    case 'y':return 1;
    case 'Y':return 1;
    default:return 0;
    }
}
#endif
