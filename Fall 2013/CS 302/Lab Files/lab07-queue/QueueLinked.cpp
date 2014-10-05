#include <stdexcept>
#include <iostream>
#include "QueueLinked.h"
using namespace std;

//--------------------------------------------------------------------

template <typename DataType>
QueueLinked<DataType>::QueueLinked(int maxNumbe)
/*
Function name: none
Abstract: default constructor
Requirements: none
Results: Constructor. Creates an empty queue. The parameter is provided for call compatibility with the array implementation and is ignored.
*/
{
	
	front = NULL;
	back = NULL;
}

//--------------------------------------------------------------------

template <typename DataType>
QueueLinked<DataType>::QueueLinked(const QueueLinked& other)
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
QueueLinked<DataType>& QueueLinked<DataType>::operator=(const QueueLinked& other)
/*
Function Name: none
Abstract: Overloaded Assignment Operator
Requirements: none
Results: Sets the queue to be equivalent to the other Queue object parameter and returns a reference to the modified queue.
*/
{
	QueueNode* cursor = other.front;
	QueueNode* newNode;
	QueueNode* thisCursor;

	if(!isEmpty()) clear();
	if(!other.isEmpty()){
		newNode = new QueueNode(cursor->dataItem, NULL);
		front = newNode;
		thisCursor = newNode;
		cursor = cursor->next;
		while(cursor != NULL){  
			newNode = new QueueNode(cursor->dataItem, NULL);
			thisCursor->next = newNode;
			thisCursor = newNode;
			if(cursor->next == NULL) back = cursor;
			cursor = cursor->next;
		}
	}
	return *this;
}

//--------------------------------------------------------------------

template <typename DataType>
QueueLinked<DataType>::~QueueLinked()
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
QueueLinked<DataType>::QueueNode::QueueNode(const DataType& nodeData, QueueNode* nextPtr)
/*
Function Name: none
Abstract: Constructor
Requirements: none
Results: Creates a queue node with the given data and points the node to the given pointer.
*/
{
	dataItem = nodeData;
	next = nextPtr;
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueLinked<DataType>::enqueue(const DataType& newDataItem) throw (logic_error)
/*
Function Name: enqueue
Abstract: Data Insertion Function
Requirements: Queue not full
Results: Inserts new data item at the rear of the queue/
*/
{
	QueueNode *newNode;
	newNode = new QueueNode(newDataItem, NULL);
	if(isEmpty()) front = back = newNode;
	else{
		back->next = newNode;
		back = back->next;
	}
}

//--------------------------------------------------------------------

template <typename DataType>
DataType QueueLinked<DataType>::dequeue() throw (logic_error)
/*
Function Name: dequeue
Abstract: Data Removal Function
Requirements: list not empty
Results: Removes the least recently added data item from the queue and returns it.
*/
{
	if(isEmpty()) throw logic_error("Empty Queue");
	DataType ret = front->dataItem;
	QueueNode *temp = front;
	front = front->next;
	delete temp;
	return ret;
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueLinked<DataType>::clear()
/*
Function Name: clear
Abstract: Full Data Removal Function
Requirements: none
Results: Removes all the data items in the queue.
*/
{
	while(!isEmpty()) dequeue();
}

//--------------------------------------------------------------------

template <typename DataType>
bool QueueLinked<DataType>::isEmpty() const
/*
Function Name: isEmpty
Abstract: Empty Check Function
Requirements: none
Results: Returns true if the queue is empty. Otherwise returns false.
*/
{
	return (front == NULL);
}

//--------------------------------------------------------------------

template <typename DataType>
bool QueueLinked<DataType>::isFull() const
/*
Function Name: isFull
Abstract: Full Check Function
Requirements: none
Results: Returns true if queue is full. Otherwise, returns false. (Linked list application assumes adequate amount of memory in RAM thus always returns false.)
*/
{
	return false;
}

template <typename DataType>
void QueueLinked<DataType>::putFront(const DataType& newDataItem) throw (logic_error)
/*
Function Name: putFront
Abstract: Data Insertion Function
Requirements: Queue not full
Results: Inserts a new data item at the front of the queue. The order of the preexisting data items is left unchanged.
*/
{
	front = new QueueNode(newDataItem, front);
}

//--------------------------------------------------------------------
    
template <typename DataType>
DataType QueueLinked<DataType>::getRear() throw (logic_error)
/*
Function Name: getRear
Abstract: Data Getter/Removal Function
Requirements: none
Results: Removes the most recently added data item from the queue and retruns it. the remainder of the queue is left unchanged.
*/
{
	if(isEmpty()) throw logic_error("Empty Queue");
	QueueNode *cursor = front;
	DataType ret = back->dataItem;
	
	if(front == back){
		dequeue();
	}
	
	else{
		while(cursor->next != back) cursor = cursor->next;
		delete back;
		back = cursor;
		cursor->next = NULL;
	}
	return ret;
}

//--------------------------------------------------------------------

template <typename DataType>
int QueueLinked<DataType>::getLength() const
/*
Function Name: getLength
Abstract: Size Getter Function
Requirements: none
Results: Returns the number of data items in a queue
*/
{
	QueueNode *cursor = front;
	int count;
	for(count = 0; cursor != NULL; count++) cursor = cursor->next;
	return count;
}

//--------------------------------------------------------------------

template <typename DataType>
void QueueLinked<DataType>::showStructure() const
/*
Function Name: showStructure
Abstract: Data Observation Function
Requirements: none
Results: outputs the data items in the queue. If the queue is empty, outputs "Empty Queue". Note that this operation is intended for testing/debugging purposes only. it only supports queue data items that are one of C++'s predefined data types (int, char, and so forth) or other data structures with an overridden ostream operator <<.
*/
{
    QueueNode *p;   // Iterates through the queue

    if ( isEmpty() )
	cout << "Empty queue" << endl;
    else
    {
	cout << "Front\t";
	for ( p = front ; p != 0 ; p = p->next )
	{
	    if( p == front ) 
	    {
		cout << '[' << p->dataItem << "] ";
	    }
	    else
	    {
		cout << p->dataItem << " ";
	    }
	}
	cout << "\trear" << endl;
    }
}

//--------------------------------------------------------------------
	
	
	
	
	
	