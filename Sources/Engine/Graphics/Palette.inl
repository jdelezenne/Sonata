/*=============================================================================
Palette.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/Exception/ArgumentOutOfRangeException.h"

SE_INLINE Palette::Palette()
{
	_Count = 1;
	_Entries = new Color8[1];
}

SE_INLINE Palette::Palette(int32 count)
{
	if (count <= 0)
		throw ArgumentOutOfRangeException("count", "Must be higher or equal to 1");

	_Count = count;
	_Entries = new Color8[count];
}

SE_INLINE Palette::~Palette()
{
	SE_DELETE_ARRAY(_Entries);
}

SE_INLINE const Color8& Palette::operator[](int index) const
{
	return _Entries[index];
}

SE_INLINE Color8& Palette::operator[](int index)
{
	return _Entries[index];
}

SE_INLINE Palette::operator const Color8*() const
{
	return (const Color8*)_Entries;
}

SE_INLINE Palette::operator Color8*() const
{
	return (Color8*)_Entries;
}

SE_INLINE const Color8& Palette::GetEntry(int index) const
{
	return _Entries[index];
}

SE_INLINE Color8& Palette::GetEntry(int index)
{
	return _Entries[index];
}

SE_INLINE const Color8* Palette::GetEntries() const
{
	return (const Color8*)_Entries;
}

SE_INLINE Color8* Palette::GetEntries()
{
	return (Color8*)_Entries;
}
