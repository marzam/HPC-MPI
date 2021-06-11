/*
 * Esse exercício resolve a integra x^2 no intervalo [0:1], ou seja,
 * o valor exato é de 1/3
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double func(double x){ return pow(x, 2.0); }
int main(int ac, char**av) {
  double h = 0.0,
		 		 k = 0.0,
		     x = 0.0,
		     global_sum = 0.0,
         local_sum = 0.0;

  int rank = 0,
      world = 0;

  unsigned int steps = 0,
               stepsProc = 0,
               last = 0;


  MPI_Init(&ac, &av);

  h = atof(av[1]);
  k = h / 2;
  

  MPI_Comm_size(MPI_COMM_WORLD, &world);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  steps = (unsigned int) 1.0 / h;
  stepsProc = steps / world;

  //garante que o último processo vai ficar responsável pelo resto
  //do domínio
  if (rank == world - 1){
    last = steps % world;
  }

  if (rank == 0){
    fprintf(stdout, "\nIntegração de uma função f(x)\n");
    fprintf(stdout, "Qtde. de trapézios: %u - valor do h = %15.10lf\n", steps, h);
    fprintf(stdout, "         Processos: %d \n", world);
  }



  for (unsigned int i = 0; i <= stepsProc + last; i++){

		  double offset = h * (double) (i + rank * stepsProc);
		  double aux = 2.0 * k;
      if ( ((i == 0) && (rank == 0)) ||  ((i == stepsProc + last) && (rank == world - 1)))
        aux = k;

		  local_sum += (func(offset) * aux);


  }

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    fprintf(stdout, "resultado: I = %15.10lf\n", global_sum);

  MPI_Finalize();
  return EXIT_SUCCESS;
}
