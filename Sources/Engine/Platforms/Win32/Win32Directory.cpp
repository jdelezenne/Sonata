/*=============================================================================
Win32Directory.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/IO/Directory.h"
#include "Core/IO/File.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/Path.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class DirectoryInternal
{
public:
	DirectoryInternal();

public:
	String _name;
};


DirectoryInternal::DirectoryInternal()
{
}


Directory::Directory() :
	_internal(new DirectoryInternal())
{
}

Directory::Directory(const String& name) :
	_internal(new DirectoryInternal())
{
	_internal->_name = name;
}

Directory::~Directory()
{
	delete _internal;
}

bool Directory::Exists() const
{
	if (_internal->_name.IsEmpty())
	{
		return false;
	}

	bool exist;
	HANDLE hFind;
	WIN32_FIND_DATA findData;

	hFind = ::FindFirstFile(_internal->_name.Data(), &findData);
	exist = (hFind != INVALID_HANDLE_VALUE && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
	::FindClose(hFind);

	return exist;
}

const String& Directory::GetName() const
{
	return _internal->_name;
}

void Directory::SetName(const String& name)
{
	_internal->_name = name;
}

Directory Directory::GetParent() const
{
	String parentName = Path::GetDirectoryName(_internal->_name);
	return Directory(parentName);
}

bool Directory::Delete()
{
	return ::RemoveDirectory(_internal->_name.Data()) != 0;
}

bool Directory::Create()
{
	return ::CreateDirectory(_internal->_name.Data(), NULL) != 0;
}

bool Directory::Move(const String& path)
{
	return ::MoveFile(_internal->_name.Data(), path.Data()) != 0;
}

const Array<String> Directory::GetDirectories() const
{
	Array<String> directories;

	HANDLE hFind;
	WIN32_FIND_DATA findData;

	hFind = ::FindFirstFile(_internal->_name.Data(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return Array<String>();

	while (true)
	{
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			if (String::Compare(findData.cFileName, _T(".")) != 0 &&
				String::Compare(findData.cFileName, _T("..")) != 0)
			{
				directories.Add(findData.cFileName);
			}
		}

		if (!::FindNextFile(hFind, &findData))
			break;
	}

	::FindClose(hFind);

	return directories;
}

const Array<String> Directory::GetFiles() const
{
	Array<String> files;

	HANDLE hFind;
	WIN32_FIND_DATA findData;

	hFind = ::FindFirstFile(_internal->_name.Data(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return Array<String>();

	while (true)
	{
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			files.Add(findData.cFileName);

		if (!::FindNextFile(hFind, &findData))
			break;
	}

	::FindClose(hFind);

	return files;
}

bool Directory::CreateSubdirectory(const String& path)
{
	return (::CreateDirectory(Path::Combine(_internal->_name, path).Data(), NULL) != 0);
}

}
