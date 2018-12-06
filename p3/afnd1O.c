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
    int i;

    if(!p_afnd1O_1 || !p_afnd1O_2)
        return NULL;
}

AFND *AFND1OConcatena(AFND *p_afnd_origen1, AFND *p_afnd_origen2){
    if(!p_afnd_origen1 || !p_afnd_origen2)
        return NULL;
}

AFND *AFND1OEstrella(AFND *p_afnd_origen){
	int i, j;
    int n_ini, n_fin;
    int tipo;
    int *indexes_ini=NULL;
    int *indexes_fin=NULL;

    /*Reservamos memoria para los indices de los estados iniciales actuales*/
	indexes_ini=(int *)malloc(sizeof(int)*p_afnd->n_est);
    if(!indexes_ini){
        return NULL;
    }

    /*Reservamos memoria para los indices de los estados finales actuales*/
    indexes_fin=(int *)malloc(sizeof(int)*p_afnd->n_est);
    if(!indexes_fin){
        return NULL;
    }

    /*Comprobacion de argumentos de entrada*/
    if(!p_afnd_origen)
        return NULL;

    /*Sacamos indices de estados iniciales, finales e iniciales-finales*/
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
        }
    }

    /*Convertimos estados iniciales a normales*/
    for(i = 0; i < n_ini; i++){
    	estado_set_tipo(p_afnd->estados[indexes_ini[i]], NORMAL);
    }

    /*Convertimos estados finales a normales*/
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
    		p_afnd->lambda_trans[i][j] = 0;
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

    AFNDInsertaLTransicion(p_afnd_origen, "_i_1O", "_f_1O");
    AFNDInsertaLTransicion(p_afnd_origen, "_f_1O", "_i_1O");

    free(indexes_ini);
    free(indexes_ini);
}

void AFNDADot(AFND * p_afnd){
    FILE *f=NULL;
    int i;
    int tipo, index_ini;

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

    /* Falta imprimir todas las transiciones */
}
