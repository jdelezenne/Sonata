/*=============================================================================
TerrainRenderer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TERRAINRENDERER_H_
#define _SE_TERRAINRENDERER_H_

#include "Core/Core.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Materials/ShaderMaterial.h"

namespace SonataEngine
{

class Terrain;
class Camera;

/**
	Terrain renderer.
*/
class SE_GRAPHICS_EXPORT TerrainRenderer
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	TerrainRenderer();

	/** Destructor. */
	virtual ~TerrainRenderer();
	//@}

	/** Gets or sets the owner terrain. */
	Terrain* GetTerrain() const { return _Terrain; }
	void SetTerrain(Terrain* value) { _Terrain = value; }

	/** Gets or sets the current camera. */
	Camera* GetCamera() const { return _camera; }
	void SetCamera(Camera* value) { _camera = value; }

	ShaderMaterial* GetShader() const { return _shader; }
	void SetShader(ShaderMaterial* value) { _shader = value; }

	virtual Vector3 GetNormal(int32 x, int32 y);

	virtual Vector3 GetInterpolatedNormal(real32 x, real32 y);

	virtual bool Create() = 0;

	virtual void Render() = 0;

protected:
	void _ComputeFaceNormals();
	void _ComputeVertexNormals();

protected:
	Terrain* _Terrain;
	Camera* _camera;
	ShaderMaterialPtr _shader;

	Array<Vector3> _Vertices;
	Array<Vector3> _FaceNormals;
	Array<Vector3> _VertexNormals;
};

}

#endif 
