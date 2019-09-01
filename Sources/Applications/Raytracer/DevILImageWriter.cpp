/*=============================================================================
DevILImageWriter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DevILImageWriter.h"
#include <IL/il.h>

namespace Raytracer
{

DevILImageWriter::DevILImageWriter()
{
	ilInit();

	ilEnable(IL_CONV_PAL); // palette are not supported by DevIL and will be converted
}

DevILImageWriter::~DevILImageWriter()
{
	ilShutDown();
}

bool DevILImageWriter::SaveImage(const String& dest, RTImage* image)
{
	ILuint imageId;
	ILboolean res = IL_FALSE;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (image->GetFormat() == PixelFormat_R8G8B8)
		res = ilTexImage(image->GetWidth(), image->GetHeight(), 1, 3, IL_BGR, IL_UNSIGNED_BYTE, NULL);
	else if (image->GetFormat() == PixelFormat_R8G8B8A8)
		res = ilTexImage(image->GetWidth(), image->GetHeight(), 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, NULL);

	if (!res)
	{
		Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.SaveImage"),
			_T("Failed to access the image data."));
		return false;
	}

	ilRegisterOrigin(IL_ORIGIN_UPPER_LEFT);

	Memory::Copy(ilGetData(), image->GetData(), image->GetDataSize());

	ilEnable(IL_FILE_OVERWRITE);

	res = ilSaveImage((ILstring)dest.Data());

	if (res != IL_TRUE)
	{
		ilDeleteImages(1, &imageId);

		if (res == IL_COULD_NOT_OPEN_FILE)
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.SaveImage"),
				_T("The file pointed to by FileName could not be opened for writing."));
		}
		else if (res == IL_INVALID_EXTENSION)
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.SaveImage"),
				_T("The file could not be saved based on the file name's extension."));
		}
		else
		{
			Logger::Current()->Log(LogLevel::Error, _T("DevILImageWriter.SaveImage"),
				_T("Failed to save the image."));
		}

		return NULL;
	}

	ilDeleteImages(1, &imageId);

	return true;
}

}
