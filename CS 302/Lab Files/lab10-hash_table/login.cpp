//////////////////////////////////////////////////////////////////////
/*
    FileName:     	Password.cpp
   
    Description: 	Program that authenticates login and password information
*/
//  Version/Revision Information  ///////////////////////////////////
/*
    1.00 ( 11/2/2013 ) - Ernest Landrito - C.S. Student
        Original Code
*/

//  Program Description/Support /////////////////////////////////////
/*
    This program uses a header file which contains the class and member
	implementations for the HashTable class, as well as iostream and filestream
*/
//  Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
//  Header Files  ///////////////////////////////////////////////////
//
#include <iostream>
#include <fstream>
#include "HashTable.cpp"
//
//  Global Constant Definitions  ////////////////////////////////////
//
/////  NONE
//
//  Class Definitions  //////////////////////////////////////////////
//
class LoginInfo
{
public:
	void setInfo(const string& newUsername, const string& newPassword);
	string getKey() const;
	string getPassword() const;
	static unsigned int hash(const string& str);
	
private:
	string key;
	string password;
};
//
//  Free Function Prototypes  ///////////////////////////////////////
//
/////  NONE
//
//  Main Function Implementation  ///////////////////////////////////
//
int main()
{
	ifstream accounts("password.dat");
	HashTable<LoginInfo, string> table(8);
	LoginInfo tempLogin;
	
	string tempUsername, tempPassword;
	accounts >> tempUsername;
	while(accounts.good())
	{
		accounts >> tempPassword;
		tempLogin.setInfo(tempUsername, tempPassword);
		table.insert(tempLogin);
		accounts >>tempUsername;
	}
	table.showStructure();
	
	cout << "Login: ";
	while(cin >> tempUsername)
	{
		cout << "Password: ";
		cin >> tempPassword;
	
		if(table.retrieve(tempUsername, tempLogin))
		{
			if(tempLogin.getPassword() == tempPassword)
			{
				cout << "Authentication successful" << endl;
			}
			else
			{
				cout << "Authentication failure" << endl;
			}
		}
		else
		{
			cout  << "Authentication failure" << endl;
		}
		cout << "Login: ";	
	}
	
	return 0;
}
//  Free Function Implementation  ///////////////////////////////////
//
/////  NONE
//
//  Class/Data Structure Member Implementation //////////////////////
//
/**
*********************************************************
 @pre	LoginInfo class
 @post 	Values set
 @param newUsername string key is going to be assigned
 @param newPassword string password is going to be assigned

 Algorithm:
	- assign values
**********************************************************
*/
void LoginInfo::setInfo(const string& newUsername, const string& newPassword)
{
	key = newUsername;
	password = newPassword;
}

/**
*********************************************************
 @pre	a LoginInfo class

 @post 	a returned key of the data

 @return The key returned

 Algorithm:
	- return key

***********************************************************
*/
string LoginInfo::getKey() const
{
	return key;
}

/**
*********************************************************
 @pre	a LoginInfo class

 @post 	a returned password of the data

 @return The password returned

 Algorithm:
	- return password

***********************************************************
*/
string LoginInfo::getPassword() const
{
	return password;
}

/**
*********************************************************
 @pre	a LoginInfo class

 @post 	a returned sum of the value of the chars in the string

 @return sum of the value of the chars in the string

 Algorithm:
	- for each char of the string, add the value to val
	- return val

***********************************************************
*/
unsigned int LoginInfo::hash(const string& str) {
    unsigned int val = 0;

    for (int i = 0; i < str.length(); ++i) {
	val += str[i];
    }

    return val;
}
