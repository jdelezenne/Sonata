/*=============================================================================
Flags.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class Enum>
Flag<Enum>::Flag() :
	_Value(0)
{
}

template <class Enum>
Flag<Enum>::Flag(Enum value) :
	_Value(value)
{
}

template <class Enum>
Flag<Enum>::Flag(const Flag& value) :
	_Value(value)
{
}
