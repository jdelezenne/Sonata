/*=============================================================================
MethodInfo.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE MemberTypes MethodInfo::GetMemberType() const
{
	return MemberTypes_Method;
}

SE_INLINE String MethodInfo::GetName() const
{
	return _MethodName;
}

SE_INLINE MethodAttributes MethodInfo::GetMethodAttributes() const
{
	return _MethodAttributes;
}

SE_INLINE TypeInfo* MethodInfo::GetReturnType() const
{
	if (_ReturnType != NULL)
		return _ReturnType;

	_ReturnType = TypeFactory::Instance()->GetType((_ReturnTypeName));
	return _ReturnType;
}

SE_INLINE const ParameterList& MethodInfo::GetParameters() const
{
	return _Parameters;
}
