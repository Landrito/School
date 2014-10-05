#include <stdexcept>
#include <iostream>
#include "QueueArray.h"
using namespace std;

const int EMPTY_QUEUE_VAL = -1;

//--------------------------------------------------------------------

template <typename DataType>
QueueArray<DataType>::QueueArray(int maxNumber)
/*
Function name: none
Abstract: default constructor
Requirements: none
Results: Constructor. Creates an empty queue. The parameter is provided for call compatibility with the array implementation and is ignored.
*/
{
	maxSize = maxNumber;
	dataItems = new DataType[maxSize];
	front = EMPTY_QUEUE_VAL;
	back = EMPTY_QUEUE_VAL;
}

//--------------------------------------------------------------------
    
template <typename DataType>
QueueArray<DataType>::QueueArray(const QueueArray& other)
/*
Function Name: none
Abstract: copy Constructor
Requirements: none
Results: Copy Constructor. Creates a copy of another queue by using an overloaded assignment operator
*/
{
	*this = other;
}

//--------------------------------------------------------------------

template <typename DataType>
QueueArray<DataType>& QueueArray<DataType>::operator=(const QueueArray& other)
/*
Function Name: none
Abstract: Overloaded Assignment Operator
Requirements: none
Results: Sets the queue to be equivalent to the other Queue object parameter and returns a reference to the modified queue.
*/
{
	if(!isEmpty()) clear();
	
	if(!other.isEmpty()){
		maxSize = other.maxSize;
		front = other.front;
		back = other.back;
		dataItems = new DataType[maxSize];
	
		for(int i = 0; i < maxSize; i++){
			dataItems[i] = other.dataItems[i];
		}
	}
	
	else this = new QueueArray();
	/*{
		maxSize = MAX_QUEUE_SIZE;
		dataItems = new DataType[maxSize];
		front = EMPTY_QUEUE_VAL;
		back = EMPTY_QUEUE_VAL;
	}*/
}

//--------------------------------------------------------------------

template <typename DataType>
QueueArray<DataType>::~QueueArray()
/*
Function Name: none
Abstract: Desructor
Requirements: none
Results: Deallocates the memory used to store the queue.
*/
{
	clear();
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueArray<DataType>::enqueue(const DataType& newDataItem) throw (logic_error)
/*
Function Name: enqueue
Abstract: Data Insertion Function
Requirements: Queue not full
Results: Inserts new data item at the rear of the queue/
*/
{
	if(!isFull()){
		if(isEmpty()){
			front = 0;
			back = -1;
		}
		dataItems[(back + 1) % maxSize] = newDataItem;
		back = (back + 1) % maxSize;
	}
}

//--------------------------------------------------------------------

template <typename DataType>
DataType QueueArray<DataType>::dequeue() throw (logic_error)
/*
Function Name: dequeue
Abstract: Data Removal Function
Requirements: list not empty
Results: Removes the least recently added data item from the queue and returns it.
*/
{
	DataType temp = dataItems[front];
	if(front != back) front = (front + 1) % maxSize;
	else{
		front = EMPTY_QUEUE_VAL;
		back = EMPTY_QUEUE_VAL;
	}
	return temp;
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueArray<DataType>::clear()
/*
Function Name: clear
Abstract: Full Data Removal Function
Requirements: none
Results: Removes all the data items in the queue.
*/
{
	delete [] dataItems;
	dataItems = new DataType[maxSize];
	front = EMPTY_QUEUE_VAL;
	back = EMPTY_QUEUE_VAL;
}

//--------------------------------------------------------------------

template <typename DataType>
bool QueueArray<DataType>::isEmpty() const
/*
Function Name: isEmpty
Abstract: Empty Check Function
Requirements: none
Results: Returns true if the queue is empty. Otherwise returns false.
*/
{
	return( (front == EMPTY_QUEUE_VAL || back == EMPTY_QUEUE_VAL) );
}

//--------------------------------------------------------------------

template <typename DataType>
bool QueueArray<DataType>::isFull() const
/*
Function Name: isFull
Abstract: Full Check Function
Requirements: none
Results: Returns true if queue is full. Otherwise, returns false. (Linked list application assumes adequate amount of memory in RAM thus always returns false.)
*/
{
	return( ((back - front + 1) % maxSize) == 0 );
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueArray<DataType>::putFront(const DataType& newDataItem) throw (logic_error)
/*
Function Name: putFront
Abstract: Data Insertion Function
Requirements: Queue not full
Results: Inserts a new data item at the front of the queue. The order of the preexisting data items is left unchanged.
*/
{
	if(!isFull()){
		if(isEmpty()){
			front = 1;
			back = 0;
		}
		dataItems[((front - 1) + maxSize) % maxSize] = newDataItem;
		front = (((front - 1) + maxSize) % maxSize);
	}
}

//--------------------------------------------------------------------

template <typename DataType>
DataType QueueArray<DataType>::getRear() throw (logic_error)
/*
Function Name: getRear
Abstract: Data Getter/Removal Function
Requirements: none
Results: Removes the most recently added data item from the queue and retruns it. the remainder of the queue is left unchanged.
*/
{
	DataType temp = dataItems[back];
	if(front != back) back = (((back - 1) + maxSize) % maxSize);
	else{
		front = EMPTY_QUEUE_VAL;
		back = EMPTY_QUEUE_VAL;
	}
	return temp;
}

//--------------------------------------------------------------------

template <typename DataType>
int QueueArray<DataType>::getLength() const
/*
Function Name: getLength
Abstract: Size Getter Function
Requirements: none
Results: Returns the number of data items in a queue
*/
{
	if(isEmpty()) return 0;
	else if(front < back) return back - front + 1;
	else return (((back - front) + 1) + maxSize);
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueArray<DataType>::showStructure() const
/*
Function Name: showStructure
Abstract: Data Observation Function
Requirements: none
Results: outputs the data items in the queue. If the queue is empty, outputs "Empty Queue". Note that this operation is intended for testing/debugging purposes only. it only supports queue data items that are one of C++'s predefined data types (int, char, and so forth) or other data structures with an overridden ostream operator <<.
*/
{
    int j;   // Loop counter

    if ( front == -1 )
       cout << "Empty queue" << endl;
    else
    {
       cout << "Front = " << front << "  Back = " << back << endl;
       for ( j = 0 ; j < maxSize ; j++ )
           cout << j << "\t";
       cout << endl;
       if ( back >= front )
          for ( j = 0 ; j < maxSize ; j++ )
              if ( ( j >= front ) && ( j <= back ) )
                 cout << dataItems[j] << "\t";
              else
                 cout << " \t";
       else
          for ( j = 0 ; j < maxSize ; j++ )
              if ( ( j >= front ) || ( j <= back ) )
                 cout << dataItems[j] << "\t";
              else
                 cout << " \t";
       cout << endl;
    }
}
	
	
	
	
	
	
	