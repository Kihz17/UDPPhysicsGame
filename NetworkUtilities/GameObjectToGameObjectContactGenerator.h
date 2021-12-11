#pragma once

#include "IContactGenerator.h"

class GameObjectToGameObjectContactGenerator : public IContactGenerator
{
public:
	GameObjectToGameObjectContactGenerator() = default;
	~GameObjectToGameObjectContactGenerator() = default;

	virtual size_t AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const override;
};