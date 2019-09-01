/*=============================================================================
Enum.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Enum.h"
#include "Core/Object.h"

namespace SonataEngine
{

SE_IMPLEMENT_ABSTRACT(EnumObject);

EnumObject::EnumObject() :
	Object()
{
}

EnumObject::~EnumObject()
{
}

Array<String> EnumObject::GetNames(TypeInfo* enumType)
{
	if (enumType == NULL || !enumType->IsEnum())
		return Array<String>();

	Array<String> names;
	const FieldList& fields = enumType->GetFields();
	const FieldInfo* fi;
	foreach (fi, fields, FieldList)
	{
		names.Add(fi->GetName());
	}

	return names;
}

Array<Variant> EnumObject::GetValues(TypeInfo* enumType)
{
	if (enumType == NULL || !enumType->IsEnum())
		return Array<Variant>();

	Array<Variant> values;
	const FieldList& fields = enumType->GetFields();
	const FieldInfo* fi;
	foreach (fi, fields, FieldList)
	{
		values.Add(fi->GetValue(NULL));
	}

	return values;
}

String EnumObject::GetName(TypeInfo* enumType, const Variant& value)
{
	if (enumType == NULL || !enumType->IsEnum())
		return String::Empty;

	const FieldList& fields = enumType->GetFields();
	const FieldInfo* fi;
	foreach (fi, fields, FieldList)
	{
		if (value == fi->GetValue(NULL))
			return fi->GetName();
	}

	return String::Empty;
}

Variant EnumObject::GetValue(TypeInfo* enumType, const String& name)
{
	if (enumType == NULL || !enumType->IsEnum())
		return Variant::Invalid;

	const FieldList& fields = enumType->GetFields();
	const FieldInfo* fi;
	foreach (fi, fields, FieldList)
	{
		if (name == fi->GetName())
			return fi->GetValue(NULL);
	}

	return Variant::Invalid;
}


EnumBuilder::EnumBuilder(const String& name) :
	_Type(),
	_Index(0)
{
	_Type = (TypeInfo*)TypeFactory::Instance()->GetType(name);
	if (_Type == NULL)
	{
		_Type = new TypeInfo(name);
		_Type->_SuperTypeName = "EnumObject";
	}
}

FieldInfo* EnumBuilder::DefineEnum(const String& enumName, const Variant& enumValue)
{
	FieldInfo* fi = new FieldInfo(enumName, "int32",
		(FieldAttributes)(FieldAttributes_Public | FieldAttributes_Static), 0);
	fi->_Value = enumValue;
	fi->_DeclaringType = _Type;
	_Type->_Fields.Add(fi);
	_Index++;

	return fi;
}

FieldInfo* EnumBuilder::DefineEnum(const String& enumName)
{
	return DefineEnum(enumName, Variant(_Index));
}

}
