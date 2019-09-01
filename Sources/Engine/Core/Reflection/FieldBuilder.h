/*=============================================================================
FieldBuilder.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_FIELDBUILDER_H_
#define _SE_FIELDBUILDER_H_

#include "Core/Common.h"
#include "Core/Reflection/FieldInfo.h"

namespace SonataEngine
{

/** Defines a field metadata. */
class SE_CORE_EXPORT FieldBuilder : public FieldInfo
{
public:
	/** Destructor. */
	virtual ~FieldBuilder();

	/** Sets all the attributes of the member. */
	void SetAttributes(const AttributeList& attributes);

protected:
	/** Constructor. */
	FieldBuilder(const String& fieldName, TypeInfo* fieldType, FieldAttributes attributes, int offset);

	friend class TypeBuilder;
	friend class EnumBuilder;
};

}

#endif 
