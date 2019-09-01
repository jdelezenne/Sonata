/*=============================================================================
SDLImageReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SDLImageReader.h"

namespace SE_SDLImage
{

SDLImageReader::SDLImageReader() :
	ImageReader()
{
	SDL_Init(SDL_INIT_VIDEO);
}

SDLImageReader::~SDLImageReader()
{
	SDL_Quit();
}

Image* SDLImageReader::LoadImage(Stream& stream, ImageReaderOptions* options)
{
	SDL_Surface* surface;

	if (stream.GetStreamType() == StreamType_Memory)
	{
		MemoryStream& memory = *(MemoryStream*)&stream;
		SDL_RWops* src = SDL_RWFromMem(memory.GetBuffer(), stream.GetLength());
		surface = IMG_Load_RW(src, 0);
		SDL_FreeRW(src);
	}
	else if (stream.GetStreamType() == StreamType_File)
	{
		FileStream& file = *(FileStream*)&stream;
		surface = IMG_Load(file.GetFileName().Data());
	}
	else
	{
		return NULL;
	}

	if (surface == NULL)
	{
		return NULL;
	}

	_Image = new Image();

	PixelFormat format(surface->format->BitsPerPixel,
		surface->format->Rmask, surface->format->Gmask,
		surface->format->Bmask, surface->format->Amask);
	_Image->Create(format, surface->w, surface->h, 0);
	SEbyte* data = _Image->GetData();
	Memory::Copy(data, surface->hwdata, _Image->GetDataSize());

	return _Image;
}

}
