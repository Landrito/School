#include <iostream>
#include <cctype>
#include <ctime>

using namespace std;

#include "Timer.h"

int main()
{
	double time;
	Timer stdTimer;
	stdTimer.start();
	wait(time);
	stdTimer.stop();
	cout << "Slept for " << setprecision(2) << time <<" seconds. Measured ";
	cout << setprecision(2) << stdTimer.getElapsedTime().
}