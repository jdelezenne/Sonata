/*=============================================================================
Char.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <ctype.h>

SE_INLINE Char::Char()
{
}

SE_INLINE Char::Char(const SEchar value)
{
	_value = value;
}

SE_INLINE Char::Char(const int value)
{
	_value = value;
}

SE_INLINE Char::Char(const Char& value)
{
	_value = value;
}

SE_INLINE Char::operator const SEchar() const
{
	return _value;
}

SE_INLINE Char::operator SEchar()
{
	return _value;
}

SE_INLINE const SEchar& Char::operator*() const
{
	return _value;
}

SE_INLINE SEchar& Char::operator*()
{
	return _value;
}

SE_INLINE bool Char::IsPrintable(const Char& c)
{
	return isprint(c) != 0;
}

SE_INLINE bool Char::IsControl(const Char& c)
{
	return iscntrl(c) != 0;
}

SE_INLINE bool Char::IsPunctuation(const Char& c)
{
	return ispunct(c) != 0;
}

SE_INLINE bool Char::IsWhiteSpace(const Char& c)
{
	return isspace(c) != 0;
}

SE_INLINE bool Char::IsLetter(const Char& c)
{
	return isalpha(c) != 0;
}

SE_INLINE bool Char::IsDigit(const Char& c)
{
	return isdigit(c) != 0;
}

SE_INLINE bool Char::IsLetterOrDigit(const Char& c)
{
	return isalnum(c) != 0;
}

SE_INLINE bool Char::IsUpper(const Char& c)
{
	return isupper(c) != 0;
}

SE_INLINE bool Char::IsNumber(const Char& c)
{
	return isalnum(c) != 0 && !isalpha(c) != 0;
}

SE_INLINE bool Char::IsLower(const Char& c)
{
	return islower(c) != 0;
}

SE_INLINE Char Char::ToUpper(const Char& c)
{
	return toupper(c);
}

SE_INLINE Char Char::ToLower(const Char& c)
{
	return tolower(c);
}
