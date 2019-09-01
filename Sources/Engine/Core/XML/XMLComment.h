/*=============================================================================
XMLComment.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLCOMMENT_H_
#define _SE_XMLCOMMENT_H_

#include "Core/Common.h"
#include "Core/XML/XMLCharacterData.h"

namespace SonataEngine
{

/** Represents an XML element. */
class SE_CORE_EXPORT XMLComment : public XMLCharacterData
{
public:
	virtual ~XMLComment();

	virtual String GetName() const;

	virtual XMLNodeType GetNodeType() const;

private:
	XMLComment();
	XMLComment(const String& comment, XMLDocument* document);

	friend class XMLDocument;
	friend class XMLReader;
	friend class XMLWriter;
};

}

#endif 
