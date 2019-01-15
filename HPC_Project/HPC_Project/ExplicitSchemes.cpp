#include "ExplicitSchemes.h"																					//include the header of this class


void ExplicitScheme::resultDt(const double Dt) {																//declare the only function of this class which handle the calculation and the print of the scheme
	Commons fx;																									//call the class common thought the varialbe fx

	if (fx.u*Dt / fx.dx <= 1) {																					//if the CFL is fulfilled =>
		vector<double> solution;																				//intiate a vector of double solution
		if (fx.getMyRank() == 0) {
			cout << "Delta t: " << Dt << "\n";
		}

		solution = fx.finit();																					//affect the result of the first solution to the vector solution
		vector <double> error(solution.size());																	//create a vector of double error which has the same size of the solution
		double n = 0;																							//initate a double n at 0
		while (n <= 0.52) {																						//create loop until n <0.5
			
			/*if (fx.getMyRank() == 0) {
				cout << "n = " << n << "\n";
				fx.showVector(solution); // Show the numerical solution
			}*/
					
			
			
			for (unsigned int i = 0; i < solution.size();i++)													//create a loop with i which will be used as an index
				error[i] = solution[i] - fx.analyticalSolution(n)[i];											//set to the vector at index i the diffrence between the solution and the analytical solution
			
			if (fx.getMyRank() == 0) {
				cout << "Max norm of the error for n = " << n << ":\n" << fx.uniform_norm(error) << "\n";			//Print the result of the uniform norm
				cout << "Two norm of the error for n = " << n << ":\n" << fx.two_norm(error) << "\n";				//Print the result of the norm two
			}
			


			solution = ExplicitScheme_nplus1(solution, Dt);														//call the function which calculate the solution at n+1
			n += Dt;																							//add delta t to n

		}
	}
	else {																										//if the CFL condition is not fulfilled =>
		if (fx.getMyRank() == 0) {
			cout << "The CFL condition isn't respected, CFL = " << (fx.u*Dt / fx.dx);								//print the result of the CFL condition
		}
	}
}

