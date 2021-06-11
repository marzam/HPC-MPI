CPUC      = gcc
CFLAGS    = -O3 --std=c11 -m64 	 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      =
INCLUDES  =
LIBDIR   = -L/usr/lib
LIBS     = -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_C  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

all:
	$(CPU_COMPILE_C) main-n-bodies.c  $(LINK)   -o n-bodies
	$(CPU_COMPILE_C) -fopenmp main-n-bodies-openmp.c  $(LINK)   -o n-bodies-openmp
	$(CPU_COMPILE_C) -fopenmp main-n-bodies-openmp-sem-struct.c  $(LINK)   -o n-bodies-openmp-st

clean:
	rm n-bodies
