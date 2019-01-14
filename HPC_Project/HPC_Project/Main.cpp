#include "Commons.h"																										//include the common class
#include "ExplicitUpwindFTBS.h"																								//include the four scheme class
#include "ImplicitUpwindFTBS.h"
#include "ImplicitFTCS.h"
using namespace std;


/*!
 *  \brief showResultsInConsole function
 *
 *  Handle the choice of scheme and call the right one
 *
 *  \param type An integer which represent the scheme chosen
 *  \param dt Delta t choosen
 */

void showResultsInConsole(const int type, const double dt) {																//declare the function which select the correct scheme
	Commons fx;
	if (type == 1) {																										//if the type is 1 =>
		if (fx.getMyRank() == 0) {
			cout << "----------- EXPLICIT UPWIND FTBS -------------------\n";													//Print a the scheme choosen
		}
		ExplicitUpwindFTBS ExUPFTBS;																						//Create an object of Explicit Upwind FTBS class
		ExUPFTBS.resultDt(dt);																								//call the function resultDt with delta t
	}	
	if (type == 2) {																										//if the type is 2 =>
		if (fx.getMyRank() == 0) {
			cout << "----------- IMPLICIT UPWIND FTBS -------------------\n";													//Print a the scheme choosen
		}
		ImplicitUpwindFTBS ImpUPFTBS;																						//Create an object of Implicit Upwind FTBS class
		ImpUPFTBS.resultDt(dt);																								//call the function resultDt with delta t
	}
	if (type == 3) {																										//if the type is 4 =>
		if (fx.getMyRank() == 0) {
			cout << "----------- IMPLICIT FTCS -------------------\n";															//Print a the scheme choosen
		}
		ImplicitFTCS ImpFTCS;																								//Create an object of Implicit FTCS class
		ImpFTCS.resultDt(dt);																								//call the function resultDt with delta t
	}
}


/*!
 *  \brief Main function
 *
 *  Let the user choosen the scheme and the delta t.
 *	Delta t is affected in this function.
 *	call showResultsInConsole(const int type, const double dt)
 *
 *  \return If everythings works correctly, return 1
 */

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	
	Commons fx;
	setprecision(10);

	int type;																												//declare a int type
	int dtIndex;																											//declare an int dtIndex

	if (fx.getMyRank() == 0) {
		cout << "1 for EXPLICIT UPWIND FTBS \n2 for IMPLICIT UPWIND FTBS \n3 for IMPLICIT FTCS \n";		//print the text to explain how to acces to each scheme
		cin >> type;																											//get the value of given by the user and store it in the int type
		cout << "1 for dt = " << fx.dta << "\n2 for dt = " << fx.dtb << "\n3 for dt = " << fx.dtc << "\n";						//print to choose between each dt
		cin >> dtIndex;										//get the value of given by the user and store it in the int dtIndex
	}
	MPI_Bcast(&type, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&dtIndex, 1, MPI_INT, 0, MPI_COMM_WORLD);

	
	
	double dt;																												//declare a double dt
	if (dtIndex == 1)																										//if dtIndex equal 1 =>
		dt = fx.dta;																										//dt equal dta(0.02)
	else if (dtIndex == 2)																									//if dtIndex equal 2 =>
		dt = fx.dtb;																										//dt equal dtb(0.01)
	else if (dtIndex == 3)																									//if dtIndex equal 3 =>
		dt = fx.dtc;																										//dt equal dtc(0.005)

	showResultsInConsole(type, dt);																							//call the function showResultInConsole with the type of scheme and the delta t choosen


	MPI_Finalize();


	return 0;																												//if everything works correctly return 0
}




