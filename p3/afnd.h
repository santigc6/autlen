/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#ifndef AFND_H
#define AFND_H

typedef struct _AFND AFND;

/* Las cabeceras estan especificadas en el enunciado de la practica */
AFND *AFNDNuevo(char *nombre, int num_estados, int num_simbolos);
void AFNDElimina(AFND *p_afnd);
void AFNDImprime(FILE *fd, AFND *p_afnd);
AFND *AFNDInsertaSimbolo(AFND *p_afnd, char *simbolo);
AFND *AFNDInsertaEstado(AFND *p_afnd, char *nombre, int tipo);
AFND *AFNDInsertaTransicion(AFND *p_afnd, char *nombre_estado_i, char *nombre_simbolo_entrada, char *nombre_estado_f );
AFND *AFNDInsertaLetra(AFND *p_afnd, char *letra);
void AFNDImprimeConjuntoEstadosActual(FILE *fd, AFND *p_afnd);
void AFNDImprimeCadenaActual(FILE *fd, AFND *p_afnd);
AFND *AFNDInsertaLTransicion(AFND *p_afnd, char *nombre_estado_i, char *nombre_estado_f);
AFND *AFNDCierraLTransicion (AFND *p_afnd);
AFND *AFNDInicializaCadenaActual (AFND *p_afnd);
AFND *AFNDInicializaEstado (AFND *p_afnd);
void AFNDProcesaEntrada(FILE *fd, AFND *p_afnd);
void AFNDTransita(AFND *p_afnd);

#endif
