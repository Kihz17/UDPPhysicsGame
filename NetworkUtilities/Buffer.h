#pragma once

#include <vector>
#include <string>

class Buffer
{
public:
	Buffer(size_t size);
	Buffer();

	void Initialize(size_t size);

	void Reset();

	void WriteUInt32BE(uint32_t data);
	void WriteUInt32BE(uint32_t data, int index);

	void WriteUInt16BE(uint16_t data);
	void WriteUInt16BE(uint16_t data, int index);

	void WriteFloat(float data);
	void WriteFloat(float data, int index);

	void WriteString(const std::string& data);
	void WriteString(const std::string& data, int index);

	uint32_t ReadUInt32LE();
	uint32_t ReadUInt32LE(int index);

	uint16_t ReadUInt16LE();
	uint16_t ReadUInt16LE(int index);

	float ReadFloat();
	float ReadFloat(int index);

	std::string ReadString(size_t length);
	std::string ReadString(size_t length, int index);

	uint8_t* GetData();

	inline size_t Length() { return data.size(); }

private:
	void CheckBufferCapacity(size_t size);
	void GrowVector();

	std::vector<uint8_t> data;
	unsigned int readIndex;
	unsigned int writeIndex;
};