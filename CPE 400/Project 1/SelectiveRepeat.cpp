#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
const int NUM_PACKETS = 100;
const int SEED = 1;
const int MAX_TRIALS = 5000;
const int MIN_WINDOW_SIZE = 4;
const int MAX_WINDOW_SIZE = 100;
const float ERROR_RATE = 0.2;

struct Packet
{
	int rtt;
	int timer;
	int sequence;
	bool error;
};

class Window
{
	public:
		Window(int ws, float er);
		~Window();
		void sendPacket(int seq);
		void checkRecieved();
		void updateTimers();
		bool isFull();
		bool complete();
		void increasePacketsInTransit();

	private:
		int packetsInTransit;
		bool sent[NUM_PACKETS];
		int windowStart; //This will hold the starting position of the window
		int windowSize;
		float errorRate;
		std::vector<Packet> packets;
};


int main()
{
	FILE * outFile = fopen("SelectiveRepeat.csv", "w");
	fprintf(outFile, "Window Size,Average Throughput\n");
	for(int windowSize = MIN_WINDOW_SIZE; windowSize <= MAX_WINDOW_SIZE; windowSize+=4)
	{
		float throughput = 0.0;
		float sum = 0.0;
		for(int trialNum = 0; trialNum < MAX_TRIALS; trialNum++)
		{
			srand( trialNum );
			Window window(windowSize, ERROR_RATE);
			int t = 0;
			int i = 0;

			while( !window.complete() )
			{
				if( !window.isFull() && i < NUM_PACKETS )
				{
					window.increasePacketsInTransit();
					window.sendPacket(i);
					i++;
				}
				window.checkRecieved();
				window.updateTimers();
				t++;
			}
			throughput = ( 100.0 * NUM_PACKETS ) / t;
			sum += throughput;
			//printf("Selective Repeat Window Size %i Trial #%i Throughput: %f \n", 
			//		windowSize, trialNum,throughput );
		}
		printf("Selective Repeat Window Size %i Average Throughput: %f \n",
				windowSize, sum / MAX_TRIALS );
		fprintf(outFile, "%i,%f\n", windowSize, sum / MAX_TRIALS);
	}

	fclose(outFile);



	//printf("Total Time: %d \n", t);
	//printf("Throughput: %f \n", 100.0*NUM_PACKETS / t);
	return 0;

}

Window::Window(int ws, float er)
{
	packetsInTransit = 0;
	windowStart = 0;
	windowSize = ws;
	errorRate = er;
	packets.clear();
	for( int i = 0; i < NUM_PACKETS; i++ )
	{
		sent[ i ] = false;
	}
}

Window::~Window()
{

}

void Window::sendPacket(int seq)
{
	Packet packet;
	packet.rtt =  	 ( rand() % 41 ) + 10;
	packet.timer = 	 0;
	packet.error = 	 ( (rand() % 101) < (errorRate * 100) );
	packet.sequence = seq;

	packets.push_back(packet);
}

void Window::checkRecieved()
{
	std::vector<int> temp;
	temp.clear();
	for( std::vector<Packet>::iterator it = packets.begin(); it != packets.end(); it++ )
	{
		if(it->error)
		{
			if( it->timer >= 45 )//if it errored out, wait 45 and resend;
			{
				//printf("Error packet %i\n", it->sequence);
				temp.push_back(it->sequence);
				it = packets.erase(it) - 1;
			}
		}
		else
		{
			if(it->timer == it->rtt)
			{
				if( sent[it->sequence] )
				{
					//printf("Already received packet %i\n", it->sequence);

				}
				else
				{
					//printf("Received packet %i\n", it->sequence);
					sent[it->sequence] = true;
					packetsInTransit--;
					windowStart++;
				}
				
				it = packets.erase(it) - 1;
			}
			else if( it->timer == 45 )
			{
				temp.push_back(it->sequence);
				//printf("Timeout packet %i resending\n", it->sequence);

			}
		}
	}

	for( std::vector<int>::iterator it = temp.begin(); it != temp.end(); it++ )
	{
		sendPacket(*it);
	}
}

void Window::updateTimers()
{
	for( std::vector<Packet>::iterator it = packets.begin(); it != packets.end(); it++ )
	{
		it->timer++;
	}
}

bool Window::isFull()
{
	return packetsInTransit >= windowSize;
}

void Window::increasePacketsInTransit()
{
	packetsInTransit++;
}

bool Window::complete()
{
	return sent[NUM_PACKETS - 1];
}