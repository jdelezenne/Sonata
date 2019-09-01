/*=============================================================================
QuakeMDLModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuakeMDLModelReader.h"

namespace SE_Quake
{

typedef uint8 byte;

typedef float vec_t;
typedef vec_t vec3_t[3];
typedef vec_t vec5_t[5];

#include "q1/modelgen.h"

#define NUMVERTEXNORMALS	162

float mdl_r_avertexnormals[NUMVERTEXNORMALS][3] = {
#include "q1/anorms.h"
};

struct MDLFrame
{
	MDLFrame()
	{
		trivert = NULL;
	}

	~MDLFrame()
	{
		SE_DELETE_ARRAY(trivert);
	}

	daliasframetype_t frametype;
	daliasframe_t aliasframe;
	trivertx_t* trivert;
};

struct MDLModel
{
	MDLModel()
	{
		model.numframes = 0;
		st = NULL;
		tri = NULL;
		//skindesc = NULL;
		frames = NULL;
	}

	~MDLModel()
	{
		SE_DELETE_ARRAY(st);
		SE_DELETE_ARRAY(tri);
		//SE_DELETE_ARRAY(skindesc);
		SE_DELETE_ARRAY(frames);
	}

	mdl_t model;
	stvert_t* st;
	dtriangle_t* tri;
	daliasskintype_t skintype;
	//maliasskindesc_t* skindesc;
	MDLFrame* frames;
};

QuakeMDLModelReader::QuakeMDLModelReader()
{
	_mdlFile = NULL;
}

QuakeMDLModelReader::~QuakeMDLModelReader()
{
	Clean();
}

void QuakeMDLModelReader::Clean()
{
	SE_DELETE(_mdlFile);
}

Model* QuakeMDLModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderSystem = RenderSystem::Current();
	if (renderSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMDLModelReader.LoadModel"),
			_T("No active render system."));
		return NULL;
	}

	Clean();

	if (!ReadMDL(stream))
	{
		return NULL;
	}

	Model* model = CreateModel();
	return model;
}

ShaderMaterial* QuakeMDLModelReader::CreateShader(Texture* texture)
{
	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);

	//pass->RasterizerState.FillMode = FillMode_WireFrame;
	//pass->RasterizerState.CullMode = CullMode_None;
	pass->LightState.Lighting = true;

	SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
	samplerState->SetTexture(texture);
	
	return shader;
}

Model* QuakeMDLModelReader::CreateModel()
{
	Model* model = new Model();

	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	Bone* bone = skeleton->AddBone();
	skeleton->SetRootBone(bone);

	AnimationSet* animationSet = NULL;

	if (_mdlFile->model.numframes > 1)
	{
		animationSet = new AnimationSet();
	}

	Mesh* mesh = CreateMesh(animationSet);
	if (mesh != NULL)
	{
		model->AddMesh(mesh);
		mesh->SetParentBone(bone);
	}

	if (animationSet != NULL)
	{
		model->SetAnimationSet(animationSet);
	}

	return model;
}

Mesh* QuakeMDLModelReader::CreateMesh(AnimationSet* animationSet)
{
	int i;
	Mesh* mesh = new Mesh();

	Array<Vector2> textureCoords(_mdlFile->model.numverts);
	Array<uint32> indices(_mdlFile->model.numtris*3);

	for (i = 0; i < _mdlFile->model.numverts; i++)
	{
		textureCoords[i] = Vector2(
			(real)_mdlFile->st[i].s / (real)_mdlFile->model.skinwidth,
			(real)_mdlFile->st[i].t / (real)_mdlFile->model.skinheight);
	}

	for (i = 0; i < _mdlFile->model.numtris; i++)
	{
		indices[3*i] = (uint32)_mdlFile->tri[i].vertindex[0];
		indices[3*i+1] = (uint32)_mdlFile->tri[i].vertindex[1];
		indices[3*i+2] = (uint32)_mdlFile->tri[i].vertindex[2];
	}

	MeshPart* meshPart = CreateMeshPart(textureCoords, indices, animationSet);
	if (meshPart != NULL)
	{
		mesh->AddMeshPart(meshPart);

		if (_shaders.Count() > 0)
		{
			meshPart->SetShader(_shaders[0]);
		}
	}

	return mesh;
}

MeshPart* QuakeMDLModelReader::CreateMeshPart(Array<Vector2>& textureCoords, Array<uint32>& indices, AnimationSet* animationSet)
{
	int i, j, f;
	RenderSystem* renderSystem = RenderSystem::Current();

	MeshPart* meshPart = new MeshPart();

	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* defaultBuffer = NULL;
	HardwareBuffer* vertexBuffer = NULL;

	if (!renderSystem->CreateVertexLayout(&vertexLayout))
	{
		return NULL;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	int32 vertexCount = _mdlFile->model.numtris*3;
	if (!renderSystem->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &defaultBuffer))
	{
		return NULL;
	}

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(defaultBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	meshPart->SetVertexData(vertexData);
	meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, vertexCount);

	AnimationSequence* sequence = NULL;
	MorphAnimationTrack* track = NULL;
	if (animationSet != NULL)
	{
		sequence = new AnimationSequence();
		animationSet->AddAnimationSequence(sequence);

		track = new MorphAnimationTrack();
		track->SetVertexData(vertexData);
	}

	for (f = 0; f < _mdlFile->model.numframes; f++)
	{
		Array<Vector3> vertices(_mdlFile->model.numverts);
		Array<Vector3> normals(_mdlFile->model.numverts);

		for (i = 0; i < _mdlFile->model.numverts; i++)
		{
			// swap y and z coordinates
			vertices[i] = Vector3(
				(real)(_mdlFile->frames[f].trivert[i].v[0] * _mdlFile->model.scale[0] + _mdlFile->model.scale_origin[0]),
				(real)(_mdlFile->frames[f].trivert[i].v[2] * _mdlFile->model.scale[2] + _mdlFile->model.scale_origin[2]),
				(real)(_mdlFile->frames[f].trivert[i].v[1] * _mdlFile->model.scale[1] + _mdlFile->model.scale_origin[1]));

			float* normal = mdl_r_avertexnormals[_mdlFile->frames[f].trivert[i].lightnormalindex];
			normals[i] = Vector3((real)normal[0], (real)normal[1], (real)normal[2]);
		}

		Array<Vector3> _vertices(vertexCount);
		Array<Vector3> _normals(vertexCount);
		Array<Vector2> _textureCoords(vertexCount);

		for (i = 0; i < _mdlFile->model.numtris; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				_vertices[3*i+j] = vertices[indices[3*i+j]];
				_normals[3*i+j] = normals[indices[3*i+j]];
				_textureCoords[3*i+j] = textureCoords[indices[3*i+j]];
			}
		}

		if (_mdlFile->model.numframes > 1)
		{
			if (!renderSystem->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
				HardwareBufferUsage_Static, &vertexBuffer))
			{
				continue;
			}
		}
		else
		{
			vertexBuffer = defaultBuffer;
		}

		SEbyte* vbData;
		vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);
		for (i = 0; i < vertexCount; ++i)
		{
			Memory::Copy(vbData, _vertices[i], sizeof(Vector3));
			vbData += sizeof(Vector3);

			Memory::Copy(vbData, _normals[i], sizeof(Vector3));
			vbData += sizeof(Vector3);

			Memory::Copy(vbData, _textureCoords[i], sizeof(Vector2));
			vbData += sizeof(Vector2);
		}
		vertexBuffer->Unmap();

		// Copy the first frame to the mesh data
		if (f == 0)
		{
			vertexBuffer->Map(HardwareBufferMode_ReadOnly, (void**)&vbData);
			SEbyte* defaultData;
			defaultBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&defaultData);
			Memory::Copy(defaultData, vbData, vertexCount * vertexLayout->GetSize());
			defaultBuffer->Unmap();
			vertexBuffer->Unmap();
		}

		if (animationSet != NULL)
		{
			MorphKeyFrame* keyFrame = (MorphKeyFrame*)track->AddKeyFrame(TimeValue((real64)f / 30.0));
			keyFrame->SetVertexBuffer(vertexBuffer);
		}
	}

	track->SetEndTime(TimeValue((real64)_mdlFile->model.numframes / 30.0));
	sequence->AddAnimationTrack(track);

	sequence->SetEndTime(track->GetEndTime());
	sequence->AddAnimationTrack(track);

	return meshPart;
}

bool QuakeMDLModelReader::ReadMDL(Stream& stream)
{
	int i, j, k;

	RenderSystem* renderSystem = RenderSystem::Current();

	BinaryStream reader(&stream);

	_mdlFile = new MDLModel;

	// Read the header
	stream.Read((SEbyte*)&_mdlFile->model, sizeof(mdl_t));

	// Validity checks
	if (_mdlFile->model.ident != IDPOLYHEADER)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMDLModelReader.LoadModel"),
			_T("Invalid file."));
		return false;
	}

	if (_mdlFile->model.version != ALIAS_VERSION)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMDLModelReader.LoadModel"),
			_T("Wrong version number."));
		return false;
	}

	if (_mdlFile->model.numverts <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMDLModelReader.LoadModel"), _T("No vertices."));
		return false;
	}

	if (_mdlFile->model.numtris <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMDLModelReader.LoadModel"), _T("No triangles."));
		return false;
	}

	if (_mdlFile->model.numframes <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMDLModelReader.LoadModel"), _T("No frames."));
		return false;
	}

	for (i = 0; i < _mdlFile->model.numskins; i++)
	{
		reader.Read((SEbyte*)&_mdlFile->skintype, sizeof(daliasskintype_t));

		if (_mdlFile->skintype.type == ALIAS_SKIN_SINGLE)
		{
			stream.Seek(_mdlFile->model.skinwidth*_mdlFile->model.skinheight, SeekOrigin_Current);
		}
		else //ALIAS_SKIN_GROUP
		{
		}
	}

	// Load base s and t vertices
	_mdlFile->st = new stvert_t[_mdlFile->model.numverts];

	for (i = 0; i < _mdlFile->model.numverts; i++)
	{
		_mdlFile->st[i].onseam = reader.ReadInt32();
		_mdlFile->st[i].s = reader.ReadInt32();// << 16;
		_mdlFile->st[i].t = reader.ReadInt32();// << 16;
	}

	// Load triangle lists
	_mdlFile->tri = new dtriangle_t[_mdlFile->model.numtris];

	for (i = 0; i < _mdlFile->model.numtris; i++)
	{
		_mdlFile->tri[i].facesfront = reader.ReadInt32();

		for (j = 0; j < 3; j++)
		{
			_mdlFile->tri[i].vertindex[j] = reader.ReadInt32();
		}
	}

	// Load the frames
	_mdlFile->frames = new MDLFrame[_mdlFile->model.numframes];

	for (i = 0; i < _mdlFile->model.numframes; i++)
	{
		reader.Read((SEbyte*)&_mdlFile->frames[i].frametype, sizeof(daliasframetype_t));

		if (_mdlFile->frames[i].frametype.type == ALIAS_SINGLE)
		{
			for (j = 0; j < 3; j++)
			{
				_mdlFile->frames[i].aliasframe.bboxmin.v[j] = reader.ReadInt8();
			}
			_mdlFile->frames[i].aliasframe.bboxmin.lightnormalindex = reader.ReadInt8();

			for (j = 0; j < 3; j++)
			{
				_mdlFile->frames[i].aliasframe.bboxmax.v[j] = reader.ReadInt8();
			}
			_mdlFile->frames[i].aliasframe.bboxmax.lightnormalindex = reader.ReadInt8();

			reader.Read((SEbyte*)_mdlFile->frames[i].aliasframe.name, sizeof(_mdlFile->frames[i].aliasframe.name));

			_mdlFile->frames[i].trivert = new trivertx_t[_mdlFile->model.numverts];

			for (j = 0; j < _mdlFile->model.numverts; j++)
			{
				for (k=0; k<3; k++)
				{
					_mdlFile->frames[i].trivert[j].v[k] = reader.ReadInt8();
				}
				_mdlFile->frames[i].trivert[j].lightnormalindex = reader.ReadInt8();
			}
		}
		else //ALIAS_GROUP
		{
		}
	}

	return true;
}

}
