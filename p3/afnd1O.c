#include <stdio.h>
#include "afnd1O.h"

AFND *AFND1ODeSimbolo(char *simbolo){
    if(!simbolo)
        return NULL;
}

AFND *AFND1ODeLambda(){

}

AFND *AFND1ODeVacio(){

}

AFND *AFNDAAFND1O(AFND *p_afnd){
    if(!p_afnd)
        return NULL;
}

AFND *AFND1OUne(AFND *p_afnd1O_1, AFND *p_afnd1O_2){
    if(!p_afnd1O_1 || !p_afnd1O_2)
        return NULL;
}

AFND *AFND1OConcatena(AFND *p_afnd_origen1, AFND *p_afnd_origen2){
    if(!p_afnd_origen1 || !p_afnd_origen2)
        return NULL;
}

AFND *AFND1OEstrella(AFND *p_afnd_origen){
    if(!p_afnd_origen)
        return NULL;
}

void AFNDADot(AFND * p_afnd){
    if(!p_afnd)
        return;
}
