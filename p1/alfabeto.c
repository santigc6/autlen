/**/
#include <stdio.h>
#include <string.h>

#include "alfabeto.h"
#include "types.h"

/********************************************************
* Name: Alfabeto
* Description: Alfabeto data type definition
********************************************************/
struct _Alfabeto{
  char **symbols;
  int size;
  int current;
}

/********************************************************
* Name: crear_alfabeto
* Description: returns an Alfabeto of the passed size
********************************************************/
Alfabeto *crear_alfabeto(int size){
	
	Alfabeto *alfabeto = NULL;
	int i = 0;
	
	if(size <= 0){
		return NULL;
	}

	alfabeto = (char**)malloc(size*(sizeof(char**)));

	if (!alfabeto){
		free(alfabeto);
		return NULL;
	}

	for (i = 0; i < size; i++){
		alfabeto->symbols[i] = (char*)malloc(MAX_SIZE * sizeof(char));
	}

	alfabeto->size = size;
	alfabeto->current = 0;

}
	

/********************************************************
* Name: destruye_alfabeto
* Description: destroys the passed Alfabeto
********************************************************/
void destruye_alfabeto(Alfabeto *a){

	if(!a){
		return;
	}

	int i = 0;
	
	for(i = 0; i < a->size; i++){
		free(a->symbols[i]);
	}

	free(a->size);
	free(a->current);
	free(palabra);
}

/********************************************************
* Name: alfabeto_aniade_palabra
* Description: adds a word to an alphabet
********************************************************/
int alfabeto_aniade_palabra(Alfabeto *a, char *palabra){
	if(!a || !palabra){
		return ERROR;
	}

	if(a->current >= a->size){
		return ERROR;
	}

	a->symbols[a->current] = palabra;
	a->current = a->current + 1;

	return OK;
}

/********************************************************
* Name: get_palabra_by_index
* Description: gets the word stored in the index passed of a passed alphabet
********************************************************/
char *get_palabra_by_index(Alfabeto *a, int index){

	if(!a || index >= a->size || index >= a->current){
		return NULL;
	}

	return a->symbols[index];
}

/********************************************************
* Name: get_words_number
* Description: get the words number of an alphabet
********************************************************/
int get_words_number(Alfabeto *a){

	if(!a){
		return 0;
	}

	return a->current;
}