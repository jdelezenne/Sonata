/*=============================================================================
XMLAttribute.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLAttribute.h"

namespace SonataEngine
{

XMLAttribute::XMLAttribute() :
	XMLNode()
{
}

XMLAttribute::XMLAttribute(const String& name, XMLDocument* document) :
	XMLNode(),
	_name(name)
{
	_OwnerDocument = document;
}

XMLAttribute::~XMLAttribute()
{
}

String XMLAttribute::GetName() const
{
	return _name;
}

String XMLAttribute::GetValue() const
{
	return _Value;
}

void XMLAttribute::SetValue(const String& value)
{
	_Value = value;
}

XMLNodeType XMLAttribute::GetNodeType() const
{
	return XMLNodeType_Attribute;
}

}
