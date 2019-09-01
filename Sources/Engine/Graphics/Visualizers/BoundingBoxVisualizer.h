/*=============================================================================
BoundingBoxVisualizer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BOUNDINGBOXVISUALIZER_H_
#define _SE_BOUNDINGBOXVISUALIZER_H_

#include "Graphics/Visualizers/Visualizer.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

/**
 * Normals visualizer.
 */
class BoundingBoxVisualizer : public Visualizer
{
public:
	BoundingBoxVisualizer();
	virtual ~BoundingBoxVisualizer();

	SceneObject* GetObject() const { return _Object; }
	void SetObject(SceneObject* object) { _Object = object; }

	Color32 GetColor() const { return _Color; }
	void SetColor(Color32 color) { _Color = color; }

	virtual void Render();

protected:
	void _BuildBBoxMesh();

	SceneObject* _Object;
	Mesh* _BBoxMesh;
	Color32 _Color;
};

}

#endif 
