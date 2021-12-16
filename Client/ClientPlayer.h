#pragma once

#include "ClientGameObject.h"

class ClientPlayer : public ClientGameObject
{
public:
	ClientPlayer(int id, const glm::mat4& transform);
	virtual ~ClientPlayer();

	inline virtual GameObjectType GetType() const override { return GameObjectType::Player; }
};