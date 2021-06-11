#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <string>
#include <ctime>
#include <fstream>
#define ALING 64
using namespace std;
void printSerialized(double *A, int w, int h, int m_pid, int m_nprocs){
  for (int p = 0; p < m_nprocs; p++){
    if (p == m_pid){
      for (int j = 0; j < h; j++){
        for (int i = 0; i < w; i++){
          int k = j * w + i;
          cout << A[k] << " ";
        }//end-for (int i = 0; i < w; i++){
        cout << endl;
      }//end-for (int j = 0; j < h; j++){

    }//end-if (i == m_pid){
    MPI_Barrier(MPI_COMM_WORLD) ;
  }//end-for (int i = 0; i < m_nprocs; i++){
}

void procedure_As(double *A, int w, int h, int m_pid, int m_nprocs, double s){
  for (int p = 0; p < m_nprocs; p++){
    if (p == m_pid){
      for (int j = 0; j < h; j++){
        for (int i = 0; i < w; i++){
          int k = j * w + i;
          A[k] *= s;
        }//end-for (int i = 0; i < w; i++){

      }//end-for (int j = 0; j < h; j++){

    }//end-if (i == m_pid){
    MPI_Barrier(MPI_COMM_WORLD) ;
  }//end-for (int i = 0; i < m_nprocs; i++){
}

void openfile(void){
  int    m_pid,
         m_nprocs;

  int w = 10,
      h = 10,
      m_offset = 0,
      m_namelen;

  char   m_hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_File m_infile, m_outfile;
  MPI_Status m_status;
  double *m_buffer = NULL;


  MPI_Comm_size(MPI_COMM_WORLD,&m_nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&m_pid);
  MPI_Get_processor_name(m_hostname, &m_namelen);

  h /= m_nprocs;
  m_offset = w * h;
  posix_memalign(reinterpret_cast <void**>(&m_buffer), ALING, sizeof(double) * m_offset);

  cout << "id: " << m_pid << " " << m_hostname << " offset " << m_pid * m_offset << endl;
  //MPI_File_open(MPI_COMM_WORLD, "/home/mzamith/Documents/Develop/C++/MPI/src/03-Matriz-load/mat.bin", MPI_MODE_RDONLY, MPI_INFO_NULL, &m_infile);
  //https://www.open-mpi.org/doc/v4.0/man3/MPI_File_open.3.php
  MPI_File_open(MPI_COMM_WORLD, "mat.bin", MPI_MODE_RDONLY, MPI_INFO_NULL, &m_infile);
  MPI_File_seek(m_infile, m_pid * m_offset * sizeof(double), MPI_SEEK_SET);
  MPI_File_read(m_infile, m_buffer, m_offset * sizeof(double), MPI_CHAR, &m_status);
  MPI_File_close(&m_infile);

  printSerialized(m_buffer, w, h, m_pid, m_nprocs);
  procedure_As(m_buffer, w, h, m_pid, m_nprocs, 0.001);

  MPI_File_open(MPI_COMM_WORLD, "mat-out.bin",   MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &m_outfile);
  MPI_File_seek(m_outfile, m_pid * m_offset * sizeof(double), MPI_SEEK_SET);
  MPI_File_write(m_outfile, m_buffer, m_offset * sizeof(double), MPI_CHAR, MPI_STATUS_IGNORE);
  MPI_File_close(&m_outfile);

  printSerialized(m_buffer, w, h, m_pid, m_nprocs);
  free(m_buffer);
}
int  main (int ac, char **av){

   MPI_Init(&ac,&av);
   openfile();

   MPI_Finalize();
   return EXIT_SUCCESS;
}
