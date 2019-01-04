#!/bin/bash
module load intel
make
mpirun -n 1 ./outputHPC


# qsub mpi.sub
# qstat -a
# icpc *.cpp
