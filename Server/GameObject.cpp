#include "GameObject.h"

static int gameObjectId = 0;

GameObject::GameObject() : id(GetNextGameObjectId()), transform(1.0f) {}

int GameObject::GetNextGameObjectId()
{
	return gameObjectId++;
}