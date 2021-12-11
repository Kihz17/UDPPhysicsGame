#pragma once

#include "ContactInstance.h"

class ContactResolver
{
public:
	ContactResolver(size_t iterations);

	void SetIterations(size_t iterations);
	void ResolveContacts(ContactInstance* contacts, size_t contactAmount, float deltaTime);

private:
	size_t iterations; // The number of iterations we are allowed to execute
	size_t iterationsUsed; // Keeps a record of the iterations used
};