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
	declarations for the heap class
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include <stdexcept>
#include <iostream>
#include <climits>    // For INT_MAX
#include <string>     // Used for labels
#include "WeightedGraph.h"
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
 @pre	New Graph Class
 @post 	dynamically allocated array
 @param maxNumber the number of verteces

 Algorithm:
	- set size to zero
	- set maxSize
	- allocate arrays

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
WeightedGraph::WeightedGraph( int maxNumber )
{
	maxSize = maxNumber;
	size = 0;
	vertexList = new Vertex[maxNumber];
	adjMatrix = new int[ maxNumber * maxNumber ];
	pathMatrix = new int[ maxNumber * maxNumber ];
}

/**
*********************************************************
 @pre	new graph class

 @post 	a deep copy of source graph

 @param other Source graph to be deep copied

 Algorithm:
	- Use overloaded assignment operator to copy the data
	  from the source to this class

***********************************************************
*/
WeightedGraph::WeightedGraph( const WeightedGraph& other )
{
	*this = other;
}

/**
*********************************************************
 @pre	a Graph class

 @post 	a deep copy of source Graph

 @param source Source graph to be deep copied

 Algorithm:
	- if the heap is not empty delete teh matrices
	- and it is itself return itself
	- allocate new arrays
	- copy the data
	- return this

***********************************************************
*/
WeightedGraph& WeightedGraph::operator=( const WeightedGraph& other )
{
	if( this == &other )
	{
		return *this;
	}
	if( !isEmpty() )
	{
		delete [] vertexList;
		delete [] adjMatrix;
		delete [] pathMatrix;
	}
	
	maxSize = other.maxSize;
	size = other.size;
	vertexList = new Vertex[ maxSize ];
	adjMatrix = new int[ maxSize * maxSize ];
	pathMatrix = new int[ maxSize * maxSize ];
	for( int i = 0; i < maxSize; i++ )
	{
		vertexList[ i ] = other.vertexList[ i ];
		for( int j = 0; j < maxSize; j++ )
		{
			adjMatrix[ ( i * maxSize ) + j ] = other.adjMatrix[ ( i * maxSize ) + j ];
			pathMatrix[ ( i * maxSize ) + j ] = other.pathMatrix[ ( i * maxSize ) + j ];
		}
	}
	
	return *this;
}

/**
*********************************************************
 @pre	a Graph

 @post 	deallocated Heap

 Algorithm:
	- delete the arrays
***********************************************************
*/
WeightedGraph::~WeightedGraph()
{
	delete [] vertexList;
	delete [] adjMatrix;
	delete [] pathMatrix;
}

/**
*********************************************************
 @pre	a Graph

 @post 	a new vertex inserted into the graph

 @param newVertex Data to be inserted into the Heap

 @throw logic_error Throws error if theg graph is full

 Algorithm:
	- see if the vertex is in the arry if so update and return
	- check if the graph is full if so throw
	- else insert the vertex and increase size

***********************************************************
*/
void WeightedGraph::insertVertex( const Vertex& newVertex ) throw ( logic_error )
{
	for( int i = 0; i < size; i++)
	{
		if(vertexList[ i ].getLabel() == newVertex.getLabel() )
		{
			for(int j = 0; j < size; j++)
			{
				setEdge(i, j, INFINITE_EDGE_WT);
			}
			vertexList[ i ] = newVertex;
			return;
		}
	}
	if(isFull())
	{
		throw logic_error("The Graph is full");
	}
	vertexList[ size ] = newVertex;
	for( int i = 0; i <= size; i++)
	{
		setEdge( size, i, INFINITE_EDGE_WT);
	}
	size++;
}

/**
*********************************************************
 @pre	a graph

 @post 	Edge inserted into the edge matrix

 @param v1 First vertex to be found
 @param v2 Second vertex to be found

 @throw logic_error Throws an error if vertex's arent found

 Algorithm:
	- searches for the index locations of the vertex's
	- if the index locations are not found throw logic error
	- else set the edge 
***********************************************************
*/
void WeightedGraph::insertEdge( const string& v1, const string& v2, int wt ) throw ( logic_error )
{
	int indexOne = -1;
	int indexTwo = -1;
	for(int i = 0; i < size; i++)
	{
		if( vertexList[ i ].getLabel() == v1 )
		{
			indexOne = i;
		}
		if( vertexList[ i ].getLabel() == v2 )
		{
			indexTwo = i;
		}
	}
	if( indexOne == -1 || indexTwo == -1 )
	{
		throw logic_error( "Vertex not found " );
	}
	
	cout << "Size = " << size << " , idx_v1 = " << indexOne << " , idx_v2 = " << indexTwo << endl;
	setEdge(indexOne, indexTwo, wt);

}

/**
*********************************************************
 @pre	a graph

 @post 	vertex data in vData

 @param v string to be searched for
 @param vData vertex data to be returned

 @return Return if successful

 Algorithm:
	- searches for the index location of the vertex
	- if the index locations is found set vData
	- else return false
***********************************************************
*/
bool WeightedGraph::retrieveVertex( const string& v, Vertex& vData ) const
{
	for( int i = 0; i < size; i++ )
	{
		if( vertexList[ i ].getLabel() == v )
		{
			vData = vertexList[ i ];
			return true;
		}
	}
	return false;
}

