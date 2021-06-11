CPUC      = mpicc
CFLAGS    = -O3 --std=c11 -m64 	 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      =
INCLUDES  =
LIBDIR   = -L/usr/lib
LIBS     = -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_C  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

all:
	$(CPU_COMPILE_C) main-jacobi-mpi.c  $(LINK)   -o jacobi-mpi

clean:
	rm jacobi-mpi
