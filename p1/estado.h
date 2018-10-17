/**/
#ifndef ESTADO_H
#define ESTADO_H

#define INICIAL 2
#define NORMAL 3
#define FINAL 4
#define INICIAL_Y_FINAL 5

/********************************************************
* Name: Estado
* Description: Estado data type declaration
********************************************************/
typedef struct _Estado Estado;

Estado **inicializar_lista_estados(int size);

void destruir_lista_estados(Estado **lista);

/********************************************************
* Name: crear_estado
* Description:  creates a state with the passed name and type
********************************************************/
Estado *crear_estado(char *name, int type);

/********************************************************
* Name: destruir_estado
* Description: destroys the state passed as argument
********************************************************/
void destruir_estado(Estado *s);

/********************************************************
* Name: estado_get_name
* Description: returns the name of the passed state
********************************************************/
char *estado_get_name(Estado *s);

/********************************************************
* Name: estado_get_tipo
* Description:  returns the state type of the state passed as argument
********************************************************/
int estado_get_tipo(Estado *s);

void print_estados(FILE *fd, Estado **e, int flag_format, int n_estados);

void print_estado(FILE *fd, Estado *e);

#endif