/**
*********************************************************
 @pre	a graph

 @post 	weight data in wt

 @param v1 string to be searched for
 @param v2 string to be searched for
 @param wt weight data to be returned

 @return Return if successful

 @throw logic_error Throw if vertex's are not found

 Algorithm:
	- searches for the index locations of the vertexs
	- if the index locations is found set wt
	- else return false
***********************************************************
*/
bool WeightedGraph::getEdgeWeight( const string& v1, const string& v2, int& wt) const throw ( logic_error )
{
	int indexOne = -1;
	int indexTwo = -1;
	for(int i = 0; i < size; i++)
	{
		if( vertexList[ i ].getLabel() == v1 )
		{
			indexOne = i;
		}
		if( vertexList[ i ].getLabel() == v2 )
		{
			indexTwo = i;
		}
	}
	if( indexOne == -1 || indexTwo == -1 )
	{
		throw logic_error("Not found");
	}
	else
	{
		if( adjMatrix[ ( indexOne * maxSize ) + indexTwo ] != INFINITE_EDGE_WT )
		{
			wt = adjMatrix[ ( indexOne * maxSize ) + indexTwo ];
			return true;
		}
		else
		{
			return false;
		}
	}
}

/**
*********************************************************
 @pre	a graph

 @post 	vertex removed

 @param v vertex to be removed

 @throw logic_error Throw if vertex is not found

 Algorithm:
	- searches for the index locations of the vertex
	- shift the adj matrix rows left and columns up
	- shift vertexs left
***********************************************************
*/
void WeightedGraph::removeVertex( const string& v ) throw ( logic_error )
{
	for( int i = 0; i < size; i++ )
	{
		if( vertexList[ i ].getLabel() == v )
		{
			for(int j = i; j < size - 1; j++)
			{
				for(int k = 0; k < size; k++)
				{
					adjMatrix[ j + k * maxSize ] = adjMatrix[ j + 1 + k * maxSize ];
				}
			}
			for(int j = 0; j < size - 1; j++)
			{
				for(int k = i; k < size; k++)
				{
					adjMatrix[ j + k * maxSize ] = adjMatrix[ j + (k + 1) * maxSize ];
				}
			}
			for(int j = i; j < size; j++)
			{
				vertexList[j] = vertexList[j+1];
			}
			size--;
			return;
		}
	}
}


/**
*********************************************************
 @pre	a graph

 @post 	edge removed from the edge matrix

 @param v1 vertex to be searched for
 @param v2 vertex to be searched for

 @throw logic_error Throw if vertexs are not found

 Algorithm:
	- searches for the index locations of the vertex
	- set edge locations to infinite edge wt
	- throw logic error if vertex's arent found
***********************************************************
*/
void WeightedGraph::removeEdge( const string& v1, const string& v2 ) throw ( logic_error )
{
	int indexOne = -1;
	int indexTwo = -1;
	for(int i = 0; i < size; i++)
	{
		if( vertexList[ i ].getLabel() == v1 )
		{
			indexOne = i;
		}
		if( vertexList[ i ].getLabel() == v2 )
		{
			indexTwo = i;
		}
	}
	if( indexOne != -1 && indexTwo != -1 )
	{
		adjMatrix[ ( indexOne * maxSize ) + indexTwo ] = INFINITE_EDGE_WT;
		adjMatrix[ ( indexTwo * maxSize ) + indexOne ] = INFINITE_EDGE_WT;
	}
	else
	{
		throw logic_error("Not Found");
	}
}

/**
*********************************************************
 @pre	a graph

 @post 	an empty graph

 Algorithm:
	- set the size of the heap to 0;

***********************************************************
*/
void WeightedGraph::clear()
{
	size = 0;
}

/**
*********************************************************
 @pre	a Graph

 @post 	returns if the Graph is empty

 @return returns if the Graph is empty

 Algorithm:
	- return if the size is equal to 0

***********************************************************
*/
bool WeightedGraph::isEmpty() const
{
	return(size == 0);
}

/**
*********************************************************
 @pre	a graph

 @post 	returns if the graph is full

 @return returns if the graph is full

 Algorithm:
	- return if the size is equal to the max size

***********************************************************
*/
bool WeightedGraph::isFull() const
{
	return(size == maxSize);
}

void WeightedGraph::showStructure() const

// Outputs a graph's vertex list and adjacency matrix. This operation
// is intended for testing/debugging purposes only.

