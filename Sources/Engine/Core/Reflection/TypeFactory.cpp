/*=============================================================================
TypeFactory.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TypeFactory.h"
#include "Core/Object.h"

namespace SonataEngine
{

TypeFactory::~TypeFactory()
{
	TypeTable::DictionaryIterator it = _types.GetIterator();
	while (it.Next())
	{
		//FIX
		//delete it.Value();
	}

	_types.Clear();
}

void TypeFactory::RegisterType(TypeInfo* type)
{
	SE_ASSERT(type);
	if (!_types.Contains(type->GetName()))
	{
		_types[type->GetName()] = type;
	}
}

void TypeFactory::UnregisterType(TypeInfo* type)
{
	SE_ASSERT(type);
	_types.Remove(type->GetName());
}

TypeInfo* TypeFactory::GetType(const String& typeName)
{
	if (_types.Contains(typeName))
	{
		return _types[typeName];
	}
	else
	{
		return NULL;
	}
}

TypeInfoList TypeFactory::GetTypes()
{
	return TypeInfoList(_types.Values());
}

Object* TypeFactory::CreateInstance(TypeInfo* type)
{
	SE_ASSERT(type);
	return type->Create();
}

Object* TypeFactory::CreateInstance(const String& typeName)
{
	if (_types.ContainsKey(typeName))
		return _types[typeName]->Create();
	else
		return NULL;
}

}
