/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#ifndef PALABRA_H
#define PALABRA_H

typedef struct _Palabra Palabra;

/********************************************************
* Name: create_word
* Description:  creates a word with size the passed size
********************************************************/
Palabra* create_word(int size);

/********************************************************
* Name: destroy_word
* Description:  destroys the passed word
********************************************************/
void destroy_word(Palabra* palabra);

/********************************************************
* Name: get_symbol_by_index
* Description:  returns the symbol stored in the index passed as argument
********************************************************/
char* get_symbol_by_index(Palabra *palabra, int index);

/********************************************************
* Name: get_word_size
* Description: returns the size of a passed word
********************************************************/
int get_word_size(Palabra *palabra);

/********************************************************
* Name: add_symbol
* Description:  adds a symbol to the word 
********************************************************/
int add_symbol(Palabra *palabra, char* symbol);

/********************************************************
* Name: set_word_size
* Description:  sets the size of the passed word
********************************************************/
int set_word_size(Palabra *palabra, int new_size);

/********************************************************
* Name: get_process
* Description: returns the index to process
********************************************************/
int get_process(Palabra *p);

/********************************************************
* Name: word_nex
* Description: to advance to the next symbol
********************************************************/
void word_next(Palabra *p);

/********************************************************
* Name: reset_word
* Description: resets the process and the current size
********************************************************/
void reset_word(Palabra *p);

#endif
