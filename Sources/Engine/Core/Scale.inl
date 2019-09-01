/*=============================================================================
Scale.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
ScaleKey<T>::ScaleKey()
{
}

template <class T>
ScaleKey<T>::ScaleKey(real32 relativeTime, T value)
{
	RelativeTime = _relativeTime;
	Value = _value;
}

template <class T>
void ScaleKey<T>::Get(real32& relativeTime, T& value) const
{
	relativeTime = RelativeTime;
	value = Value;
}

template <class T>
void ScaleKey<T>::Set(real32 relativeTime, T value)
{
	RelativeTime = relativeTime;
	Value = value;
}

template <class T>
bool ScaleKey<T>::operator==(const ScaleKey<T>& other) const
{
	return (RelativeTime == other.RelativeTime && Value == other.Value);
}

template <class T>
bool ScaleKey<T>::operator!=(const ScaleKey<T>& other) const
{
	return (!(*this == other));
}


template <class T>
Scale<T>::Scale()
{
}

template <class T>
bool Scale<T>::operator==(const Scale<T>& other) const
{
	if (Count() != other.Count())
		return false;
	
	int count = Count();
	for (int i = 0; i < count; i++)
		if (GetItem(i) != other)
			return false;

	return true;
}

template <class T>
bool Scale<T>::operator!=(const Scale<T>& other) const
{
	return !(*this == other);
}
