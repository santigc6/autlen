/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#ifndef ESTADO_H
#define ESTADO_H

/* State types */
#define INICIAL 2
#define NORMAL 3
#define FINAL 4
#define INICIAL_Y_FINAL 5

typedef struct _Estado Estado;

/********************************************************
* Description: Initializes a states' list
* WARNING: it doesnt free all the states in the list
* (they must be freed before calling this function)
********************************************************/
Estado **inicializar_lista_estados(int size);

/********************************************************
* Description: Extends a states' list
********************************************************/
Estado ***ampliar_lista_estados(Estado ***e, int new_size);

/********************************************************
* Description: Destroys a states' list
********************************************************/
void destruir_lista_estados(Estado **lista);

/********************************************************
* Description: Creates a state with the given name and type
********************************************************/
Estado *crear_estado(char *name, int type);

/********************************************************
* Description: Destroys a state
********************************************************/
void destruir_estado(Estado *s);

/********************************************************
* Description: returns the name of a given state
********************************************************/
char *estado_get_name(Estado *s);

/********************************************************
* Description: changes the name of a given state
********************************************************/
char *estado_set_name(Estado *s, char *new_name);

/********************************************************
* Description: changes the type of a given state
********************************************************/
void estado_set_tipo(Estado *s, int tipo);

/********************************************************
* Description: returns the type of a given state
********************************************************/
int estado_get_tipo(Estado *s);

/********************************************************
* Description: returns the index of the given state name
********************************************************/
int estado_get_index(Estado **s, int length, char *state_name);

/********************************************************
* Description: prints the states of a given list.
* flag_format:  0   : Q={ } using -> and * format
*               1   : Transition format
*               else: Actualmente en format
********************************************************/
void print_estados(FILE *fd, Estado **e, int flag_format, int n_estados);

/********************************************************
* Description: prints the name of a state
********************************************************/
void print_estado(FILE *fd, Estado *e);

#endif
