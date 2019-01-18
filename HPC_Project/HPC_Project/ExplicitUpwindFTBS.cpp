#include "ExplicitUpwindFTBS.h"																				//include the header of the class


vector<double> ExplicitUpwindFTBS::ExplicitSchemeUntiln(vector <double> previousSolution, double Dt, int lastIndex) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;																								//define fx as commons to use common varaible
	vector <double> res(fx.numberOfPointsPerProcessor()+1, 99);
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	
	int firstIndex = lastIndex - fx.numberOfPointsPerProcessor() + 1;
	
	//------------
	if (fx.getMyRank() == 0) {
		cout << "THIS FIRST INDEX SHOULD BE ZERO: " << firstIndex << "\n";
	}

	//---CHANGING TO IF MY RANK == 0
	//---if (firstIndex == 0)
	//res.push_back(((1 - c)*previousSolution[0] + c * fx.fboundaryLeft()));
	res[0] = (((1 - c)*previousSolution[0] + c * fx.fboundaryLeft()));

	if (lastIndex > fx.numberOfPointsPerResult() - 1)
		lastIndex = fx.numberOfPointsPerResult() - 1;

	//------ SHOULD BE CHANGED
	if (firstIndex > 0)
		firstIndex--;
	//------

	int resIndex = 1;
	for (int xIndex = firstIndex; xIndex <= lastIndex; xIndex++) {																					//create loop while x is lower than Xtot (400)
		res[resIndex] = (((1 - c)*previousSolution[xIndex] + c * previousSolution[xIndex - 1]));				//add the value of the scheme to the vector res
		resIndex++;

		/*if (fx.getMyRank() == 1) { // && (previousSolution[xIndex]!=0|| previousSolution[xIndex - 1]!=0)
			cout << res.back() << "; ";
		}*/
	}


	/*cout << "\n";

	if (fx.getMyRank() == 1) {
		cout << "RANK 1 PREVIOUS SOLUTION\n";
		fx.showVector(previousSolution);
		cout << "NEXT RESULT\n";
		fx.showVector(res);
		//cout << "last Index: " << lastIndex << "\n";
		//cout << "1st Index: " << firstIndex << "\n";
	}*/

	return res;																								//return the vector of double res
}


vector<double> ExplicitUpwindFTBS::ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;
	vector <double> res(fx.numberOfPointsPerProcessor()+1, 99);
	// CHANGEEEE ????
	vector <double> finalRes(fx.numberOfPointsPerResult()+5, 88);
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	int lastIndex;
	vector <bool> allProcessorsFinished(fx.getNpes(), false);

	/*if (fx.getMyRank() != 0) { // If I'm not the first processor, I need to receive the first value
		MPI_Status status;
		MPI_Recv(&firstValue, 1, MPI_DOUBLE, fx.getMyRank() - 1, 1, MPI_COMM_WORLD, &status);
		//cout << "VALUE : " << firstValue << " RECEIVED!\n";
	}
	else
		firstValue = ((1 - c)*previousSolution[0] + c * fx.fboundaryLeft());*/

	lastIndex = (int)(fx.getMyRank() + 1)*(fx.numberOfPointsPerProcessor());

	res = ExplicitSchemeUntiln(previousSolution, Dt, lastIndex);

	if (fx.getMyRank() + 1 != fx.getNpes()) { // my rank is not the last, I need to send the value of the last point to the next processor
		/*MPI_Send(&res[(res.size() - 2)], 1, MPI_DOUBLE, fx.getMyRank() + 1, 1, MPI_COMM_WORLD);*/
		/*if (fx.getMyRank() == 0 && res[(res.size() - 2)] != 0)
			cout << "SENT VALUE: " << res[(res.size() - 2)] << "\n";*/
		// MINUS 2 OR 1 ???

	}

	for (int i = 0;i < fx.getNpes();i++)
		if (fx.getMyRank() == i)
			allProcessorsFinished[i] = true;

	/*MPI_Status status;
	if (fx.getMyRank() == 0) {
		MPI_Send(&res[(res.size() - 1)], 1, MPI_DOUBLE, fx.getMyRank() + 1, 1, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&firstValue, 1, MPI_DOUBLE, fx.getMyRank() - 1, 1, MPI_COMM_WORLD, &status);
	}*/

	// ???????????????????????????????????????????????
	MPI_Allgather(res.data(), res.size(), MPI_DOUBLE, finalRes.data(), finalRes.size(), MPI_DOUBLE, MPI_COMM_WORLD);

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
