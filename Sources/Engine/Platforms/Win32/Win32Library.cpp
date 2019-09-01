/*=============================================================================
Win32Library.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <windows.h>

#include "Core/System/Library.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class LibraryInternal
{
public:
	LibraryInternal();

public:
	String _name;
	HMODULE _handle;
};


LibraryInternal::LibraryInternal() :
	_handle(NULL)
{
}


Library::Library(const String& name) :
	_internal(new LibraryInternal())
{
	_internal->_name = name;
}

Library::~Library()
{
	//Unload();

	delete _internal;
}

String Library::GetName() const
{
	return _internal->_name;
}

bool Library::Load()
{
	if (_internal->_handle != NULL)
	{
		return false;
	}

	_internal->_handle = ::LoadLibrary(_internal->_name.Data());
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception(String::Concat("Failed loading the library: ", _internal->_name)));
		return false;
	}
	else
	{
		return true;
	}
}

bool Library::Unload()
{
	if (_internal->_handle == NULL)
	{
		return false;
	}

	return ::FreeLibrary(_internal->_handle) != 0;
}

bool Library::IsLoaded()
{
	return (_internal->_handle != NULL);
}

void* Library::GetSymbol(const String& name)
{
	if (_internal->_handle == NULL)
	{
		return NULL;
	}

	return ::GetProcAddress(_internal->_handle, name.Data());
}

void* Library::GetSymbol(const String& library, const String& name)
{
	return Library(library).GetSymbol(name);
}

}
