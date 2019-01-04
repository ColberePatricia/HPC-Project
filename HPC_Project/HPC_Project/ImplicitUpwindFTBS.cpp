#include "ImplicitUpwindFTBS.h"																				//include the header of the class



vector<double> ImplicitUpwindFTBS::ImplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//declaration of the function of the Implicit Upwind FTBS
	Commons fx;																								//create a variable of common to use all the common variable
	vector <double> resX;																					//create a vector of double resX
	Matrix A(previousSolution.size(), previousSolution.size());												//create a matrix which as the same size of the last solution
	double c = fx.u * Dt / fx.dx;																			//define the c variable

	for (unsigned int i = 0; i < previousSolution.size(); i++) {											//create a loop to go trought all the column
		for (unsigned int j = 0;j < previousSolution.size();j++) {											//create a loop to go trought all the row
			if (i == j)																						//if we are on the diagonal =>
				A[i][j] = 1 + c;																			//set the value to 1+c
			else if (j == i - 1)																			//if we are on the lower diagonal =>
				A[i][j] = -c;																				//set the value to -c
			else																							//in every other case =>
				A[i][j] = 0;																				//set the value to 0
		}
	}

	resX = ThomasAlgorithm(A, previousSolution);															//call the Thomas algorithm in the Implicit Scheme class with the A Matrix and the previous solution

	return resX;																							//return the value of resX
}

