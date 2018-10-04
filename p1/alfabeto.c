/**/
#include <stdio.h>
#include <string.h>

#include "alfabeto.h"
#include "types.h"

struct _Alfabeto{
  char **symbols;
  int size;
}

Alfabeto *crear_alfabeto(int size){
  Alfabeto *a=NULL;
  
  if(size <= 0)
    return NULL;
  
  a=(Alfabeto *)malloc(sizeof(Alfabeto));
  if(!a)
    return NULL;
  
  a->size=size;
  
  a->symbols=(char **)malloc(sizeof(char *)*size);
  if(!a->symbols){
    free(a);
    return NULL;
  }
  
  return s;
}

void destruye_alfabeto(Alfabeto *a){
  int i;
  
  if(!a)
    return;
  
  if(!a->symbols){
    free(a);
    return;
  }
    
  for(i=0; i<a->size; i++){
    if(a->symbols[i])
      free(a->symbols[i]);
  }
  
  free(a->symbols);
  free(a);
}

int alfabeto_aniade_palabra(Alfabeto *a, char *palabra);
char *get_palabra_by_index(Alfabeto *a, int index);
int get_words_number(Alfabeto *a);
