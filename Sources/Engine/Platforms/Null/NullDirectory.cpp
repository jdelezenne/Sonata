/*=============================================================================
NullDirectory.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/Directory.h"
#include "Core/IO/File.h"
#include "Core/IO/FileSystem.h"

namespace SonataEngine
{

typedef Directory NullDirectory;

NullDirectory::Directory()
{
}

NullDirectory::Directory(const String& name) :
	_name(name)
{
}

NullDirectory::~Directory()
{
}

bool NullDirectory::Exists() const
{
	return false;
}

bool NullDirectory::Delete()
{
	return true;
}

bool NullDirectory::Move(const String& name)
{
	return true;
}

Directory* NullDirectory::GetParent() const
{
	return NULL;
}

const Array<String> NullDirectory::GetDirectories() const
{
	return Array<String>();
}

const Array<String> NullDirectory::GetFiles() const
{
	return Array<String>();
}

bool NullDirectory::Create()
{
	return true;
}

bool NullDirectory::CreateSubdirectory(const String& name)
{
	return true;
}

}
