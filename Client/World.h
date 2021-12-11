#pragma once

#include "ClientGameObject.h"

#include <ContactResolver.h>
#include <IContactGenerator.h>

#include <unordered_map>

class World
{
public:
	World();
	virtual ~World();

	void OnUpdate(float deltaTime);

	void AddGameObject(int id, GameObjectType type, float x, float y, float z);
	void RemoveGameObject(int id);
	ClientGameObject* GetGameObject(int id);

	inline std::unordered_map<int, ClientGameObject*>::iterator begin() { return gameObjects.begin(); }
	inline std::unordered_map<int, ClientGameObject*>::iterator end() { return gameObjects.end(); }

private:
	void UpdateGameObjects(float deltaTime);

	std::unordered_map<int, ClientGameObject*> gameObjects;
};