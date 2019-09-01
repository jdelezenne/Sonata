/*=============================================================================
XMLSerializer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLSerializer.h"
#include "Core/IO/FileStream.h"
#include "Core/XML/XML.h"
#include "Core/Logging/Logger.h"

namespace SonataEngine
{

XMLSerializer::XMLSerializer()
{
}

XMLSerializer::~XMLSerializer()
{
}

void XMLSerializer::Serialize(XMLDocument* document, XMLNode* parentNode, Object* obj, TypeInfo* type)
{
	if (type->IsAbstract())
		return;

	XMLElement* typeElement = document->CreateElement(type->GetName());

	if (obj == NULL)
	{
		parentNode->AppendChild(typeElement);
		return;
	}

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
		XMLElement* memberElement = document->CreateElement(member->GetName());
		XMLNode* valueElement = document->CreateTextNode(String::Empty);

		const Variant& value = it.Current();
		switch (value.GetType())
		{
		case Variant_Boolean:
			valueElement->SetValue(String::ToString(value.ToBoolean()));
			break;
		case Variant_Int8:
			valueElement->SetValue(String::ToString(value.ToInt8()));
			break;
		case Variant_UInt8:
			valueElement->SetValue(String::ToString(value.ToUInt8()));
			break;
		case Variant_Int16:
			valueElement->SetValue(String::ToString(value.ToInt16()));
			break;
		case Variant_UInt16:
			valueElement->SetValue(String::ToString(value.ToUInt16()));
			break;
		case Variant_Int32:
			valueElement->SetValue(String::ToString(value.ToInt32()));
			break;
		case Variant_UInt32:
			valueElement->SetValue(String::ToString(value.ToUInt32()));
			break;
		case Variant_Int64:
			valueElement->SetValue(String::ToString(value.ToInt64()));
			break;
		case Variant_UInt64:
			valueElement->SetValue(String::ToString(value.ToUInt64()));
			break;
		case Variant_Real32:
			valueElement->SetValue(String::ToString(value.ToReal32()));
			break;
		case Variant_Real64:
			valueElement->SetValue(String::ToString(value.ToReal64()));
			break;
		case Variant_String:
			valueElement->SetValue(value.ToString());
			break;
		case Variant_Struct:
			{
			StructObject* structField = (StructObject*)
				TypeFactory::Instance()->CreateInstance(field->GetFieldType()->GetName());

			void* ptr = value.GetData();
			structField->Set(ptr);
			Serialize(document, memberElement, structField, structField->GetType());
			delete structField;
			}
			break;
		case Variant_Object:
			{
			Object* objField = value.ToObject();
			Serialize(document, memberElement, objField, field->GetFieldType());
			}
			break;
		case Variant_Enum:
			valueElement->SetValue(EnumObject::GetName(field->GetFieldType(),
				Variant((int32)value.ToEnum())));
			break;
		}

		memberElement->AppendChild(valueElement);
		typeElement->AppendChild(memberElement);
		index++;
	}

	obj->OnSerialized(this, typeElement);

	parentNode->AppendChild(typeElement);
}

Object* XMLSerializer::Deserialize(XMLDocument* document, XMLNode* typeElement)
{
	XMLElement* memberElement = (XMLElement*)typeElement->GetFirstChild();

	if (memberElement == NULL)
		return NULL;

	String typeName = typeElement->GetName();
	Object* obj = TypeFactory::Instance()->CreateInstance(typeName);
	if (obj == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("XMLSerializer.Deserialize"),
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
			Variant* value = NULL;

			FieldInfo* field = (FieldInfo*)it.Current();
			XMLElement* memberElement = (XMLElement*)typeElement->GetItem(field->GetName());
			if (memberElement != NULL)
			{
				TypeInfo* fieldType = field->GetFieldType();

				XMLNode* valueElement = memberElement->GetFirstChild();
				if (valueElement != NULL)
				{
					if (valueElement->GetNodeType() == XMLNodeType_Text)
					{
						String str = valueElement->GetValue();

						if (fieldType == typeof(bool))
							value = new Variant(str != "0");
						else if (fieldType == typeof(int8))
							value = new Variant(str.ToInt8());
						else if (fieldType == typeof(uint8))
							value = new Variant(str.ToUInt8());
						else if (fieldType == typeof(int16))
							value = new Variant(str.ToInt16());
						else if (fieldType == typeof(uint16))
							value = new Variant(str.ToUInt16());
						else if (fieldType == typeof(int32))
							value = new Variant(str.ToInt32());
						else if (fieldType == typeof(uint32))
							value = new Variant(str.ToUInt32());
						else if (fieldType == typeof(int64))
							value = new Variant(str.ToInt64());
						else if (fieldType == typeof(uint64))
							value = new Variant(str.ToUInt64());
						else if (fieldType == typeof(real32))
							value = new Variant(str.ToReal32());
						else if (fieldType == typeof(real64))
							value = new Variant(str.ToReal64());
						else if (fieldType == typeof(String))
							value = new Variant(str);
						else if (fieldType->IsEnum())
						{
							String enumName = str;
							value = new Variant(EnumObject::GetValue(fieldType, enumName));
						}
					}
					else if (fieldType->IsStruct())
					{
						if (valueElement->GetNodeType() == XMLNodeType_Element &&
							fieldType->GetName() == valueElement->GetName())
						{
							StructObject* structField = (StructObject*)
								Deserialize(document, valueElement);
							value = new Variant((StructObject*)structField);
						}
					}
					else if (fieldType->IsClass())
					{
						if (valueElement->GetNodeType() == XMLNodeType_Element &&
							fieldType->GetName() == valueElement->GetName())
						{
							Object* objField = Deserialize(document, valueElement);
							value = new Variant((Object*)objField);
						}
					}
				}
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
		res->OnDeserialized(this, (XMLElement*)typeElement);
	}

	return res;
}

void XMLSerializer::Serialize(Stream* stream, Object* obj)
{
	if (stream == NULL || obj == NULL)
		return;

	XMLDocument* document = new XMLDocument();

	Serialize(document, document, obj, obj->GetType());

	if (stream->GetStreamType() == StreamType_File)
	{
		String fileName = ((FileStream*)stream)->GetFileName();
		stream->Close();
		XMLWriter writer(document, fileName);
	}

	delete document;
}

Object* XMLSerializer::Deserialize(Stream* stream)
{
	if (stream == NULL)
		return NULL;

	if (stream->GetStreamType() != StreamType_File)
		return NULL;

	String fileName = ((FileStream*)stream)->GetFileName();
	stream->Close();
	XMLReader reader(fileName);
	XMLDocument* document = reader.GetDocument();

	Object* obj = Deserialize(document, document->GetDocumentElement());
	delete document;
	return obj;
}

}
