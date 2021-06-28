#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#define ALING 64
using namespace std;
void save2file_text(double *m_buffer, int w, int h, string filename){
  fstream output;
  //cout << "Salvando arquivo: " << filename << endl;
  output.open(filename, fstream::out | fstream::trunc);
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      int p = i * w + j;
      output << m_buffer[p] << " ";
    }
    output << endl;
  }

  output.close();
}
int  main (int ac, char **av){

   int    m_pid,
          m_nprocs;

   int W = 0,
       H = 0,
       w = 0,
       h = 0,
       m_offset = 0,
       m_namelen;


   char   m_hostname[MPI_MAX_PROCESSOR_NAME];
   MPI_File m_infile, m_outfile;
   MPI_Status m_status;
   double *m_buffer = NULL,
          *m_buffer_aux = NULL;

   MPI_Init(&ac,&av);
   MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
   MPI_Get_processor_name(m_hostname, &m_namelen);
   W = stoi(av[1]);
   H = stoi(av[2]);
   w = W;
   h = H / m_nprocs;

   posix_memalign(reinterpret_cast <void**>(&m_buffer), ALING, sizeof(double) * w * h);
   posix_memalign(reinterpret_cast <void**>(&m_buffer_aux), ALING, sizeof(double) * w * h);
   memset(m_buffer_aux, 0x00, sizeof(double) * w * h);
   if (m_pid == 0){
     cout << "Processos: " << m_pid << "/" << m_nprocs << "\t" << m_hostname << " Matriz(" << w << "," << (m_nprocs * h) << ") " << endl;
     cout << "Memória locada por processo: " << (w * h * sizeof(double) * 2) << " bytes" << endl;
   }

   for (int i = 0; i < h; i++){
     for (int j = 0; j < w; j++){
       int p = i * w + j;
       m_buffer[p] = static_cast<double>((w * h * m_pid) + p) * 10.0;
     }
   }
   MPI_Barrier(MPI_COMM_WORLD);
   //Salvando
   string filename =  string("processo." + to_string(m_pid) + ".txt");
   //salvando o arquivo original
   save2file_text(m_buffer, w, h, filename);
   //calcular a submatriz

   int lW = 0, lH = 0;
   double *m_buffer_local = NULL ; //submatriz
   //a submatriz sempre é quadrada, funciona pq as linhas são divididas pelos processos
   lW = H / m_nprocs;
   lH = W / m_nprocs ;

   //Variáveis de gerencia
   //endereçamentp da matriz transposta
   int h_aux = W / m_nprocs;
   int w_aux = H;

   posix_memalign(reinterpret_cast <void**>(&m_buffer_local), ALING, sizeof(double) * lW * lH);
   //inverte a matriz antes de enviar
   for (int to_pid = 0; to_pid < m_nprocs; to_pid++){
       for (int j = 0; j < lH; j++){
         for (int i = 0; i < lW; i++){
           int k = j * lW + i;
           //int l = j * w + (i + (m_pid * lW)) ;
           int l = (i * w + (j + (to_pid * lH))) ;
           m_buffer_local[k] = m_buffer[l];

        }//end-for (int i = 0; i < lW; i++){
       }//end-for (int j = 0; j < lH; j++){

        //se for a diagonal principal, já coloca no buffer_aux a matriz transposta
       if ((to_pid == m_pid) /*&& (m_pid == 2)*/){

         for (int j = 0; j < lH; j++){
           for (int i = 0; i < lW; i++){
             int k = j * lW + i;
             //int l = (m_pid * w_aux) + j * lW + i;
             int l = (m_pid * lW) + (j * w_aux + i);
             m_buffer_aux[l] = m_buffer_local[k];
           }//end-for (int i = 0; i < lW; i++){
           //cout << endl;
           //cout << endl;
         }//end-for (int j = 0; j < lH; j++){
       }else{
         MPI_Send(m_buffer_local, (lW * lH * sizeof(double)), MPI_BYTE, to_pid, 0, MPI_COMM_WORLD);
       }

   }//end-for (int to_id = 0; to_id < m_nprocs; to_id++){

   //Sincroniza os processos a fim de garantir a todos os processos já enviaram suas matrizes
   //garante que a variabel m_buffer_local não será escrita quando ainda estiver enviando msgs
   MPI_Barrier(MPI_COMM_WORLD);
   for (int from_pid = 0; from_pid < m_nprocs; from_pid++){
     if (from_pid != m_pid){
       MPI_Recv(m_buffer_local, (lW * lH * sizeof(double)), MPI_BYTE, from_pid, 0, MPI_COMM_WORLD, &m_status);
       //filename =  string("MPI_Recv.from." + to_string(from_pid) + ".to." + to_string(m_pid) + ".txt");
       //salvando o arquivo original
       //save2file_text(m_buffer_local, lW, lH, filename);

       for (int j = 0; j < lH; j++){
         for (int i = 0; i < lW; i++){
           int k = j * lW + i;
           int l = (j * w_aux) + (from_pid * lW) +  i;//(i * w + (j + (from_pid * lH))) ;
           m_buffer_aux[l] = m_buffer_local[k];
           //if (m_pid == 0)           cout << j * w + (i + (to_id * lW))  << " ";
         }//end-for (int i = 0; i < lW; i++){
       }//end-for (int j = 0; j < lH; j++){

     }//end-if (from_pid != m_pid){
   }//end-for (int from_pid = 0; from_pid < m_nprocs; from_pid++){

   filename =  string("processo.transposta." + to_string(m_pid) + ".txt");
   //salvando o arquivo original
   save2file_text(m_buffer_aux, w_aux, h_aux, filename);

   free(m_buffer);
   free(m_buffer_aux);
   free(m_buffer_local);
   MPI_Finalize();
   return EXIT_SUCCESS;
}
