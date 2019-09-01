/*=============================================================================
ISerializable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ISERIALIZABLE_H_
#define _SE_ISERIALIZABLE_H_

#include "Core/Common.h"
#include "Core/IO/BinaryStream.h"

namespace SonataEngine
{

class BinarySerializer;
class XMLSerializer;
class XMLElement;

/**
	@brief Interface for the serializable objects.

	Allows an object to control its own serialization and deserialization.
*/
class SE_CORE_EXPORT ISerializable
{
public:
	/** Serialize the object using an XML serializer. */
	virtual void Serialize(BinarySerializer* context, BinaryStream& stream) = 0;

	/** Serialize the object using an XML serializer. */
	virtual void Serialize(XMLSerializer* context, XMLElement* element) = 0;

	/** Deserialize the object using a binary serializer. */
	virtual void Deserialize(BinarySerializer* context, BinaryStream& stream) = 0;

	/** Deserialize the object using a binary serializer. */
	virtual void Deserialize(XMLSerializer* context, XMLElement* element) = 0;

	/** Method called after serialization of the object using a binary serializer. */
	virtual void OnSerialized(BinarySerializer* context, BinaryStream& stream) = 0;

	/** Method called after serialization of the object using an XML serializer. */
	virtual void OnSerialized(XMLSerializer* context, XMLElement* element) = 0;

	/** Method called after deserialization of the object using a binary serializer. */
	virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream) = 0;

	/** Method called after deserialization of the object using an XML serializer. */
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element) = 0;
};

}

#endif 
