#include "ImplicitSchemes.h"																			//include the header of the class

int ImplicitScheme::numberOfPointsPerResult(Matrix A) {													//gives the number of points in each result vector for each time step
	A.getNcols();
}

int ImplicitScheme::numberOfPointsPerProcessor(Matrix A) {												//gives us the number of points in each results for one processor
	Commons fx;
	return (int)((numberOfPointsPerResult(A) / fx.getNpes()) + 1);
}

vector <double> ImplicitScheme::ThomasAlgorithmUntiln(Matrix A, vector <double> f, int lastIndex) {		//thomas algorithm function to the lastIndex for numberOfPointsPerProcessor points
	Commons fx;
	int sizeOfA = A.getNcols();
	vector <double> m;																					//declare the vectors m for the algorithm
	vector <double> bPrime;
	vector <double> dPrime;
	int firstIndex = lastIndex - fx.numberOfPointsPerProcessor() + 1;

	if (lastIndex > fx.numberOfPointsPerResult() - 1)													//Check that lastIndex is not bigger than the size of the final result
		lastIndex = fx.numberOfPointsPerResult() - 1;

	if (lastIndex > sizeOfA - 1) {																		//Check that lastIndex is not bigger than the size of A
		cout << "The last index is too big: " << lastIndex << "\n";
		cout << "The size of A is: " << sizeOfA << "\n";
		lastIndex = sizeOfA - 1;
	}

	
	vector <double> a;																					// We create vectors from the non null coefficients of the matrix
	vector <double> b;
	vector <double> c;

	if (firstIndex == 0)																				//put first value of a
		a.push_back(0);

	for (int i = firstIndex; i <= lastIndex; i++) {														//Use the thomas algorithm to calculate a, b and c vectors
		b.push_back(A[i][i]);
		if (i > 0)
			a.push_back(A[i][i - 1]);
		if (i < sizeOfA - 1)
			c.push_back(A[i][i + 1]);
	}

	if (lastIndex == sizeOfA - 1)																		//put last value of c
		c.push_back(0);




	// First values of the vectors m bPrime and dPrime
	// The first value of m is never used so we can give it the value zero like m[0] of a sequential program
	m.push_back(0);
	m.push_back(a[1] / b[0]);
	bPrime.push_back(b[0]);
	dPrime.push_back(f[firstIndex]);

	int bPrimeIndex = 1;
	for (int i = firstIndex+1; i <= lastIndex; i++) {													// We calculate all values of the vectors m, bPrime and dPrime according to the thomas algorithm
		bPrime.push_back(b[bPrimeIndex] - m[bPrimeIndex] * c[bPrimeIndex - 1]);
		dPrime.push_back(f[i] - m[bPrimeIndex] * dPrime[bPrimeIndex - 1]);
		if (i <= lastIndex - 1)
			m.push_back(a[bPrimeIndex + 1] / bPrime[bPrimeIndex]);
		bPrimeIndex++;
	}


	vector <double> x(bPrime.size(), 8888);																// x will contain the result for a processor

	if (bPrime[lastIndex - firstIndex] == 0) {
		cout << "Attempting to divide by zero!!\n";
		return x;
	}
	else {
		x[lastIndex-firstIndex] = dPrime[lastIndex - firstIndex] / bPrime[lastIndex - firstIndex];		//last value of x
		for (int i = lastIndex - 1; i >= firstIndex; i--) {
			if (bPrime[i - firstIndex] == 0) {
				cout << "Attempting to divide by zero!\n";
				return x;
			}
			else
				x[i - firstIndex] = (dPrime[i - firstIndex] - c[i-firstIndex] * x[i - firstIndex + 1]) / bPrime[i - firstIndex];	//we finish calculating x
		}
	}

	while (x.size() < fx.numberOfPointsPerProcessor())													//the last processor can have fewer points than the others so we add fake values that we will truncate later
		x.push_back(0);


	return x;																							//we return the result calculated by one processor
}



