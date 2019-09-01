/*=============================================================================
MethodBuilder.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_METHODBUILDER_H_
#define _SE_METHODBUILDER_H_

#include "Core/Common.h"
#include "Core/Reflection/MethodInfo.h"
#include "Core/Reflection/ParameterInfo.h"

namespace SonataEngine
{

/** Defines a method metadata. */
class SE_CORE_EXPORT MethodBuilder : public MethodInfo
{
public:
	/** Destructor. */
	virtual ~MethodBuilder();

	/** Sets all the attributes of the member. */
	void SetAttributes(const AttributeList& attributes);

	/** Sets the return type of this method. */
	void SetReturnType(TypeInfo* type);

	/** Sets the parameters of this method. */
	void SetParameters(const ParameterList& parameters);

	/** Defines a parameter of this method. */
	ParameterInfo* DefineParameter(const String& paramName, const String& paramTypeName, ParameterAttributes attributes, int position);

protected:
	/** Constructor. */
	MethodBuilder(const String& methodName, TypeInfo* returnType, const ParameterList& parameters, MethodAttributes attributes, int offset);

	friend class TypeBuilder;
};

}

#endif 
