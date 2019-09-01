/*=============================================================================
XMLText.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLText.h"

namespace SonataEngine
{

XMLText::XMLText() :
	XMLCharacterData()
{
}

XMLText::XMLText(const String& text, XMLDocument* document) :
	XMLCharacterData()
{
	_Value = text;
	_OwnerDocument = document;
}

XMLText::~XMLText()
{
}

String XMLText::GetName() const
{
	return _T("#text");
}

XMLNodeType XMLText::GetNodeType() const
{
	return XMLNodeType_Text;
}

}
