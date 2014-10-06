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
	bool canMoveForward, canMoveBack, justMoved;
};

class traffic
{
public:
	traffic();
	traffic(const traffic &other);
	void getData(int numCars);
	void checkCanMove() const;
	void solve();
	void recSolve(int movesSoFar);
	void moveBackward(int xPos, int yPos);
	void moveForward(int xPos, int yPos);
	void printMoves() const;
	void setDefault(vehicle &other);
	
private:
	vehicle **grid;
	int numMoves;
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
 @pre	Unitialized Traffic Class
 @post 	Deep Copy of the class

 Algorithm:
	- Deep Copy the Grid;
	- Copied Data Members;

 Exceptional/Error Conditions:
 	- none

**********************************************************
*/
traffic::traffic(const traffic &other)
{
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			grid[i][j] = other.grid[i][j];
		}
	}
	numMoves = other.numMoves;
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
		for(int j = 0; j < carLength; j++)
		{
			if(orientation == 'H')
			{
				grid[yCoord][xCoord + j].orientation = 'H';
				grid[yCoord][xCoord + j].id = i;
				grid[yCoord][xCoord + j].size = carLength;
				grid[yCoord][xCoord + j].leftX = xCoord;
				grid[yCoord][xCoord + j].topY = yCoord;
			}
			else
			{
				grid[yCoord + j][xCoord].orientation = 'V';
				grid[yCoord + j][xCoord].id = i;
				grid[yCoord + j][xCoord].size = carLength;
				grid[yCoord + j][xCoord].leftX = xCoord;
				grid[yCoord + j][xCoord].topY = yCoord;
			}
		}
	}
	checkCanMove();
}

/**
*********************************************************
 @pre	none

 @post 	numMoves will now be the smallest moves possible

 Algorithm:
	- Call recursive solve

***********************************************************
*/
void traffic::solve()
{
	recSolve(0);
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
	//Check if passed max number of move
	if(movesSoFar >= numMoves)
	{
		cout << "Case 1" << endl;
		return;
	}
	//Check if finished
	else if(grid[0][5].id == 0 ||
			grid[1][5].id == 0 ||
			grid[2][5].id == 0 ||
			grid[3][5].id == 0 ||
			grid[4][5].id == 0 ||
			grid[5][5].id == 0)
	{
		cout << "Case 2" << endl;
		if(movesSoFar < numMoves)
		{
			numMoves = movesSoFar;
		}
		return;
	}
	//else check if there are any cars that can be moved, and move them
	else
	{
		cout << "Case 3" << endl;
		//loop through the grid
		//cout << "Checked" << endl;
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				//cout << "j and i val: " << j << " " << i << endl;
				if(i == grid[i][j].topY && j == grid[i][j].leftX)
				{
					
					cout << "x and y val: " <<  grid[i][j].leftX << " " << grid[i][j].topY << endl;
					cout << "Forward: " << grid[i][j].canMoveForward << endl;
					cout << "Backward: " << grid[i][j].canMoveBack << endl << endl;
					if(!grid[i][j].justMoved)
					{
						if(grid[i][j].canMoveForward)
						{
							cout << "Moving forward at " << j << " " << i << endl;
							cout << "Orientation: "<< grid[i][j].orientation << endl;
							horizontal = (grid[i][j].orientation == 'H');
							moveForward(j, i);
							recSolve(movesSoFar + 1);
							cout << "Undoing Move. Current Move Count: " << movesSoFar << endl;
							if(horizontal)
							{
								moveBackward(j + 1, i);
							}
							else
							{
								moveBackward(j, i + 1);
							}
						}
						if(grid[i][j].canMoveBack)
						{
							cout << "Moving backward at " << j << " " << i << endl;
							cout << "Orientation: "<< grid[i][j].orientation << endl;
							horizontal = (grid[i][j].orientation == 'H');
							moveBackward(j, i);
							recSolve(movesSoFar + 1);
							cout << "Undoing Move. Current Move Count: " << movesSoFar << endl;
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
						grid[i][j].justMoved = false;
					}
				}
			}
		}
		//Check if the position is a vehicle head
		//if a vehicle head, check if a vehicle can move forward and backwards
		//if it can move forward, move make a new array and and move the 
		//Make a new grid 
		
	}
}

