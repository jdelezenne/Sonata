/*=============================================================================
MethodInfo.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "MethodInfo.h"
#include "Core/Object.h"

namespace SonataEngine
{

MethodInfo::MethodInfo() :
	_ReturnType(NULL),
	_MethodAttributes(MethodAttributes_Private),
	_Offset(0)
{
}

MethodInfo::MethodInfo(const String& methodName, const String& returnTypeName, const ParameterList& parameters, MethodAttributes attributes, int offset) :
	_MethodName(methodName),
	_ReturnTypeName(returnTypeName),
	_Parameters(parameters),
	_MethodAttributes(attributes),
	_Offset(offset)
{
	_ReturnType = TypeFactory::Instance()->GetType((_ReturnTypeName));
}

MethodInfo::~MethodInfo()
{
}

Variant* MethodInfo::Invoke(Object* obj, Array<Variant>* parameters)
{
	return NULL;
}

}
