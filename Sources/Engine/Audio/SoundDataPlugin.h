/*=============================================================================
SoundDataPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_SOUNDDATAPLUGIN_H_
#define _SE_AUDIO_SOUNDDATAPLUGIN_H_

#include "Core/Core.h"
#include "Audio/Common.h"

#define SE_ID_DATA_SOUND SonataEngine::SE_ID(0xf5198962,0x83724c04)

namespace SonataEngine
{
	namespace Audio
	{
		class SoundReader;
		class SoundWriter;

		/**
			@brief Base class for sound plugins.

			This class must be implemented to read/write sounds.
		*/
		class SE_AUDIO_EXPORT SoundDataPlugin : public DataPlugin
		{
		public:
			virtual SonataEngine::SE_ID GetDataType() const { return SE_ID_DATA_SOUND; }

			/** @name Operations. */
			//@{
			/**
				Creates a sound reader.
				@return A sound reader if successful, NULL otherwise.
				@see CreateReader
			*/
			virtual SoundReader* CreateReader() = 0;

			/**
				Creates a sound writer.
				@return A sound writer if successful, NULL otherwise.
				@see CreateWriter
			*/
			virtual SoundWriter* CreateWriter() = 0;

			/**
				Destroys a sound reader.
				@param reader An sound reader to destroy.
				@see CreateReader
			*/
			virtual void DestroyReader(SoundReader* reader) = 0;

			/**
				Creates a sound writer.
				@writer A sound writer to destroy.
				@see CreateWriter
			*/
			virtual void DestroyWriter(SoundWriter* writer) = 0;
			//@}
		};
	}
}

#endif 
