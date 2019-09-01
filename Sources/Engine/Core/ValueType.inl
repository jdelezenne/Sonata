/*=============================================================================
ValueType.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/
/*
SE_INLINE int32 ValueType::GetHashCode(int8 value)
{
	return ((int32)(value));
}

SE_INLINE int32 ValueType::GetHashCode(int16 value)
{
	return ((int32)(value | (value << 16)));
}

SE_INLINE int32 ValueType::GetHashCode(int32 value)
{
	return (value);
}

SE_INLINE int32 ValueType::GetHashCode(int64 value)
{
	return (((int32)value) ^ ((int32)(value >> 32)));
}

SE_INLINE int32 ValueType::GetHashCode(real32 value)
{
	int32 i = (int32)(*(&value));
	return i;
}

SE_INLINE int32 ValueType::GetHashCode(real64 value)
{
	int64 l = (int64)(*(&value));
	return (((int32)l) ^ ((int32)(l >> 32)));
}
*/