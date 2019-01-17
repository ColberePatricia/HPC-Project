#pragma once
#ifndef IMPLICITUPWINDFTBS_H //include guard
#define IMPLICITUPWINDFTBS_H
#include "ImplicitSchemes.h"

/*!
 * \file ImplicitUpwindFTBS.h
 * \brief Implicit FTBS Class
 * \class ImplicitUpwindFTBS
 * To calculate the function f at time n + 1 with the implicit upwind FTBS scheme
 */

class ImplicitUpwindFTBS : public ImplicitScheme
{
public:

/*!
 *  \brief Implicit Upwind FTBS function
 *
 *  Function called to calculate the function f at time n + 1
 *
 *  \param previousSolution A vector of f at time n
 *  \param Dt Delta t
 *  \return Return a vector of the solution at time n + 1
 */

	vector<double> ImplicitScheme_nplus1(vector <double> previousSolution, double Dt);

private:

};

#endif