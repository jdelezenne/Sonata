/*=============================================================================
WestwoodW3DReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "WestwoodW3DReader.h"

namespace SE_Westwood
{

String _Path;

Texture* CreateTexture(const String& fileName)
{
	Resource* resource;
	Image* image;
	Texture* texture;

	resource = ResourceHelper::LoadFromFile(Path::Combine(_Path, fileName), SE_ID_DATA_IMAGE);
	if (resource == NULL)
	{
		String dds = Path::Combine(_Path, Path::ChangeExtension(fileName, _T("dds")));
		resource = ResourceHelper::LoadFromFile(dds, SE_ID_DATA_IMAGE);
		if (resource == NULL)
		{
			return NULL;
		}
	}

	image = (Image*)resource->GetData();

	if (!RenderSystem::Current()->CreateTexture(image, TextureUsage_Static, &texture))
		return NULL;

	return texture;
}

W3DFile::W3DFile()
{
	Hierarchy.Hierarchy.NumPivots = 0;
	HModel.Header.NumConnections = 0;
	HLod.Header.LodCount = 0;
	HLod.Lods = NULL;
}

W3DFile::~W3DFile()
{
	int i, j;

	if (Hierarchy.Hierarchy.NumPivots > 0)
	{
		delete[] Hierarchy.Pivots;
		delete[] Hierarchy.PivotFixups;
	}

	for (i=0; i<Meshes.Count(); i++)
	{
		const W3dMesh& mesh = Meshes[i];
		delete[] mesh.Vertices;
		if (mesh.MaterialInfo.TextureCount > 0)
		{
			for (j=0; j<(int)mesh.MaterialInfo.TextureCount; j++)
			{
				delete[] mesh.Textures.Textures[j].Name;
			}
			delete[] mesh.Textures.Textures;
		}
		if (mesh.MaterialInfo.ShaderCount > 0)
		{
			delete[] mesh.Shaders;
		}
		for (j=0; j<(int)mesh.MaterialInfo.PassCount; j++)
		{
			delete[] mesh.MaterialPasses[j].VertexMaterialIDs;
		}
		delete[] mesh.MaterialPasses;
		if (mesh.MaterialInfo.VertexMaterialCount > 0)
		{
			for (j=0; j<(int)mesh.MaterialInfo.PassCount; j++)
			{
				delete[] mesh.VertexMaterials.VertexMaterials[j].Name;
			}
			delete[] mesh.VertexMaterials.VertexMaterials;
		}
	}
	if (HModel.Header.NumConnections > 0)
	{
		delete[] HModel.Nodes;
	}
	for (i=0; i<(int)HLod.Header.LodCount; i++)
	{
		delete[] HLod.Lods[i].SubObjects;
	}
	delete HLod.Lods;
}

WestwoodW3DReader::WestwoodW3DReader() :
	ModelReader()
{
	_Reader = NULL;
	_W3DFile = NULL;
}

WestwoodW3DReader::~WestwoodW3DReader()
{
	Clean();
}

void WestwoodW3DReader::Clean()
{
	SE_DELETE(_W3DFile);
}

Model* WestwoodW3DReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	uint32 i;

	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("WestwoodW3DReader.LoadModel"),
			_T("No active renderer."));
		return NULL;
	}

	if (stream.GetStreamType() != StreamType_File)
	{
		return NULL;
	}

	_Path = Path::GetDirectoryName(((FileStream*)&stream)->GetFileName());

	Clean();

	_W3DFile = new W3DFile;

	_Reader = new BinaryStream(&stream);

	if (!ReadW3D())
		return NULL;

	SE_DELETE(_Reader);

	String HierarchyName(_W3DFile->HLod.Header.HierarchyName);
	if (!HierarchyName.IsEmpty())
	{
		File* file = new File(Path::Combine(_Path, HierarchyName) + ".W3D");
		FileStreamPtr fileStream = file->Open(FileMode_Open, FileAccess_Read, FileShare_Read);
		if (fileStream != NULL)
		{
			_Reader = new BinaryStream(fileStream);

			if (!ReadW3D())
				return NULL;
		}

		SE_DELETE(_Reader);
		SE_DELETE(file);
	}

	_Model = new Model();
	Skeleton* skeleton = new Skeleton();
	Bone* rootBone = skeleton->AddBone(skeleton);
	skeleton->SetRootBone(rootBone);
	_Model->SetSkeleton(skeleton);

	for (i=0; i<_W3DFile->Hierarchy.Hierarchy.NumPivots; ++i)
	{
		const W3dPivotStruct& w3dPivot = _W3DFile->Hierarchy.Pivots[i];
		if (w3dPivot.ParentIdx == ~0)
		{
			Bone* bone = skeleton->AddBone();
			Convert(bone, _Model, i);
			rootBone->AddChild(bone);
			break;
		}
	}

	rootBone->SetPoseOrientation(Quaternion::FromAxisAngle(Vector3::UnitX, -Math::Pi/2) *
		Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi/2));

	_Model->UpdateBounds();

	return _Model;
}

bool WestwoodW3DReader::ReadChunk(uint32 ChunkSize, uint32* CurrentSize, W3dChunkHeader* current)
{
	if (*CurrentSize >= ChunkSize)
		return false;

	if (_Reader->Read((SEbyte*)current, sizeof(W3dChunkHeader)) != sizeof(W3dChunkHeader))
		return false;

	current->ChunkSize &= 0x7FFFFFFF;
	*CurrentSize += sizeof(W3dChunkHeader) + current->ChunkSize;

	return (!_Reader->GetStream()->IsEOF());
}

void WestwoodW3DReader::SkipChunk(W3dChunkHeader* current)
{
	_Reader->GetStream()->Seek(current->ChunkSize, SeekOrigin_Current);
}

bool WestwoodW3DReader::ReadW3D()
{
	uint32 ChunkSize = _Reader->GetStream()->GetLength();

	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 MeshCount = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_MESH:
			_W3DFile->Meshes.Add(W3dMesh());
			ReadW3dMesh(current.ChunkSize, &_W3DFile->Meshes[MeshCount]);
			MeshCount++;
			break;

		case W3D_CHUNK_HIERARCHY:
			ReadW3dHierarchy(current.ChunkSize, &_W3DFile->Hierarchy);
			break;

		case W3D_CHUNK_ANIMATION:
			ReadW3dAnimation(current.ChunkSize, &_W3DFile->Animation);
			break;

		case W3D_CHUNK_HMODEL:
			ReadW3dHModel(current.ChunkSize, &_W3DFile->HModel);
			break;

		case W3D_CHUNK_HLOD:
			ReadW3dHLod(current.ChunkSize, &_W3DFile->HLod);
			break;

		case W3D_CHUNK_BOX:
			_Reader->Read((SEbyte*)&_W3DFile->Box, sizeof(W3dBoxStruct));
			break;

		default:
			SkipChunk(&current);
		}
	}

	return true;
}

void WestwoodW3DReader::ReadW3dMesh(uint32 ChunkSize, W3dMesh* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 PassIndex = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_MESH_HEADER3:
			_Reader->Read((SEbyte*)&chunk->Header, sizeof(W3dMeshHeader3Struct));
			chunk->Vertices = new W3dVectorStruct[chunk->Header.NumVertices];
			chunk->VertexNormals = new W3dVectorStruct[chunk->Header.NumVertices];
			chunk->VertexInfluences = NULL;
			chunk->Triangles = new W3dTriStruct[chunk->Header.NumTris];
			chunk->VertexShadeIndices = new uint32[chunk->Header.NumVertices];
			break;

		case W3D_CHUNK_VERTICES:
			_Reader->Read((SEbyte*)chunk->Vertices, chunk->Header.NumVertices*sizeof(W3dVectorStruct));
			break;

		case W3D_CHUNK_VERTEX_NORMALS:
			_Reader->Read((SEbyte*)chunk->VertexNormals, chunk->Header.NumVertices*sizeof(W3dVectorStruct));
			break;

		case W3D_CHUNK_VERTEX_INFLUENCES:
			chunk->VertexInfluences = new W3dVertInfStruct[chunk->Header.NumVertices];
			_Reader->Read((SEbyte*)chunk->VertexInfluences,
				chunk->Header.NumVertices*sizeof(W3dVertInfStruct));
			break;

		case W3D_CHUNK_TRIANGLES:
			_Reader->Read((SEbyte*)chunk->Triangles, chunk->Header.NumTris*sizeof(W3dTriStruct));
			break;

		case W3D_CHUNK_VERTEX_SHADE_INDICES:
			_Reader->Read((SEbyte*)chunk->VertexShadeIndices, chunk->Header.NumVertices*sizeof(uint32));
			break;

		case W3D_CHUNK_MATERIAL_INFO:
			_Reader->Read((SEbyte*)&chunk->MaterialInfo, sizeof(W3dMaterialInfoStruct));
			chunk->VertexMaterials.VertexMaterials =
				new W3dVertexMaterial[chunk->MaterialInfo.VertexMaterialCount];
			chunk->Shaders = new W3dShaderStruct[chunk->MaterialInfo.ShaderCount];
			chunk->Textures.Textures = new W3dTexture[chunk->MaterialInfo.TextureCount];
			chunk->MaterialPasses = new W3dMaterialPass[chunk->MaterialInfo.PassCount];
			break;

		case W3D_CHUNK_VERTEX_MATERIALS:
			ReadW3dVertexMaterials(current.ChunkSize, &chunk->VertexMaterials);
			break;

		case W3D_CHUNK_SHADERS:
			_Reader->Read((SEbyte*)chunk->Shaders,
				chunk->MaterialInfo.ShaderCount*sizeof(W3dShaderStruct));
			break;

		case W3D_CHUNK_TEXTURES:
			ReadW3dTextures(current.ChunkSize, &chunk->Textures);
			break;

		case W3D_CHUNK_MATERIAL_PASS:
			ReadW3dMaterialPass(current.ChunkSize, &chunk->MaterialPasses[PassIndex++]);
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dVertexMaterials(uint32 ChunkSize, W3dVertexMaterials* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 VertexMaterialIndex = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_VERTEX_MATERIAL:
			ReadW3dVertexMaterial(current.ChunkSize, &chunk->VertexMaterials[VertexMaterialIndex++]);
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dVertexMaterial(uint32 ChunkSize, W3dVertexMaterial* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_VERTEX_MATERIAL_NAME:
			chunk->Name = new char[current.ChunkSize];
			_Reader->Read((SEbyte*)chunk->Name, current.ChunkSize);
			break;

		case W3D_CHUNK_VERTEX_MATERIAL_INFO:
			_Reader->Read((SEbyte*)&chunk->VertexMaterialInfo, sizeof(W3dVertexMaterialStruct));
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dTextures(uint32 ChunkSize, W3dTextures* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 Index = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_TEXTURE:
			ReadW3dTexture(current.ChunkSize, &chunk->Textures[Index++]);
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dTexture(uint32 ChunkSize, W3dTexture* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_TEXTURE_NAME:
			chunk->Name = new char[current.ChunkSize];
			_Reader->Read((SEbyte*)chunk->Name, current.ChunkSize);
			break;

		case W3D_CHUNK_TEXTURE_INFO:
			_Reader->Read((SEbyte*)&chunk->TextureInfo, sizeof(W3dTextureInfoStruct));
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dMaterialPass(uint32 ChunkSize, W3dMaterialPass* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint Count = 0;
	chunk->TextureStageCount = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_VERTEX_MATERIAL_IDS:
			chunk->VertexMaterialIDCount = current.ChunkSize / sizeof(uint32);
			chunk->VertexMaterialIDs = new uint32[chunk->VertexMaterialIDCount];
			_Reader->Read((SEbyte*)chunk->VertexMaterialIDs, current.ChunkSize);
			break;

		case W3D_CHUNK_SHADER_IDS:
			chunk->ShaderIDCount = current.ChunkSize / sizeof(uint32);
			chunk->ShaderIDs = new uint32[chunk->ShaderIDCount];
			_Reader->Read((SEbyte*)chunk->ShaderIDs, current.ChunkSize);
			break;

		case W3D_CHUNK_DCG:
			Count = current.ChunkSize / sizeof(W3dRGBAStruct);
			chunk->DCG = new W3dRGBAStruct[Count];
			_Reader->Read((SEbyte*)chunk->DCG, current.ChunkSize);
			break;

		case W3D_CHUNK_DIG:
			Count = current.ChunkSize / sizeof(W3dRGBAStruct);
			chunk->DIG = new W3dRGBAStruct[Count];
			_Reader->Read((SEbyte*)chunk->DIG, current.ChunkSize);
			break;

		case W3D_CHUNK_SCG:
			Count = current.ChunkSize / sizeof(W3dRGBAStruct);
			chunk->SCG = new W3dRGBAStruct[Count];
			_Reader->Read((SEbyte*)chunk->SCG, current.ChunkSize);
			break;

		case W3D_CHUNK_TEXTURE_STAGE:
			ReadW3dTextureStage(current.ChunkSize, &chunk->TextureStages[chunk->TextureStageCount]);
			chunk->TextureStageCount++;
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dTextureStage(uint32 ChunkSize, W3dTextureStage* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint Count = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_TEXTURE_IDS:
			chunk->TextureIDCount = current.ChunkSize / sizeof(uint32);
			chunk->TextureIDs = new uint32[chunk->TextureIDCount];
			_Reader->Read((SEbyte*)chunk->TextureIDs, current.ChunkSize);
			break;

		case W3D_CHUNK_STAGE_TEXCOORDS:
			Count = current.ChunkSize / sizeof(W3dTexCoordStruct);
			chunk->TexCoords = new W3dTexCoordStruct[Count];
			_Reader->Read((SEbyte*)chunk->TexCoords, current.ChunkSize);
			break;

		case W3D_CHUNK_PER_FACE_TEXCOORD_IDS:
			chunk->PerFaceTexCoordIDCount = current.ChunkSize / sizeof(W3dVectorStruct);
			chunk->PerFaceTexCoordIDs = new W3dVectorStruct[chunk->PerFaceTexCoordIDCount];
			_Reader->Read((SEbyte*)chunk->PerFaceTexCoordIDs, current.ChunkSize);
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dHierarchy(uint32 ChunkSize, W3dHierarchy* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_HIERARCHY_HEADER:
			_Reader->Read((SEbyte*)&chunk->Hierarchy, sizeof(W3dHierarchyStruct));
			chunk->Pivots = new W3dPivotStruct[chunk->Hierarchy.NumPivots];
			chunk->PivotFixups = new W3dPivotFixupStruct[chunk->Hierarchy.NumPivots];
			break;

		case W3D_CHUNK_PIVOTS:
			_Reader->Read((SEbyte*)chunk->Pivots, chunk->Hierarchy.NumPivots*sizeof(W3dPivotStruct));
			break;

		case W3D_CHUNK_PIVOT_FIXUPS:
			_Reader->Read((SEbyte*)chunk->PivotFixups,
				chunk->Hierarchy.NumPivots*sizeof(W3dPivotFixupStruct));
			break;

		default:
			SkipChunk(&current);
		}
	}
}
void WestwoodW3DReader::ReadW3dAnimation(uint32 ChunkSize, W3dAnimation* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 DataSize = 0;
	//float32* Data;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_ANIMATION_HEADER:
			_Reader->Read((SEbyte*)&chunk->AnimHeader, sizeof(W3dAnimHeaderStruct));
			break;
/*
		case W3D_CHUNK_ANIMATION_CHANNEL:
			W3dAnimChannelStruct animChannel;
			_Reader->Read((SEbyte*)&animChannel, sizeof(W3dAnimChannelStruct) - sizeof(float32));
			DataSize = (animChannel.LastFrame - animChannel.FirstFrame + 1) * animChannel.VectorLen;
			//Memory::Copy(animChannel.Data, new float32[DataSize], sizeof(float32*));
			//_Reader->Read((SEbyte*)animChannel.Data, DataSize * sizeof(float32));
			Data = new float32[DataSize];
			_Reader->Read((SEbyte*)Data, DataSize * sizeof(float32));
			chunk->AnimChannels.Add(animChannel);
			break;

		case W3D_CHUNK_BIT_CHANNEL:
			W3dBitChannelStruct bitChannel;
			_Reader->Read((SEbyte*)&bitChannel, sizeof(W3dBitChannelStruct) - sizeof(float32));
			DataSize = (bitChannel.LastFrame - bitChannel.FirstFrame + 1) / 8;
			//Memory::Copy(bitChannel.Data, new float32[DataSize], sizeof(float32*));
			//_Reader->Read((SEbyte*)bitChannel.Data, DataSize * sizeof(float32));
			Data = new float32[DataSize];
			_Reader->Read((SEbyte*)Data, DataSize * sizeof(float32));
			chunk->BitChannels.Add(bitChannel);
			break;
*/
		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dHModel(uint32 ChunkSize, W3dHModel* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 NodeIndex = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_HMODEL_HEADER:
			_Reader->Read((SEbyte*)&chunk->Header, sizeof(W3dHModelHeaderStruct));
			chunk->Nodes = new W3dHModelNodeStruct[chunk->Header.NumConnections];
			break;

		case W3D_CHUNK_NODE:
		case W3D_CHUNK_COLLISION_NODE:
		case W3D_CHUNK_SKIN_NODE:
			_Reader->Read((SEbyte*)&chunk->Nodes[NodeIndex++], sizeof(W3dHModelNodeStruct));
			break;

		case OBSOLETE_W3D_CHUNK_HMODEL_AUX_DATA:
		case OBSOLETE_W3D_CHUNK_SHADOW_NODE:
		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dHLod(uint32 ChunkSize, W3dHLod* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 LodIndex = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_HLOD_HEADER:
			_Reader->Read((SEbyte*)&chunk->Header, sizeof(W3dHLodHeaderStruct));
			chunk->Lods = new W3dHLodArray[chunk->Header.LodCount];
			break;

		case W3D_CHUNK_HLOD_LOD_ARRAY:
			ReadW3dHLodArray(current.ChunkSize, &chunk->Lods[LodIndex++]);
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::ReadW3dHLodArray(uint32 ChunkSize, W3dHLodArray* chunk)
{
	W3dChunkHeader current;
	uint32 CurrentSize = 0;
	uint32 SubObjectIndex = 0;

	while (ReadChunk(ChunkSize, &CurrentSize, &current))
	{
		switch (current.ChunkType)
		{
		case W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER:
			_Reader->Read((SEbyte*)&chunk->ArrayHeader, sizeof(W3dHLodArrayHeaderStruct));
			chunk->SubObjects = new W3dHLodSubObjectStruct[chunk->ArrayHeader.ModelCount];
			break;

		case W3D_CHUNK_HLOD_SUB_OBJECT:
			_Reader->Read((SEbyte*)&chunk->SubObjects[SubObjectIndex++], sizeof(W3dHLodSubObjectStruct));
			break;

		default:
			SkipChunk(&current);
		}
	}
}

void WestwoodW3DReader::Convert(Bone* parent, Model* model, uint32 index)
{
	uint32 i, j;

	const W3dPivotStruct& w3dPivot = _W3DFile->Hierarchy.Pivots[index];

	parent->SetName(w3dPivot.Name);
	parent->SetPosePosition(Vector3(w3dPivot.Translation.x, w3dPivot.Translation.y, w3dPivot.Translation.z));
	parent->SetPoseOrientation(Quaternion(
		w3dPivot.Rotation.x, w3dPivot.Rotation.y, -w3dPivot.Rotation.z, w3dPivot.Rotation.w));

	uint32 meshCount = _W3DFile->Meshes.Count();
	int32 meshIndex = -1;
	for (i=0; i<meshCount; i++)
	{
		const W3dMesh& w3dMesh = _W3DFile->Meshes[i];

		String pivotName;
		if ((w3dMesh.Header.Attributes & W3D_MESH_FLAG_GEOMETRY_TYPE_SKIN) != 0)
		{
			pivotName = String(w3dPivot.Name) + "_SKIN";
		}
		else
		{
			pivotName = w3dPivot.Name;
		}

		if (String::Compare(pivotName, w3dMesh.Header.MeshName, true) == 0)
		{
			meshIndex = i;
			break;
		}
	}

	if (meshIndex != -1)
	{
		const W3dMesh& w3dMesh = _W3DFile->Meshes[meshIndex];

		Array<Texture*> textures(w3dMesh.MaterialInfo.TextureCount);

		for (i=0; i<w3dMesh.MaterialInfo.TextureCount; i++)
		{
			textures[i] = CreateTexture(w3dMesh.Textures.Textures[i].Name);
		}

		Geometry* geometry = new Geometry();

		Array<Vector3> vertices(w3dMesh.Header.NumVertices);
		Array<Vector3> normals(w3dMesh.Header.NumVertices);
		Array<int32> indices(w3dMesh.Header.NumTris * 3);

		Memory::Copy(&vertices[0], w3dMesh.Vertices, w3dMesh.Header.NumVertices*sizeof(W3dVectorStruct));
		if (w3dMesh.VertexInfluences != NULL)
		{
			for (j=0; j<w3dMesh.Header.NumVertices; ++j)
			{
				Matrix4 m = Matrix4::Identity;
				int bone = w3dMesh.VertexInfluences[j].BoneIdx;
				while (bone > 0 && bone != index)
				{
					const W3dPivotStruct& w3dPivot =
						_W3DFile->Hierarchy.Pivots[bone];

					m *= Matrix4(Matrix3::CreateFromQuaternion(Quaternion(w3dPivot.Rotation.x,
						w3dPivot.Rotation.y, -w3dPivot.Rotation.z, w3dPivot.Rotation.w)), Vector3(w3dPivot.Translation.x, w3dPivot.Translation.y, w3dPivot.Translation.z));

					bone = w3dPivot.ParentIdx;
				}

				vertices[j] = Vector3::Transform(vertices[j], m);
			}
		}

		Memory::Copy(&normals[0], w3dMesh.VertexNormals, w3dMesh.Header.NumVertices*sizeof(W3dVectorStruct));

		geometry->SetVertices(vertices);
		geometry->SetNormals(normals);

		if (w3dMesh.MaterialInfo.PassCount > 0 && w3dMesh.MaterialPasses[0].TextureStageCount > 0)
		{
			Array<Vector2> texcoords(w3dMesh.Header.NumVertices);
			Memory::Copy(&texcoords[0], w3dMesh.MaterialPasses[0].TextureStages[0].TexCoords,
				w3dMesh.Header.NumVertices*sizeof(W3dTexCoordStruct));
			for (i=0; i<w3dMesh.Header.NumVertices; ++i)
				texcoords[i].Y = -texcoords[i].Y;
			geometry->SetTextureCoords(texcoords);
		}

		for (i=0; i<w3dMesh.Header.NumTris; ++i)
		{
			indices[3*i] = w3dMesh.Triangles[i].Vindex[0];
			indices[3*i+1] = w3dMesh.Triangles[i].Vindex[1];
			indices[3*i+2] = w3dMesh.Triangles[i].Vindex[2];
		}
		geometry->SetIndices(indices);

		MeshPart* meshPart;
		if (!geometry->CreatePrimitive(&meshPart))
		{
			Logger::Current()->Log(LogLevel::Error, _T("WestwoodW3DReader.LoadModel"),
				_T("Failed to create the primitive for the object: ") + String(w3dMesh.Header.MeshName));
			return;
		}

		ShaderMaterial* shader = new Shader();

		CullMode cullMode;
		if ((w3dMesh.Header.Attributes & W3D_MESH_FLAG_TWO_SIDED) != 0)
			cullMode = CullMode_None;
		else
			cullMode = CullMode_Back;

		ShaderTechnique* technique = new ShaderTechnique();

		for (i=0; i<w3dMesh.MaterialInfo.PassCount; i++)
		{
			ShaderPass* pass = new ShaderPass();

			pass->RasterizerState.CullMode = cullMode;
			pass->LightState.Lighting = false;//FIX

			const W3dVertexMaterialStruct& w3dVertexMaterial =
				w3dMesh.VertexMaterials.VertexMaterials[
				w3dMesh.MaterialPasses[i].VertexMaterialIDs[0]].VertexMaterialInfo;
			const W3dShaderStruct& w3dShader =
				w3dMesh.Shaders[w3dMesh.MaterialPasses[i].ShaderIDs[0]];

			switch (w3dShader.DepthCompare)
			{
			case W3DSHADER_DEPTHCOMPARE_PASS_NEVER:
				pass->DepthState.Function = ComparisonFunction_Never;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_LESS:
				pass->DepthState.Function = ComparisonFunction_Less;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_EQUAL:
				pass->DepthState.Function = ComparisonFunction_Equal;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_LEQUAL:
				pass->DepthState.Function = ComparisonFunction_LessEqual;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_GREATER:
				pass->DepthState.Function = ComparisonFunction_Greater;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_NOTEQUAL:
				pass->DepthState.Function = ComparisonFunction_NotEqual;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_GEQUAL:
				pass->DepthState.Function = ComparisonFunction_GreaterEqual;
				break;
			case W3DSHADER_DEPTHCOMPARE_PASS_ALWAYS:
				pass->DepthState.Function = ComparisonFunction_Always;
				break;
			default:
				pass->DepthState.Function = ComparisonFunction_LessEqual;
				break;
			}

			if (w3dShader.DepthMask == W3DSHADER_DEPTHMASK_WRITE_DISABLE)
				pass->DepthState.WriteEnable = false;
			else// if (w3dShader.DepthMask == W3DSHADER_DEPTHMASK_WRITE_ENABLE)
				pass->DepthState.WriteEnable = true;

			switch (w3dShader.DestBlend)
			{
			case W3DSHADER_DESTBLENDFUNC_ZERO:
				pass->AlphaState.DestinationBlend = BlendMode_Zero;
				break;
			case W3DSHADER_DESTBLENDFUNC_ONE:
				pass->AlphaState.DestinationBlend = BlendMode_One;
				break;
			case W3DSHADER_DESTBLENDFUNC_SRC_COLOR:
				pass->AlphaState.DestinationBlend = BlendMode_SourceColor;
				break;
			case W3DSHADER_DESTBLENDFUNC_ONE_MINUS_SRC_COLOR:
				pass->AlphaState.DestinationBlend = BlendMode_InvSourceColor;
				break;
			case W3DSHADER_DESTBLENDFUNC_SRC_ALPHA:
				pass->AlphaState.DestinationBlend = BlendMode_SourceAlpha;
				break;
			case W3DSHADER_DESTBLENDFUNC_ONE_MINUS_SRC_ALPHA:
				pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
				break;
			case W3DSHADER_DESTBLENDFUNC_SRC_COLOR_PREFOG:
				pass->AlphaState.DestinationBlend = BlendMode_SourceColor;
				break;
			default:
				pass->AlphaState.DestinationBlend = BlendMode_Zero;
				break;
			}

			switch (w3dShader.SrcBlend)
			{
			case W3DSHADER_SRCBLENDFUNC_ZERO:
				pass->AlphaState.SourceBlend = BlendMode_Zero;
				break;
			case W3DSHADER_SRCBLENDFUNC_ONE:
				pass->AlphaState.SourceBlend = BlendMode_One;
				break;
			case W3DSHADER_SRCBLENDFUNC_SRC_ALPHA:
				pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
				break;
			case W3DSHADER_SRCBLENDFUNC_ONE_MINUS_SRC_ALPHA:
				pass->AlphaState.SourceBlend = BlendMode_InvSourceAlpha;
				break;
			default:
				pass->AlphaState.SourceBlend = BlendMode_One;
				break;
			}

			if (w3dShader.AlphaTest == W3DSHADER_ALPHATEST_DISABLE)
				pass->AlphaState.TestEnable = false;
			else// if (w3dShader.AlphaTest == W3DSHADER_ALPHATEST_ENABLE)
				pass->AlphaState.TestEnable = true;

			if (w3dShader.AlphaTest == W3DSHADER_ALPHATEST_ENABLE)
			{
				pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
				pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
			}

			pass->AlphaState.BlendEnable[0] = true;

			pass->MaterialState.AmbientColor = Color32(
				w3dVertexMaterial.Ambient.R/255.0f,
				w3dVertexMaterial.Ambient.G/255.0f,
				w3dVertexMaterial.Ambient.B/255.0f,
				w3dVertexMaterial.Opacity);

			pass->MaterialState.DiffuseColor = Color32(
				w3dVertexMaterial.Diffuse.R/255.0f,
				w3dVertexMaterial.Diffuse.G/255.0f,
				w3dVertexMaterial.Diffuse.B/255.0f,
				w3dVertexMaterial.Opacity);

			pass->MaterialState.SpecularColor = Color32(
				w3dVertexMaterial.Specular.R/255.0f,
				w3dVertexMaterial.Specular.G/255.0f,
				w3dVertexMaterial.Specular.B/255.0f,
				w3dVertexMaterial.Opacity);

			pass->MaterialState.EmissiveColor = Color32(
				w3dVertexMaterial.Emissive.R/255.0f,
				w3dVertexMaterial.Emissive.G/255.0f,
				w3dVertexMaterial.Emissive.B/255.0f,
				w3dVertexMaterial.Opacity);

			pass->MaterialState.Shininess = w3dVertexMaterial.Shininess;

			for (j=0; j<w3dMesh.MaterialPasses[i].TextureStageCount; j++)
			{
				SamplerState* samplerState = new SamplerState();
				samplerState->SetTexture(textures[w3dMesh.MaterialPasses[i].TextureStages[j].TextureIDs[0]]);
				pass->AddSamplerState(samplerState);

				TextureState* textureState = new TextureState();
				pass->AddTextureState(textureState);
			}

			technique->AddPass(pass);
		}

		shader->AddTechnique(technique);

		Mesh* mesh = new Mesh();
		mesh->SetParentBone(parent);
		mesh->AddMeshPart(meshPart);
		mesh->SetShader(shader);
		mesh->SetBoundingBox(geometry->GetBoundingBox());
		model->AddMesh(mesh);
	}

	// Convert children
	for (i=0; i<_W3DFile->Hierarchy.Hierarchy.NumPivots; ++i)
	{
		const W3dPivotStruct& w3dPivot = _W3DFile->Hierarchy.Pivots[i];
		if (w3dPivot.ParentIdx == index)
		{
			Bone* child = new Bone(parent->GetSkeleton());
			Convert(child, model, i);
			parent->AddChild(child);

			parent->GetSkeleton()->AddBone(child);
		}
	}

	//frame->UpdateBoundingBox();
}

}
