const int NUM_PACKETS = 100;
const int WINDOW_SIZE = 4;
const int SEED = 1;
const float ERROR_RATE = 0.0;

struct Packet()
{
	int rtt;
	bool error;
}

int main()
{
	Packet packets[NUM_PACKETS];

	srand( SEED );

	
	for(int i = 0; i < NUM_PACKETS; i++)
	{
		rtt = ( rand() + 10 ) % 50; // Get the round trip time of the packet
		error = ( rand() % 100 < ERROR_RATE * 100 ); // Set if failed


	}

}

