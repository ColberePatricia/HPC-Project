#include "ImplicitSchemes.h"																			//include the header of the class


vector <double> ImplicitScheme::ThomasAlgorithm(Matrix A, vector <double> f) {							//declare the function which handle the thomas algorithm
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
		if (i < sizeOfA-1)																				//if i is lower than sizeOfA - 1 =>
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