/**
*********************************************************
 @pre	none

 @post 	The boolean can moves in each vehicle will be correct

 Algorithm:
	- Check if moving will move it out of bounds
	- check if moving will cause a collision

***********************************************************
*/
void traffic::checkCanMove() const
{
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			if(grid[i][j].orientation == 'H')
			{
				if(grid[i][j].leftX > 0)
				{
					if ( grid[ grid[i][j].topY ]
					[ grid[i][j].leftX - 1 ].id == EMPTY_VAL )
					{
						grid[i][j].canMoveBack = true;
					}
					else
					{
						grid[i][j].canMoveBack = false;
					}
				}
				else
				{
					grid[i][j].canMoveBack = false;
				}
				
				if( grid[i][j].leftX + grid[i][j].size <= 5 )
				{
					if( grid[ grid[i][j].topY ]
					[ grid[i][j].leftX + grid[i][j].size ].id == EMPTY_VAL )
					{
						grid[i][j].canMoveForward = true;
					}
					else
					{
						grid[i][j].canMoveForward = false;
					}
				}
				else
				{
					grid[i][j].canMoveForward = false;
				}
				
			}
			else if(grid[i][j].orientation == 'V')
			{
				if(grid[i][j].topY > 0)
				{
					if ( grid[ grid[i][j].topY - 1 ]
					[ grid[i][j].leftX ].id == EMPTY_VAL )
					{
						grid[i][j].canMoveBack = true;
					}
					else
					{
						grid[i][j].canMoveBack = false;
					}
				}
				else
				{
					grid[i][j].canMoveBack = false;
				}
				
				if(grid[i][j].topY + grid[i][j].size <= 5)
				{
					if( grid[ grid[i][j].topY + grid[i][j].size]
					[ grid[i][j].leftX ].id == EMPTY_VAL )
					{
						grid[i][j].canMoveForward = true;
					}
					else
					{
						grid[i][j].canMoveForward = false;
					}
				}
				else
				{
					grid[i][j].canMoveForward = false;
				}
			}
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
	//if(grid[yPos][xPos].canMoveBack)
	//{
		if(grid[yPos][xPos].orientation == 'H')
		{
			for(int i = 0; i < offset; i++)
			{
				grid[yPos][xPos + i].leftX--;
			}
			grid[yPos][xPos - 1].id = grid[yPos][xPos].id;
			grid[yPos][xPos - 1].size = grid[yPos][xPos].size;
			grid[yPos][xPos - 1].orientation = grid[yPos][xPos].orientation;
			grid[yPos][xPos - 1].leftX = grid[yPos][xPos].leftX;
			grid[yPos][xPos - 1].topY = grid[yPos][xPos].topY;
			grid[yPos][xPos - 1].canMoveBack = false;
			setDefault(grid[yPos][xPos + offset - 1]);
			checkCanMove();
			for(int i = 0; i < offset; i++)
			{
				grid[yPos][xPos + i - 1].justMoved = true;
			}
		}
		else if(grid[yPos][xPos].orientation == 'V')
		{
			for(int i = 0; i < offset; i++)
			{
				grid[yPos + i][xPos].topY;
			}
			grid[yPos - 1][xPos].id = grid[yPos][xPos].id;
			grid[yPos - 1][xPos].size = grid[yPos][xPos].size;
			grid[yPos - 1][xPos].orientation = grid[yPos][xPos].orientation;
			grid[yPos - 1][xPos].leftX = grid[yPos][xPos].leftX;
			grid[yPos - 1][xPos].topY = grid[yPos][xPos].topY;
			grid[yPos - 1][xPos].canMoveBack = false;
			setDefault(grid[yPos + offset - 1][xPos]);
			checkCanMove();
			for(int i = 0; i < offset; i++)
			{
				grid[yPos + i - 1][xPos].justMoved = true;
			}
			
		}
		//}
	
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
	//if(grid[yPos][xPos].canMoveForward)
	//{
		if(grid[yPos][xPos].orientation == 'H')
		{
			for(int i = 0; i < offset; i++)
			{
				grid[yPos][xPos + i].leftX++;
			}
			grid[yPos][xPos + offset].id = grid[yPos][xPos].id;
			grid[yPos][xPos + offset].size = grid[yPos][xPos].size;
			grid[yPos][xPos + offset].orientation = grid[yPos][xPos].orientation;
			grid[yPos][xPos + offset].leftX = grid[yPos][xPos].leftX;
			grid[yPos][xPos + offset].topY = grid[yPos][xPos].topY;
			grid[yPos][xPos + offset].canMoveForward = false;
			setDefault(grid[yPos][xPos]);
			checkCanMove();
			for(int i = 1; i <= offset; i++)
			{
				grid[yPos][xPos + i].justMoved = true;
			}
			
		}
		else if(grid[yPos][xPos].orientation == 'V')
		{
			for(int i = 0; i < offset; i++)
			{
				grid[yPos + i][xPos].topY++;
			}
			grid[yPos + offset][xPos].id = grid[yPos][xPos].id;
			grid[yPos + offset][xPos].size = grid[yPos][xPos].size;
			grid[yPos + offset][xPos].orientation = grid[yPos][xPos].orientation;
			grid[yPos + offset][xPos].leftX = grid[yPos][xPos].leftX;
			grid[yPos + offset][xPos].topY = grid[yPos][xPos].topY;
			grid[yPos + offset][xPos].canMoveForward = false;
			setDefault(grid[yPos][xPos]);
			checkCanMove();
			for(int i = 1; i <= offset; i++)
			{
				grid[yPos + i][xPos].justMoved = true;
			}
		}	
		//}
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
	canMoveForward = false;
	canMoveBack = false;
	orientation = 'n';
	justMoved = false;
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
	orientation = other.id;
	size = other.size;
	leftX = other.leftX;
	topY = other.topY;
	canMoveForward = other.canMoveForward;
	canMoveBack = other.canMoveForward;
	justMoved = false;
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
	other.canMoveForward = false;
	other.canMoveBack = false;
	other.orientation = 'n';
	other.justMoved = false;
}

