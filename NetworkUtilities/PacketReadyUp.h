#pragma once

#include "Packet.h"

class PacketReadyUp : public Packet
{
public:
	PacketReadyUp();
	virtual ~PacketReadyUp() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int); };
};