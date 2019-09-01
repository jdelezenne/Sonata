/*=============================================================================
WAVSoundReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "WAVSoundReader.h"

namespace SE_WAV
{

WAVSoundReader::WAVSoundReader() :
	SoundReader()
{
}

WAVSoundReader::~WAVSoundReader()
{
}

bool WAVSoundReader::OpenSound(Stream& stream, SoundReaderOptions* options)
{
	MMCKINFO ck;
	WAVEFORMATEX wfx;

	BinaryStream reader(&stream);

	ck.ckid = reader.ReadInt32();
	ck.cksize = reader.ReadInt32();
	ck.fccType = reader.ReadInt32();

	// Check to make sure this is a valid wave file
	if ((ck.ckid != FOURCC_RIFF) || (ck.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
	{
		Logger::Current()->Log(LogLevel::Error, _T("WAVSoundReader.OpenSound"),
			_T("Invalid waveform-audio data."));
		return false;
	}

	// Search the 'fmt ' chunk
	ck.fccType = reader.ReadInt32();
	ck.cksize = reader.ReadInt32();

	if (ck.fccType != mmioFOURCC('f', 'm', 't', ' '))
	{
		Logger::Current()->Log(LogLevel::Error, _T("WAVSoundReader.OpenSound"),
			_T("Invalid waveform-audio data."));
		return false;
	}

	// Read the 'fmt ' chunk
	wfx.wFormatTag = reader.ReadInt16();
	wfx.nChannels = reader.ReadInt16();
	wfx.nSamplesPerSec = reader.ReadInt32();
	wfx.nAvgBytesPerSec = reader.ReadInt32();
	wfx.nBlockAlign = reader.ReadInt16();
	wfx.wBitsPerSample = reader.ReadInt16();

	if (wfx.wFormatTag != WAVE_FORMAT_PCM)
	{
		wfx.cbSize = 0;
	}
	else
	{
		wfx.cbSize = reader.ReadInt16();
	}

	if (wfx.wBitsPerSample == 8)
		_Format.FormatType = AudioFormatType_PCM8;
	else if (wfx.wBitsPerSample == 16)
		_Format.FormatType = AudioFormatType_PCM16;
	else if (wfx.wBitsPerSample == 24)
		_Format.FormatType = AudioFormatType_PCM24;
	else if (wfx.wBitsPerSample == 32)
		_Format.FormatType = AudioFormatType_PCM32;
	else
		return false;

	_Format.Channels = wfx.nChannels;
	_Format.SamplesPerSecond = wfx.nSamplesPerSec;

	// Search the 'data' chunk
	ck.fccType = reader.ReadInt32();
	while (ck.fccType != mmioFOURCC('d', 'a', 't', 'a'))
	{
		ck.cksize = reader.ReadInt32();
		stream.Seek(ck.cksize, SeekOrigin_Current);
		ck.fccType = reader.ReadInt32();

		if (stream.IsEOF())
			return false;
	}

	int32 chunkSize = reader.ReadInt32();
	_DataOffset = stream.GetPosition();

	int32 sampleSize = _Format.GetBlockAlign();
	_DataSize = chunkSize / sampleSize;
	_RemainingSize = _DataSize;

	_Stream = &stream;

	return true;
}

bool WAVSoundReader::CloseSound()
{
	return true;
}

bool WAVSoundReader::GetFormat(AudioFormat& format)
{
	format = _Format;
	return true;
}

Stream* WAVSoundReader::GetStream()
{
	return _Stream;
}

int32 WAVSoundReader::GetLength()
{
	return _DataSize;
}

int32 WAVSoundReader::GetPosition()
{
	return (_DataSize - _RemainingSize);
}

bool WAVSoundReader::SetPosition(int32 value)
{
	int32 sampleSize = _Format.GetBlockAlign();
	_RemainingSize = _DataSize - value;
	_Stream->Seek(_DataOffset + value * sampleSize, SeekOrigin_Begin);

	return true;
}

int32 WAVSoundReader::ReadSound(SEbyte* buffer, int32 count)
{
	int32 sampleCount = Math::Min(count, _RemainingSize);
	int32 sampleSize =  _Format.GetBlockAlign();
	int32 bufferSize = sampleCount * sampleSize;
	_Stream->Read(buffer, bufferSize);
	return bufferSize;
}

}
