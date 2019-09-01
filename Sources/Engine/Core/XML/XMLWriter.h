/*=============================================================================
XMLWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLWRITER_H_
#define _SE_XMLWRITER_H_

#include "Core/Common.h"
#include "Core/XML/XML.h"

namespace SonataEngine
{

/** Represents a writer that provides writing to XML data. */
class SE_CORE_EXPORT XMLWriter
{
public:
	XMLWriter(XMLDocument* document, const String& fileName);
	virtual ~XMLWriter();

	/// Creates a new XMLWriter instance.
	static XMLWriter* Create(XMLDocument* document, const String& fileName);

private:
	void WriteNode(XMLNode* node, void* parent);
};

}

#endif 
