#pragma once
#ifndef COMMONS_H //include guard
#define COMMONS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <fstream>
#include <mpi.h>
using namespace std;


/*!
 * \file Commons.h
 * \brief Commons Class
 * \class Commons
 * All the tool and the variable generic to all the scheme
 */

class Commons {
public:
	const double PI = atan(1.0) * 4;
	const double u = 250;
	const double xTot = 400;
	const double dx = 0.5;
	const double dta = 0.002;
	const double dtb = 0.001;
	const double dtc = 0.0005;

	int numberOfPointsPerResult();
	int numberOfPointsPerProcessor();


	int getMyRank();
	int getNpes();



/*!
 *  \brief function f(x)
 *
 *  Return the value of f(x)
 *
 *  \param x Value of x
 *  \return Return a double, the value of f(x)
 */
	double finitx(double x);
	/*!
 *  \brief Create the vector f(x)
 *
 *  Get the value of f(x) for each x and put it in the return vector
 *
 *  \return Return a vector of every f(x)
 */
	vector<double> finit();
	/*!
 *  \brief Handle the left boundary
 *
 *  Handler for the left boundary
 *
 *  \return Return the value for f(0)
 */
	double fboundaryLeft();
	/*!
 *  \brief Handle the right boundary
 *
 *  Handler for the right boundary
 *
 *  \return Return the value for f(400)
 */
	double fboundaryRight();
	/*!
 *  \brief Function f(x,t)
 *
 *  Return the value of f(x,t)
 *
 *  \param t the value of t
 *  \param x the value of x
 *  \return Return the value of f(x,t)
 */
	double analyticalSolutionx(double t, double x);
	/*!
 *  \brief Create the vector f(x,t) for the given time
 *
 *  Get the value of f(x) for each x at time t and put it in the return vector
 *
 *  \param t the value of t
 *  \return Return a vector of every f(x,t)
 */
	vector<double> analyticalSolution(double t);
	/*!
 *  \brief Print the vector given
 *
 *  Print every composant of the vector
 *
 *  \param vect A vector to print
 */
	void showVector(vector <double> vect);


	// THREE NORMS

	/*!
 *  \brief Norm one
 *
 *  Calculate the norm one
 *
 *  \param vect A vector
 *  \return Return the value of the norm one
 */
	double one_norm(vector <double> vect) const;
	/*!
 *  \brief Norm two
 *
 *  Calculate the norm two
 *
 *  \param vect A vector
 *  \return Return the value of the norm two
 */
	double two_norm(vector <double> vect) const;
	/*!
 *  \brief Uniform norm
 *
 *  Calculate the uniform norm
 *
 *  \param vect A vector
 *  \return Return the value of the uniform norm
 */
	double uniform_norm(vector <double> vect) const;
};

#endif 
