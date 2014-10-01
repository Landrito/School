#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Garden.h"
using namespace std;

const int STD_STR_LEN = 256;

int main(){
	int tempX;
	int tempY;
	int gardenCounter = 0;
	garden *pumpkinPatch;
	
	cin >> tempY;
	cin >> tempX;
	while(tempX != 0 && tempY != 0)
	{
		
		if(tempX > 0 && tempY > 0) pumpkinPatch = new garden(tempX, tempY);
		
		for(int i = 0; i < tempY; i++)
		{
			cin >> pumpkinPatch->arr[i];
		}
		if(tempX > 0 && tempY > 0) 
		{
			pumpkinPatch->patchChecker();
		}
		
		gardenCounter++;	
		
		cout << "Garden # " << gardenCounter << ": ";
		
		if (tempX > 0 && tempY > 0)
		{
			cout << pumpkinPatch->patchList;
		} 
		else
		{
			cout << "0 patches, sizes: " << endl;
		}
		
		cin >> tempY;
		cin >> tempX;
	} 
}




