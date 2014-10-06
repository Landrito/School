#include <iostream>
#include <cctype>
#include <ctime>
#include <iomanip>

using namespace std;

#include "Timer.h"

int main()
{
	double time;
	Timer stdTimer;
	cin >> time;
	stdTimer.start();
	usleep(time * 1000000);
	stdTimer.stop();
	//cout << fixed << setprecision(2);
	cout << "Slept for " << time << " seconds. Measured ";
	cout << stdTimer.getElapsedTime() << " seconds." << endl;
}