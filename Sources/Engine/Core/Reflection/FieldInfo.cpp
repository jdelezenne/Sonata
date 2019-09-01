/*=============================================================================
FieldInfo.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FieldInfo.h"
#include "Core/Object.h"

namespace SonataEngine
{

FieldInfo::FieldInfo() :
	_FieldType(NULL),
	_FieldAttributes(FieldAttributes_Private),
	_Offset(0)
{
}

FieldInfo::FieldInfo(const String& fieldName, const String& fieldTypeName, FieldAttributes attributes, int offset) :
	_FieldName(fieldName),
	_FieldTypeName(fieldTypeName),
	_FieldAttributes(attributes),
	_Offset(offset)
{
	_FieldType = TypeFactory::Instance()->GetType((_FieldTypeName));
}

FieldInfo::~FieldInfo()
{
}

Variant FieldInfo::GetValue(Object* obj) const
{
	if ((_FieldAttributes & FieldAttributes_Static) != 0)
	{
		return _Value;
	}

	void* ptr = (void*)((((const SEbyte*)obj) + _Offset));

	if (_FieldType == typeof(bool))
		return Variant(*(bool*)ptr);
	if (_FieldType == typeof(int8))
		return Variant(*(int8*)ptr);
	if (_FieldType == typeof(uint8))
		return Variant(*(uint8*)ptr);
	if (_FieldType == typeof(int16))
		return Variant(*(int16*)ptr);
	if (_FieldType == typeof(uint16))
		return Variant(*(uint16*)ptr);
	if (_FieldType == typeof(int32))
		return Variant(*(int32*)ptr);
	if (_FieldType == typeof(uint32))
		return Variant(*(uint32*)ptr);
	if (_FieldType == typeof(int64))
		return Variant(*(int64*)ptr);
	if (_FieldType == typeof(uint64))
		return Variant(*(uint64*)ptr);
	if (_FieldType == typeof(real32))
		return Variant(*(real32*)ptr);
	if (_FieldType == typeof(real64))
		return Variant(*(real64*)ptr);
	if (_FieldType == typeof(String))
		return Variant(*(String*)ptr);
	if (_FieldType->IsStruct())
		return Variant((StructObject*)ptr);
	if (_FieldType->IsClass())
	{
		Object** ppObj = (Object**)ptr;
		if (ppObj != NULL)
			return Variant((Object*)*ppObj);
		else
			return Variant((Object*)NULL);
	}
	if (_FieldType->IsEnum())
		return Variant((void*)*(int32*)ptr, Variant_Enum);

	return Variant::Invalid;
}

void FieldInfo::SetValue(Object* obj, const Variant& value) const
{
	if (value.GetType() == Variant_Invalid)
		return;

	if ((_FieldAttributes & FieldAttributes_Static) != 0)
	{
		_Value = value;
	}

	if (_FieldType == typeof(bool))
		*(bool*)(((const SEbyte*)obj) + _Offset) = value.ToBoolean();
	else if (_FieldType == typeof(int8))
		*(int8*)(((const SEbyte*)obj) + _Offset) = value.ToInt8();
	else if (_FieldType == typeof(uint8))
		*(uint8*)(((const SEbyte*)obj) + _Offset) = value.ToUInt8();
	else if (_FieldType == typeof(int16))
		*(int16*)(((const SEbyte*)obj) + _Offset) = value.ToInt16();
	else if (_FieldType == typeof(uint16))
		*(uint16*)(((const SEbyte*)obj) + _Offset) = value.ToUInt16();
	else if (_FieldType == typeof(int32))
		*(int32*)(((const SEbyte*)obj) + _Offset) = value.ToInt32();
	else if (_FieldType == typeof(uint32))
		*(uint32*)(((const SEbyte*)obj) + _Offset) = value.ToUInt32();
	else if (_FieldType == typeof(int64))
		*(int64*)(((const SEbyte*)obj) + _Offset) = value.ToInt64();
	else if (_FieldType == typeof(uint64))
		*(uint64*)(((const SEbyte*)obj) + _Offset) = value.ToUInt64();
	else if (_FieldType == typeof(real32))
		*(real32*)(((const SEbyte*)obj) + _Offset) = value.ToReal32();
	else if (_FieldType == typeof(real64))
		*(real64*)(((const SEbyte*)obj) + _Offset) = value.ToReal64();
	else if (_FieldType == typeof(String))
		*(String*)(((const SEbyte*)obj) + _Offset) = value.ToString();
	else if (_FieldType->IsStruct())
	{
		void* ptr = (void*)(((const SEbyte*)obj) + _Offset);
		(value.ToStruct())->Get(ptr);
	}
	else if (_FieldType->IsClass())
	{
		void* ptr = (void*)(((const SEbyte*)obj) + _Offset);
		Object* pObj = value.ToObject();
		Memory::Copy(ptr, &pObj, sizeof(Object*));
	}
	else if (_FieldType->IsEnum())
		*(int32*)(((const SEbyte*)obj) + _Offset) = value.ToInt32();
}

}
