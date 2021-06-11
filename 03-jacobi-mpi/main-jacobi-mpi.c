#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <mpi.h>
struct stMatrix{
    float *A;
    float *B;
    float *X;
    int mn;
};
typedef struct stMatrix tpMatrix;
void LoadMatrixAndVector(char *matrixFile, char *vectorFile, tpMatrix *matrix);
void PrintMatrixAndVector(const tpMatrix *matrix);
void JacobiMethod(tpMatrix *matrix, const unsigned int inter, const int rank, const int world);
void PrintX(float *X, const int size);

int main(int ac, char**av) {
  tpMatrix matrix;
  int      iter = 0;
  int      flagSave = 0;
  int      rank = 0;
  int      world = 0;

  MPI_Init(&ac, &av);
  MPI_Comm_size(MPI_COMM_WORLD, &world);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  if (rank == 0){
    fprintf(stdout, "\nMétodo iterativo de solução de sistema linear - Jacobi\n");
    fprintf(stdout, "Passos: %d\n", iter);
    fprintf(stdout, "Processos: %d\n", world);
  }


  iter = atoi(av[4]);
  flagSave = atoi(av[5]);

  matrix.mn = atoi(av[3]);
  matrix.A = (float*) malloc (matrix.mn * matrix.mn *  sizeof(float));
  matrix.B = (float*) malloc (matrix.mn * sizeof(float));
  matrix.X = (float*) malloc (matrix.mn *  sizeof(float));
  memset(matrix.X, 0x00, matrix.mn *  sizeof(float));

  if (rank == 0){
    LoadMatrixAndVector(av[1], av[2], &matrix);
  }
  MPI_Bcast(matrix.A, matrix.mn * matrix.mn, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrix.B, matrix.mn, MPI_FLOAT, 0, MPI_COMM_WORLD);

  JacobiMethod(&matrix, iter, rank, world);

  if ((flagSave == 1) && (rank == 0))
    PrintX(matrix.X, matrix.mn);


  free(matrix.A);
  free(matrix.B);
  free(matrix.X);

  MPI_Finalize();
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

void JacobiMethod(tpMatrix *matrix, const unsigned int inter, const int rank, const int world){

  int submn = matrix->mn / world;
  int last  = 0;

  if (rank == (world - 1))
    last = matrix->mn % world;


   for (int s = 1; s <= inter; s++){
      for (int k = 0; k < submn + last; k++){//paralelo
        float aux = 0.0, valueX = 0.0, div = 0.0;

        //endereço global
        int j = k + rank * submn;

        for (int i = 0; i < matrix->mn; i++){
           valueX = matrix->X[i];
           if (j != i)
              aux += ((matrix->A[matrix->mn * j + i] * valueX) * -1.0f);
           else
              div =  matrix->A[matrix->mn * j + i];
        }//end-for (int i = 0; i < matrix->mn; i++){

        matrix->X[j] = ((matrix->B[j] + aux) / div );
      }//end-for (int k = 0; k < matrix->mn; k++){

      //Sincronização dos dados
      if (rank != 0)
        MPI_Send(&(matrix->X[rank * submn]), submn + last, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
      else{
        for (int p = 1; p < world; p++)
            MPI_Recv(&(matrix->X[p * submn]), submn + last, MPI_FLOAT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
      MPI_Bcast(matrix->X, matrix->mn, MPI_FLOAT, 0, MPI_COMM_WORLD);



   }//end-for (int s = 1; s <= inter; s++){
}

void PrintX(float *X, const int size){
  FILE *ptr = fopen("solucao-mpi.txt", "w+");
  assert(ptr != NULL);
  for (int i = 0; i < size; i++){
    fprintf(ptr, "%f ", X[i]);
  }
  fprintf(ptr, "\n");
  fclose(ptr);
}
