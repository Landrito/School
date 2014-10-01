//////////////////////////////////////////////////////////////////////
/*
    FileName:     	ExpressionTree.cpp
   
    Description: 	Implementation for an expression tree data structure
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 10/23/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the class and member
	declarations for the expressionTree class
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include <iostream>
#include "ExpressionTree.h"
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
 @pre	New ExprTree Class
 @post 	root pointing to null

 Algorithm:
	- point root to null

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
template <typename DataType>
ExprTree<DataType>::ExprTree()
{
	root = NULL;
}

/**
*********************************************************
 @pre	new ExprTree class

 @post 	a deep copy of source ExprTree

 @param source Source ExprTree to be deep copied

 Algorithm:
	- Use overloaded assignment operator to copy the data
	  from the source to this class

***********************************************************
*/
template <typename DataType>
ExprTree<DataType>::ExprTree(const ExprTree& source)
{
	*this = source;
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	a deep copy of source ExprTree

 @param source Source ExprTree to be deep copied

 Algorithm:
	- Use recCopy to make a copy of each node in the ExprTree

***********************************************************
*/
template <typename DataType>
ExprTree<DataType>& ExprTree<DataType>::operator=(const ExprTree<DataType>& source)
{
	if(root != NULL)
	{
		clear();
	}
	recCopy(root, source.root);
	return *this;
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	deallocated ExprTreeClass

 Algorithm:
	- Use clear member function to deallocate the tree

***********************************************************
*/
template <typename DataType>
ExprTree<DataType>::~ExprTree()
{
	clear();
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	a built Expression tree

 Algorithm:
	- Use recBuild member function to make the expression tree

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::build()
{
	recBuild(root);
}

/**
*********************************************************
 @pre	a built ExprTreeClass

 @post 	The mathematical expression is printed on screen infix notation

 Algorithm:
	- Use recExpression member function to print the expression

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::expression() const
{
	recExpression(root);
}

/**
*********************************************************
 @pre	a built ExprTreeClass

 @post 	The mathematical expression is evaluated

 @return The mathematical evaluation of the expression of the tree

 Algorithm:
	- use the recEvaluate member function to evaluate the exprTree

 @throw A logic error is thrown if the tree is empty

***********************************************************
*/
template <typename DataType>
DataType ExprTree<DataType>::evaluate() const throw (logic_error)
{
	if(root == NULL)
	{
		throw logic_error("Empty List: Unable to Evaluate");
	}
	else
	{
		return recEvaluate(root);
	}
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	deallocated ExprTreeClass

 Algorithm:
	- use the recClear member function to deallocate the exprTree

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::clear()
{
	recClear(root);
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	commuted Expression Tree

 Algorithm:
	- use the recCommute member function to commute the exprTree

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::commute()
{
	recCommute(root);
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	returned boolean if the trees are equivalent

 @param source Source ExprTree to be compared

 @return This function returns if the trees are equivalent based on commutative properties

 Algorithm:
	- use the recIsEquivalent member function to check equivalence

***********************************************************
*/
template <typename DataType>
bool ExprTree<DataType>::isEquivalent(const ExprTree& source) const
{
	return recIsEquivalent(root, source.root);
}

/**
*********************************************************
 @pre	an new ExprTreeNode

 @post 	initialized ExprTreeNode

 @param elem Element to be stored in the node
 @param *leftPtr Pointer to the next left node
 @param *rightPtr Pointer to the next right node

 Algorithm:
	- Assign parameters to the corresponding variables

***********************************************************
*/
template <typename DataType>
ExprTree<DataType>::ExprTreeNode::ExprTreeNode( char elem, ExprTreeNode *leftPtr, ExprTreeNode *rightPtr )
{
	dataItem = elem;
	left = leftPtr;
	right = rightPtr;
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	a deep copy of source ExprTree

 @param exprTreePtr node pointer to the node in this tree
 @param source node pointer to the node in the source tree

 Algorithm:
	- if the source node is not empty build a new node based on the data
	- if the source data is an operator call the copy again.

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::recCopy(ExprTreeNode * & exprTreePtr, ExprTreeNode * source )
{
	if(source != NULL)
	{
		ExprTreeNode *tempLeft = NULL;
		ExprTreeNode *tempRight = NULL;
		switch(source->dataItem)
		{
			case '*':
			case '/':
			case '+':
			case '-':
			recCopy(tempLeft, source->left);
			recCopy(tempRight, source->right);
			exprTreePtr = new ExprTreeNode(source->dataItem, tempLeft, tempRight);
			break;
		
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			exprTreePtr = new ExprTreeNode(source->dataItem, NULL, NULL);
			break;
		
			default:
			break;
		}
	}
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	a built Expression tree

 @param exprTreePtr Node pointer of the node to be built

 Algorithm:
	- get input char
	- if it is an operator call the function for left and right node
	- create a new node and point it to the left and right nodes

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::recBuild(ExprTreeNode * & exprTreePtr)
{
	ExprTreeNode *tempLeft = NULL;
	ExprTreeNode *tempRight = NULL;
	char input;
	cin >> input;
	switch(input)
	{
		case '*':
		case '/':
		case '+':
		case '-':
			recBuild(tempLeft);
			recBuild(tempRight);
			exprTreePtr = new ExprTreeNode(input, tempLeft,  tempRight);
		break;
		
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			exprTreePtr = new ExprTreeNode(input, NULL, NULL);
		break;
	}
}

/**
*********************************************************
 @pre	a built ExprTreeClass

 @post 	The mathematical expression is printed on screen infix notation

 @param exprTreeNode The node of the tree to be expressed

 Algorithm:
	- Traverse the tree in order printing out the items with parenthesis

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::recExpression(ExprTreeNode * exprTreePtr) const
{
	if(exprTreePtr != NULL)
	{
		switch(exprTreePtr->dataItem)
		{
			case '*':
			case '/':
			case '+':
			case '-':
			cout << "( "; 
			recExpression(exprTreePtr->left);
			cout << ' ' << exprTreePtr->dataItem << ' ';
			recExpression(exprTreePtr->right);
			cout << " )";
			break; 
		
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			cout << exprTreePtr->dataItem;
			break;
		
			default:
			break; 
		}
	}
}

/**
*********************************************************
 @pre	a built ExprTreeClass

 @post 	The mathematical expression is evaluated

 @return The mathematical evaluation of the expression of the tree

 Algorithm:
	- based on the data return either the function call of left and right using the operand
	- or return the number

***********************************************************
*/
template <typename DataType>
DataType ExprTree<DataType>::recEvaluate(ExprTreeNode * exprTreePtr) const
{
	switch(exprTreePtr->dataItem)
	{
		case '*':
		return recEvaluate(exprTreePtr->left) * recEvaluate(exprTreePtr->right);
		break; 
		
		case '/':
		return recEvaluate(exprTreePtr->left) / recEvaluate(exprTreePtr->right);
		break; 
		
		case '+':
		return recEvaluate(exprTreePtr->left)+ recEvaluate(exprTreePtr->right);
		break; 
		
		case '-':
		return recEvaluate(exprTreePtr->left) - recEvaluate(exprTreePtr->right);
		break; 
		
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		return (exprTreePtr->dataItem - '0');
		break;
		
		default:
		return 0;
		break; 
	}
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	deallocated ExprTreeClass

 @param exprTreePtr the node to be deleted.

 Algorithm:
	- Post order traverse the tree, deleting the node when bottom of tree is reached

***********************************************************
*/
template <typename DataType>
void ExprTree<DataType>::recClear(ExprTreeNode * & exprTreePtr)
{
	if(exprTreePtr != NULL)
	{
		recClear(exprTreePtr->left);
		recClear(exprTreePtr->right);
		delete exprTreePtr;
		if(exprTreePtr->left != NULL)
		{
			exprTreePtr->left = NULL;
		}
		if(exprTreePtr->right != NULL)
		{
			exprTreePtr->right = NULL;
		}
		exprTreePtr = NULL;
	}
	
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	commuted Expression Tree

 @param exprTreePtr the pointer of the node to swap the below nodes.

 Algorithm:
	- for each pointer, swap the left and right nodes using a temp
	- call the function for left and right pointers

***********************************************************
*/
template <typename DataType> 
void ExprTree<DataType>::recCommute(ExprTreeNode * & exprTreePtr)
{
	if(exprTreePtr != NULL)
	{
		ExprTreeNode *temp = (exprTreePtr->right);
		exprTreePtr->right = exprTreePtr->left;
		exprTreePtr->left = temp;
		
		recCommute(exprTreePtr->left);
		recCommute(exprTreePtr->right);
	}
}

/**
*********************************************************
 @pre	an ExprTreeClass

 @post 	returned boolean if the trees are equivalent

 @param thisExprTreePtr the node of this list to be compared
 @param otherExprTreePtr the node of the other list to be compared

 @return This function returns if the trees are equivalent based on commutative properties

 Algorithm:
	- if both nodes are pointing to null return true
	- if both arent pointing to null check if they are the same
	- if they are the same, check if the left and rights are the same by calling the function recursively
	- else check if the right is equal to the left for both nodes and return true else return false

***********************************************************
*/
template <typename DataType>
bool ExprTree<DataType>::recIsEquivalent(ExprTreeNode * thisExprTreePtr, ExprTreeNode * otherExprTreePtr) const
{
	if(thisExprTreePtr == NULL && otherExprTreePtr == NULL)
	{
		return true;
	}
	else if(thisExprTreePtr != NULL && thisExprTreePtr != NULL)
	{
		if(thisExprTreePtr->dataItem == otherExprTreePtr->dataItem)
		{
			if(recIsEquivalent(thisExprTreePtr->right, thisExprTreePtr->right) 
				&&
			   recIsEquivalent(thisExprTreePtr->left, thisExprTreePtr->left))
			{
				return true;
			}
			else if
				(recIsEquivalent(thisExprTreePtr->right, thisExprTreePtr->left)
				 &&
				recIsEquivalent(thisExprTreePtr->left, thisExprTreePtr->right))
			{
				if (thisExprTreePtr->dataItem == '+' ||
				 	 thisExprTreePtr->dataItem == '*')
				{
					return true;
				}
				else
				{
					return false;
				}
			}
				
			else
			{
				return false;
			}
		}
	}
	return false;	
}

template <typename DataType>
void ExprTree<DataType>:: showStructure () const

// Outputs an expression tree. The tree is output rotated counter-
// clockwise 90 degrees from its conventional orientation using a
// "reverse" inorder traversal. This operation is intended for testing
// and debugging purposes only.

{
    if ( root == NULL )
       cout << "Empty tree" << endl;
    else
    {
       cout << endl;
       showHelper(root,1);
       cout << endl;
    }
}

template <typename DataType>
void ExprTree<DataType>:: showHelper ( ExprTreeNode *p, int level ) const

// Recursive helper for the showStructure() function. Outputs the
// subtree whose root node is pointed to by p. Parameter level is the
// level of this node within the expression tree.

{
     int j;   // Loop counter

     if ( p != 0 )
     {
        showHelper(p->right,level+1);        // Output right subtree
        for ( j = 0 ; j < level ; j++ )   // Tab over to level
            cout << "\t";
        cout << " " << p->dataItem;        // Output dataItem
        if ( ( p->left != 0 ) &&          // Output "connector"
             ( p->right != 0 ) )
           cout << "<";
        else if ( p->right != 0 )
           cout << "/";
        else if ( p->left != 0 )
           cout << "\\";
        cout << endl;
        showHelper(p->left,level+1);         // Output left subtree
     }
}
//
//  Terminating Precompiler Directives ///////////////////////////////
//
/////  NONE
//



