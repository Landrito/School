//////////////////////////////////////////////////////////////////////
/*
    FileName:     	Pumpkin.cpp
   
    Description: 	Analyzes a garden and outputs how many pumpkin 
					patches there are and how many pumpkins there
					are in each
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 09/24/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the objects for
	a the garden, linked list, and list nodes.
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
	#include <iostream>
	#include "Garden.h"
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
	int main(  ){
		int width; 				//temporarily holds the width of the garden
		int height;				//temporarily holds the height of the garden
		int gardenCounter = 0;	//holds the number of gardens being checked
		Garden *pumpkinPatch;	//pointer to a garden
	
		cin >> height;
		cin >> width;
		while( width > 0 && height > 0 )
		{
		
			pumpkinPatch = new Garden( width, height );
			pumpkinPatch->getData( );
			pumpkinPatch->patchChecker( );
		
			gardenCounter++;
		
			cout << "Garden # " << gardenCounter << ": " 
				 << pumpkinPatch->patchList.getCount() << " patches, sizes: " 
				 << pumpkinPatch->patchList;
		
			delete pumpkinPatch;
			
			cin >> height;
			cin >> width;
			
			
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





