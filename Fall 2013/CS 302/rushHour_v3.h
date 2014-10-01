#include <iostream>
using namespace std;

const int EMPTY_VAL = -1;
class vehicle
{
public:
	vehicle();
	vehicle(const vehicle &other);
	int id;
	int size;
	char orientation;
	int leftX, topY;
	bool justMovedForward, justMovedBackward;
};

class traffic
{
public:
	traffic();
	void getData(int numCars);
	bool canMoveForward(int xPos, int yPos) const;
	bool canMoveBack(int xPos, int yPos) const;
	void recSolve(int movesSoFar);
	void moveBackward(int xPos, int yPos);
	void moveForward(int xPos, int yPos);
	void printMoves() const;
	void setDefault(vehicle &other);
	
private:
	vehicle **grid;
	int numMoves;
	int mainX, mainY;
	
};

/**
*********************************************************
 @pre	Unitialized Traffic Class
 @post 	Grid Initialized
		numCars and numMoves intialized

 Algorithm:
	- Assign data members values

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
traffic::traffic()
{
	grid = new vehicle*[6];
	for(int i = 0; i < 6; i++)
	{
		grid[i] = new vehicle[6];
	}
	numMoves = 10;
}

/**
*********************************************************
 @pre	Initialized Grid

 @post 	Grid filled with vehicle data

 @param numCars This is how many cars to put into the grid

 Algorithm:
	- Get number of cars
	- Insert cars into the Grid based on the input values

***********************************************************
*/
void traffic::getData(int numCars)
{
	int carLength, xCoord, yCoord;
	char orientation;
	for(int i = 0; i < numCars; i++)
	{
		cin >> carLength >> orientation >> yCoord >> xCoord;
		if(i == 0)
		{
			mainX = xCoord;
			mainY = yCoord;
			orientation = 'M';
		}
		for(int j = 0; j < carLength; j++)
		{
			if(orientation == 'H' || orientation == 'M')
			{
				grid[yCoord][xCoord + j].orientation = orientation;
				grid[yCoord][xCoord + j].id = i;
				grid[yCoord][xCoord + j].size = carLength;
				grid[yCoord][xCoord + j].leftX = xCoord;
				grid[yCoord][xCoord + j].topY = yCoord;
			}
			else
			{
				grid[yCoord + j][xCoord].orientation = orientation;
				grid[yCoord + j][xCoord].id = i;
				grid[yCoord + j][xCoord].size = carLength;
				grid[yCoord + j][xCoord].leftX = xCoord;
				grid[yCoord + j][xCoord].topY = yCoord;
			}
		}
	}
}

/**
*********************************************************
 @pre	none

 @post 	numMoves will now be the smallest moves possible

 @param movesSoFar This is how many moves have been taken

 Algorithm:
	- Check if used too many moves
	- Check if finished the puzzle
	- If not, find any cars that can move
	- Move the cars that can and repeat

***********************************************************
*/
void traffic::recSolve(int movesSoFar)
{
	bool horizontal;
	//cout << "Moves so far: " << movesSoFar << endl;
	if(movesSoFar >= numMoves)
	{
		//cout << "Case 1" << endl;
		return;
	}
	else if(grid[0][5].id == 0 ||
			grid[1][5].id == 0 ||
			grid[2][5].id == 0 ||
			grid[3][5].id == 0 ||
			grid[4][5].id == 0 ||
			grid[5][5].id == 0)
	{
		//cout << "Case 2" << endl;
		if(movesSoFar < numMoves)
		{
			numMoves = movesSoFar;
		}
		return;
	}
	else if( canMoveForward(mainX, mainY) )
	{
		//cout << "Case 3" << endl;
		moveForward(mainX, mainY);
		mainX++;
		recSolve(movesSoFar + 1);
		moveBackward(mainX, mainY);
		mainX--;
		//cout << "Undoing Move" << endl;
	}
	else
	{
		//cout << "Case 4" << endl;
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				if(i == grid[i][j].topY && j == grid[i][j].leftX &&
				   grid[i][j].orientation != 'M')
				{
					//cout << "Check 1" << endl;
					if(!grid[i][j].justMovedBackward)
					{
						//cout << "Check 2" << endl;
						if(canMoveForward(j,i))
						{
							//cout << "Moving Forward" << endl;
							horizontal = (grid[i][j].orientation == 'H');
							moveForward(j,i);
							recSolve(movesSoFar + 1);
							//cout << "Undoing Move" << endl;
							if(horizontal)
							{
								moveBackward(j + 1, i);
							}
							else
							{
								moveBackward(j, i + 1);
							}
						}
					}
					else
					{
						grid[i][j].justMovedForward = false;
					}
					if(!grid[i][j].justMovedForward)
					{
						//cout << "Check 2" << endl;
						if(canMoveBack(j,i))
						{
							//cout << "Moving Backward" << endl;
							horizontal = (grid[i][j].orientation == 'H');
							moveBackward(j,i);
							recSolve(movesSoFar + 1);
							//cout << "Undoing Move" << endl;
							if(horizontal)
							{
								moveForward(j - 1, i);
							}
							else
							{
								moveForward(j, i - 1);
							}
						}
					}
					else
					{
						grid[i][j].justMovedBackward = false;
					}
				}
			}
		}
	}
}

