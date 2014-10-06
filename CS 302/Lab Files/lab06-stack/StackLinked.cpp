#include <stdexcept>
#include <iostream>
#include "StackLinked.h"
using namespace std;

//--------------------------------------------------------------------

template <typename DataType>
StackLinked<DataType>::StackLinked(int maxNumber)

//Default Constructor. Initializes the top pointer to point to NULL.
{
	top = NULL;
}

//--------------------------------------------------------------------

template <typename DataType>
StackLinked<DataType>::StackLinked(const StackLinked& other)

//Copy Constructor. Uses the "this" pointer and the overloaded assignment
//operator to copy the contents of the other stack to this stack
{
	*this = other;
}

//--------------------------------------------------------------------

template <typename DataType>
StackLinked<DataType>& StackLinked<DataType>::operator=(const StackLinked& other)

//Overloaded Assignment Operator. Clears this stack if not empty
//then if the other stack is not empty, creates new stack nodes with 
//data from the other stack and points the nodes to the next node
{
	StackNode* cursor = other.top;
	StackNode* newNode;
	StackNode* thisCursor;

	if(!isEmpty()) clear();
	if(!other.isEmpty()){
		newNode = new StackNode(cursor->dataItem, NULL);
		top = newNode;
		thisCursor = newNode;
		cursor = cursor->next;
		while(cursor != NULL){  
			newNode = new StackNode(cursor->dataItem, NULL);
			thisCursor->next = newNode;
			thisCursor = newNode;
			cursor = cursor->next;
		}
	}
	return *this;
}

//--------------------------------------------------------------------

template <typename DataType>
StackLinked<DataType>::~StackLinked()

//Desctructor. Uses the clear function to delete the stack.
{
	clear();
}

//--------------------------------------------------------------------

template <typename DataType>
StackLinked<DataType>::StackNode::StackNode(const DataType& nodeData, StackNode* nextPtr)

//StackNode Constructor. Takes in data and a pointer and assigns the data
//accordingly.
{
	dataItem = nodeData;
	next = nextPtr;
}

//--------------------------------------------------------------------

template <typename DataType>
void StackLinked<DataType>::push(const DataType& newDataItem) throw (logic_error)

//Push Function. Creates a new stack node at top
//and has it point to the previous top
{
	top = new StackNode(newDataItem, top);
}

//--------------------------------------------------------------------

template <typename DataType>
DataType StackLinked<DataType>::pop() throw (logic_error)

//Pop Function. Deletes the top node, moving top pointer
//to the next node and returns value stored in top node.
{
	DataType ret = top->dataItem;
	StackNode *temp = top;
	top = top->next;
	delete temp;
	return ret;
}

//--------------------------------------------------------------------

template <typename DataType>
void StackLinked<DataType>::clear()

//Clear Function. Pops the stack until it is empty.
{
	while(!isEmpty()) pop();
}

//--------------------------------------------------------------------

template <typename DataType>
bool StackLinked<DataType>::isEmpty() const

//IsEmpty Function. Returns whether the top is pointing to nothing
//indicating status of the stack
{
	return(top == NULL);
}

//--------------------------------------------------------------------

template <typename DataType>
bool StackLinked<DataType>::isFull() const

//IsFull function. Since its a linked list implementation always
//returns false.

{
	return false;
}

//--------------------------------------------------------------------

template <typename DataType>
void StackLinked<DataType>::showStructure() const 

// ShowStructure function. Linked list implementation. Outputs the data 
// elements in a stack. If the stack is empty, outputs "Empty stack". 
// This operation isintended for testing and debugging purposes only.
{
    if( isEmpty() )
    {
	cout << "Empty stack" << endl;
    }
    else
    {
        cout << "Top\t";
	for (StackNode* temp = top; temp != 0; temp = temp->next) {
	    if( temp == top ) {
		cout << '[' << temp->dataItem << ']'<< '\t';
	    }
	    else {
		cout << temp->dataItem << '\t';
	    }
	}
        cout << "Bottom" << endl;
    }

}

