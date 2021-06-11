#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
struct stMatrix{
    float *A;
    float *B;
    float *X;
    int mn;
};
typedef struct stMatrix tpMatrix;
void LoadMatrixAndVector(char *matrixFile, char *vectorFile, tpMatrix *matrix);
void PrintMatrixAndVector(const tpMatrix *matrix);
void JacobiMethod(tpMatrix *matrix, const unsigned int inter);
void PrintX(float *X, const int size);

int main(int ac, char**av) {
  tpMatrix matrix;
  int      iter = atoi(av[4]);
  int      flagSave = atoi(av[5]);
  fprintf(stdout, "\nMétodo iterativo de solução de sistema linear - Jacobi\n");
  fprintf(stdout, "Passos: %d\n", iter);
  matrix.mn = atoi(av[3]);
  matrix.A = (float*) malloc (matrix.mn * matrix.mn *  sizeof(float));
  matrix.B = (float*) malloc (matrix.mn * sizeof(float));
  matrix.X = (float*) malloc (matrix.mn *  sizeof(float));
  memset(matrix.X, 0x00, matrix.mn *  sizeof(float));

  LoadMatrixAndVector(av[1], av[2], &matrix);
  //PrintMatrixAndVector(&matrix);
  JacobiMethod(&matrix, iter);

  if (flagSave == 1)
    PrintX(matrix.X, matrix.mn);



  free(matrix.A);
  free(matrix.B);
  free(matrix.X);

  return EXIT_SUCCESS;
}

/*
 * Carrega a matrix e o vetor B do arquivo
 */
void LoadMatrixAndVector(char *matrixFile, char *vectorFile, tpMatrix *matrix){
  FILE *ptr = fopen(matrixFile, "r+");
  assert(ptr != NULL);
  for (int j = 0; j < matrix->mn; j++){
    for (int i = 0; i < matrix->mn; i++){
      int k = j * matrix->mn + i;
      int ret = fscanf(ptr, "%f", &matrix->A[k]);
    }
  }
  fclose(ptr);

  ptr = fopen(vectorFile, "r+");
  assert(ptr != NULL);
  for (int i = 0; i < matrix->mn; i++){
    int ret = fscanf(ptr, "%f", &matrix->B[i]);
  }
  fclose(ptr);
}

void PrintMatrixAndVector(const tpMatrix *matrix){

  fprintf(stdout, "Matrix (%d, %d)\n", matrix->mn, matrix->mn);
  for (int j = 0; j < matrix->mn; j++){
    for (int i = 0; i < matrix->mn; i++){
      int k = j * matrix->mn + i;
      fprintf(stdout, "%.7f ", matrix->A[k]);
    }
   fprintf(stdout, " \t %.7f \n", matrix->B[j]);
  }
}

void JacobiMethod(tpMatrix *matrix, const unsigned int inter){

   float aux, valueX, div;

   for (int s = 1; s <= inter; s++){
      for (int j = 0; j < matrix->mn; j++){
        aux    = 0.0f;
        div    = 0.0f;
        valueX = 0.0f;
        for (int i = 0; i < matrix->mn; i++){
           valueX = matrix->X[i];
           if (j != i)
              aux += ((matrix->A[matrix->mn * j + i] * valueX) * -1.0f);
           else
              div =  matrix->A[matrix->mn * j + i];
        }
        matrix->X[j] = ((matrix->B[j] + aux) / div );
      }
   }
}

void PrintX(float *X, const int size){
  FILE *ptr = fopen("solucao.txt", "w+");
  assert(ptr != NULL);
  for (int i = 0; i < size; i++){
    fprintf(ptr, "%f ", X[i]);
  }
  fprintf(ptr, "\n");
  fclose(ptr);
}
