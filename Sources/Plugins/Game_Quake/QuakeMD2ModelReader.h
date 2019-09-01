/*=============================================================================
QuakeMD2ModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUAKEMD2MODELREADER_H_
#define _SE_QUAKEMD2MODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_Quake
{

struct MD2Model;

class QuakeMD2ModelReader : public ModelReader
{
protected:
	Array<ShaderMaterial*> _shaders;
	MD2Model* _md2File;

public:
	QuakeMD2ModelReader();
	virtual ~QuakeMD2ModelReader();

	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL);

protected:
	void Destroy();
	bool ReadMD2(Stream& stream);
	ShaderMaterial* CreateShader(Texture* texture);
	Model* CreateModel();
	Mesh* CreateMesh(AnimationSet* animationSet);
	MeshPart* CreateMeshPart(Array<Vector2>& textureCoords, Array<uint32>& indices, Array<uint32>& textureIndices, AnimationSet* animationSet);
};

}

#endif 
