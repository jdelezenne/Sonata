/*=============================================================================
MethodBuilder.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "MethodBuilder.h"

namespace SonataEngine
{

MethodBuilder::MethodBuilder(const String& methodName, TypeInfo* returnType, const ParameterList& parameters, MethodAttributes attributes, int offset) :
	MethodInfo()
{
	_MethodName = methodName;
	_ReturnType = returnType;
	_Parameters = parameters;
	_MethodAttributes = attributes;
	_Offset = offset;
}

MethodBuilder::~MethodBuilder()
{
}

void MethodBuilder::SetAttributes(const AttributeList& attributes)
{
	_Attributes = attributes;
}

void MethodBuilder::SetReturnType(TypeInfo* type)
{
	_ReturnType = type;
}

void MethodBuilder::SetParameters(const ParameterList& parameters)
{
	_Parameters = parameters;
}

ParameterInfo* MethodBuilder::DefineParameter(const String& paramName, const String& paramTypeName, ParameterAttributes attributes, int position)
{
	ParameterInfo* pi = new ParameterInfo(paramName, paramTypeName, attributes, position);
	_Parameters.Add(pi);

	return pi;
}

}
