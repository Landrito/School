#include <cstdio.h>
#include <vector.h>

const int NUM_PACKETS = 100;
const int WINDOW_SIZE = 4;
const int SEED = 1;
const float ERROR_RATE = 0.0;

struct Packet()
{
	int rtt;
	int timer;
	bool error;
	bool timeout;
};

class Window()
{
	public:
		Window();
		void sendPacket(int seq);
		void checkRecieved();
		void updateTimers();


	private:
		bool goodTrans[NUM_PACKETS];
		int windowStart; //This will hold the starting position of the window
		std::vector<Packet> packets;
}


int main()
{
	Window window;
	int t = 0;

	for( int i = 0; i < NUM_PACKETS;)
	{
		if( !window.isFull() )
		{
			window.sendPacket(i);
			i++;
			packetsInTransit++;
		}
		window.updateTimers();
		window.checkRecieved();
		t++;
	}
}

Window::Window()
{
	windowStart = 0;
	for( int i = 0; i < NUM_PACKETS; i++ )
	{
		goodTrans[ i ] = false;
	}
}


void Window::sendPacket(int seq)
{
	packet.rtt = 	 ( rand() + 10 ) % 51;
	packet.timer = 	 0;
	packet.error = 	 ( (rand() % 101) < (ERROR_RATE * 100) );
	packet.timeout = false
	packet.sequence = seq;
	packets.push_back(packet);
}

void Window::checkRecieved()
{
	bool timeout = false;
	for( std::vector<Packet>::iterator it = packets.begin(); it != packets.end(); it++ )
	{
		if( it->timer == 45 && it->error )//Packet Errored out;
		{
			printf( "Packet %i errored out. Timeout. Resending\n", it->sequence );
			sendPacket( it->sequence );
			window.erase(it);
			it--;
		}
		else if ( it->timer == 45 && !it->error ) //Packet took too long send another
		{
			printf( "Packet %i has no error. Timeout. Resending\n", it->sequence );
			sendPacket( it->sequence );
		}
		if( it->timer == it->rtt && !it->error ) 
		{
			if( it->sequence == windowStart || goodTrans[it->sequence] ) //If the packet that was recieved was the next expected packet, accept it.
			{
				if( goodTrans[it->sequence] )
					printf( "Packet %i has already been recieved.\n", it->sequence );
				else
				{
					printf( "Packet %i has been recieved.\n", it->sequence );
					packetsInTransit--;
				}
				goodTrans[it->sequence] = true;
				window.erase(it);
				it--;
				windowStart++;
				
			}

			else
			{
				printf( "Packet %i was recieved out of order. Resemding \n", it->sequence );
				sendPacket( it->sequence );
				window.erase(it);
				it--;
			}
		}
	}
}

void Window::updateTimers()
{
	for( std::vector<Packet>::iterator it = packets.begin(); it != packets.end(); it++ )
	{
		it->timer++;
	}
}









