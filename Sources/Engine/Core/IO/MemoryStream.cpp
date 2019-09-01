/*=============================================================================
MemoryStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MemoryStream.h"
#include "Core/System/Memory.h"
#include "Core/IO/IOException.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"
 
namespace SonataEngine
{

MemoryStream::MemoryStream()
{
	Initialize();
}

MemoryStream::MemoryStream(int32 capacity) :
	Stream()
{
	if (capacity < 0)
	{
		SEthrow(ArgumentOutOfRangeException("capacity"));
		return;
	}

	Initialize();

	_capacity = capacity;
}

MemoryStream::MemoryStream(SEbyte* buffer, int32 count, bool writable) :
	Stream()
{
	if (buffer == NULL)
	{
		SEthrow(ArgumentNullException("buffer"));
		return;
	}

	if (count < 0)
	{
		SEthrow(ArgumentOutOfRangeException("count"));
		return;
	}

	Initialize();

	_buffer = buffer;
	_capacity = count;
	_writable = writable;
	_expandable = false;
	_exposable = false;
	_length = count;
}

MemoryStream::MemoryStream(SEbyte* buffer, int32 index, int32 count, bool writable) :
	Stream()
{
	if (buffer == NULL)
	{
		SEthrow(ArgumentNullException("buffer"));
		return;
	}

	if (index < 0)
	{
		SEthrow(ArgumentOutOfRangeException("index"));
		return;
	}

	if (count < 0)
	{
		SEthrow(ArgumentOutOfRangeException("count"));
		return;
	}

	Initialize();

	_buffer = buffer;
	_capacity = count;
	_writable = writable;
	_expandable = false;
	_exposable = false;
	_origin = index;
	_length = count;
}

MemoryStream::~MemoryStream()
{
}

void MemoryStream::Initialize()
{
	_buffer = NULL;
	_capacity = 0;
	_writable = true;
	_expandable = true;
	_exposable = true;
	_isOpen = true;
	_origin = 0;
	_length = 0;
	_position = 0;
}

bool MemoryStream::CanRead() const
{
	return _isOpen;
}

bool MemoryStream::CanWrite() const
{
	return _isOpen && _writable;
}

bool MemoryStream::CanSeek() const
{
	return _isOpen;
}

int32 MemoryStream::GetLength() const
{
	if (!_isOpen)
		return 0;

	return _length;
}

void MemoryStream::SetLength(int32 value)
{
	if (!_isOpen)
		return;

	if (!_writable)
		return;

	_length = value;
	if (_position > _length)
		_position = _length;
}

int32 MemoryStream::GetPosition() const
{
	if (!_isOpen)
		return 0;

	return _position;
}

void MemoryStream::SetPosition(int32 value)
{
	Seek(value, SeekOrigin_Begin);
}

int32 MemoryStream::GetCapacity() const
{
	if (!_isOpen)
		return 0;

	return _capacity - _origin;
}

void MemoryStream::SetCapacity(int32 value)
{
	if (!_isOpen)
		return;

	if (value <= 0)
	{
		_buffer = NULL;
		_capacity = 0;
	}
	else if (value < _capacity)
	{
		_capacity = value;
	}
	else if (value > _capacity)
	{
		_capacity = value;
	}
}

SEbyte* MemoryStream::GetBuffer() const
{
	if (!_exposable)
		return NULL;

	return _buffer;
}

void MemoryStream::Close()
{
	Initialize();
}

void MemoryStream::Flush()
{
}

int32 MemoryStream::Seek(int32 offset, SeekOrigin origin)
{
	if (!_isOpen)
		return 0;

	switch (origin)
	{
	case SeekOrigin_Begin:
		if (offset < 0)
		{
			SEthrow(IOException("SeekBeforeBegin"));
			return 0;
		}
		_position = (_origin + offset);
		break;

	case SeekOrigin_Current:
		if ((_position + offset) < _origin)
		{
			SEthrow(IOException("SeekBeforeBegin"));
			return 0;
		}
		_position += offset;
		break;

	case SeekOrigin_End:
		if ((_length + offset) < _origin)
		{
			SEthrow(IOException("SeekBeforeBegin"));
			return 0;
		}
		_position = (_length + offset);
		break;

	default:
		SEthrow(IOException("InvalidSeekOrigin"));
		return 0;
	}

	return _position;
}

bool MemoryStream::IsEOF() const
{
	return _position >= _length;
}

SEbyte MemoryStream::ReadByte()
{
	if (!_isOpen)
	{
		return 0;
	}

	if (_position >= _length)
	{
		return -1;
	}

	return _buffer[_position++];
}

int32 MemoryStream::Read(SEbyte* buffer, int32 count)
{
	if (!_isOpen)
		return 0;

	if (buffer == NULL)
	{
		SEthrow(ArgumentNullException("buffer"));
		return 0;
	}

	if (count < 0)
	{
		SEthrow(ArgumentOutOfRangeException("count"));
		return 0;
	}

	int32 size = (_length - _position);
	if (size > count)
	{
		size = count;
	}

	if (size < 0)
	{
		return 0;
	}

	if (_position >= _length)
	{
		return -1;
	}

	Memory::Copy(buffer, _buffer + _position, size);

	_position += size;
	return _position;
}

void MemoryStream::WriteByte(SEbyte value)
{
	if (!_isOpen)
		return;

	if (!_writable)
		return;

	int32 size = _position + 1;
	if (size >= _capacity)
	{
		if (size < _capacity * 2)
			_capacity = _capacity * 2;
		else
			_capacity = size;

		_length = size;
	}

	_buffer[_position++] = value;
}

int32 MemoryStream::Write(const SEbyte* buffer, int32 count)
{
	if (!_isOpen)
		return 0;

	if (!_writable)
		return 0;

	if (buffer == NULL)
	{
		SEthrow(ArgumentNullException("buffer"));
		return 0;
	}

	if (count < 0)
	{
		SEthrow(ArgumentOutOfRangeException("count"));
		return 0;
	}

	int32 size = (_position + count);
	if (size > count)
	{
		size = count;
	}

	if (size < 0)
	{
		return 0;
	}

	if (_position >= _length)
	{
		return -1;
	}

	Memory::Copy(_buffer + _position, (void*)buffer, size);

	_position += size;
	return _position;
}

}
