/*=============================================================================
GLTexture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLTexture.h"

namespace SE_GL
{

GLTexture::GLTexture() :
	_textureID(0)
{
}

GLTexture::~GLTexture()
{
	Destroy();
}

bool GLTexture::Create(Image* image)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);

	uint8 bpp = image->GetBitsPerPixel() / 8;
	uint width = image->GetWidth();
	uint height = image->GetHeight();
	SEbyte* data = image->GetData();
/*
	// Swap upside down
	int size = width * height * bpp;
	int stride = bpp * width;
	SEbyte* gldata = new SEbyte[size];
	SEbyte* swap = new SEbyte[stride];
	for (uint i = 0; i < height; i++)
	{
		Memory::Copy(swap, (void*)&data[i * stride], stride);
		int index = size - (i+1) * stride;
		Memory::Copy((void*)&gldata[(size - (i+1) * stride)], swap, stride);
	}
*/
	GLenum glFormat;
	if (bpp == 3)
		glFormat = GL_RGB;
	else
		glFormat = GL_RGBA;

#if 1
	SEbyte* gldata = new SEbyte[image->GetDataSize()];
	for (uint i = 0; i < width*height; i++)
	{
		gldata[bpp*i+2] = data[bpp*i];
		gldata[bpp*i+1] = data[bpp*i+1];
		gldata[bpp*i] = data[bpp*i+2];
		if (bpp == 4)
			gldata[bpp*i+3] = data[bpp*i+3];
	}

	glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 
		0, glFormat, GL_UNSIGNED_BYTE, gldata);
	delete[] gldata;
#else
	Image* destImage = new Image();
	destImage->Create(PixelFormat_R8G8B8A8, width, height);

	ImageHelper::Convert(destImage, image, PixelFormat_R8G8B8A8);

	glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 
		0, glFormat, GL_UNSIGNED_BYTE, destImage->GetData());

	delete destImage;
	//delete[] swap;
	//delete[] gldata;
#endif

	_Width = width;
	_Height = height;
	_BitsPerPixel = bpp * 8;

	return true;
}

bool GLTexture::Destroy()
{
	glDeleteTextures(1, &_textureID);

	return true;
}

}
