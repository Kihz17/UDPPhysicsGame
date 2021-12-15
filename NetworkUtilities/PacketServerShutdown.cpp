#include "PacketServerShutdown.h"

PacketServerShutdown::PacketServerShutdown()
{

}

void PacketServerShutdown::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(5); // Packet type ID
}

Buffer PacketServerShutdown::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(5); // Packet type ID
	return buffer;
}

void PacketServerShutdown::Deserialize(Buffer& buffer)
{
	
}