vector <double> ImplicitScheme::ThomasAlgorithm(Matrix A, vector <double> f) {							//thomas algorithm that gives a result coming from all the processors
	Commons fx;
	int lastIndex;																						//lastIndex is the last index handled by one processor
	vector <double> x(numberOfPointsPerProcessor(A), 99);												//x will contain the individual results of each processor

	vector <int> displs;																				//displs and recv_counts will be used for MPI_Gatherv
	vector <int> recv_counts;

	lastIndex = (int)(fx.getMyRank() + 1)*(fx.numberOfPointsPerProcessor()) - 1;

	if (lastIndex <= 0)
		cout << "The problem is too little for the number of processors used!\n";


	x = ThomasAlgorithmUntiln(A, f, lastIndex);															//we get the individual solution of a processor

	vector <double> finalX(x.size()*fx.getNpes(), 88);													//contains the total solution from all the processors

	for (int i = 0; i < fx.getNpes(); i++) {
		displs.push_back(i * x.size());
		recv_counts.push_back(x.size());
	}
	
	MPI_Allgatherv(x.data(), x.size(), MPI_DOUBLE, finalX.data(), recv_counts.data(), displs.data(), MPI_DOUBLE, MPI_COMM_WORLD); //get all individual solution

	if (finalX.size() != numberOfPointsPerResult(A))													//We truncate the last values of the result that  were added for a simplification of the gather but are meaningless
		finalX.resize(numberOfPointsPerResult(A));

	return finalX;																						//return the actual solution
}

void ImplicitScheme::resultDt(const double Dt) {														//declare the function resultDt wich is called in the main class
	Commons fx;																							//create an object fx wich can call all the variable of the class commons

	if (fx.u*Dt / fx.dx <= 1) {																			//if the CFL condition is fulfilled =>
		vector<double> solution;																		//create a vector of double solution
		if (fx.getMyRank() == 0) {
			cout << "Delta t: " << Dt << "\n";
		}

		solution = fx.finit();																			//affect the vector from the function finit in commons to solution
		vector <double> error(solution.size());															//create a vector of double error which has the same size as solution 
		double n = 0;																					//create a double n equal to 0
		while (n <= 0.52) {																				//create a loop while n is lower than 0.5 plus dx
			if (fx.getMyRank() == 0 && (n == 0 || fabs(n - 0.1) < 0.001 || fabs(n - 0.2) < 0.001 || fabs(n - 0.3) < 0.001 || fabs(n - 0.4) < 0.001 || fabs(n - 0.5) < 0.001)) {
				cout << "n = " << n << "\n";																//print the value of n
				fx.showVector(solution);																	//print the value of the vector solution

			}
			
			for (unsigned int i = 0; i < solution.size(); i++)											//create a loop with an index i until i reach the size of solution
				error[i] = solution[i] - fx.analyticalSolution(n)[i];									//set the error at index i the differrence between the solution and the analytical solution
			
			if (fx.getMyRank() == 0 && (n == 0 || fabs(n - 0.1) < 0.001 || fabs(n - 0.2) < 0.001 || fabs(n - 0.3) < 0.001 || fabs(n - 0.4) < 0.001 || fabs(n - 0.5) < 0.001)) {
				cout << "Max norm of the error for n = " << n << ":\n" << fx.uniform_norm(error) << "\n";	//print the value of the norm unifrom apply to error
				cout << "Two norm of the error for n = " << n << ":\n" << fx.two_norm(error) << "\n";		//print the value of the norm two apply to error
			}


			solution = ImplicitScheme_nplus1(solution, Dt);												//called the function which calculate the vector for f at n+1
			n += Dt;																					//add the value of delta t to n

		}
	}
	else {																								//if the CFL condition is not fulfilled =>
		if (fx.getMyRank() == 0) {
			cout << "The CFL condition isn't respected, CFL = " << (fx.u*Dt / fx.dx);						//print the value of the CFL condition
		}
	}
}


