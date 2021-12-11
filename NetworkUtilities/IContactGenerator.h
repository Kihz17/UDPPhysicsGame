#pragma once


#include "ContactInstance.h"

#include <vector>

class IContactGenerator
{
public:
	virtual ~IContactGenerator() = default;

	virtual size_t AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const = 0;
};