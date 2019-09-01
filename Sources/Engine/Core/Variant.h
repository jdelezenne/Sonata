/*=============================================================================
Variant.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VARIANT_H_
#define _SE_VARIANT_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

class Object;
class StructObject;

/** Data types of a variant. */
enum VariantType
{
	Variant_Invalid = 0,
	Variant_Boolean,
	Variant_Char,
	Variant_Int8,
	Variant_UInt8,
	Variant_Int16,
	Variant_UInt16,
	Variant_Int32,
	Variant_UInt32,
	Variant_Int64,
	Variant_UInt64,
	Variant_Real32,
	Variant_Real64,
	Variant_String,
	Variant_Struct,
	Variant_Object,
	Variant_Enum,
	Variant_Array
};

/**
	@brief Variant.

	Wrapper around a data type.
	This is a typesafe version of (void*).
*/
class SE_CORE_EXPORT Variant
{
protected:
	union
	{
		bool b;
		int8 i8;
		uint8 ui8;
		int16 i16;
		uint16 ui16;
		int32 i32;
		uint32 ui32;
		int64 i64;
		uint64 ui64;
		real32 r32;
		real64 r64;
		void* ptr;
	} _data;

	VariantType _type;

public:
	/** Invalid variant. */
	static const Variant Invalid;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Variant();

	/** Constructor. */
	Variant(bool value);

	/** Constructor. */
	Variant(int8 value);

	/** Constructor. */
	Variant(uint8 value);

	/** Constructor. */
	Variant(int16 value);

	/** Constructor. */
	Variant(uint16 value);

	/** Constructor. */
	Variant(int32 value);

	/** Constructor. */
	Variant(uint32 value);

	/** Constructor. */
	Variant(int64 value);

	/** Constructor. */
	Variant(uint64 value);

	/** Constructor. */
	Variant(real32 value);

	/** Constructor. */
	Variant(real64 value);

	/** Constructor. */
	Variant(const String& value);

	/** Constructor. */
	Variant(StructObject* value);

	/** Constructor. */
	Variant(Object* value);

	/** Constructor. */
	Variant(void* value, VariantType type);

	/** Destructor. */
	~Variant();
	//@}

	/** @name Operators. */
	//@{
	Variant& operator=(const Variant& value);

	bool operator==(const Variant& value) const;
	bool operator!=(const Variant& value) const;
	//@}

	/** @name Properties. */
	//@{
	/** Gets the data type of the variant. */
	VariantType GetType() const;

	/** Gets a pointer to the raw data of the variant. */
	void* GetData() const;
	//@}

	/** @name Conversion. */
	//@{
	bool ToBoolean() const;
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
	String ToString() const;
	StructObject* ToStruct() const;
	Object* ToObject() const;
	int32 ToEnum() const;
	SEbyte* ToArray() const;
	//@}
};

}

#endif
