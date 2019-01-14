#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int n;
	printf("Input the size of the array:\n");
	scanf("%d", &n);
	int a[n], npes, myrank;
	MPI_Init(NULL, NULL);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Request requests[1];
	
	for (int i=0; i<n;i++) {
		a[i]=i;
	}
	
	if (myrank==0){
		double t1 = MPI_Wtime();
		for (unsigned int i=1;i<npes-1;i++)
			MPI_Send(a,n,MPI_INT,i,1,MPI_COMM_WORLD);
		double t2 = MPI_Wtime();
		double Dt = t2-t1;
		printf("Elapsed send time is %f\n", Dt);
	} else {
		MPI_Irecv(a, n, MPI_INT, 0, 1, MPI_COMM_WORLD, &requests[0]);
		printf("Hello World!\n rank: %d, first element: %d \n", myrank, a[0]);
	}
	
	
	MPI_Finalize();
}
