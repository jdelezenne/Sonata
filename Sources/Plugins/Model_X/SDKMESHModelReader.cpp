/*=============================================================================
SDKMESHModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SDKMESHModelReader.h"

namespace SE_X
{

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("SDKMESHModelReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("SDKMESHModelReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("SDKMESHModelReader"), ##message##);

SDKMESHModelReader::SDKMESHModelReader() :
	ModelReader()
{
	_sdkmesh.m_pStaticMeshData = NULL;
	_sdkmesh.m_pAnimationData = NULL;

	_sdkmesh.m_pMeshHeader = NULL;
	_sdkmesh.m_pVertexBufferArray = NULL;
	_sdkmesh.m_pIndexBufferArray = NULL;
	_sdkmesh.m_pMeshArray = NULL;
	_sdkmesh.m_pSubsetArray = NULL;
	_sdkmesh.m_pFrameArray = NULL;
	_sdkmesh.m_pMaterialArray = NULL;

	_sdkmesh.m_pAnimationHeader = NULL;
	_sdkmesh.m_pAnimationFrameData = NULL;
}

SDKMESHModelReader::~SDKMESHModelReader()
{
	Destroy();
}

void SDKMESHModelReader::D3DXMATRIXToTransform(D3DXMATRIX* d3dxmatrix, Vector3& position, Quaternion& orientation, Vector3& scale)
{
	Matrix4 matrix = Matrix4::Transpose(Matrix4((real32*)d3dxmatrix->m));
	position = Vector3(matrix.M03, matrix.M13, matrix.M23);

	//TODO: unscale the matrix
	Matrix3 rotationMatrix = Matrix3(
		matrix.M00, matrix.M01, matrix.M02,
		matrix.M10, matrix.M11, matrix.M12,
		matrix.M20, matrix.M21, matrix.M22);

	orientation = Quaternion::CreateFromRotationMatrix(rotationMatrix);
	scale = Vector3::One;
}

PrimitiveType SDKMESHModelReader::GetPrimitiveType(SDKMESH_PRIMITIVE_TYPE primType)
{
	switch (primType)
	{
	case PT_TRIANGLE_LIST:
		return PrimitiveType_TriangleList;
	case PT_TRIANGLE_STRIP:
		return PrimitiveType_TriangleStrip;
	case PT_LINE_LIST:
		return PrimitiveType_LineList;
	case PT_LINE_STRIP:
		return PrimitiveType_LineStrip;
	case PT_POINT_LIST:
		return PrimitiveType_PointList;
	default:
		return PrimitiveType_TriangleList;
	};
}

IndexBufferFormat SDKMESHModelReader::GetIBFormat(UINT indexType)
{
	switch (indexType)
	{
	case IT_16BIT:
		return IndexBufferFormat_Int16;
	case IT_32BIT:
		return IndexBufferFormat_Int32;
	default:
		return IndexBufferFormat_Int16;
	};
}

VertexFormat SDKMESHModelReader::GetVertexFormatType(D3DDECLTYPE value)
{
	switch (value)
	{
	case D3DDECLTYPE_FLOAT1: return VertexFormat_Float1;
	case D3DDECLTYPE_FLOAT2: return VertexFormat_Float2;
	case D3DDECLTYPE_FLOAT3: return VertexFormat_Float3;
	case D3DDECLTYPE_FLOAT4: return VertexFormat_Float4;
	case D3DDECLTYPE_D3DCOLOR: return VertexFormat_Color;
	case D3DDECLTYPE_UBYTE4: return VertexFormat_UByte4;
	case D3DDECLTYPE_SHORT2: return VertexFormat_Short2;
	case D3DDECLTYPE_SHORT4: return VertexFormat_Short4;
	case D3DDECLTYPE_UBYTE4N: return VertexFormat_UByte4N;
	default: return VertexFormat_Float1;
	}
}

VertexSemantic SDKMESHModelReader::GetVertexSemantic(D3DDECLUSAGE value)
{
	switch (value)
	{
	case D3DDECLUSAGE_POSITION: return VertexSemantic_Position;
	case D3DDECLUSAGE_BLENDWEIGHT: return VertexSemantic_BlendWeight;
	case D3DDECLUSAGE_BLENDINDICES: return VertexSemantic_BlendIndices;
	case D3DDECLUSAGE_NORMAL: return VertexSemantic_Normal;
	case D3DDECLUSAGE_PSIZE: return VertexSemantic_PSize;
	case D3DDECLUSAGE_TEXCOORD: return VertexSemantic_TextureCoordinate;
	case D3DDECLUSAGE_TANGENT: return VertexSemantic_Tangent;
	case D3DDECLUSAGE_BINORMAL: return VertexSemantic_Binormal;
	case D3DDECLUSAGE_TESSFACTOR: return VertexSemantic_TesselateFactor;
	case D3DDECLUSAGE_POSITIONT: return VertexSemantic_PositionTransformed;
	case D3DDECLUSAGE_COLOR: return VertexSemantic_Color;
	case D3DDECLUSAGE_FOG: return VertexSemantic_Fog;
	case D3DDECLUSAGE_DEPTH: return VertexSemantic_Depth;
	case D3DDECLUSAGE_SAMPLE: return VertexSemantic_Sample3;
	default: return VertexSemantic_Position;
	}
}

SDKMESH_FRAME* SDKMESHModelReader::FindFrame(const String& name)
{
	for (UINT i = 0; i < _sdkmesh.m_pMeshHeader->NumFrames; i++)
	{
		if (String::Compare(_sdkmesh.m_pFrameArray[i].Name, name) == 0)
		{
			return &_sdkmesh.m_pFrameArray[i];
		}
	}
	return NULL;
}

Model* SDKMESHModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		SE_LOGERROR(_T("No active renderer."));
		return NULL;
	}

	_fileName = ((FileStream*)&stream)->GetFileName();
	_path = Path::GetDirectoryName(_fileName);

	_sdkmesh.m_pStaticMeshData = new BYTE[stream.GetLength()];
	stream.Read(_sdkmesh.m_pStaticMeshData, stream.GetLength());

	if (!ReadModel())
	{
		return NULL;
	}

	if (!ReadAnimation())
	{
		return NULL;
	}

	return CreateModel();
}

bool SDKMESHModelReader::ReadModel()
{
	// Pointer fixup
	_sdkmesh.m_pMeshHeader = (SDKMESH_HEADER*)_sdkmesh.m_pStaticMeshData;
	_sdkmesh.m_pVertexBufferArray = (SDKMESH_VERTEX_BUFFER_HEADER*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->VertexStreamHeadersOffset);
	_sdkmesh.m_pIndexBufferArray = (SDKMESH_INDEX_BUFFER_HEADER*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->IndexStreamHeadersOffset);
	_sdkmesh.m_pMeshArray = (SDKMESH_MESH*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->MeshDataOffset);
	_sdkmesh.m_pSubsetArray = (SDKMESH_SUBSET*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->SubsetDataOffset);
	_sdkmesh.m_pFrameArray = (SDKMESH_FRAME*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->FrameDataOffset);
	_sdkmesh.m_pMaterialArray = (SDKMESH_MATERIAL*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->MaterialDataOffset);

	// Setup subsets
	for (UINT i=0; i<_sdkmesh.m_pMeshHeader->NumMeshes; i++)
	{
		_sdkmesh.m_pMeshArray[i].pSubsets = (UINT*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshArray[i].SubsetOffset);
		_sdkmesh.m_pMeshArray[i].pFrameInfluences = (UINT*)(_sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshArray[i].FrameInfluenceOffset);
	}

	// error condition
	if (_sdkmesh.m_pMeshHeader->Version != SDKMESH_FILE_VERSION)
	{
		return false;
	}

	return true;
}

bool SDKMESHModelReader::ReadAnimation()
{
	// Opens the animation file if one exists
	String animPath = String::Concat(_fileName, "_anim");
	File animFile(animPath);
	if (!animFile.Exists())
	{
		return true;
	}

	FileStreamPtr stream = animFile.Open(FileMode_Open, FileAccess_Read);
	if (stream == NULL)
	{
		return false;
	}

	SDKANIMATION_FILE_HEADER fileHeader;
	stream->Read((SEbyte*)&fileHeader, sizeof(SDKANIMATION_FILE_HEADER));

	_sdkmesh.m_pAnimationData = new BYTE[(sizeof(SDKANIMATION_FILE_HEADER) + fileHeader.AnimationDataSize)];
	stream->Seek(0, SeekOrigin_Begin);
	stream->Read(_sdkmesh.m_pAnimationData, sizeof(SDKANIMATION_FILE_HEADER) + fileHeader.AnimationDataSize);

	_sdkmesh.m_pAnimationHeader = (SDKANIMATION_FILE_HEADER*)_sdkmesh.m_pAnimationData;
	_sdkmesh.m_pAnimationFrameData = (SDKANIMATION_FRAME_DATA*)(_sdkmesh.m_pAnimationData + _sdkmesh.m_pAnimationHeader->AnimationDataOffset);

	for (UINT i = 0; i < _sdkmesh.m_pAnimationHeader->NumFrames; i++)
	{
		_sdkmesh.m_pAnimationFrameData[i].pAnimationData = (SDKANIMATION_DATA*)(_sdkmesh.m_pAnimationData + _sdkmesh.m_pAnimationFrameData[i].DataOffset + sizeof(SDKANIMATION_FILE_HEADER));
        SDKMESH_FRAME* pFrame = FindFrame(_sdkmesh.m_pAnimationFrameData[i].FrameName);
		if (pFrame != NULL)
		{
			pFrame->AnimationDataIndex = i;
		}
	}

	return true;
}

void SDKMESHModelReader::Destroy()
{
	SE_DELETE_ARRAY(_sdkmesh.m_pStaticMeshData);
	SE_DELETE_ARRAY(_sdkmesh.m_pAnimationData);

	_sdkmesh.m_pMeshHeader = NULL;
	_sdkmesh.m_pVertexBufferArray = NULL;
	_sdkmesh.m_pIndexBufferArray = NULL;
	_sdkmesh.m_pMeshArray = NULL;
	_sdkmesh.m_pSubsetArray = NULL;
	_sdkmesh.m_pFrameArray = NULL;
	_sdkmesh.m_pMaterialArray = NULL;

	_sdkmesh.m_pAnimationHeader = NULL;
	_sdkmesh.m_pAnimationFrameData = NULL;
}

Model* SDKMESHModelReader::CreateModel()
{
	_model = new Model();
	Skeleton* skeleton = new Skeleton();
	_model->SetSkeleton(skeleton);

	CreateFrames(_sdkmesh.m_pFrameArray, _sdkmesh.m_pMeshHeader->NumFrames);

	// Setup buffer data pointer
	BYTE* pBufferData = _sdkmesh.m_pStaticMeshData + _sdkmesh.m_pMeshHeader->HeaderSize + _sdkmesh.m_pMeshHeader->NonBufferDataSize;

	// Get the start of the buffer data
	UINT64 BufferDataStart = _sdkmesh.m_pMeshHeader->HeaderSize + _sdkmesh.m_pMeshHeader->NonBufferDataSize;

	// Create VBs
	for (UINT i = 0; i < _sdkmesh.m_pMeshHeader->NumVertexBuffers; i++)
	{
		BYTE* pVertices = NULL;
		pVertices = (BYTE*)(pBufferData + (_sdkmesh.m_pVertexBufferArray[i].DataOffset - BufferDataStart));
		CreateVertexBuffer(&_sdkmesh.m_pVertexBufferArray[i], pVertices);
	}

	// Create IBs
	for (UINT i = 0; i < _sdkmesh.m_pMeshHeader->NumIndexBuffers; i++)
	{
		BYTE* pIndices = NULL;
		pIndices = (BYTE*)(pBufferData + (_sdkmesh.m_pIndexBufferArray[i].DataOffset - BufferDataStart));
		CreateIndexBuffer(&_sdkmesh.m_pIndexBufferArray[i], pIndices);
	}

	CreateMaterials(_sdkmesh.m_pMaterialArray, _sdkmesh.m_pMeshHeader->NumMaterials);
	CreateMeshes(_sdkmesh.m_pMeshArray, _sdkmesh.m_pMeshHeader->NumMeshes);
	CreateHierarchy();

	for (int i = 0; i < skeleton->GetBoneCount(); i++)
	{
		Bone* bone = skeleton->GetBoneByIndex(i);
		bone->SetPoseTransform(bone->GetGlobalTransform());
	}

	if (_sdkmesh.m_pAnimationData != NULL)
	{
		CreateAnimation();
	}

	return _model;
}

void SDKMESHModelReader::CreateFrames(SDKMESH_FRAME* pFrames, UINT numFrames)
{
	Skeleton* skeleton = _model->GetSkeleton();
	// Create the bones
	for (UINT frame = 0; frame < numFrames; frame++)
	{
		SDKMESH_FRAME* pFrame = &pFrames[frame];

		Bone* bone = skeleton->AddBone();
		bone->SetName(pFrame->Name);

		Vector3 position;
		Quaternion orientation;
		Vector3 scale;
		D3DXMATRIXToTransform(&pFrame->Matrix, position, orientation, scale);
		bone->SetLocalPosition(position);
		bone->SetLocalOrientation(orientation);
		bone->SetLocalScale(scale);
	}
}

void SDKMESHModelReader::CreateVertexBuffer(SDKMESH_VERTEX_BUFFER_HEADER* pHeader, void* pVertices)
{
	RenderSystem* renderSystem = RenderSystem::Current();

	HardwareBuffer* vertexBuffer;
	if (!renderSystem->CreateVertexBuffer(pHeader->SizeBytes, HardwareBufferUsage_Static, &vertexBuffer))
	{
		_vertexStreams.Add(SDKMESHVertexStream());
		return;
	}

	void* lockedVertices;
	vertexBuffer->Map(HardwareBufferMode_Normal, &lockedVertices);
	Memory::Copy(lockedVertices, pVertices, pHeader->SizeBytes);
	vertexBuffer->Unmap();

	SDKMESHVertexStream vertexStream;
	vertexStream._vertexBuffer = vertexBuffer;
	vertexStream._strideBytes = pHeader->StrideBytes;

	for (UINT vertexElement = 0; vertexElement < MAX_VERTEX_ELEMENTS; vertexElement++)
	{
		D3DVERTEXELEMENT9* pVertexElement = &pHeader->Decl[vertexElement];
		if (pVertexElement->Stream == 0xff)
			break;

		vertexStream._vertexElements.Add(VertexElement(pVertexElement->Stream, pVertexElement->Offset, GetVertexFormatType((D3DDECLTYPE)pVertexElement->Type), GetVertexSemantic((D3DDECLUSAGE)pVertexElement->Usage), pVertexElement->UsageIndex));
	}

	_vertexStreams.Add(vertexStream);
}

void SDKMESHModelReader::CreateIndexBuffer(SDKMESH_INDEX_BUFFER_HEADER* pHeader, void* pIndices)
{
	RenderSystem* renderSystem = RenderSystem::Current();

	HardwareBuffer* indexBuffer;
	IndexBufferFormat ibFormat = GetIBFormat(pHeader->IndexType);
	if (!renderSystem->CreateIndexBuffer(pHeader->SizeBytes, ibFormat, HardwareBufferUsage_Static, &indexBuffer))
	{
		_indexBuffers.Add(NULL);
		return;
	}

	void* lockedIndices;
	indexBuffer->Map(HardwareBufferMode_Normal, &lockedIndices);
	Memory::Copy(lockedIndices, pIndices, pHeader->SizeBytes);
	indexBuffer->Unmap();

	_indexBuffers.Add(indexBuffer);
}

void SDKMESHModelReader::CreateMaterials(SDKMESH_MATERIAL* pMaterials, UINT numMaterials)
{
	for (UINT m = 0; m < numMaterials; m++)
	{
		SDKMESH_MATERIAL* pMaterial = &pMaterials[m];

		DefaultMaterial* shader = new DefaultMaterial();
		FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
		pass->SetName(pMaterial->Name);
		pass->MaterialState.DiffuseColor = Color32(pMaterial->Diffuse.x,
			pMaterial->Diffuse.y, pMaterial->Diffuse.z, pMaterial->Diffuse.w);
		pass->MaterialState.SpecularColor = Color32(pMaterial->Specular.x,
			pMaterial->Specular.y, pMaterial->Specular.z);
		pass->MaterialState.EmissiveColor = Color32(pMaterial->Emissive.x,
			pMaterial->Emissive.y, pMaterial->Emissive.z);
		pass->MaterialState.Shininess = pMaterial->Power;
		pass->LightState.Lighting = true;
		if (pMaterial->Diffuse.w != 1.0f)
		{
			pass->AlphaState.BlendEnable[0] = true;
			pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
			pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
		}

		String diffuseTexture = pMaterial->DiffuseTexture;
		if (!diffuseTexture.IsEmpty())
		{
			String imagePath = Path::Combine(_path, diffuseTexture);

			Resource* resource = ResourceHelper::LoadFromFile(
				imagePath, SE_ID_DATA_IMAGE);
			if (resource != NULL)
			{
				Texture* texture;
				if (RenderSystem::Current()->CreateTexture(&texture) &&
					texture->Create((Image*)resource->GetData(), TextureUsage_Static))
				{
					SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
					samplerState->SetTexture(texture);
				}
			}
		}

		_materials.Add(shader);
	}
}

void SDKMESHModelReader::CreateMeshes(SDKMESH_MESH* pMeshes, UINT numMeshes)
{
	RenderSystem* renderSystem = RenderSystem::Current();

	for (UINT m = 0; m < numMeshes; m++)
	{
		SDKMESH_MESH* pMesh = &pMeshes[m];

		Mesh* mesh = new Mesh();
		_model->AddMesh(mesh);

		mesh->SetName(pMesh->Name);

		Vector3 min, max;
		min.X = pMesh->BoundingBoxCenter.x - pMesh->BoundingBoxExtents.x;
		min.Y = pMesh->BoundingBoxCenter.y - pMesh->BoundingBoxExtents.y;
		min.Z = pMesh->BoundingBoxCenter.z - pMesh->BoundingBoxExtents.z;
		max.X = pMesh->BoundingBoxExtents.x - pMesh->BoundingBoxCenter.x;
		max.Y = pMesh->BoundingBoxExtents.y - pMesh->BoundingBoxCenter.y;
		max.Z = pMesh->BoundingBoxExtents.z - pMesh->BoundingBoxCenter.z;
		mesh->SetBoundingBox(BoundingBox(min, max));

		if (pMesh->NumFrameInfluences > 0)
		{
			Skin* skin = new Skin();
			mesh->SetSkin(skin);

			skin->SkinVertices.Resize(pMesh->NumFrameInfluences);
			for (UINT fi = 0; fi < pMesh->NumFrameInfluences; fi++)
			{
				skin->SkinVertices[fi].BoneIndex = pMesh->pFrameInfluences[fi];
			}
		}

		for (UINT subset = 0; subset < pMesh->NumSubsets; subset++)
		{
			SDKMESH_SUBSET* pSubset = &_sdkmesh.m_pSubsetArray[pMesh->pSubsets[subset]];

			MeshPart* meshPart = new MeshPart();
			mesh->AddMeshPart(meshPart);

			VertexLayout* vertexLayout;
			if (!renderSystem->CreateVertexLayout(&vertexLayout))
			{
				continue;
			}

			VertexData* vertexData = new VertexData();
			for (UINT vertexBuffer = 0; vertexBuffer < pMesh->NumVertexBuffers; vertexBuffer++)
			{
				UINT vbIndex = pMesh->VertexBuffers[vertexBuffer];
				vertexData->VertexStreams.Add(VertexStream(_vertexStreams[vbIndex]._vertexBuffer, _vertexStreams[vbIndex]._strideBytes));

				vertexLayout->SetElements((const VertexElement*)&_vertexStreams[vbIndex]._vertexElements, _vertexStreams[vbIndex]._vertexElements.Count());
			}
			vertexData->VertexLayout = vertexLayout;
			vertexData->VertexCount = pSubset->VertexCount;
			meshPart->SetVertexData(vertexData);

#if 0
			SDKMESH_PRIMITIVE_TYPE primType = (SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType;
			if (pSubset->IndexCount > 0)
			{
				IndexData* indexData = new IndexData();
				indexData->IndexBuffer = _indexBuffers[pMesh->IndexBuffer];
				indexData->IndexCount = pSubset->IndexCount;
				meshPart->SetIndexData(indexData);
				meshPart->SetStartIndex(pSubset->IndexStart);
				meshPart->SetIndexed(true);
				meshPart->SetPrimitiveTypeAndCount(GetPrimitiveType(primType), pSubset->IndexCount);
			}
			else
			{
				meshPart->SetPrimitiveTypeAndCount(GetPrimitiveType(primType), pSubset->VertexCount);
			}
#else
			meshPart->SetPrimitiveTypeAndCount(PrimitiveType_PointList, pSubset->VertexCount);
#endif
			ShaderMaterial* shader = _materials[pSubset->MaterialID];
			meshPart->SetShader(shader);
		}
	}
}

void SDKMESHModelReader::CreateHierarchy()
{
	Skeleton* skeleton = _model->GetSkeleton();
	BaseArray<Bone*> rootBones;

	// Setup the hierarchy
	for (UINT frame = 0; frame < _sdkmesh.m_pMeshHeader->NumFrames; frame++)
	{
		SDKMESH_FRAME* pFrame = &_sdkmesh.m_pFrameArray[frame];

		Bone* child = skeleton->GetBoneByIndex(frame);

		if (pFrame->Mesh != INVALID_MESH)
		{
			_model->GetMeshByIndex(pFrame->Mesh)->SetParentBone(child);
		}

		if (pFrame->ParentFrame == INVALID_FRAME)
		{
			rootBones.Add(child);
		}
		else
		{
			Bone* parent = skeleton->GetBoneByIndex(pFrame->ParentFrame);
			parent->AddChild(child);
		}
	}

	if (rootBones.Count() == 1)
	{
		skeleton->SetRootBone(rootBones[0]);
	}
	else
	{
		Bone* rootBone = skeleton->AddBone();
		skeleton->SetRootBone(rootBone);
		for (int i = 0; i < rootBones.Count(); i++)
		{
			rootBone->AddChild(rootBones[i]);
		}
	}

	skeleton->GetRootBone()->Update();
}

void SDKMESHModelReader::CreateAnimation()
{
	Skeleton* skeleton = _model->GetSkeleton();

	AnimationSet* animationSet = new AnimationSet();

	AnimationSequence* sequence = new AnimationSequence();
	TimeValue endTime;

	for (UINT frame = 0; frame < _sdkmesh.m_pMeshHeader->NumFrames; frame++)
	{
		SDKMESH_FRAME* pFrame = &_sdkmesh.m_pFrameArray[frame];

		if (pFrame->AnimationDataIndex != INVALID_ANIMATION_DATA)
		{
			SDKANIMATION_FRAME_DATA* pFrameData = &_sdkmesh.m_pAnimationFrameData[pFrame->AnimationDataIndex];

			BoneAnimationTrack* track = new BoneAnimationTrack();
			track->SetBone(skeleton->GetBoneByIndex(frame));

			TimeValue keyTime;
			Vector3 translation;
			Quaternion rotation;
			Vector3 scale;

			for (UINT animationKey = 0; animationKey < _sdkmesh.m_pAnimationHeader->NumAnimationKeys; animationKey++)
			{
				SDKANIMATION_DATA* pData = &pFrameData->pAnimationData[animationKey];

				keyTime = TimeValue((real64)animationKey /
					(real64)_sdkmesh.m_pAnimationHeader->AnimationFPS);

				TransformKeyFrame* keyFrame = (TransformKeyFrame*)track->AddKeyFrame(keyTime);
				keyFrame->SetTransformType(TransformKeyFrameType_All);

				keyFrame->SetTranslation(Vector3((real32*)&pData->Translation));
				keyFrame->SetRotation(Quaternion(pData->Orientation.x, pData->Orientation.y, pData->Orientation.z, pData->Orientation.w));
				keyFrame->SetScale(Vector3((real32*)&pData->Scaling));
			}

			track->SetEndTime(keyTime);
			sequence->AddAnimationTrack(track);

			if (keyTime > endTime)
			{
				endTime = keyTime;
			}
		}
	}

	sequence->SetEndTime(endTime);
	animationSet->AddAnimationSequence(sequence);
	skeleton->SetAnimationSet(animationSet);
}

}
