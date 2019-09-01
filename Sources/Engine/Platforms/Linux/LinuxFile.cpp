/*=============================================================================
LinuxFile.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/File.h"
#include "Core/IO/FileStream.h"
#include "Core/IO/FileSystem.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace SonataEngine
{

typedef File LinuxFile;

LinuxFile::File()
{
	Init();
}

LinuxFile::File(const String& name) :
	_name(name)
{
	Init();
}

LinuxFile::~File()
{
	Close();
}

void LinuxFile::Init()
{
	_fileAccess = FA_Read;
	_fileShare = FS_None;
	_fileMode = FM_Open;
	_handle = NULL;
}

bool LinuxFile::Exists() const
{
	if (_name.IsEmpty())
		return false;

	if (IsOpen())
		return true;

	FILE *fp;

	fp = fopen(_name.Data(), "rb");
	if (fp == NULL)
		return false;

	fclose(fp);
	return true;
}

bool LinuxFile::Delete()
{
	return unlink(_name.Data()) == 0;
}

bool LinuxFile::Copy(const String& fileName, bool overwrite)
{
	return false;
}

bool LinuxFile::Move(const String& fileName)
{
	return false;
}

WindowHandle LinuxFile::GetHandle() const
{
	return _handle;
}

uint32 LinuxFile::GetLength() const
{
	FILE *fp;

	fp = fopen(_name.Data(), "rb");
	if (fp == NULL)
		return 0;

	int pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, pos, SEEK_SET);

	fclose(fp);

	return len;
}

bool LinuxFile::IsOpen() const
{
	return _handle != NULL;
}

SmartPtr<FileStream> LinuxFile::Create()
{
	FileSystem* fileSystem = FileSystem::Instance();

	String fileName;
	if (!Path::IsAbsolutePath(GetName()))
	{
		if (fileSystem->GetRootPathCount() == 0)
		{
			//EXCEPT(ExceptionType_Abort, "No Root Paths Defined", "LinuxFile::Open");
			return NULL;
		}
		fileName = fileSystem->GetRootPath(0) + Path::GetPathSeparator() + GetName();
	}
	else
	{
		fileName = GetName();
	}

	if (IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is already opened", "LinuxFile::Open");
		return NULL;
	}

	if (_name.IsEmpty())
	{
		//EXCEPT(ExceptionType_Abort, "No filename specified", "LinuxFile::Open");
		return NULL;
	}

	_handle = fopen(fileName.Data(), "w");

	if (_handle == NULL)
	{
		//EXCEPT(ExceptionType_Fail, "Can't open the file", "LinuxFile::Open");
		return NULL;
	}

	//Creates a file stream.
	FileStream* stream = new FileStream(this);

	return stream;
}

SmartPtr<FileStream> LinuxFile::Open(FileMode FileMode, FileAccess FileAccess, FileShare FileShare)
{
	FileSystem* fileSystem = FileSystem::Instance();

	String fileName;
	if (!Path::IsAbsolutePath(GetName()))
	{
		if (fileSystem->GetRootPathCount() == 0)
		{
			//EXCEPT(ExceptionType_Abort, "No Root Paths Defined", "LinuxFile::Open");
			return NULL;
		}
		fileName = fileSystem->GetRootPath(0) + Path::GetPathSeparator() + GetName();
	}
	else
	{
		fileName = GetName();
	}

	if (IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is already opened", "LinuxFile::Open");
		return NULL;
	}

	if (_name.IsEmpty())
	{
		//EXCEPT(ExceptionType_Abort, "No filename specified", "LinuxFile::Open");
		return NULL;
	}

	_fileMode = FileMode;
	_fileAccess = FileAccess;
	_fileShare = FileShare;

	String mode;

	switch (_fileAccess)
	{
	case FA_Read:
		mode = "r";
		break;
	case FA_ReadWrite:
		mode = "r+";
		break;
	case FA_Write:
		mode = "w";
		break;
	}

	if (_fileMode == FM_Append)
	{
		mode = (_fileAccess == FA_Write) ? "a" : "a+";
	}
	else
	{
		if (_fileAccess == FA_ReadWrite)
			mode = (_fileMode == FM_Truncate) ? "w+" : "r+";
		else if (_fileAccess == FA_Read)
			mode = "r";
		else if (_fileAccess == FA_Write)
			mode = "w";
	}

	mode += "b";

	_handle = fopen(fileName.Data(), mode.Data());

	if (_handle == INVALID_HANDLE_VALUE)
	{
		_handle = NULL;
		//EXCEPT(ExceptionType_Fail, "Can't open the file", "LinuxFile::Open");
		return NULL;
	}

	//Creates a file stream.
	FileStream* stream = new FileStream(this);

	return stream;
}

bool LinuxFile::Close()
{
	if (IsOpen())
	{
		if (fclose(_handle) != 0)
		{
			//EXCEPT(ExceptionType_Fail, "Can't close the file", "LinuxFile::Close");
			return false;
		}

		Init();
		return true;
	}

	return false;
}

}