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

			if (ch == 27) break; // Escape
		}

		server.OnUpdate();
	}

	return 0;
}