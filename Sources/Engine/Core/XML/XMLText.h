/*=============================================================================
XMLText.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLTEXT_H_
#define _SE_XMLTEXT_H_

#include "Core/Common.h"
#include "Core/XML/XMLCharacterData.h"

namespace SonataEngine
{

/** Represents an XML text. */
class SE_CORE_EXPORT XMLText : public XMLCharacterData
{
public:
	virtual ~XMLText();

	virtual String GetName() const;

	virtual XMLNodeType GetNodeType() const;

private:
	XMLText();
	XMLText(const String& text, XMLDocument* document);

	friend class XMLDocument;
	friend class XMLReader;
	friend class XMLWriter;
};

}

#endif 
