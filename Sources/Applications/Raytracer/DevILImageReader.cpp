/*=============================================================================
DevILImageReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DevILImageReader.h"
#include <IL/il.h>

namespace Raytracer
{

DevILImageReader::DevILImageReader()
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

RTImage* DevILImageReader::LoadImage(const String& source)
{
	ILuint imageId;
	ILboolean res;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	res = ilLoad(IL_TYPE_UNKNOWN, (ILstring)source.Data());

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

	RTImage* image = new RTImage();

	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILint format = ilGetInteger(IL_IMAGE_FORMAT);
	ILint type = ilGetInteger(IL_IMAGE_TYPE);

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
	default:
		ilDeleteImages(1, &imageId);

		Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.LoadImage"),
			_T("Image format not supported."));
		return NULL;
	}

	image->Create(imageFormat, width, height);
	kmByte* data = image->GetData();
	Memory::Copy(data, ilGetData(), image->GetDataSize());

	ilDeleteImages(1, &imageId);

	return image;
}

}
