//////////////////////////////////////////////////////////////////////
/*
    FileName:     	HashTable.cpp
   
    Description: 	Implementation for a Hash Table Class
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 11/18/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the class and member
	declarations for the Priority Queue class
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include <stdexcept>
#include <iostream>
#include "PriorityQueue.h"
using namespace std;
//
//  Global Constant Definitions  ////////////////////////////////////
//
/////  NONE
//
//  Class Definitions  //////////////////////////////////////////////
//
/////  NONE
//
//  Free Function Prototypes  ///////////////////////////////////////
//
/////  NONE
//
//  Main Function Implementation  ///////////////////////////////////
//
/////  NONE
//
//  Free Function Implementation  ///////////////////////////////////
//
/////  NONE
//
//  Class/Data Structure Member Implementation //////////////////////
//

/**
*********************************************************
 @pre	a Heap class

 @post 	a deep copy of source heap

 @param source Source Heap to be deep copied

 Algorithm:
	- if the heap is not empt and it isnt itself clear the heap
	- copy the data

***********************************************************
*/
template <typename DataType, typename KeyType, typename Comparator>
PriorityQueue<DataType, KeyType, Comparator>& PriorityQueue<DataType, KeyType, Comparator>::operator=(const PriorityQueue& other)
{
	if(!Heap<DataType, KeyType, Comparator>::isEmpty() && Heap<DataType, KeyType, Comparator>::other.dataItems != Heap<DataType, KeyType, Comparator>::dataItems)
	{
		Heap<DataType, KeyType, Comparator>::clear();
	}
	
	Heap<DataType, KeyType, Comparator>::maxSize = Heap<DataType, KeyType, Comparator>::other.maxSize;
	Heap<DataType, KeyType, Comparator>::size = Heap<DataType, KeyType, Comparator>::other.size;
	Heap<DataType, KeyType, Comparator>::dataItems = new DataType[Heap<DataType, KeyType, Comparator>::maxSize];
	for(int i = 0; i < Heap<DataType, KeyType, Comparator>::maxSize; i++)
	{
		Heap<DataType, KeyType, Comparator>::dataItems[i] = Heap<DataType, KeyType, Comparator>::other.dataItems[i];
	}
	return *this;
}


/**
*********************************************************
 @pre	a Priority Queue

 @post 	a new data inserted at the appropriate array 
		location of the Heap

 @param newDataItem Data to be inserted into the Heap

 Algorithm:
	- use the Heap insert function

***********************************************************
*/
template <typename DataType, typename KeyType, typename Comparator>
void PriorityQueue<DataType, KeyType, Comparator>::enqueue(const DataType &newDataItem)
{
	Heap<DataType, KeyType, Comparator>::insert(newDataItem);
}

/**
*********************************************************
 @pre	a Priority Queue

 @post 	Priority Queue with front removed

 @return returns the data at the front of the Queue

 Algorithm:
	- use heap remove function

***********************************************************
*/
template <typename DataType, typename KeyType, typename Comparator>
DataType PriorityQueue<DataType, KeyType, Comparator>::dequeue()
{
	return this->Heap<DataType>::remove();
}

/**
*********************************************************
 @pre	a Priority Queue

 @post 	a cleared Priority Queue

 Algorithm:
	- use heap clear function

***********************************************************
*/
template <typename DataType, typename KeyType, typename Comparator>
void PriorityQueue<DataType, KeyType, Comparator>::clear()
{
	Heap<DataType, KeyType, Comparator>::clear();
}

/**
*********************************************************
 @pre	a Priority Queue

 @post 	returns if the Priority Queue is empty

 @return returns if the Priority Queue is empty

 Algorithm:
	- use heap is empty function

***********************************************************
*/
template <typename DataType, typename KeyType, typename Comparator>
bool PriorityQueue<DataType, KeyType, Comparator>::isEmpty()
{
	return Heap<DataType, KeyType, Comparator>::isEmpty();
}

/**
*********************************************************
 @pre	a Priority Queue

 @post 	returns if the Priority Queue is full

 @return returns if the Priority Queue is full

 Algorithm:
	- use heap is empty function

***********************************************************
*/
template <typename DataType, typename KeyType, typename Comparator>
bool PriorityQueue<DataType, KeyType, Comparator>::isFull()
{
	return Heap<DataType, KeyType, Comparator>::isFull();
}