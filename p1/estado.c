/**/
#include <stdio.h>
#include <string.h>
#include "estado.h"

struct _Estado{
	char* nombre;
	int tipo;
}


Estado *crear_estado(char *name, int type);
void destruir_estado(Estado *s);
char *estado_get_name(Estado *s);
int estado_get_tipo(Estado *s);
