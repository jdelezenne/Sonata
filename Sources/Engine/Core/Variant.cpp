/*=============================================================================
Variant.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Variant.h"
#include "Core/Object.h"

namespace SonataEngine
{

const Variant Variant::Invalid = Variant();

Variant::Variant() :
	_type(Variant_Invalid)
{
}

Variant::Variant(bool value)
{
	_type = Variant_Boolean;
	_data.b = value;
}

Variant::Variant(int8 value)
{
	_type = Variant_Int8;
	_data.i8 = value;
}

Variant::Variant(uint8 value)
{
	_type = Variant_UInt8;
	_data.ui8 = value;
}

Variant::Variant(int16 value)
{
	_type = Variant_Int16;
	_data.i16 = value;
}

Variant::Variant(uint16 value)
{
	_type = Variant_UInt16;
	_data.ui16 = value;
}

Variant::Variant(int32 value)
{
	_type = Variant_Int32;
	_data.i32 = value;
}

Variant::Variant(uint32 value)
{
	_type = Variant_UInt32;
	_data.ui32 = value;
}

Variant::Variant(int64 value)
{
	_type = Variant_Int64;
	_data.i64 = value;
}

Variant::Variant(uint64 value)
{
	_type = Variant_UInt64;
	_data.ui64 = value;
}

Variant::Variant(real32 value)
{
	_type = Variant_Real32;
	_data.r32 = value;
}

Variant::Variant(real64 value)
{
	_type = Variant_Real64;
	_data.r64 = value;
}

Variant::Variant(const String& value)
{
	_type = Variant_String;
	_data.ptr = new String(value);
}

Variant::Variant(StructObject* value)
{
	_type = Variant_Struct;
	_data.ptr = value;
}

Variant::Variant(Object* value)
{
	_type = Variant_Object;
	_data.ptr = value;
}

Variant::Variant(void* value, VariantType type)
{
	_type = type;
	_data.ptr = value;
}

Variant::~Variant()
{
	//if (_type == Variant_String)
	//	delete _data.ptr;
}

Variant& Variant::operator=(const Variant& value)
{
	_type = value._type;
	if (_type == Variant_String)
	{
		_data.ptr = new String(*(String*)value._data.ptr);
	}
	else
	{
		_data = value._data;
	}

	return *this;
}

bool Variant::operator==(const Variant& value) const
{
	if (_type != value.GetType())
		return false;

	switch (_type)
	{
	case Variant_Boolean:
		return (ToBoolean() == value.ToBoolean());
	case Variant_Int8:
		return (ToInt8() == value.ToInt8());
	case Variant_UInt8:
		return (ToUInt8() == value.ToUInt8());
	case Variant_Int16:
		return (ToInt16() == value.ToInt16());
	case Variant_UInt16:
		return (ToUInt16() == value.ToUInt16());
	case Variant_Int32:
		return (ToInt32() == value.ToInt32());
	case Variant_UInt32:
		return (ToUInt32() == value.ToUInt32());
	case Variant_Int64:
		return (ToInt64() == value.ToInt64());
	case Variant_UInt64:
		return (ToUInt64() == value.ToUInt64());
	case Variant_Real32:
		return (ToReal32() == value.ToReal32());
	case Variant_Real64:
		return (ToReal64() == value.ToReal64());
	case Variant_String:
		return (ToString() == value.ToString());
	case Variant_Struct:
		return (ToStruct() == value.ToStruct());
	case Variant_Object:
		return (ToObject() == value.ToObject());
	case Variant_Enum:
		return (ToEnum() == value.ToEnum());
	case Variant_Array:
		return (ToArray() == value.ToArray());
	default:
		return false;
	}
}

bool Variant::operator!=(const Variant& value) const
{
	return !(*this == value);
}

VariantType Variant::GetType() const
{
	return _type;
}

void* Variant::GetData() const
{
	return _data.ptr;
}

bool Variant::ToBoolean() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return _data.b;
	case Variant_Int8:
		return (_data.i8 != 0);
	case Variant_UInt8:
		return (_data.ui8 != 0);
	case Variant_Int16:
		return (_data.i16 != 0);
	case Variant_UInt16:
		return (_data.ui16 != 0);
	case Variant_Int32:
		return (_data.i32 != 0);
	case Variant_UInt32:
		return (_data.ui32 != 0);
	case Variant_Int64:
		return (_data.i64 != 0);
	case Variant_UInt64:
		return (_data.ui64 != 0);
	case Variant_Real32:
		return (_data.r32 != 0.0f);
	case Variant_Real64:
		return (_data.r64 != 0.0);
	case Variant_String:
		{
		String str = *((String*)_data.ptr);
		if (str.IsEmpty() || str == _T("0") || str == _T("false"))
			return false;
		else
			return true;
		}
	case Variant_Object:
		return (_data.ptr != NULL);
	default:
		return false;
	}
}

SEchar Variant::ToChar() const
{
	return (SEchar)_data.i32;
}

int8 Variant::ToInt8() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (int8)_data.b;
	case Variant_Int8:
		return (int8)_data.i8;
	case Variant_UInt8:
		return (int8)_data.ui8;
	case Variant_Int16:
		return (int8)_data.i16;
	case Variant_UInt16:
		return (int8)_data.ui16;
	case Variant_Int32:
		return (int8)_data.i32;
	case Variant_UInt32:
		return (int8)_data.ui32;
	case Variant_Int64:
		return (int8)_data.i64;
	case Variant_UInt64:
		return (int8)_data.ui64;
	case Variant_Real32:
		return (int8)_data.r32;
	case Variant_Real64:
		return (int8)_data.r64;
	case Variant_String:
		return ToString().ToInt8();
	default:
		return 0;
	}
}

uint8 Variant::ToUInt8() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (uint8)_data.b;
	case Variant_Int8:
		return (uint8)_data.i8;
	case Variant_UInt8:
		return (uint8)_data.ui8;
	case Variant_Int16:
		return (uint8)_data.i16;
	case Variant_UInt16:
		return (uint8)_data.ui16;
	case Variant_Int32:
		return (uint8)_data.i32;
	case Variant_UInt32:
		return (uint8)_data.ui32;
	case Variant_Int64:
		return (uint8)_data.i64;
	case Variant_UInt64:
		return (uint8)_data.ui64;
	case Variant_Real32:
		return (uint8)_data.r32;
	case Variant_Real64:
		return (uint8)_data.r64;
	case Variant_String:
		return ToString().ToUInt8();
	default:
		return 0;
	}
}

int16 Variant::ToInt16() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (int16)_data.b;
	case Variant_Int8:
		return (int16)_data.i8;
	case Variant_UInt8:
		return (int16)_data.ui8;
	case Variant_Int16:
		return (int16)_data.i16;
	case Variant_UInt16:
		return (int16)_data.ui16;
	case Variant_Int32:
		return (int16)_data.i32;
	case Variant_UInt32:
		return (int16)_data.ui32;
	case Variant_Int64:
		return (int16)_data.i64;
	case Variant_UInt64:
		return (int16)_data.ui64;
	case Variant_Real32:
		return (int16)_data.r32;
	case Variant_Real64:
		return (int16)_data.r64;
	case Variant_String:
		return ToString().ToInt16();
	default:
		return 0;
	}
}

uint16 Variant::ToUInt16() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (uint16)_data.b;
	case Variant_Int8:
		return (uint16)_data.i8;
	case Variant_UInt8:
		return (uint16)_data.ui8;
	case Variant_Int16:
		return (uint16)_data.i16;
	case Variant_UInt16:
		return (uint16)_data.ui16;
	case Variant_Int32:
		return (uint16)_data.i32;
	case Variant_UInt32:
		return (uint16)_data.ui32;
	case Variant_Int64:
		return (uint16)_data.i64;
	case Variant_UInt64:
		return (uint16)_data.ui64;
	case Variant_Real32:
		return (uint16)_data.r32;
	case Variant_Real64:
		return (uint16)_data.r64;
	case Variant_String:
		return ToString().ToUInt16();
	default:
		return 0;
	}
}

int32 Variant::ToInt32() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (int32)_data.b;
	case Variant_Int8:
		return (int32)_data.i8;
	case Variant_UInt8:
		return (int32)_data.ui8;
	case Variant_Int16:
		return (int32)_data.i16;
	case Variant_UInt16:
		return (int32)_data.ui16;
	case Variant_Int32:
		return _data.i32;
	case Variant_UInt32:
		return (int32)_data.ui32;
	case Variant_Int64:
		return (int32)_data.i64;
	case Variant_UInt64:
		return (int32)_data.ui64;
	case Variant_Real32:
		return (int32)_data.r32;
	case Variant_Real64:
		return (int32)_data.r64;
	case Variant_String:
		return ToString().ToInt32();
	default:
		return 0;
	}
}

uint32 Variant::ToUInt32() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (uint32)_data.b;
	case Variant_Int8:
		return (uint32)_data.i8;
	case Variant_UInt8:
		return (uint32)_data.ui8;
	case Variant_Int16:
		return (uint32)_data.i16;
	case Variant_UInt16:
		return (uint32)_data.ui16;
	case Variant_Int32:
		return (uint32)_data.i32;
	case Variant_UInt32:
		return _data.ui32;
	case Variant_Int64:
		return (uint32)_data.i64;
	case Variant_UInt64:
		return (uint32)_data.ui64;
	case Variant_Real32:
		return (uint32)_data.r32;
	case Variant_Real64:
		return (uint32)_data.r64;
	case Variant_String:
		return ToString().ToUInt32();
	default:
		return 0;
	}
}

int64 Variant::ToInt64() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (int64)_data.b;
	case Variant_Int8:
		return (int64)_data.i8;
	case Variant_UInt8:
		return (int64)_data.ui8;
	case Variant_Int16:
		return (int64)_data.i16;
	case Variant_UInt16:
		return (int64)_data.ui16;
	case Variant_Int32:
		return (int64)_data.i32;
	case Variant_UInt32:
		return (int64)_data.ui32;
	case Variant_Int64:
		return (int64)_data.i64;
	case Variant_UInt64:
		return (int64)_data.ui64;
	case Variant_Real32:
		return (int64)_data.r32;
	case Variant_Real64:
		return (int64)_data.r64;
	case Variant_String:
		return ToString().ToInt64();
	default:
		return 0;
	}
}

uint64 Variant::ToUInt64() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (uint64)_data.b;
	case Variant_Int8:
		return (uint64)_data.i8;
	case Variant_UInt8:
		return (uint64)_data.ui8;
	case Variant_Int16:
		return (uint64)_data.i16;
	case Variant_UInt16:
		return (uint64)_data.ui16;
	case Variant_Int32:
		return (uint64)_data.i32;
	case Variant_UInt32:
		return (uint64)_data.ui32;
	case Variant_Int64:
		return (uint64)_data.i64;
	case Variant_UInt64:
		return (uint64)_data.ui64;
	case Variant_Real32:
		return (uint64)_data.r32;
	case Variant_Real64:
		return (uint64)_data.r64;
	case Variant_String:
		return ToString().ToUInt64();
	default:
		return 0;
	}
}

real32 Variant::ToReal32() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (real32)_data.b;
	case Variant_Int8:
		return (real32)_data.i8;
	case Variant_UInt8:
		return (real32)_data.ui8;
	case Variant_Int16:
		return (real32)_data.i16;
	case Variant_UInt16:
		return (real32)_data.ui16;
	case Variant_Int32:
		return (real32)_data.i32;
	case Variant_UInt32:
		return (real64)_data.ui32;
	case Variant_Int64:
		return (real32)_data.i64;
	case Variant_UInt64:
		return (real32)_data.ui64;
	case Variant_Real32:
		return (real32)_data.r32;
	case Variant_Real64:
		return (real32)_data.r64;
	case Variant_String:
		return ToString().ToReal32();
	default:
		return 0.0f;
	}
}

real64 Variant::ToReal64() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return (real64)_data.b;
	case Variant_Int8:
		return (real64)_data.i8;
	case Variant_UInt8:
		return (real64)_data.ui8;
	case Variant_Int16:
		return (real64)_data.i16;
	case Variant_UInt16:
		return (real64)_data.ui16;
	case Variant_Int32:
		return (real64)_data.i32;
	case Variant_UInt32:
		return (real64)_data.ui32;
	case Variant_Int64:
		return (real64)_data.i64;
	case Variant_UInt64:
		return (real64)_data.ui64;
	case Variant_Real32:
		return (real64)_data.r32;
	case Variant_Real64:
		return (real64)_data.r64;
	case Variant_String:
		return ToString().ToReal64();
	default:
		return 0.0;
	}
}

String Variant::ToString() const
{
	switch (_type)
	{
	case Variant_Boolean:
		return String::ToString(ToBoolean());
	case Variant_Int8:
		return String::ToString(ToInt8());
	case Variant_UInt8:
		return String::ToString(ToUInt8());
	case Variant_Int16:
		return String::ToString(ToInt16());
	case Variant_UInt16:
		return String::ToString(ToUInt16());
	case Variant_Int32:
		return String::ToString(ToInt32());
	case Variant_UInt32:
		return String::ToString(ToUInt32());
	case Variant_Int64:
		return String::ToString(ToInt64());
	case Variant_UInt64:
		return String::ToString(ToUInt64());
	case Variant_Real32:
		return String::ToString(ToReal32());
	case Variant_Real64:
		return String::ToString(ToReal64());
	case Variant_String:
		return *((String*)_data.ptr);
	case Variant_Object:
		return String::Empty;//ToObject()->ToString();
	default:
		return String::Empty;
	}
}

StructObject* Variant::ToStruct() const
{
	if (_type == Variant_Struct)
		return (StructObject*)_data.ptr;
	else
		return NULL;
}

Object* Variant::ToObject() const
{
	if (_type == Variant_Object)
		return (Object*)_data.ptr;
	else
		return NULL;
}

int32 Variant::ToEnum() const
{
	if (_type == Variant_Enum)
		return _data.i32;
	else
		return 0;
}

SEbyte* Variant::ToArray() const
{
	if (_type == Variant_Array)
		return (SEbyte*)_data.ptr;
	else
		return NULL;
}

}
