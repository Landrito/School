/***************************************************************/
/*HEADER FILES AND DEPENDENCIES*/
/***************************************************************/
#include <iostream>
using namespace std;


/***************************************************************/
/*FUNCTION PROTOTYPES*/
/***************************************************************/
template <typename dataType>
void BUMergeSort(dataType data[], int size);

template <typename dataType>
void merge(dataType data[], int lowIndex, int midIndex, int highIndex);

template <typename dataType>
void display(dataType data[], int size);

/***************************************************************/
/*MAIN PROGRAM*/
/***************************************************************/
int main()
{
    char data[20] = "ASORTINGEXAMPLE";
    int size = 15;

    BUMergeSort(data, size);

    display(data, size);

    return 0;
}

/***************************************************************/
/*FUNCTION IMPLEMENTATION*/
/***************************************************************/
template <typename dataType>
void BUMergeSort(dataType data[], int size)
{
    int lowIndex;
    int midIndex;
    int highIndex;

    //Go through the items in the list by 2 then 4 etc
    for( int i = 1; i <= ( (size / 2) + 1); i*=2)
    {
        for( int j = i; j < size; j+= (2*i))
        {
            //Set the lower index to the start 
            //  of where you are measuring
            lowIndex = j - i;

            //Set the middle of the array and that will
            //  what the function merges about
            midIndex = j;

            //Set the upper bound of the merge
            if( j + i < size )
                highIndex = j + i;

            else
                highIndex = size;

            //Merge the points in the data
            merge(data, lowIndex, midIndex, highIndex);
        }
    }
}

template <typename dataType>
void merge(dataType data[], int lowIndex, int midIndex, int highIndex)
{
    //Initialize Variables
    int left = lowIndex;
    int right = midIndex;
    dataType * newArray = new dataType[highIndex - lowIndex];
    int index = 0;

    //Print the data before the merge
    for( int i = lowIndex; i < highIndex; i++)
        cout << data[i] << " ";
    cout << endl;

    //Merge by inserting the lowest of the items from the left
    //  and right array
    while( left < midIndex && right < highIndex)
    {
        if( data[left] <= data[right] )
        {
            newArray[index] = data[left];
            left++; 
        }
        else
        {
            newArray[index] = data[right];
            right++;
        }
        index++;
    }

    //Insert the rest of the right or left arrays
    while( left < midIndex )
    {
        newArray[index] = data[left];
        left++;
        index++;
    }

    while( right < highIndex )
    {
        newArray[index] = data[right];
        right++;
        index++;
    }

    //Replace original array with new array
    for( int i = lowIndex, index = 0; i < highIndex; i++, index++)
        data[i] = newArray[index];

    //Deallocate temporary memory
    delete [] newArray;
}

template <typename dataType>
void display(dataType data[], int size)
{
    for( int i = 0; i < size; i++)
        cout << data[i] <<" ";

    cout << endl;
}
