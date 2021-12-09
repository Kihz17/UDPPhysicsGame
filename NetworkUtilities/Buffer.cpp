#include "Buffer.h"

Buffer::Buffer(size_t size)
	: readIndex(0)
	, writeIndex(0)
{
	Initialize(size);
}

Buffer::Buffer(void)
	: readIndex(0)
	, writeIndex(0)
{
	Initialize(512);
}

void Buffer::Initialize(size_t size)
{
	this->data.resize(size, 0);
}

void Buffer::Reset(void)
{
	readIndex = 0;
	writeIndex = 0;
}

void Buffer::WriteUInt32BE(uint32_t data)
{
	WriteUInt32BE(data, writeIndex);
	writeIndex += sizeof(uint32_t);
}

void Buffer::WriteUInt32BE(uint32_t data, int index)
{
	CheckBufferCapacity(sizeof(data));
	this->data[index] = data;
	this->data[index + 1] = data >> 8;
	this->data[index + 2] = data >> 16;
	this->data[index + 3] = data >> 24;
}

void Buffer::WriteUInt16BE(uint16_t data)
{
	WriteUInt16BE(data, writeIndex);
	writeIndex += sizeof(uint16_t);
}

void Buffer::WriteUInt16BE(uint16_t data, int index)
{
	CheckBufferCapacity(sizeof(data));
	this->data[index] = data;
	this->data[index + 1] = data >> 8;
}

void Buffer::WriteFloat(float data)
{
	uint32_t internal;
	memcpy(&internal, &data, sizeof(data));
	this->data[writeIndex] = internal;
	this->data[writeIndex + 1] = internal >> 8;
	this->data[writeIndex + 2] = internal >> 16;
	this->data[writeIndex + 3] = internal >> 24;
	writeIndex += sizeof(float);
}

void Buffer::WriteFloat(float data, int index)
{
	uint32_t internal;
	memcpy(&internal, &data, sizeof(data));
	this->data[index] = internal;
	this->data[index + 1] = internal >> 8;
	this->data[index + 2] = internal >> 16;
	this->data[index + 3] = internal >> 24;
}

void Buffer::WriteString(const std::string& data)
{
	WriteString(data, writeIndex);
	writeIndex += data.length();
}

void Buffer::WriteString(const std::string& data, int index)
{
	size_t strLength = data.length();
	CheckBufferCapacity(strLength);
	memcpy(&this->data[index], &data[0], strLength);
}

uint32_t Buffer::ReadUInt32LE(void)
{
	uint32_t value = ReadUInt32LE(readIndex);
	readIndex += sizeof(uint32_t);
	return value;
}

uint32_t Buffer::ReadUInt32LE(int index)
{
	uint32_t value = this->data[index];
	value |= this->data[index + 1] << 8;
	value |= this->data[index + 2] << 16;
	value |= this->data[index + 3] << 24;
	return value;
}

uint16_t Buffer::ReadUInt16LE(void)
{
	uint16_t value = ReadUInt16LE(readIndex);
	readIndex += sizeof(uint16_t);
	return value;
}

uint16_t Buffer::ReadUInt16LE(int index)
{
	uint16_t value = this->data[index];
	value |= this->data[index + 1] << 8;
	return value;
}

float Buffer::ReadFloat()
{
	uint32_t internal = this->data[readIndex];
	internal |= this->data[readIndex + 1] << 8;
	internal |= this->data[readIndex + 2] << 16;
	internal |= this->data[readIndex + 3] << 24;
	readIndex += sizeof(float);

	float value;
	memcpy(&value, &internal, sizeof(float));
	return value;
}

float Buffer::ReadFloat(int index)
{
	uint32_t internal = this->data[index];
	internal |= this->data[index + 1] << 8;
	internal |= this->data[index + 2] << 16;
	internal |= this->data[index + 3] << 24;

	float value;
	memcpy(&value, &internal, sizeof(float));
	return value;
}

std::string Buffer::ReadString(size_t length)
{
	std::string value = ReadString(length, readIndex);
	readIndex += length;
	return value;
}

std::string Buffer::ReadString(size_t length, int index)
{
	std::string value = "";
	for (int i = 0; i < length; i++)
	{
		value += this->data[index + i];
	}
	readIndex += length;
	return value;
}

void Buffer::CheckBufferCapacity(size_t size)
{
	if (writeIndex + size >= this->data.size())
		GrowVector();
}

void Buffer::GrowVector(void)
{
	this->data.resize(this->data.size() * 2, 0);
}

uint8_t* Buffer::GetData(void)
{
	return this->data.data();
}