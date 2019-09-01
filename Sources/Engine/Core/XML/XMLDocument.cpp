/*=============================================================================
XMLDocument.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLDocument.h"
#include "Core/XML/XMLElement.h"
#include "Core/XML/XMLAttribute.h"
#include "Core/XML/XMLText.h"
#include "Core/XML/XMLComment.h"

namespace SonataEngine
{

XMLDocument::XMLDocument() :
	XMLNode()
{
	_OwnerDocument = this;
}

XMLDocument::~XMLDocument()
{
}

String XMLDocument::GetName() const
{
	return _T("#document");
}

XMLNodeType XMLDocument::GetNodeType() const
{
	return XMLNodeType_Document;
}

XMLElement* XMLDocument::GetDocumentElement() const
{
	return (XMLElement*)FindChild(XMLNodeType_Element);
}

XMLNode* XMLDocument::CreateNode(XMLNodeType type, const String& name)
{
	switch (type)
	{
	case XMLNodeType_Element:
		return CreateElement(name);
	case XMLNodeType_Attribute:
		return CreateAttribute(name);
	case XMLNodeType_Text:
		return CreateTextNode(name);
	case XMLNodeType_Comment:
		return CreateComment(name);
	default:
		return NULL;
	}
}

XMLElement* XMLDocument::CreateElement(const String& name)
{
	return new XMLElement(name, this);
}

XMLAttribute* XMLDocument::CreateAttribute(const String& name)
{
	return new XMLAttribute(name, this);
}

XMLText* XMLDocument::CreateTextNode(const String& text)
{
	return new XMLText(text, this);
}

XMLComment* XMLDocument::CreateComment(const String& comment)
{
	return new XMLComment(comment, this);
}

}
