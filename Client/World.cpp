#include "World.h"
#include "ClientPlayer.h"

#include <PlayerInfo.h>
#include <iostream>

World::World()
{

}

World::~World()
{

}

void World::OnUpdate(float deltaTime)
{
	UpdateGameObjects(deltaTime);
}

void World::AddGameObject(int id, GameObjectType type, float x, float y, float z)
{
	if (gameObjects.find(id) != gameObjects.end())
	{
		printf("GameObject with ID %d already present! We should not be getting this!\n", id);
		return;
	}

	ClientGameObject* gameObject;
	glm::mat4 transform(1.0f);
	transform[3].x = x;
	transform[3].y = y;
	transform[3].z = z;

	switch (type)
	{
	case GameObjectType::Player:
		gameObject = new ClientPlayer(id, transform);
		break;
	case GameObjectType::Cuboid:
	case GameObjectType::Sphere:
	default:
		printf("No factory handler for GameObjectType %d", type);
		return;
	}

	gameObjects.insert({ id, gameObject });
}

void World::RemoveGameObject(int id)
{
	gameObjects.erase(id);
}

ClientGameObject* World::GetGameObject(int id)
{
	std::unordered_map<int, ClientGameObject*>::iterator it = gameObjects.find(id);
	if (it == gameObjects.end()) return nullptr;
	return it->second;
}

void World::UpdateGameObjects(float deltaTime)
{
	std::unordered_map<int, ClientGameObject*>::iterator it = gameObjects.begin();
	while (it != gameObjects.end())
	{
		ClientGameObject* gameObject = it->second;
		gameObject->ApplyForce(PlayerInfo::Gravity * gameObject->GetMass());
		gameObject->Update(deltaTime);
		gameObject->timeSinceLastUpdate += deltaTime;

		//glm::mat4& transform = gameObject->GetTransform();

		//glm::vec3 velocity = gameObject->GetLastVelocity() * (0.25f + gameObject->timeSinceLastUpdate);

		//// Update postion based off of the last move state update
		//transform[3].x += velocity.x;
		//transform[3].y += velocity.y;
		//transform[3].z += velocity.z;


		it++;
	}
}