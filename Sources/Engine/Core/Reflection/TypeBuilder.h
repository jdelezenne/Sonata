/*=============================================================================
TypeBuilder.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TYPEBUILDER_H_
#define _SE_TYPEBUILDER_H_

#include "Core/Common.h"
#include "Core/Reflection/TypeFactory.h"
#include "Core/Reflection/TypeInfo.h"
#include "Core/Reflection/FieldInfo.h"
#include "Core/Reflection/MethodInfo.h"

namespace SonataEngine
{

/** Defines and creates new instances of classes. */
class SE_CORE_EXPORT TypeBuilder
{
public:
	TypeBuilder(const String& name);

	FieldInfo* DefineField(const String& fieldName, const String& typeName, FieldAttributes attributes, int offset);

	MethodInfo* DefineMethod(const String& methodName, const String& returnTypeName, const ParameterList& parameters, MethodAttributes attributes, int offset);

	MethodInfo* DefineMethod(const String& methodName, const String& returnTypeName, const String& parameters, MethodAttributes attributes, int offset);

	Attribute* SetAttribute(Attribute* attribute);

protected:
	TypeInfo* _Type;
	AttributeList _Attributes;
};

}

#endif 
