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

   int w = 0,
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
   w = stoi(av[1]);
   h = w;
   h /= m_nprocs;

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
       m_buffer[p] = static_cast<double>((w * h * m_pid) + p);
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
   lW = h;
   lH = h;
   posix_memalign(reinterpret_cast <void**>(&m_buffer_local), ALING, sizeof(double) * lW * lH);
   for (int to_pid = 0; to_pid < m_nprocs; to_pid++){
       for (int j = 0; j < lH; j++){
         for (int i = 0; i < lW; i++){
           int k = i * lH + j;
           //int l = j * w + (i + (m_pid * lW)) ;
           int l = j * w + (i + (to_pid * lW));
           m_buffer_local[k] = m_buffer[l];

           if (to_pid == m_pid){
              k = i * w + (j + (to_pid * lW));
              m_buffer_aux[k] = m_buffer[l];
           }
           //if (m_pid == 0)           cout << j * w + (i + (to_id * lW))  << " ";
         }//end-for (int i = 0; i < lW; i++){
       }//end-for (int j = 0; j < lH; j++){
       if (to_pid != m_pid)
          MPI_Send(m_buffer_local, (lW * lW * sizeof(double)), MPI_BYTE, to_pid, 0, MPI_COMM_WORLD);
   }//end-for (int to_id = 0; to_id < m_nprocs; to_id++){
   MPI_Barrier(MPI_COMM_WORLD);

   for (int from_pid = 0; from_pid < m_nprocs; from_pid++){
     if (from_pid != m_pid){
         MPI_Recv(m_buffer_local, (lW * lW * sizeof(double)), MPI_BYTE, from_pid, 0, MPI_COMM_WORLD, &m_status);
         for (int j = 0; j < lH; j++){
           for (int i = 0; i < lW; i++){
             int k = j * lH + i;
             //int l = j * w + (i + (m_pid * lW)) ;
             int l = j * w + (i + (from_pid * lW));
             m_buffer_aux[l] = m_buffer_local[k];
             //if (m_pid == 0)           cout << j * w + (i + (to_id * lW))  << " ";
           }//end-for (int i = 0; i < lW; i++){
         }//end-for (int j = 0; j < lH; j++){
     }


   }//end-for (int from_id = 0; from_id < m_nprocs; from_id++){

 filename =  string("processo.invert." + to_string(m_pid) + ".txt");
 //salvando o arquivo original
 save2file_text(m_buffer_aux, w, h, filename);




/*

   MPI_Recv(m_buffer_aux, (w * h * sizeof(double)), MPI_BYTE, from_id, 0, MPI_COMM_WORLD, &m_status);
*/
   //Por que não precisa sincronizar neste ponto ??
   /*
   filename =  string("processo.invert." + to_string(m_pid) + ".txt");
   //salvando o arquivo original
   save2file_text(m_buffer_aux, w, h, filename);
   */
   free(m_buffer);
   free(m_buffer_aux);

   MPI_Finalize();
   return EXIT_SUCCESS;
}
