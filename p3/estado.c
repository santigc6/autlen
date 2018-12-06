/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "estado.h"
#include "types.h"

/********************************************************
* Name: Estado
* Description: Estado data type definition
********************************************************/
struct _Estado{
	char* nombre; /* Name of the state */
	int tipo; /* State type */
};

Estado **inicializar_lista_estados(int size){
	Estado **e=NULL;

	if(size <= 0)
		return NULL;

	e=(Estado **)malloc(sizeof(Estado *) * size);
	if(!e)
		return NULL;

	return e;
}

Estado **ampliar_lista_estados(Estado **e, int new_size){
	if(new_size <= 0)
		return NULL;

	e=(Estado **)realloc(e, sizeof(Estado *) * new_size);
	if(!e)
		return NULL;

	return e;
}

void destruir_lista_estados(Estado **lista){
	if(!lista){
		return;
	}

	free(lista);
	lista=NULL;
}

Estado *crear_estado(char *name, int tipo){
	Estado *s=NULL;
	
	if(!name || (tipo != INICIAL && tipo != NORMAL && tipo != FINAL && tipo != INICIAL_Y_FINAL))
		return NULL;

	s=(Estado *)malloc(sizeof(Estado));
	if(!s)
		return NULL;
	
	s->nombre=(char *)malloc((strlen(name)+1)*sizeof(char));
	if(!s->nombre){
		free(s);
		return NULL;
	}
  strcpy(s->nombre, name);
	
	s->tipo=tipo;

	return s;
}

void destruir_estado(Estado *s){
	if(!s)
		return;
	if(s->nombre)
		free(s->nombre);
	if(s){
		free(s);
		s=NULL;
	}
	
}

char *estado_set_name(Estado *s, char *new_name){
	if(!s || !new_name)
		return NULL;
	
	s->nombre=(char *)realloc(s->nombre, (strlen(new_name)+1)*sizeof(char));
	if(!s->nombre){
		free(s);
		return NULL;
	}
  	strcpy(s->nombre, new_name);

	return s->nombre;
}

char *estado_get_name(Estado *s){
	if(!s || !s->nombre)
		return NULL;
	
	return s->nombre;
}

void estado_set_tipo(Estado *s, int tipo){
	if(!s || (tipo != INICIAL && tipo != NORMAL && tipo != FINAL && tipo != INICIAL_Y_FINAL))
		return;

	s->tipo = tipo;
}

int estado_get_tipo(Estado *s){
	if(!s || (s->tipo != INICIAL && s->tipo != NORMAL && s->tipo != FINAL && s->tipo != INICIAL_Y_FINAL))
		return ERROR;
	
	return s->tipo;
}

int estado_get_index(Estado **s, int length, char *state_name){
  int i;
  
  if(!s || !state_name || length <= 0)
    return -1;
    
  for(i=0; i<length; i++){
    if(!strcmp(s[i]->nombre, state_name)){
      return i;
    }
  }
  
  return -1;
}

void print_estados(FILE *fd, Estado **e, int flag_format, int n_estados){
	int i;

	if(!fd || !e)
		return;

	if(flag_format==0){ /* Format to represent the list of possible states */
		fprintf(fd, "Q={ ");
		for(i=0; i<n_estados; i++){
			if(e[i]->tipo == INICIAL || e[i]->tipo == INICIAL_Y_FINAL){
				fprintf(fd, "->");
			}
			fprintf(fd, "%s", e[i]->nombre);
			if(e[i]->tipo == FINAL || e[i]->tipo == INICIAL_Y_FINAL){
				fprintf(fd, "*");
			}
			fprintf(fd, " ");
		}
		fprintf(fd, "}\n\n");
	} else if(flag_format==1){ /* Transition format */
		fprintf(fd, "={ ");
		for(i=0; i<n_estados; i++){
			fprintf(fd, "%s ", e[i]->nombre);
		}
		fprintf(fd, "}\n");
	}else{ /* Actualmente en */
		fprintf(fd, "{ ");
		for(i=0; i<n_estados; i++){
			if(e[i]->tipo == INICIAL || e[i]->tipo == INICIAL_Y_FINAL){
				fprintf(fd, "->");
			}
			fprintf(fd, "%s", e[i]->nombre);
			if(e[i]->tipo == FINAL || e[i]->tipo == INICIAL_Y_FINAL){
				fprintf(fd, "*");
			}
			fprintf(fd, " ");
		}
		fprintf(fd, "}\n");
	}
}

void print_estado(FILE *fd, Estado *e){
	if(!fd || !e)
		return;

	fprintf(fd, "%s", e->nombre);
}
