CPUC      = mpicc
CFLAGS    = -O3 -m64 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      =
INCLUDES  =
LIBDIR   = -L/usr/lib -L/opt/mpich/lib
LIBS     = -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_C  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

seq:
	$(CPU_COMPILE_C) main-gol.c  $(LINK) -o gol
mpi:
	$(CPU_COMPILE_C) main-gol-mpi.c  $(LINK) -o gol-mpi
mpi-async:
	$(CPU_COMPILE_C) main-gol-mpi-async.c  $(LINK) -o gol-mpi-async
mpi-comm-time:
	$(CPU_COMPILE_C) main-gol-mpi-comm-time.c  $(LINK) -o gol-mpi-comm-time
mpi-omp:
	$(CPU_COMPILE_C) -fopenmp main-gol-mpi-omp.c  $(LINK) -o gol-mpi-omp


clean:
	rm gol-mpi
