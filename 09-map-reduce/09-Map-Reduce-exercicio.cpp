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
   double h = stof(av[1]);
   unsigned int m_steps = static_cast<unsigned int>( 1.0 / h );
   unsigned int m_junk = m_steps / m_nprocs;
   unsigned int m_start = static_cast<unsigned int>( m_pid ) * m_junk;
   unsigned int m_end   = 0;

   if (m_pid == 0){
     cout << "Integração de uma função f(x)" << endl;
     cout << "Qtde. de trapézios steps: " << m_steps << " h: " << h << endl;
     cout << "Qtde. por processos: " << m_junk << endl;
     cout << "              Resto: " << m_steps % m_nprocs << endl;
   }

   if (m_pid == (m_nprocs - 1)){
     m_junk += m_steps % m_nprocs;
   }
   m_end = m_start + m_junk;
   cout << m_start << ", " << m_end << endl;

   MPI_Finalize();
   return EXIT_SUCCESS;
}
