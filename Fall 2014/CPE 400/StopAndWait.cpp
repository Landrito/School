#include <stdio.h>
#include <stdlib.h>
#include <vector>

/****************************************************************************/
//Stop and wait is the essentially the same code as GBN but it accepts the 
//file if the file is even if it isnt at the start of the window.
/****************************************************************************/
const int WINDOW_SIZE = 1; 
const int NUM_PACKETS = 100;
const int SEED = 1;
const float ERROR_RATE = 0.0;

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
		Window();
		void sendPacket(int seq);
		void checkRecieved();
		void updateTimers();
		bool isFull();
		bool complete();
		void increasePacketsInTransit();

	private:
		int numInQueue;
		int packetsInTransit;
		bool sent[NUM_PACKETS];
		int windowStart; //This will hold the starting position of the window
		std::vector<Packet> packets;
};


int main()
{
	Window window;
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

	printf("Total Time: %i \n", t);
	printf("Throughput: %f \n", 100.0*NUM_PACKETS / t);
	return 0;

}

Window::Window()
{
	windowStart = 0;
	for( int i = 0; i < NUM_PACKETS; i++ )
	{
		sent[ i ] = false;
	}
}


void Window::sendPacket(int seq)
{
	Packet packet;
	packet.rtt =  	 ( rand() % 41 ) + 10;
	packet.timer = 	 0;
	packet.error = 	 ( (rand() % 101) < (ERROR_RATE * 100) );
	packet.sequence = seq;

	packets.push_back(packet);
}

void Window::checkRecieved()
{
	std::vector<int> temp;
	for( std::vector<Packet>::iterator it = packets.begin(); it != packets.end(); it++ )
	{
		if(it->error)
		{
			if( it->timer >= 45 )//if it errored out, wait 45 and resend;
			{
				printf("Error packet %i\n", it->sequence);
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
					printf("Already received packet %i\n", it->sequence);

				}
				else
				{
					if( it->sequence != windowStart )
					{
						printf("Received packet %i out of order resending\n", it->sequence);
						temp.push_back(it->sequence);
					}
					else
					{
						printf("Received packet %i\n", it->sequence);
						sent[it->sequence] = true;
						packetsInTransit--;
						windowStart++;
					}
					
				}
				
				it = packets.erase(it) - 1;
			}
			else if( it->timer == 45 )
			{
				temp.push_back(it->sequence);
				printf("Timeout packet %i resending\n", it->sequence);

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
	return packetsInTransit >= WINDOW_SIZE;
}

void Window::increasePacketsInTransit()
{
	packetsInTransit++;
}

bool Window::complete()
{
	return sent[NUM_PACKETS - 1];
}