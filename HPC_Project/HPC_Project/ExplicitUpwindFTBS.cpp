#include "ExplicitUpwindFTBS.h"																				//include the header of the class


vector<double> ExplicitUpwindFTBS::ExplicitSchemeUntiln(vector <double> previousSolution, double Dt, int lastIndex) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;																								//define fx as commons to use common varaible
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	int resIndex = 0;
	//vector <double> res(fx.numberOfPointsPerProcessor(), 88);
	vector <double> res;
	int firstIndex = lastIndex - fx.numberOfPointsPerProcessor() + 1;
		
	
	if (lastIndex > fx.numberOfPointsPerResult() - 1)
		lastIndex = fx.numberOfPointsPerResult() - 1;

	if (lastIndex > previousSolution.size() - 1) {
		cout << "LAST INDEX TOO BIG: " << lastIndex << "\n";
		cout << "PREV SOL SIZE: " << previousSolution.size() << "\n";
		lastIndex = previousSolution.size() - 1;
	}

	//------ SHOULD BE CHANGED-------------
	//if (firstIndex > 0)
	//	firstIndex--;
	//------


	//vector <double> res(lastIndex - firstIndex + 1, 99);

	//---
	if (firstIndex == 0) {
		//res[0] = (1 - c)*previousSolution[0] + c * fx.fboundaryLeft();
		res.push_back((1 - c)*previousSolution[0] + c * fx.fboundaryLeft());
		resIndex++;
		firstIndex++;
	}

	
	for (int xIndex = firstIndex; xIndex <= lastIndex; xIndex++) {																					//create loop while x is lower than Xtot (400)
		//res[resIndex] = (1 - c)*previousSolution[xIndex] + c * previousSolution[xIndex - 1];				//add the value of the scheme to the vector res
		//res[resIndex] = 22;
		//cout << "RES INDEX IS: " << resIndex << "\n";
		//res.push_back(22);
		res.push_back((1 - c)*previousSolution[xIndex] + c * previousSolution[xIndex - 1]);
		resIndex++;
	}
	
	while (res.size() < fx.numberOfPointsPerProcessor())
		res.push_back(0);


	//if (res.size() != 401)
	//	cout << "The wrong size is from the rank: " << fx.getMyRank() << "\n";

	
	/*if (fx.getMyRank() == 0) {
		cout << "RANK 0\n";
		//cout << "FIRST INDEX: " << firstIndex << "\n";
		cout << "RES I: " << resIndex-1 << "\n";
		cout << "RES SIZE IS: " << res.size() << "\n";
		//cout << "LAST INDEX - FIRST INDEX: " << lastIndex - firstIndex << "\n";
		//cout << "SIZE PER PROC: " << fx.numberOfPointsPerProcessor() << "\n";//401
		//cout << "SIZE TOTAL: " << fx.numberOfPointsPerResult() << "\n";//800
	}

	if (fx.getMyRank() == 1) {
		cout << "RANK 1\n";
		//cout << "FIRST INDEX: " << firstIndex << "\n";
		cout << "RES I: " << resIndex-1 << "\n";
		cout << "RES SIZE IS: " << res.size() << "\n";
		//cout << "LAST INDEX - FIRST INDEX: " << lastIndex - firstIndex << "\n";
	}*/
	
	/*
	if (fx.getMyRank() == 0) {
		cout << "RANK 0\n";
		cout << "PREV SOL SIZE: " << previousSolution.size() << "\n";
	}

	if (fx.getMyRank() == 1) {
		cout << "RANK 1\n";
		cout << "PREV SOL SIZE: " << previousSolution.size() << "\n";
	}
	*/
	return res;																								//return the vector of double res
}


vector<double> ExplicitUpwindFTBS::ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//declaration of the only function of the class which return a vector of the value of f at n+1
	Commons fx;
	vector <double> res(fx.numberOfPointsPerProcessor(), 99);
	//vector <double> finalRes(fx.numberOfPointsPerProcessor()*fx.getNpes(), 88);
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	int lastIndex;

	/*vector <int> displs(fx.getNpes());
	vector <int> recv_counts(fx.getNpes());*/
	vector <int> displs;
	vector <int> recv_counts;



	/*if (fx.getMyRank() != 0) { // If I'm not the first processor, I need to receive the first value
		MPI_Status status;
		MPI_Recv(&firstValue, 1, MPI_DOUBLE, fx.getMyRank() - 1, 1, MPI_COMM_WORLD, &status);
		//cout << "VALUE : " << firstValue << " RECEIVED!\n";
	}
	else
		firstValue = ((1 - c)*previousSolution[0] + c * fx.fboundaryLeft());*/

	lastIndex = (int)(fx.getMyRank() + 1)*(fx.numberOfPointsPerProcessor()) - 1;
	
	if (lastIndex <= 0)
		cout << "The problem is too little for the number of processors used!\n";

	/*if (fx.getMyRank() == 0) {
		cout << "RANK 0\n";
		cout << "SIZE BEFORE: " << res.size() << "\n";
	}

	if (fx.getMyRank() == 1) {
		cout << "RANK 1\n";
		cout << "SIZE BEFORE: " << res.size() << "\n";
	}*/
	res = ExplicitSchemeUntiln(previousSolution, Dt, lastIndex);

	/*if (fx.getMyRank() == 0) {
		cout << "RANK 0\n";
		cout << "SIZE AFTER: " << res.size() << "\n";
	}

	if (fx.getMyRank() == 1) {
		cout << "RANK 1\n";
		cout << "SIZE AFTER: " << res.size() << "\n";
	}*/



	/*if (fx.getMyRank() + 1 != fx.getNpes()) { // my rank is not the last, I need to send the value of the last point to the next processor
		MPI_Send(&res[(res.size() - 2)], 1, MPI_DOUBLE, fx.getMyRank() + 1, 1, MPI_COMM_WORLD);
		if (fx.getMyRank() == 0 && res[(res.size() - 2)] != 0)
			cout << "SENT VALUE: " << res[(res.size() - 2)] << "\n";
		// MINUS 2 OR 1 ???

	}*/


	/*MPI_Status status;
	if (fx.getMyRank() == 0) {
		MPI_Send(&res[(res.size() - 1)], 1, MPI_DOUBLE, fx.getMyRank() + 1, 1, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&firstValue, 1, MPI_DOUBLE, fx.getMyRank() - 1, 1, MPI_COMM_WORLD, &status);
	}*/

	vector <double> finalRes(res.size()*fx.getNpes(), 88);

	//cout << "RES SIZE IN GATHER: " << res.size()<<"\n";
	//cout << "FINAL RES SIZE IN GATHER: " << finalRes.size() << "\n";
	//MPI_Allgather(res.data(), res.size(), MPI_DOUBLE, finalRes.data(), finalRes.size(), MPI_DOUBLE, MPI_COMM_WORLD);

	for (int i = 0; i < fx.getNpes(); i++) {
		/*displs[i] = i * fx.numberOfPointsPerProcessor() * fx.getNpes();
		recv_counts[i] = res.size();*/
		displs.push_back(i * res.size());
		recv_counts.push_back(res.size());
	}

	MPI_Allgatherv(res.data(), res.size(), MPI_DOUBLE, finalRes.data(), recv_counts.data(), displs.data(), MPI_DOUBLE, MPI_COMM_WORLD);
	
	if (finalRes.size() != previousSolution.size())
		finalRes.resize(previousSolution.size());
	//cout << "FINAL RES SIZE SENT: " << finalRes.size() << "\n";

	return finalRes;
	//return res;
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
