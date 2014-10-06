#include <iostream>
#include <queue>
#include <map>
using namespace std;
#include "rushHour.h"

int main()
{
	int numCars;
	char* boardString;
	traffic jam;
	queue<char*> trafficQ; 
	map<char*, int> trafficMap;
	map<char*, int> trafficIt;
	bool solved;
	int numMoves;
	
	//for(int scenario = 1; ; scenario++)
	//{
		cin >> numCars;
		boardString = jam.getData(numCars);
		trafficQ.push(boardString);
		trafficMap.insert( pair<char*,int>(boardString, 0) );
	
		while( trafficQ.size() & !solved)
		{
			boardString = trafficQ.front();
			trafficQ.pop();
			jam.stringToBoard(boardString, numCars);
			int numMoves = ( trafficMap.find(boardString) ) -> second;
			for(int i = 0; i < numCars; i++)
			{
				if ( jam.canMoveForward(i) )
				{
					cout << "Moving Forward";
					jam.moveForward(i);
					if( trafficMap.count( jam.getBoardString() ) == 0 )
					{
						if(jam.isSolved())
						{
							solved = true;
							numMoves = numMoves + 1;
						}
						trafficMap.insert( pair<char*,int>(boardString, numMoves + 1) );
						trafficQ.push( jam.getBoardString() );
					}
					jam.moveBackward(i);
				}
				if( jam.canMoveBack(i) )
				{
					cout << "Moving Forward";
					jam.moveBackward(i);
					if( trafficMap.count( jam.getBoardString() ) == 0 )
					{
						if(jam.isSolved())
						{
							solved = true;
							numMoves = numMoves + 1;
						}
						trafficMap.insert( pair<char*,int>(boardString, numMoves + 1) );
						trafficQ.push( jam.getBoardString() );
					}
					jam.moveForward(i);
				}
			}
		}
		cout << "Scenario " << scenario << " requires " 
			 << numMoves << " moves" << endl;*/
		//}
	
}