#pragma once
#include "Commons.h"
#include "matrix.h"

/*!
 * \file ImplicitScheme.h
 * \brief Implicit Scheme Class
 * \class ImplicitScheme
 * Make the interface between the both implicit scheme
 */

class ImplicitScheme
{
public:

/*!
 *  \brief Implement other class function
 *
 *  Function called to calculate the function f at time n + 1 in the selected scheme
 *
 *  \param previousSolution A vector of f at time n
 *  \param Dt Delta t
 *  \return Return a vector of the solution at time n + 1
 */

	virtual vector<double> ImplicitScheme_nplus1(vector <double> previousSolution, double Dt) = 0;

/*!
 *  \brief Handle the Thomas algorithm
 *
 *  Function called to calculate the Thomas algorithm
 *
 *  \param A The matrix A
 *  \param f The vector f
 *  \return Return a vector of the solution at time n + 1
 */

	int numberOfPointsPerResult(Matrix A);
	int numberOfPointsPerProcessor(Matrix A);

	vector <double> ThomasAlgorithmUntiln(Matrix A, vector <double> f, double firstValue, int lastIndex);
	vector <double> ThomasAlgorithm(Matrix A, vector <double> f);

/*!
 *  \brief Handler for both scheme
 *
 *  This function call the correct scheme, print this result, calculate the error and the norm for all necesary time
 *
 *  \param Dt Delta t
 *  \return Nothing but it prompt all the information in the terminal
 */
	void resultDt(const double Dt);
};