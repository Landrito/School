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
	2.00 ( 12/4/2013 ) - Ernest Landrito - C.S. Student
		Totally overhauled the way to solve and implemented new 
		classes
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
#include "rushHour.h"
#include <iostream>
#include <queue>
#include <map>
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
int solve(board & first);
//
//  Main Function Implementation  ///////////////////////////////////
//
int main()
{
	int numCars, numMoves;
	int scenario = 1;
	board gameBoard;
	
	cin >> numCars;
	while(numCars > 0)
	{
		gameBoard.getData(numCars);
		numMoves = solve(gameBoard);
	
		cout << "Scenario " << scenario << " requires " 
			 << numMoves << " moves" << endl;
		scenario++;
		cin >> numCars;
	}
}
//
//  Free Function Implementation  ///////////////////////////////////
//
int solve(board & first)
{
	queue<board> trafficQ;
	map<board, int> trafficMap;
	int numMoves;
	
	if( first.isSolved() )
	{
		return 0;
	}
	trafficQ.push(first);
	trafficMap.insert( pair<board,int>(first, 0) );
	
	while( trafficQ.size() > 0 )
	{
		first = trafficQ.front();
		trafficQ.pop();
		numMoves = ( trafficMap.find(first) ) -> second;
		
		if( first.isSolved() )
		{
			return numMoves;
		}
		
		for( int i = 0; i < first.getNumCars(); i++)
		{
			if( first.moveForward(i) )
			{
				if( trafficMap.count(first) == 0 )
				{
					trafficMap.insert( pair<board,int>(first,numMoves + 1) );
					trafficQ.push(first);
				}
				first.moveBackward(i);
			}
			
			if( first.moveBackward(i) )
			{
				if( trafficMap.count(first) == 0 )
				{
					trafficMap.insert( pair<board,int>(first,numMoves + 1) );
					trafficQ.push(first);
				}
				first.moveForward(i);
			}
		}
	}
	return -1;
}
//
//  Class/Data Structure Member Implementation //////////////////////
//
/////  NONE
//
//  Terminating Precompiler Directives ///////////////////////////////
//
/////  NONE
//
