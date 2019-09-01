/*=============================================================================
XMLDocument.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLDOCUMENT_H_
#define _SE_XMLDOCUMENT_H_

#include "Core/Common.h"
#include "Core/XML/XMLNode.h"
#include "Core/String.h"

namespace SonataEngine
{

class XMLElement;
class XMLAttribute;
class XMLText;
class XMLComment;

/** Represents an XML document. */
class SE_CORE_EXPORT XMLDocument : public XMLNode
{
public:
	XMLDocument();
	virtual ~XMLDocument();

	virtual String GetName() const;

	virtual XMLNodeType GetNodeType() const;

	/** Gets the root XMLElement for the document. */
	XMLElement* GetDocumentElement() const;

	/** Creates an XMLNode. */
	XMLNode* CreateNode(XMLNodeType type, const String& name);

	/** Creates an XMLElement with the specified name. */
	XMLElement* CreateElement(const String& name);

	/** Creates an XMLAttribute with the specified name. */
	XMLAttribute* CreateAttribute(const String& name);

	/** Creates an XMLText with the specified text. */
	XMLText* CreateTextNode(const String& text);

	/** Creates an XMLComment containing the specified data. */
	XMLComment* CreateComment(const String& comment);
};

}

#endif 
