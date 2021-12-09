#pragma once

#include "Packet.h"

class PacketPlayerConnectSuccess : public Packet
{
public:
	PacketPlayerConnectSuccess(int playerId);
	PacketPlayerConnectSuccess(Buffer& buffer);
	virtual ~PacketPlayerConnectSuccess() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int) * 2; };

	int playerId;
};