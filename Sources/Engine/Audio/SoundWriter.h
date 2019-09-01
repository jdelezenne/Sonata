/*=============================================================================
SoundWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_SOUNDWRITER_H_
#define _SE_AUDIO_SOUNDWRITER_H_

#include "Core/Core.h"
#include "Audio/Sound.h"

namespace SonataEngine
{
	namespace Audio
	{
		/** Sound writer options. */
		struct SoundWriterOptions
		{
			AudioFormat Format;
		};

		/** Sound writer. */
		class SE_AUDIO_EXPORT SoundWriter
		{
		public:
			/** Destructor. */
			virtual ~SoundWriter() {}

			/** Opens a sound. */
			virtual bool OpenSound(Stream& stream, SoundWriterOptions* options = NULL) = 0;

			/** Closes a sound. */
			virtual bool CloseSound() = 0;

			/**
				Writes a sound.
				@param buffer The buffer to write sound data from.
				@param count The number of bytes to write.
				@return The number of bytes actually written.
			*/
			virtual int32 WriteSound(SEbyte* buffer, uint32 count) = 0;

		protected:
			/** Constructor. */
			SoundWriter() {}
		};
	}
}

#endif 
