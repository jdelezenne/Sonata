/*=============================================================================
XMLAttribute.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLATTRIBUTE_H_
#define _SE_XMLATTRIBUTE_H_

#include "Core/Common.h"
#include "Core/XML/XMLNode.h"
#include "Core/String.h"

namespace SonataEngine
{

/** Represents an XML element. */
class SE_CORE_EXPORT XMLAttribute : public XMLNode
{
public:
	virtual ~XMLAttribute();

	virtual String GetName() const;

	virtual String GetValue() const;

	virtual void SetValue(const String& value);

	virtual XMLNodeType GetNodeType() const;

protected:
	String _name;
	String _Value;

private:
	XMLAttribute();
	XMLAttribute(const String& name, XMLDocument* document);

	friend class XMLDocument;
	friend class XMLElement;
	friend class XMLReader;
	friend class XMLWriter;
};

}

#endif 
