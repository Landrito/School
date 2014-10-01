#include <stdexcept>
#include <iostream>
#include <cstring>
#include <iomanip>
#include "StackLinked.cpp"
using namespace std;
//--------------------------------------------------------------------
/*
Postfix Calculator
Calculator that takes in a string of numbers and operations and performs
calculations by pushing numbers onto the stack and popping them to perform
the calculation.
*/

const int MAX_STR_LEN = 80;

int main(){
	//Declare variables
	StackLinked<double> Stack;
	double operand1, operand2, result;
	char statement[MAX_STR_LEN];
	
	//Get input
	cout << endl;
	cout << "Enter expression in postfix form : ";
	cin >> statement;

	//Go through the input
	for(int i = 0; i < strlen(statement); i++ ){
		
		//Push if a number
		if(statement[i] >= '0' && statement[i] <= '9'){
			Stack.push(statement[i] - '0');
		}

		//otherwise perform operation
		else{
			switch(statement[i]){
				case '+':
					operand1 = Stack.pop();
					operand2 = Stack.pop();
					result = operand2 + operand1;
					Stack.push(result);
					break;
				
				case '-':
					operand2 = Stack.pop();
					operand1 = Stack.pop();
					result = operand1 - operand2;
					Stack.push(result);
					break;
				
				case '/':
					operand1 = Stack.pop();
					operand2 = Stack.pop();
					result = operand2 / operand1;
					Stack.push(result);
					break;
					
				case '*':
					operand1 = Stack.pop();
					operand2 = Stack.pop();
					result = operand2 * operand1;
					Stack.push(result);
					break;
			}
		}
	}
	//output the result
	result = Stack.pop();
	cout << " = " << setprecision(2) << result << endl;
	return 0;
}
