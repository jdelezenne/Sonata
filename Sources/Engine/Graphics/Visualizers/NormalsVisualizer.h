/*=============================================================================
NormalsVisualizer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NORMALSVISUALIZER_H_
#define _SE_NORMALSVISUALIZER_H_

#include "Graphics/Visualizers/Visualizer.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

/**
 * Normals visualizer.
 */
class NormalsVisualizer : public Visualizer
{
public:
	NormalsVisualizer();
	virtual ~NormalsVisualizer();

	Mesh* GetMesh() const { return _mesh; }
	void SetMesh(Mesh* geometry) { _mesh = geometry; }

	real32 GetLength() const { return _Length; }
	void SetLength(real32 length) { _Length = length; }

	Color32 GetColor() const { return _Color; }
	void SetColor(Color32 color) { _Color = color; }

	virtual void Render();

protected:
	void _BuildNormalMesh();

	Mesh* _mesh;
	Mesh* _NormalMesh;
	real32 _Length;
	Color32 _Color;
};

}

#endif 
