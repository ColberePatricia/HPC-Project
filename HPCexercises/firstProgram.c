#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int npes, myrank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	printf("From processor rank %d out of %d processors, Hello World!\n", myrank, npes);
	MPI_Finalize();
}
