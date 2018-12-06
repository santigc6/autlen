/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "estado.h"
#include "alfabeto.h"
#include "palabra.h"
#include "relaciones.h"
#include "afnd.h"

#define DEFAULT 1

/* Estructura para las transiciones (privada) */
typedef struct _Transiciones{
	Estado *inicial; /* Estado inicial */
	Estado **final; /* Conjunto de estados finales */
  int n_final; /* Numero de estados finales */
	char *trans_symbol; /* Simbolo de la transicion */
} Transicion;

/* Private function */
/****************************************************************************************
* Description: OK   : las dos transiciones tienen el mismo estado inicial y el mismo simbolo de transicion
*              ERROR: en caso contrario
****************************************************************************************/
int transitions_equal(Transicion *t, char *name_ini, char *trans_symbol){
  if(!t || !name_ini || !trans_symbol)
    return ERROR;

  if(strcmp(t->trans_symbol, trans_symbol) == 0){
    if(strcmp(estado_get_name(t->inicial), name_ini) == 0){
      return OK;
    }

    return ERROR; /* No es necesario */
  }

  return ERROR;
}

/* Private function */
/****************************************************************************************
* Description: OK   : el estado ya esta en actuales
*              ERROR: en caso contrario
****************************************************************************************/
int is_in_actuales(AFND *pa, int index, int len){
  int i;

  if(!pa || index < 0 || len < 0)
    return -1;

  for(i = 0; i < len; i++){
    if(!strcmp(estado_get_name(pa->actuales[i]), estado_get_name(pa->estados[index]))){ /* El estado ya esta en la lista */
      return OK;
    }
  }

  return ERROR;
}

AFND *AFNDNuevo(char* nombre, int num_estados, int num_simbolos){
  AFND *a=NULL;
  int i, j;

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
  a->actuales = NULL;
  a->n_act=0;
  a->n_simb=num_simbolos;

  /* Default word size, it doesnt matter if we exceed it because, in that case, we would use realloc */
  a->word=create_word(DEFAULT);
  if(!a->word){
    free(a->name);
    destruye_alfabeto(a->alfabeto);
    free(a);

    return NULL;
  }

  a->estados=inicializar_lista_estados(num_estados);
  if(!a->estados){
    free(a->name);
    destruye_alfabeto(a->alfabeto);
    destroy_word(a->word);
    free(a);

    return NULL;
  }

  a->lambda_trans = (int **)malloc(sizeof(int *)*num_estados);
  if(!a->lambda_trans){
    free(a->name);
    destruye_alfabeto(a->alfabeto);
    destroy_word(a->word);
    free(a->estados);
    free(a);

    return NULL;
  }

  for(i = 0; i < num_estados; i++){
    a->lambda_trans[i]=(int *)malloc(sizeof(int)*num_estados);
    if(!a->lambda_trans[i]){
      for(j=i-1; j >= 0; j--){
        free(a->lambda_trans[j]);
      }
      free(a->name);
      destruye_alfabeto(a->alfabeto);
      destroy_word(a->word);
      free(a->estados);
      free(a->lambda_trans);
      free(a);

      return NULL;
    } else{
      for(j=0; j<num_estados; j++){
        a->lambda_trans[i][j]=0; /* Inicializamos toda la matriz a cero */
      }
    }
  }

  a->current_est=0;

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
      if(p_afnd->transitions[i]){
        free(p_afnd->transitions[i]->trans_symbol);
        destruir_lista_estados(p_afnd->transitions[i]->final);
        free(p_afnd->transitions[i]);
      }
    }
    free(p_afnd->transitions);
  }

  if(p_afnd->word)
    destroy_word(p_afnd->word);

  for(i=0; i<p_afnd->n_est; i++){
    free(p_afnd->lambda_trans[i]);
  }
  free(p_afnd->lambda_trans);

  free(p_afnd->name);
  free(p_afnd);
}

