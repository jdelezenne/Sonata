/*=============================================================================
TypeInfo.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TypeInfo.h"
#include "Core/Object.h"
#include "Core/Reflection/Enum.h"
#include "Core/Reflection/FieldInfo.h"
#include "Core/Reflection/MethodInfo.h"

namespace SonataEngine
{

TypeInfo::TypeInfo(const String& typeName) :
	_name(typeName),
	_SuperType(NULL),
	_Creator(NULL)
{
	_Primitive = true;
	TypeFactory::Instance()->RegisterType(this);
}

TypeInfo::TypeInfo(const String& typeName, const String& superTypeName, ObjectCreator creator) :
	_name(typeName),
	_SuperTypeName(superTypeName),
	_Creator(creator)
{
	_SuperType = TypeFactory::Instance()->GetType((_SuperTypeName));
	_Primitive = false;
	TypeFactory::Instance()->RegisterType(this);
}

TypeInfo::~TypeInfo()
{
	//FIX: unregister while delete in TypeFactor dtor
	//TypeFactory::Instance()->UnregisterType(this);

	const FieldInfo* fi;
	foreach (fi, _Fields, FieldList)
	{
		delete fi;
	}

	_Fields.Clear();
}

Object* TypeInfo::Create() const
{
	return (_Creator != NULL ? (_Creator)() : NULL);
}

bool TypeInfo::IsPrimitive() const
{
	if (this == typeof(bool))
		return true;
	else if (this == typeof(int8) || this == typeof(uint8))
		return true;
	else if (this == typeof(int16) || this == typeof(uint16))
		return true;
	else if (this == typeof(int32) || this == typeof(uint32))
		return true;
	else if (this == typeof(int64) || this == typeof(uint64))
		return true;
	else if (this == typeof(real32))
		return true;
	else if (this == typeof(real64))
		return true;
	else if (this == typeof(String))
		return true;

	return false;
}

bool TypeInfo::IsClass() const
{
	return (IsSubclassOf(typeof(Object)) &&
		!IsPrimitive() && !IsStruct() && !IsEnum() && !IsArray());
}

bool TypeInfo::IsStruct() const
{
	return IsSubclassOf(typeof(StructObject));
}

bool TypeInfo::IsEnum() const
{
	return IsSubclassOf(typeof(EnumObject));
}

bool TypeInfo::IsArray() const
{
	//TODO
	return false;
}

bool TypeInfo::IsSubclassOf(TypeInfo* type) const
{
	SE_ASSERT(type);
	const TypeInfo* base = this;
	while (base != NULL)
	{
		base = base->GetBaseType();

		if (base == type)
			return true;
	}

	return false;
}

bool TypeInfo::IsInstanceOfType(Object* obj) const
{
	return (this == obj->GetType());
}

TypeInfo* TypeInfo::GetType(const String& typeName)
{
	return TypeFactory::Instance()->GetType(typeName);
}

MemberList TypeInfo::GetMember(const String& name) const
{
	MemberList members;
	MemberList allmembers = GetMembers();
	MemberInfo* mi;
	foreach (mi, allmembers, MemberList)
	{
		if (mi->GetName() == name)
			members.Add(mi);
	}

	return members;
}

MemberList TypeInfo::GetMember(const String& name, MemberTypes types) const
{
	MemberList members;
	MemberList allmembers = GetMembers();
	MemberInfo* mi;
	foreach (mi, allmembers, MemberList)
	{
		if (mi->GetName() == name && (mi->GetMemberType() & types) != 0)
			members.Add(mi);
	}

	return members;
}

MemberList TypeInfo::GetMembers() const
{
	MemberList members;

	// fields
	{
		FieldList fields = GetFields();
		FieldInfo* fi;
		foreach (fi, fields, FieldList)
		{
			members.Add(fi);
		}
	}

	// methods
	{
		MethodList methods = GetMethods();
		MemberInfo* mi;
		foreach (mi, methods, MethodList)
		{
			members.Add(mi);
		}
	}

	return members;
}

const FieldInfo* TypeInfo::GetField(const String& name) const
{
	FieldList fields = GetFields();
	const FieldInfo* fi;
	foreach (fi, fields, FieldList)
	{
		if (fi->GetName() == name)
			return fi;
	}

	return NULL;
}

FieldList TypeInfo::GetFields() const
{
	// Returns the members of this class and the members of its base classes
	FieldList fields = _Fields;
	TypeInfo* base = GetBaseType();
	// Gets the members from the base class, all the members from
	// all the base classes will be returned
	if (base != NULL)
	{
		FieldList baseFields = base->GetFields();
		FieldInfo* fi;
		foreach (fi, baseFields, FieldList)
		{
			fields.Add(fi);
		}
	}
	return fields;
}

const MethodInfo* TypeInfo::GetMethod(const String& name) const
{
	MethodList methods = GetMethods();
	MethodInfo* mi;
	foreach (mi, methods, MethodList)
	{
		if (mi->GetName() == name)
			return mi;
	}

	return NULL;
}

MethodList TypeInfo::GetMethods() const
{
	// Returns the members of this class and the members of its base classes
	MethodList methods = _Methods;
	TypeInfo* base = GetBaseType();
	// Gets the members from the base class, all the members from
	// all the base classes will be returned
	if (base != NULL)
	{
		MethodList baseMethods = base->GetMethods();
		MethodInfo* mi;
		foreach (mi, baseMethods, MethodList)
		{
			methods.Add(mi);
		}
	}
	return methods;
}

}
