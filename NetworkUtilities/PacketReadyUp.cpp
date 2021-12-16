#include "PacketReadyUp.h"

PacketReadyUp::PacketReadyUp()
{

}

void PacketReadyUp::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(6); // Packet type ID
}

Buffer PacketReadyUp::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(6); // Packet type ID
	return buffer;
}

void PacketReadyUp::Deserialize(Buffer& buffer)
{

}