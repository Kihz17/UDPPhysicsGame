#include "CollisionContainer.h"

#include <iostream>

CollisionContainer::CollisionContainer(size_t maxContacts, size_t iterations)
	: contactResolver(iterations),
	maxContacts(maxContacts),
	calculateIterations(iterations == 0),
	contacts(new ContactInstance[maxContacts])
{

}

CollisionContainer::~CollisionContainer()
{
	for (IContactGenerator* generator : contactGenerators)
	{
		delete generator;
	}

	delete[] contacts;
}

void CollisionContainer::OnUpdate(float deltaTime, std::vector<GameObject*>& gameObjects)
{
	size_t contactsGenerated = GenerateContacts(gameObjects);

	if (contactsGenerated)
	{
		if (this->calculateIterations)
		{
			this->contactResolver.SetIterations(contactsGenerated * 2);
		}

		this->contactResolver.ResolveContacts(this->contacts, contactsGenerated, deltaTime);
	}
}

size_t CollisionContainer::GenerateContacts(std::vector<GameObject*>& gameObjects)
{
	size_t contactLimit = this->maxContacts;
	ContactInstance* currentContact = this->contacts;

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