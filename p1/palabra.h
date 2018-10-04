/**/

#ifndef PALABRA_H
#define PALABRA_H

typedef struct _Palabra Palabra;

char* get_symbol_by_index(Palabra *palabra, int index);

char* get_word_size(Palabra *palabra);

int set_symbol_by_index(Palabra *palabra, int index, char* symbol);

int set_word_size(Palabra *palabra, int new_size);