/*=============================================================================
BinarySerializer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BinarySerializer.h"
#include "Core/IO/BinaryStream.h"
#include "Core/Logging/Logger.h"

namespace SonataEngine
{

template <typename T>
class VariantHolder
{
public:
	static T GetValue(const Variant& value)
	{
		switch (value.GetType())
		{
		case Variant_Boolean:
			return value.ToBoolean());
		case Variant_Int32:
			return value.ToInt32();
		case Variant_UInt32:
			return value.ToUInt32();
		case Variant_Int64:
			return value.ToInt64();
		case Variant_UInt64:
			return value.ToUInt64();
		case Variant_Real64:
			return value.ToReal64();
		case Variant_String:
			return value.ToString();
		}
	}
};

void BinarySerializer::Serialize(BinaryStream& writer, Object* obj, TypeInfo* type)
{
	if (type->IsAbstract())
		return;

	writer.WriteString(type->GetName());

	int8 refindex = (obj == NULL ? 0 : 1);
	writer.WriteInt8(refindex);

	if (obj == NULL)
		return;

	Array<MemberInfo*> members;
	members = ISerializer::GetSerializableMembers(obj->GetType());

	BaseArray<Variant> values;
	values = ISerializer::GetObjectValues(obj, members);

	int32 index = 0;
	BaseArray<Variant>::Iterator it = values.GetIterator();
	while (it.Next())
	{
		MemberInfo* member = members[index];
		FieldInfo* field = (FieldInfo*)member;
		const Variant& value = it.Current();
		switch (value.GetType())
		{
		case Variant_Boolean:
			writer.WriteUInt8(value.ToBoolean());
			break;
		case Variant_Int8:
			writer.WriteInt8(value.ToInt8());
			break;
		case Variant_UInt8:
			writer.WriteUInt8(value.ToUInt8());
			break;
		case Variant_Int16:
			writer.WriteInt16(value.ToInt16());
			break;
		case Variant_UInt16:
			writer.WriteUInt16(value.ToUInt16());
			break;
		case Variant_Int32:
			writer.WriteInt32(value.ToInt32());
			break;
		case Variant_UInt32:
			writer.WriteUInt32(value.ToUInt32());
			break;
		case Variant_Int64:
			writer.WriteInt64(value.ToInt64());
			break;
		case Variant_UInt64:
			writer.WriteUInt64(value.ToUInt64());
			break;
		case Variant_Real32:
			writer.WriteReal32(value.ToReal32());
			break;
		case Variant_Real64:
			writer.WriteReal64(value.ToReal64());
			break;
		case Variant_String:
			writer.WriteString(value.ToString());
			break;
		case Variant_Struct:
			{
			FieldInfo* field = (FieldInfo*)member;

			StructObject* structField = (StructObject*)
				TypeFactory::Instance()->CreateInstance(field->GetFieldType()->GetName());

			void* ptr = value.GetData();
			structField->Set(ptr);
			Serialize(writer, structField, structField->GetType());
			delete structField;
			}
			break;
		case Variant_Object:
			{
			Object* objField = value.ToObject();
			Serialize(writer, objField, field->GetFieldType());
			}
			break;
		case Variant_Enum:
			writer.WriteString(EnumObject::GetName(field->GetFieldType(),
				Variant((int32)value.ToEnum())));
			break;
		}

		index++;
	}

	obj->OnSerialized(this, writer);
}

Object* BinarySerializer::Deserialize(BinaryStream& reader)
{
	String typeName = reader.ReadString();

	int8 refindex = reader.ReadInt8();
	if (refindex == 0)
		return NULL;

	Object* obj = TypeFactory::Instance()->CreateInstance(typeName);
	if (obj == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("BinarySerializer.Deserialize"),
			_T("The instance of the object cannot be created because the type name is not registered.") +
			String(_T("Type name: ")) + typeName);
		return NULL;
	}

	Array<MemberInfo*> members;
	members = ISerializer::GetSerializableMembers(obj->GetType());

	BaseArray<Variant> values;
	Array<MemberInfo*>::Iterator it = members.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetMemberType() == MemberTypes_Field)
		{
			FieldInfo* field = (FieldInfo*)it.Current();
			TypeInfo* fieldType = field->GetFieldType();

			Variant* value = NULL;
			if (fieldType == typeof(bool))
				value = new Variant(reader.ReadUInt8());
			else if (fieldType == typeof(int8))
				value = new Variant(reader.ReadInt8());
			else if (fieldType == typeof(uint8))
				value = new Variant(reader.ReadUInt8());
			else if (fieldType == typeof(int16))
				value = new Variant(reader.ReadInt16());
			else if (fieldType == typeof(uint16))
				value = new Variant(reader.ReadUInt16());
			else if (fieldType == typeof(int32))
				value = new Variant(reader.ReadInt32());
			else if (fieldType == typeof(uint32))
				value = new Variant(reader.ReadUInt32());
			else if (fieldType == typeof(int64))
				value = new Variant(reader.ReadInt64());
			else if (fieldType == typeof(uint64))
				value = new Variant(reader.ReadUInt64());
			else if (fieldType == typeof(real32))
				value = new Variant(reader.ReadReal32());
			else if (fieldType == typeof(real64))
				value = new Variant(reader.ReadReal64());
			else if (fieldType == typeof(String))
				value = new Variant(reader.ReadString());
			else if (fieldType->IsStruct())
			{
				StructObject* structField = (StructObject*)
					Deserialize(reader);
				value = new Variant((StructObject*)structField);
			}
			else if (fieldType->IsClass())
			{
				Object* objField = Deserialize(reader);
				value = new Variant((Object*)objField);
			}
			else if (fieldType->IsEnum())
			{
				String enumName = reader.ReadString();
				value = new Variant(EnumObject::GetValue(fieldType, enumName));
			}

			if (value != NULL)
			{
				values.Add(*value);
				delete value;
			}
			else
			{
				values.Add(Variant());
			}
		}
		else
		{
			values.Add(Variant());
		}
	}

	Object* res = ISerializer::BuildObjectMembers(obj, members, values);

	//FIX
	BaseArray<Variant>::Iterator itValue = values.GetIterator();
	while (itValue.Next())
	{
		const Variant& value = itValue.Current();
		if (value.GetType() == Variant_Struct)
			delete value.GetData();
	}

	if (res != NULL)
	{
		res->OnDeserialized(this, reader);
	}

	return res;
}

BinarySerializer::BinarySerializer()
{
}

BinarySerializer::~BinarySerializer()
{
}

void BinarySerializer::Serialize(Stream* stream, Object* obj)
{
	if (stream == NULL || obj == NULL)
		return;

	BinaryStream writer(stream);

	Serialize(writer, obj, obj->GetType());
}

Object* BinarySerializer::Deserialize(Stream* stream)
{
	if (stream == NULL)
		return NULL;

	BinaryStream reader(stream);

	return Deserialize(reader);
}

}
