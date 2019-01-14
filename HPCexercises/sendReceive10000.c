#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);
	int a[10], b[10], npes, myrank;
	MPI_Status status;
	double t1, t2;
	t1 = MPI_Wtime();
	
	for (int i=0; i<10;i++) {
		a[i]=i;
		b[i]=i*10;
	}
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	for(unsigned int i=0;i<10000;i++){
		MPI_Sendrecv(a,10,MPI_INT, (myrank+1)%npes, 1, b, 10, MPI_INT,(myrank-1+npes)%npes,1,MPI_COMM_WORLD, &status);
	
		/*
		if (myrank == 1) {
			printf("\na = ");
			for (int i=0; i<10;i++)
				printf("%d, ", a[i]);
			printf("\nb = ");
			for (int i=0; i<10;i++)
				printf("%d, ", b[i]);
		}
		*/
	}
	
	t2 = MPI_Wtime();
	double Dt = t2-t1;
	double Dtmax;
	MPI_Reduce(&Dt, &Dtmax, 1, MPI_DOUBLE, MPI_MAX, npes-1, MPI_COMM_WORLD);
	
	if (myrank==npes-1)
		printf("Elapsed time is %f\n", Dtmax);
	
	MPI_Finalize();
}
