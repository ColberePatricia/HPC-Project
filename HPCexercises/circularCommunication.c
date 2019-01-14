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
	
	if (myrank%2==1){
		MPI_Send(a,10,MPI_INT,(myrank+1)%npes,1,MPI_COMM_WORLD);
		MPI_Send(b,10,MPI_INT,(myrank-1+npes)%npes,1,MPI_COMM_WORLD);
	}
	else{
		MPI_Recv(a,10,MPI_INT,(myrank+1)%npes,1,MPI_COMM_WORLD, &status);
		MPI_Recv(b,10,MPI_INT,(myrank-1+npes)%npes,1,MPI_COMM_WORLD, &status);
	/*
	printf("\na = ");
	for (int i=0; i<10;i++)
		printf("%d, ", a[i]);
	printf("\nb = ");
	for (int i=0; i<10;i++)
		printf("%d, ", b[i]);
	*/
	}
	
	
	t2 = MPI_Wtime();
	printf("Elapsed time is %f\n", t2-t1);
	
	MPI_Finalize();
}
