#include <stdio.h>
#include <stdlib.h>

void cierre_reflexivo(int **matrix, int matrix_dim){
  int i;
  
  if (!matrix || matrix_dim <= 0)
    return;
    
  for(i = 0; i < matrix_dim; i++)
    matrix[i][i] = 1;
}

void busca_unos_rec(int fila, int col, int **matrix, int dim){
  int i;
  
  if (!matrix || fila < 0 || col < 0 || dim <= 0)
    return;
    
  for(i = 0; i < dim; i++){
    if(col != i && matrix[col][i] == 1){
      matrix[fila][i] = 1;
      busca_unos_rec(fila, i, matrix, dim);
    }
  }

}

void cierre_transitivo(int **matrix, int matrix_dim){
  int i, j, k;

  if (!matrix || matrix_dim <= 0)
    return;
    
  for(i = 0; i < matrix_dim; i++){
    for(j = 0; j < matrix_dim; j++){
      if (i != j){
        if (matrix[i][j] == 1){
          busca_unos_rec(i, j, matrix, matrix_dim);
        }
      }
    }
  }
}
