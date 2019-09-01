/*=============================================================================
QuakeMD2ModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuakeMD2ModelReader.h"

namespace SE_Quake
{

typedef uint8 byte;
#include "q2/qfiles.h"

#define NUMVERTEXNORMALS	162

float md2_r_avertexnormals[NUMVERTEXNORMALS][3] = {
#include "q2/anorms.h"
};

struct MD2Model
{
	MD2Model()
	{
		mod.num_frames = 0;
		st = NULL;
		tri = NULL;
		frame = NULL;
		cmd = NULL;
		skins = NULL;
	}

	~MD2Model()
	{
		SE_DELETE_ARRAY(st);
		SE_DELETE_ARRAY(tri);
		SE_DELETE_ARRAY(frame);
		SE_DELETE_ARRAY(cmd);
		SE_DELETE_ARRAY(skins);
	}

	dmdl_t mod;
	dstvert_t* st;
	dtriangle_t* tri;
	daliasframe_t* frame;
	int* cmd;
	char* skins;
};

QuakeMD2ModelReader::QuakeMD2ModelReader()
{
	_md2File = NULL;
}

QuakeMD2ModelReader::~QuakeMD2ModelReader()
{
	Destroy();
}

void QuakeMD2ModelReader::Destroy()
{
	SE_DELETE(_md2File);
}

Model* QuakeMD2ModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderSystem = RenderSystem::Current();
	if (renderSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"),
			_T("No active render system."));
		return NULL;
	}

	Destroy();

	if (!ReadMD2(stream))
	{
		return NULL;
	}

	Model* model = CreateModel();
	return model;
}

ShaderMaterial* QuakeMD2ModelReader::CreateShader(Texture* texture)
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

Model* QuakeMD2ModelReader::CreateModel()
{
	Model* model = new Model();

	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	Bone* bone = skeleton->AddBone();
	skeleton->SetRootBone(bone);

	AnimationSet* animationSet = NULL;
	if (_md2File->mod.num_frames > 1)
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

	model->SetTransform(Matrix4::CreateFromQuaternion(
		Quaternion::CreateFromAxisAngle(Vector3::UnitY, -Math::Pi/2)));

	return model;
}

Mesh* QuakeMD2ModelReader::CreateMesh(AnimationSet* animationSet)
{
	int i;
	Mesh* mesh = new Mesh();

	Array<Vector2> textureCoords(_md2File->mod.num_st);
	Array<uint32> indices(_md2File->mod.num_tris*3);
	Array<uint32> textureIndices(_md2File->mod.num_tris*3);

	for (i = 0; i < _md2File->mod.num_st; i++)
	{
		textureCoords[i] = Vector2(
			(real)_md2File->st[i].s / (real)_md2File->mod.skinwidth,
			(real)_md2File->st[i].t / (real)_md2File->mod.skinheight);
	}

	for (i = 0; i < _md2File->mod.num_tris; i++)
	{
		indices[3*i] = (uint32)_md2File->tri[i].index_xyz[0];
		indices[3*i+1] = (uint32)_md2File->tri[i].index_xyz[1];
		indices[3*i+2] = (uint32)_md2File->tri[i].index_xyz[2];

		textureIndices[3*i] = (uint32)_md2File->tri[i].index_st[0];
		textureIndices[3*i+1] = (uint32)_md2File->tri[i].index_st[1];
		textureIndices[3*i+2] = (uint32)_md2File->tri[i].index_st[2];
	}

	MeshPart* meshPart = CreateMeshPart(textureCoords, indices, textureIndices, animationSet);
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

MeshPart* QuakeMD2ModelReader::CreateMeshPart(Array<Vector2>& textureCoords, Array<uint32>& indices, Array<uint32>& textureIndices, AnimationSet* animationSet)
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

	int32 vertexCount = _md2File->mod.num_tris*3;
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
	TimeValue sequenceLength;
	int frameStart = 0;

	for (f = 0; f < _md2File->mod.num_frames; f++)
	{
		String frameName = _md2File->frame[f].name;
		String name = frameName.Left(frameName.Length() - 2);

		if (sequence == NULL || sequence->GetName() != name)
		{
			sequenceLength = TimeValue((real64)frameStart / 15.0);
			frameStart = 0;

			sequence = new AnimationSequence();
			sequence->SetName(name);
			sequence->SetEndTime(sequenceLength);
			animationSet->AddAnimationSequence(sequence);

			track = new MorphAnimationTrack();
			track->SetName(name);
			track->SetEndTime(sequenceLength);
			track->SetVertexData(vertexData);
			sequence->AddAnimationTrack(track);
		}

		Array<Vector3> vertices(_md2File->mod.num_xyz);
		Array<Vector3> normals(_md2File->mod.num_xyz);

		for (i = 0; i < _md2File->mod.num_xyz; i++)
		{
			dtrivertx_t* verts = *(dtrivertx_t**)_md2File->frame[f].verts;
			// swap y and z coordinates
			vertices[i] = Vector3(
				(real)(verts[i].v[0] * _md2File->frame[f].scale[0] + _md2File->frame[f].translate[0]),
				(real)(verts[i].v[2] * _md2File->frame[f].scale[2] + _md2File->frame[f].translate[2]),
				(real)(verts[i].v[1] * _md2File->frame[f].scale[1] + _md2File->frame[f].translate[1]));

			float* normal = md2_r_avertexnormals[_md2File->frame[f].verts[i].lightnormalindex];
			normals[i] = Vector3((real)normal[0], (real)normal[1], (real)normal[2]);
		}

		Array<Vector3> _vertices(_md2File->mod.num_tris*3);
		Array<Vector3> _normals(_md2File->mod.num_tris*3);
		Array<Vector2> _textureCoords(_md2File->mod.num_tris*3);

		for (i = 0; i < _md2File->mod.num_tris; i++)
		{
			for (j = 0; j < 3; j++)
			{
				_vertices[3*i+j] = vertices[indices[3*i+j]];
				_normals[3*i+j] = normals[indices[3*i+j]];
				_textureCoords[3*i+j] = textureCoords[textureIndices[3*i+j]];
			}
		}

		if (_md2File->mod.num_frames > 1)
		{
			if (!renderSystem->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
				HardwareBufferUsage_Dynamic, &vertexBuffer))
			{
				continue;
			}
		}
		else
		{
			vertexBuffer = defaultBuffer;
		}

		SEbyte* vbData;
		vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);
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

		MorphKeyFrame* keyFrame = (MorphKeyFrame*)track->AddKeyFrame(TimeValue((real64)frameStart / 15.0));
		keyFrame->SetVertexBuffer(vertexBuffer);
		frameStart++;
	}

	return meshPart;
}

bool QuakeMD2ModelReader::ReadMD2(Stream& stream)
{
	int i, j;
	RenderSystem* renderSystem = RenderSystem::Current();

	BinaryStream reader(&stream);

	_md2File = new MD2Model;

	// Read the header
	stream.Read((SEbyte*)&_md2File->mod, sizeof(dmdl_t));

	// Validity checks
	if (_md2File->mod.ident != IDALIASHEADER)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"),
			_T("Invalid file."));
		return false;
	}

	if (_md2File->mod.version != ALIAS_VERSION)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"),
			_T("Wrong version number."));
		return false;
	}

	if (_md2File->mod.num_xyz <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"), _T("No vertices."));
		return false;
	}

	if (_md2File->mod.num_st <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"), _T("No st vertices."));
		return false;
	}

	if (_md2File->mod.num_tris <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"), _T("No triangles."));
		return false;
	}

	if (_md2File->mod.num_frames <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"), _T("No frames."));
		return false;
	}

	// Load base s and t vertices
	stream.Seek(_md2File->mod.ofs_st, SeekOrigin_Begin);
	_md2File->st = new dstvert_t[_md2File->mod.num_st];

	for (i = 0; i < _md2File->mod.num_st; i++)
	{
		_md2File->st[i].s = reader.ReadInt16();
		_md2File->st[i].t = reader.ReadInt16();
	}

	// Load triangle lists
	stream.Seek(_md2File->mod.ofs_tris, SeekOrigin_Begin);
	_md2File->tri = new dtriangle_t[_md2File->mod.num_tris];

	for (i = 0; i < _md2File->mod.num_tris; i++)
	{
		for (j = 0; j < 3; j++)
		{
			_md2File->tri[i].index_xyz[j] = reader.ReadInt16();
		}
		for (j = 0; j < 3; j++)
		{
			_md2File->tri[i].index_st[j] = reader.ReadInt16();
		}
	}

	// Load the frames
	stream.Seek(_md2File->mod.ofs_frames, SeekOrigin_Begin);
	_md2File->frame = new daliasframe_t[_md2File->mod.num_frames];

	for (i = 0; i < _md2File->mod.num_frames; i++)
	{
		for (j = 0; j < 3; j++)
		{
			_md2File->frame[i].scale[j] = reader.ReadReal32();
		}
		for (j = 0; j < 3; j++)
		{
			_md2File->frame[i].translate[j] = reader.ReadReal32();
		}

		reader.Read((SEbyte*)_md2File->frame[i].name, sizeof(_md2File->frame->name));

		// verts are all 8 bit, so no swapping needed
		dtrivertx_t* verts = new dtrivertx_t[_md2File->mod.num_xyz];
		reader.Read((SEbyte*)verts, _md2File->mod.num_xyz*sizeof(dtrivertx_t));
		Memory::Copy(_md2File->frame[i].verts, &verts, sizeof(dtrivertx_t*));
	}

	// Load the glcmds
	stream.Seek(_md2File->mod.ofs_glcmds, SeekOrigin_Begin);
	_md2File->cmd = new int[_md2File->mod.num_glcmds];

	for (i = 0; i < _md2File->mod.num_glcmds; i++)
		_md2File->cmd[i] = reader.ReadInt32();

	// Register all skins
	stream.Seek(_md2File->mod.ofs_skins, SeekOrigin_Begin);
	_md2File->skins = new char[_md2File->mod.num_skins*MAX_SKINNAME];
	reader.Read((SEbyte*)_md2File->skins, _md2File->mod.num_skins*MAX_SKINNAME);

	String currentDir = Environment::GetCurrentDirectory();
	String source = ((FileStream*)&stream)->GetFileName();
	Environment::SetCurrentDirectory(Path::GetDirectoryName(source));

	for (i = 0; i < _md2File->mod.num_skins; i++)
	{
		Texture* texture = NULL;
		String fileName = Path::GetFileName(Path::ConvertSeparators(_md2File->skins+i*MAX_SKINNAME, false));
		Resource* resource = ResourceHelper::LoadFromFile(fileName, SE_ID_DATA_IMAGE);
		if (resource != NULL)
		{
			Image* image = (Image*)resource->GetData();
			if (!renderSystem->CreateTexture(&texture) ||
				!texture->Create(image, TextureUsage_Static))
			{
				SE_DELETE(texture);
				Logger::Current()->Log(LogLevel::Error, _T("QuakeMD2ModelReader.LoadModel"),
					_T("Failed to create the texture: ") + fileName);
			}
		}

		ShaderMaterial* shader = CreateShader(texture);
		_shaders.Add(shader);
	}
	
	Environment::SetCurrentDirectory(currentDir);

	return true;
}

}
