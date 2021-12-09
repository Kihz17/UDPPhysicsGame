#include "Client.h"
#include "GLCommon.h"

#include <conio.h>

int main(int argc, char** argv)
{
	Client client;
	client.CreateSocket("127.0.0.1", 5149);

	int ch;
	float previousTime = static_cast<float>(glfwGetTime());
	while (true)
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// Safety, mostly for first frame
		if (deltaTime == 0.0f)
		{
			deltaTime = 0.03f;
		}

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
				buffer.WriteUInt32BE(7);
				client.Send(buffer);
				break;
			}	
			}
		}
		client.OnUpdate(deltaTime);
	}
}