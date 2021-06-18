CPUCC       = mpic++ #/opt/gnu-gcc/bin/g++
CPPFLAGS    = --std=c++11 -m64 -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS        = $(APPLICATION)  $(VERSION) -DALIGN=64

INCLUDES  =
LIBDIR   =
#LIBDIR      = -L/opt/gnu-gcc/lib64
LIBS     =

LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

all: matrix-mpi-01 file-mpi

file-mpi:
	$(CPU_COMPILE) 03-File.cpp $(LINK) -o file-mpi

matrix-mpi-01:
	$(CPU_COMPILE) 03-Matrix.cpp $(LINK) -o matrix-mpi-01

clean:
	rm file-mpi matrix-mpi-01 *.txt
