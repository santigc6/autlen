/******************************************************************************
* Autores: Aitor Arnaiz del Val y Santiago Gonzalez- Carvajal Centenera
* Grupo 1401. Pareja 9.
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "relaciones.h"
#include "afnd.h"

#define DEFAULT 1
#define MAX_NAME 256

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
        p_afnd->transitions[i] = NULL;
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

    for(j=0; j<p_afnd->current_est; j++){
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
        for(j=0; j<p_afnd->current_est; j++){
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

      for(j=0; j<p_afnd->current_est; j++){
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

AFND *AFND1ODeSimbolo(char *simbolo){
    AFND *p_a=NULL;
    
    if(!simbolo)
        return NULL;
        
    p_a = AFNDNuevo("afnd", 2, 1);
    if(!p_a)
        return NULL;

    if(!AFNDInsertaSimbolo(p_a, simbolo)){
        return NULL;
    }
    
    if(!AFNDInsertaEstado(p_a, "q0", INICIAL)){
        return NULL;
    }

    if(!AFNDInsertaEstado(p_a, "qf", FINAL)){
        return NULL;
    }

    if(!AFNDInsertaTransicion(p_a, "q0", simbolo, "qf")){
        return NULL;
    }

    return p_a;    
}

AFND *AFND1ODeLambda(){
    AFND *p_a=NULL;
    
    p_a = AFNDNuevo("afnd", 2, 1);
    if(!p_a)
        return NULL;

    if(!AFNDInsertaEstado(p_a, "q0", INICIAL)){
        return NULL;
    }

    if(!AFNDInsertaEstado(p_a, "qf", FINAL)){
        return NULL;
    }

    if(!AFNDInsertaLTransicion(p_a, "q0", "qf")){
        return NULL;
    }

    return p_a;
}

AFND *AFND1ODeVacio(){
    AFND *p_a=NULL;
    
    p_a = AFNDNuevo("afnd", 2, 1);
    if(!p_a)
        return NULL;

    if(!AFNDInsertaEstado(p_a, "q0", INICIAL)){
        return NULL;
    }

    if(!AFNDInsertaEstado(p_a, "qf", FINAL)){
        return NULL;
    }

    return p_a;
}

AFND *AFNDAAFND1O(AFND *p_afnd){
  int i, j;
  int n_ini, n_fin;
  int flag_ini_fin=0;
  int tipo;
  int *indexes_ini=NULL;
  int *indexes_fin=NULL;

  indexes_ini=(int *)malloc(sizeof(int)*p_afnd->n_est);
  if(!indexes_ini){
      return NULL;
  }

  indexes_fin=(int *)malloc(sizeof(int)*p_afnd->n_est);
  if(!indexes_fin){
      return NULL;
  }

  if(!p_afnd)
      return NULL;

  for(i=0, n_ini=0, n_fin=0; i<p_afnd->n_est; i++){ /* Los estados actuales son todos aquellos que sean iniciales o iniciales y finales */
    tipo = estado_get_tipo(p_afnd->estados[i]);
    if(tipo == INICIAL){
        indexes_ini[n_ini] = i;
        n_ini++;
    } else if(tipo == FINAL){
        indexes_fin[n_fin] = i;
        n_fin++;
    } else if(tipo == INICIAL_Y_FINAL){
        indexes_ini[n_ini] = i;
        indexes_ini[n_fin] = i;
        n_ini++;
        n_fin++;
        flag_ini_fin=1;
    }
  }

  if(n_ini == 1 && n_fin == 1 && !flag_ini_fin){
    return p_afnd;
  }

  for(i = 0; i < n_ini; i++){
    estado_set_tipo(p_afnd->estados[indexes_ini[i]], NORMAL);
  }

  for(i = 0; i < n_fin; i++){
    estado_set_tipo(p_afnd->estados[indexes_fin[i]], NORMAL);
  }

  /* Actualizamos el numero de estados */
  p_afnd->n_est += 2;

  /* Actualizamos la matriz de lambdas */
  p_afnd->lambda_trans = (int **)realloc(p_afnd->lambda_trans, p_afnd->n_est * sizeof(int *));
  if(!p_afnd->lambda_trans){
    return NULL;
  }

  for(i = 0; i < p_afnd->n_est; i++){
    if(i < p_afnd->n_est - 2){
      p_afnd->lambda_trans[i] = (int *)realloc(p_afnd->lambda_trans[i], p_afnd->n_est * sizeof(int));
    } else{
      p_afnd->lambda_trans[i] = (int *)malloc(p_afnd->n_est * sizeof(int));
    }
    if(!p_afnd->lambda_trans[i]){
      return NULL;
    }
    for(j = 0; j < p_afnd->n_est; j++){
      if(i >= p_afnd->n_est - 2){
        p_afnd->lambda_trans[i][j] = 0;
      } else{
        if(j >= p_afnd->n_est - 2){
          p_afnd->lambda_trans[i][j] = 0;
        }
      }
    }
  }

  if(!ampliar_lista_estados(&p_afnd->estados, p_afnd->n_est)){
    return NULL;
  }

  if(!AFNDInsertaEstado(p_afnd, "_i_1O", INICIAL) || !AFNDInsertaEstado(p_afnd, "_f_1O", FINAL)){
    return NULL;
  }

  for(i = 0; i < n_ini; i++){
    /* Transiciones lambda del inicial */
    p_afnd->lambda_trans[p_afnd->n_est - 2][indexes_ini[i]] = 1;
  }

  for(i = 0; i < n_fin; i++){
    /* Transiciones lambda al final */
    p_afnd->lambda_trans[indexes_fin[i]][p_afnd->n_est - 1] = 1;
  }

  free(indexes_ini);
  free(indexes_fin);

  return p_afnd;
}

