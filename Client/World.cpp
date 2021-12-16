#include "World.h"
#include "ClientPlayer.h"

#include <PlayerInfo.h>
#include <iostream>

World::World(size_t maxContacts, size_t iterations)
	: player(nullptr), contactResolver(iterations),
	 enableDeadReckoning(true),
	enablingLerping(true)
{
	this->calculateIterations = (iterations == 0);
	this->contacts = new ContactInstance[maxContacts];
	this->maxContacts = maxContacts;
}

World::~World()
{
	for (IContactGenerator* generator : this->contactGenerators)
	{
		delete generator;
	}

	delete[] this->contacts;
}

void World::OnUpdate(float deltaTime)
{
	UpdateGameObjects(deltaTime);

	size_t contactsGenerated = GenerateContacts();

	if (contactsGenerated)
	{
		if (this->calculateIterations)
		{
			this->contactResolver.SetIterations(contactsGenerated * 2);
		}

		this->contactResolver.ResolveContacts(this->contacts, contactsGenerated, deltaTime);
	}
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
	if (player)
	{
		player->Update(deltaTime);
	}

	{
		std::unordered_map<int, ClientGameObject*>::iterator it = gameObjects.begin();
		while (it != gameObjects.end())
		{
			ClientGameObject* gameObject = it->second;
			gameObject->ApplyForce(PlayerInfo::Gravity * gameObject->GetMass());
			gameObject->Update(deltaTime);
			it++;
		}
	}
}

size_t World::GenerateContacts()
{
	size_t contactLimit = this->maxContacts;
	ContactInstance* currentContact = this->contacts;

	std::vector<GameObject*> gameObjects;
	for(ClientGameObject* go : GetGameObjects()) gameObjects.push_back(go);

	for (IContactGenerator* generator : this->contactGenerators)
	{
		size_t contactsGenerated = generator->AddContact(gameObjects, currentContact, contactLimit); // Creates contacts, and returns the amount of contacts generated
		contactLimit -= contactsGenerated;
		currentContact += contactsGenerated; // Move to our next available position in the contacts array

		if (contactLimit <= 0)
		{
			std::cout << "WARNING: We have created too many contacts to process!" << std::endl;
			break;
		}
	}

	return this->maxContacts - contactLimit; // Return the amount of contacts generated
}

std::vector<ClientGameObject*> World::GetGameObjects()
{
	std::vector<ClientGameObject*> objects;

	if(player) objects.push_back(player);

	std::unordered_map<int, ClientGameObject*>::iterator it = gameObjects.begin();
	while (it != gameObjects.end())
	{
		objects.push_back(it->second);
		it++;
	}
	return objects;
}

bool World::AddContactGenerator(IContactGenerator* generator)
{
	if (!generator)
	{
		return false;
	}

	std::vector<IContactGenerator*>::iterator it = std::find(this->contactGenerators.begin(), this->contactGenerators.end(), generator);
	if (it == this->contactGenerators.end())
	{
		this->contactGenerators.push_back(generator);
		return true;
	}

	return false;
}

bool World::RemoveContactGenerator(IContactGenerator* generator)
{
	if (!generator)
	{
		return false;
	}

	std::vector<IContactGenerator*>::iterator it = std::find(this->contactGenerators.begin(), this->contactGenerators.end(), generator);
	if (it == this->contactGenerators.end())
	{
		return false;
	}

	this->contactGenerators.erase(it);
	return true;
}
