CPUCC       = mpic++ #/opt/gnu-gcc/bin/g++
CPPFLAGS    = --std=c++11 -m64 -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS        = $(APPLICATION)  $(VERSION) -DALIGN=64

INCLUDES  =
LIBDIR   =
#LIBDIR      = -L/opt/gnu-gcc/lib64
LIBS     =

LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)


all: put get accumulate

put:
	$(CPU_COMPILE) 04-RMA-PUT.cpp $(LINK) -o rma-put

get:
	$(CPU_COMPILE) 04-RMA-GET.cpp $(LINK) -o rma-get

accumulate:
	$(CPU_COMPILE) 04-RMA-ACCUMULATE.cpp $(LINK) -o rma-accumulate

clean:
	rm rma-put rma-get rma-accumulate
