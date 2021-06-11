#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define EPSILON 1E-9
/*Declarando as structs de particula e forca*/
struct stCoord{
    float x,
          y,
          z;
};
typedef struct stCoord tpCoord;
struct stParticle
{
    tpCoord p,
            v,
            f;

};
typedef struct stParticle tpParticle;

void printLog(tpParticle *particles, int nParticles, int timestep);
void initialCondition(tpParticle *particles, int nParticles);
float distance(float *dx,
               float *dy,
               float *dz,
               tpParticle A,
               tpParticle B);
void particleParticle (tpParticle *particles, int nParticles, int timesteps, float dt);


int main (int ac, char **av){
    int timesteps  = atoi(av[1]),
        nParticles = atoi(av[2]),
        flagSave = atoi(av[3]);


    float              dt =  0.01f;
    tpParticle *particles = NULL;


    fprintf(stdout, "\nParcile system particle to particle \n");
    fprintf(stdout, "Memory used %lu bytes \n", nParticles * sizeof(tpParticle));

    particles = (tpParticle *) malloc ( nParticles * sizeof(tpParticle));
    assert(particles != NULL);

    initialCondition(particles, nParticles);

    particleParticle(particles, nParticles, timesteps, dt);

    if (flagSave == 1)
      printLog(particles, nParticles, timesteps);
    free(particles);



}

void printLog(tpParticle *particles, int nParticles, int timestep){
    char fileName[128];
    sprintf(fileName, "%s-%d-log.txt", __FILE__,  timestep);
    fprintf(stdout, "Saving file [%s] ", fileName); fflush(stdout);
    FILE *ptr = fopen(fileName, "w+");
      for(int i = 0; i < nParticles; i++){
            fprintf(ptr, "%d \t %.10f %.10f %.10f \t %.10f %.10f %.10f \t %.10f %.10f %.10f \n", i,  particles[i].p.x, particles[i].p.y, particles[i].p.z,  particles[i].v.x, particles[i].v.y, particles[i].v.z, particles[i].f.x, particles[i].f.y, particles[i].f.z);

    }
    fclose(ptr);
    fprintf(stdout, "[OK]\n"); fflush(stdout);
}

void initialCondition(tpParticle *particles, int nParticles){

    srand(42);

    memset(particles, 0x00, nParticles * sizeof(tpParticle));

    for (int i = 0; i < nParticles ; i++){
        particles[i].p.x =  2.0 * (rand() / (float)RAND_MAX) - 1.0;
        particles[i].p.y =  2.0 * (rand() / (float)RAND_MAX) - 1.0;
        particles[i].p.z =  2.0 * (rand() / (float)RAND_MAX) - 1.0;
      }




}

float distance(float *dx,
               float *dy,
               float *dz,
               tpParticle A,
               tpParticle B){
    float x = A.p.x - B.p.x;
    float y = A.p.y - B.p.y;
    float z = A.p.z - B.p.z;
    *dx = x; *dy = y; *dz = z;
    x *= x; y *= y; z *= z;
    return 1.0f / sqrt( x + y + z + EPSILON);

}



void particleParticle (tpParticle *particles, int nParticles, int timesteps, float dt){

    for (int t = 0; t < timesteps; t++){
        //---------------------------------------------------
            for(int i = 0; i < nParticles; i++){
                for(int j = 0; j < nParticles; j++){
                    float dx = 0.0f, dy = 0.0f, dz = 0.0f;
                    float d  = distance(&dx, &dy, &dz, particles[i], particles[j]);
                    particles[i].f.x += dx * d;
                    particles[i].f.y += dy * d;
                    particles[i].f.z += dz * d;

                }
            }//end-for(int i = 0; i < nParticles; i++){

            for(int i = 0; i < nParticles; i++){
              particles[i].v.x += dt * particles[i].f.x;
              particles[i].v.y += dt * particles[i].f.y;
              particles[i].v.z += dt * particles[i].f.z;

              particles[i].p.x += dt * particles[i].v.x;
              particles[i].p.y += dt * particles[i].v.y;
              particles[i].p.z += dt * particles[i].v.z;

            }//end-for(int i = 0; i < nParticles; i++){

    }//end-for (int t = 0; t < timesteps; t++){

}
