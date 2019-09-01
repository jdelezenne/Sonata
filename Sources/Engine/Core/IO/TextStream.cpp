/*=============================================================================
TextStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TextStream.h"
#include "Core/IO/IOException.h"
#include "Core/System/Environment.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"

namespace SonataEngine
{

TextStream::TextStream(Stream* stream)
{
	if (stream == NULL)
	{
		SEthrow(ArgumentNullException("stream", "stream is a null reference"));
		return;
	}

	_stream = stream;
	_newLine = Environment::NewLine();
}

TextStream::~TextStream()
{
}

void TextStream::Close()
{
	_stream->Close();
}

SEchar TextStream::Peek()
{
	if (!_stream->CanRead() || !_stream->CanSeek())
	{
		SEthrow(IOException("The stream must be readable and seekable"));
		return 0;
	}

	SEchar value;
	value = _stream->ReadByte();
	_stream->SetPosition(_stream->GetPosition() - 1);
	return value;
}

SEchar TextStream::Read()
{
	if (!_stream->CanRead())
	{
		SEthrow(IOException("The stream must be readable"));
		return 0;
	}

	SEchar value;
	value = _stream->ReadByte();
	return value;
}

int32 TextStream::Read(SEchar* buffer, int32 count)
{
	if (!_stream->CanRead())
	{
		SEthrow(IOException("The stream must be readable"));
		return 0;
	}

	SEbyte* data = new SEbyte[count];
	int32 res;
	res = _stream->Read(data, count);

	for (int i = 0; i < count; i++)
	{
		buffer[i] = data[i];
	}

	delete data;

	return res;
}

String TextStream::ReadLine()
{
	if (!_stream->CanRead())
	{
		SEthrow(IOException("The stream must be readable"));
		return String::Empty;
	}

	if (_newLine.IsEmpty())
	{
		return ReadToEnd();
	}

	String value;
	SEchar next;
	int index = 0;
	while ((next = _stream->ReadByte()) != -1)
	{
		if (next == _newLine[index])
		{
			index++;
			if (index == _newLine.Length())
			{
				value = value.Left(value.Length() - _newLine.Length() + 1);
				break;
			}
		}
		else
		{
			index = 0;
		}

		value += String(next);
	}
	return value;
}

String TextStream::ReadToEnd()
{
	if (!_stream->CanRead())
	{
		SEthrow(IOException("The stream must be readable"));
		return String::Empty;
	}

	String value;
	SEchar next;
	while ((next = _stream->ReadByte()) != -1)
	{
		value += String(next);
	}

	return value;
}

void TextStream::Write(SEchar value)
{
	if (!_stream->CanWrite())
	{
		SEthrow(IOException("The stream must be writable"));
		return;
	}

	_stream->Write((SEbyte*)&value, sizeof(SEchar));
}

void TextStream::Write(const String& value)
{
	if (!_stream->CanWrite())
	{
		SEthrow(IOException("The stream must be writable"));
		return;
	}

	_stream->Write((SEbyte*)value.Data(), value.Length()*sizeof(SEchar) + 1);
}

void TextStream::WriteLine()
{
	Write(_newLine);
}

void TextStream::WriteLine(SEchar value)
{
	Write(value);
	WriteLine();
}

void TextStream::WriteLine(const String& value)
{
	Write(value);
	WriteLine();
}

}
