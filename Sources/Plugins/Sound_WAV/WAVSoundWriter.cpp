/*=============================================================================
WAVSoundWriter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "WAVSoundWriter.h"

namespace SE_WAV
{

WAVSoundWriter::WAVSoundWriter() :
	SoundWriter()
{
}

WAVSoundWriter::~WAVSoundWriter()
{
}

bool WAVSoundWriter::OpenSound(Stream& stream, SoundWriterOptions* options)
{
	return false;
}

bool WAVSoundWriter::CloseSound()
{
	return false;
}

int32 WAVSoundWriter::WriteSound(SEbyte* buffer, uint32 count)
{
	return 0;
}

}
