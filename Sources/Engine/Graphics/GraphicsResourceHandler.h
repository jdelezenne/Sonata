/*=============================================================================
GraphicsResourceHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GRAPHICSRESOURCEHANDLER_H_
#define _SE_GRAPHICSRESOURCEHANDLER_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	@brief Graphics resource handler.

	Provides the handling of graphical resources.
*/
class SE_GRAPHICS_EXPORT GraphicsResourceHandler : public ResourceHandler
{
public:
	GraphicsResourceHandler();
	virtual ~GraphicsResourceHandler();

	virtual bool CanHandle(const SE_ID& type);

	virtual Resource* Load(const String& name, const SE_ID& type, const String& path, Stream& stream);

	virtual bool Save(Resource* resource, const String& path, Stream& stream);

	virtual bool Unload(Resource* resource);
};

}

#endif
