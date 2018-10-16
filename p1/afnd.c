#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "estado.h"
#include "alfabeto.h"
#include "palabra.h"

/* Estructura para las transiciones (privada) */
typedef struct _Transiciones{
	Estado *inicial;
	Estado **final;
  int n_final;
	char *trans_symbol;
} Transicion;

struct _AFND{
	char *name;
	Alfabeto *alfabeto;
  Palabra *word;
	int n_simb;
	int n_est;
  int n_trans;
	Estado **estados;
	Transicion **transitions;
}

AFND * AFNDNuevo(char* nombre, int num_estados, int num_simbolos){
  AFND *a=NULL;
  
  if(!nombre || num_estados <= 0 || num_simbolos <= 0)
    return NULL;
    
  a=(AFND *)malloc(sizeof(AFND));
  if(!a)
    return NULL;
    
  a->name=(char *)malloc(sizeof(char)*(strlen(nombre)+1));
  if(!a->name){
    free(a);
    
    return NULL;
  }
  strcpy(a->name, nombre);
  
  a->alfabeto=crear_alfabeto(num_simbolos);
  if(!a->alfabeto){
    free(a->name);
    free(a);
    
    return NULL;
  }
  
  a->n_est=num_estados;
  a->transitions=NULL;
  a->n_trans=0;

  a->word=create_word(size);
  if(!a->word){
    free(a->name);
    destruye_alfabeto(a->alfabeto);
    free(a);
  }

  a->estados=incicializar_lista_estados(num_estados);
  if(!a->estados){
    free(a->name);
    destruye_alfabeto(a->alfabeto);
    destroy_word(a->word);
    free(a);
  }
  
  return a;
}

void AFNDElimina(AFND * p_afnd){
  int i;
  
  if(!p_afnd)
    return;
  
  if(p_afnd->estados){  
    for(i=0; i < p_afnd->n_est; i++){
      destruir_estado(p_afnd->estados[i]);
    }
    destruir_lista_estados(p_afnd->estados);
  }

  if(p_afnd->alfabeto)
    destruye_alfabeto(p_afnd->alfabeto);

  if(p_afnd->transitions){
    for(i=0; i<p_afnd->n_trans; i++){
      if(p_afnd->transitions[i])
        free(p_afnd->transitions[i]);
    }
    free(p_afnd->transitions);
  }

  if(p->word)
    destroy_word(p->word);

  free(p_afnd->name);
}

void AFNDImprime(FILE * fd, AFND* p_afnd){
  int i;

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "%s={\n", p_afnd->name);
  fprintf(fd, "\tnum_simbolos = %d\n\n", p_afnd->n_simb);
  print_alfabeto(fd, p_afnd->alfabeto);
  fprintf(fd, "\tnum_estados = %d\n\n", p_afnd->n_est);
  print_estados(fd, p_afnd->estados, 0, p_afnd->n_est);
  fprintf(fd, "Funcion de Transicion = {\n\n");

  for(i=0; i<n_trans; i++){
    fprintf(fd, "\t\tf(", );
    print_estado(fd, p_afnd->transitions[i]->inicial);
    fprintf(fd, ",%s)={ ", p_afnd->transitions[i]->trans_symbol);
    print_estados(fd, p_afnd->transitions[i]->final, 1, n_final);
  }

  fprintf(fd, "\t}\n}\n\n");

}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){
  
}


AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);

AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ){
	Transicion *t=NULL;

	if(!p_afnd || !nombre_estado_i || !nombre_simbolo_entrada || !nombre_estado_f){
		return NULL;
	}

	t=(Transicion *)malloc(sizeof(Transicion));
	if(!t)
		return NULL;

	/*t->inicial=*/
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);
AFND * AFNDInicializaEstado (AFND * p_afnd);
void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);
void AFNDTransita(AFND * p_afnd);
