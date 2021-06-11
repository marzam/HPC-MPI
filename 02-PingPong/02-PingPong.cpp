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
   MPI_Status  m_status;
   MPI_Init(&ac,&av);
   srand (42);

   m_vetSize = stoi(av[1]); //stoi(av[1]);

   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);

   cout << m_hostname << " started" << endl;
   if (m_pid == 0)
      cout << "Tamanho do vetor: " << m_vetSize << " \tEspaço em memória: " << m_vetSize * sizeof(int64_t) << endl;

   posix_memalign(reinterpret_cast <void**>(&m_vet), ALING, sizeof(int64_t) * m_vetSize);
   memset(m_vet, 0x00, sizeof(int64_t) * m_vetSize);


   if (m_pid == 0){
     for (int64_t i = 0; i < m_vetSize; i++){
        m_vet[i] = static_cast<unsigned char> ( rand() % INT64_MAX);
     }//end-for (int64_t = i; i < m_vetSize; i++){

     elapsedtime = MPI_Wtime();
     MPI_Send(m_vet, m_vetSize * sizeof(int64_t), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
     elapsedtime =  MPI_Wtime() - elapsedtime;
   }else  if (m_pid == 1){
        MPI_Recv(m_vet, m_vetSize * sizeof(int64_t), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &m_status);
        /*
        for (int64_t i = 0; i < m_vetSize; i++){
           cout << m_vet[i] << endl;
        }//
        */
   }
/*
   MPI_Barrier(MPI_COMM_WORLD);
   if (m_pid == 0){
     cout << "Tempo:" << elapsedtime << " s" << endl;
     int64_t j = 10;
     if (m_vetSize < j)
      j = m_vetSize;

     cout << "Host: " << m_hostname << endl;
     for (int64_t i = 0; i < j; i++){
       cout << m_vet[i] << endl;
     }//end-for (int64_t = i; i < m_vetSize; i++){

   }


   MPI_Barrier(MPI_COMM_WORLD);
   if (m_pid == 1){
     cout << "\tTempo:" << elapsedtime << " s" << endl;
     int64_t j = 10;
     if (m_vetSize < j)
      j = m_vetSize;

     cout << "Host: " << m_hostname << endl;
     for (int64_t i = 0; i < j; i++){
       cout << m_vet[i] << endl;
     }//end-for (int64_t = i; i < m_vetSize; i++){

   }
*/
   if (m_pid == 0){
      string filename = string(m_hostname) + ".sync.log";
      mLog.open(filename, fstream::app | fstream::out);
      cout << elapsedtime << endl;
      mLog << m_vetSize * sizeof(int64_t) << ";" << elapsedtime << endl;
      mLog.close();
      cout << m_hostname << endl;
  }
/*
  if (m_pid == 1){
    cout <<  "Status: S: " << m_status.MPI_SOURCE << " \t T:" << m_status.MPI_TAG << "\t E:" << m_status.MPI_ERROR << endl;
  }
  */
   free(m_vet);
   MPI_Finalize();
   return EXIT_SUCCESS;
}
