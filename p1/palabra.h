/**/

#ifndef PALABRA_H
#define PALABRA_H

/********************************************************
* Name: create_word
* Description:  creates a word with size the passed size
********************************************************/
Palabra* create_word( int size );

/********************************************************
* Name: destroy_word
* Description:  destroys the passed word
********************************************************/
void destroy_word(Palabra* palabra);

/********************************************************
* Name: Palabra
* Description: Palabra data type declaration
********************************************************/
typedef struct _Palabra Palabra;

/********************************************************
* Name: get_symbol_by_index
* Description:  returns the symbol stored in the index passed as argument
********************************************************/
char* get_symbol_by_index(Palabra *palabra, int index);

/********************************************************
* Name: get_word_size
* Description: returns the size of a passed word
********************************************************/
char* get_word_size(Palabra *palabra);

/********************************************************
* Name: set_symbol_by_index
* Description:  sets the symbol of a certain index in the word 
********************************************************/
int add_symbol(Palabra *palabra, char* symbol);

/********************************************************
* Name: set_word_size
* Description:  sets the size of the passed word
********************************************************/
int set_word_size(Palabra *palabra, int new_size);

#endif