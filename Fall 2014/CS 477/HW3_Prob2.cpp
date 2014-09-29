#include <iostream>
using namespace std;

template <typename dataType>
void BUMergeSort(dataType data[], int size);

template <typename dataType>
void merge(dataType data[], int lowIndex, int midIndex, int highIndex);

template <typename dataType>
void display(dataType data[], int size);

int main()
{
    char data[20] = "ASORTINGEXAMPLE";
    int size = 15;

    BUMergeSort(data, size);

    display(data, size);

    return 0;
}

template <typename dataType>
void BUMergeSort(dataType data[], int size)
{
    int lowIndex;
    int midIndex;
    int highIndex;
    for( int i = 1; i <= ( (size / 2) + 1); i*=2)
    {
        for( int j = i; j < size; j+= (2*i))
        {
            lowIndex = j - i;
            midIndex = j;

            if( j + i < size )
                highIndex = j + i;

            else
                highIndex = size;

            merge(data, lowIndex, midIndex, highIndex);
        }
    }
}

template <typename dataType>
void merge(dataType data[], int lowIndex, int midIndex, int highIndex)
{
    int left = lowIndex;
    int right = midIndex;
    dataType * newArray = new dataType[highIndex - lowIndex];
    int index = 0;

    for( int i = lowIndex; i < highIndex; i++)
        cout << data[i] << " ";
    cout << endl;

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

    for( int i = lowIndex, index = 0; i < highIndex; i++, index++)
        data[i] = newArray[index];
}

template <typename dataType>
void display(dataType data[], int size)
{
    for( int i = 0; i < size; i++)
        cout << data[i] <<" ";

    cout << endl;
}
