#include "ImplicitSchemes.h"																			//include the header of the class

int ImplicitScheme::numberOfPointsPerResult(Matrix A) {
	A.getNcols();
}

int ImplicitScheme::numberOfPointsPerProcessor(Matrix A) {
	Commons fx;
	return (int)((numberOfPointsPerResult(A) / fx.getNpes()) + 1);
}

vector <double> ImplicitScheme::ThomasAlgorithmUntiln(Matrix A, vector <double> f, int lastIndex) {							//declare the function which handle the thomas algorithm
	Commons fx;
	int sizeOfA = A.getNcols();																			//declare an int x to the number of columunm of the matrix A
	vector <double> x(sizeOfA);																			//declare a vector x
	vector <double> m(sizeOfA);																			//declare a vector m
	vector <double> bPrime(sizeOfA);																	//declare a vector bPrime
	vector <double> dPrime(sizeOfA);																	//declare a vector dPrime
	//---------------
	int firstIndex = lastIndex - fx.numberOfPointsPerProcessor() + 1;

	if (lastIndex > numberOfPointsPerResult(A) - 1)
		lastIndex = numberOfPointsPerResult(A) - 1;
	//----CHECK THE 1st INDEX OF EACH PROCESSOR

	// We create vectors from the non null coefficients of the matrix
	vector <double> a(fx.numberOfPointsPerProcessor());																			//declare a vector a
	vector <double> b(fx.numberOfPointsPerProcessor());																			//declare a vector b
	vector <double> c(fx.numberOfPointsPerProcessor());																			//declare a vector c


	for (int i = firstIndex; i <= lastIndex; i++) {																	//create a loop with an index i until i reach the sizeofA
		b[i-firstIndex] = A[i-firstIndex][i-firstIndex];																					//set the vector b at the index i the value of the matrix A at [i][i]
		if (i > 0)																						//if i higher than 0 =>
			a[i-firstIndex] = A[i-firstIndex][i-firstIndex - 1];																			//set the vector a at the index i the value of the matrix A at [i][i-1]
		if (i < sizeOfA - 1)																				//if i is lower than sizeOfA - 1 =>
			c[i-firstIndex] = A[i-firstIndex][i-firstIndex + 1];																			//set the vector c at the index i the value of the matrix A at [i][i+1]
	}
	//--a[0] = 0;																							//set the first value of a to 0
	//--c[sizeOfA - 1] = 0;																					//set last value of c to 0

	//-----
	if (fx.getMyRank() == 0)
		a[0] = 0;
	//else
		//a[0] = A[i][i - 1];

	if (fx.getMyRank() == fx.getNpes()-1)
		c[sizeOfA - 1] = 0;
	//else
		//c[sizeOfA - 1] = A[i][i + 1];



	// First values of the vectors
	m[0] = 0;																							//set the first value of m to 0
	m[1] = a[1] / b[0];																					//set the second value of m to the second value of a divided by the first value of b
	bPrime[0] = b[0];																					//set the first value of bPrime to the fisrt value of b
	dPrime[0] = f[0];																					//set the first value of dPrime to the fisrt value of f

	// We calculate all values of the vectors
	for (int i = firstIndex+1; i <= lastIndex; i++) {																	//create a loop with an index i until i reach sizeOfA
		bPrime[i-firstIndex] = b[i-firstIndex] - m[i-firstIndex] * c[i-firstIndex - 1];																//set bPrime at index i
		dPrime[i-firstIndex] = f[i-firstIndex] - m[i-firstIndex] * dPrime[i-firstIndex - 1];														//set dPrime at index i
		if (i <= lastIndex - 1)																			//if i is lower than sizeOfA - 1 =>
			m[i-firstIndex + 1] = a[i-firstIndex + 1] / bPrime[i-firstIndex];															//set m at index i+1
	}

	//--- LAST last index should be sizeOfA-1

	x[lastIndex] = dPrime[lastIndex] / bPrime[lastIndex];											//set x at index sizeOfA - 1
	for (int i = lastIndex-1; i >= 0; i--)																//create a loop in reverse, the index i start at sizeOfA and decrease until reach 0
		x[i] = (dPrime[i] - c[i] * x[i + 1]) / bPrime[i];												//set x at index i

	return x;																							//return vector x
}



