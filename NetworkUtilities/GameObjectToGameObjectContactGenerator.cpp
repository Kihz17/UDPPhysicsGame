#include "GameObjectToGameObjectContactGenerator.h"
#include "CollisionHandler.h"

size_t GameObjectToGameObjectContactGenerator::AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const
{
	if (gameObjects.empty()) // No need to loop
	{
		return 0;
	}

	size_t contactsGenerated = 0;
	for (int i = 0; i < gameObjects.size() - 1; i++)
	{
		for (int j = i + 1; j < gameObjects.size(); j++)
		{
			if (contactsGenerated > contactLimit)
			{
				return contactsGenerated;
			}

			GameObject* gameObject1 = gameObjects[i];
			GameObject* gameObject2 = gameObjects[j];
			if (gameObject1 == gameObject2)
			{
				continue;
			}

			CollisionHandler::HandleCollision(contactInstance, gameObject1, gameObject2, &contactsGenerated);
		}
	}

	return contactsGenerated;
}