/*=============================================================================
BinarySerializer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BINARYSERIALIZER_H_
#define _SE_BINARYSERIALIZER_H_

#include "Core/Common.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/IO/BinaryStream.h"

namespace SonataEngine
{

/**
	@brief Binary serializer.
*/
class SE_CORE_EXPORT BinarySerializer : public ISerializer
{
public:
	/** @name Constructor / Destructor. */
	//@{
	/** Constructor. */
	BinarySerializer();

	/** Destructor. */
	virtual ~BinarySerializer();
	//@}

	/** Serialize this object. */
	virtual void Serialize(Stream* stream, Object* obj);
	void Serialize(BinaryStream& writer, Object* obj, TypeInfo* type);

	/** Deserialize this object. */
	virtual Object* Deserialize(Stream* stream);
	Object* Deserialize(BinaryStream& reader);
};

}

#endif 
