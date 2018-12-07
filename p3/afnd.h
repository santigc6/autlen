/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#ifndef AFND_H
#define AFND_H

#include "estado.h"
#include "alfabeto.h"
#include "palabra.h"

/* Estructura para las transiciones */
typedef struct _Transiciones{
	Estado *inicial; /* Estado inicial */
	Estado **final; /* Conjunto de estados finales */
  	int n_final; /* Numero de estados finales */
	char *trans_symbol; /* Simbolo de la transicion */
} Transicion;

typedef struct _AFND{ /* Estructura principal AFND */
	char *name; /* Nombre del AFND */
	int **lambda_trans; /* Transiciones lambda del AFND */
	Alfabeto *alfabeto; /* Alfabeto del AFND */
	Palabra *word;/* Palabra a procesar por el AFND */
	int n_simb; /* Numero de simbolos del AFND */
	int n_est; /* Numero de estados del AFND */
	int current_est; /* Numero de estados actualmente aniadidos al AFND */
	int n_trans; /* Numero de transiciones */
	int n_act; /* Numero de estados actuales */
	Estado **estados; /* Conjunto de estados del AFND */
	Estado **actuales; /* Conjunto de estados actuales del AFND */
	Transicion **transitions; /* Transiciones del AFND */
} AFND;

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
AFND *AFND1ODeSimbolo(char *simbolo);
AFND *AFND1ODeLambda();
AFND *AFND1ODeVacio();
AFND *AFNDAAFND1O(AFND *p_afnd);
AFND *AFND1OUne(AFND *p_afnd1O_1, AFND *p_afnd1O_2);
AFND *AFND1OConcatena(AFND *p_afnd_origen1, AFND *p_afnd_origen2);
AFND *AFND1OEstrella(AFND *p_afnd_origen);
void AFNDADot(AFND * p_afnd);


#endif
