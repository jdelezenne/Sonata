/*=============================================================================
String.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_STRING_H_
#define _SE_STRING_H_

#include <string>

#include "Core/Common.h"
#include "Core/Char.h"
#include "Core/Containers/Array.h"

namespace SonataEngine
{

const int32 StringFormatMaxLength = 4096;

class StringPrivate;

/**
	@brief String.

	Represents an array of characters.
*/
class SE_CORE_EXPORT String
{
public:
	/** @name Constructors. */
	//@{
	/** Constructor. */
	String();

	/** Constructor. */
	String(const SEchar* value);

	/** Constructor. */
	String(const SEchar value);

	/** Constructor. */
	String(const Char* value);

	/** Constructor. */
	String(const Char value);

	/** Constructor. */
	String(const String& value);
	//@}

	/** Characters. */
	//@{
	//operator const SEchar*() const;
	//operator const Char*() const;
    Char operator[](int index) const;
    SEchar& operator[](int index); //TOFIX
	//@}

	/** @name Assignment Operators. */
	//@{
	String& operator+=(const String& value);
	String& operator+=(const SEchar* value);
#if SE_USE_UNICODE
	String& operator+=(const SEchar value);
#endif
	//@}

	/** @name Binary Operators. */
	//@{
	bool operator==(const String& value) const;
	bool operator!=(const String& value) const;

    bool operator<(const String& value) const;
    bool operator>(const String& value) const;

	bool operator<=(const String& value) const;
    bool operator>=(const String& value) const;
	//@}

	/** Gets the string. */
	const SEchar* Data() const;

	/** Gets the number of characters in this instance. */
	int Length() const;

	/** Gets the character at a specified character position in this instance. */ 
	Char Chars(int index) const;

	/** Retrieves an object that can iterate through the individual characters in this instance. */
	//CharEnumerator GetEnumerator();

	/** Returns whether the string is empty. */
	bool IsEmpty() const;

	/** Copies the characters in this instance to a Unicode character array. */ 
	Array<Char> ToCharArray() const;
	Array<Char> ToCharArray(int startIndex, int count) const;

	/** Identifies the substrings in this instance that are delimited by one or more characters specified in an array, then places the substrings into a string array. */
	Array<String> Split(Char separator) const;

	/** Compares two specified strings. */
	static int Compare(const String& left, const String& right, bool ignoreCase = false);

	/** Compares this instance with a specified object. */
	int CompareTo(const String& str, bool ignoreCase = false) const;

	/** Gets the index position of the first occurrence of a specified character within this string. */
	int IndexOf(Char value) const;

	/** Gets the index position of the last occurrence of a specified character within this string. */
	int LastIndexOf(Char value) const;

	/** Replaces all occurrences of a specified Unicode character or String in this instance, with another specified Unicode character or String. */
	String Replace(const Char& oldValue, const Char& newValue);
	String Replace(const String& oldValue, const String& newValue);

	/** Deletes a specified number of characters from this instance beginning at a specified position. */
	String Remove(int start, int length);

	/** Retrieves a substring from this string. */
	String Substring(int start) const;
	String Substring(int start, int length) const;

	/** Retrieves a substring that contains the leftmost characters of this string. */
	String Left(int length) const;

	/** Retrieves a substring that contains the rightmost characters of this string. */
	String Right(int length) const;

	/** Returns a copy of this String in uppercase. */
	String ToUpper() const;

	/** Returns a copy of this String in lowercase. */
	String ToLower() const;

	/** Removes all occurrences of white space characters from the beginning and end of this string. */
	String Trim();

	/** Removes all occurrences of a set of characters specified in an array from the beginning of this instance. */
	String TrimStart();

	/** Removes all occurrences of a set of characters specified in an array from the end of this instance. */
	String TrimEnd();

	/** Replaces the format item in a specified String with the text equivalent of the value of a corresponding Object instance in a specified array. */
	static String Format(const SEchar* format, ...);

	/** Concatenates one or more instances of String. */
	static String Concat(const String& left, const String& right);

	/** Converts the String representation of a number to an equivalent value type. */
	SEchar ToChar() const;
	int8 ToInt8() const;
	uint8 ToUInt8() const;
	int16 ToInt16() const;
	uint16 ToUInt16() const;
	int32 ToInt32() const;
	uint32 ToUInt32() const;
	int64 ToInt64() const;
	uint64 ToUInt64() const;
	real32 ToReal32() const;
	real64 ToReal64() const;

	/** Converts the specified value to its equivalent String representation. */
	static String ToString(SEchar value);
	static String ToString(SEchar* value);
	static String ToString(const SEchar* value);
	static String ToString(int8 value);
	static String ToString(uint8 value);
	static String ToString(int16 value);
	static String ToString(uint16 value);
	static String ToString(int32 value);
	static String ToString(uint32 value);
	static String ToString(int64 value);
	static String ToString(uint64 value);
	static String ToString(real32 value);
	static String ToString(real64 value);

public:
	/** Empty string. */
	static const String Empty;

protected:
	typedef std::basic_string<SEchar,
		std::char_traits<SEchar>, std::allocator<SEchar> > InnerType;
	InnerType _string;
};

SE_INLINE const String operator+(const String& left, const String& right)
{ String s(left); s += right; return s; }
SE_INLINE const String operator+(const String& left, const Char& right)
{ String s(left); s += right; return s; }
SE_INLINE const String operator+(const Char& left, const String& right)
{ String s(left); s += right; return s; }

#include "String.inl"

}

#endif
