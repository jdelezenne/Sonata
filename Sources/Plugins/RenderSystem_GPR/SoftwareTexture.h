/*=============================================================================
SoftwareTexture.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _SOFTWARETEXTURE_H_
#define _SOFTWARETEXTURE_H_

#include "OGPR/OGPR.h"
#include "Material/Texture.h"
using namespace Teesside;

/**
 * Software texture.
 */
class SoftwareTexture : public Texture
{
public:
	/** Constructors / Destructor. */
	//@{
	SoftwareTexture(OGPRRenderer* renderer);
	virtual ~SoftwareTexture();
	//@}

	virtual bool Create(Image* image);
	virtual bool Destroy();

	OGPRBuffer* GetBuffer() const { return _Buffer; }
	void SetBuffer(OGPRBuffer* buffer) { _Buffer = buffer; }

protected:
	OGPRRenderer* _Renderer;
	OGPRBuffer* _Buffer;
};

#endif
