/*=============================================================================
QuakeMD3ModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUAKEMD3MODELREADER_H_
#define _SE_QUAKEMD3MODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_Quake
{

struct MD3Model;
struct MD3Surface;
struct MD3Animation;

class QuakeMD3ModelReader : public ModelReader
{
protected:
	String _fileName;
	String _path;
	String _skinName;
	bool _isCharacter;
	bool _isAnimated;
	MD3Model* _md3Files[3];
	MD3Animation* _md3Animation;

public:
	QuakeMD3ModelReader();
	virtual ~QuakeMD3ModelReader();

	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL);

protected:
	void Destroy();
	bool ReadMD3(int index, Stream* stream);
	bool ReadSkin(int index, const String& fileName);
	bool ReadAnimation(const String& fileName);
	ShaderMaterial* CreateShader(const String& name);
	Model* CreateModel();
	Mesh* CreateMesh(MD3Model* md3File, MD3Surface* md3Surface);
	MeshPart* CreateMeshPart(MD3Model* md3File, MD3Surface* md3Surface, Array<uint32>& indices, Array<Vector2>& textureCoords);
};

}

#endif 
