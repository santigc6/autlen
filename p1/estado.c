/**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "estado.h"
#include "types.h"

/********************************************************
* Name: Estado
* Description: Estado data tipo definition
********************************************************/

struct _Estado{
	char* nombre;
	int tipo;
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

void destruir_lista_estados(Estado **lista){
	if(!lista){
		return;
	}

	free(lista);
}

/********************************************************
* Name: crear_estado
* Description:  creates a state with the passed name and tipo
********************************************************/

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

/********************************************************
* Name: destruir_estado
* Description: destroys the state passed as argument
********************************************************/

void destruir_estado(Estado *s){
	if(!s)
		return;
	if(s->nombre)
		free(s->nombre);
	if(s)
		free(s);
	
}

/********************************************************
* Name: estado_get_name
* Description: returns the name of the passed state
********************************************************/

char *estado_get_name(Estado *s){
	if(!s || !s->nombre)
		return NULL;
	
	return s->nombre;
}
	
/********************************************************
* Name: estado_get_tipo
* Description:  returns the state tipo of the state passed as argument
********************************************************/

int estado_get_tipo(Estado *s){
	if(!s || (s->tipo != INICIAL && s->tipo != NORMAL && s->tipo != FINAL && s->tipo != INICIAL_Y_FINAL))
		return ERROR;
	
	return s->tipo;
}

void print_estados(FILE *fd, Estado **e, int flag_format, int n_estados){
	int i;

	if(!fd || !e)
		return;

	if(flag_format==0){
		fprintf(fd, "Q={ ");
		for(i=0; i<n_estados; i++){
			if(e[i]->tipo == INICIAL || e[i]->tipo == INICIAL_Y_FINAL){
				fprintf(fd, " ->");
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
	}else{
		fprintf(fd, "{ ");
		for(i=0; i<n_estados; i++){
			if(e[i]->tipo == INICIAL || e[i]->tipo == INICIAL_Y_FINAL){
				fprintf(fd, " ->");
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
