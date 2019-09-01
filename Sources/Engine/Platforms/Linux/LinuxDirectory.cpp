/*=============================================================================
LinuxDirectory.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/Directory.h"
#include "Core/IO/File.h"
#include "Core/IO/FileSystem.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace SonataEngine
{

typedef Directory LinuxDirectory;

LinuxDirectory::Directory()
{
}

LinuxDirectory::Directory(const String& name) :
	_name(name)
{
}

LinuxDirectory::~Directory()
{
}

bool LinuxDirectory::Exists() const
{
	if (_name.IsEmpty())
		return false;

    DIR *dirp;
	dirp = opendir(_name.Data());

	if (dirp != NULL)
		return false;

	closedir(dirp);
	return true;

}

bool LinuxDirectory::Delete()
{
	return rmdir(_name.Data()) == 0;
}

bool LinuxDirectory::Create()
{
	return mkdir(_name.Data(), S_IRWXU | S_IRGRP | S_IROTH) == 0;
}

bool LinuxDirectory::CreateSubdirectory(const String& name)
{
	String subpath = _name + name;
	return mkdir(subpath.Data(), S_IRWXU | S_IRGRP | S_IROTH) == 0;
}

bool LinuxDirectory::Move(const String& name)
{
	return false;
}

Directory* LinuxDirectory::GetParent() const
{
	return NULL;
}

const Array<String> LinuxDirectory::GetDirectories() const
{
	Array<String> directories;
	String filter; //TODO

	DIR *dirp;
	struct dirent *direntp;
	dirp = opendir(_name.Data());
	if (dirp == NULL)
		return dirs;

	while ((direntp = readdir(dirp)) != NULL)
	{
		if (fnmatch(filter.Data(), (const char*)direntp->d_name, 0))
			continue;

		directories.Add(new LinuxDirectory(_name + '/' + direntp->d_name));
	}

	closedir(dirp);

	return directories;
}

const Array<String> LinuxDirectory::GetFiles() const
{
	Array<String> files;
	String filter; //TODO

	DIR *dirp;
	struct dirent *direntp;
	dirp = opendir(_name.Data());
	if (dirp == NULL)
		return files;

	while ((direntp = readdir(dirp)) != NULL)
	{
		if (fnmatch(filter.Data(), (const char*)direntp->d_name, 0))
			continue;

		files.Add(new LinuxDirectory(_name + '/' + direntp->d_name));
	}

	closedir(dirp);

	return files;
}

}
