//////////////////////////////////////////////////////////////////////
/*
    FileName:     	HashTable.cpp
   
    Description: 	Implementation for a Hash Table Class
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 11/2/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the class and member
	declarations for the HashTable class
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include "HashTable.h"
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
 @pre	New HashTable Class
 @post 	dynamically allocated array of size initTableSize
 @param initTableSize the size of the array to be formed

 Algorithm:
	- assign table size to the init tablesize
	- dynamically declare array

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
template < typename DataType, class KeyType >
HashTable<DataType, KeyType>::HashTable(int initTableSize)
{
	tableSize = initTableSize;
	dataTable = new BSTree<DataType, KeyType>[initTableSize];
}

/**
*********************************************************
 @pre	new HashTable class

 @post 	a deep copy of source HashTable

 @param other Source HashTable to be deep copied

 Algorithm:
	- Use overloaded assignment operator to copy the data
	  from the source to this class

***********************************************************
*/
template < typename DataType, class KeyType >
HashTable<DataType, KeyType>::HashTable(const HashTable<DataType, KeyType>& other)
{
	*this = other;
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a deep copy of source HashTable

 @param source Source BSTree to be deep copied

 Algorithm:
	- Use copyTable to make a copy of the HashTable

***********************************************************
*/
template < typename DataType, class KeyType >
HashTable<DataType, KeyType>& HashTable<DataType, KeyType>::operator=(const HashTable<DataType, KeyType>& other)
{
	if(!isEmpty())
	{
		clear();
	}
	copyTable(other);
	return this;
}

/**
*********************************************************
 @pre	a HashTable

 @post 	deallocated HashTable

 Algorithm:
	- Use clear member function to deallocate the HashTable

***********************************************************
*/
template < typename DataType, class KeyType >
HashTable<DataType, KeyType>::~HashTable()
{
	clear();
}

/**
*********************************************************
 @pre	a HashTable

 @post 	a new node inserted into the BSTree at the appropriate array 
		location of the HashTable

 @param newDataItem Data to be inserted into the HashTable

 Algorithm:
	- Hash the data item's key and use the return value as a location
	- Insert into BST the new data at that location

***********************************************************
*/
template < typename DataType, class KeyType >
void HashTable<DataType, KeyType>::insert(const DataType& newDataItem)
{
	dataTable[ newDataItem.hash(newDataItem.getKey()) % tableSize].insert(newDataItem);
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a BST removed from the HashTable if 

 @return Returns true if deleted, false if not

 @param deleteKey The key value to be searched for in the HashTable

 Algorithm:
	- Use BST remove to remove the key at the Hashed value of the delete Key

***********************************************************
*/
template < typename DataType, class KeyType >
bool HashTable<DataType, KeyType>::remove(const KeyType& deleteKey)
{
	DataType temp;
	return dataTable[ temp.hash(deleteKey) % tableSize ].remove(deleteKey);
}

/**
*********************************************************
 @pre	a HashTable

 @post 	a copy of the dataItem found

 @return Returns true if found, false if not

 @param searchKey The key value to be searched for in the tree
 @param returnItem The reference value to be changed if found

 Algorithm:
	- Hash the searchKey
	- Use BST retrieve to get the data from the BST at location
	- Return success of BST retrieve

***********************************************************
*/
template < typename DataType, class KeyType >
bool HashTable<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& returnItem) const
{
	return dataTable[ returnItem.hash(searchKey) % tableSize].retrieve(searchKey, returnItem);
}

/**
*********************************************************
 @pre	a HashTable

 @post 	a deallocated HashTable

 Algorithm:
	- For every location in the table, clear the BST

***********************************************************
*/
template < typename DataType, class KeyType >
void HashTable<DataType, KeyType>::clear()
{
	for(int i = 0; i < tableSize; i++)
	{
		dataTable[i].clear();
	}
}

/**
*********************************************************
 @pre	a HashTable

 @post 	returns if the HashTable is empty

 @return Returns true if every point in the array contains an empty tree

 Algorithm:
	- return false if any location of the data table is not empty
	- otherwise, return true

***********************************************************
*/
template < typename DataType, class KeyType >
bool HashTable<DataType, KeyType>::isEmpty() const
{
	for(int i = 0; i < tableSize; i++)
	{
		if(!dataTable[i].isEmpty())
		{
			return false;
		}
	}
	return true;
}
	
template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::showStructure() const 
{
    for (int i = 0; i < tableSize; ++i) 
	{
		cout << i << ": ";
		dataTable[i].writeKeys();
    }
}

/**
*********************************************************
 @pre	a Hash Table

 @post 	a deep copy of source HashTable

 @param source Source HashTable to be copied

 Algorithm:
	- clear the table
	- delete the prior array
	- dynamically allocate new array
	- at each array location, assign the source BST to the data BST
***********************************************************
*/
template < typename DataType, class KeyType >
void HashTable<DataType, KeyType>::copyTable(const HashTable& source)
{
	clear();
	delete dataTable;
	dataTable = new HashTable[source.tableSize];
	for(int i = 0; i < source.tableSize; i++)
	{
		dataTable[i] = source.dataTable[i];
	}
}

	