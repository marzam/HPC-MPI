#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
struct stLattice{
    unsigned char *buff0;
    unsigned char *buff1;
    unsigned char *inBuffTop;
    unsigned char *inBuffBot;

    int width;
    int height;
    int steps;
    int rank;
    int world;
};
typedef struct stLattice tpLattice;

void InitRandness(tpLattice *mLattice, float p);
void GameOfLife(tpLattice *mLattice);
void print2File(tpLattice *mLattice);
int main(int ac, char **av)
{
    int flagSave = 0;

    float prob   = 0.0f;
    tpLattice mLattice;

    MPI_Init(&ac, &av);
    MPI_Comm_size(MPI_COMM_WORLD, &mLattice.world);
    MPI_Comm_rank(MPI_COMM_WORLD, &mLattice.rank);


    flagSave = atoi(av[4]);
    prob     = atof(av[5]);

    //Inicializa variável
    mLattice.width  = atoi(av[1]);
    mLattice.height = atoi(av[2])  / mLattice.world;
    mLattice.steps  = atoi(av[3]);

    if (mLattice.rank == 0){
      fprintf(stdout, "\nGame of life");
      fprintf(stdout, "\nDomínio: (%d, %d x %d, %d) Prob. %5.3f",   mLattice.width,   mLattice.height, mLattice.world, mLattice.steps, prob);
      fprintf(stdout, "\nMPI - total de processos %d\n", mLattice.world);
      fflush(stdout);
    }

    mLattice.buff0 = (unsigned char*) malloc (mLattice.width *   mLattice.height *  sizeof(unsigned char));
    mLattice.buff1 = (unsigned char*) malloc (mLattice.width *   mLattice.height *  sizeof(unsigned char));

    //Buffer de comunicação
    //A forma mais simples é comunicar a borda de cima e de baixo
    mLattice.inBuffTop  = (unsigned char*) malloc (mLattice.width * sizeof(unsigned char));
    mLattice.inBuffBot  = (unsigned char*) malloc (mLattice.width * sizeof(unsigned char));;


    InitRandness(&mLattice, prob);



    for (int t = 0; t < mLattice.steps; t++)
    {
      printf("\n>> %d ", t);
      
      GameOfLife(&mLattice);
      unsigned char *swap = mLattice.buff0;
      mLattice.buff0 = mLattice.buff1;
      mLattice.buff1 = swap;
      
    }

    if (flagSave == 1)
      print2File(&mLattice);
    free(mLattice.buff0);
    free(mLattice.buff1);
    free(mLattice.inBuffTop);
    free(mLattice.inBuffBot);

    MPI_Finalize();
    return EXIT_SUCCESS;
}

/*
 * Função utilizada para iniciar a matriz. Não mudar o valor constante do seed do rand
 */
void InitRandness(tpLattice *mLattice, float p){
  memset(mLattice->buff0, 0x00,  mLattice->width *   mLattice->height *  sizeof(unsigned char));
  memset(mLattice->buff1, 0x00,  mLattice->width *   mLattice->height *  sizeof(unsigned char));

  memset(mLattice->inBuffTop, 0x00,  mLattice->width *   sizeof(unsigned char));
  memset(mLattice->inBuffBot, 0x00,  mLattice->width *   sizeof(unsigned char));


  //cada processo inicia sua própria matrix
  srand (42*mLattice->rank);
  for (int j = 1; j < mLattice->height - 1; j++){
      for (int i = 1; i < mLattice->width - 1; i++){
          int k = j * mLattice->width  +  i;
          float r = (rand() / (float)RAND_MAX);
          //if (r <= p)
          mLattice->buff0[k] = 1;

      }//end-  for (int i = 0; i < mLattice->width; i++){
  }//end-for (int j = 0; j < mLattice->height; j++){


}//end-void InitRandness(tpLattice *mLattice, float p){

/*
 * Função que resolve o GOL. Adota-se os chamados pontos fantasmas como condição de contorno, ou seja,
 * os elementos da borda não são alterados em nenhum dos dois buffers.
 * Adota-se zero para esses pontos como valor padrão
 */
