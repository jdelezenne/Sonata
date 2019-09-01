/*=============================================================================
TypeInfo.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE MemberTypes TypeInfo::GetMemberType() const
{
	return MemberTypes_Type;
}

SE_INLINE String TypeInfo::GetName() const
{
	return _name;
}

/*SE_INLINE SE_ID TypeInfo::GetSE_ID() const
{
	return _SE_ID;
}*/

SE_INLINE TypeInfo* TypeInfo::GetBaseType() const
{
	if (_SuperType != NULL)
		return _SuperType;

	_SuperType = TypeFactory::Instance()->GetType((_SuperTypeName));
	return _SuperType;
}

SE_INLINE bool TypeInfo::IsAbstract() const
{
	return (_Creator == NULL);
}
