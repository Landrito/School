#ifndef TIMER_CPP
#define TIMER_CPP

#include <iostream>
#include <sys/time.h>
#include <Timer.h>
using namespace std;

/**
*********************************************************
 @param tod1 This is the initial timeval
 @param tod2 This is the final timeval

 @pre none

 @post The difference between the intial and final time is returned.

 @return Returns the difference between the intial and final time in usec.

 Algorithm:
	- Converts the timevals to be measured in usecs
	- Returns the difference between the two times in usecs

 Exceptional/Error Conditions:
 	- none

***********************************************************
*/
long long int toddiff(struct timeval *tod1, struct timeval *tod2)
{
    long long t1, t2;
    t1 = tod1->tv_sec * 1000000 + tod1->tv_usec;
    t2 = tod2->tv_sec * 1000000 + tod2->tv_usec;
    return t1 - t2;
}

/**
*********************************************************
 @pre	Unitialized Timer Class
 @post 	Duration assigned to -1
		timerWasStarted assigned false

 Algorithm:
	- Assign data members values

 Exceptional/Error Conditions:
 	- none
**********************************************************
*/
Timer::Timer()
{
	duration = -1;
	timerWasStarted = false;
}

/**
*********************************************************
 @pre	none
 @post 	beginTime set to the timeval
		timerWasStarted assigned true

 Algorithm:
	- Get time of day assigned to the begin time
	- Assign timerWasStarted true

 Exceptional/Error Conditions:
 	- none

**********************************************************
*/
void Timer::start() throw (runtime_error) 
{
    gettimeofday(&beginTime, NULL);
	timerWasStarted = true;
}

/**
*********************************************************
 @pre	none

 @post 	Duration set to the number of milliseconds passed
		since the start function was called

 Algorithm:
	- Get time of day assigned to the begin time
	- Assign timerWasStarted true

 @throw A logic error is thrown if the start function was
		never called.

***********************************************************
*/
void Timer::stop() throw (logic_error)
{
    if( !timerWasStarted ) 
	{
		throw logic_error("Timer was never started.");
    }
	struct timeval endTime;
	gettimeofday(&endTime, NULL);
    duration = toddiff(&endTime, &beginTime);
}

/**
*********************************************************
 Parameters:
	none

 @return The time in seconds as a double

 @pre none

 @post 	Duration set to the number of milliseconds passed
		since the start function was called

 Algorithm:
	- Get time of day assigned to the begin time
	- Assign timerWasStarted true

 @throw logic_error A logic error is thrown if the start function was never called or if the stop function was never called.

***********************************************************
*/
double Timer::getElapsedTime() const throw (logic_error)
{
    // Place your code here
	if( !timerWasStarted )
	{
		throw logic_error("Timer was never started.");
	}
	else if( duration == -1 )
	{
		throw logic_error("Timer was never stopped");
	}
	return duration/1000000.0;
}

#endif	//#ifndef TIMER_CPP
