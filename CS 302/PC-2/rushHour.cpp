//////////////////////////////////////////////////////////////////////
/*
    FileName:     	rushHour.cpp
   
    Description: 	Creates a new traffic jam and solves the traffic
					problem and outputs the minimum number of moves
					to solve it.
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 10/15/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the objects for
	a the traffic and vehicle classes.
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include <iostream>
#include "rushHour.h"
using namespace std;
//
//  Global Constant Definitions  ////////////////////////////////////
//
/////  NONE
//
//  Class Definitions  //////////////////////////////////////////////
//
/////  NONE
//
//  Free Function Prototypes  ///////////////////////////////////////
//
/////  NONE
//
//  Main Function Implementation  ///////////////////////////////////
//
int main()
{
	int numCars;
	traffic *jam;
	jam = new traffic;
	cin >> numCars;
	for(int i = 1; numCars > 0; i++)
	{
		jam->getData(numCars);
		jam->recSolve(0);
		cout << "Scenario " <<  i << " requires ";
		jam->printMoves();
		cout << " moves" << endl;
		delete jam;
		jam = new traffic;
		cin >> numCars;
	}
	
	return 0;
}
//
//  Free Function Implementation  ///////////////////////////////////
//
/////  NONE
//
//  Class/Data Structure Member Implementation //////////////////////
//
/////  NONE
//
//  Terminating Precompiler Directives ///////////////////////////////
//
/////  NONE
//
