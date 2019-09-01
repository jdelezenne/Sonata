/*=============================================================================
SkyDome.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SKYDOME_H_
#define _SE_SKYDOME_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Scene/Sky.h"
#include "Graphics/System/Texture.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

enum MappingMode
{
	MappingMode_Spherical,
	MappingMode_Planar
};

/**
	@brief SkyDome.

	A sky dome represents the distant environment which is rendered onto an
	hemisphere. The camera is placed at the center of this hemisphere.

	remarks:
		The sky dome should be rendered first.
		The color buffer doesn't need to be cleared.
		Depth testing and depth writing should be disabled.
		Lighting and fog should be disabled.
*/
class SE_GRAPHICS_EXPORT SkyDome : public Sky
{
	SE_DECLARE_CLASS(SkyDome, Object);
	SE_BEGIN_REFLECTION(SkyDome);
		SE_Field(_radius, real32, Public);
		SE_Field(_height, real32, Public);
		SE_Field(_rings, int32, Public);
		SE_Field(_segments, int32, Public);
		//SE_Field(_mapping, MappingMode, Public);
		//SE_Field(_texture, Texture, Public);
		SE_Field(_textureScale, Vector2, Public);
	SE_END_REFLECTION(SkyDome);

protected:
	real32 _radius;
	real32 _height;
	int32 _rings;
	int32 _segments;
	MappingMode _mapping;
	TexturePtr _texture;
	Vector2 _textureScale;
	MeshPtr _mesh;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	SkyDome();

	/** Destructor. */
	virtual ~SkyDome();
	//@}

	/** @name Properties. */
	//@{
	/** Gets or sets the radius of the sky dome. */
	real32 GetRadius() const { return _radius; }
	void SetRadius(real32 value) { _radius = value; }

	real32 GetHeight() const { return _height; }
	void SetHeight(real32 value) { _height = value; }

	int32 GetRings() const { return _rings; }
	void SetRings(int32 value) { _rings = value; }

	int32 GetSegments() const { return _segments; }
	void SetSegments(int32 value) { _segments = value; }

	/** Gets or sets the texture of the sky. */
	Texture* GetTexture() const { return _texture; }
	void SetTexture(Texture* value) { _texture = value; }

	Vector2 GetTextureScale() const { return _textureScale; }
	void SetTextureScale(Vector2 value) { _textureScale = value; }
	//@}

	virtual bool Create();
	virtual void Destroy();

	virtual void Update(const TimeValue& timeValue);
	virtual void Render();
};

}

#endif 
