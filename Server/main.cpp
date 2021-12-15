#include <conio.h>

#include "Server.h"

int main(int argc, char** argv)
{
	Server server;
	int ch;

	while (true)
	{
		if (_kbhit())
		{
			ch = _getch();

			if (ch == 27)  // Escape
			{
				server.OnShutdown();
				break;
			}
			else if (ch == '1')
			{
				server.UpdateFrequency(1.0f);
				printf("Server freqency set to %d\n", 1);
			}
			else if (ch == '2')
			{
				server.UpdateFrequency(10.0f);
				printf("Server freqency set to %d\n", 10);
			}
			else if (ch == '3')
			{
				server.UpdateFrequency(60.0f);
				printf("Server freqency set to %d\n", 60);
			}
			else if (ch == '4')
			{
				server.UpdateFrequency(100.0f);
				printf("Server freqency set to %d\n", 100);
			}
		}

		server.OnUpdate();
	}

	return 0;
}