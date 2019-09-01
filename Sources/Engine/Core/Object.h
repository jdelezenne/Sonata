/*=============================================================================
Object.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_OBJECT_H_
#define _SE_OBJECT_H_

#include "Core/Common.h"
#include "Core/RefCounter.h"
#include "Core/SmartPtr.h"
#include "Core/String.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Serialization/ISerializable.h"

namespace SonataEngine
{

/**
	@brief Base object.

	As in .NET and Java, most classes derive from the Object class.
	This class provides some basic functionalities such as serialization and
	reflection.

	Derived classes have to implement the following methods:
	@li @c Clone - Creates a shallow copy of the current Object.
	@li @c Equals - Supports comparisons between objects.
	@li @c GetHashCode - Serves as a hash function for a particular type.

	Derived classes can override the following methods:
	@li @c ToString - Returns a String that represents the current Object.
	@li @c Deserialize - Deserializes the current Object.
	@li @c Serialize - Serializes the current Object.
*/
class SE_CORE_EXPORT Object : public ISerializable
{
	SE_DECLARE_OBJECT(Object);

public:
	/** Destructor. */
	virtual ~Object();

	/**
		Creates a shallow copy of the current Object.
		@return A shallow copy of the current Object.
	*/
	virtual Object* Clone() const { return NULL; }

	/**
		Determines whether the specified Object is equal to the current Object.
		@param obj The Object to compare with the current Object.
	*/
	virtual bool Equals(const Object& obj) const { return false; }

	/**
		Serves as a hash function for a particular type.
		@return A hash code for the current Object.
	*/
	virtual int32 GetHashCode() const { return 0; }

	/**
		Determines whether the specified Object instances are considered equal.
		@param left The first Object to compare.
		@param right The second Object to compare.
	*/
	static bool Equals(const Object& left, const Object& right);

	bool operator==(const Object& obj) const;
	bool operator!=(const Object& obj) const;

	virtual String ToString() const;

	virtual void Serialize(BinarySerializer* context, BinaryStream& stream) {}
	virtual void Serialize(XMLSerializer* context, XMLElement* element) {}
	virtual void Deserialize(BinarySerializer* context, BinaryStream& stream) {}
	virtual void Deserialize(XMLSerializer* context, XMLElement* element) {}
	virtual void OnSerialized(BinarySerializer* context, BinaryStream& stream) {}
	virtual void OnSerialized(XMLSerializer* context, XMLElement* element) {}
	virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream) {}
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element) {}

protected:
	/** Protected constructor. */
	Object();
};


/** Base class for reference counted objects. */
class SE_CORE_EXPORT RefObject : public Object, public RefCounter
{
public:
	/** Destructor. */
	virtual ~RefObject();

protected:
	/** Protected constructor. */
	RefObject();
};


/** Base class for named reference counted objects. */
class SE_CORE_EXPORT NamedObject : public RefObject
{
	SE_DECLARE_ABSTRACT(NamedObject, Object);
	SE_BEGIN_REFLECTION(NamedObject);
		SE_Field(_name, String, Public);
	SE_END_REFLECTION(NamedObject);

protected:
	String _name;

public:
	/** Destructor. */
	virtual ~NamedObject();

	/** Create a hash value from a string. */
	static uint32 HashString(const String& str);

	/**
		Retrieves the name of the object.
		@return Name of the mesh.
	*/
	String GetName() const { return _name; }

	/**
		Sets the name of the object.
		@param value Name of the object.
	*/
	void SetName(const String& value) { _name = value; }

protected:
	/** Protected constructor. */
	NamedObject();
};


/** Provides the base class for structures. */
class SE_CORE_EXPORT StructObject : public Object
{
	SE_DECLARE_ABSTRACT(StructObject, Object);

public:
	StructObject() : Object() {}
	virtual void Get(void* value) = 0;
	virtual void Set(void* value) = 0;
};


/** Provides the base class for enumerations. */
class SE_CORE_EXPORT EnumObject : public Object
{
public:
	SE_DECLARE_ABSTRACT(EnumObject, Object);

	/** Destructor. */
	virtual ~EnumObject();

	/** Retrieves an array of the names of the constants in a specified enumeration. */
	static Array<String> GetNames(TypeInfo* enumType);

	/** Retrieves an array of the values of the constants in a specified enumeration. */
	static Array<Variant> GetValues(TypeInfo* enumType);

	/** Retrieves the name of the constant in the specified enumeration that has the specified value. */
	static String GetName(TypeInfo* enumType, const Variant& value);

	/** Retrieves the value of the constant in the specified enumeration that has the specified name. */
	static Variant GetValue(TypeInfo* enumType, const String& name);

protected:
	/** Constructor. */
	EnumObject();
};

#include "Object.inl"

}

#endif
