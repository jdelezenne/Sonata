/*=============================================================================
WestwoodW3DReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WESTWOODW3DREADER_H_
#define _SE_WESTWOODW3DREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

#include "w3d/w3d_file.h"

namespace SE_Westwood
{

struct W3dHierarchy
{
	W3dHierarchyStruct Hierarchy;
	W3dPivotStruct* Pivots;
	W3dPivotFixupStruct* PivotFixups;
};

struct W3dAnimation
{
	W3dAnimHeaderStruct AnimHeader;
	BaseArray<W3dAnimChannelStruct> AnimChannels;
	BaseArray<W3dBitChannelStruct> BitChannels;
};

struct W3dVertexMaterial
{
	char* Name;
	W3dVertexMaterialStruct VertexMaterialInfo;
};

struct W3dVertexMaterials
{
	W3dVertexMaterial* VertexMaterials;
};

struct W3dTexture
{
	char* Name;
	W3dTextureInfoStruct TextureInfo;
};

struct W3dTextures
{
	W3dTexture* Textures;
};

struct W3dTextureStage
{
	uint32 TextureIDCount;
	uint32* TextureIDs;
	W3dTexCoordStruct* TexCoords;
	uint32 PerFaceTexCoordIDCount;
	W3dVectorStruct* PerFaceTexCoordIDs;
};

struct W3dMaterialPass
{
	uint32 VertexMaterialIDCount;
	uint32* VertexMaterialIDs;
	uint32 ShaderIDCount;
	uint32* ShaderIDs;
	W3dRGBAStruct* DCG;
	W3dRGBAStruct* DIG;
	W3dRGBAStruct* SCG;
	uint32 TextureStageCount;
	W3dTextureStage TextureStages[8];
};

struct W3dMesh
{
	W3dMeshHeader3Struct Header;
	W3dVectorStruct* Vertices;
	W3dVectorStruct* VertexNormals;
	W3dVertInfStruct* VertexInfluences;
	W3dTriStruct* Triangles;
	uint32* VertexShadeIndices;
	W3dMaterialInfoStruct MaterialInfo;
	W3dVertexMaterials VertexMaterials;
	W3dShaderStruct* Shaders;
	W3dTextures Textures;
	W3dMaterialPass* MaterialPasses;
};

struct W3dHModel
{
	W3dHModelHeaderStruct Header;
	W3dHModelNodeStruct* Nodes;
};

struct W3dHLodArray
{
	W3dHLodArrayHeaderStruct ArrayHeader;
	W3dHLodSubObjectStruct* SubObjects;
};

struct W3dHLod
{
	W3dHLodHeaderStruct Header;
	W3dHLodArray* Lods;
};

struct W3DFile
{
	W3DFile();
	~W3DFile();

	W3dHierarchy Hierarchy;
	W3dAnimation Animation;
	W3dBoxStruct Box;
	BaseArray<W3dMesh> Meshes;
	W3dHModel HModel;
	W3dHLod HLod;
};

class WestwoodW3DReader : public ModelReader
{
public:
	WestwoodW3DReader();
	virtual ~WestwoodW3DReader();

	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL);

protected:
	void Clean();
	bool ReadW3D();
	void Convert(Bone* parent, Model* model, uint32 index);

	bool ReadChunk(uint32 ChunkSize, uint32* CurrentSize, W3dChunkHeader* current);
	void SkipChunk(W3dChunkHeader* current);

	void ReadW3dMesh(uint32 ChunkSize, W3dMesh* chunk);
	void ReadW3dVertexMaterials(uint32 ChunkSize, W3dVertexMaterials* chunk);
	void ReadW3dVertexMaterial(uint32 ChunkSize, W3dVertexMaterial* chunk);
	void ReadW3dTextures(uint32 ChunkSize, W3dTextures* chunk);
	void ReadW3dTexture(uint32 ChunkSize, W3dTexture* chunk);
	void ReadW3dMaterialPass(uint32 ChunkSize, W3dMaterialPass* chunk);
	void ReadW3dHierarchy(uint32 ChunkSize, W3dHierarchy* chunk);
	void ReadW3dAnimation(uint32 ChunkSize, W3dAnimation* chunk);
	void ReadW3dTextureStage(uint32 ChunkSize, W3dTextureStage* chunk);
	void ReadW3dHModel(uint32 ChunkSize, W3dHModel* chunk);
	void ReadW3dHLod(uint32 ChunkSize, W3dHLod* chunk);
	void ReadW3dHLodArray(uint32 ChunkSize, W3dHLodArray* chunk);

protected:
	BinaryStream* _Reader;
	Model* _Model;
	W3DFile* _W3DFile;
};

}

#endif 
