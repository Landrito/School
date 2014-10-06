#include <iostream>
#include "config.h"
#include "ListLinked.h"

template <typename DataType>
List<DataType>::List(int ignored){ 
	/*
	Function name: none
	Abstract: default constructor
	Requirements: none
	Results: Constructor. Creates an empty list. The parameter is provided for call compatibility with the array implementation and is ignored.
	*/
	head = NULL;
	cursor = NULL;
}

template <typename DataType>
List<DataType>::List(const List& other){ //Copy Constructor
	/*
	Function Name: none
	Abstract: copy Constructor
	Requirements: none
	Results: Constructor. Creates an initialized ListNode by setting the ListNode's data item to the value nodeData and the ListNode's next pointer to the value of nextPtr.
	*/
	this = other;
}

template <typename DataType>
List<DataType>& List<DataType>::operator=(const List& other){
	/*
	Function Name: =
	Abstract: overloaded assignment operator
	Requirements: none
	Results: Overloaded assignment operator. Sets the list to be equivalent to the other List and returns a reference to this object.
	*/
	
	//Declare a list node to go through the other List
	ListNode *current;
	
	//Clear the list if it is not empty
	if(!isEmpty()) clear();
	
	//If other list is empty, set this list to NULL
	if(other.head == NULL){
		head = NULL;
		cursor = NULL;
	}
	
	//Otherwise
	else{
		//Go through the other list and insert the info.
		current = other.head;
		while(current != NULL){
			insert(current->dataItem);
			current = current->next;
		}
		//Set the cursor back to head
		cursor = head;
	}
	
	//Return the List
	return *this;
}

template <typename DataType>
List<DataType>::~List(){ //Deconstructor
	/*
	Function name: none
	Abstract: Deconstructor
	Requirements: none
	Results: Destructor. Deallocates (frees) the memory used to store the nodes in the list.
	*/
	
	//Clear the list
	clear();
}

template <typename DataType>
List<DataType>::ListNode::ListNode(const DataType& nodeData, ListNode* nextPtr){
	/*
	Function Name: none
	Abstract: Constructor
	Requirements: none
	Results: Constructor for ListNode which assigns the dataItem and the next Pointer.
	*/
	dataItem = nodeData;
	next = nextPtr;
}

template <typename DataType>
void List<DataType>::insert(const DataType& newDataItem) throw (logic_error){
	/*
	Function name: insert
	Abstract: inserts data into the list
	Requirements: List not full
	Results: Inserts a new data item into the list. If the list is not empty, then inserts new data item after the cursor. Otherwise, inserts new data item as the first (and only) data item in the list. In either case, moves the cursor to new data item.
	*/
	
	//If the list is empty, make head and cursor point to the new node.
	if(isEmpty()) head = cursor = new ListNode(newDataItem, NULL);
	
	//Otherwise make cursor poinr to newNode and make newNode point to next node.
	else{
		cursor->next = new ListNode(newDataItem, cursor->next);
		gotoNext();
	}
}

template <typename DataType>
void List<DataType>::remove() throw (logic_error){
	/*
	Function Name: remove
	Abstract: removes cursor marked node from the list
	Requirements: List not empty.
	Results: Removes the data item marked by the cursor from the list. If the resulring list is not empty then moves the cursor to the data item that followed the deleted data item. If the deleted data item was at the end of the list, then moves the cursor to the beginning of the list.
	*/
	
	//Declare a List node to go through the list and have it point to cursor
	ListNode *current;
	current = cursor;
	
	//If the list is not empty, check location of the cursor.
	if(!isEmpty()){
		//If there is only one node in the list, delete the node and set head and cursor to NULL
		if(head->next == NULL){
			delete current;
			head = NULL;
			cursor = NULL;
		}
		
		//If the cursor is pointing to the first node, move the cursor forward, delete the head, and assign head to the cursor.
		else if(cursor == head){
			gotoNext();
			delete current;
			head = cursor;
		}
		
		//Otherwise, move cursor back, assign cursor's next to the current's next, delete the current.
		else{
			gotoPrior();
			cursor->next = current->next;
			delete current;
			//If item deleted was at end of the list, then move it to the beginning
			if(cursor->next == NULL){
				gotoBeginning();
			}
			//otherwise, go to the next node.
			else{
				gotoNext();
			}
		}
	}
}

template <typename DataType>
void List<DataType>::replace(const DataType& newDataItem) throw (logic_error){
	/*
	Function Name: replace
	Abstract: replaces dataItem of cursor
	Requirements: List not empty.
	Results: Replaces the data item marked by the cursor with newDataItem. The cursor now points to the new data Item.
	*/
	
	//If the list is not empty, assign the cursor's data with newDataItem.
	if(!isEmpty()){
		cursor->dataItem = newDataItem;
	}
}

template <typename DataType>
void List<DataType>::clear(){ 
	/*
	Function Name: clear
	Abstract: clears the list
	Requirements: none
	Results: removes all data items in the list.
	*/
	
	//While the list is not empty, remove the the node the cursor is pointing to.
	while(!isEmpty()) remove();
}

template <typename DataType>
bool List<DataType>::isEmpty() const{ 
	/*
	Function Name: isEmpty
	Abstract: checks if list is empty.
	Requirements: none
	Results: returns true if the list is empty. otherwise returns false.
	*/
	
	//Return whether or not the head is pointing to NULL
	return(head == NULL);
}

