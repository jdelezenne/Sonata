/*=============================================================================
ParameterInfo.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE String ParameterInfo::GetName() const
{
	return _ParameterName;
}

SE_INLINE ParameterAttributes ParameterInfo::GetParameterAttributes() const
{
	return _ParameterAttributes;
}

SE_INLINE IMemberInfo* ParameterInfo::GetMember() const
{
	return _Member;
}

SE_INLINE TypeInfo* ParameterInfo::GetParameterType() const
{
	if (_ParameterType != NULL)
		return _ParameterType;

	_ParameterType = TypeFactory::Instance()->GetType((_ParameterTypeName));
	return _ParameterType;
}

SE_INLINE int32 ParameterInfo::GetPosition() const
{
	return _Position;
}
