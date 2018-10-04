/**/
#ifndef ALFABETO_H
#define ALFABETO_H

typedef struct _Alfabeto Alfabeto;

Alfabeto *crear_alfabeto(int size);
void destruye_alfabeto(Alfabeto *a);
int alfabeto_aniade_palabra(Alfabeto *a, char *palabra);
char *get_palabra_by_index(Alfabeto *a, int index);
int get_words_number(Alfabeto *a);

#endif
