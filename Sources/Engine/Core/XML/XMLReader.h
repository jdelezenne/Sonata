/*=============================================================================
XMLReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLREADER_H_
#define _SE_XMLREADER_H_

#include "Core/Common.h"
#include "Core/XML/XML.h"

namespace SonataEngine
{

/** Represents a reader that provides access to XML data. */
class SE_CORE_EXPORT XMLReader
{
public:
	XMLReader(const String& fileName);
	virtual ~XMLReader();

	/// Creates a new XMLReader instance.
	static XMLReader* Create(const String& fileName);

	XMLDocument* GetDocument() const { return _Document; }

private:
	void ReadNode(XMLNode* node, void* parent);

	XMLDocument* _Document;
};

}

#endif 
