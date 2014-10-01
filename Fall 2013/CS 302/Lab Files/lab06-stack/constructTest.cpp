#include <iostream>
#include "StackLinked.cpp"

int main() {
	StackLinked<int> testStack;
	testStack.push(1);
	testStack.push(1);
	testStack.push(1);
	testStack.push(1);
	testStack.showStructure();	
	StackLinked<int> other(testStack);
	other.showStructure();
	
}