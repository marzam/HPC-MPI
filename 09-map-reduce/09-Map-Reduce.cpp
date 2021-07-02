#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cmath>
#define ALING 64
using namespace std;
double func(double x){ return pow(x, 2.0); }
int  main (int ac, char **av){

   int    m_pid,
          m_nprocs;

   int    m_namelen;
   char   m_hostname[MPI_MAX_PROCESSOR_NAME];


   MPI_Init(&ac,&av);
   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);
   int m_steps = stoi(av[1]);
   int m_junk = m_steps / m_nprocs;
   int m_start = m_pid * m_junk + 1;
   int m_end   = 0;
   int m_sum_local = 0;
   int m_sum_global = 0;

   if (m_pid == 0){
     cout << "Mapping reducing - MPI" << endl;
     cout << "              Qtde. de passos: " << m_steps << endl;
     cout << "Qtde. de passos por processos: " << m_junk << endl;
     cout << "                        Resto: " << m_steps % m_nprocs << endl;
   }
//   MPI_Barrier(MPI_COMM_WORLD);
   if (m_pid == (m_nprocs - 1)){
     m_junk += m_steps % m_nprocs;
   }
   m_end = m_start + m_junk;

   for (int i = m_start; i < m_end; i++){
     m_sum_local += i;
   }//end-for (int i = m_start; i < m_end; i++){

   //MPI_Reduce(&m_sum_local, &m_sum_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Allreduce(&m_sum_local, &m_sum_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   if (m_pid == 0){
     cout << "Total da soma: " << m_sum_global << endl;
     cout << "------------------------------------------" << endl;
   }//end-if (m_pid == 0){
   MPI_Barrier(MPI_COMM_WORLD);
   cout << m_pid << " Soma: " << m_sum_global << endl;

/*
MPI_Op op:
Name                Meaning
 ---------           --------------------
MPI_MAX             maximum
MPI_MIN             minimum
MPI_SUM             sum
MPI_PROD            product
MPI_LAND            logical and
MPI_BAND            bit-wise and
MPI_LOR             logical or
MPI_BOR             bit-wise or
MPI_LXOR            logical xor
MPI_BXOR            bit-wise xor
MPI_MAXLOC          max value and location
MPI_MINLOC          min value and location
*/
   //cout << m_start << ", " << m_end << ", " << m_sum_local << endl;

   MPI_Finalize();
   return EXIT_SUCCESS;
}
