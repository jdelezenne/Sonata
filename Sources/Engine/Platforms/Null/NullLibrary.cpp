/*=============================================================================
NullLibrary.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/System/Library.h"

namespace SonataEngine
{

typedef Library NullLibrary;

NullLibrary::Library(const String& name) :
	_name(name)
{
	_handle = NULL;
}

NullLibrary::~Library()
{
	Unload();
}

String NullLibrary::GetName() const
{
	return _name;
}

LibraryHandle NullLibrary::GetHandle() const
{
	return _handle;
}

bool NullLibrary::Load()
{
	return false;
}

bool NullLibrary::Unload()
{
	return false;
}

bool NullLibrary::IsLoaded()
{
	return false;
}

void* NullLibrary::GetSymbol(const String& name)
{
	return NULL;
}

void* NullLibrary::GetSymbol(const String& library, const String& name)
{
	return NULL;
}

}
