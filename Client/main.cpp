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

	client.CreateSocket("173.34.12.232", 5149);

	int ch;
	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;
	float previousTime = static_cast<float>(glfwGetTime());
	while (true)
	{
		if (Input::IsKeyPressed(Key::PageUp)) break;

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
		world->OnUpdate(deltaTime);

		// Handle collisions
		{
			std::vector<GameObject*> gameObjects;
			gameObjects.push_back(client.GetPlayerController());

			std::unordered_map<int, ClientGameObject*>::iterator it = world->begin();
			while (it != world->end())
			{
				gameObjects.push_back(it->second);
				it++;
			}

			collisionContainer->OnUpdate(deltaTime, gameObjects);
		}

		scene.OnUpdate(client.GetCamera()->position, deltaTime);

		std::unordered_map<int, ClientGameObject*>::iterator it = world->begin();
		while (it != world->end())
		{
			GameObject* obj = it->second;
			glm::mat4 transform = obj->GetTransformSafe();
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