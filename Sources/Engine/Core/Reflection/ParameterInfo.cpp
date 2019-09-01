/*=============================================================================
ParameterInfo.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "ParameterInfo.h"
#include "Core/Object.h"

namespace SonataEngine
{

ParameterInfo::ParameterInfo() :
	_ParameterType(NULL),
	_ParameterAttributes(ParameterAttributes_None),
	_Position(0)
{
}

ParameterInfo::ParameterInfo(const String& paramName, const String& paramTypeName, ParameterAttributes attributes, int position) :
	_ParameterName(paramName),
	_ParameterTypeName(paramTypeName),
	_ParameterAttributes(attributes),
	_Position(position)
{
	_ParameterType = TypeFactory::Instance()->GetType((paramTypeName));
}

}
