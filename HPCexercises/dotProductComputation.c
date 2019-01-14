#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void readVector(char *s, double v[], int n){
	/*
	printf("%s",s);
	printf("\n");
	*/
	for (unsigned int i=0;i<n;i++){
		scanf("%lf", &v[i]);
	}
}

double dotProduct(double x[], double y[], int n){
	double sum = 0.0;
	for (unsigned int i=0; i<n; i++) {
		sum += x[i] * y[i];
	}
	return sum;
}

int main(int argc, char** argv) {
	int MAX_ORDER, i, n, npes, myrank, index;
	MAX_ORDER=20000;
	double dot, x[MAX_ORDER], y[MAX_ORDER], xStar[MAX_ORDER], yStar[MAX_ORDER], dotProductResult, t1, t2, Dt, Dtmax;
	MPI_Init(&argc, &argv);
	
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	
	t1 = MPI_Wtime();
	/*
	printf("Enter the vector size\n");
	*/
	if (myrank==0) {
		scanf("%d", &n);
		readVector("The first vector",x,n);
		readVector("The second vector", y, n);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(x, (int)(n/npes), MPI_DOUBLE, xStar, (int)(n/npes), MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(y, (int)(n/npes), MPI_DOUBLE, yStar, (int)(n/npes), MPI_DOUBLE, 0, MPI_COMM_WORLD);
	/*
	MPI_Bcast(&x, MAX_ORDER, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&y, MAX_ORDER, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	*/
		
	//dot = dotProduct(x,y,n);
	index = 0;
	dot = 0;
	if (npes == 1) {
		for (unsigned int i=0;i<1000000;i++){
			dot = dotProduct(xStar,yStar,n);
		}
	}
	else {
		for (unsigned int i=0;i<1000000;i++){
			dot = dotProduct(xStar,yStar,(int)(n/npes));
		}
	}
	
	MPI_Reduce(&dot, &dotProductResult, 1, MPI_DOUBLE, MPI_SUM, npes-1, MPI_COMM_WORLD);
	
	t2 = MPI_Wtime();
	Dt = t2-t1;
	MPI_Reduce(&Dt, &Dtmax, 1, MPI_DOUBLE, MPI_MAX,npes-1,  MPI_COMM_WORLD);

	if (myrank==npes-1){
		printf("Max user + system time = %lf\n", (double)Dtmax);
		printf("The dot product is %lf\n", dotProductResult);
	}
	
	MPI_Finalize();
}
