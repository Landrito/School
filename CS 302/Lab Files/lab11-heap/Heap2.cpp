#include <stdexcept>
#include <iostream>
#include "Heap.h"
using namespace std;

template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType, KeyType, Comparator>::Heap(int maxNumber)
{
	size = 0;
	maxSize = maxNumber;
	dataItems = new DataType[maxNumber];
}

template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType, KeyType, Comparator>::Heap(const Heap& other)
{
	*this = other;
}

template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType, KeyType, Comparator>& Heap<DataType, KeyType, Comparator>::operator=(const Heap<DataType, KeyType, Comparator>& other)
{
	if(!isEmpty() && this != &other)
	{
		clear();
	}
	maxSize = other.maxSize;
	size = other.size;
	for(int i = 0; i < size; i++)
	{
		dataItems[i] = other.dataItems[i];
	}
}

template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType, KeyType, Comparator>::~Heap()
{
	clear();
	delete dataItems;
}

template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType, KeyType, Comparator>::insert(const DataType &newDataItem) throw (logic_error)
{
	int tempIndex;
	DataType tempData;
	Comparator compare;
	dataItems[size] = newDataItem;
	tempIndex = size;
	if(!isFull())
	{
		while(compare( dataItems[ tempIndex ].getPriority() , dataItems[ (tempIndex - 1) / 2 ].getPriority() ) )
		{
			tempData = dataItems[ tempIndex ];
			dataItems[ tempIndex ] = dataItems[ (tempIndex - 1) / 2 ];
			dataItems[ (tempIndex - 1) / 2 ] = tempData;
			tempIndex = (tempIndex - 1) / 2;
		}
		size++;
	}
	else
	{
		throw logic_error("Full List");
	}
}

template <typename DataType, typename KeyType, typename Comparator>
DataType Heap<DataType, KeyType, Comparator>::remove() throw (logic_error)
{
	Comparator compare;
	DataType returnData, tempData;
	int tempIndex = 0;
	int switchVal;
	
	if(isEmpty())
	{
		throw logic_error("Empty List");
	}
	
	size--; returnData = dataItems[0]; 
	dataItems[0] = dataItems[size];
	
	
	while(tempIndex < size)
	{
		if( (tempIndex * 2) + 2 <= size)
		{
			if( compare( dataItems[ tempIndex ].getPriority(), dataItems[ (tempIndex * 2) + 1 ].getPriority() ) &&
				compare( dataItems[ tempIndex ].getPriority(), dataItems[ (tempIndex * 2) + 2 ].getPriority() ) )
			{
				return returnData;
			}
			else if( compare( dataItems[ (tempIndex * 2) + 2 ].getPriority(), dataItems[ (tempIndex * 2) + 1 ].getPriority() ) )
			{
				tempData = dataItems[tempIndex];
				dataItems[tempIndex] = dataItems[ ( tempIndex * 2 ) + 2];
				dataItems[ ( tempIndex * 2 ) + 2 ] = tempData;
				tempIndex = ( tempIndex * 2 ) + 2;
			}
			else
			{
				tempData = dataItems[tempIndex];
				dataItems[tempIndex] = dataItems[ ( tempIndex * 1 ) + 1];
				dataItems[ ( tempIndex * 2 ) + 1 ] = tempData;
				tempIndex = ( tempIndex * 2 ) + 1;
			}
		}
		
		else if( (tempIndex * 2) + 1 <= size)
		{
			if( compare( dataItems[ (tempIndex * 2) + 1 ].getPriority(), dataItems[ tempIndex ].getPriority() ) )
			{
				tempData = dataItems[tempIndex];
				dataItems[tempIndex] = dataItems[ ( tempIndex * 2 ) + 1 ];
				dataItems[ ( tempIndex * 2 ) + 1 ] = tempData;
				tempIndex = ( tempIndex * 2 ) + 1;
			}
			else
			{
				return returnData; 
			}
		}
		
		else
		{
			return returnData;
		}
			
	}
	
	return returnData;
}

template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType, KeyType, Comparator>::clear()
{
	maxSize = 0;
	size = 0;
}

template <typename DataType, typename KeyType, typename Comparator>
bool Heap<DataType, KeyType, Comparator>::isEmpty() const
{
	return (size == 0);
}

template <typename DataType, typename KeyType, typename Comparator>
bool Heap<DataType, KeyType, Comparator>::isFull() const
{
	return (size == maxSize);
}

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>::writeLevels() const
{
	int toPrintCounter = 1;
	int printedCounter = 0;
	for(int i = 0; i < size; i++, printedCounter++)
	{
		if(printedCounter == toPrintCounter)
		{
			cout << endl;
			printedCounter = 0;
			toPrintCounter *= 2;
		}
		cout << dataItems[i].getPriority() << " ";
	}
	cout << endl;
}

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: showStructure () const

// Outputs the priorities of the data items in a heap in both array
// and tree form. If the heap is empty, outputs "Empty heap". This
// operation is intended for testing/debugging purposes only.

{
    int j;   // Loop counter

    cout << endl;
    if ( size == 0 )
       cout << "Empty heap" << endl;
    else
    {
       cout << "size = " << size << endl;       // Output array form
       for ( j = 0 ; j < maxSize ; j++ )
           cout << j << "\t";
       cout << endl;
       for ( j = 0 ; j < size ; j++ )
           cout << dataItems[j].getPriority() << "\t";
       cout << endl << endl;
       showSubtree(0,0);                        // Output tree form
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: showSubtree ( int index, int level ) const

// Helper function for the showStructure() function. Outputs the
// subtree (subheap) whose root is stored in dataItems[index]. Argument
// level is the level of this dataItems within the tree.

{
     int j;   // Loop counter

     if ( index < size )
     {
        showSubtree(2*index+2,level+1);        // Output right subtree
        for ( j = 0 ; j < level ; j++ )        // Tab over to level
            cout << "\t";
        cout << " " << dataItems[index].getPriority();   // Output dataItems's priority
        if ( 2*index+2 < size )                // Output "connector"
           cout << "<";
        else if ( 2*index+1 < size )
           cout << "\\";
        cout << endl;
        showSubtree(2*index+1,level+1);        // Output left subtree
    }
}
