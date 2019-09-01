/*=============================================================================
ResourceHelper.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ResourceHelper.h"
#include "Core/IO/File.h"
#include "Core/IO/FileStream.h"

namespace SonataEngine
{

Resource* ResourceHelper::LoadFromFile(const String& fileName, const SE_ID& type)
{
	Resource* resource;

	if (!File::Exists(fileName))
		return NULL;

	File* file = new File(fileName);
	if (file == NULL)
		return NULL;

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read, FileShare_Read);
	if (stream == NULL)
	{
		delete file;
		return NULL;
	}

	resource = ResourceManager::Instance()->Load(fileName, type, fileName, *stream);
	if (resource == NULL)
	{
		delete file;
		return NULL;
	}

	delete file;

	return resource;
}

bool ResourceHelper::SaveToFile(Resource* resource, const String& fileName)
{
	if (!File::Exists(fileName))
		return false;

	File* file = new File(fileName);
	if (file == NULL)
		return false;

	FileStreamPtr stream = file->Open(FileMode_Create, FileAccess_Write, FileShare_Write);
	if (stream == NULL)
	{
		delete file;
		return false;
	}

	if (!ResourceManager::Instance()->Save(resource, fileName, *stream))
	{
		delete file;
		return false;
	}

	delete file;

	return true;
}

}
