#include "ExplicitUpwindFTBS.h"																				//include the header of the class


vector<double> ExplicitSchemeUntiln(vector <double> previousSolution, double Dt, double firstValue, int lastIndex) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;																								//define fx as commons to use common varaible
	vector <double> res;
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	int firstIndex = lastIndex - fx.numberOfPointsPerProcessor() + 1;
	// TODO
	res.push_back(firstValue);

	if (lastIndex > fx.numberOfPointsPerResult() - 1)
		lastIndex = fx.numberOfPointsPerResult() - 1;

	for (int xIndex = firstIndex; xIndex <= lastIndex; xIndex++) {																					//create loop while x is lower than Xtot (400)
		res.push_back(((1 - c)*previousSolution[xIndex] + c * previousSolution[xIndex - 1]));				//add the value of the scheme to the vector res
		/*if (fx.getMyRank() == 1) {
			cout << "RES 0 SENT: " << res[0] << "\n";
			cout << "1st TERM: " << previousSolution[xIndex] << "\n";
			cout << "2nd TERM: " << previousSolution[xIndex - 1] << "\n";
		}*/
	}

	return res;																								//return the vector of double res
}


vector<double> ExplicitUpwindFTBS::ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;
	vector <double> res;
	vector <double> finalRes(fx.numberOfPointsPerResult(), 10);
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	double firstValue;
	int lastIndex;

	if (fx.getMyRank() != 0) { // If I'm not the first processor, I need to receive the first value
		MPI_Status status;
		MPI_Recv(&firstValue, 1, MPI_DOUBLE, fx.getMyRank() - 1, 1, MPI_COMM_WORLD, &status);
		//cout << "VALUE : " << firstValue << " RECEIVED!\n";
	}
	else
		firstValue = ((1 - c)*previousSolution[0] + c * fx.fboundaryLeft());

	lastIndex = (int)(fx.getMyRank() + 1)*(fx.numberOfPointsPerProcessor());

	res = ExplicitSchemeUntiln(previousSolution, Dt, firstValue, lastIndex);

	if (fx.getMyRank() + 1 != fx.getNpes()) { // my rank is not the last, I need to send the value of the last point to the next processor
		MPI_Send(&res.back(), 1, MPI_DOUBLE, fx.getMyRank() + 1, 1, MPI_COMM_WORLD);
		//cout << "VALUE : " << res.back() << " SENT!\n";

	}


	MPI_Allgather(res.data(), (int)(fx.numberOfPointsPerResult() / fx.getNpes()), MPI_DOUBLE, finalRes.data(), (int)(fx.numberOfPointsPerResult() / fx.getNpes()), MPI_DOUBLE, MPI_COMM_WORLD);

	return finalRes;
}



/*
vector<double> ExplicitUpwindFTBS::ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;																								//define fx as commons to use common varaible
	vector <double> res;																					//initiate a vector of double to res
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	res.push_back((1 - c)*previousSolution[0] + c * fx.fboundaryLeft());										//for the first one, the use of left boundary is require
	double x = fx.dx;																						//define a double x as dx
	int index = 1;																							//define a int index to 1
	while (x <= fx.xTot) {																					//create loop while x is lower than Xtot (400)
		res.push_back((1 - c)*previousSolution[index] + c * previousSolution[index - 1]);						//add the value of the scheme to the vector res
		index++;																							//add one to the index
		x += fx.dx;																							//add the value of delta x to x
	}
	return res;																								//return the vector of double res
}
*/
