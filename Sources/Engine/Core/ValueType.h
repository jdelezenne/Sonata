/*=============================================================================
ValueType.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VALUETYPE_H_
#define _SE_VALUETYPE_H_

#include "Core/Common.h"
#include "Core/Variant.h"
#include "Core/String.h"

namespace SonataEngine
{

/**
	Specifies the type of an object.
	@remarks From .NET
*/
enum TypeCode
{
	/** A null reference. */
	TypeCode_Empty = 0,

	/** An object. */
	TypeCode_Object = 1,

	/** A simple type representing Boolean values of true or false. */
	TypeCode_Boolean = 3,

	/** An integral type representing unsigned 16-bit integers with values between 0 and 65535. */
	TypeCode_Char = 4,

	/** An integral type representing signed 8-bit integers with values between -128 and 127. */
	TypeCode_SByte = 5,

	/** An integral type representing unsigned 8-bit integers with values between 0 and 255. */
	TypeCode_Byte = 6,

	/** An integral type representing signed 16-bit integers with values
	between -32768 and 32767. */
	TypeCode_Int16 = 7,

	/** An integral type representing unsigned 16-bit integers with values
	between 0 and 65535. */
	TypeCode_UInt16 = 8,

	/** An integral type representing signed 32-bit integers with values
	between -2147483648 and 2147483647. */
	TypeCode_Int32 = 9,

	/** An integral type representing unsigned 32-bit integers with values
	between 0 and 4294967295. */
	TypeCode_UInt32 = 10,

	/** An object. */
	TypeCode_Int64 = 11,

	/** An integral type representing unsigned 64-bit integers with values
	between 0 and 18446744073709551615. */
	TypeCode_UInt64 = 12,

	/** A floating point type representing values ranging from
	approximately 1.5 x 10 -45 to 3.4 x 10 38 with a precision of 7 digits. */
	TypeCode_Real32 = 13,

	/** A floating point type representing values ranging from
	approximately 5.0 x 10 -324 to 1.7 x 10 308 with a precision
	of 15-16 digits. */
	TypeCode_Real64 = 14,

	/** A sealed class type representing character strings. */
	TypeCode_String = 18,
};

/** @brief Types traits.

	Provides the base class for value types.
	This class provides information about the primitive data types.
*/
template <class T>
class SE_CORE_EXPORT ValueType
{
public:
	static const T MaxValue;
	static const T MinValue;

public:
	ValueType()
	{
	}

	ValueType(const T& value)
	{
	}

	int32 CompareTo(const T& value)
	{
		return 0;
	}

	bool Equals(const T& value)
	{
		return false;
	}

	int32 GetHashCode()
	{
		return 0;
	}

	static T Parse(const String& s)
	{
		return T();
	}

	String ToString()
	{
		return String::Empty;
	}

	TypeCode GetTypeCode()
	{
		return TypeCode_Empty;
	}
};

/** A simple type representing Boolean values of true or false. */
template<> class ValueType<bool>
{
public:
	int32 CompareTo(const bool& value)
	{
		return (!_Value && value ? -1 : (_Value && !value ? +1 : 0));
	}

	bool Equals(const bool& value)
	{
		return (_Value == value);
	}

	int32 GetHashCode()
	{
		return (_Value ? 1 : 0);
	}

	static bool Parse(const String& s)
	{
		return !s.IsEmpty();
	}

	String ToString()
	{
		return String::ToString(_Value);
	}

	TypeCode GetTypeCode()
	{
		return TypeCode_Boolean;
	}

protected:
	bool _Value;
};

/** Represents an 8-bit unsigned integer. */
template<> class ValueType<SEbyte>
{
public:
	int32 CompareTo(const SEbyte& value)
	{
		return (_Value < value ? -1 : (_Value > value ? +1 : 0));
	}

	bool Equals(const SEbyte& value)
	{
		return (_Value == value);
	}

	int32 GetHashCode()
	{
		return ((int32)(_Value));
	}

	static SEbyte Parse(const String& s)
	{
		return s.ToUInt8();
	}

	String ToString()
	{
		return String::ToString(_Value);
	}

	TypeCode GetTypeCode()
	{
		return TypeCode_Byte;
	}

protected:
	SEbyte _Value;
};

typedef ValueType<bool> BoolType;
typedef ValueType<SEbyte> ByteType;
typedef ValueType<int8> Int8Type;
typedef ValueType<int16> Int16;
typedef ValueType<int32> Int32Type;
typedef ValueType<int64> Int64Type;
typedef ValueType<real32> Real32Type;
typedef ValueType<real64> Real64Type;
typedef ValueType<real> RealType;

#include "ValueType.inl"

}

#endif 
