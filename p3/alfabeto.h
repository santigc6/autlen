/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#ifndef ALFABETO_H
#define ALFABETO_H

typedef struct _Alfabeto Alfabeto;

/********************************************************
* Description: crea un alfabeto del tamanio especificado
********************************************************/
Alfabeto *crear_alfabeto(int size);

/********************************************************
* Description: Destruye un alfabeto
********************************************************/
void destruye_alfabeto(Alfabeto *a);

/********************************************************
* Description: Aniade una palabra al alfabeto
********************************************************/
int alfabeto_aniade_palabra(Alfabeto *a, char *palabra);

/********************************************************
* Description: Devuelve la palabra de un indice del alfabeto
********************************************************/
char *get_palabra_by_index(Alfabeto *a, int index);

/********************************************************
* Description: Devuelve el numero de palabras del alfabeto
********************************************************/
int get_words_number(Alfabeto *a);

/********************************************************
* Description: Devuelve el tamanio del alfabeto
********************************************************/
int get_size_alfabeto(Alfabeto *a);

/********************************************************
* Description: Devuelve el tamanio actual del alfabeto
********************************************************/
int get_curr_alfabeto(Alfabeto *a);

/********************************************************
* Description: Imprime el alfabeto
********************************************************/
void print_alfabeto(FILE *fd, Alfabeto* a);

#endif