AFND *AFND1OUne(AFND *p_afnd1O_1, AFND *p_afnd1O_2){
  int i, j;
  AFND *p_a=NULL;
  char *name_aux=NULL, *name_aux1=NULL;

  if(!p_afnd1O_1 || !p_afnd1O_2)
      return NULL;

  p_a = AFNDNuevo("AFND_U", p_afnd1O_1->n_est + p_afnd1O_2->n_est, p_afnd1O_1->n_simb + p_afnd1O_2->n_simb);
    if(!p_a)
      return NULL;

  name_aux=(char *)malloc(sizeof(char)* MAX_NAME);
  if(!name_aux)
    return NULL;

  name_aux1=(char *)malloc(sizeof(char)* MAX_NAME);
  if(!name_aux1)
    return NULL;

  /* Guardamos todos los estados como referencias */
  for(i = 0; i < p_afnd1O_1->n_est; i++){
    strcpy(name_aux, estado_get_name(p_afnd1O_1->estados[i]));
    strcat(name_aux, "_U1");
    AFNDInsertaEstado(p_a, name_aux, estado_get_tipo(p_afnd1O_1->estados[i]));
  }
  for(i = 0; i < p_afnd1O_2->n_est; i++){
    strcpy(name_aux, estado_get_name(p_afnd1O_2->estados[i]));
    strcat(name_aux, "_U2");
    AFNDInsertaEstado(p_a, name_aux, estado_get_tipo(p_afnd1O_2->estados[i]));
  }

  /* Insertamos los simbolos */
  for(i=0; i < p_afnd1O_1->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd1O_1->alfabeto, i));

  for(i=0; i < p_afnd1O_2->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd1O_2->alfabeto, i));

  /* Guardamos las referencias a las transiciones */
  for(i=0; i < p_afnd1O_1->n_trans; i++){
    for(j=0; j < p_afnd1O_1->transitions[i]->n_final; j++){
      strcpy(name_aux, estado_get_name(p_afnd1O_1->transitions[i]->inicial));
      strcat(name_aux, "_U1");
      strcpy(name_aux1, estado_get_name(p_afnd1O_1->transitions[i]->final[j]));
      strcat(name_aux1, "_U1");
      AFNDInsertaTransicion(p_a, name_aux, p_afnd1O_1->transitions[i]->trans_symbol, name_aux1);
    }
  }

  for(i=0; i < p_afnd1O_2->n_trans; i++){
    for(j=0; j < p_afnd1O_2->transitions[i]->n_final; j++){
      strcpy(name_aux, estado_get_name(p_afnd1O_2->transitions[i]->inicial));
      strcat(name_aux, "_U2");
      strcpy(name_aux1, estado_get_name(p_afnd1O_2->transitions[i]->final[j]));
      strcat(name_aux1, "_U2");
      AFNDInsertaTransicion(p_a, name_aux, p_afnd1O_2->transitions[i]->trans_symbol, name_aux1);
    }
  }

  /* Lambda transitions */
  for(i = 0; i < p_afnd1O_1->n_est; i++){
    for(j = 0; j < p_afnd1O_1->n_est; j++){
      p_a->lambda_trans[i][j]=p_afnd1O_1->lambda_trans[i][j];
    }
  }

  for(i = 0; i < p_afnd1O_2->n_est; i++){
    for(j = 0; j < p_afnd1O_2->n_est; j++){
      p_a->lambda_trans[i + p_afnd1O_1->n_est][j + p_afnd1O_1->n_est]=p_afnd1O_2->lambda_trans[i][j];
    }
  }

  free(name_aux);
  free(name_aux1);

  return AFNDAAFND1O(p_a);
}

