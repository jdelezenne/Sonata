/*=============================================================================
IMemberInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMEMBERINFO_H_
#define _SE_IMEMBERINFO_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Containers/Array.h"
#include "Core/Reflection/Attribute.h"

namespace SonataEngine
{

class TypeInfo;

enum MemberTypes
{
	MemberTypes_Field = (1<<0),
	MemberTypes_Method = (1<<1),
	MemberTypes_Property = (1<<2),
	MemberTypes_Event = (1<<3),
	MemberTypes_Type = (1<<4)
};

typedef Array<const Attribute*> AttributeList;

/** Provides access to member metadata. */
class SE_CORE_EXPORT IMemberInfo
{
public:
	/** Gets a MemberTypes value indicating the type of the member. */
	virtual MemberTypes GetMemberType() const = 0;

	/** Gets the name of the member. */
	virtual String GetName() const = 0;

	/** Gets the class that declares this member. */
	virtual TypeInfo* GetDeclaringType() const = 0;

	/** Returns all the attributes of the member. */
	virtual const AttributeList& GetAttributes() const = 0;
};

}

#endif 
