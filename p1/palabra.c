/**/
#include "types.h"
#include <string.h>


/********************************************************
* Name: Palabra
* Description: Palabra data type definition
********************************************************/

struct _Palabra {
  char **symbols;
  int size;
  int current;
}


/********************************************************
* Name: get_symbol_by_index
* Description:  returns the symbol stored in the index passed as argument
********************************************************/

char* get_symbol_by_index(Palabra *palabra, int index){
	
	if ( palabra && (index >= 0) && (index < (palabra->size)) ){
		return palabra->symbols[index];
	}
}

/********************************************************
* Name: get_word_size
* Description: returns the size of a passed word
********************************************************/

char* get_word_size(Palabra *palabra){

	if( palabra ){
		return palabra->current+1;
	}
}

/********************************************************
* Name: set_symbol_by_index
* Description:  sets the symbol of a certain index in the word 
********************************************************/

int set_symbol_by_index(Palabra *palabra, int index, char* symbol){

	if( palabra && (index >= 0) && (index < (palabra->size)) ){
		palabra->symbols[index] = symbol;

		return OK;
	}

	return ERROR;
}

/********************************************************
* Name: set_word_size
* Description:  sets the size of the passed word
********************************************************/

int set_word_size(Palabra *palabra, int new_size){

	if(palabra && (new_size >= 0) & (new_size < 256) ){
		palabra->size = new_size;
		return OK;
	}

	return ERROR;
}

/********************************************************
* Name: create_word
* Description:  creates a word with size the passed size
********************************************************/

Palabra* create_word( int size ){
	 
	int i = 0;
	Palabra* palabra = NULL;

	if(size <= 0){
		return NULL;
	}
	 
	palabra = (char **)malloc(size*(sizeof(char *)));

	if (!palabra){
		free(palabra);
		return NULL;
	}

	for (i = 0; i < size; i++){
		palabra->symbols[i] = (char*)malloc(MAX_SIZE * sizeof(char));
	}

	palabra->size = size;
	palabra->current = 0;
}

/********************************************************
* Name: destroy_word
* Description:  destroys the passed word
********************************************************/

void destroy_word(Palabra* palabra){

	if(!palabra){
		return;
	}

	int i = 0;
	
	for(i = 0; i < palabra->size; i++){
		free(palabra->symbols[i]);
	}

	free(palabra->size);
	free(palabra->current);
	free(palabra);
}
