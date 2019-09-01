/*=============================================================================
XMLSerializer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLSERIALIZER_H_
#define _SE_XMLSERIALIZER_H_

#include "Core/Common.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/XML/XMLDocument.h"

namespace SonataEngine
{

/**
	@brief XML serializer.
*/
class SE_CORE_EXPORT XMLSerializer : public ISerializer
{
public:
	/** @name Constructor / Destructor. */
	//@{
	/** Constructor. */
	XMLSerializer();

	/** Destructor. */
	virtual ~XMLSerializer();
	//@}

	/** Serialize this object. */
	virtual void Serialize(Stream* stream, Object* obj);
	void Serialize(XMLDocument* document, XMLNode* parentNode, Object* obj, TypeInfo* type);

	/** Deserialize this object. */
	virtual Object* Deserialize(Stream* stream);
	Object* Deserialize(XMLDocument* document, XMLNode* typeElement);
};

}

#endif 