vector <double> ImplicitScheme::ThomasAlgorithm(Matrix A, vector <double> f) {	
	Commons fx;
	double firstValue;
	int lastIndex;
	//vector <double> x(lastIndex+1, 99);
	// CHECK THAT first LAST INDEX+1 == number of points per proc
	vector <double> x(numberOfPointsPerProcessor(A), 99);
	vector <double> finalX(numberOfPointsPerResult(A), 88);

	lastIndex = (int)(fx.getMyRank() + 1)*(fx.numberOfPointsPerProcessor()) - 1;

	if (lastIndex <= 0)
		cout << "The problem is too little for the number of processors used!\n";


	/*
	x = ThomasAlgorithmUntiln(A, f, lastIndex);
	
	MPI_Allgather(x.data(), x.size(), MPI_DOUBLE, finalX.data(), finalX.size(), MPI_DOUBLE, MPI_COMM_WORLD);
	*/

	return x;																							//return vector x
}

void ImplicitScheme::resultDt(const double Dt) {														//declare the function resultDt wich is called in the main class
	Commons fx;																							//create an object fx wich can call all the variable of the class commons

	if (fx.u*Dt / fx.dx <= 1) {																			//if the CFL condition is fulfilled =>
		vector<double> solution;																		//create a vector of double solution
		if (fx.getMyRank() == 0) {
		}
		solution = fx.finit();																			//affect the vector from the function finit in commons to solution
		vector <double> error(solution.size());															//create a vector of double error which has the same size as solution 
		double n = 0;																					//create a double n equal to 0
		while (n <= 0.52) {																				//create a loop while n is lower than 0.5 plus dx
			if (fx.getMyRank() == 0) {
				cout << "n = " << n << "\n";																//print the value of n
				fx.showVector(solution);																	//print the value of the vector solution

			}

			for (unsigned int i = 0; i < solution.size(); i++)											//create a loop with an index i until i reach the size of solution
				error[i] = solution[i] - fx.analyticalSolution(n)[i];									//set the error at index i the differrence between the solution and the analytical solution
			
			if (fx.getMyRank() == 0) {
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


/*
vector <double> ImplicitScheme::ThomasAlgorithmUntiln(Matrix A, vector <double> f, double firstValue, int lastIndex) {							//declare the function which handle the thomas algorithm
	int sizeOfA = A.getNcols();																			//declare an int x to the number of columunm of the matrix A
	vector <double> x(sizeOfA);																			//declare a vector x
	vector <double> m(sizeOfA);																			//declare a vector m
	vector <double> bPrime(sizeOfA);																	//declare a vector bPrime
	vector <double> dPrime(sizeOfA);																	//declare a vector dPrime

	// We create vectors from the non null coefficients of the matrix
	vector <double> a(sizeOfA);																			//declare a vector a
	vector <double> b(sizeOfA);																			//declare a vector b
	vector <double> c(sizeOfA);																			//declare a vector c
																										//all of this vector have the same size which is sizeofA
	for (int i = 0; i < sizeOfA; i++) {																	//create a loop with an index i until i reach the sizeofA
		b[i] = A[i][i];																					//set the vector b at the index i the value of the matrix A at [i][i]
		if (i > 0)																						//if i higher than 0 =>
			a[i] = A[i][i - 1];																			//set the vector a at the index i the value of the matrix A at [i][i-1]
		if (i < sizeOfA - 1)																				//if i is lower than sizeOfA - 1 =>
			c[i] = A[i][i + 1];																			//set the vector c at the index i the value of the matrix A at [i][i+1]
	}
	a[0] = 0;																							//set the first value of a to 0
	c[sizeOfA - 1] = 0;																					//set last value of c to 0


	// First values of the vectors
	m[0] = 0;																							//set the first value of m to 0
	m[1] = a[1] / b[0];																					//set the second value of m to the second value of a divided by the first value of b
	bPrime[0] = b[0];																					//set the first value of bPrime to the fisrt value of b
	dPrime[0] = f[0];																					//set the first value of dPrime to the fisrt value of f

	// We calculate all values of the vectors
	for (int i = 1; i < sizeOfA; i++) {																	//create a loop with an index i until i reach sizeOfA
		bPrime[i] = b[i] - m[i] * c[i - 1];																//set bPrime at index i
		dPrime[i] = f[i] - m[i] * dPrime[i - 1];														//set dPrime at index i
		if (i < sizeOfA - 1)																			//if i is lower than sizeOfA - 1 =>
			m[i + 1] = a[i + 1] / bPrime[i];															//set m at index i+1
	}

	x[sizeOfA - 1] = dPrime[sizeOfA - 1] / bPrime[sizeOfA - 1];											//set x at index sizeOfA - 1
	for (int i = sizeOfA - 2; i >= 0; i--)																//create a loop in reverse, the index i start at sizeOfA and decrease until reach 0
		x[i] = (dPrime[i] - c[i] * x[i + 1]) / bPrime[i];												//set x at index i

	return x;																							//return vector x
}
*/