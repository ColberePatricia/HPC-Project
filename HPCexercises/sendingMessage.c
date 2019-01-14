#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);
	int a[10], b[10], myrank;
	MPI_Status status;
	
	for (int i=0; i<10;i++) {
		a[i]=i;
		b[i]=i*10;
	}
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	if (myrank==0){
		MPI_Send(a,10,MPI_INT,1,1,MPI_COMM_WORLD);
		MPI_Send(b,10,MPI_INT,1,2,MPI_COMM_WORLD);
	}
	else if (myrank==1){
		MPI_Recv(a,10,MPI_INT,0,1,MPI_COMM_WORLD, &status);
		MPI_Recv(b,10,MPI_INT,0,2,MPI_COMM_WORLD, &status);
	}
	
	printf("a = ");
	for (int i=0; i<10;i++)
		printf("%d, ", a[i]);
	printf("b = ");
	for (int i=0; i<10;i++)
		printf("%d, ", b[i]);
	MPI_Finalize();
}
