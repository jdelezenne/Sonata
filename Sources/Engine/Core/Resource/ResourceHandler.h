/*=============================================================================
ResourceHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RESOURCEHANDLER_H_
#define _SE_RESOURCEHANDLER_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/SE_ID.h"
#include "Core/Resource/Resource.h"
#include "Core/IO/BinaryStream.h"
#include "Core/IO/TextStream.h"

namespace SonataEngine
{

/** @brief Resource handler.
	Base class for resource handler implementations.
*/
class SE_CORE_EXPORT ResourceHandler
{
public:
	ResourceHandler();
	virtual ~ResourceHandler();

	virtual void Create();
	
	virtual void Destroy();

	virtual bool CanHandle(const SE_ID& type) = 0;

	virtual Resource* Load(const String& name, const SE_ID& type, const String& path, Stream& stream) = 0;

	virtual bool Save(Resource* resource, const String& path, Stream& stream) = 0;

	virtual bool Unload(Resource* resource) = 0;
};

}

#endif
