#include <iostream>
using namespace std;

const int DIMENSIONS = 6;

class car
{
public:
	bool operator==(const car & other) const;
	bool operator!=(const car & other) const;
	bool operator<(const car & other) const;

	int row, col, length;
	char orientation;
};

class board
{
public:
	board();
	board(const board & other);
	board& operator=(const board & other);
	bool operator<(const board & other) const;
	bool operator==(const board & other) const;
	bool operator!=(const board & other) const;
	
	void getData(int Cars);
	bool canMoveHere(int row, int col);
	bool isSolved() const;
	bool moveForward(int carID);
	bool moveBackward(int carID);
	int getNumCars() const;
	

private:
	int numCars;
	car gameBoard[18];
};
/**
*********************************************************
 @pre	Unitialized board Class
 @post 	numCars intialized

 Algorithm:
	- Assign data members values

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
board::board()
{
	numCars = 0;
}

/**
*********************************************************
 @pre	Unitialized board Class
 @post 	Grid Initialized
		numCars intialized
 @param other Other grid to be copied from

 Algorithm:
	- copy data from other board

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
board::board(const board & other)
{
	numCars = other.numCars;
	for(int i = 0; i < numCars; i++)
	{
		gameBoard[i] = other.gameBoard[i];
	}
}

/**
*********************************************************
 @pre	Board Class
 @post 	Grid Initialized
		numCars intialized
 @param other Other grid to be copied from

 Algorithm:
	- copy data from other board

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
board& board::operator=(const board & other)
{
	if(this == &other)
	{
		return *this;
	}
	numCars = other.numCars;
	for(int i = 0; i < numCars; i++)
	{
		gameBoard[i] = other.gameBoard[i];
	}
	return *this;
}

/**
*********************************************************
 @pre	Board Class
 @post 	boolean returned
 @return Bolean if the board is less than the other
 @param other Other grid to be check

 Algorithm:
	- go through the grid and check if this car is less than the other car

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool board::operator<(const board & other) const
{
	if( numCars != other.numCars )
	{
		return numCars < other.numCars;
	}
	for(int i = 0; i < numCars; i++)
	{
		if(gameBoard[i] != other.gameBoard[i])
		{
			return gameBoard[i] < other.gameBoard[i];
		}
	}
	return false;
}

/**
*********************************************************
 @pre	Board Class
 @post 	boolean returned
 @return Bolean if the board is equivalent to the other
 @param other Other grid to be checked

 Algorithm:
	- go through the grid and check if each car is equivalent

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool board::operator==(const board & other) const
{
	if( numCars != other.numCars )
	{
		return false;
	}
	for(int i = 0; i < numCars; i++)
	{
		if(gameBoard[i] != other.gameBoard[i])
		{
			return false;
		}
	}
	return true;
}

/**
*********************************************************
 @pre	Board Class
 @post 	boolean returned
 @return Bolean if the board is not equivalent to the other
 @param other Other grid to be checked

 Algorithm:
	- use the equivalent operator

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool board::operator!=(const board & other) const
{
	return !(*this == other);
}

/**
*********************************************************
 @pre	Board Class
 @post 	data inserted into the board
 @param Cars number of cars to be inserted into the board

 Algorithm:
	- assign the value of each car

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
void board::getData(int Cars)
{
	numCars = Cars;
	for( int i = 0; i < numCars; i++)
	{
		cin >> gameBoard[i].length >> gameBoard[i].orientation
			>> gameBoard[i].row >> gameBoard[i].col;
	}
}

/**
*********************************************************
 @pre	Board Class
 @post 	returned boolean
 @return if a car can move to tht location
 @param row row location to check
 @param col column location to check

 Algorithm:
	- go through each car and check their row and column
	- check the row or column that the rest of the car hits

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool board::canMoveHere(int row, int col)
{
	for(int i = 0; i < numCars; i++)
	{
		if( gameBoard[i].orientation == 'H' )
		{
			for( int j = 0; j < gameBoard[i].length; j++)
			{
				if(row == gameBoard[i].row && col == gameBoard[i].col + j)
				{
					return false;
				}
			}
		}
		else
		{
			for( int j = 0; j < gameBoard[i].length; j++)
			{
				if(row == gameBoard[i].row + j && col == gameBoard[i].col)
				{
					return false;
				}
			}
		}
	}
	return true;
}

/**
*********************************************************
 @pre	Board Class
 @post 	returned boolean
 @return if the main car is at the end of the row

 Algorithm:
	- check the column location of the main car

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool board::isSolved() const
{
	return gameBoard[0].col + gameBoard[0].length == DIMENSIONS;
}

/**
*********************************************************
 @pre	none
 @post 	Vehicle in the grid will be moved right or Up
 @return return true if moved
 @param carID

 Algorithm:
	- Check which direction to move
	- Check if moving will go out of bounds
	- check if can move
	- move and return true

***********************************************************
*/
bool board::moveForward(int carID)
{
	
	if ( gameBoard[carID].orientation == 'H' )
	{
		if( gameBoard[carID].col + gameBoard[carID].length < DIMENSIONS &&
			canMoveHere(gameBoard[carID].row, gameBoard[carID].col + gameBoard[carID].length) )
		{
			gameBoard[carID].col++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if( gameBoard[carID].row + gameBoard[carID].length < DIMENSIONS &&
			canMoveHere(gameBoard[carID].row + gameBoard[carID].length, gameBoard[carID].col) )
		{
			gameBoard[carID].row++;
			return true;
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
 @post 	Vehicle in the grid will be moved Left or down
 @return return true if moved
 @param carID

 Algorithm:
	- Check which direction to move
	- Check if moving will go out of bounds
	- check if can move
	- move and return true

***********************************************************
*/
bool board::moveBackward(int carID)
{
	if ( gameBoard[carID].orientation == 'H' )
	{
		if( gameBoard[carID].col > 0 &&
			canMoveHere(gameBoard[carID].row, gameBoard[carID].col - 1) )
		{
			gameBoard[carID].col--;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if( gameBoard[carID].row > 0 &&
			canMoveHere(gameBoard[carID].row - 1, gameBoard[carID].col) )
		{
			gameBoard[carID].row--;
			return true;
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
 @post 	int returned
 @return return number of cars in the board

 Algorithm:
	- return the number of cars

***********************************************************
*/
int board::getNumCars() const
{
	return numCars;
}

/**
*********************************************************
 @pre	Two Car Classes
 @post 	returns boolean if equivalent
 @return Returns true if every private data value of the class is equal

 Algorithm:
	- return if equal

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool car::operator==(const car & other) const
{
	return( row == other.row &&
			col == other.col &&
			length == other.length &&
			orientation == other.orientation );
}

/**
*********************************************************
 @pre	Two Car Classes

 @post 	returns boolean if not equivalent

 @return Returns true if one private data value of the class is not equal.

 Algorithm:
	- return not if equal

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool car::operator!=(const car & other) const
{
	return !(*this == other);
}

/**
*********************************************************
 @pre	Two Car Classes

 @post 	returns boolean if less than

 @return True if the rhs is less than the left

 Algorithm:
	- check each value if one is less than starting with length

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
bool car::operator<(const car & other) const
{
	if(length != other.length)
	{
		return length < other.length;
	}
	if(row != other.row)
	{
		return row < other.row;
	}
	if(col != other.col)
	{
		return col < other.col;
	}
	if(orientation != other.orientation)
	{
		return orientation < other.orientation;
	}
	return false;
}