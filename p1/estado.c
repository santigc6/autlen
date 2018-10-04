/**/
#include <stdio.h>
#include <string.h>

#include "estado.h"
#include "types.h"

struct _Estado{
	char* nombre;
	int tipo;
}


Estado *crear_estado(char *name, int type){
	Estado *s=NULL;
	
	if(!name || (type != INCIAL && type != NORMAL && type != FINAL))
		return NULL;
	
	s=(Estado *)malloc(sizeof(Estado));
	if(!s)
		return NULL;
	
	s->nombre=(char *)malloc((strlen(name)+1)*sizeof(char));
	if(!s->nombre){
		free(s);
		return NULL;
	}
	
	s->tipo=type;
	
	return s;
}

void destruir_estado(Estado *s){
	if(!s)
		return;
	if(s->nombre)
		free(s->nombre);
	if(s)
		free(s);
	
}

char *estado_get_name(Estado *s){
	if(!s || !s->nombre)
		return NULL;
	
	return s->nombre;
}
	
int estado_get_tipo(Estado *s){
	if(!s || (s->type != INCIAL && s->type != NORMAL && s->type != FINAL))
		return ERROR;
	
	return s->type;
}
