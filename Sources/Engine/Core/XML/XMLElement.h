/*=============================================================================
XMLElement.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLELEMENT_H_
#define _SE_XMLELEMENT_H_

#include "Core/Common.h"
#include "Core/XML/XMLNode.h"
#include "Core/String.h"

namespace SonataEngine
{

/** Represents an XML element. */
class SE_CORE_EXPORT XMLElement : public XMLNode
{
public:
	virtual ~XMLElement();

	virtual String GetName() const;

	virtual XMLNodeType GetNodeType() const;

	virtual const XMLAttributeList GetAttributes() const;

	/// Gets a boolean value indicating whether the current node has any attributes.
	virtual bool HasAttributes() const;

	/// Determines whether the current node has the specified attribute.
	virtual bool HasAttribute(const String& name) const;

	/// Returns the attribute with the specified name.
	virtual XMLAttribute* GetAttributeNode(const String& name);

	/// Returns the value for the attribute with the specified name.
	virtual String GetAttribute(const String& name);

	/// Adds the specified attribute.
	virtual void SetAttributeNode(XMLAttribute* attribute);

	/// Sets the value of the attribute with the specified name.
	virtual void SetAttribute(const String& name, const String& value);

	/// Removes the specified attribute.
	virtual XMLAttribute* RemoveAttributeNode(XMLAttribute* attribute);

	/// Removes all specified attributes from the element.
	virtual void RemoveAllAttributes();

	/// Removes an attribute by name.
	virtual void RemoveAttribute(const String& name);

	/// Removes the attribute node with the specified index from the element.
	virtual XMLNode* RemoveAttributeAt(int32 index);

protected:
	String _name;
	XMLAttributeList _Attributes;

private:
	XMLElement();
	XMLElement(const String& name, XMLDocument* document);

	friend class XMLDocument;
	friend class XMLReader;
	friend class XMLWriter;
};

}

#endif 
