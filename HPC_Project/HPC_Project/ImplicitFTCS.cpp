#include "ImplicitFTCS.h"																			//include the header of the class



vector<double> ImplicitFTCS::ImplicitScheme_nplus1(vector <double> previousSolution, double Dt) {	//declaration of the function of the Implicit FTCS
	//declaration of all the variable
	Commons fx;																						//create a variable of common to use all the common variable
	vector <double> resX;																			//create a vector of double resX
	Matrix A(previousSolution.size(), previousSolution.size());										//create a matrix which as the same size of the last solution
	double c = fx.u * Dt / fx.dx;																	//define the c variable

	//create the matrix A

	for (unsigned int i = 0; i < previousSolution.size(); i++) {									//create a loop to go trought all the column
		for (unsigned int j = 0;j < previousSolution.size();j++) {									//create a loop to go trought all the row
			if (i == j)																				//if we are on the diagonal =>
				A[i][j] = 1;																		//set the value to 1
			else if (j == i - 1)																	//if we are on the lower diagonal =>
				A[i][j] = -c / 2;																	//set the value to -c/2
			else if (j == i + 1)																	//if we are on the higher diagonal =>
				A[i][j] = c / 2;																	//set the value to c/2
			else																					//in every other case =>
				A[i][j] = 0;																		//set the value to 0
		}
	}

	resX = ThomasAlgorithm(A, previousSolution);													//call the Thomas algorithm in the Implicit Scheme class with the A Matrix and the previous solution

	return resX;																					//return the value of resX
}

