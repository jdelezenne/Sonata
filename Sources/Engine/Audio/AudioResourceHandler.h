/*=============================================================================
AudioResourceHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIORESOURCEHANDLER_H_
#define _SE_AUDIORESOURCEHANDLER_H_

#include "Audio/Common.h"

namespace SonataEngine
{
	namespace Audio
	{
		/**
			@brief Audio resource handler.

			Provides methods for handling audio resources.
		*/
		class SE_AUDIO_EXPORT AudioResourceHandler : public ResourceHandler
		{
		public:
			AudioResourceHandler();
			virtual ~AudioResourceHandler();

			virtual bool CanHandle(const SE_ID& type);

			virtual Resource* Load(const String& name, const SE_ID& type, const String& path, Stream& stream);

			virtual bool Save(Resource* resource, const String& path, Stream& stream);

			virtual bool Unload(Resource* resource);
		};
	}
}

#endif
