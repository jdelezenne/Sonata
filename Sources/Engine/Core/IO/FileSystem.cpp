/*=============================================================================
FileSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FileSystem.h"
#include "Core/Exception/ArgumentException.h"
#include "Core/IO/Path.h"
#include "Core/IO/File.h"

namespace SonataEngine
{

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

int FileSystem::GetRootPathCount() const
{
	return _rootPaths.Count();
}

FileSystem::RootPathList::Iterator FileSystem::GetRootPathIterator() const
{
	return _rootPaths.GetIterator();
}

bool FileSystem::AddRootPath(const String& value)
{
	if (value.IsEmpty())
	{
		SEthrow(ArgumentException("value"));
		return false;
	}
	else
	{
		_rootPaths.Add(value);
		return true;
	}
}

bool FileSystem::InsertRootPath(int index, const String& value)
{
	if (value.IsEmpty())
	{
		SEthrow(ArgumentException("value"));
		return false;
	}
	else
	{
		_rootPaths.Insert(index, value);
		return true;
	}
}

bool FileSystem::ReplaceRootPath(const String& previous, const String& value)
{
	if (previous.IsEmpty() || value.IsEmpty())
	{
		SEthrow(ArgumentException("value"));
		return false;
	}
	else
	{
		int index = _rootPaths.IndexOf(previous);
		if (index == -1)
		{
			SEthrow(ArgumentException("value"));
			return false;
		}
		else
		{
			_rootPaths.SetItem(index, value);
			return true;
		}
	}
}

bool FileSystem::RemoveRootPath(const String& value)
{
	if (value.IsEmpty())
	{
		SEthrow(ArgumentException("value"));
		return false;
	}
	else
	{
		_rootPaths.Remove(value);
		return true;
	}
}

void FileSystem::RemoveAllRootPathes()
{
	_rootPaths.Clear();
}

int FileSystem::GetRootPathIndex(const String& value) const
{
	if (value.IsEmpty())
	{
		SEthrow(ArgumentException("value"));
		return -1;
	}
	else
	{
		return _rootPaths.IndexOf(value);
	}
}

String FileSystem::GetRootPath(int index) const
{
	return _rootPaths[index];
}

bool FileSystem::SetRootPath(int index, const String& value)
{
	if (value.IsEmpty())
	{
		SEthrow(ArgumentException("value"));
		return false;
	}
	else
	{
		_rootPaths.SetItem(index, value);
		return true;
	}
}

String FileSystem::GetFullPath(const String& path, bool check)
{
	if (!Path::IsAbsolutePath(path))
	{
		String fullPath;

		// always use current directory first
		fullPath = Path::Combine(Environment::GetCurrentDirectory(), path);
		if (!check || (check && File::Exists(fullPath)))
		{
			return fullPath;
		}
		else if (GetRootPathCount() != 0)
		{
			Array<String>::Iterator it = _rootPaths.GetIterator();
			while (it.Next())
			{
				fullPath = Path::Combine(it.Current(), path);
				if (!check || (check && File::Exists(fullPath)))
				{
					return fullPath;
				}
			}
		}
	}

	// absolute path or no root paths mounted
	return path;
}

}