void AFNDImprime(FILE * fd, AFND* p_afnd){
  int i, j, k;
  int flag=ERROR;
  char *name=NULL, *symb=NULL;

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "%s={\n", p_afnd->name);
  fprintf(fd, "\tnum_simbolos = %d\n\n\t", p_afnd->n_simb);
  print_alfabeto(fd, p_afnd->alfabeto);
  fprintf(fd, "\tnum_estados = %d\n\n\t", p_afnd->n_est);
  print_estados(fd, p_afnd->estados, 0, p_afnd->n_est);

  fprintf(fd, "\tRL++*={\n");
  for(i = 0; i <= p_afnd->n_est; i++){
    if(i != 0){
      fprintf(fd, "\t\t[%d]\t", i-1);
    } else{
      fprintf(fd, "\t\t\t");
    }
    for(j = 0; j < p_afnd->n_est; j++){
      if(i != 0){
        fprintf(fd, "%d\t", p_afnd->lambda_trans[i-1][j]);
      } else{
        fprintf(fd, "[%d]\t", j);
      }
    }
    fprintf(fd, "\n");
  }

  fprintf(fd, "\t}\n\n");
  fprintf(fd, "Funcion de Transicion = {\n\n");

  for(j=0; j<p_afnd->n_est; j++){ /* For each state */
    name=estado_get_name(p_afnd->estados[j]);
    for(k=0; k<get_size_alfabeto(p_afnd->alfabeto); k++){ /* For each symbol in the alphabet */
      symb=get_palabra_by_index(p_afnd->alfabeto, k);
      for(flag=ERROR, i=0; i<p_afnd->n_trans; i++){
        if(transitions_equal(p_afnd->transitions[i], name, symb) == OK){ /* We try to find a transition */
          fprintf(fd, "\t\tf(");
          print_estado(fd, p_afnd->transitions[i]->inicial);
          fprintf(fd, ",%s)", p_afnd->transitions[i]->trans_symbol);
          print_estados(fd, p_afnd->transitions[i]->final, 1, p_afnd->transitions[i]->n_final);
          flag=OK;
        }
      }
      if(flag != OK){ /* If we havent found a transition, the transition is empty */
        fprintf(fd, "\t\tf(");
        print_estado(fd, p_afnd->estados[j]);
        fprintf(fd, ",%s)={ }\n", symb);
      }
    }
  }

  fprintf(fd, "\t}\n}\n\n");

}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){
  if(!p_afnd || !simbolo)
    return NULL;

  if(alfabeto_aniade_palabra(p_afnd->alfabeto, simbolo) == ERROR)
    return NULL;

  return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){
  if(!p_afnd || !nombre || (tipo != INICIAL && tipo != NORMAL && tipo !=FINAL && tipo != INICIAL_Y_FINAL))
    return NULL;

  p_afnd->estados[p_afnd->current_est]= crear_estado(nombre, tipo);
  if(!p_afnd->estados[p_afnd->current_est])
    return NULL;

  p_afnd->current_est++;

  return p_afnd;
}

AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ){
  int i;
  int j;
  int flag_add = ERROR;

	if(!p_afnd || !nombre_estado_i || !nombre_simbolo_entrada || !nombre_estado_f){
		return NULL;
	}

  if(!p_afnd->transitions){ /* Si la lista de transiciones no esta inicializada la inicializamos */
    p_afnd->transitions=(Transicion **)malloc(sizeof(Transicion *));
    if(!p_afnd->transitions)
      return NULL;

    /* Creamos una nueva transicion y guardamos los datos */
    p_afnd->n_trans++;
    p_afnd->transitions[0]=(Transicion *)malloc(sizeof(Transicion));
    if(!p_afnd->transitions[0]){
      free(p_afnd->transitions);

      return NULL;
    }
    p_afnd->transitions[0]->n_final=0;
    p_afnd->transitions[0]->final=inicializar_lista_estados(p_afnd->n_est);
    if(!p_afnd->transitions[0]->final)
      return NULL;

    p_afnd->transitions[0]->trans_symbol=(char *)malloc((strlen(nombre_simbolo_entrada) + 1) * sizeof(char));
    if(!p_afnd->transitions[0]->trans_symbol){
      free(p_afnd->transitions[0]);

      return NULL;
    }
    strcpy(p_afnd->transitions[0]->trans_symbol, nombre_simbolo_entrada);

    for(j=0; j<p_afnd->n_est; j++){
      if(strcmp(estado_get_name(p_afnd->estados[j]), nombre_estado_i) == 0){
        p_afnd->transitions[0]->inicial=p_afnd->estados[j];
      }
      if(strcmp(estado_get_name(p_afnd->estados[j]), nombre_estado_f) == 0){
        p_afnd->transitions[0]->final[0]=p_afnd->estados[j];
        p_afnd->transitions[0]->n_final++;
      }
    }
  } else{ /* Si la lista ya estaba inicializada */
    for(i=0; i<p_afnd->n_trans; i++){
      if(transitions_equal(p_afnd->transitions[i], nombre_estado_i, nombre_simbolo_entrada) == OK){ /* Buscamos si hay alguna transicion con la misma entrada en el mismo estado para aniadirselo a su lista de finales */
        for(j=0; j<p_afnd->n_est; j++){
          if(strcmp(estado_get_name(p_afnd->estados[j]), nombre_estado_f) == 0){
            p_afnd->transitions[i]->final[p_afnd->transitions[i]->n_final]=p_afnd->estados[j];
            p_afnd->transitions[i]->n_final++;
            flag_add=OK;
          }
        }
      }
    }
    if(flag_add != OK){ /* No se ha encontrado una transicion con el mismo estado inicial y simbolo de transicion */
      /* Entonces creamos otra transicion */
      p_afnd->n_trans++;
    	p_afnd->transitions=(Transicion **)realloc(p_afnd->transitions, sizeof(Transicion *) * p_afnd->n_trans);
    	if(!p_afnd->transitions)
    		return NULL;

      p_afnd->transitions[p_afnd->n_trans - 1]=(Transicion *)malloc(sizeof(Transicion));
      if(!p_afnd->transitions[p_afnd->n_trans - 1]){
        perror("Allocating transtion");

        return NULL;
      }
      p_afnd->transitions[p_afnd->n_trans - 1]->n_final=0;
      p_afnd->transitions[p_afnd->n_trans-1]->trans_symbol=(char *)malloc((strlen(nombre_simbolo_entrada) + 1) * sizeof(char));
      if(!p_afnd->transitions[p_afnd->n_trans-1]->trans_symbol){
        free(p_afnd->transitions[p_afnd->n_trans-1]);

        return NULL;
      }
      strcpy(p_afnd->transitions[p_afnd->n_trans-1]->trans_symbol, nombre_simbolo_entrada);

      p_afnd->transitions[p_afnd->n_trans-1]->final=inicializar_lista_estados(p_afnd->n_est);
      if(!p_afnd->transitions[p_afnd->n_trans-1]->final)
        return NULL;

      for(j=0; j<p_afnd->n_est; j++){
        if(strcmp(estado_get_name(p_afnd->estados[j]), nombre_estado_i) == 0){
          p_afnd->transitions[p_afnd->n_trans-1]->inicial=p_afnd->estados[j];
        }
        if(strcmp(estado_get_name(p_afnd->estados[j]), nombre_estado_f) == 0){
          p_afnd->transitions[p_afnd->n_trans-1]->final[0]=p_afnd->estados[j];
          p_afnd->transitions[p_afnd->n_trans-1]->n_final++;
        }
      }
    }
  }

  return p_afnd;
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){
  if(!p_afnd || !letra)
    return NULL;

  if(add_symbol(p_afnd->word, letra) == ERROR)
    return NULL;

  return p_afnd;
}

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){
  if(!fd | !p_afnd)
    return;

  fprintf(fd, "ACTUALMENTE EN ");
  print_estados(fd, p_afnd->actuales, 2, p_afnd->n_act);
}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){
  int i;
  int size, actual;

  if(!fd | !p_afnd)
    return;

  size=get_word_size(p_afnd->word);
  actual=get_process(p_afnd->word);

  fprintf(fd, "[(%d) ", size-actual);
  for(i=actual; i<size; i++){
    fprintf(fd, "%s ", get_symbol_by_index(p_afnd->word, i));
  }
  fprintf(fd, "]\n\n");

}

AFND *AFNDInsertaLTransicion(AFND *p_afnd, char *nombre_estado_i, char *nombre_estado_f){
  int index_i, index_f;

  if (!p_afnd || !nombre_estado_i || !nombre_estado_f)
    return NULL;

  if((index_i = estado_get_index(p_afnd->estados, p_afnd->n_est, nombre_estado_i)) < 0){
    return NULL;
  }
  if((index_f = estado_get_index(p_afnd->estados, p_afnd->n_est, nombre_estado_f)) < 0){
    return NULL;
  }

  p_afnd->lambda_trans[index_i][index_f]=1;

  return p_afnd;
}