template <typename DataType>
bool List<DataType>::isFull() const{
	/*
	Function Name: isFull
	Abstract: checks if list is full
	Requirements: none
	Results: returns true if the list is full. otherwise returns false. Default, always return false.
	*/
	return false;
}


template <typename DataType>
void List<DataType>::gotoBeginning() throw (logic_error){ 
	/*
	Function Name: gotoBeginning
	Abstract: moves the cursor to the beginning of the list.
	Requirements: List is not empty
	Results: Moves the cursor to the beginning of the list.
	*/
	
	//If the list is not empty, assign the cursor to the head.
	if(!isEmpty()){
		cursor = head;
	}
}

template <typename DataType>
void List<DataType>::gotoEnd() throw (logic_error){ 
	/*
	Function Name: gotoEnd
	Abstract: moves the cursor to the end of the list
	Requirements: List is not empty
	Results: Moves the cursor to the end of the list
	*/
	
	//if the list is not empty move the cursor to the end of the list.
	if(!isEmpty()){
		//until the cursor node points to NULL, assign the cursor to the next node.
		while(cursor->next != NULL){
			cursor = cursor->next;
		}
	}
}

template <typename DataType>
bool List<DataType>::gotoNext() throw (logic_error){
	/*
	Function Name: goToNext
	Abstract: Moves cursor to the next Node.
	Requirements: List is not empty.
	Results: If the cursor is not at the end of the list, then moves the cursor to mark the next data item in the list and returns true, otherwise, returns false.
	*/
	
	//If the cursor node points to NULL, return false. 
	if(cursor->next == NULL) return false;
	
	//Otherwise, have cursor point to next node and return true.
	else{
		cursor = cursor->next;
		return true;
		} 
}

template <typename DataType>
bool List<DataType>::gotoPrior() throw (logic_error){
	/*
	Function Name: goToPrior
	Abstract: Moves cursor to previous node.
	Requirements: List is not empty.
	Results: If the cursor is not at the beginning of the list, then moves the cursor to mark the preceding data item in the list and returns true. Otherwise, returns false.
	*/
	
	//Declare node to go through the list
	ListNode *current;
	
	//Return false if the cursor is at the start of the list.
	if(cursor == head) return false;
	
	//Otherwise, go through list until you get to the node pointing to the cursor and make the cursor point to it. return true
	else{
		current = head;
		while(current->next != cursor){
			current = current->next;
		}
		cursor = current;
		return true;
	}
}

template <typename DataType>
DataType List<DataType>::getCursor() const throw (logic_error){
	/*
	Function Name: getCursor
	Abstract: Returns the value of the data item marked by the cursor.
	Requirements: List is not empty
	Results: Returns the value of the data item marked by the cursor.
	*/
	
	//If the list is not empty, return the data.
	if(!isEmpty()) return cursor->dataItem;
	//otherwise return 0
	else return 0;
}

template <typename DataType>
void List<DataType>::moveToBeginning() throw (logic_error){
	/*
	Function name: moveToBeginning
	Abstract: moves the node marked by the cursor to the beginning of the list.
	Requirements: List is not empty
	Results: Removes the data item marked by the cursor from the list and reinserts the data item at the beginning of the list. Moves the cursor to the beginning of the list.
	*/
	
	//declare a new node to hold the data
	ListNode *newNode;
	
	//if the cursor isn't already at the head, set value of the new node and have it point to head and become the new head. remove the cursor, and move the cursor to head.
	if(cursor != head){
		newNode = new ListNode(cursor->dataItem, head);
		head = newNode;
		remove();
		cursor = head;
	}
}

template <typename DataType>
void List<DataType>::insertBefore(const DataType& newDataItem) throw (logic_error){
	/*
	Function Name: insertBefore
	Abstract: inserts data before the cursor
	Requirements: List is not full
	Results: Inserts newDataItem into a list. If the list is not empty, then inserts newDataItem immediately before the cursor. Otherwise, inserts newDataItem as the first (and only) data item in the list. In either case, moves the cursor to newDataItem.
	*/
	
	//if the list is not empty insert the data
	if(!isEmpty()){
		if(cursor != head) { // if the cursor is not at the beginning move the cursor back and insert the new data
			gotoPrior();
			insert(newDataItem);
		}

		else{ //otherwise insert the data then move it to the beginning.
			insert(newDataItem);
			moveToBeginning();
		}
	}
	else insert(newDataItem); //if list is empty simply insert the data.
}

template <typename DataType>
void List<DataType>::showStructure() const {
	/*
	Function Name: showStructure
	Abstract: shows the values and cursor of the list.
	Requirenemnts: none
	Results: Outputs the values in the list in order having brackets around the data item that is marked by the cursor.
	*/
	
	//initialize a node to go through the list.
	ListNode *current;
	
	//if list is empty output empty list
	if(isEmpty()){
		cout << "Empty list" << endl;
	}
	
	//otherwise, start from the head and output each value until end of list.
	else{
		current = head;
		while(current != NULL){
			
			if(cursor == current){
				cout << "[" << current->dataItem << "] ";
			} 
			else cout << current-> dataItem << " ";
			current = current->next;
		}
		cout << endl;
	}
}











