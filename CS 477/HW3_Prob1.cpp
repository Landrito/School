/***************************************************************/
/*HEADER FILES AND DEPENDENCIES*/
/***************************************************************/
#include <iostream>
using namespace std;


/***************************************************************/
/*FUNCTION PROTOTYPES*/
/***************************************************************/
template<typename dataType>
dataType max(dataType data[], int lowIndex, int highIndex);

/***************************************************************/
/*MAIN PROGRAM*/
/***************************************************************/
int main()
{
	char A[12] = "TINYEXAMPLE";
	max(A, 0, 10);
}

/***************************************************************/
/*FUNCTION IMPLEMENTATION*/
/***************************************************************/
template<typename dataType>
dataType max(dataType data[], int lowIndex, int highIndex)
{
	//Base Case 1: Get to a point where the low and the high index
	//	are pointing to the same piece of data
	if( lowIndex == highIndex )
	{
		cout 	<< "<" << data[lowIndex] << ", 'Max( " 
				<< lowIndex << ", " << highIndex << " )'>" << endl;
			return data[lowIndex];
		return data[lowIndex];
	}

	//Base Case 2: Get to a point where the low and the high index
	// are one value apart thus making it a comparison between two
	// datatypes.
	if( lowIndex == highIndex - 1 )
	{
		if( data[lowIndex] > data[highIndex] )
		{
			cout 	<< "<" << data[lowIndex] << ", 'Max( " 
					<< lowIndex << ", " << highIndex << " )'>" << endl;
			return data[lowIndex];
		}
		else
		{
			cout 	<< "<" << data[highIndex] << ", 'Max( " 
					<< lowIndex << ", " << highIndex << " )'>" << endl;
			return data[highIndex];
		}
	}

	//If it isn't a base call find the middle index and do a recursive
	//	call on both sides of the middle index.
	int midIndex = ( lowIndex + highIndex ) / 2;
	dataType leftMax = max( data, lowIndex, midIndex );
	dataType rightMax = max( data, midIndex + 1, highIndex );

	//Return the high value of the recursive calls.
	if( leftMax > rightMax )
	{
		cout 	<< "<" << leftMax << ", 'Max( " 
				<< lowIndex << ", " << highIndex << " )'>" << endl;
		return leftMax;
	}
	else
	{
		cout 	<< "<" << rightMax << ", 'Max( " 
				<< lowIndex << ", " << highIndex << " )'>" << endl;
		return rightMax;
	}
}