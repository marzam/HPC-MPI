#include <iostream>
#include <cstdlib>
#include <mpi.h>
using namespace std;
int  main (int argc, char **argv){

   int    m_pid,
          m_nprocs,
          m_namelen;

   char   m_hostname[MPI_MAX_PROCESSOR_NAME];
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);

   cout << "Hi, I am " << m_hostname << " pid: " << m_pid << " / " << m_nprocs << endl;

   MPI_Finalize();
   return EXIT_SUCCESS;
}
