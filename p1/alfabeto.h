/**/
#ifndef ALFABETO_H
#define ALFABETO_H

/********************************************************
* Name: Alfabeto
* Description: Alfabeto data type declaration
********************************************************/
typedef struct _Alfabeto Alfabeto;

/********************************************************
* Name: crear_alfabeto
* Description: returns an Alfabeto of the passed size
********************************************************/
Alfabeto *crear_alfabeto(int size);

/********************************************************
* Name: destruye_alfabeto
* Description: destroys the passed Alfabeto
********************************************************/
void destruye_alfabeto(Alfabeto *a);

/********************************************************
* Name: alfabeto_aniade_palabra
* Description: adds a word to an alphabet
********************************************************/
int alfabeto_aniade_palabra(Alfabeto *a, char *palabra);

/********************************************************
* Name: get_palabra_by_index
* Description: gets the word stored in the index passed of a passed alphabet
********************************************************/
char *get_palabra_by_index(Alfabeto *a, int index);

/********************************************************
* Name: get_words_number
* Description: get the words number of an alphabet
********************************************************/
int get_words_number(Alfabeto *a);

#endif
