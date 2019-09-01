/*=============================================================================
QuakeMDLModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUAKEMDLMODELREADER_H_
#define _SE_QUAKEMDLMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_Quake
{

struct MDLModel;

/**
	Quake MDL Model reader.

	@todo Palette and Skins.
*/
class QuakeMDLModelReader : public ModelReader
{
protected:
	Array<ShaderMaterial*> _shaders;
	MDLModel* _mdlFile;

public:
	QuakeMDLModelReader();
	virtual ~QuakeMDLModelReader();

	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL);

protected:
	void Clean();
	bool ReadMDL(Stream& stream);
	ShaderMaterial* CreateShader(Texture* texture);
	Model* CreateModel();
	Mesh* CreateMesh(AnimationSet* animationSet);
	MeshPart* CreateMeshPart(Array<Vector2>& textureCoords, Array<uint32>& indices, AnimationSet* animationSet);
};

}

#endif 
