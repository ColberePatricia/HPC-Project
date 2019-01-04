#pragma once
#ifndef EXPLICITEUPWINDFTBS_H //include guard
#define EXPLICITEUPWINDFTBS_H
#include "ExplicitSchemes.h"

/*!
 * \file ExplicitUpwindFTBS.h
 * \brief Explicit Upwind FTBS Class
 * \class ExplicitUpwindFTBS
 * To calculate the function f at time n + 1 with the implicit upwind FTBS scheme
 */

class ExplicitUpwindFTBS : public ExplicitScheme
{
public:

/*!
 *  \brief Explicit Upwind FTBS function
 *
 *  Function called to calculate the function f at time n + 1
 *
 *  \param previousSolution A vector of f at time n
 *  \param Dt Delta t
 *  \return Return a vector of the solution at time n + 1
 */

	vector<double> ExplicitScheme_nplus1(vector <double> previousSolution, double Dt);

private:

};

#endif