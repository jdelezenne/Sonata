/*=============================================================================
String.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <string>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

#include "String.h"
#include "Core/Exception/FormatException.h"
#include "Core/Exception/OverflowException.h"

namespace SonataEngine
{

const String String::Empty = _T("");

static char WhitespaceChars[] =
{
	_T('\t'), _T('\n'), _T('\t'), _T('\f'), _T('\r'), _T(' ')
};
static int WhitespaceCharsCount = sizeof(WhitespaceChars) / sizeof(WhitespaceChars[0]);

bool Contains(const char* array, int size, char value)
{
	for (int i = 0; i < size; i++)
		if (array[i] == value)
			return true;

	return false;
}

Array<Char> String::ToCharArray() const
{
	//TODO
	return Array<Char>((const Char*) _string.c_str(), (int) _string.length());
}

Array<Char> String::ToCharArray(int startIndex, int count) const
{
	//TODO
	return Array<Char>();
}

Array<String> String::Split(Char separator) const
{
	Array<String> substrings;
	int length = Length() - 1;
	int start = 0;
	int end = 0;
	int sublength;
	while (end < length)
	{
		if (Chars(end) == separator)
		{
			if (end-1 > 0 && Chars(end-1) == separator)
			{
				start = end + 1;
			}
			else
			{
				sublength = end - start;
				substrings.Add(Substring(start, sublength));
				start = end + 1;
			}
		}
		end++;
	}

	sublength = end - start + 1;
	if (sublength > 0)
		substrings.Add(Substring(start, sublength));

	return substrings;
}

String String::Left(int length) const
{
	if (length > Length() || length < 0)
		return *this;

	return Substring(0, length);
}

String String::Right(int length) const
{
	if (length > Length() || length < 0)
		return *this;

	return Substring(Length() - length, length);
}

String String::ToUpper() const
{
	std::string value = _string;
	for (std::string::iterator it = value.begin(), en = value.end(); it != en; ++it)
		*it = (char)toupper(*it);

	return value.c_str();
}

String String::ToLower() const
{
	std::string value = _string;
	for (std::string::iterator it = value.begin(), en = value.end(); it != en; ++it)
		*it = (char)tolower(*it);

	return value.c_str();
}

String String::Trim()
{
	if (IsEmpty())
		return *this;

	const char* str = Data();
	int start = 0;
	int end = Length() - 1;

	// check first and last char
	if (!Contains(WhitespaceChars, WhitespaceCharsCount, str[0]) &&
		!Contains(WhitespaceChars, WhitespaceCharsCount, str[end]))
		return *this;

	while (start <= end && Contains(WhitespaceChars, WhitespaceCharsCount, str[start]))
		start++;

	if (start <= end)
	{
		while (end && Contains(WhitespaceChars, WhitespaceCharsCount, str[end]))
		end--;
	}

	int length = end - start + 1;
	if (length <= 0)
		return "";

	return Substring(start, length);
}

String String::TrimStart()
{
	if (IsEmpty())
		return *this;

	const char* str = Data();
	int start = 0;
	int end = Length() - 1;

	// check first char
	if (!Contains(WhitespaceChars, WhitespaceCharsCount, str[0]))
		return *this;

	while (start <= end && Contains(WhitespaceChars, WhitespaceCharsCount, str[start]))
		start++;

	int length = end - start + 1;
	if (length <= 0)
		return "";

	return Substring(start, length);
}

String String::TrimEnd()
{
	if (IsEmpty())
		return *this;

	const char* str = Data();
	int start = 0;
	int end = Length() - 1;

	// check first and last char
	if (!Contains(WhitespaceChars, WhitespaceCharsCount, str[end]))
		return *this;

	if (start <= end)
	{
		while (end && Contains(WhitespaceChars, WhitespaceCharsCount, str[end]))
		end--;
	}

	int length = end - start + 1;
	if (length <= 0)
		return "";

	return Substring(start, length);
}

String String::Format(const SEchar* format, ...)
{
	SEchar buffer[StringFormatMaxLength];
	buffer[StringFormatMaxLength-1] = _T('\0');
	va_list args;
	va_start(args, format);
#ifdef _WIN32
	_vsnprintf(buffer, StringFormatMaxLength-1, format, args);
#else
	vsnprintf(buffer, StringFormatMaxLength-1, format, args);
#endif
	va_end(args);

	return buffer;
}

String String::Concat(const String& strA, const String& strB)
{
	return strA + strB;
}

SEchar String::ToChar() const
{
	if (Length() != 1)
		throw FormatException(_T("String must be only one character long."));
	return Chars(0);
}

int8 String::ToInt8() const
{
	SEchar* end;
	long r = strtol(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_I8 || r > SE_MAX_I8)
		throw OverflowException(_T("Value was either too large or too small for a Int8."));
	return (int8)r;
}

uint8 String::ToUInt8() const
{
	SEchar* end;
	unsigned long r = strtoul(_string.c_str(), &end, 10);
	if (errno == ERANGE)// || r < SE_MIN_U8 || r > SE_MAX_U8)
		throw OverflowException(_T("Value was either too large or too small for a UInt8."));
	return (uint8)r;
}

int16 String::ToInt16() const
{
	SEchar* end;
	long r = strtol(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_I16 || r > SE_MAX_I16)
		throw OverflowException(_T("Value was either too large or too small for a Int16."));
	return (int16)r;
}

uint16 String::ToUInt16() const
{
	SEchar* end;
	unsigned long r = strtoul(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_U16 || r > SE_MAX_U16)
		throw OverflowException(_T("Value was either too large or too small for a UInt16."));
	return (uint16)r;
}

int32 String::ToInt32() const
{
	SEchar* end;
	long r = strtol(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_I32 || r > SE_MAX_I32)
		throw OverflowException(_T("Value was either too large or too small for a Int32."));
	return (int32)r;
}

uint32 String::ToUInt32() const
{
	SEchar* end;
	unsigned long r = strtoul(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_U32 || r > SE_MAX_U32)
		throw OverflowException(_T("Value was either too large or too small for a UInt32."));
	return (uint32)r;
}

int64 String::ToInt64() const
{
#ifdef _WIN32
	SEchar* end;
	uint64 r = _strtoi64(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_I64 || r > SE_MAX_I64)
		throw OverflowException(_T("Value was either too large or too small for a Int64."));
	return (int64)r;
#else
	throw NotImplementedException();
#endif
}

uint64 String::ToUInt64() const
{
#ifdef _WIN32
	SEchar* end;
	unsigned __int64 r = _strtoui64(_string.c_str(), &end, 10);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)// || r < SE_MIN_U64 || r > SE_MAX_U64)
		throw OverflowException(_T("Value was either too large or too small for a UInt64."));
	return (uint64)r;
#else
	throw NotImplementedException();
#endif
}

real32 String::ToReal32() const
{
	SEchar* end;
	real32 r = (real32)strtod(_string.c_str(), &end);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)
		throw FormatException(_T("Input string was not in a correct format."));
	return (real32)r;
}

real64 String::ToReal64() const
{
	SEchar* end;
	real64 r = strtod(_string.c_str(), &end);
	if (end == '\0')
		throw FormatException(_T("Input string was not in a correct format."));
	if (errno == ERANGE)
		throw FormatException(_T("Input string was not in a correct format."));
	return (real64)r;
}

String String::ToString(SEchar value)
{
	return value;
}

String String::ToString(SEchar* value)
{
	return (String)value;
}

String String::ToString(const SEchar* value)
{
	return (String)value;
}

String String::ToString(int8 value)
{
	SEchar buffer[65];
	return _itoa(value, buffer, 10);
}

String String::ToString(uint8 value)
{
	SEchar buffer[65];
	return _ultoa(value, buffer, 10);
}

String String::ToString(int16 value)
{
	SEchar buffer[65];
	return _itoa(value, buffer, 10);
}

String String::ToString(uint16 value)
{
	SEchar buffer[65];
	return _ultoa(value, buffer, 10);
}

String String::ToString(int32 value)
{
	SEchar buffer[65];
	return _itoa(value, buffer, 10);
}

String String::ToString(uint32 value)
{
	SEchar buffer[65];
	return _ultoa(value, buffer, 10);
}

String String::ToString(int64 value)
{
#ifdef _WIN32
	SEchar buffer[65];
	return _i64toa(value, buffer, 10);
#else
	throw NotImplementedException();
#endif
}

String String::ToString(uint64 value)
{
#ifdef _WIN32
	SEchar buffer[65];
	return _ui64toa(value, buffer, 10);
#else
	throw NotImplementedException();
#endif
}

String String::ToString(real32 value)
{
	SEchar buffer[1024];
	sprintf(buffer, _T("%f"), value);
	return buffer;
}

String String::ToString(real64 value)
{
	SEchar buffer[1024];
	sprintf(buffer, _T("%f"), value);
	return buffer;
}

}
