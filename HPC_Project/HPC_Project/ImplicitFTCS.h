#pragma once
#ifndef IMPLICITFTCS_H //include guard
#define IMPLICITFTCS_H
#include "ImplicitSchemes.h"

/*!
 * \file ImplicitFTCS.h
 * \brief Implicit FTCS Class
 * \class ImplicitFTCS  
 * To calculate the function f at time n + 1 with the implicit FTCS scheme
 */


class ImplicitFTCS : public ImplicitScheme
{
public:

/*!
 *  \brief Implicit FTCS function
 *
 *  Function called to calculate the function f at time n + 1
 *
 *  \param previousSolution A vector of f at time n
 *  \param Dt Delta t
 *  \return Return a vector of the solution at time n + 1 
 */
	vector<double> ImplicitScheme_nplus1(vector <double> previousSolution, double Dt);
};

#endif