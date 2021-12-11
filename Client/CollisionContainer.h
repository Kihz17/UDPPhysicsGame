#pragma once

#include <ContactInstance.h>
#include <IContactGenerator.h>
#include <ContactResolver.h>

class CollisionContainer
{
public:
	CollisionContainer(size_t maxContacts, size_t iterations = 0);
	virtual ~CollisionContainer();

	inline void AddGenerator(IContactGenerator* generator) { contactGenerators.push_back(generator); }

	void OnUpdate(float deltaTime, std::vector<GameObject*>& gameObjects);
private:
	size_t GenerateContacts(std::vector<GameObject*>& gameObjects);

	ContactInstance* contacts;
	size_t maxContacts;

	std::vector<IContactGenerator*> contactGenerators;

	ContactResolver contactResolver;
	bool calculateIterations;
};