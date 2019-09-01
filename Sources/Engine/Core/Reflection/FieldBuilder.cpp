/*=============================================================================
FieldBuilder.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "FieldBuilder.h"

namespace SonataEngine
{

FieldBuilder::FieldBuilder(const String& fieldName, TypeInfo* fieldType, FieldAttributes attributes, int offset) :
	FieldInfo()
{
	_FieldName = fieldName;
	_FieldType = fieldType;
	_FieldAttributes = attributes;
	_Offset = offset;
}

FieldBuilder::~FieldBuilder()
{
}

void FieldBuilder::SetAttributes(const AttributeList& attributes)
{
	_Attributes = attributes;
}

}
