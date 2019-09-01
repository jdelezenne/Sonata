/*=============================================================================
FieldInfo.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE MemberTypes FieldInfo::GetMemberType() const
{
	return MemberTypes_Field;
}

SE_INLINE String FieldInfo::GetName() const
{
	return _FieldName;
}

SE_INLINE FieldAttributes FieldInfo::GetFieldAttributes() const
{
	return _FieldAttributes;
}

SE_INLINE TypeInfo* FieldInfo::GetFieldType() const
{
	if (_FieldType != NULL)
		return _FieldType;

	_FieldType = TypeFactory::Instance()->GetType((_FieldTypeName));
	return _FieldType;
}

/*template <class T>
T& FieldInfo::GetValue(Object* obj)
{
	return *(const T*)(((const byte*)obj) + _Offset);
}

template <class T>
void FieldInfo::SetValue(Object* obj, const T& value)
{
	*(T*)(((const byte*)obj) + _Offset) = value;
}*/
