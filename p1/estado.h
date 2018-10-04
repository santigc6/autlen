/**/
#ifndef ESTADO_H
#define ESTADO_H

#define INICIAL 2
#define NORMAL 3
#define FINAL 4

typedef struct _Estado Estado;

Estado *crear_estado(char *name, int type);
void destruir_estado(Estado *s);
char *estado_get_name(Estado *s);
int estado_get_tipo(Estado *s);

#endif
