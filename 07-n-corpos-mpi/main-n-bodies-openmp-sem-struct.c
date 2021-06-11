#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define EPSILON 1E-9
#define ALING 64
void printLog(float *px, float *py, float *pz,
              float *vx, float *vy, float *vz,
              float *fx, float *fy, float *fz, int nParticles, int timestep);

void initialCondition(float *px, float *py, float *pz,
                      float *vx, float *vy, float *vz,
                      float *fx, float *fy, float *fz, int nParticles);

float distance(float *dx,
               float *dy,
               float *dz,
               const float A_px,
               const float A_py,
               const float A_pz,
               const float B_px,
               const float B_py,
               const float B_pz);

float distance(float *dx,
              float *dy,
              float *dz,
              const float A_px,
              const float A_py,
              const float A_pz,
              const float B_px,
              const float B_py,
              const float B_pz);

void particleParticle (float *px, float *py, float *pz,
                       float *vx, float *vy, float *vz,
                       float *fx, float *fy, float *fz,
                       int nParticles, int timesteps, float dt);


int main (int ac, char **av){
    int timesteps  = atoi(av[1]),
        nParticles = atoi(av[2]),
        flagSave = atoi(av[3]);

    float      dt =  0.01f,
               *px = NULL,
               *py = NULL,
               *pz = NULL,
               *vx = NULL,
               *vy = NULL,
               *vz = NULL,
               *fx = NULL,
               *fy = NULL,
               *fz = NULL;

    fprintf(stdout, "\nParcile system particle to particle \n");
    fprintf(stdout, "Memory used %lu bytes \n", nParticles * sizeof(float) * 9);

    px = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(px != NULL);
    py = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(py != NULL);
    pz = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(pz != NULL);
//-------------------------
    vx = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(vx != NULL);
    vy = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(vy != NULL);
    vz = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(vz != NULL);

//-------------------------
    fx = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(fx != NULL);
    fy = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(fy != NULL);
    fz = (float *) aligned_alloc(ALING, nParticles * sizeof(float));
    assert(fz != NULL);
//-------------------------

    initialCondition(px, py, pz,
                     vx, vy, vz,
                     fx, fy, fz, nParticles);


  //  printLog(px, py, pz, vx, vy, vz, fx, fy, fz,  nParticles, 0);
//    START_STOPWATCH(stopwatch);
    particleParticle(px, py, pz, vx, vy, vz, fx, fy, fz, nParticles, timesteps, dt);
//    STOP_STOPWATCH(stopwatch);
  //  printLog(px, py, pz, vx, vy, vz, fx, fy, fz,  nParticles, timesteps);
    if (flagSave == 1)
      printLog(px, py, pz, vx, vy, vz, fx, fy, fz,  nParticles, timesteps);

    //fprintf(stdout, "Elapsed time: %lf \n", stopwatch.mElapsedTime);

    free(px);free(py); free(pz);
    free(vx);free(vy); free(vz);
    free(fx);free(fy); free(fz);



}

void printLog(float *px, float *py, float *pz,
              float *vx, float *vy, float *vz,
              float *fx, float *fy, float *fz, int nParticles, int timestep){
    char fileName[128];
    sprintf(fileName, "%s-%d-log.txt", __FILE__,  timestep);
    fprintf(stdout, "Saving file [%s] ", fileName); fflush(stdout);
    FILE *ptr = fopen(fileName, "w+");
      for(int i = 0; i < nParticles; i++){
            fprintf(ptr, "%d \t %.10f %.10f %.10f \t %.10f %.10f %.10f \t %.10f %.10f %.10f \n", i,  px[i], py[i], pz[i], vx[i], vy[i], vz[i], fx[i], fy[i], fz[i]);

    }
    fclose(ptr);
    fprintf(stdout, "[OK]\n"); fflush(stdout);
}


void initialCondition(float *px, float *py, float *pz,
                      float *vx, float *vy, float *vz,
                      float *fx, float *fy, float *fz, int nParticles){

    srand(42);
    memset(vx, 0x00, nParticles * sizeof(float));
    memset(vy, 0x00, nParticles * sizeof(float));
    memset(vz, 0x00, nParticles * sizeof(float));

    memset(fx, 0x00, nParticles * sizeof(float));
    memset(fy, 0x00, nParticles * sizeof(float));
    memset(fz, 0x00, nParticles * sizeof(float));

    for (int i = 0; i < nParticles ; i++){
        px[i] =  2.0 * (rand() / (float)RAND_MAX) - 1.0;
        py[i] =  2.0 * (rand() / (float)RAND_MAX) - 1.0;
        pz[i] =  2.0 * (rand() / (float)RAND_MAX) - 1.0;
    }//end-for (int i = 0; i < nParticles ; i++){

}

float distance(float *dx,
               float *dy,
               float *dz,
               const float A_px,
               const float A_py,
               const float A_pz,
               const float B_px,
               const float B_py,
               const float B_pz){
    float x = A_px - B_px;
    float y = A_py - B_py;
    float z = A_pz - B_pz;
    *dx = x; *dy = y; *dz = z;
    x *= x; y *= y; z *= z;
    return 1.0f / sqrt( x + y + z + EPSILON);


}



void particleParticle (float *px, float *py, float *pz,
                       float *vx, float *vy, float *vz,
                       float *fx, float *fy, float *fz,
                       int nParticles, int timesteps, float dt){
//omp_set_num_threads(6);
#pragma omp parallel shared(px, py, pz, vx, vy, vz, fx, fy, fz, timesteps, nParticles, dt)
{

    for (int t = 0; t < timesteps; t++){
        //---------------------------------------------------
            #pragma omp for
            for(int i = 0; i < nParticles; i++){
                for(int j = 0; j < nParticles; j++){
                    float dx = 0.0f;
                    float dy = 0.0f;
                    float dz = 0.0f;
                    float d  = distance(&dx, &dy, &dz, px[i], py[i], pz[i],
                                                       px[j], py[j], pz[j]);
                    fx[i] += dx * d;
                    fy[i] += dy * d;
                    fz[i] += dz * d;
                }
            }
        ////---------------------------------------------------
            #pragma omp barrier
            #pragma omp for
            for(int i = 0; i < nParticles; i++){
              vx[i] += dt * fx[i];
              vy[i] += dt * fy[i];
              vz[i] += dt * fz[i];

              px[i] += vx[i] * dt;
              py[i] += vy[i] * dt;
              pz[i] += vz[i] * dt;
            }
            #pragma omp barrier

    }//end-for (int t = 0; t < timesteps; t++){
}

}//end-void particleParticle
