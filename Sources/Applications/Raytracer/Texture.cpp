/*=============================================================================
Texture.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Texture.h"
#include "Scene.h"

namespace Raytracer
{
	SE_IMPLEMENT_ABSTRACT(RTTexture);
	SE_IMPLEMENT_REFLECTION(RTTexture);

	RTTexture::RTTexture()
	{
		_Image = NULL;
	}

	RTTexture::~RTTexture()
	{
		SE_SAFE_DELETE(_Image);
	}
}