{
	if ( size == 0 ) {
	       cout << "Empty graph" << endl;
	    } else {
	       cout << endl << "Vertex list : " << endl;
	       for ( int row = 0 ; row < size ; row++ )
		   {
	           cout << row << '\t' << vertexList[row].getLabel();
#if! LAB12_TEST2
	                cout << '\t' << vertexList[row].getColor();
#endif
	                cout << endl;
		   }
	           

	       cout << endl << "Edge matrix : " << endl << '\t';
	       for ( int col = 0 ; col < size ; col++ )
	           cout << col << '\t';
	       cout << endl;
	       for ( int row = 0 ; row < size ; row++ )
	       {
	           cout << row << '\t';
	           for ( int col = 0 ; col < size ; col++ )
	           {
	               int wt = getEdge(row,col);
	               if ( wt == INFINITE_EDGE_WT )
	                  cout << "- \t";
	               else
	                  cout << wt << '\t';
	           }
	           cout << endl;
	       }
	    }
}
/**
*********************************************************
 @pre	a graph

 @post 	paths changed to shortest paths

 Algorithm:
	- Initialize path array
	- if there is a path between j and m
	- and there is a path between m and k
	- andt the sum of j,m and m,k is less than j,k
	- replace j,k with j,m + m,k
***********************************************************
*/
void WeightedGraph::showShortestPaths() const
/*
	
*/
{
	for( int i = 0; i < size; i++)
	{
		for( int j = 0; j < size; j++)
		{
			setPath(i, j, getEdge(i,j));
		}
		setPath(i, i, 0);
	}
	for( int m = 0; m < size; m++)
	{
		for( int j = 0; j < size; j++)
		{
			for( int k = 0; k < size; k++)
			{
				if( getPath(j, m) != INFINITE_EDGE_WT && 
					getPath(m, k) != INFINITE_EDGE_WT )
				{
					if( getPath(j, m) + getPath(m, k) < getPath(j, k) ||
					getPath(j, k) == INFINITE_EDGE_WT )
					{
						setPath(j, k, ( getPath(j, m) + getPath(m, k) ) );
					}
				}
			}
		}
	}
	
    cout << endl << "Path matrix : " << endl << '\t';
    for ( int col = 0 ; col < size ; col++ )
        cout << col << '\t';
    cout << endl;
    for ( int row = 0 ; row < size ; row++ )
    {
        cout << row << '\t';
        for ( int col = 0 ; col < size ; col++ )
        {
            int wt = getPath(row,col);
            if ( wt == INFINITE_EDGE_WT )
               cout << "- \t";
            else
               cout << wt << '\t';
        }
        cout << endl;
    }
}

/**
*********************************************************
 @pre	a graph

 @post 	edge removed from the edge matrix

 Algorithm:
	- search if any vertex has the same color as another
***********************************************************
*/
bool WeightedGraph::hasProperColoring() const
{
	for( int i = 0; i < size; i++ )
	{
		for( int j = 0; j < size; j++ )
		{
			if( getEdge(i, j) != INFINITE_EDGE_WT &&
				vertexList[ i ].getColor() == vertexList[ j ].getColor() &&
				i != j )
			{
				return false;	
			}
		}
	}
	return true;
}

/**
*********************************************************
 @pre	a graph

 @post 	edge removed from the edge matrix

 Algorithm:
	-  count the number of edges per vertex
	- if the number of edges isnt even, return false
	- else return true
***********************************************************
*/
bool WeightedGraph::areAllEven() const
{
	int counter = 0;
	for( int i = 0; i < size; i++)
	{
		for( int j = 0; j < size; j++)
		{
			if( adjMatrix[ ( i * maxSize ) + j ] != INFINITE_EDGE_WT )
			{
				counter++;
			}
		}
		if( counter % 2 != 0)
		{
			return false;
		}
		counter = 0;
	}
	return true;
}

/**
*********************************************************
 @pre	a graph

 @post indexLocation of the string returned

 @return index location of the string returned

 Algorithm:
	- traverse the list until the string is found
***********************************************************
*/
int WeightedGraph::getIndex( const string& v ) const
{
	for( int i = 0; i < size; i++ )
	{
		if( vertexList[ i ].getLabel() == v )
		{
			return i;
		}
	}
	return -1;
}

/**
*********************************************************
 @pre	a graph

 @post return edge of the row and column

 @return edge of the row and column

 Algorithm:
	- return the edge
***********************************************************
*/
int WeightedGraph::getEdge( int row, int col ) const
{
	return adjMatrix[ ( row * maxSize ) + col ];
}

/**
*********************************************************
 @pre	a graph

 @post set edge of the row and column

 Algorithm:
	- set the edge
***********************************************************
*/
void WeightedGraph::setEdge( int row, int col, int wt )
{
	adjMatrix[ ( row * maxSize ) + col ] = wt;
	adjMatrix[ ( col * maxSize ) + row ] = wt;
}

/**
*********************************************************
 @pre	a graph

 @post return path of the row and column

 @return path of the row and column

 Algorithm:
	- return the path
***********************************************************
*/
int WeightedGraph::getPath(int row, int col) const
{
	return pathMatrix[ (row * maxSize) + col ];
}

/**
*********************************************************
 @pre	a graph

 @post set path of the row and column

 Algorithm:
	- set the path
***********************************************************
*/
void WeightedGraph::setPath(int row, int col, int wt) const
{
	pathMatrix[ (row * maxSize) + col ] = wt;
	pathMatrix[ (col * maxSize) + row ] = wt;
}

	
	
	
	
	
	
	