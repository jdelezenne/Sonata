/*=============================================================================
DevILImageReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DevILImageReader.h"

namespace SE_DevIL
{

DevILImageReader::DevILImageReader() :
	ImageReader()
{
	ilInit();

    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
    ilEnable(IL_ORIGIN_SET);

	ilEnable(IL_CONV_PAL); // palette are not supported by DevIL and will be converted

	ilEnable(IL_FILE_OVERWRITE);

    ilSetInteger(IL_FORMAT_MODE, IL_BGRA);
    ilEnable(IL_FORMAT_SET);
}

DevILImageReader::~DevILImageReader()
{
	ilShutDown();
}

Image* DevILImageReader::LoadImage(Stream& stream, ImageReaderOptions* options)
{
	ILuint imageId;
	ILboolean res;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (stream.GetStreamType() == StreamType_Memory)
	{
		MemoryStream& memory = *(MemoryStream*)&stream;
		res = ilLoadL(IL_TYPE_UNKNOWN, memory.GetBuffer(), stream.GetLength());
	}
	else if (stream.GetStreamType() == StreamType_File)
	{
		FileStream& file = *(FileStream*)&stream;
		res = ilLoad(IL_TYPE_UNKNOWN, (ILstring)file.GetFileName().Data());
	}
	else
	{
		ilDeleteImages(1, &imageId);

		Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
			_T("Failed to load the image."));
		return NULL;
	}

	if (res != IL_TRUE)
	{
		ilDeleteImages(1, &imageId);

		if (res == IL_COULD_NOT_OPEN_FILE)
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
				_T("The file pointed to by FileName could not be opened for writing."));
		}
		else if (res == IL_INVALID_FILE_HEADER)
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
				_T("The file had an invalid header and could not be loaded."));
		}
		else if (res == IL_ILLEGAL_FILE_VALUE)
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
				_T("The file could not be loaded due to an invalid value present."));
		}
		else if (res == IL_OUT_OF_MEMORY)
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
				_T("Could not allocate memory for the new image data."));
		}
		else
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
				_T("Failed to load the image."));
		}

		return NULL;
	}

	Image* image = new Image();

	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILint format = ilGetInteger(IL_IMAGE_FORMAT);
	ILint type = ilGetInteger(IL_IMAGE_TYPE);
	ILint mipmaps = ilGetInteger(IL_NUM_MIPMAPS);

	PixelFormat imageFormat;

	switch (format)
	{
	case IL_RGB:
		imageFormat = PixelFormat_R8G8B8;
		break;
	case IL_RGBA:
		imageFormat = PixelFormat_R8G8B8A8;
		break;
	case IL_BGR:
		imageFormat = PixelFormat_R8G8B8;
		break;
	case IL_BGRA:
		imageFormat = PixelFormat_R8G8B8A8;
		break;
	case IL_LUMINANCE:
		imageFormat = PixelFormat_Luminance;
		break;
	/*case IL_COLOR_INDEX:
		imageFormat = PixelFormat(PixelFormatFlag_Indexed);
		break;*/
	default:
		ilDeleteImages(1, &imageId);

		Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
			_T("Image format not supported."));
		return NULL;
	}

	/*switch (type)
	{
		IL_BYTE
		IL_UNSIGNED_BYTE
		IL_SHORT
		IL_UNSIGNED_SHORT
		IL_INT
		IL_UNSIGNED_INT
		IL_FLOAT 
	}*/

	image->Create(imageFormat, width, height, 1, mipmaps);
	SEbyte* data = image->GetData();
	Memory::Copy(data, ilGetData(), image->GetDataSize());

	ilDeleteImages(1, &imageId);

	return image;
}

}
