#pragma once

#include "PlayerController.h"

#include "ClientGameObject.h"

#include <ContactResolver.h>
#include <IContactGenerator.h>

#include <unordered_map>
#include <vector>

class World
{
public:
	World(size_t maxContacts, size_t iterations = 0);
	virtual ~World();

	void OnUpdate(float deltaTime);

	void AddGameObject(int id, GameObjectType type, float x, float y, float z);
	void RemoveGameObject(int id);
	ClientGameObject* GetGameObject(int id);

	std::vector<ClientGameObject*> GetGameObjects();

	bool AddContactGenerator(IContactGenerator* generator);
	bool RemoveContactGenerator(IContactGenerator* generator);

	inline void SetPlayerController(PlayerController* player) { this->player = player; }

	bool enableDeadReckoning;
	bool enablingLerping;

private:
	void UpdateGameObjects(float deltaTime);
	size_t GenerateContacts();

	std::unordered_map<int, ClientGameObject*> gameObjects;

	ContactInstance* contacts; // An array of contacts (length = maxContacts)
	size_t maxContacts;

	std::vector<IContactGenerator*> contactGenerators; // A list of the actual things creation contacts (AKA: The logic behind the collisions)

	ContactResolver contactResolver;
	bool calculateIterations;

	PlayerController* player;
};