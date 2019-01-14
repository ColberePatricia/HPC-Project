#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int npes, myrank, a[10], b[10];
	MPI_Init(NULL, NULL);

	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	printf("From processor rank %d out of %d processors, Hello World!\n", myrank, npes);

	if (myrank==0) {
		a={0,1,2,3,4,5,6,7,8,9};
		b={11,22,33,44,55,66,77,88,99,100};
		MPI_Send(a, 10, MPI_INT, 1, 1, MPI_COMM_WORLD);
		MPI_Send(b, 10, MPI_INT, 1, 2, MPI_COMM_WORLD);
	}
	else if (myrank == 1) {
		MPI_Recv(b, 10, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		MPI_Recv(a, 10, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	}

	MPI_Finalize();
}
