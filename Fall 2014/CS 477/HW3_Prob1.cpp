template<typename, dataType>
dataType max(dataType data[], int lowIndex, int highIndex);

int main()
{
	char A[11] = "TINYEXAMPLE";
	max(A, 0, 10);
}

template<typename, dataType>
dataType max(dataType data[], int lowIndex, int highIndex)
{
	if( lowIndex == highIndex )
	{
		return data[lowIndex];
	}

	if( lowIndex == highIndex - 1 ) //base case
	{
		if( data[lowIndex] > data[highIndex] )
		{
			cout 	<< "<" << data[lowIndex] << ", 'Max( " 
					<< lowIndex << ", " << highIndex << " )'>";
			return data[lowIndex];
		}
		else
		{
			cout 	<< "<" << data[highIndex] << ", 'Max( " 
					<< lowIndex << ", " << highIndex << " )'>";
			return data[highIndex];
		}
	}

	int midIndex = ( lowIndex + hgihIndex ) / 2;
	dataType leftMax = max( data, lowIndex, midIndex );
	dataType rightMax = max( data, midIndex + 1, highIndex );

	if( leftMax > rightMax )
	{
		cout 	<< "<" << leftMax << ", 'Max( " 
				<< lowIndex << ", " << highIndex << " )'>";
		return leftMax;
	}
	else
	{
		cout 	<< "<" << rightMax << ", 'Max( " 
				<< lowIndex << ", " << highIndex << " )'>";
		return rightMax;
	}
}