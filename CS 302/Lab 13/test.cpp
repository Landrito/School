#include <iostream>
#include <cctype>
#include <ctime>

using namespace std;

int main()
{
	double timer = clock();
	cin.get();
	cout << (clock() - timer) / CLOCKS_PER_SEC << endl;
	return 0;
}