AFND *AFND1OConcatena(AFND *p_afnd_origen1, AFND *p_afnd_origen2){
  int i, j;
  AFND *p_a=NULL;
  int ini1, ini2;
  int fin1, fin2;
  char *name_aux=NULL, *name_aux1=NULL;

  /* + 2 por el estado inicial y final que se van a aniadir */
  p_a = AFNDNuevo("AFND_C", p_afnd_origen1->n_est + p_afnd_origen2->n_est + 2, p_afnd_origen1->n_simb + p_afnd_origen2->n_simb);
  if(!p_a)
    return NULL;

  name_aux=(char *)malloc(sizeof(char)* MAX_NAME);
  if(!name_aux)
    return NULL;

  name_aux1=(char *)malloc(sizeof(char)* MAX_NAME);
  if(!name_aux1)
    return NULL;

  /* Guardamos todos los estados como referencias */
  for(i = 0; i < p_afnd_origen1->n_est; i++){
    if(estado_get_tipo(p_afnd_origen1->estados[i]) == INICIAL){
      ini1=p_a->current_est;
    } else if(estado_get_tipo(p_afnd_origen1->estados[i]) == FINAL){
      fin1=p_a->current_est;
    }
    strcpy(name_aux, estado_get_name(p_afnd_origen1->estados[i]));
    strcat(name_aux, "_C1");
    AFNDInsertaEstado(p_a, name_aux, NORMAL);
  }
  for(i = 0; i < p_afnd_origen2->n_est; i++){
    if(estado_get_tipo(p_afnd_origen2->estados[i]) == INICIAL){
      ini2=p_a->current_est;
    } else if(estado_get_tipo(p_afnd_origen2->estados[i]) == FINAL){
      fin2=p_a->current_est;
    }
    strcpy(name_aux, estado_get_name(p_afnd_origen2->estados[i]));
    strcat(name_aux, "_C2");
    AFNDInsertaEstado(p_a, name_aux, NORMAL);
  }

  /* Insertamos los simbolos */
  for(i=0; i < p_afnd_origen1->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd_origen1->alfabeto, i));

  for(i=0; i < p_afnd_origen2->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd_origen2->alfabeto, i));

  /* Guardamos las referencias a las transiciones */
  for(i=0; i < p_afnd_origen1->n_trans; i++){
    for(j=0; j < p_afnd_origen1->transitions[i]->n_final; j++){
      strcpy(name_aux, estado_get_name(p_afnd_origen1->transitions[i]->inicial));
      strcat(name_aux, "_C1");
      strcpy(name_aux1, estado_get_name(p_afnd_origen1->transitions[i]->final[j]));
      strcat(name_aux1, "_C1");
      AFNDInsertaTransicion(p_a, name_aux, p_afnd_origen1->transitions[i]->trans_symbol, name_aux1);
    }
  }

  for(i=0; i < p_afnd_origen2->n_trans; i++){
    for(j=0; j < p_afnd_origen2->transitions[i]->n_final; j++){
      strcpy(name_aux, estado_get_name(p_afnd_origen2->transitions[i]->inicial));
      strcat(name_aux, "_C2");
      strcpy(name_aux1, estado_get_name(p_afnd_origen2->transitions[i]->final[j]));
      strcat(name_aux1, "_C2");
      AFNDInsertaTransicion(p_a, name_aux, p_afnd_origen2->transitions[i]->trans_symbol, name_aux1);
    }
  }

  /* Lambda transitions */
  for(i = 0; i < p_afnd_origen1->n_est; i++){
    for(j = 0; j < p_afnd_origen1->n_est; j++){
      p_a->lambda_trans[i][j]=p_afnd_origen1->lambda_trans[i][j];
    }
  }

  for(i = 0; i < p_afnd_origen2->n_est; i++){
    for(j = 0; j < p_afnd_origen2->n_est; j++){
      p_a->lambda_trans[i + p_afnd_origen1->n_est][j + p_afnd_origen1->n_est]=p_afnd_origen2->lambda_trans[i][j];
    }
  }

  /* Nuevos estados inicial y final y transiciones lambda que concatenan */
  if(!AFNDInsertaEstado(p_a, "_i_1O", INICIAL) || !AFNDInsertaEstado(p_a, "_f_1O", FINAL)){
    return NULL;
  }
  p_a->lambda_trans[p_a->n_est - 2][ini1]=1;
  p_a->lambda_trans[fin1][ini2]=1;
  p_a->lambda_trans[fin2][p_a->n_est - 1]=1;

  free(name_aux);
  free(name_aux1);

  return p_a;
}

