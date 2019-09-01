/*=============================================================================
BinaryStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BinaryStream.h"
#include "Core/IO/IOException.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"
 
namespace SonataEngine
{

BinaryStream::BinaryStream(Stream* stream)
{
	if (stream == NULL)
	{
		SEthrow(ArgumentNullException("stream", "stream is a null reference."));
	}

	_stream = stream;
}

BinaryStream::~BinaryStream()
{
}

void BinaryStream::Close()
{
	_stream->Close();
}

SEbyte BinaryStream::Peek()
{
	if (!_stream->CanRead() || !_stream->CanSeek())
		SEthrow(IOException("The stream must be readable and seekable."));

	SEbyte value;
	value = _stream->ReadByte();
	_stream->SetPosition(_stream->GetPosition() - 1);
	return value;
}

SEbyte BinaryStream::Read()
{
	if (!_stream->CanRead())
		SEthrow(IOException("The stream must be readable."));

	SEbyte value;
	value = _stream->ReadByte();
	return value;
}

int8 BinaryStream::ReadInt8()
{
	int8 value;
	Read((SEbyte*)&value, sizeof(int8));
	return value;
}

uint8 BinaryStream::ReadUInt8()
{
	uint8 value;
	Read((SEbyte*)&value, sizeof(uint8));
	return value;
}

int16 BinaryStream::ReadInt16()
{
	int16 value;
	Read((SEbyte*)&value, sizeof(int16));
	return value;
}

uint16 BinaryStream::ReadUInt16()
{
	uint16 value;
	Read((SEbyte*)&value, sizeof(uint16));
	return value;
}

int32 BinaryStream::ReadInt32()
{
	int32 value;
	Read((SEbyte*)&value, sizeof(int32));
	return value;
}

uint32 BinaryStream::ReadUInt32()
{
	uint32 value;
	Read((SEbyte*)&value, sizeof(uint32));
	return value;
}

int64 BinaryStream::ReadInt64()
{
	int64 value;
	Read((SEbyte*)&value, sizeof(int64));
	return value;
}

uint64 BinaryStream::ReadUInt64()
{
	uint64 value;
	Read((SEbyte*)&value, sizeof(uint64));
	return value;
}

real32 BinaryStream::ReadReal32()
{
	real32 value;
	Read((SEbyte*)&value, sizeof(real32));
	return value;
}

real64 BinaryStream::ReadReal64()
{
	real64 value;
	Read((SEbyte*)&value, sizeof(real64));
	return value;
}

String BinaryStream::ReadString()
{
	uint32 length;
	SEchar* buffer;

	length = ReadUInt32();
	buffer = new SEchar[length+1];
	Read((SEbyte*)buffer, length*sizeof(SEchar));
	buffer[length] = _T('\0');
	String str = buffer;
	delete[] buffer;
	return str;
}

int32 BinaryStream::Read(SEbyte* buffer, int32 count)
{
	if (!_stream->CanRead())
		SEthrow(IOException("The stream must be readable."));

	return _stream->Read(buffer, count);
}

void BinaryStream::Write(SEbyte value)
{
	if (!_stream->CanWrite())
		SEthrow(IOException("The stream must be writable."));

	_stream->WriteByte(value);
}

void BinaryStream::WriteInt8(int8 value)
{
	Write((SEbyte*)&value, sizeof(int8));
}

void BinaryStream::WriteUInt8(uint8 value)
{
	Write((SEbyte*)&value, sizeof(uint8));
}

void BinaryStream::WriteInt16(int16 value)
{
	Write((SEbyte*)&value, sizeof(int16));
}

void BinaryStream::WriteUInt16(uint16 value)
{
	Write((SEbyte*)&value, sizeof(uint16));
}

void BinaryStream::WriteInt32(int32 value)
{
	Write((SEbyte*)&value, sizeof(int32));
}

void BinaryStream::WriteUInt32(uint32 value)
{
	Write((SEbyte*)&value, sizeof(uint32));
}

void BinaryStream::WriteInt64(int64 value)
{
	Write((SEbyte*)&value, sizeof(int64));
}

void BinaryStream::WriteUInt64(uint64 value)
{
	Write((SEbyte*)&value, sizeof(uint64));
}

void BinaryStream::WriteReal32(real32 value)
{
	Write((SEbyte*)&value, sizeof(real32));
}

void BinaryStream::WriteReal64(real64 value)
{
	Write((SEbyte*)&value, sizeof(real64));
}

void BinaryStream::WriteString(const String& value)
{
	WriteUInt32(value.Length());
	Write((SEbyte*)value.Data(), value.Length()*sizeof(SEchar));
}

void BinaryStream::Write(SEbyte* buffer, int32 count)
{
	if (!_stream->CanWrite())
		SEthrow(IOException("The stream must be writable."));

	_stream->Write(buffer, count);
}

}
