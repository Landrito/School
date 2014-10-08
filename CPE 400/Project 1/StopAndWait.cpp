#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
const int NUM_PACKETS = 1000;
const int SEED = 1;
const int INITIAL = 645;
const int MAX_TRIALS = 10;
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
		int getNumRetransmissions();

	private:
		int packetsInTransit;
		bool sent[NUM_PACKETS];
		int windowStart; //This will hold the starting position of the window
		int windowSize;
		float errorRate;
		int numRetransmissions;
		std::vector<Packet> * packets;
};


int main()
{
	FILE * outFile = fopen("StopAndWait.csv", "w");
	FILE * outFile2 = fopen("StopAndWait.txt", "w");
	fprintf(outFile, "Error Rate,Average Throughput,Number of Retransmissions\n");
	fprintf(outFile2, "Stop and Wait\n");
	for(int err = 0; err <= 50; err+=10)
	{
		float throughput = 0.0;
		float sum = 0.0;
		int rtSum = 0;
		for(int trialNum = INITIAL; trialNum < MAX_TRIALS + INITIAL; trialNum++)
		{
			srand( trialNum );
			Window * window = new Window(1, float(err) / 100.0);
			int t = 0;
			int i = 0;

			while( !window->complete() )
			{
				if( !window->isFull() && i < NUM_PACKETS )
				{
					window->increasePacketsInTransit();
					window->sendPacket(i);
					i++;
				}
				window->checkRecieved();
				window->updateTimers();
				t++;
			}
			throughput = ( 100.0 * NUM_PACKETS ) / t;
			sum += throughput;
			rtSum += window->getNumRetransmissions();

			delete window;
			printf("Stop and Wait Error Rate 0.%i Trial #%i Throughput: %f \n", 
					err, trialNum + 1 - INITIAL, throughput );
			fprintf(outFile2, "Stop and Wait Error Rate 0.%i Trial #%i Throughput: %f \n", 
					err, trialNum + 1 - INITIAL, throughput );
		}
		printf("\nStop and Wait Error Rate 0.%i Average Throughput: %f \n \n",
				err, sum / MAX_TRIALS );
		fprintf(outFile2, "\nStop and Wait Error Rate 0.%i Average Throughput: %f \n \n",
				err, sum / MAX_TRIALS );
		fprintf(outFile, "0.%i,%f, %i\n", err, sum / MAX_TRIALS, rtSum / MAX_TRIALS );
	}

	fclose(outFile);
	fclose(outFile2);

	return 0;

}

Window::Window(int ws, float er)
{
	packets = new std::vector<Packet>;
	numRetransmissions = 0;
	packetsInTransit = 0;
	windowStart = 0;
	windowSize = ws;
	errorRate = er;
	packets->clear();
	for( int i = 0; i < NUM_PACKETS; i++ )
	{
		sent[ i ] = false;
	}
}

Window::~Window()
{
	packets->clear();
	delete packets;
}

void Window::sendPacket(int seq)
{
	Packet packet;
	packet.rtt =  	 ( rand() % 41 ) + 10;
	packet.timer = 	 0;
	packet.error = 	 ( (rand() % 101) < (errorRate * 100) );
	packet.sequence = seq;

	packets->push_back(packet);
}

void Window::checkRecieved()
{
	std::vector<int> * temp = new std::vector<int>;
	temp->clear();
	for( std::vector<Packet>::iterator it = packets->begin(); it != packets->end(); it++ )
	{
		if(it->error)
		{
			if( it->timer >= 45 )//if it errored out, wait 45 and resend;
			{
				//printf("Error packet %i\n", it->sequence);
				numRetransmissions++;
				temp->push_back(it->sequence);
				it = packets->erase(it) - 1;
			}
		}
		else
		{
			if(it->timer == it->rtt)
			{
				if( it->sequence != windowStart )
				{
					//printf("Received packet %i out of order resending\n", it->sequence);
					numRetransmissions++;
					temp->push_back(it->sequence);
				}
				else
				{
					//printf("Received packet %i\n", it->sequence);
					sent[it->sequence] = true;
					packetsInTransit--;
					windowStart++;
				}
				
				it = packets->erase(it) - 1;
			}
			else if(sent[it->sequence] && it->timer >= 45 )
			{
				//printf("Received packet already %i\n", it->sequence);
				it = packets->erase(it) - 1;
			}
			else if( it->timer == 45 )
			{
				numRetransmissions++;
				temp->push_back(it->sequence);
				//printf("Timeout packet %i resending\n", it->sequence);

			}
		}
	}

	for( std::vector<int>::iterator it = temp->begin(); it != temp->end(); it++ )
	{
		sendPacket(*it);
	}
	temp->clear();
	delete temp;
}

void Window::updateTimers()
{
	for( std::vector<Packet>::iterator it = packets->begin(); it != packets->end(); it++ )
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

int Window::getNumRetransmissions()
{
	return numRetransmissions;
}