#include "ExplicitSchemes.h"																					//include the header of this class


void ExplicitScheme::resultDt(const double Dt) {																//declare the only function of this class which handle the calculation and the print of the scheme
	Commons fx;																									//call the class common thought the varialbe fx
	// cout << "\nResult for dt = " << Dt << ": \n";															//print the value of the delta t

	if (fx.u*Dt / fx.dx <= 1) {																					//if the CFL is fulfilled =>
		vector<double> solution;																				//intiate a vector of double solution
		// cout << "DELTA X: " << fx.dx << "---- X TOT: " << fx.xTot << "\n";									//print the value of delta x
		cout << "Delta t: " << Dt << "\n";																		//print the value of Delta t
		solution = fx.finit();																					//affect the result of the first solution to the vector solution
		vector <double> error(solution.size());																	//create a vector of double error which has the same size of the solution
		double n = 0;																							//initate a double n at 0
		while (n <= 0.52) {																						//create loop until n <0.5
			// cout << "NUMERICAL solution for n = " << n << ":\n";												//print a title
			cout << "n = " << n << "\n";																		//print the value of n and an enter
			fx.showVector(solution);																			//print the vector of the solution
			/*
			cout << "\n";																						//print an enter
			cout << "ANALYTICAL solution for n = " << n << ":\n";												//print a title before print a solution
			fx.showVector(fx.analyticalSolution(n));															//print the vector of analytical solution
			cout << "\n";*/																						//print an enter
			for (unsigned int i = 0; i < solution.size();i++)													//create a loop with i which will be used as an index
				error[i] = solution[i] - fx.analyticalSolution(n)[i];											//set to the vector at index i the diffrence between the solution and the analytical solution
			/*
			cout << "ERROR for n = " << n << ":\n";																//print a title before print vector
			fx.showVector(error);																				//show the vector error
			*/
			cout << "Max norm of the error for n = " << n << ":\n" << fx.uniform_norm(error) << "\n";			//Print the result of the uniform norm
			cout << "Two norm of the error for n = " << n << ":\n" << fx.two_norm(error) << "\n";				//Print the result of the norm two
			/*cout << "\n";																						//print an enter
			cout << "\n";*/																						//print an enter


			solution = ExplicitScheme_nplus1(solution, Dt);														//call the function which calculate the solution at n+1
			n += Dt;																							//add delta t to n

		}
	}
	else {																										//if the CFL condition is not fulfilled =>
		cout << "The CFL condition isn't respected, CFL = " << (fx.u*Dt / fx.dx);								//print the result of the CFL condition
	}
}

