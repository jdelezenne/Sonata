/*=============================================================================
Win32MappedFileStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/IO/FileStream.h"
#include "Core/IO/File.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

typedef FileStream Win32MappedFileStream;

Win32MappedFileStream::FileStream() :
	Stream()
{
	Init();
}

Win32MappedFileStream::FileStream(File* file) :
	Stream()
{
	if (file == NULL)
	{
		throw new ArgumentNullException("file");
		return;
	}

	Init();

	_File = file;
	_SourceName = file->GetName();

	HANDLE hMapFile;
	LPCTSTR pBuffer;

	HANDLE hFile = (HANDLE)_File->GetHandle();
	hMapFile = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0,
		_BufferSize, _SourceName);
	if (hMapFile == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Win32MappedFileStream (CreateFileMapping)"),
			_T("Failed to create the file mapping object."));
		return;
	}

	pBuffer = (LPTSTR)::MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS,
		0, 0, _BufferSize);           
 
	if (pBuffer == NULL) 
	{ 
		Logger::Current()->Log(LogLevel::Error, _T("Win32MappedFileStream (MapViewOfFile)"),
			_T("Failed to map a view  of the file."));
		return;
	}
}

Win32MappedFileStream::~FileStream()
{
	::UnmapViewOfFile(pBuffer);
}

void Win32MappedFileStream::Init()
{
	_BufferSize = SE_FILESTREAM_BUFFERSIZE;
	_File = NULL;
}

int32 Win32MappedFileStream::GetLength() const
{
	if (_File == NULL)
		return -1;

	return GetFileSize(_File->GetHandle(), NULL);
}

void Win32MappedFileStream::SetLength(int32 value)
{
}

int32 Win32MappedFileStream::GetPosition() const
{
	SE_ASSERT(_File);
	return ::SetFilePointer(_File->GetHandle(), 0, NULL, FILE_CURRENT);
}

void Win32MappedFileStream::SetPosition(int32 value)
{
	Seek(value, SeekOrigin_Begin);
}

void Win32MappedFileStream::Close()
{
	if (_File == NULL || !_File->IsOpen())
		return;

	_File->Close();
	Init();
}

void Win32MappedFileStream::Flush()
{
	if (_File == NULL || !_File->IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is not opened", "Win32MappedFileStream::Flush");
		return;
	}

	if (_File->GetHandle())
	{
		if (!::FlushFileBuffers(_File->GetHandle()))
		{
			//EXCEPT(ExceptionType_Abort, "Can't flush the file", "Win32MappedFileStream::Flush");
			return;
		}
	}
}

int32 Win32MappedFileStream::Seek(int32 offset, SeekOrigin origin)
{
	if (_File == NULL || !_File->IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is not opened", "Win32MappedFileStream::Seek");
		return 0;
	}

	if (!CanSeek())
		return GetPosition();

	// Origin
	DWORD dwMoveMethod = FILE_CURRENT;
	if (origin == SeekOrigin_Begin)
		dwMoveMethod = FILE_BEGIN;
	else if (origin == SeekOrigin_End)
		dwMoveMethod = FILE_END;

	if (!::SetFilePointer(_File->GetHandle(), offset, NULL, dwMoveMethod))
	{
		//EXCEPT(ExceptionType_Fail, "Cannot set new file position", "Win32MappedFileStream::Seek");
		return 0;
	}

	return GetPosition();
}

bool Win32MappedFileStream::IsEOF() const
{
	return GetPosition() == GetLength();
}

byte Win32MappedFileStream::ReadByte()
{
	if (_File == NULL || !_File->IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is not opened", "Win32MappedFileStream::ReadByte");
		return EOF;
	}

	if (!CanRead())
	{
		//EXCEPT(ExceptionType_Abort, "Read operation not permitted", "Win32MappedFileStream::ReadByte");
		return EOF;
	}

	byte value;
	return Read(&value, 1) == EOF ? EOF : value;
}

int32 Win32MappedFileStream::Read(byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		//EXCEPT(ExceptionType_Pointer, "Null pointer error", "Win32MappedFileStream::Read");
		return EOF;
	}

	if (_File == NULL || !_File->IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is not opened", "Win32MappedFileStream::Read");
		return EOF;
	}

	if (!CanRead())
	{
		//EXCEPT(ExceptionType_Abort, "Read operation not permitted", "Win32MappedFileStream::Read");
		return EOF;
	}

	DWORD dwNumberOfBytesRead;
	if (::ReadFile(_File->GetHandle(), (LPVOID) buffer, count, &dwNumberOfBytesRead, NULL) == FALSE)
	{
		//EXCEPT(ExceptionType_Fail, "Read error", "Win32MappedFileStream::Read");
		return EOF;
	}

	return dwNumberOfBytesRead;
}

void Win32MappedFileStream::WriteByte(byte value)
{
	if (_File == NULL || !_File->IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is not opened", "Win32MappedFileStream::WriteByte");
		return;
	}

	if (!CanWrite())
	{
		//EXCEPT(ExceptionType_Abort, "Write operation not permitted", "Win32MappedFileStream::WriteByte");
		return;
	}

	Write(&value, 1);
}

int32 Win32MappedFileStream::Write(const byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		//EXCEPT(ExceptionType_Pointer, "Null pointer error", "Win32MappedFileStream::Write");
		return EOF;
	}

	if (_File == NULL || !_File->IsOpen())
	{
		//EXCEPT(ExceptionType_Abort, "File is not opened", "Win32MappedFileStream::Write");
		return EOF;
	}

	if (!CanWrite())
	{
		//EXCEPT(ExceptionType_Abort, "Write operation not permitted", "Win32MappedFileStream::Write");
		return EOF;
	}

	DWORD dwNumberOfBytesWritten;
	if (::WriteFile(_File->GetHandle(), buffer, count, &dwNumberOfBytesWritten, NULL) == FALSE)
	{
		//EXCEPT(ExceptionType_Fail, "Write error", "Win32MappedFileStream::Write");
		return EOF;
	}

	return dwNumberOfBytesWritten;
}

String Win32MappedFileStream::GetName() const
{
	if (_File == NULL)
		return String::Empty;

	return _File->GetName();
}

}
