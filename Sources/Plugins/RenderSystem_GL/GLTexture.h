/*=============================================================================
GLTexture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLTEXTURE_H_
#define _SE_GLTEXTURE_H_

#include "GLRenderSystem.h"

namespace SE_GL
{

/**
 * OpenGL texture.
 */
class GLTexture : public Texture
{
public:
	/** Constructors / Destructor. */
	//@{
	GLTexture();
	virtual ~GLTexture();
	//@}

	virtual bool Create(Image* image);
	virtual bool Destroy();

	GLuint GetTextureID() const { return _textureID; }

protected:
	GLuint _textureID;
};

}

#endif
