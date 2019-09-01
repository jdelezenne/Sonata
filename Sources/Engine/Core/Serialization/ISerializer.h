/*=============================================================================
ISerializer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ISERIALIZER_H_
#define _SE_ISERIALIZER_H_

#include "Core/Common.h"
#include "Core/Object.h"
#include "Core/Variant.h"
#include "Core/Containers/BaseArray.h"
#include "Core/IO/Stream.h"
#include "Core/Reflection/MemberInfo.h"

namespace SonataEngine
{

/**
	@brief Base class for serializer implementations.
*/
class SE_CORE_EXPORT ISerializer
{
public:
	/** Serialize this object. */
	virtual void Serialize(Stream* stream, Object* obj) = 0;

	/** Deserialize this object. */
	virtual Object* Deserialize(Stream* stream) = 0;

	/** Gets all the serializable members for a class of the specified type. */
	static Array<MemberInfo*> GetSerializableMembers(TypeInfo* type);

	/** Extracts the values from the specified object. */
	static BaseArray<Variant> GetObjectValues(Object* obj, const Array<MemberInfo*>& members);

	/** Builds the specified object with values for each field. */
	static Object* BuildObjectMembers(Object* obj, const Array<MemberInfo*>& members, const BaseArray<Variant>& values);
};

}

#endif 
