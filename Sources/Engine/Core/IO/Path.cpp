/*=============================================================================
Path.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Path.h"
#include "Core/System/Environment.h"

namespace SonataEngine
{

String Path::ConvertSeparators(const String& path, bool toSlash)
{
	String s = path;
	int length = path.Length();

	if (toSlash)
	{
		for (int i = 0; i < length; ++i)
		{
			if (s[i] == _T('\\'))
				s[i] = _T('/');
		}
	}
	else
	{
		for (int i = 0; i < length; ++i)
		{
			if (s[i] == _T('/'))
				s[i] = _T('\\');
		}
	}

	return s;
}

String Path::GetFileName(const String& path)
{
	int p = path.LastIndexOf(Environment::GetPathSeparator());
	if (p == -1)
		return path;
	else
		return path.Substring(p+1);
}

String Path::GetDirectoryName(const String& path, bool absPath)
{
	String s;

	if (absPath)
		s = GetFullPath(path);
	else
		s = path;

	int pos = s.LastIndexOf(Environment::GetPathSeparator());
	if (pos == -1)
		return ".";
	else
		return s.Left(pos);
}

String Path::GetFullPath(const String& path)
{
	if (Path::IsRelativePath(path))
	{
		String s = Environment::GetCurrentDirectory();
		s += Environment::GetPathSeparator();
		s += path;
		return s;
	}
	else
		return path;
}

String Path::GetPathRoot(const String& path)
{
	int length = Environment::GetRootDirectory().Length();
	if (path.Length() < length)
		return String::Empty;

	return path.Left(length);
}

String Path::GetExtension(const String& path, bool complete)
{
	String s(GetFileName(path));
	int pos = complete ? s.IndexOf('.') : s.LastIndexOf('.');
	if (pos < 0)
		return String();
	else
		return s.Right(s.Length() - pos - 1);
}

String Path::GetFileNameWithoutExtension(const String& path, bool complete)
{
	String tmp(GetFileName(path));
	int pos = complete ? tmp.IndexOf('.') : tmp.LastIndexOf('.');
	if (pos == -1)
		return tmp;
	else
		return tmp.Left(pos);
}

String Path::ChangeExtension(const String& path, const String& ext, bool complete)
{
	String s(GetFileName(path));
	int pos = complete ? s.IndexOf('.') : s.LastIndexOf('.');
	if (pos < 0)
		return String(path);
	else
		return s.Left(pos + 1) + ext;
}

String Path::Combine(const String& left, const String& right)
{
	if (left.IsEmpty())
		return right;
	if (right.IsEmpty())
		return left;

	// '/' at the end of left
	if (left[left.Length()-1] == Environment::GetPathSeparator())
	{
		// '/' at the strart of right, combine the rest of right
		if (right[0] == Environment::GetPathSeparator())
		{
			return (left + right.Right(right.Length()-1));
		}
		// no '/' at the strart of right, combine the strings
		else
		{
			return (left + right);
		}
	}
	// no '/' at the end of left
	else
	{
		// '/' at the start of right, combine the strings
		if (right[0] == Environment::GetPathSeparator())
		{
			return (left + right);
		}
		else
		{
			// no '/' in neither strings, combine the strings with a separator
			return (left + Environment::GetPathSeparator() + right);
		}
	}
}

bool Path::HasExtension(const String& path)
{
	return !Path::GetExtension(path).IsEmpty();
}

bool Path::IsAbsolutePath(const String& path)
{
	if (path.IsEmpty())
		return false;

	return Environment::IsAbsolutePath(path);
}

bool Path::IsRelativePath(const String& path)
{
	if (path.IsEmpty())
		return false;

	return !Environment::IsAbsolutePath(path);
}

}
