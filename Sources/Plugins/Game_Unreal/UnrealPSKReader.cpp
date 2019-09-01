/*=============================================================================
UnrealPSKReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "UnrealPSKReader.h"

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("UnrealPSKReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("UnrealPSKReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("UnrealPSKReader"), ##message##);

namespace SE_Unreal
{

UnrealPSKReader::UnrealPSKReader() :
	ModelReader()
{
}

UnrealPSKReader::~UnrealPSKReader()
{
	Destroy();
}

Model* UnrealPSKReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("UnrealPSKReader.LoadModel"),
			_T("No active renderer."));
		return NULL;
	}

	if (stream.GetStreamType() != StreamType_File)
	{
		return NULL;
	}

	Destroy();

	_fileName = ((FileStream*)&stream)->GetFileName();
	_path = Path::GetDirectoryName(_fileName);
	_stream = new BinaryStream(&stream);
	if (!ReadPSK())
	{
		SE_DELETE(_stream);
		return NULL;
	}
	SE_DELETE(_stream);

	File pkaFile(Path::Combine(_path, String::Concat(Path::GetFileNameWithoutExtension(_fileName), ".psa")));
	if (pkaFile.Exists())
	{
		FileStreamPtr pkaStream = pkaFile.Open(FileMode_Open, FileAccess_Read);
		if (pkaStream == NULL)
		{
			return NULL;
		}
		_stream = new BinaryStream(pkaStream.Get());
		if (!ReadPSA())
		{
			SE_DELETE(_stream);
			return NULL;
		}
		SE_DELETE(_stream);
	}
	return CreateModel();
}

bool UnrealPSKReader::ReadPSK()
{
	VChunkHeader header;

	// Main
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "ACTRHEAD", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: ACTRHEAD");
		return false;
	}

	// Points
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "PNTS0000", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: PNTS0000");
		return false;
	}
	_pskFile.points.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_pskFile.points[0], sizeof(VPoint) * header.DataCount);

	// Vertices
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "VTXW0000", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: VTXW0000");
		return false;
	}
	_pskFile.vertices.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_pskFile.vertices[0], sizeof(VVertex) * header.DataCount);

	// Triangles
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "FACE0000", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: FACE0000");
		return false;
	}
	_pskFile.triangles.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_pskFile.triangles[0], sizeof(VTriangle) * header.DataCount);

	// Materials
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "MATT0000", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: MATT0000");
		return false;
	}
	_pskFile.materials.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_pskFile.materials[0], sizeof(VMaterial) * header.DataCount);

	// Bones
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "REFSKELT", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: REFSKELT");
		return false;
	}
	_pskFile.bones.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_pskFile.bones[0], sizeof(VBone) * header.DataCount);

	// Bone Influences
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "RAWWEIGHTS", 10) != 0)
	{
		SE_LOGERROR("Invalid chunk header: RAWWEIGHTS");
		return false;
	}
	_pskFile.rawBoneInfluence.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_pskFile.rawBoneInfluence[0], sizeof(VRawBoneInfluence) * header.DataCount);

	return true;
}

bool UnrealPSKReader::ReadPSA()
{
	VChunkHeader header;

	// Main
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "ANIMHEAD", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: ANIMHEAD");
		return false;
	}

	// Named Bones
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "BONENAMES", 9) != 0)
	{
		SE_LOGERROR("Invalid chunk header: BONENAMES");
		return false;
	}
	_psaFile.namedBones.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_psaFile.namedBones[0], sizeof(FNamedBoneBinary) * header.DataCount);

	// Anim Infos
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "ANIMINFO", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: ANIMINFO");
		return false;
	}
	_psaFile.animInfos.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_psaFile.animInfos[0], sizeof(AnimInfoBinary) * header.DataCount);

	// Anim Keys
	_stream->Read((SEbyte*)&header, sizeof(VChunkHeader));
	if (Memory::Compare(header.ChunkID, "ANIMKEYS", 8) != 0)
	{
		SE_LOGERROR("Invalid chunk header: ANIMKEYS");
		return false;
	}
	_psaFile.quatAnimKeys.Resize(header.DataCount);
	_stream->Read((SEbyte*)&_psaFile.quatAnimKeys[0], sizeof(VQuatAnimKey) * header.DataCount);

	return true;
}

void UnrealPSKReader::Destroy()
{
	_pskFile.points.Clear();
	_pskFile.vertices.Clear();
	_pskFile.triangles.Clear();
	_pskFile.materials.Clear();
	_pskFile.bones.Clear();
	_pskFile.rawBoneInfluence.Clear();

	_psaFile.namedBones.Clear();
	_psaFile.animInfos.Clear();
	_psaFile.quatAnimKeys.Clear();
}

ShaderMaterial* UnrealPSKReader::CreateMaterial()
{
	DefaultMaterial* shader = new DefaultMaterial();

	return shader;
}

void BuildPose(PSKFile& pskFile, Bone* bone)
{
	VBone* vbone = &pskFile.bones[bone->GetIndex()];

	Vector3 position = Vector3((real32*)&vbone->BonePos.Position);

	// The first quaternion is stored with the X and Z negated. 
	Quaternion orientation;
	if (bone->GetIndex() != 0)
	{
		orientation = Quaternion(-vbone->BonePos.Orientation.X, -vbone->BonePos.Orientation.Y,
			-vbone->BonePos.Orientation.Z, vbone->BonePos.Orientation.W);
	}
	else
	{
		orientation = Quaternion(vbone->BonePos.Orientation.X, -vbone->BonePos.Orientation.Y,
			vbone->BonePos.Orientation.Z, vbone->BonePos.Orientation.W);
	}

	bone->SetLocalPosition(position);
	bone->SetLocalOrientation(orientation);
	bone->SetLocalPoseTransform(position, orientation, Vector3::One);

	for (int i = 0; i < bone->GetChildCount(); ++i)
	{
		BuildPose(pskFile, bone->GetChildByIndex(i));
	}
}

Model* UnrealPSKReader::CreateModel()
{
	int i;
	RenderSystem* renderSystem = RenderSystem::Current();

	Model* model = new Model();
	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	int boneCount = _pskFile.bones.Count();

	// Create the bones
	for (i = 0; i < boneCount; ++i)
	{
		VBone* vbone = &_pskFile.bones[i];
		Bone* bone = skeleton->AddBone();

		bone->SetName(String((SEchar*)vbone->Name));
	}

	// Setup the hierarchy.
	for (i = 1; i < boneCount; ++i)
	{
		VBone* vbone = &_pskFile.bones[i];
		Bone* bone = skeleton->GetBoneByIndex(i);
		skeleton->GetBoneByIndex(vbone->ParentIndex)->AddChild(bone);
	}

	// The first bone is the root bone
	Bone* rootBone = skeleton->GetBoneByIndex(0);
	skeleton->SetRootBone(rootBone);
	BuildPose(_pskFile, rootBone);
	rootBone->Update();

	Mesh* mesh = CreateMesh();
	if (mesh != NULL)
	{
		//mesh->SetParentBone(rootBone);
		model->AddMesh(mesh);
	}

	AnimationSet* animationSet = CreateAnimation(skeleton);
	if (animationSet != NULL)
	{
		skeleton->SetAnimationSet(animationSet);
	}

	return model;
}

Mesh* UnrealPSKReader::CreateMesh()
{
	int i;
	RenderSystem* renderSystem = RenderSystem::Current();

	// Create the mesh
	Mesh* mesh = new Mesh();
	mesh->SetName(Path::GetFileNameWithoutExtension(_fileName));

	int boneCount = _pskFile.bones.Count();
	int faceCount = _pskFile.triangles.Count();
	int vertexCount = _pskFile.vertices.Count();
	int weightCount = _pskFile.rawBoneInfluence.Count();

	BaseArray<Vector3> faceNormals(faceCount);
	Vector3 point1, point2, point3;
	for (i = 0; i < faceCount; ++i)
	{
		VPoint* vpoint1 = &_pskFile.points[_pskFile.vertices[_pskFile.triangles[i].WedgeIndex[0]].PointIndex];
		VPoint* vpoint2 = &_pskFile.points[_pskFile.vertices[_pskFile.triangles[i].WedgeIndex[1]].PointIndex];
		VPoint* vpoint3 = &_pskFile.points[_pskFile.vertices[_pskFile.triangles[i].WedgeIndex[2]].PointIndex];
		point1 = Vector3(vpoint1->Point.X, vpoint1->Point.Y, vpoint1->Point.Z);
		point2 = Vector3(vpoint2->Point.X, vpoint2->Point.Y, vpoint2->Point.Z);
		point3 = Vector3(vpoint3->Point.X, vpoint3->Point.Y, vpoint3->Point.Z);
		Vector3 normal = Vector3::Cross(point2 - point1, point3 - point1);
		normal.Normalize();
		faceNormals[i] = normal;
	}

	BaseArray<Vector3> normals(vertexCount);
	for (int v = 0; v < vertexCount; ++v)
	{
		int adjacentFaceCount = 0;
		BaseArray<int> adjacentFaces(faceCount);
		for (i = 0; i < faceCount; ++i)
		{
			if (_pskFile.triangles[i].WedgeIndex[0] == v)
				adjacentFaces[adjacentFaceCount++] = i;
			else if (_pskFile.triangles[i].WedgeIndex[1] == v)
				adjacentFaces[adjacentFaceCount++] = i;
			else if (_pskFile.triangles[i].WedgeIndex[2] == v)
				adjacentFaces[adjacentFaceCount++] = i;
		}

		Vector3 normal = Vector3::Zero;
		for (i = 0; i < adjacentFaceCount; ++i)
		{
			normal += faceNormals[adjacentFaces[i]];
		}
		normal /= adjacentFaceCount;
		normal.Normalize();
		normals[v] = normal;
	}

	// Skinning
	Skin* skin = NULL;
	std::vector<bool> blendBool(vertexCount * 4);
	BaseArray<uint8> blendIndices(vertexCount * 4);
	BaseArray<real32> blendWeights(vertexCount * 4);

	if (weightCount > 0)
	{
		int pointCount = _pskFile.points.Count();
		std::vector<bool> pointBool(pointCount * 4);
		BaseArray<uint8> pointIndices(pointCount * 4);
		BaseArray<real32> pointWeights(pointCount * 4);

		for (i = 0; i < pointCount; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				pointBool[4*i+k] = false;
				pointIndices[4*i+k] = 0;
				pointWeights[4*i+k] = 0.0f;
			}
		}

		int boneIndex;
		int vertexIndex;
		real32 weight;

		for (i = 0; i < weightCount; ++i)
		{
			boneIndex = _pskFile.rawBoneInfluence[i].BoneIndex;
			vertexIndex = _pskFile.rawBoneInfluence[i].PointIndex;
			weight = _pskFile.rawBoneInfluence[i].Weight;

			for (int k = 0; k < 4; ++k)
			{
				if (!pointBool[4*vertexIndex+k])
				{
					pointIndices[4*vertexIndex+k] = boneIndex;
					pointWeights[4*vertexIndex+k] = weight;
					pointBool[4*vertexIndex+k] = true;
					break;
				}
			}
		}

		skin = new Skin();
		skin->SkinVertices.Resize(boneCount);
		for (i = 0; i < boneCount; ++i)
		{
			skin->SkinVertices[i].BoneIndex = i;
		}

		for (i = 0; i < vertexCount; ++i)
		{
			uint16 pointIndex = _pskFile.vertices[i].PointIndex;
			Memory::Copy(&blendIndices[4*i], &pointIndices[4*pointIndex], sizeof(uint8) * 4);
			Memory::Copy(&blendWeights[4*i], &pointWeights[4*pointIndex], sizeof(real32) * 4);
		}
	}

	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* vertexBuffer = NULL;
	HardwareBuffer* indexBuffer = NULL;

	Vector3 position;
	Vector2 texCoords;

	if (!renderSystem->CreateVertexLayout(&vertexLayout))
	{
		goto Error;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	if (weightCount > 0)
	{
		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float4, VertexSemantic_BlendWeight));
		offset += VertexElement::GetTypeSize(VertexFormat_Float4);

		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_UByte4, VertexSemantic_BlendIndices));
		offset += VertexElement::GetTypeSize(VertexFormat_UByte4);
	}

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	if (!renderSystem->CreateVertexBuffer(vertexLayout->GetSize() * vertexCount, HardwareBufferUsage_Static, &vertexBuffer))
	{
		goto Error;
	}

	SEbyte* lockedVertices;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedVertices);
	for (i = 0; i < vertexCount; ++i)
	{
		VPoint* point = &_pskFile.points[_pskFile.vertices[i].PointIndex];
		position = Vector3(point->Point.X, point->Point.Y, point->Point.Z);
		Memory::Copy(lockedVertices, &position, sizeof(Vector3));
		lockedVertices += sizeof(Vector3);

		if (weightCount > 0)
		{
			Memory::Copy(lockedVertices, &blendWeights[4*i], sizeof(real32) * 4);
			lockedVertices += sizeof(real32) * 4;

			Memory::Copy(lockedVertices, &blendIndices[4*i], sizeof(uint8) * 4);
			lockedVertices += sizeof(uint8) * 4;
		}

		Memory::Copy(lockedVertices, &normals[i], sizeof(Vector3));
		lockedVertices += sizeof(Vector3);

		texCoords = Vector2(_pskFile.vertices[i].U, _pskFile.vertices[i].V);
		Memory::Copy(lockedVertices, &texCoords, sizeof(Vector2));
		lockedVertices += sizeof(Vector2);
	}
	vertexBuffer->Unmap();

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	mesh->SetVertexData(vertexData);

	int triangleCount = 3 * faceCount;
	int materialCount = _pskFile.materials.Count();
	if (materialCount == 1)
	{
		if (!renderSystem->CreateIndexBuffer(sizeof(uint16) * triangleCount, IndexBufferFormat_Int16, HardwareBufferUsage_Static, &indexBuffer))
		{
			goto Error;
		}

		SEbyte* lockedIndices;
		indexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedIndices);
		for (i = 0; i < faceCount; ++i)
		{
			Memory::Copy(lockedIndices, &_pskFile.triangles[i].WedgeIndex[2], sizeof(uint16));
			lockedIndices += sizeof(uint16);
			Memory::Copy(lockedIndices, &_pskFile.triangles[i].WedgeIndex[1], sizeof(uint16));
			lockedIndices += sizeof(uint16);
			Memory::Copy(lockedIndices, &_pskFile.triangles[i].WedgeIndex[0], sizeof(uint16));
			lockedIndices += sizeof(uint16);
		}
		indexBuffer->Unmap();

		int indexCount = faceCount * 3;
		IndexData* indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;

		MeshPart* meshPart = new MeshPart();

		meshPart->SetVertexData(vertexData);

		meshPart->SetIndexData(indexData);
		meshPart->SetIndexed(true);
		meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, indexCount);

		mesh->AddMeshPart(meshPart);
		meshPart->SetShader(CreateMaterial());
	}
	else
	{
		// Sort the faces by material
		BaseArray< BaseArray<VTriangle> > faceArray(materialCount);
		for (i = 0; i < materialCount; ++i)
		{
			faceArray[i].SetCapacity(faceCount);
		}

		for (i = 0; i < faceCount; ++i)
		{
			faceArray[_pskFile.triangles[i].MatIndex].Add(_pskFile.triangles[i]);
		}

		if (!renderSystem->CreateIndexBuffer(sizeof(uint16) * triangleCount, IndexBufferFormat_Int16, HardwareBufferUsage_Static, &indexBuffer))
		{
			goto Error;
		}

		SEbyte* lockedIndices;
		indexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedIndices);
		for (i = 0; i < materialCount; ++i)
		{
			int primitiveCount = faceArray[i].Count();
			for (int f = 0; f < primitiveCount; ++f)
			{
				Memory::Copy(lockedIndices, &faceArray[i][f].WedgeIndex[2], sizeof(uint16));
				lockedIndices += sizeof(uint16);
				Memory::Copy(lockedIndices, &faceArray[i][f].WedgeIndex[1], sizeof(uint16));
				lockedIndices += sizeof(uint16);
				Memory::Copy(lockedIndices, &faceArray[i][f].WedgeIndex[0], sizeof(uint16));
				lockedIndices += sizeof(uint16);
			}
		}
		indexBuffer->Unmap();

		int indexCount = faceCount * 3;
		IndexData* indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;

		// Split the mesh in multiple mesh parts if there are multiple materials
		int primitiveCount = 0;
		int lastCount = 0;
		for (i = 0; i < materialCount; ++i)
		{
			MeshPart* meshPart = new MeshPart();

			meshPart->SetVertexData(vertexData);

			meshPart->SetIndexData(indexData);
			meshPart->SetIndexed(true);

			primitiveCount = faceArray[i].Count();
			meshPart->SetPrimitiveType(PrimitiveType_TriangleList);
			meshPart->SetPrimitiveCount(primitiveCount);
			meshPart->SetStartIndex(lastCount * 3);
			lastCount += primitiveCount;

			mesh->AddMeshPart(meshPart);
			meshPart->SetShader(CreateMaterial());
		}
	}

	if (skin != NULL)
	{
		mesh->SetSkin(skin);
	}

	return mesh;

Error:
	SE_DELETE(vertexLayout);
	SE_DELETE(vertexBuffer);
	SE_DELETE(indexBuffer);
	return NULL;
}

AnimationSet* UnrealPSKReader::CreateAnimation(Skeleton* skeleton)
{
	int i, j, k;

	int boneCount = _psaFile.namedBones.Count();
	int animCount = _psaFile.animInfos.Count();

	if (animCount == 0)
	{
		return NULL;
	}

	AnimationSet* animationSet = new AnimationSet();
	int animIndex = 0;
	int keyIndex = 0;

	for (i = 0; i < animCount; ++i)
	{
		AnimInfoBinary* animInfo = &_psaFile.animInfos[i];

		AnimationSequence* sequence = new AnimationSequence();
		sequence->SetName(String((SEchar*)animInfo->Name));
		TimeValue endTime;

		for (j = 0; j < boneCount; ++j)
		{
			BoneAnimationTrack* track = new BoneAnimationTrack();
			track->SetBone(skeleton->GetBoneByIndex(j));

			TimeValue keyTime;
			Vector3 translation;
			Quaternion rotation;

			for (k = 0; k < animInfo->NumRawFrames; ++k)
			{
				keyIndex = animIndex + k * boneCount + j;
				VQuatAnimKey* vQuatAnimKey = &_psaFile.quatAnimKeys[keyIndex];

				keyTime += TimeValue(vQuatAnimKey->Time / animInfo->AnimRate);
				TransformKeyFrame* keyFrame = (TransformKeyFrame*)track->AddKeyFrame(keyTime);
				keyFrame->SetTransformType((TransformKeyFrameType)
					(TransformKeyFrameType_Translation | TransformKeyFrameType_Rotation));

				translation = Vector3((real32*)&vQuatAnimKey->Position);
				keyFrame->SetTranslation(translation);

				if ((j % boneCount) != 0)
				{
					rotation = Quaternion(-vQuatAnimKey->Orientation.X, -vQuatAnimKey->Orientation.Y,
						-vQuatAnimKey->Orientation.Z, vQuatAnimKey->Orientation.W);
				}
				else
				{
					rotation = Quaternion(vQuatAnimKey->Orientation.X, -vQuatAnimKey->Orientation.Y,
						vQuatAnimKey->Orientation.Z, vQuatAnimKey->Orientation.W);
				}
				keyFrame->SetRotation(rotation);
			}

			track->SetEndTime(keyTime);
			sequence->AddAnimationTrack(track);

			if (keyTime > endTime)
			{
				endTime = keyTime;
			}
		}

		sequence->SetEndTime(endTime);
		animationSet->AddAnimationSequence(sequence);

		animIndex +=  boneCount * animInfo->NumRawFrames;
	}

	return animationSet;
}

}
