#pragma once

#include "Buffer.h"

class Packet
{
public:
	virtual ~Packet() = default;

	virtual void Serialize(Buffer& buffer) const = 0;
	virtual Buffer Serialize() const = 0;
	virtual void Deserialize(Buffer& buffer) = 0;

	virtual size_t GetSize() const = 0;
};