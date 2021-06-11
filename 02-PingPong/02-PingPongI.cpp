#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <cstdint>
#include <ctime>
#include <fstream>
#define ALING 64
using namespace std;
int  main (int ac, char **av){

   int    m_pid,
          m_nprocs,
          m_namelen,
          m_vetSize;
   double elapsedtime = 0.0;
   int64_t *m_vet;
   char   m_hostname[MPI_MAX_PROCESSOR_NAME];
   fstream mLog;
   MPI_Request m_request;
   MPI_Status  m_status;

   MPI_Init(&ac,&av);
   srand (42);

   m_vetSize = stoi(av[1]); //stoi(av[1]);

   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);


   if (m_pid == 0)
      cout << "Tamanho do vetor: " << m_vetSize << " \tEspaço em memória: " << m_vetSize * sizeof(int64_t) << endl;

   posix_memalign(reinterpret_cast <void**>(&m_vet), ALING, sizeof(int64_t) * m_vetSize);
   memset(m_vet, 0x00, sizeof(int64_t) * m_vetSize);


   if (m_pid == 0){
     for (int64_t i = 0; i < m_vetSize; i++){
        m_vet[i] = static_cast<unsigned char> ( rand() % INT64_MAX);
     }//end-for (int64_t = i; i < m_vetSize; i++){

     elapsedtime = MPI_Wtime();
     MPI_Isend(m_vet, m_vetSize * sizeof(int64_t), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &m_request);
     elapsedtime =  MPI_Wtime() - elapsedtime;
     MPI_Wait(&m_request, &m_status);
   }else  if (m_pid == 1){
        MPI_Irecv(m_vet, m_vetSize * sizeof(int64_t), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &m_request);
        MPI_Wait(&m_request, &m_status);
        /*
        for (int64_t i = 0; i < m_vetSize; i++){
           cout << m_vet[i] << endl;
        }//
        */
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (m_pid == 0){
      string filename = string(m_hostname) + ".async.log";
      mLog.open(filename, fstream::app | fstream::out);
      cout << elapsedtime << endl;
      mLog << m_vetSize * sizeof(int64_t) << ";" << elapsedtime << endl;
      mLog.close();
      cout << m_hostname << endl;
  }
   //MPI_Send(void* data, int count, MPI_Datatype datatype, int destination,  int tag,  MPI_Comm communicator)
   //MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
  /*
   int number;
   if (world_rank == 0) {
       number = -1;
       MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
   } else if (world_rank == 1) {
       MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
       printf("Process 1 received number %d from process 0\n",
              number);
   }

*/
   free(m_vet);
   MPI_Finalize();
   return EXIT_SUCCESS;
}