AFND *AFND1OEstrella(AFND *p_afnd_origen){
  AFND *p_a = NULL;
  int i, j;
  int ini;
  int fin;
  char *name_aux=NULL, *name_aux1=NULL;

  if(!p_afnd_origen)
    return NULL;

  p_a = AFNDNuevo("AFND_X", p_afnd_origen->n_est + 2, p_afnd_origen->n_simb);
  if(!p_a)
    return NULL;

  name_aux=(char *)malloc(sizeof(char)* MAX_NAME);
  if(!name_aux)
    return NULL;

  name_aux1=(char *)malloc(sizeof(char)* MAX_NAME);
  if(!name_aux1)
    return NULL;

  /* Buscamos los estados inicial y final */
  for(i = 0; i < p_afnd_origen->n_est; i++){
    if(estado_get_tipo(p_afnd_origen->estados[i]) == INICIAL){
      ini=p_a->current_est;
    } else if(estado_get_tipo(p_afnd_origen->estados[i]) == FINAL){
      fin=p_a->current_est;
    }
    strcpy(name_aux, estado_get_name(p_afnd_origen->estados[i]));
    strcat(name_aux, "_X");
    AFNDInsertaEstado(p_a, name_aux, NORMAL);
  }

   /* Insertamos los simbolos */
  for(i=0; i < p_afnd_origen->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd_origen->alfabeto, i));

  /* Guardamos las referencias a las transiciones */
  for(i=0; i < p_afnd_origen->n_trans; i++){
    for(j=0; j < p_afnd_origen->transitions[i]->n_final; j++){
      strcpy(name_aux, estado_get_name(p_afnd_origen->transitions[i]->inicial));
      strcat(name_aux, "_X");
      strcpy(name_aux1, estado_get_name(p_afnd_origen->transitions[i]->final[j]));
      strcat(name_aux1, "_X");
      AFNDInsertaTransicion(p_a, name_aux, p_afnd_origen->transitions[i]->trans_symbol, name_aux1);
    }
  }

  /* Lambda transitions */
  for(i = 0; i < p_afnd_origen->n_est; i++){
    for(j = 0; j < p_afnd_origen->n_est; j++){
      p_a->lambda_trans[i][j]=p_afnd_origen->lambda_trans[i][j];
    }
  }

  /* Actualizamos la matriz de lambdas */
  if(!AFNDInsertaEstado(p_a, "_i_1O", INICIAL) || !AFNDInsertaEstado(p_a, "_f_1O", FINAL)){
    return NULL;
  }

  p_a->lambda_trans[p_a->n_est - 2][ini]=1;
  p_a->lambda_trans[fin][p_a->n_est - 1]=1;
  p_a->lambda_trans[p_a->n_est - 2][p_a->n_est - 1]=1;
  p_a->lambda_trans[p_a->n_est - 1][p_a->n_est - 2]=1;

  free(name_aux);
  free(name_aux1);

  return p_a;
}

