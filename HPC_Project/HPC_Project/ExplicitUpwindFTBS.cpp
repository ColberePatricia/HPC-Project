#include "ExplicitUpwindFTBS.h"																				//include the header of the class


vector<double> ExplicitUpwindFTBS::ExplicitSchemeUntiln(vector <double> previousSolution, double Dt, int lastIndex) { //gives the result for the required indexes
	Commons fx;																								//define fx as commons to use the components of Commons class
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	vector <double> res;
	int firstIndex = lastIndex - fx.numberOfPointsPerProcessor() + 1;										//The indexes are respective to each processor
		
	
	if (lastIndex > fx.numberOfPointsPerResult() - 1)														//The index has to be within the size of the final result
		lastIndex = fx.numberOfPointsPerResult() - 1;

	if (lastIndex > previousSolution.size() - 1) {															//The index has to be within the size of the previous solution because it is a fixed size
		cout << "The last index is too big: " << lastIndex << "\n";
		cout << "The size of the previous solution is: " << previousSolution.size() << "\n";
		lastIndex = previousSolution.size() - 1;
	}


	if (firstIndex == 0) {																					//We apply the boundary condition to the very first point
		res.push_back((1 - c)*previousSolution[0] + c * fx.fboundaryLeft());
		firstIndex++;
	}

	
	for (int xIndex = firstIndex; xIndex <= lastIndex; xIndex++) {											//We calculate the rest of the solution
		res.push_back((1 - c)*previousSolution[xIndex] + c * previousSolution[xIndex - 1]);
	}
	
	while (res.size() < fx.numberOfPointsPerProcessor())													//If this is the last processor, it can have less points than the other ones, so we add useless values that will be truncated later to simplify the gather
		res.push_back(0);

	return res;																								//return the result for the required indexes
}


vector<double> ExplicitUpwindFTBS::ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) {		//gives the result for the whole solution
	Commons fx;
	vector <double> res(fx.numberOfPointsPerProcessor(), 99);
	const double c = (double)fx.u*Dt / fx.dx;																//define the value of c
	int lastIndex;

	vector <int> displs;																					//vector necessary for MPI_Gatherv
	vector <int> recv_counts;																				//vector necessary for MPI_Gatherv


	lastIndex = (int)(fx.getMyRank() + 1)*(fx.numberOfPointsPerProcessor()) - 1;							//last index of the solution calculated by each processor
	
	if (lastIndex <= 0)																						//We should use fewer processors for a little problem
		cout << "The problem is too little for the number of processors used!\n";

	res = ExplicitSchemeUntiln(previousSolution, Dt, lastIndex);											//We calculate the results for each processor


	vector <double> finalRes(res.size()*fx.getNpes(), 88);													//This will contain the result of all the processors


	for (int i = 0; i < fx.getNpes(); i++) {
		displs.push_back(i * res.size());
		recv_counts.push_back(res.size());
	}

	MPI_Allgatherv(res.data(), res.size(), MPI_DOUBLE, finalRes.data(), recv_counts.data(), displs.data(), MPI_DOUBLE, MPI_COMM_WORLD); //We gather the results
	
	if (finalRes.size() != previousSolution.size())															//We truncate the last values of the result that  were added for a simplification of the gather but are meaningless
		finalRes.resize(previousSolution.size());

	return finalRes;
}


