/**/
#include "types.h"
#include <string.h>


#define MAX_SIZE 256

struct _Palabra {
  char **symbols;
  int size;
  int current;
}

char* get_symbol_by_index(Palabra *palabra, int index){
	
	if ( palabra && (index >= 0) && (index < (palabra->size)) ){
		return palabra->symbols[index];
	}
}


char* get_word_size(Palabra *palabra){

	if( palabra ){
		return palabra->size;
	}
}

int set_symbol_by_index(Palabra *palabra, int index, char* symbol){

	if( palabra && (index >= 0) && (index < (palabra->size)) ){
		palabra->symbols[index] = symbol;

		return OK;
	}

	return ERROR;
}

int set_word_size(Palabra *palabra, int new_size){

	if(palabra && (new_size >= 0) & (new_size < 256) ){
		palabra->size = new_size;
		return OK;
	}

	return ERROR;
}


Palabra* create_word( int size ){
	 Palabra* palabra = NULL;

	 if(size <= 0){
	 	return NULL;
	 }
	 
	 palabra = (char **)malloc(size*(char *));

}

