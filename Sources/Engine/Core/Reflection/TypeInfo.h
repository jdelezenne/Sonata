/*=============================================================================
TypeInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TYPEINFO_H_
#define _SE_TYPEINFO_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Containers/Array.h"
#include "Core/Reflection/MemberInfo.h"
#include "Core/Reflection/TypeFactory.h"

namespace SonataEngine
{

class Object;
class FieldInfo;
class MethodInfo;
class TypeBuilder;

typedef Object* (*ObjectCreator) ();

typedef Array<MemberInfo*> MemberList;
typedef Array<FieldInfo*> FieldList;
typedef Array<MethodInfo*> MethodList;

/**
	@brief Represents type layouts.
*/
class SE_CORE_EXPORT TypeInfo : public MemberInfo
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	TypeInfo(const String& typeName);
	TypeInfo(const String& typeName, const String& superTypeName, ObjectCreator creator);

	/** Destructor. */
	virtual ~TypeInfo();
	//@}

	virtual MemberTypes GetMemberType() const;

	/** Creates an instance of the Type. */
	Object* Create() const;

	/** Gets the name of the Type. */
	virtual String GetName() const;

	/** Gets the SE_ID associated with the Type. */
	//SE_ID GetSE_ID() const;

	/** Gets the type from which the Type directly inherits. */
	TypeInfo* GetBaseType() const;

	/** Returns whether the Type is abstract and must be overridden. */
	bool IsAbstract() const;

	/** Returns whether the Type is one of the primitive types. */
	bool IsPrimitive() const;

	/** Returns whether the current Type represents a class. */
	bool IsClass() const;

	/** Returns whether the Type is a structure. */
	bool IsStruct() const;

	/** Returns whether the current Type represents an enumeration. */
	bool IsEnum() const;

	/** Returns whether the current Type represents an array. */
	bool IsArray() const;

	/**
		Determines whether the class represented by the Type derives from the
		class represented by the specified Type.
	*/
	bool IsSubclassOf(TypeInfo* type) const;

	/** Determines whether the specified object is an instance of the Type. */
	bool IsInstanceOfType(Object* obj) const;

	/** Gets a Type by its name. */
	static TypeInfo* GetType(const String& typeName);

	/** Returns all the members of the Type. */
	MemberList GetMembers() const;

	/** Searches for the members with the specified name. */
	MemberList GetMember(const String& name) const;

	/** Searches for the members with the specified name and type. */
	MemberList GetMember(const String& name, MemberTypes types) const;

	/** Returns all the fields of the Type. */
	FieldList GetFields() const;

	/** Searches for the field with the specified name. */
	const FieldInfo* GetField(const String& name) const;

	/** Returns all the methods of the Type. */
	MethodList GetMethods() const;

	/** Searches for the method with the specified name. */
	const MethodInfo* GetMethod(const String& name) const;

protected:
	String _name;
	String _SuperTypeName;
	mutable TypeInfo* _SuperType;
	const ObjectCreator _Creator;
	bool _Primitive;
	FieldList _Fields;
	MethodList _Methods;

	friend class TypeBuilder;
	friend class EnumBuilder;
};

#include "TypeInfo.inl"

}

#endif 
