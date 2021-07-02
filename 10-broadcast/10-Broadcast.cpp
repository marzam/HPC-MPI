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

int  main (int ac, char **av){

   int    m_pid,
          m_nprocs;

   int    m_namelen;
   char   m_hostname[MPI_MAX_PROCESSOR_NAME];
   int    m_msg = 0;

   MPI_Init(&ac,&av);
   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);
   if (m_pid == 0){
     cout << "Broadcast - MPI" << endl;
     m_msg = 42;
   }
   cout.flush();
   MPI_Barrier(MPI_COMM_WORLD);
   cout << m_pid << " msg: " << m_msg << endl;
   MPI_Barrier(MPI_COMM_WORLD);
   if (m_pid == 0){
     cout << "Broadcast - msg" << endl;
   }
   MPI_Bcast(&m_msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
   cout << m_pid << " msg: " << m_msg << endl;

   MPI_Finalize();
   return EXIT_SUCCESS;
}