void AFNDADot(AFND * p_afnd){
  FILE *f=NULL;
  int i, j, k, l;
  int tipo, index_ini;
  char *name=NULL, *symb=NULL;
  int flag=ERROR;

  if(!p_afnd)
      return;

  f = fopen("automata.dot", "w+");
  if(!f){
      return;
  }

  fprintf(f, "digraph %s { rankdir=LR;\n\t_invisible [style=\"invis\"];\n", p_afnd->name);
  for(i = 0; i < p_afnd->n_est; i++){
      tipo = estado_get_tipo(p_afnd->estados[i]);
      if(tipo == FINAL){
          fprintf(f, "\t%s [penwidth=\"2\"];\n", estado_get_name(p_afnd->estados[i]));
      } else if (tipo == INICIAL){
          index_ini = i;
          fprintf(f, "\t%s;\n", estado_get_name(p_afnd->estados[i]));
      } else{
          fprintf(f, "\t%s;\n", estado_get_name(p_afnd->estados[i]));
      }
  }
  fprintf(f, "\t_invisible -> %s;\n" , estado_get_name(p_afnd->estados[index_ini]));

  for(j=0; j<p_afnd->n_est; j++){ /* For each state */
    name=estado_get_name(p_afnd->estados[j]);
    for(k=0; k<get_size_alfabeto(p_afnd->alfabeto); k++){ /* For each symbol in the alphabet */
      symb=get_palabra_by_index(p_afnd->alfabeto, k);
      for(flag=ERROR, i=0; i<p_afnd->n_trans; i++){
        if(transitions_equal(p_afnd->transitions[i], name, symb) == OK){ /* We try to find a transition */
          for(l = 0; l < p_afnd->transitions[i]->n_final; l++){
            fprintf(f, "\t%s -> ", name);
            fprintf(f, "%s [label=\"%s\"];\n", estado_get_name(p_afnd->transitions[i]->final[l]) , p_afnd->transitions[i]->trans_symbol);

            flag=OK;
          }
        }
      }
      if(flag != OK){ /* If we havent found a transition, the transition is empty */
        continue;
      }
    }
  }

  for(i = 0; i < p_afnd->n_est; i++){
    for(j = 0; j < p_afnd->n_est; j++){
      if(p_afnd->lambda_trans[i][j] == 1 && i != j){
        fprintf(f, "\t%s -> %s [label=\"&lambda;\"];\n", estado_get_name(p_afnd->estados[i]), estado_get_name(p_afnd->estados[j]));
      }
    }
  }

  fprintf(f, "}\n");
  fclose(f);
}
