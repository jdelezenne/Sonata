/*=============================================================================
XMLComment.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLComment.h"

namespace SonataEngine
{

XMLComment::XMLComment() :
	XMLCharacterData()
{
}

XMLComment::XMLComment(const String& comment, XMLDocument* document) :
	XMLCharacterData()
{
	_Value = comment;
	_OwnerDocument = document;
}

XMLComment::~XMLComment()
{
}

String XMLComment::GetName() const
{
	return _T("#comment");
}

XMLNodeType XMLComment::GetNodeType() const
{
	return XMLNodeType_Comment;
}

}
