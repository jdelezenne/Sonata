/*=============================================================================
ISerializer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ISerializer.h"

namespace SonataEngine
{

Array<MemberInfo*> ISerializer::GetSerializableMembers(TypeInfo* type)
{
	const FieldList& fields = type->GetFields();
	Array<MemberInfo*> members;
	FieldList::Iterator it = fields.GetIterator();
	while (it.Next())
	{
		members.Add(it.Current());
	}
	return members;
}

BaseArray<Variant> ISerializer::GetObjectValues(Object* obj, const Array<MemberInfo*>& members)
{
	BaseArray<Variant> values;
	Array<MemberInfo*>::Iterator it = members.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetMemberType() == MemberTypes_Field)
		{
			FieldInfo* field = (FieldInfo*)it.Current();
			field->GetFieldType();//FIX
			values.Add(field->GetValue(obj));
		}
		else
		{
			values.Add(Variant());
		}
	}
	return values;
}

Object* ISerializer::BuildObjectMembers(Object* obj, const Array<MemberInfo*>& members, const BaseArray<Variant>& values)
{
	int32 index = 0;
	Array<MemberInfo*>::Iterator it = members.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetMemberType() == MemberTypes_Field)
		{
			FieldInfo* field = (FieldInfo*)it.Current();
			field->GetFieldType();//FIX

			const Variant& value = values[index];
			if (value.GetType() != Variant_Invalid)
				field->SetValue(obj, value);
		}
		else
		{
		}

		index++;
	}

	return obj;
}

}
