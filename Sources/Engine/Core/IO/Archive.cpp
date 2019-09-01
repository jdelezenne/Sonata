/*=============================================================================
Archive.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Archive.h"

namespace SonataEngine
{

Archive::Archive()
{
}

Archive::~Archive()
{
}

void Archive::SetName(const String& name)
{
	Close();

	_name = name;
}

bool Archive::ExtractFile(const String& name, const String& destination)
{
	// Opens the entry stream
	Stream* streamIn = OpenFile(name);
	if (streamIn == NULL)
	{
		return false;
	}

	// Check the length of the stream
	int32 length = streamIn->GetLength();
	if (length == 0)
	{
		SE_DELETE(streamIn);
		return false;
	}

	// Opens the destination stream
	FileStreamPtr streamOut = File(destination).Open(FileMode_OpenOrCreate, FileAccess_Write);
	if (streamOut == NULL)
	{
		SE_DELETE(streamIn);
		return false;
	}

	// Read the source data
	SEbyte* data = new SEbyte[length];
	if (streamIn->Read(data, length) != length)
	{
		SE_DELETE(streamIn);
		return false;
	}

	// Wrtie the destination data
	if (streamOut->Write(data, length) != length)
	{
		SE_DELETE_ARRAY(data);
		SE_DELETE(streamIn);
		return false;
	}

	SE_DELETE_ARRAY(data);
	SE_DELETE(streamIn);
	return true;
}

bool Archive::ExtractFile(int index, const String& destination)
{
	ArchiveEntry entry;
	if (!GetEntry(index, entry))
	{
		return false;
	}
	return ExtractFile(entry.Name, destination);
}

bool Archive::ExtractDirectory(const String& name, const String& destination, bool isRecursive)
{
	String basePath;
	String entryPath;
	String entryBaseName;
	String entryFileName;

	if (IsDirectory())
	{
		basePath = Path::Combine(destination, Path::GetFileNameWithoutExtension(_name));
		if (!Directory::Create(basePath))
		{
			return false;
		}
	}
	else
	{
		basePath = destination;
	}

	ArchiveEntryList entries;
	if (!GetEntries(entries))
	{
		return false;
	}

	ArchiveEntryList::Iterator it = entries.GetIterator();
	while (it.Next())
	{
		ArchiveEntry entry = it.Current();

		entryBaseName = Path::GetDirectoryName(entry.Name);
		if (entryBaseName == name)
		{
			entryFileName = Path::GetFileName(entry.Name);
			ExtractFile(entry.Name, Path::Combine(basePath, entryFileName));
		}
		else if (isRecursive)
		{
			/*fileName = entry.szName.Left(name.length() + 1);
			if (fileName == name + Path::GetPathSeparator())
			{
				if (name.IsEmpty())
					fileName = entry.szName;
				else
					fileName = entry.szName.Right(entry.szName.length() - name.length() - 1);

				fileName3 = strFinalDest;
				fileName2 = fileName.Left(fileName.find_first_of(Path::GetPathSeparator()));
				fileName3 += Path::GetPathSeparator() + fileName2;
				fileName = fileName.Right(fileName.length() - fileName.find_first_of(Path::GetPathSeparator()) - 1);
				_mkdir(fileName3);
				while (fileName.find_first_of(Path::GetPathSeparator()) != -1)
				{
					fileName2 = fileName.Left(fileName.find_first_of(Path::GetPathSeparator()));
					fileName3 += Path::GetPathSeparator() + fileName2;
					fileName = fileName.Right(fileName.length() - fileName.find_first_of(Path::GetPathSeparator()) - 1);
					_mkdir(fileName3);
				}

				ExtractFile(entry.szName, fileName3 + Path::GetPathSeparator() +	fileName);
			}*/
		}
	}

	return true;
}

}
