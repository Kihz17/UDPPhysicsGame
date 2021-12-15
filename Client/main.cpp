#include "Client.h"
#include "GLCommon.h"
#include "Input.h"
#include "Renderer.h"
#include "Light.h"
#include "AlphaTexture.h"
#include "DiffuseTexture.h"
#include "HeightmapTexture.h"
#include "DiscardTexture.h"
#include "Shader.h"
#include "Mesh.h"
#include "StaticScene.h"
#include "CollisionContainer.h"

#include <conio.h>
#include <sstream>

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(int argc, char** argv)
{
	World* world = new World();

	Client client(world);

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Initialize our window
	window = glfwCreateWindow(client.windowWidth, client.windowHeight, "Game", NULL, NULL);

	// Make sure the window initialized properly
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // Give glad this process ID
	glfwSwapInterval(1);

	Ref<Mesh> player = CreateRef<Mesh>("assets\\models\\sphere.obj");

	std::vector<Ref<SaveableTexture>> playerTextures;
	Ref<SaveableTexture> playerTexture = CreateRef<SaveableTexture>(CreateRef<DiffuseTexture>("assets\\textures\\danny.png", TextureFilterType::Linear, TextureWrapType::Clamp, true));
	playerTextures.push_back(playerTexture);

	Ref<Shader> shader = CreateRef<Shader>("Shader#1", "assets\\shaders\\vertexShader.glsl", "assets\\shaders\\fragmentShader.glsl");

	Renderer::Initialize(shader);
	Light::InitializeUniforms(shader);
	DiffuseTexture::InitializeUniforms(shader);
	HeightMapTexture::InitializeUniforms(shader);
	DiscardTexture::InitializeUniforms(shader);
	AlphaTexture::InitializeUniforms(shader);

	CollisionContainer* collisionContainer = new CollisionContainer(100);

	StaticScene scene(shader);
	scene.Load("scene.yaml", collisionContainer);

	client.CreateSocket("127.0.0.1", 5149);

	const glm::vec3 cameraOffset(0.0f, 2.0f, 0.0f);

	int ch;
	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;
	float previousTime = static_cast<float>(glfwGetTime());
	while (true)
	{
		if (Input::IsKeyPressed(Key::PageUp)) break;

		if (Input::IsKeyPressed(Key::One))
		{
			client.UpdateFrequency(1.0f);
		}
		else if (Input::IsKeyPressed(Key::Two))
		{
			client.UpdateFrequency(10.0f);
		}
		else if (Input::IsKeyPressed(Key::Three))
		{
			client.UpdateFrequency(60.0f);
		}
		else if (Input::IsKeyPressed(Key::Four))
		{
			client.UpdateFrequency(100.0f);
		}
		

		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "Game FPS: " + fps + "   MS: " + ms;
				glfwSetWindowTitle(window, newTitle.c_str());

				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}

		Renderer::BeginFrame(shader, client.GetCamera());

		// Safety, mostly for first frame
		if (deltaTime == 0.0f)
		{
			deltaTime = 0.03f;
		}

		client.OnUpdate(deltaTime);
		if (client.IsConnected())
		{	
			world->OnUpdate(deltaTime);
		}
	
		// Handle collisions
		{
			std::vector<GameObject*> gameObjects;
			if (client.GetPlayerController())
			{
				gameObjects.push_back(client.GetPlayerController());
			}
		
			std::unordered_map<int, ClientGameObject*>::iterator it = world->begin();
			while (it != world->end())
			{
				gameObjects.push_back(it->second);
				it++;
			}

			collisionContainer->OnUpdate(deltaTime, gameObjects);
		}

		scene.OnUpdate(client.GetCamera()->position, deltaTime);

		if (client.GetPlayerController())
		{
			glm::mat4& clientTransform = client.GetPlayerController()->GetTransform();
			client.GetCamera()->position = glm::vec3(clientTransform[3]) + cameraOffset + (client.GetCamera()->direction * -30.0f); // Update camera with controller after collisions take place
			Renderer::RenderMeshWithTextures(shader, player, playerTextures, clientTransform, 1.0f);
		}

		std::unordered_map<int, ClientGameObject*>::iterator it = world->begin();
		while (it != world->end())
		{
			GameObject* obj = it->second;
			Renderer::RenderMeshWithTextures(shader, player, playerTextures, obj->GetTransform(), 1.0f);
			it++;
		}

		Renderer::EndFrame();
	}

	delete world;
	delete collisionContainer;

	glfwDestroyWindow(window); // Clean up the window

	glfwTerminate();
	exit(EXIT_SUCCESS);
}