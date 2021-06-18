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
  cout << "Salvando arquivo: " << filename << endl;
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
   if (m_pid == 0){
     cout << "Processos: " << m_pid << "/" << m_nprocs << "\t" << m_hostname << " Matriz(" << w << "," << (m_nprocs * h) << ") " << endl;
     cout << "Memória locada por processo: " << (w * h * sizeof(double)) << " bytes" << endl;
   }

   for (int i = 0; i < h; i++){
     for (int j = 0; j < w; j++){
       int p = i * w + j;
       m_buffer[p] = static_cast<double>(m_pid);
     }
   }
   MPI_Barrier(MPI_COMM_WORLD);
   //Salvando
   string filename =  string("processo." + to_string(m_pid) + ".txt");
   //salvando o arquivo original
   save2file_text(m_buffer, w, h, filename);
   //comunicação entre os processos
   int from_id = m_pid - 1;
   int to_id = m_pid + 1;
   if (to_id == m_nprocs)
      to_id = 0;

   if (from_id < 0)
    from_id = m_nprocs - 1;

   //Criando a comunicação
   MPI_Send(m_buffer, (w * h * sizeof(double)), MPI_BYTE, to_id, 0, MPI_COMM_WORLD);
   MPI_Recv(m_buffer_aux, (w * h * sizeof(double)), MPI_BYTE, from_id, 0, MPI_COMM_WORLD, &m_status);
   //Por que não precisa sincronizar neste ponto ??
   filename =  string("processo.new." + to_string(m_pid) + ".txt");
   //salvando o arquivo original
   save2file_text(m_buffer_aux, w, h, filename);

   free(m_buffer);
   free(m_buffer_aux);

   MPI_Finalize();
   return EXIT_SUCCESS;
}
