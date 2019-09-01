/*=============================================================================
SoundReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_SOUNDREADER_H_
#define _SE_AUDIO_SOUNDREADER_H_

#include "Core/Core.h"
#include "Audio/Sound.h"

namespace SonataEngine
{
	namespace Audio
	{
		/** Sound reader options. */
		struct SoundReaderOptions
		{
		};

		/** Sound reader. */
		class SE_AUDIO_EXPORT SoundReader
		{
		public:
			/** Destructor. */
			virtual ~SoundReader() {}

			/** Opens a sound. */
			virtual bool OpenSound(Stream& stream, SoundReaderOptions* options = NULL) = 0;

			/** Closes a sound. */
			virtual bool CloseSound() = 0;

			/** Retrieves the format of the sound. */
			virtual bool GetFormat(AudioFormat& format) = 0;

			/** Retrieves the stream of the sound. */
			virtual Stream* GetStream() = 0;

			/** Retrieves the length of the sound. */
			virtual int32 GetLength() = 0;

			/** Retrieves the current position in the sound. */
			virtual int32 GetPosition() = 0;

			/** Sets the current position in the sound. */
			virtual bool SetPosition(int32 value) = 0;

			/**
				Reads a sound.
				@param buffer The buffer to read sound data to.
				@param count The number of bytes to read.
				@return The number of bytes actually read.
			*/
			virtual int32 ReadSound(SEbyte* buffer, int32 count) = 0;

		protected:
			/** Constructor. */
			SoundReader() {}
		};
	}
}

#endif 
