/*=============================================================================
Stream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Stream.h"
#include "Core/System/Environment.h"

namespace SonataEngine
{

#define SE_ENDIAN_READ(i, t) \
	if (_swap) \
	{ \
		Read((SEbyte*)&i, sizeof(t)); \
		Environment::SwapEndian((SEbyte*)&i, sizeof(t)); \
	} \
	else \
	{ \
		Read((SEbyte*)&i, sizeof(t)); \
	}

#define SE_ENDIAN_WRITE(i, t) \
	if (_swap) \
	{ \
		Environment::SwapEndian((SEbyte*)&i, sizeof(t)); \
		Write((SEbyte*)&i, sizeof(t)); \
	} \
	else \
	{ \
		Write((SEbyte*)&i, sizeof(t)); \
	}

Stream::Stream()
{
	_byteOrder = ByteOrder_LittleEndian;
	_swap = (_byteOrder != SE_ENDIAN);
}

Stream::~Stream()
{
}

void Stream::SetByteOrder(ByteOrder byteOrder)
{
	_byteOrder = byteOrder;
	_swap = (_byteOrder != SE_ENDIAN);
}

Stream& Stream::operator>>(int8& i)
{
	Read((SEbyte*)&i, sizeof(int8));
	return *this;
}

Stream& Stream::operator>>(uint8& i)
{
	Read((SEbyte*)&i, sizeof(uint8));
	return *this;
}

Stream& Stream::operator>>(int16& i)
{
	SE_ENDIAN_READ(i, int16);
	return *this;
}

Stream& Stream::operator>>(uint16& i)
{
	SE_ENDIAN_READ(i, uint16);
	return *this;
}

Stream& Stream::operator>>(int32& i)
{
	SE_ENDIAN_READ(i, int32);
	return *this;
}

Stream& Stream::operator>>(uint32& i)
{
	SE_ENDIAN_READ(i, uint32);
	return *this;
}

Stream& Stream::operator>>(int64& i)
{
	SE_ENDIAN_READ(i, int64);
	return *this;
}

Stream& Stream::operator>>(uint64& i)
{
	SE_ENDIAN_READ(i, uint64);
	return *this;
}

Stream& Stream::operator>>(bool& b)
{
	SE_ENDIAN_READ(b, bool);
	return *this;
}

Stream& Stream::operator>>(real32& r)
{
	SE_ENDIAN_READ(r, real32);
	return *this;
}

Stream& Stream::operator>>(real64& r)
{
	SE_ENDIAN_READ(r, real64);
	return *this;
}

Stream& Stream::operator>>(String& str)
{
	int32 length;
	*this >> length;

	SEbyte* buffer = new SEbyte[length+1];
	Read(buffer, length);
	buffer[length] = '\0';

	str = (char*)buffer;
	delete buffer;
	return *this;
}

Stream& Stream::operator<<(int8 i)
{
	Write((SEbyte*)&i, sizeof(int8));
	return *this;
}

Stream& Stream::operator<<(uint8 i)
{
	Write(&i, sizeof(uint8));
	return *this;
}

Stream& Stream::operator<<(int16 i)
{
	SE_ENDIAN_WRITE(i, int16);
	return *this;
}

Stream& Stream::operator<<(uint16 i)
{
	SE_ENDIAN_WRITE(i, uint16);
	return *this;
}

Stream& Stream::operator<<(int32 i)
{
	SE_ENDIAN_WRITE(i, int32);
	return *this;
}

Stream& Stream::operator<<(uint32 i)
{
	SE_ENDIAN_WRITE(i, uint32);
	return *this;
}

Stream& Stream::operator<<(int64 i)
{
	SE_ENDIAN_WRITE(i, int64);
	return *this;
}

Stream& Stream::operator<<(uint64 i)
{
	SE_ENDIAN_WRITE(i, uint64);
	return *this;
}

Stream& Stream::operator<<(bool b)
{
	SE_ENDIAN_WRITE(b, bool);
	return *this;
}

Stream& Stream::operator<<(real32 r)
{
	SE_ENDIAN_WRITE(r, real32);
	return *this;
}

Stream& Stream::operator<<(real64 r)
{
	SE_ENDIAN_WRITE(r, real64);
	return *this;
}

Stream& Stream::operator<<(const String& str)
{
	*this << str.Length();
	Write((SEbyte*)str.Data(), str.Length());
	return *this;
}

}
