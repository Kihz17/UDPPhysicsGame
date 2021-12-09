#include "Client.h"

#include <conio.h>

int main(int argc, char** argv)
{
	Client client;
	client.CreateSocket("127.0.0.1", 5149);

	int ch;
	while (true)
	{
		if (_kbhit())
		{
			ch = _getch();

			switch (ch)
			{
			case 'a':
				system("CLS");
				break;
			default:
			{
				Buffer buffer(4);
				buffer.WriteInt(7);
				client.Send(buffer);
				break;
			}	
			}
		}
		client.OnUpdate();
	}
}