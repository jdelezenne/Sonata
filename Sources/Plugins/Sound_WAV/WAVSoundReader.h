/*=============================================================================
WAVSoundReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WAVSOUNDREADER_H_
#define _SE_WAVSOUNDREADER_H_

#include "WAVSoundCommon.h"

namespace SE_WAV
{

class WAVSoundReader : public SoundReader
{
public:
	WAVSoundReader();
	virtual ~WAVSoundReader();

	virtual bool OpenSound(Stream& stream, SoundReaderOptions* options = NULL);

	virtual bool CloseSound();

	virtual bool GetFormat(AudioFormat& format);

	virtual Stream* GetStream();

	virtual int32 GetLength();

	virtual int32 GetPosition();

	virtual bool SetPosition(int32 value);

	virtual int32 ReadSound(SEbyte* buffer, int32 count);

private:
	AudioFormat _Format;
	Stream* _Stream;
	int32 _DataOffset;
	int32 _DataSize;
	int32 _RemainingSize;
};

}

#endif 
