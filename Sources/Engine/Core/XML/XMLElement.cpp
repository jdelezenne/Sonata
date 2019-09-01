/*=============================================================================
XMLElement.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLElement.h"
#include "Core/XML/XMLAttribute.h"
#include "Core/XML/XMLDocument.h"

namespace SonataEngine
{

XMLElement::XMLElement() :
	XMLNode()
{
}

XMLElement::XMLElement(const String& name, XMLDocument* document) :
	XMLNode()
{
	_name = name;
	_OwnerDocument = document;
}

XMLElement::~XMLElement()
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		delete attribute;
	}
}

String XMLElement::GetName() const
{
	return _name;
}

XMLNodeType XMLElement::GetNodeType() const
{
	return XMLNodeType_Element;
}

const XMLNode::XMLAttributeList XMLElement::GetAttributes() const
{
	return _Attributes;
}

bool XMLElement::HasAttributes() const
{
	return !_Attributes.IsEmpty();
}

bool XMLElement::HasAttribute(const String& name) const
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		if (attribute->GetName() == name)
			return true;
	}

	return false;
}

XMLAttribute* XMLElement::GetAttributeNode(const String& name)
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		if (attribute->GetName() == name)
			return attribute;
	}

	return NULL;
}

String XMLElement::GetAttribute(const String& name)
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		if (attribute->GetName() == name)
			return attribute->GetValue();
	}

	return String::Empty;
}

void XMLElement::SetAttributeNode(XMLAttribute* attribute)
{
	if (attribute->GetParentNode() == NULL)
	{
		attribute->_Parent = this;
		_Attributes.Add(attribute);
	}
}

void XMLElement::SetAttribute(const String& name, const String& value)
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		if (String::Compare(attribute->_name, name, true) == 0)
		{
			attribute->_Value = value;
			return;
		}
	}

	attribute = _OwnerDocument->CreateAttribute(name);
	attribute->_Value = value;
	_Attributes.Add(attribute);
}

XMLAttribute* XMLElement::RemoveAttributeNode(XMLAttribute* attribute)
{
	XMLAttribute* attr;
	foreach (attr, _Attributes, XMLNode::XMLAttributeList)
	{
		if (attr == attribute)
		{
			_Attributes.Remove(attr);
			attr->_Parent = NULL;
			return attr;
		}
	}

	return NULL;
}

void XMLElement::RemoveAllAttributes()
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		attribute->_Parent = NULL;
	}

	_Attributes.Clear();
}

void XMLElement::RemoveAttribute(const String& name)
{
	XMLAttribute* attribute;
	foreach (attribute, _Attributes, XMLNode::XMLAttributeList)
	{
		if (attribute->GetName() == name)
		{
			_Attributes.Remove(attribute);
			attribute->_Parent = NULL;
		}
	}
}

XMLNode* XMLElement::RemoveAttributeAt(int32 index)
{
	XMLAttribute* attribute = _Attributes.GetItem(index);
	if (attribute != NULL)
	{
		_Attributes.Remove(attribute);
		attribute->_Parent = NULL;
	}

	return attribute;
}

}
