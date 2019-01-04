#include "ExplicitUpwindFTBS.h"																				//include the header of the class


vector<double> ExplicitUpwindFTBS::ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;																								//define fx as commons to use common varaible
	vector <double> res;																					//initiate a vector of double to res
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	res.push_back((1 - c)*previousSolution[0] + c*fx.fboundaryLeft());										//for the first one, the use of left boundary is require
	double x = fx.dx;																						//define a double x as dx
	int index = 1;																							//define a int index to 1
	while (x <= fx.xTot) {																					//create loop while x is lower than Xtot (400)
		res.push_back((1 - c)*previousSolution[index] + c*previousSolution[index - 1]);						//add the value of the scheme to the vector res
		index++;																							//add one to the index
		x += fx.dx;																							//add the value of delta x to x
	}
	return res;																								//return the vector of double res
}



