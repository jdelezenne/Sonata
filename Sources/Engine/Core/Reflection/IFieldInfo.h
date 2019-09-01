/*=============================================================================
IFieldInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IFIELDINFO_H_
#define _SE_IFIELDINFO_H_

#include "Core/Common.h"
#include "Core/System/Memory.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "Core/Reflection/TypeInfo.h"

namespace SonataEngine
{

class TypeInfo;
class Object;

enum FieldAttributes
{
	FieldAttributes_Private = (1<<0),
	FieldAttributes_Public = (1<<1),
	FieldAttributes_Static = (1<<2),
	FieldAttributes_HasDefault = (1<<3)
};

/** Provides access to field metadata. */
class SE_CORE_EXPORT IFieldInfo
{
public:
	/** Gets the attributes associated with this field. */
	virtual FieldAttributes GetFieldAttributes() const = 0;

	/** Gets the type of this field object. */
	virtual TypeInfo* GetFieldType() const = 0;

	/** Retrieves the value of the field supported by the given object. */
	virtual Variant GetValue(Object* obj) const = 0;

	/** Sets the value of the field supported by the given object. */
	virtual void SetValue(Object* obj, const Variant& value) const = 0;
};

}

#endif 
