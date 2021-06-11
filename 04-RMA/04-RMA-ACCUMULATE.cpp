//https://docs.microsoft.com/en-us/message-passing-interface/mpi-datatype-enumeration
#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <ctime>
#include <fstream>
#include <cassert>
#define ALING 64
using namespace std;
void printVet(int *A, int s, int m_pid){
  cout << "ID :" << m_pid << endl;
  for (int i = 0; i < s; i++){
    cout << A[i] << endl;
  }//end-for (int i = 0; i < s; i++){
  cout << "--------------------" << endl;
}//end-void printVet(int *A, int s){

int  main (int ac, char **av){

   MPI_Init(&ac,&av);
   int    m_pid,
          m_nprocs;

   int A[20],
       m_namelen,
       disp_unit = 1;

   char   m_hostname[MPI_MAX_PROCESSOR_NAME];
   MPI_Status m_status;
   MPI_Win win;
   MPI_Aint my_variable_address;


   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);
   //posix_memalign(reinterpret_cast <void**>(&m_buffer), ALING, sizeof(double) * s);
   if (m_pid == 0){
     cout << "Exemplo de RMA (Remote Mempory Access)" << endl;

   }

   for (int i = 0; i < 20; i++){
     A[i] = (i+1) ;
   }


   assert(MPI_Win_create(A, 20*sizeof(int), disp_unit, MPI_INFO_NULL, MPI_COMM_WORLD, &win) == MPI_SUCCESS);
   MPI_Win_fence( 0, win ) ;

   if (m_pid == 1){
     /*
      A -> buffer
      7 -> número de elementos, posições
      MPI_INT -> tipo na origem
      3 -> rank de destino
      1*sizeof(int) -> deslocamento
      7 -> number of entries in target buffer (nonnegative integer)
      quantos elementos serão atualizados no processo de destino
      deve ser igual ao número de elementos, posições
      MPI_INT tipo no destino
      win -> janel
     */
     //MPI_Put(A, 7, MPI_INT, 3,  1*sizeof(int), 3, MPI_INT, win);
     MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
     //MPI_Get(A, 8, MPI_INT, 0,  1*sizeof(int), 8, MPI_INT, win);
     //https://www.open-mpi.org/doc/v4.0/man3/MPI_Win_fence.3.php
     //operações: https://docs.microsoft.com/en-us/message-passing-interface/mpi-op-enumeration
     //https://cvw.cac.cornell.edu/mpionesided/exercise
     //MPI_Accumulate(A, 5, MPI_INT, 0, sizeof(int), 5, MPI_INT, MPI_SUM, win);
     MPI_Accumulate(A, 20, MPI_INT, 0, 0, 20, MPI_INT, MPI_SUM, win);
     MPI_Win_unlock(0, win);

     /*
      MPI_Get(void *origin_addr,
              int origin_count,
              MPI_Datatype origin_datatype,
              int target_rank,
              MPI_Aint target_disp,
              int target_count,
              MPI_Datatype target_datatype,
              MPI_Win win)
    */
   }
   MPI_Win_fence( 0, win ) ;
   assert(MPI_Win_free(&win) == MPI_SUCCESS);

    for (int i = 0; i < m_nprocs; i++){
      if (i == m_pid){
         printVet(A, 20, m_pid);
      }
      MPI_Barrier(MPI_COMM_WORLD);
    }//end-for (int i = 0; i < m_nprocs; i++){


   //free(m_buffer);
   MPI_Finalize();
   return EXIT_SUCCESS;
}
