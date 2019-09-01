/*=============================================================================
NullFile.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/File.h"
#include "Core/IO/FileStream.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/Path.h"
#include "Core/System/Environment.h"

namespace SonataEngine
{

typedef File NullFile;

NullFile::File()
{
	Init();
}

NullFile::File(const String& name) :
	_name(name)
{
	Init();
}

NullFile::~File()
{
	Close();
}

void NullFile::Init()
{
	_fileAccess = FileAccess_Read;
	_fileShare = FileShare_None;
	_fileMode = FileMode_Open;
	_handle = NULL;
}

bool NullFile::Exists() const
{
	return false;
}

bool NullFile::Delete()
{
	return true;
}

bool NullFile::Copy(const String& fileName, bool overwrite)
{
	return true;
}

bool NullFile::Move(const String& fileName)
{
	return true;
}

WindowHandle NullFile::GetHandle() const
{
	return _handle;
}

int32 NullFile::GetLength() const
{
	return 0;
}

bool NullFile::IsOpen() const
{
	return false;
}

FileStreamPtr NullFile::Create()
{
	if (IsOpen())
	{
		return NULL;
	}

	if (_name.IsEmpty())
	{
		return NULL;
	}

	FileStream* stream = new FileStream(this);

	return stream;
}

FileStreamPtr NullFile::Open(FileMode FileMode, FileAccess FileAccess, FileShare FileShare)
{
	if (IsOpen())
	{
		return NULL;
	}

	if (_name.IsEmpty())
	{
		return NULL;
	}

	_fileMode = FileMode;
	_fileAccess = FileAccess;
	_fileShare = FileShare;

	FileStream* stream = new FileStream(this);

	return stream;
}

bool NullFile::Close()
{
	Init();

	return false;
}

}