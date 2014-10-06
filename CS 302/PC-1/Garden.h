//////////////////////////////////////////////////////////////////////
/*
    FileName:     	Garden.h
   
    Description: 	header file containing definitions and implementations
					of an ordered LinkedList class, ListNode class, and 
					Garden class.
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 09/24/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
//
/////  NONE
//
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
	#include <iostream>
	using namespace std;
//
//  Class Definitions  //////////////////////////////////////////////
//
	/***********************************************************
	Class
	***********************************************************/
	class ListNode
	{
	public:
		ListNode( const int &newData, ListNode *nextPtr );
		int data;
		ListNode *next;
	};	

	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();
		void insert( const int &num );
		int getCount();
		friend ostream& operator<<( ostream& stream, const LinkedList &list);

	private:
		ListNode *head;
		int count;
	};

	class Garden
	{
	public:
		Garden( const int &width, const int &height );
		~Garden();
		void getData();
		void patchChecker();
		int pumpkinCounter( int x, int y );
	
		LinkedList patchList;
		int maxRows, maxCols;
		char **gardenArr;
		bool **checkedArr;
	};
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
	/***********************************************************
	Function Specification: Garden::Garden( int width, int height)
	============================================================
	 Preconditions:
		- maxRows and maxCols are uninitialized
		- char gardenArr[][] and bool checkedArr[][] are not allocated
		- LinkedList patchList is empty
		- int height and int width are passed as parameters
	 Postconditions:
		- maxRows is assigned height
		- maxCols is assigned width
		- gardenArr is allocated
		- checkedArr is allocated and initialized
	 Algorithm:
		- maxRows and maxCols is assigned values
		- allocate both gardenArr and checkedArr based on maxRows and maxCols
		- initialize checkedArr to false
	 Exceptional/Error Conditions:
	   - if maxRows and maxCols are less than or equal to zero, 
			point the arrays to NULL
	============================================================
	***********************************************************/
	Garden::Garden( const int &width, const int &height )
	{
		int i, j;			//Loop Counters
		maxRows = height;
		maxCols = width;
		if( ( maxRows <= 0 )
		 && ( maxCols <= 0 ) )
		{
			gardenArr = NULL;
			checkedArr = NULL;
		}
		else
		{	
			gardenArr = new char * [ maxRows ];
			checkedArr = new bool * [ maxRows ];
			for( i = 0; i < maxRows; i++ )
			{
				gardenArr[ i ] = new char[ maxCols ];
				checkedArr[ i ] = new bool[ maxCols ];
				for( j = 0; j < maxCols; j++ )
				{
					checkedArr[ i ][ j ] = false;
				}
			}
		}
	}

	/***********************************************************
	Function Specification: Garden::~Garden()
	============================================================
	 Preconditions:
		- initialized Garden
	 Postconditions:
		- gardenArr and checkedArr are deallocated
	 Algorithm:
		- delete each row of gardenArr and checkedArr
		- delete gardenArr and checkedArr
	 Exceptional/Error Conditions:
	 	- none
	============================================================
	***********************************************************/
	Garden::~Garden()
	{
		int i;			//Loop counter
		for (i = 0; i < maxRows; i++)
		  	{
		  		delete [] gardenArr [i];
				delete [] checkedArr [i];
		  	}
			delete [] gardenArr;
			delete [] checkedArr;
		
	}
	
	/***********************************************************
	Function Specification: Garden::getData
	============================================================
	 Preconditions:
		- gardenArr is allocated
	 Postconditions:
		- gardenArr contains data about the garden
	 Algorithm:
		- run a loop that gets input from a keyboard and assigns it to each
			row of the gardenArr
	 Exceptional/Error Conditions:
	   - none
	============================================================
	***********************************************************/
	void Garden::getData()
	{
		int i;			//Loop Counter
		for( i = 0; i < maxRows; i++ )
		{
			cin >> gardenArr[ i ];
		}
	}

	/***********************************************************
	Function Specification: Garden::patchChecker()
	============================================================
	 Preconditions:
		- gardenArr holds data
		- checkedArr is initialized to false
		- maxRows and maxCols are set
	 Postconditions:
		- patchList is filled with an ordered list of the sizes of the 
			pumpkin patches
	 Algorithm:
		- go through each position in the gardenArr
		- check if it has been checked before
		- check if it is a pumpkin
		- if it is a pumpkin and hasnt been checked before count the number
			of pumpkins in the patch using the pumpkinCounter method
		- insert the number of patches into the list
	 Exceptional/Error Conditions:
	   - if maxRows and maxCols are less than or equal to zero, 
			the function does nothing
	============================================================
	***********************************************************/
	void Garden::patchChecker()
	{
		int i, j;		//Loop Counter
		if( maxRows > 0 && maxCols > 0 )
		{
			for( i = 0; i < maxRows; i++ )
			{
				for( j = 0; j < maxCols; j++ )
				{
					if( ( checkedArr[ i ][ j ] == false ) 
					 && ( gardenArr[ i ][ j ] == 'p' ) )
					{
						patchList.insert( pumpkinCounter( j,i ) );
					}
					checkedArr[ i ][ j ] = true;
				}
			}
		}
	}

	/***********************************************************
	Function Specification: Garden::pumpkinCounter( int xPos, int yPos )
	============================================================
	 Preconditions:
		- there is data in the gardenArr
		- checkedArr has been initialized
		- function is passed the x and y position to be checked
	 Postconditions:
		- returnes amount of pumpkins in the patch
	 Algorithm:
		- returns if 1 if a pumpkin is found and calls itself for the positions
			around it
	 Exceptional/Error Conditions:
	 	- if the function xPos or yPos is outside of bounds it returns 0;
		- if the function xPos or yPos is not a pumpkin it marks that it
			checked the spot and returns 0;
		- if the function xPos and yPos has been already checked 
			the function returns 0;
	============================================================
	***********************************************************/
	int Garden::pumpkinCounter( int xPos, int yPos ){
		if( ( xPos < 0 ) 
		 || ( xPos >= maxCols ) 
		 || ( yPos < 0 )
		 || ( yPos >= maxRows ) ) return 0;

		else if ( ( checkedArr[ yPos ][ xPos ] == false ) 
			   && ( gardenArr[ yPos ][ xPos ] == 'p' ) )
		{
			checkedArr[ yPos ][ xPos ] = true;
			return ( pumpkinCounter( ( xPos + 1 ), yPos ) 
				   + pumpkinCounter( ( xPos - 1 ), yPos ) 
				   + pumpkinCounter( xPos, ( yPos + 1 ) ) 
				   + pumpkinCounter( xPos, ( yPos - 1 ) ) 
				   + 1 );
		}
	
		else if ( gardenArr[ yPos ][ xPos ] != 'p' )
		{ 
			checkedArr[ yPos ][ xPos ] = true;
			return 0;
		}
	
		else
		{
			return 0;
		}
	}

	/***********************************************************
	Function Specification: LinkedList::LinkedList()
	============================================================
	 Preconditions:
		- unitialized list and count
	 Postconditions:
		- head is pointing to NULL
		- count is initialized to 0
	 Algorithm:
		- point head to NULL
		- set count to 0
	 Exceptional/Error Conditions:
	 	- NONE
	============================================================
	***********************************************************/
	LinkedList::LinkedList()
	{
		head = NULL;
		count = 0;
	}

	/***********************************************************
	Function Specification: LinkedList::~LinkedList()
	============================================================
	 Preconditions:
		- none
	 Postconditions:
		- all nodes in list are deleted
	 Algorithm:
		- Have a temporary pointer point to the head
		- Point head to next
		- Delete the temporary pointer
		- Repeat until empty list
		- set count to 0
	 Exceptional/Error Conditions:
	 	- NONE
	============================================================
	***********************************************************/
	LinkedList::~LinkedList()
	{
		ListNode* temp;			//temporary pointer to traverse the list
		while( head != NULL )
		{
			temp = head;
			head = head->next;
			delete temp;
		}
		count = 0;
	}

	/***********************************************************
	Function Specification: LinkedList::~LinkedList()
	============================================================
	 Preconditions:
		- num is the number to be inserted into the list
	 Postconditions:
		- the data is inserted in order into the list
	 Algorithm:
		- if the list is empty regularly insert the node
		- if the num is smaller than the head data insert it before the list
		- traverse the list until you find a data point that is larger than num
			or you have reached the list
		- if the num is less than or equal to the data at that point, insert 
			the num after the point
		- otherwise insert the num before that point
		- increase count
	 Exceptional/Error Conditions:
	 	- NONE
	============================================================
	***********************************************************/
	void LinkedList::insert( const int &num )
	{
		ListNode *temp;		//pointer to traverse the list
		ListNode *prev;		//pointer to traverse the list
		if( head == NULL ) 
		{
			head = new ListNode( num, NULL );
		}
	
		else if( num < head-> data ) 
		{
			head = new ListNode( num, head );
		}
	
		else 
		{
			temp = head;
			prev = head;
			while( ( num > temp->data ) 
			    && ( temp->next != NULL ) )
			{
				prev = temp;
				temp = temp->next;
			}
		
			if( num >=  temp->data )
			{
				temp->next = new ListNode( num, temp->next );
			}
		
			else
			{
				prev->next = new ListNode( num, temp );
			}
		}
		count++;
	}

	/***********************************************************
	Function Specification: LinkedList::getCount();
	============================================================
	 Preconditions:
		- none
	 Postconditions:
		- returns count
	 Algorithm:
		- returns count
	 Exceptional/Error Conditions:
	 	- NONE
	============================================================
	***********************************************************/
	int LinkedList::getCount()
	{
		return count;
	}

	/***********************************************************
	Function Specification: 
	ListNode::ListNode( const int &newData, ListNode *nextPtr ) 
	============================================================
	 Preconditions:
		- unitialized ListNode
	 Postconditions:
		- data item is set
		- next pointer is set
	 Algorithm:
		- data is assigned newData
		- next is assigned nextPtr
	 Exceptional/Error Conditions:
	 	- NONE
	============================================================
	***********************************************************/
	ListNode::ListNode( const int &newData, ListNode *nextPtr )
	{
		data = newData;
		next = nextPtr;
	}

	/***********************************************************
	Function Specification: 
	ostream& operator<<( ostream& stream, const LinkedList &list )
	============================================================
	 Preconditions:
		- output stream 
		- LinkedList with data in it
	 Postconditions:
		- outputed list of numbers
	 Algorithm:
		- traverse the list printing the data followed by a space.
		- return the output stream
	 Exceptional/Error Conditions:
	 	- NONE
	============================================================
	***********************************************************/
	ostream& operator<<( ostream& stream, const LinkedList &list )
	{
		ListNode *temp = list.head;		//temporary pointer
		while( temp != NULL )
		{
			stream << temp->data << " ";
			temp = temp->next;
		}
		stream << endl;
		return stream;
	}
//
//  Terminating Precompiler Directives ///////////////////////////////
//
/////  NONE
//
