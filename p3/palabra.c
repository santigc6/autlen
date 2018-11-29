/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "palabra.h"

#define DEFAULT 1

/********************************************************
* Name: Palabra
* Description: Palabra data type definition
********************************************************/
struct _Palabra {
  char **symbols; /* Each symbol of the word */
  int size; /* Size of the word */
  int current; /* Current size of the word */
  int process; /* Index that is being processed */
};

Palabra* create_word(int size){
	Palabra* palabra = NULL;
	int i;
	
	if(size <= 0){
		return NULL;
	}
	 
	palabra = (Palabra *)malloc(sizeof(Palabra));
	if (!palabra){
		return NULL;
	}

	palabra->symbols = (char**)malloc(size*sizeof(char*));
  for(i=0; i<size; i++)
    palabra->symbols[i]=NULL;

	if(!palabra->symbols){
		free(palabra);
		return NULL;
	}

	palabra->size = size;
	palabra->current = 0;
	palabra->process = 0;

	return palabra;
}

void destroy_word(Palabra* palabra){
	int i = 0;

	if(!palabra){
		return;
	}

	if(!palabra->symbols){
		free(palabra);
		return;
	}
	
	for(i = 0; i < palabra->size; i++){
		if(palabra->symbols[i])
			free(palabra->symbols[i]);
	}

	free(palabra->symbols);
	free(palabra);
}

char* get_symbol_by_index(Palabra *palabra, int index){
	if(!palabra || index < 0 || index >= palabra->current){
		return NULL;
	}

	return palabra->symbols[index];
}

int get_word_size(Palabra *palabra){
	if(!palabra){
		return -1;
	}

	return palabra->current;
}

int add_symbol(Palabra *palabra, char* symbol){
	if(!palabra || !symbol){
		return ERROR;
	}
	if(palabra->current == palabra->size){ /* Necesitamos mas memoria antes de aniadir el simbolo */
		palabra->symbols=(char **)realloc(palabra->symbols, (palabra->size+DEFAULT) * sizeof(char *));
		if(!palabra->symbols)
		  return ERROR;

    palabra->symbols[palabra->current]=NULL;
		palabra->size += DEFAULT;
	}
	if(palabra->symbols[palabra->current]){
	  free(palabra->symbols[palabra->current]);
	}
	palabra->symbols[palabra->current]=(char *)malloc((strlen(symbol)+1)*sizeof(char));
	if(!palabra->symbols[palabra->current])
	  return ERROR;
	  
	strcpy(palabra->symbols[palabra->current], symbol);
	palabra->current++;

	return OK;
}

int set_word_size(Palabra *palabra, int new_size){
	if(!palabra || new_size <= 0 || palabra->current <= new_size){
		return ERROR;
	}

	palabra->size = new_size;

	return OK;
}

int get_process(Palabra *p){
	if(!p)
		return -1;

	return p->process;
}

void word_next(Palabra *p){
	if(!p)
		return;

	p->process++;
}

void reset_word(Palabra *p){
  if(!p)
    return;
    
  p->current=0;
  p->process=0;
}
