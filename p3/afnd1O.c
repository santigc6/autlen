#include <stdio.h>
#include <stdlib.h>

#include "afnd1O.h"
#include "estado.h"

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
  	p_afnd->lambda_trans[i] = (int *)realloc(p_afnd->lambda_trans[i], p_afnd->n_est * sizeof(int));
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

  if(!ampliar_lista_estados(p_afnd->estados, p_afnd->n_est)){
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
  free(indexes_ini);

  return p_afnd;
}

AFND *AFND1OUne(AFND *p_afnd1O_1, AFND *p_afnd1O_2){
  int i, j;
  AFND *p_a=NULL;
  int ntrans;

  if(!p_afnd1O_1 || !p_afnd1O_2)
      return NULL;

  p_a = AFNDNuevo("AFND_U", p_afnd1O_1->n_est + p_afnd1O_2->n_est, p_afnd1O_1->n_simb + p_afnd1O_2->n_simb);
    if(!p_a)
      return NULL;

  /* Guardamos todos los estados como referencias */
  for(i = 0; i < p_afnd1O_1->n_est; i++){
    AFNDInsertaEstado(p_a, strcat(estado_get_name(p_afnd1O_1->estados[i]), "_U1"), estado_get_tipo(p_afnd1O_1->estados[i]));
  }
  for(i = 0; i < p_afnd1O_2->n_est; i++){
    AFNDInsertaEstado(p_a, strcat(estado_get_name(p_afnd1O_2->estados[i]), "_U2"), estado_get_tipo(p_afnd1O_2->estados[i]));
  }

  /* Insertamos los simbolos */
  for(i=0; i < p_afnd1O_1->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd1O_1->alfabeto, i));

  for(i=0; i < p_afnd1O_2->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd1O_2->alfabeto, i));

  /* Guardamos las referencias a las transiciones */
  for(i=0; i < p_afnd1O_1->n_trans; i++){
    for(j=0; j < p_afnd1O_1->transitions[i]){
      AFNDInsertaTransicion(p_a, strcat(estado_get_name(p_afnd1O_1->transitions[i]->inicial), "_U1"), p_afnd1O_1->transitions[i]->trans_symbol, strcat(estado_get_name(p_afnd1O_1->transitions[i]->final[j]), "_U1"));
    }
  }

  for(i=0; i < p_afnd1O_2->n_trans; i++){
    for(j=0; j < p_afnd1O_2->transitions[i]){
      AFNDInsertaTransicion(p_a, strcat(estado_get_name(p_afnd1O_2->transitions[i]->inicial), "_U2"), p_afnd1O_2->transitions[i]->trans_symbol, strcat(estado_get_name(p_afnd1O_2->transitions[i]->final[j]), "_U2"));
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

  return AFNDAAFND1O(p_a);
}

AFND *AFND1OConcatena(AFND *p_afnd_origen1, AFND *p_afnd_origen2){
  int i, j;
  AFND *p_a=NULL;
  int ntrans;
  int ini1, ini2;
  int fin1, fin2;

  /* + 2 por el estado inicial y final que se van a aniadir */
  p_a = AFNDNuevo("AFND_C", p_afnd_origen1->n_est + p_afnd_origen2->n_est + 2, p_afnd_origen1->n_simb + p_afnd_origen2->n_simb);
  if(!p_a)
    return NULL;

  /* Guardamos todos los estados como referencias */
  for(i = 0; i < p_afnd_origen1->n_est; i++){
    if(estado_get_tipo(p_afnd_origen1->estados[i]) == INICIAL){
      ini1=p_a->current_est;
    } else if(estado_get_tipo(p_afnd_origen1->estados[i]) == FINAL){
      fin1=p_a->current_est;
    }
    AFNDInsertaEstado(p_a, strcat(estado_get_name(p_afnd_origen1->estados[i]), "_C1"), NORMAL);
  }
  for(i = 0; i < p_afnd_origen2->n_est; i++){
    if(estado_get_tipo(p_afnd_origen2->estados[i]) == INICIAL){
      ini2=p_a->current_est;
    } else if(estado_get_tipo(p_afnd_origen2->estados[i]) == FINAL){
      fin2=p_a->current_est;
    }
    AFNDInsertaEstado(p_a, strcat(estado_get_name(p_afnd_origen2->estados[i]), "_C2"), NORMAL);
  }

  /* Insertamos los simbolos */
  for(i=0; i < p_afnd_origen1->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd_origen1->alfabeto, i));

  for(i=0; i < p_afnd_origen2->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd_origen2->alfabeto, i));

  /* Guardamos las referencias a las transiciones */
  for(i=0; i < p_afnd_origen1->n_trans; i++){
    for(j=0; j < p_afnd_origen1->transitions[i]){
      AFNDInsertaTransicion(p_a, strcat(estado_get_name(p_afnd_origen1->transitions[i]->inicial), "_C1"), p_afnd_origen1->transitions[i]->trans_symbol, strcat(estado_get_name(p_afnd_origen1->transitions[i]->final[j]), "_C1"));
    }
  }

  for(i=0; i < p_afnd_origen2->n_trans; i++){
    for(j=0; j < p_afnd_origen2->transitions[i]){
      AFNDInsertaTransicion(p_a, strcat(estado_get_name(p_afnd_origen2->transitions[i]->inicial), "_C2"), p_afnd_origen2->transitions[i]->trans_symbol, strcat(estado_get_name(p_afnd_origen2->transitions[i]->final[j]), "_C2"));
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

  return p_a;
}

AFND *AFND1OEstrella(AFND *p_afnd_origen){
  AFND *p_a = NULL;
  int i, j;
  int ntrans;
  int ini;
  int fin;

  if(!p_afnd_origen)
    return NULL;

  p_a = AFNDNuevo("AFND_*", p_afnd_origen->n_est + 2, p_afnd_origen->n_simb);
  if(!p_a)
    return NULL;

  /* Buscamos los estados inicial y final */
  for(i = 0; i < p_afnd_origen->n_est; i++){
    if(estado_get_tipo(p_afnd_origen->estados[i]) == INICIAL){
      ini=p_a->current_est;
    } else if(estado_get_tipo(p_afnd_origen->estados[i]) == FINAL){
      fin=p_a->current_est;
    }
    AFNDInsertaEstado(p_a, strcat(estado_get_name(p_afnd_origen->estados[i]), "_*"), NORMAL);
  }

   /* Insertamos los simbolos */
  for(i=0; i < p_afnd_origen->n_simb; i++)
    AFNDInsertaSimbolo(p_a, get_palabra_by_index(p_afnd_origen->alfabeto, i));

  /* Guardamos las referencias a las transiciones */
  for(i=0; i < p_afnd_origen->n_trans; i++){
    for(j=0; j < p_afnd_origen->transitions[i]){
      AFNDInsertaTransicion(p_a, strcat(estado_get_name(p_afnd_origen->transitions[i]->inicial), "_*"), p_afnd_origen->transitions[i]->trans_symbol, strcat(estado_get_name(p_afnd_origen->transitions[i]->final[j]), "_*"));
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

  return p_a;
}

void AFNDADot(AFND * p_afnd){
  FILE *f=NULL;
  int i, j, k, l;
  int tipo, index_ini;
  char *name=NULL, *symb=NULL;

  if(!p_afnd)
      return;

  f = fopen("automata.dot", "w+");
  if(!f){
      return;
  }

  fprintf(f, "diagraph %s { rankdir=LR;\n\t_invisible [style=\"invis\"];\n" p_afnd->name);
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
          for(l = 0; l < p_afnd->n_trans; l++){
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
