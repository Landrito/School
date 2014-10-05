//////////////////////////////////////////////////////////////////////
/*
    FileName:     	BSTree.cpp
   
    Description: 	Implementation for an expression tree data structure
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 10/30/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the class and member
	declarations for the BSTree class
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include <stdexcept>
#include <iostream>
#include "BSTree.h"

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
 @pre	New BSTree Class
 @post 	root pointing to null

 Algorithm:
	- point root to null

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
template < typename DataType, class KeyType > 
BSTree<DataType, KeyType >::BSTree()
{
	root = NULL;
}

/**
*********************************************************
 @pre	new BSTree class

 @post 	a deep copy of source BSTree

 @param source Source BSTree to be deep copied

 Algorithm:
	- Use overloaded assignment operator to copy the data
	  from the source to this class

***********************************************************
*/
template < typename DataType, class KeyType > 
BSTree<DataType, KeyType>::BSTree(const BSTree<DataType,KeyType>& other)
{
	*this = other;
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a deep copy of source BSTree

 @param source Source BSTree to be deep copied

 Algorithm:
	- Use recCopy to make a copy of each node in the BSTree

***********************************************************
*/
template < typename DataType, class KeyType > 
BSTree<DataType, KeyType>& BSTree<DataType, KeyType>::operator=(const BSTree<DataType,KeyType>& other)
{
	clear();
	recCopy(root, other.root);
	return *this;
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	deallocated BSTreeClass

 Algorithm:
	- Use clear member function to deallocate the tree

***********************************************************
*/
template < typename DataType, class KeyType > 
BSTree<DataType, KeyType>::~BSTree()
{
	clear();
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a new node inserted into the BSTree

 @param newDataItem Data to be inserted into the BSTree

 Algorithm:
	- Use recInsert member function 

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::insert(const DataType& newDataItem)
{
	recInsert(newDataItem, root);
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a copy of the dataItem found

 @return Returns true if found, false if not

 @param searchKey The key value to be searched for in the tree
 @param searchDataItem The reference value to be changed if found

 Algorithm:
	- Use recRetrieve member function 

***********************************************************
*/
template < typename DataType, class KeyType > 
bool BSTree<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& searchDataItem ) const
{
	return recRetrieve(searchKey, searchDataItem, root);
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a data item removed from the list if found

 @return Returns true if deleted, false if not

 @param deleteKey The key value to be searched for in the tree

 Algorithm:
	- Use recRemove member function

***********************************************************
*/
template < typename DataType, class KeyType > 
bool BSTree<DataType, KeyType>::remove(const KeyType& deleteKey)
{
	return recRemove(deleteKey, root);
}


/**
*********************************************************
 @pre	a BSTreeClass

 @post 	The key values of the list printed in ascending order

 Algorithm:
	- Use recWriteKeys member function 

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::writeKeys() const
{
	recWriteKeys(root);
	cout << endl;
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	a deallocated tree

 Algorithm:
	- Use recClear member function to make the expression tree

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::clear()
{
	recClear(root);
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	returns if the tree is empty

 @return Returns true if root is pointing to NULL

 Algorithm:
	- return true if root is pointing to NULL

***********************************************************
*/
template < typename DataType, class KeyType > 
bool BSTree<DataType, KeyType>::isEmpty() const
{
	return (root == NULL);
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	returns the height of the tree

 @return returns the height of the tree

 Algorithm:
	- use recGetHeight member function

***********************************************************
*/
template < typename DataType, class KeyType > 
int BSTree<DataType, KeyType>::getHeight() const
{
	int ret = 0;
	recGetHeight(root, 0, ret);
	return ret;
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	returns the number of nodes in the tree

 @return returns the height of the tree

 Algorithm:
	- use recGetCount member function

***********************************************************
*/
template < typename DataType, class KeyType > 
int BSTree<DataType, KeyType>::getCount() const
{
	return recGetCount(root);
}

/**
*********************************************************
 @pre	a BSTreeClass

 @post 	prints the key values less than the one given

 @param searchKey the key value to upper bound the list

 Algorithm:
	- use recWriteLessThan member function

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::writeLessThan( const KeyType& searchKey ) const
{
	recWriteLessThan(searchKey, root);
}

/**
*********************************************************
 @pre	an new BSTreeNode

 @post 	initialized BSTreeNode

 @param nodeDataItem Element to be stored in the node
 @param *leftPtr Pointer to the next left node
 @param *rightPtr Pointer to the next right node

 Algorithm:
	- Assign parameters to the corresponding variables

***********************************************************
*//*
template < typename DataType, class KeyType > 
BSTree<DataType, KeyType>::BSTreeNode::BSTreeNode ( const DataType &nodeDataItem, BSTreeNode *leftPtr, BSTreeNode *rightPtr )
{
	dataItem = nodeDataItem;
	left = leftPtr;
	right = rightPtr;
}*/

/**
*********************************************************
 @pre	an new BSTreeNode

 @post 	inserted Value in the tree

 @param newDataItem new node data to be put into the tree
 @param *node Pointer to current node

 Algorithm:
	- check if youre at a leaf, if so insert the data
	- check if you found a value of the same key, if so update
	- check if your key is greater than current, if so traverse right
	- else traverse left

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::recInsert(const DataType& newDataItem, BSTreeNode * & node)
{
	if(node == NULL)
	{
		node = new BSTreeNode(newDataItem, NULL, NULL);
	}
	else
	{
		if((node->dataItem).getKey() == newDataItem.getKey())
		{
			node->dataItem = newDataItem;
		}
		else if((node->dataItem).getKey() > newDataItem.getKey())
		{
			recInsert(newDataItem, node->left);
		}
		else if((node->dataItem).getKey() < newDataItem.getKey())
		{
			recInsert(newDataItem, node->right);
		}
	}
}

/**
*********************************************************
 @pre	a BSTreeNode

 @post 	a copy of the search data item

 @return Returns if successful

 @param searchKey The key to be searched for in the tree.
 @param searchDataItem The reference variable to store the found data
 @param node the location in the tree to be checked

 Algorithm:
	- check if youre at the end of the list
	- check if you are at the right key if so assign data
	- else check if key is less than location, if so call function to left
	- else call function to the right
	- if reaches end of the list return false

***********************************************************
*/
template < typename DataType, class KeyType > 
bool BSTree<DataType, KeyType>::recRetrieve(const KeyType& searchKey, DataType& searchDataItem, BSTreeNode * node) const
{
	if(node != NULL)
	{
		if(searchKey == (node->dataItem).getKey())
		{
			searchDataItem = node->dataItem;
			return true;
		}
		else if(searchKey < (node->dataItem).getKey())
		{
			return recRetrieve(searchKey, searchDataItem, node->left);
		}
		else
		{
			return recRetrieve(searchKey, searchDataItem, node->right);
		}
	}
	else
	{
		return false;
	}
}

/**
*********************************************************
 @pre	a BSTreeNode

 @post 	node removed from the list

 @return Returns if successful

 @param deleteKey The key to be searched for in the tree.
 @param node the location in the tree to be checked

 Algorithm:
	- check if youre at the end of the list
	- check if you're at the right location if so move on
	- check if you are at a leaf if so delete the node
	- else check if there is one child that is null
	- if there is one child that is null delete the node
	- else find the node just less than the node, replace it and delete the node
	- if reaches end of the list return false

***********************************************************
*/
template < typename DataType, class KeyType > 
bool BSTree<DataType, KeyType>::recRemove(const KeyType& deleteKey, BSTreeNode * & node)
{
	if(node != NULL)
	{
		if(deleteKey == (node->dataItem).getKey())
		{
			if(node->left == NULL && node->right == NULL)
			{
				delete node;
				node = NULL;
			}
			else if(node->left == NULL)
			{
				BSTreeNode *temp = node;
				node = node->right;
				delete temp;
			}
			else if(node->right == NULL)
			{
				BSTreeNode *temp = node;
				node = node->left;
				delete temp;
			}
			else
			{
				BSTreeNode *temp = node->left;
				while(temp->right != NULL)
				{
					temp = temp->right;
				}
				node->dataItem = temp->dataItem;
				recRemove((temp->dataItem).getKey(), node->left);
			}
			return true;
		}
		else if(deleteKey < (node->dataItem).getKey())
		{
			return recRemove(deleteKey, node->left);
		}
		else
		{
			return recRemove(deleteKey, node->right);
		}
	}
	else
	{
		return false;
	}
}

/**
*********************************************************
 @pre	a BSTreeNode

 @post 	printed list of the keys

 @param node the location in the tree to be checked

 Algorithm:
	- in order traversal of the tree printing the nodes

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::recWriteKeys(BSTreeNode * node) const
{
	if(node != NULL)
	{
		recWriteKeys(node->left);
		cout << (node->dataItem).getKey() << " ";
		recWriteKeys(node->right);
	}
}

/**
*********************************************************
 @pre	an BSTreeClass

 @post 	deallocated BSTreeClass

 @param node the node to be deleted.

 Algorithm:
	- Post order traverse the tree, deleting the node when bottom of tree is reached

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::recClear(BSTreeNode * & node)
{
	if(node != NULL)
	{
		recClear(node->left);
		recClear(node->right);
		delete node;
		if(node->left != NULL)
		{
			node->left = NULL;
		}
		if(node->right != NULL)
		{
			node->right = NULL;
		}
		node = NULL;
	}
}
	
/**
*********************************************************
 @pre	a BSTreeNode

 @post 	returned value of the height of the tree

 @param node the location in the tree
 @param currentLevel the current level of the tree
 @param maxLevel reference to the value to be returned

 Algorithm:
	- if at a leaf check if level is greater than max, if so return the value
	- if not at a leaf go to children

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::recGetHeight(BSTreeNode * node, int currentLevel, int & maxLevel) const
{
	if(node == NULL)
	{
		if(currentLevel > maxLevel)
		{
			maxLevel = currentLevel;
		}
	}
	else
	{
		recGetHeight(node->left, currentLevel + 1, maxLevel);
		recGetHeight(node->right, currentLevel + 1, maxLevel);
	}
}

/**
*********************************************************
 @pre	a BSTreeNode

 @post 	returned amount of nodes in the tree

 @return Amount of nodes in the tree

 @param node the location in the tree

 Algorithm:
	- if at a null position return 0
	- else return 1 plus recursive call on the children

***********************************************************
*/
template < typename DataType, class KeyType > 
int BSTree<DataType, KeyType>::recGetCount(BSTreeNode * node) const
{
	if(node == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + recGetCount(node->left) + recGetCount(node->right);
	}
}

/**
*********************************************************
 @pre	a BSTreeNode

 @post 	printed list of the keys

 @param node the location in the tree to be checked

 Algorithm:
	- in order traversal of the tree printing the nodes only printing the trees
	  if the key is less than the search key

***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::recWriteLessThan(const KeyType& searchKey, BSTreeNode * node) const
{
	if(node != NULL)
	{
		recWriteLessThan(searchKey, node->left);
		if(searchKey > (node->dataItem).getKey())
		{
			cout << (node->dataItem).getKey() << " ";
		}
		recWriteLessThan(searchKey, node->right);
	}
}

/**
*********************************************************
 @pre	an BSTreeClass

 @post 	a deep copy of source BSTree

 @param node location in this tree
 @param otherNode location in source tree

 Algorithm:
	- if the source node is not at a NULL location
		-create temp nodes ptrs and copy those from source
		-create a node and point to the temp nodes
***********************************************************
*/
template < typename DataType, class KeyType > 
void BSTree<DataType, KeyType>::recCopy(BSTreeNode * & node, BSTreeNode * otherNode)
{
	if(otherNode != NULL)
	{
		BSTreeNode *tempLeft = NULL;
		BSTreeNode *tempRight = NULL;
		recCopy(tempLeft, otherNode->left);
		recCopy(tempRight, otherNode->right);
		node = new BSTreeNode(otherNode->dataItem, tempLeft, tempRight);
	}
}


template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>:: showStructure () const

// Outputs the keys in a binary search tree. The tree is output
// rotated counterclockwise 90 degrees from its conventional
// orientation using a "reverse" inorder traversal. This operation is
// intended for testing and debugging purposes only.

{
    if ( root == 0 )
       cout << "Empty tree" << endl;
    else
    {
       cout << endl;
       showHelper(root,1);
       cout << endl;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>:: showHelper ( BSTreeNode *p,
                               int level             ) const

// Recursive helper for showStructure. 
// Outputs the subtree whose root node is pointed to by p. 
// Parameter level is the level of this node within the tree.

{
     int j;   // Loop counter

     if ( p != 0 )
     {
        showHelper(p->right,level+1);         // Output right subtree
        for ( j = 0 ; j < level ; j++ )    // Tab over to level
            cout << "\t";
        cout << " " << p->dataItem.getKey();   // Output key
        if ( ( p->left != 0 ) &&           // Output "connector"
             ( p->right != 0 ) )
           cout << "<";
        else if ( p->right != 0 )
           cout << "/";
        else if ( p->left != 0 )
           cout << "\\";
        cout << endl;
        showHelper(p->left,level+1);          // Output left subtree
    }
}
