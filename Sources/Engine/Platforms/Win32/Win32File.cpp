/*=============================================================================
Win32File.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/IO/File.h"
#include "Core/IO/FileStream.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/Path.h"
#include "Core/System/Environment.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class FileInternal
{
public:
	FileInternal();

public:
	String _name;
	FileAccess _fileAccess;
	FileShare _fileShare;
	FileMode _fileMode;
	HANDLE _handle;
};


FileInternal::FileInternal() :
	_fileAccess(FileAccess_Read),
	_fileShare(FileShare_None),
	_fileMode(FileMode_Open),
	_handle(NULL)
{
}


File::File() :
	_internal(new FileInternal())
{
}

File::File(const String& name) :
	_internal(new FileInternal())
{
	_internal->_name = name;
}

File::~File()
{
	Close();

	delete _internal;
}

const String& File::GetName() const
{
	return _internal->_name;
}

void File::SetName(const String& name)
{
	if (_internal->_handle != NULL)
	{
		Close();
	}

	_internal->_name = name;
}

FileAccess File::GetFileAccess() const
{
	return _internal->_fileAccess;
}

FileShare File::GetFileShare() const
{
	return _internal->_fileShare;
}

FileMode File::GetFileMode() const
{
	return _internal->_fileMode;
}

FileHandle File::GetHandle() const
{
	return (FileHandle)_internal->_handle;
}

int32 File::GetLength() const
{
	int32 nLength = ::GetFileSize(_internal->_handle, NULL);
	if (nLength == INVALID_FILE_SIZE)
	{
		SEthrow(Exception("Failed retrieving the file size."));
		return 0;
	}

	return nLength;
}

bool File::IsOpen() const
{
	return (_internal->_handle != NULL);
}

bool File::Exists() const
{
	bool exist;
	HANDLE hFind;
	WIN32_FIND_DATA findData;

	hFind = ::FindFirstFile(_internal->_name.Data(), &findData);
	exist = (hFind != INVALID_HANDLE_VALUE && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
	::FindClose(hFind);

	return exist;
}

bool File::Delete()
{
	return (::DeleteFile(_internal->_name.Data()) != 0);
}

bool File::Copy(const String& name, bool overwrite)
{
	return (::CopyFile(_internal->_name.Data(), name.Data(), !overwrite) != 0);
}

bool File::Move(const String& name)
{
	return (::MoveFile(_internal->_name.Data(), name.Data()) != 0);
}

FileStreamPtr File::Create()
{
	if (_internal->_handle != NULL)
	{
		return NULL;
	}

	if (_internal->_name.IsEmpty())
	{
		SEthrow(Exception("The file name is empty."));
		return NULL;
	}

	String fileName = _internal->_name;

	_internal->_handle = ::CreateFile(
		fileName.Data(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (_internal->_handle == INVALID_HANDLE_VALUE)
	{
		_internal->_handle = NULL;
		SEthrow(Exception("Failed opening the file."));
		return NULL;
	}

	// Creates a file stream.
	FileStream* stream = new FileStream(this);
	return stream;
}

FileStreamPtr File::Open(FileMode fileMode, FileAccess fileAccess, FileShare fileShare)
{
	if (_internal->_handle != NULL)
	{
		return NULL;
	}

	if (_internal->_name.IsEmpty())
	{
		SEthrow(Exception("The file name is empty."));
		return NULL;
	}

	String fileName = _internal->_name;

	_internal->_fileMode = fileMode;
	_internal->_fileAccess = fileAccess;
	_internal->_fileShare = fileShare;

	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = 0;
	DWORD dwCreationDisposition = 0;

	switch (_internal->_fileAccess)
	{
	case FileAccess_Read:
		dwDesiredAccess = GENERIC_READ;
		break;
	case FileAccess_ReadWrite:
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	case FileAccess_Write:
		dwDesiredAccess = GENERIC_WRITE;
		break;
	}

	switch (_internal->_fileShare)
	{
	case FileShare_None:
		break;
	case FileShare_Read:
		dwShareMode = FILE_SHARE_READ;
		break;
	case FileShare_ReadWrite:
		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		break;
	case FileShare_Write:
		dwShareMode = FILE_SHARE_WRITE;
		break;
	}

	switch (_internal->_fileMode)
	{
	case FileMode_Append:
		dwCreationDisposition = OPEN_ALWAYS;
		break;
	case FileMode_Create:
		dwCreationDisposition = CREATE_ALWAYS;
		break;
	case FileMode_CreateNew:
		dwCreationDisposition = CREATE_NEW;
		break;
	case FileMode_Open:
		dwCreationDisposition = OPEN_EXISTING;
		break;
	case FileMode_OpenOrCreate:
		dwCreationDisposition = CREATE_NEW;
		break;
	case FileMode_Truncate:
		dwCreationDisposition = TRUNCATE_EXISTING;
		break;
	}

	_internal->_handle = ::CreateFile(
		fileName.Data(),
		dwDesiredAccess,
		dwShareMode,
		NULL,
		dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (_internal->_handle == INVALID_HANDLE_VALUE)
	{
		_internal->_handle = NULL;
		SEthrow(Exception("Failed opening the file."));
		return NULL;
	}

	// Creates a file stream.
	FileStream* stream = new FileStream(this);
	return stream;
}

bool File::Close()
{
	if (_internal->_handle == NULL)
	{
		return false;
	}

	if (::CloseHandle(_internal->_handle) == 0)
	{
		SEthrow(Exception("Failed closing the file."));
		return false;
	}

	_internal->_handle = NULL;

	return true;
}

}
