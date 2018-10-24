/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "alfabeto.h"
#include "types.h"

/********************************************************
* Name: Alfabeto
* Description: Alfabeto data type definition
********************************************************/
struct _Alfabeto{
  char **symbols; /* Palabras del alfabeto */
  int size; /* Tamanio del alfabeto */
  int current; /* Tamanio actual del alfabeto */
};

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
  
  a->current=0;
  
  return a;
}

void destruye_alfabeto(Alfabeto *a){
  int i;
  
  if(!a)
    return;
  
  if(!a->symbols){
    free(a);
    return;
  }
    
  for(i=0; i<a->current; i++){
    if(a->symbols[i])
      free(a->symbols[i]);
  }
  
  free(a->symbols);
  free(a);
}

int alfabeto_aniade_palabra(Alfabeto *a, char *palabra){
  if(!a || !palabra || a->current == a->size)
    return ERROR;
  
  a->symbols[a->current]=(char *)malloc((strlen(palabra)+1)*sizeof(char));
  if(!a->symbols[a->current])
    return ERROR;
  
  strcpy(a->symbols[a->current], palabra);
  a->current++;
  
  return OK;
}

char *get_palabra_by_index(Alfabeto *a, int index){
  if(!a || index < 0 || index >= a->current)
    return NULL;
  
  return a->symbols[index];
}

int get_size_alfabeto(Alfabeto *a){
  if(!a)
    return -1;
  
  return a->current;

}

void print_alfabeto(FILE *fd, Alfabeto* a){
	int i=0;

	if(!fd || !a)
		return;

	fprintf(fd, "A={ ");
	for(i=0; i<a->current; i++){
		fprintf(fd, "%s ", a->symbols[i]);
	}
	fprintf(fd, "}\n\n");
}