AFND *AFNDCierraLTransicion (AFND *p_afnd){
  if (!p_afnd)
    return NULL;

  cierre_reflexivo(p_afnd->lambda_trans, p_afnd->n_est);
  cierre_transitivo(p_afnd->lambda_trans, p_afnd->n_est);

  return p_afnd;
}

AFND *AFNDInicializaCadenaActual (AFND *p_afnd){
  if (!p_afnd)
    return NULL;

  reset_word(p_afnd->word);

  return p_afnd;
}

AFND * AFNDInicializaEstado (AFND * p_afnd){
  int i, j=0, k;

  if(!p_afnd)
    return NULL;

  p_afnd->actuales=inicializar_lista_estados(p_afnd->n_est);
  if(!p_afnd->actuales)
    return NULL;

  for(i=0; i<p_afnd->n_est; i++){ /* Los estados actuales son todos aquellos que sean iniciales o iniciales y finales */
    if(estado_get_tipo(p_afnd->estados[i]) == INICIAL || estado_get_tipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
      p_afnd->actuales[j]=p_afnd->estados[i];
      j++;
      p_afnd->n_act++;

      for(k=0; k<p_afnd->n_est; k++){ /* Buscamos todas las posibles transiciones lambda */
        if(k != i && p_afnd->lambda_trans[i][k] == 1 && is_in_actuales(p_afnd, k, p_afnd->n_act) == ERROR){
          p_afnd->actuales[j]=p_afnd->estados[k];
          j++;
          p_afnd->n_act++;
        }
      }
    }
  }

  return p_afnd;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){
  int actual, size;

  if(!fd || !p_afnd)
    return;

  size=get_word_size(p_afnd->word);
  actual=get_process(p_afnd->word);
  while(actual<=size){ /* Mientras que quede palabra por procesar vamos procesando simbolo a simbolo */
    AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
    AFNDImprimeCadenaActual(fd, p_afnd);

    if(p_afnd->n_act == 0)
      break;

    AFNDTransita(p_afnd);

    word_next(p_afnd->word);
    size=get_word_size(p_afnd->word);
    actual=get_process(p_afnd->word);
  }

  /* Destruimos la lista de estados actuales y reiniciamos la palabra */
  destruir_lista_estados(p_afnd->actuales);
}

void AFNDTransita(AFND * p_afnd){
  int i, j, k, l;
  int index, index_final;
  int last;
  int actual;
  char *symb=NULL, *name=NULL;
  Estado **aux=NULL;

  if(!p_afnd)
    return;

  /* Lista auxiliar para guardar una copia de los estados actuales que tenenmos que procesar */
  aux=inicializar_lista_estados(p_afnd->n_act);
  if(!aux)
    return;

  /* We keep the references */
  last=p_afnd->n_act;
  for(i=0; i<p_afnd->n_act; i++){
    aux[i]=p_afnd->actuales[i];
  }

  actual=get_process(p_afnd->word);
  symb=get_symbol_by_index(p_afnd->word, actual);

  k=0;
  for(i=0; i<last; i++){ /* For each actual state */
    name=estado_get_name(aux[i]);
    for(j=0; j<p_afnd->n_trans; j++){ /* We find all the transitions for the given state and symbol */
      if(transitions_equal(p_afnd->transitions[j], name, symb) == OK){
        for(index=0; index<p_afnd->transitions[j]->n_final; index++){ /* We add all the final states of the founded transition */
          p_afnd->actuales[k]=p_afnd->transitions[j]->final[index];
          k++;
          index_final = estado_get_index(p_afnd->estados, p_afnd->n_est, estado_get_name(p_afnd->transitions[j]->final[index])); /* We get the name of the final state we are proccessing */
          for(l=0; l<p_afnd->n_est; l++){ /* Buscamos todas las posibles transiciones lambda */
            if(l != index_final && p_afnd->lambda_trans[index_final][l] == 1 && is_in_actuales(p_afnd, l, k) == ERROR){
              p_afnd->actuales[k]=p_afnd->estados[l];
              k++;
            }
          }
        }
      }
    }
  }
  p_afnd->n_act=k; /* Actualizamos el numero de estados actuales */
  destruir_lista_estados(aux); /* Destruimos la lista auxiliar */
}