void GameOfLife(tpLattice *mLattice){
  int nw = -1, n = -1, ne = -1, w = -1, e = -1, sw = -1, s = -1, se = -1, c = -1, sum;
   
    /*
        nw | n | ne
       ----|---|----
        w  | c |  e
       ----|---|----
        sw | s | se
    */

//send-
//recv
    if ( mLattice->world > 1){
      if ((mLattice->rank > 0) && (mLattice->rank < mLattice->world - 1)){
        MPI_Send(mLattice->buff0, mLattice->width, MPI_BYTE, mLattice->rank-1, 0, MPI_COMM_WORLD);
        MPI_Send(&(mLattice->buff0[(mLattice->width * mLattice->height - 1)]), mLattice->width, MPI_BYTE, mLattice->rank+1, 0, MPI_COMM_WORLD);
        MPI_Recv(mLattice->inBuffTop, mLattice->width, MPI_BYTE, mLattice->rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(mLattice->inBuffBot, mLattice->width, MPI_BYTE, mLattice->rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        exit(EXIT_FAILURE);

      }else if (mLattice->rank == 0){
        MPI_Send(&(mLattice->buff0[(mLattice->width * mLattice->height - 1)]), mLattice->width, MPI_BYTE, mLattice->rank+1, 0, MPI_COMM_WORLD);
        MPI_Recv(mLattice->inBuffBot, mLattice->width, MPI_BYTE, mLattice->rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        exit(EXIT_FAILURE);
      
      }else if (mLattice->rank == mLattice->world - 1){
        MPI_Send(mLattice->buff0, mLattice->width, MPI_BYTE, mLattice->rank-1, 0, MPI_COMM_WORLD);
        MPI_Recv(mLattice->inBuffTop, mLattice->width, MPI_BYTE, mLattice->rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        exit(EXIT_FAILURE);

      }else{
        fprintf(stderr, "ERROR line %d\n", __LINE__);
        exit(EXIT_FAILURE);
      }

    }
    

  for (int j = 1; j < mLattice->height - 1; j++){
        for (int i = 1; i < mLattice->width - 1; i++){

          nw = mLattice->buff0[(j - 1) * mLattice->width  +  (i - 1)];
           n = mLattice->buff0[(j - 1) * mLattice->width  +  i];
          ne = mLattice->buff0[(j - 1) * mLattice->width  +  (i + 1)];
          w  = mLattice->buff0[j * mLattice->width  +  (i - 1)];
          c  = mLattice->buff0[j * mLattice->width  +  i];
          e  = mLattice->buff0[j * mLattice->width  +  (i + 1)];
          sw = mLattice->buff0[(j + 1) * mLattice->width  +  (i - 1)];
          s  = mLattice->buff0[(j + 1) * mLattice->width  +  i];
          se = mLattice->buff0[(j + 1) * mLattice->width  +  i+1];

          sum = nw + n + ne + w + e + sw + s + se;

              //mRule
              if ((sum == 3) && (c == 0))
                 mLattice->buff1[j  * mLattice->width  +  i] = 1;
              else if ((sum >= 2) && (sum <= 3) && (c == 1))
                 mLattice->buff1[j  * mLattice->width  +  i] = 1;
              else
                mLattice->buff1[j  * mLattice->width  +  i] = 0;
        }//end-  for (int i = 0; i < mLattice->width; i++){
    }//end-for (int j = 0; j < mLattice->height; j++){

}

/*
 * Função para imprimir para arquivo. Formato do arquivo .txt
 */
void print2File(tpLattice *mLattice)
{
  char filename[1024];
  sprintf(filename, "game_of_life-mpi-proc-%d.txt", mLattice->rank);
  fprintf(stdout, "\n[Save to file: %s]\n", filename);
  FILE *ptr = fopen(filename, "w+");
  assert(ptr  != NULL);

  for (int j = 1; j < mLattice->height - 1; j++){
      for (int i = 1; i < mLattice->width - 1; i++){
          int k = j * mLattice->width  +  i;
          if (mLattice->buff0[k] == 1)
            fputc('#', ptr);
          else
           fputc(' ', ptr);
      }//end-  for (int i = 0; i < mLattice->width; i++){
      fputc('\n', ptr);
  }//end-for (int j = 0; j < mLattice->height; j++){

  fclose(ptr);
  fprintf(stdout, "\t[OK]\n");

}
