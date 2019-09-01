/*=============================================================================
StaticTerrainRenderer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_STATICTERRAINRENDERER_H_
#define _SE_STATICTERRAINRENDERER_H_

#include "Graphics/Terrain/TerrainRenderer.h"
#include "Graphics/System/Texture.h"

namespace SonataEngine
{

/**
	Static terrain renderer.
*/
class SE_GRAPHICS_EXPORT StaticTerrainRenderer : public TerrainRenderer
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	StaticTerrainRenderer();

	/** Destructor. */
	virtual ~StaticTerrainRenderer();
	//@}

	/** @name Properties. */
	//@{
	Mesh* GetMesh() const { return _mesh; }

	Texture* GetTexture() const { return _texture; }
	void SetTexture(Texture* value) { _texture = value; }

	Texture* GetDetailTexture() const { return _detailTexture; }
	void SetDetailTexture(Texture* value) { _detailTexture = value; }

	const Vector2& GetTextureScale() const { return _textureScale; }
	void SetTextureScale(const Vector2& value) { _textureScale = value; }

	const Vector2& GetDetailTextureScale() const { return _detailTextureScale; }
	void SetDetailTextureScale(const Vector2& value) { _detailTextureScale = value; }
	//@}

	virtual bool Create();

	virtual void Render();

protected:
	void _BuildNormalMesh();

	MeshPtr _mesh;
	TexturePtr _texture;
	TexturePtr _detailTexture;
	Vector2 _textureScale;
	Vector2 _detailTextureScale;
};

}

#endif 
