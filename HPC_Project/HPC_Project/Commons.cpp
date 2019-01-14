#include "Commons.h"													//include the header of the class


int Commons::numberOfPointsPerResult() {
	return (int)(xTot*(1 / dx));
}

int Commons::numberOfPointsPerProcessor() {
	return (int)((numberOfPointsPerResult() / getNpes()) + 1);
}



int Commons::getMyRank() {
	int myrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	return myrank;
}

int Commons::getNpes() {
	int npes;
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	return npes;
}




double Commons::finitx(double x) {										//declaration of the function f(x)
	if (x <= 50 || x >= 110)											//if x is lower than 50 or higher than 110 =>
		return 0.0;														//return 0
	return 100 * (sin(PI*(x - 50) / 60));								//if x is between 50 and 110 return the wave
}

vector<double> Commons::finit() {										//declaration of the function which create the vector with all the value of f(x)
	vector <double> res;												//decalartion of the vector of double
	double x = 0;														//initiate the variable x at 0
	while (x <= xTot) {													//while x is lower than xtot (400) =>
		res.push_back(finitx(x));										//get the value of f(x) and add it to the vector res
		x += dx;														//add dx (5) to x
	}
	return res;															//return the vector with all the value of f(x)
}

double Commons::fboundaryLeft() {										//define the function to handle the left boundary
	return 0;															//return the value 0
}

double Commons::fboundaryRight() {										//define the function to handle the right boundary
	return 0;															//return the value 0
}

double Commons::analyticalSolutionx(double t, double x) {				//declaration of the function f(x,t)
	if (x <= 50 + 250 * t || x >= 110+250*t)							//if x is lower than 50 + 250 * t or higher than 110 + 250 * t =>
		return 0;														//return 0
	return 100 * (sin(PI*(x - 50 - 250 * t) / 60));						//if x is between 50 and 110 return the wave
}

vector<double> Commons::analyticalSolution(double t) {					//declaration of the function which create the vector with all the value of f(x) analytical
	vector <double> res;												//decalartion of the vector of double
	double x = 0;														//initiate the variable x at 0
	while (x <= xTot) {													//while x is lower than xtot (400) =>
		res.push_back(analyticalSolutionx(t, x));						//get the value of f(x) analytical and add it to the vector res
		x += dx;														//add dx (5) to x
	}
	return res;															//return the vector with all the value of f(x) analytical
}

void Commons::showVector(vector <double> vect) {						//decalaration of the function which can show all the value of a vector of double
	for (unsigned int i = 0; i < vect.size(); i++) {					//create a loop with i which will be used as an index
		cout << vect[i] << "; ";										//print the value of the vector at the index i
	}
	cout << "\n";														//print a enter for more clarity
}



// NORMS
// TAKEN FROM EXERCISE CORRECTION 1 OF C++ EXERCISES

double Commons::one_norm(vector <double> vect) const{					//declaration of the function whcich return the value of norm one of a vector
	int i;																//intiate a int i
	double sum = 0;														//intiate a double sum at 0
	int n = vect.size();												//intiate a int to the value of the size of the vector

	for (i = 0; i < n; i++)												//create a loop with i which will be used as an index
		sum += fabs(vect[i]);											//add to sum the ablosute value of the vector at index i

	return sum;															//return the value of sum
}

double Commons::two_norm(vector <double> vect) const{					//declaration of the function whcich return the value of norm two of a vector
	int i;																//intiate a int i
	double sum = 0;														//intiate a double sum at 0
	int n = vect.size();												//intiate a int to the value of the size of the vector

	for (i = 0; i < n; i++)												//create a loop with i which will be used as an index
		sum += fabs(vect[i] * vect[i]);									//add to sum the square value of the vector at index i

	return (sqrt(sum));													//return the root value of sum
}

double Commons::uniform_norm(vector <double> vect) const{				//declaration of the function whcich return the value of norm two of a vector
	int i;																//intiate a int i
	double max = 0;														//intiate a double max at 0
	int n = vect.size();												//intiate a int to the value of the size of the vector

	for (i = 0; i < n; i++)												//create a loop with i which will be used as an index
		if (max < fabs(vect[i])) max = fabs(vect[i]);					//if the value of the absolute value of the vector at index i is higher than max => set max to the absolute value of the vector at index i
	return max;															//return the value of max
}