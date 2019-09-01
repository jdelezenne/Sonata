/*=============================================================================
WAVSoundWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WAVSOUNDWRITER_H_
#define _SE_WAVSOUNDWRITER_H_

#include "WAVSoundCommon.h"

namespace SE_WAV
{

class WAVSoundWriter : public SoundWriter
{
public:
	WAVSoundWriter();
	virtual ~WAVSoundWriter();

	virtual bool OpenSound(Stream& stream, SoundWriterOptions* options = NULL);

	virtual bool CloseSound();

	virtual int32 WriteSound(SEbyte* buffer, uint32 count);
};

}

#endif 