/**
*********************************************************
 @pre	none

 @post 	none

 @param xPos The x position of the vehicle to be checked
 @param yPos The y position of the vehicle to be checked

 @return Returns if the car can move left or up

 Algorithm:
	- Check if moving will move it out of bounds
	- check if moving will cause a collision

***********************************************************
*/
bool traffic::canMoveBack(int xPos, int yPos) const
{
	if(grid[yPos][xPos].orientation == 'H')
	{
		if(grid[yPos][xPos].leftX > 0)
		{
			if ( grid[ grid[yPos][xPos].topY ]
			[ grid[yPos][xPos].leftX - 1 ].id == EMPTY_VAL )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(grid[yPos][xPos].topY > 0)
		{
			if ( grid[ grid[yPos][xPos].topY - 1 ]
			[ grid[yPos][xPos].leftX ].id == EMPTY_VAL )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

/**
*********************************************************
 @pre	none

 @post 	none

 @param xPos The x position of the vehicle to be checked
 @param yPos The y position of the vehicle to be checked

 @return Returns if the car can move right or down

 Algorithm:
	- Check if moving will move it out of bounds
	- check if moving will cause a collision

***********************************************************
*/
bool traffic::canMoveForward(int xPos, int yPos) const
{
	if(grid[yPos][xPos].orientation == 'H' || 
	   grid[yPos][xPos].orientation == 'M' )
	{
		if( grid[yPos][xPos].leftX + grid[yPos][xPos].size <= 5 )
		{
			if( grid[ grid[yPos][xPos].topY ]
			[ grid[yPos][xPos].leftX + grid[yPos][xPos].size ].id == EMPTY_VAL )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	else
	{
		if(grid[yPos][xPos].topY + grid[yPos][xPos].size <= 5)
		{
			if( grid[ grid[yPos][xPos].topY + grid[yPos][xPos].size]
			[ grid[yPos][xPos].leftX ].id == EMPTY_VAL )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

/**
*********************************************************
 @pre	none

 @post 	Vehicle in the grid will be moved Left or Up

 @param xPos This is the x Position of the vehicle head to be moved
 @param yPos This is the y Position of the vehicle head to be moved

 Algorithm:
	- Change the head coordinates
	- Assign new head coordinate the vehicle
	- Set the tail of the vehicle to default values

***********************************************************
*/
void traffic::moveBackward(int xPos, int yPos)
{
	int offset = grid[yPos][xPos].size;
	if(grid[yPos][xPos].orientation == 'H' ||
	   grid[yPos][xPos].orientation == 'M')
	{
		for(int i = 0; i < offset; i++)
		{
			grid[yPos][xPos + i].leftX--;
		}
		grid[yPos][xPos - 1] = grid[yPos][xPos];
		setDefault(grid[yPos][xPos + offset - 1]);
		for(int i = 0; i < offset; i++)
		{
			grid[yPos][xPos + i - 1].justMovedBackward = true;
			grid[yPos][xPos + i - 1].justMovedForward = false;
		}
	}
	else if(grid[yPos][xPos].orientation == 'V')
	{
		for(int i = 0; i < offset; i++)
		{
			grid[yPos + i][xPos].topY--;
		}
		grid[yPos - 1][xPos] = grid[yPos][xPos];
		setDefault(grid[yPos + offset - 1][xPos]);
		for(int i = 0; i < offset; i++)
		{
			grid[yPos + i - 1][xPos].justMovedBackward = true;
			grid[yPos + i - 1][xPos].justMovedForward = false;
		}
	}
}

/**
*********************************************************
 @pre	none

 @post 	Vehicle in the grid will be moved right or down

 @param xPos This is the x Position of the vehicle head to be moved
 @param yPos This is the y Position of the vehicle head to be moved

 Algorithm:
	- Change the head coordinates
	- Assign new head coordinate the vehicle
	- Set the head of the vehicle to default values

***********************************************************
*/
void traffic::moveForward(int xPos, int yPos)
{ 
	int offset = grid[yPos][xPos].size;
	if(grid[yPos][xPos].orientation == 'H' ||
	   grid[yPos][xPos].orientation == 'M')
	{
		for(int i = 0; i < offset; i++)
		{
			grid[yPos][xPos + i].leftX++;
		}
		grid[yPos][xPos + offset] = grid[yPos][xPos];
		setDefault(grid[yPos][xPos]);
		for(int i = 1; i <= offset; i++)
		{
			grid[yPos][xPos + i].justMovedForward = true;
			grid[yPos][xPos + i].justMovedBackward = false;
		}
		
	}
	else if(grid[yPos][xPos].orientation == 'V')
	{
		for(int i = 0; i < offset; i++)
		{
			grid[yPos + i][xPos].topY++;
		}
		grid[yPos + offset][xPos] = grid[yPos][xPos];
		setDefault(grid[yPos][xPos]);
		for(int i = 1; i <= offset; i++)
		{
			grid[yPos + i][xPos].justMovedForward = true;
			grid[yPos + i][xPos].justMovedBackward = false;
		}
	}	
}

/**
*********************************************************
 @pre	none

 @post 	printed moves

 Algorithm:
	- output numMoves

***********************************************************
*/
void traffic::printMoves() const
{
	cout << numMoves;
}

/**
*********************************************************
 @pre	unitialized Vehicle class

 @post  initialized data members

 Algorithm:
	- initialize data members

***********************************************************
*/
vehicle::vehicle()
{
	id = EMPTY_VAL;
	size = EMPTY_VAL;
	leftX = EMPTY_VAL;
	topY = EMPTY_VAL;
	orientation = 'n';
	justMovedForward = false;
	justMovedBackward = false;
}

/**
*********************************************************
 @pre	unitialized Vehicle class

 @post 	copied vehicle class

 Algorithm:
	- initialize data members using others

***********************************************************
*/
vehicle::vehicle(const vehicle &other)
{
	id = other.id;
	orientation = other.orientation;
	size = other.size;
	leftX = other.leftX;
	topY = other.topY;
	justMovedForward = false;
	justMovedBackward = false;
}

/**
*********************************************************
 @pre	none

 @post 	vehicle values changed to default

 @param other This is the vehicle to be changed

 Algorithm:
	- assign vehicle data members default values

***********************************************************
*/
void traffic::setDefault(vehicle &other)
{
	other.id = EMPTY_VAL;
	other.size = EMPTY_VAL;
	other.leftX = EMPTY_VAL;
	other.topY = EMPTY_VAL;
	other.orientation = 'n';
	other.justMovedForward = false;
	other.justMovedBackward = false;
}

