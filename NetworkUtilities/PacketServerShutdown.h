#pragma once

#include "Packet.h"

class PacketServerShutdown : public Packet
{
public:
	PacketServerShutdown();
	virtual ~PacketServerShutdown() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int); };
};