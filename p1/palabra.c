/**/
#include "types.h"
#include <string.h>

#define DEFAULT 1

/********************************************************
* Name: Palabra
* Description: Palabra data type definition
********************************************************/

struct _Palabra {
  char **symbols;
  int size;
  int current;
  int process;
}

/********************************************************
* Name: create_word
* Description:  creates a word with size the passed size
********************************************************/

Palabra* create_word(int size){
	 
	int i = 0;
	Palabra* palabra = NULL;

	if(size <= 0){
		return NULL;
	}
	 
	palabra = (Palabra *)malloc(sizeof(Palabra));
	if (!palabra){
		return NULL;
	}

	palabra->symbols = (char**)malloc(size*sizeof(char*));

	if(!palabra->symbols){
		free(palabra);
		return NULL;
	}

	palabra->size = size;
	palabra->current = 0;
	palabra->process = 0;

	return palabra;
}

/********************************************************
* Name: destroy_word
* Description:  destroys the passed word
********************************************************/

void destroy_word(Palabra* palabra){
	int i = 0;

	if(!palabra){
		return;
	}

	if(!palabra->symbols){
		free(palabra);
		return;
	}
	
	for(i = 0; i < palabra->current; i++){
		if(palabra->symbols[i])
			free(palabra->symbols[i]);
	}

	free(palabra->symbols);
	free(palabra);
}

/********************************************************
* Name: get_symbol_by_index
* Description:  returns the symbol stored in the index passed as argument
********************************************************/

char* get_symbol_by_index(Palabra *palabra, int index){
	if(!palabra || index < 0 || index >= palabra->current){
		return NULL;
	}

	return palabra->symbols[index];
}

/********************************************************
* Name: get_word_size
* Description: returns the size of a passed word
********************************************************/

char* get_word_size(Palabra *palabra){
	if(!palabra){
		return NULL;
	}

	return palabra->current;
}

/********************************************************
* Name: set_symbol_by_index
* Description:  sets the symbol of a certain index in the word 
********************************************************/

int add_symbol(Palabra *palabra, char* symbol){
	if(!palabra || !symbol){
		return ERROR;
	}
	if(palabra->current == palabra->size){ /* Necesitamos mas memoria */
		palabra->symbols=(char **)realloc(palabra->symbols, (palabra->size+DEFAULT) * sizeof(char *));
		if(!palabra->symbols)
		  return ERROR;

		palabra->size += DEFAULT;
	}
	palabra->symbols[palabra->current]=(char *)malloc((strlen(symbol)+1)*sizeof(char));
	if(!palabra->symbols[palabra->current])
	  return ERROR;
	  
	strcpy(palabra->symbols[palabra->current], symbol);
	palabra->current++;

	return OK;
}

/********************************************************
* Name: set_word_size
* Description:  sets the size of the passed word
********************************************************/
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