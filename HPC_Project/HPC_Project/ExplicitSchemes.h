#pragma once
#include "Commons.h"

/*!
 * \file ExplicitScheme.h
 * \brief Explicit Scheme Class
 * \class ExplicitScheme
 * Make the interface between the both explicit scheme
 */

class ExplicitScheme
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
	virtual vector<double> ExplicitScheme_nplus1(vector <double> previousSolution, double Dt) = 0;

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