/*=============================================================================
QuakeMD3ModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuakeMD3ModelReader.h"

namespace SE_Quake
{

typedef uint8 byte;

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef	int	fixed4_t;
typedef	int	fixed8_t;
typedef	int	fixed16_t;

#include "q3/qfiles.h"

struct MD3Surface
{
	MD3Surface()
	{
		surf = NULL;
		shader = NULL;
		tri = NULL;
		st = NULL;
		xyz = NULL;
	}

	~MD3Surface()
	{
		SE_DELETE_ARRAY(shader);
		SE_DELETE_ARRAY(tri);
		SE_DELETE_ARRAY(st);
		SE_DELETE_ARRAY(xyz);
	}

	md3Surface_t* surf;
	md3Shader_t* shader;
	md3Triangle_t* tri;
	md3St_t* st;
	md3XyzNormal_t* xyz;
	Array<ShaderMaterial*> shaders;
	Array<HardwareBuffer*> vertexBuffers;
};

struct MD3Model
{
	MD3Model()
	{
		frame = NULL;
		surf = NULL;
		tag = NULL;
		version = 0;
		size = 0;
	}

	~MD3Model()
	{
		SE_DELETE_ARRAY(frame);
		SE_DELETE_ARRAY(surf);
		SE_DELETE_ARRAY(tag);
	}

	md3Header_t pinmodel;
	md3Frame_t* frame;
	md3Surface_t* surf;
	md3Tag_t* tag;
	int version;
	int size;

	BaseArray<MD3Surface> md3Surfaces;
	Dictionary<String, ShaderMaterial*> md3Skins;
};

struct MD3Sequence
{
	int firstFrame;
	int numFrames;
	int loopingFrames;
	int framesPerSecond;
	String name;
};

struct MD3Animation
{
	SEchar sex;
	BaseArray<MD3Sequence> sequences;
};

QuakeMD3ModelReader::QuakeMD3ModelReader()
{
	_isCharacter = false;
	_isAnimated = false;
	_md3Files[0] = NULL;
	_md3Files[1] = NULL;
	_md3Files[2] = NULL;
	_md3Animation = NULL;
}

QuakeMD3ModelReader::~QuakeMD3ModelReader()
{
	Destroy();
}

void QuakeMD3ModelReader::Destroy()
{
	SE_DELETE(_md3Files[0]);
	SE_DELETE(_md3Files[1]);
	SE_DELETE(_md3Files[2]);
	SE_DELETE(_md3Animation);
}

Model* QuakeMD3ModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderSystem = RenderSystem::Current();
	if (renderSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"),
			_T("No active render system."));
		return NULL;
	}

	Destroy();

	_fileName = ((FileStream*)&stream)->GetFileName();
	_path = Path::GetDirectoryName(_fileName);

	_skinName = "default";
	_isCharacter = false;
	_isAnimated = false;

	String fileName = Path::GetFileName(_fileName);
	if (String::Compare(fileName, "lower.md3", true) == 0 ||
		String::Compare(fileName, "upper.md3", true) == 0 ||
		String::Compare(fileName, "head.md3", true) == 0)
	{
		_isCharacter = true;
	}

	File lowerFile(Path::Combine(_path, "lower.md3"));
	File upperFile(Path::Combine(_path, "upper.md3"));
	File headFile(Path::Combine(_path, "head.md3"));
	
	if (!lowerFile.Exists() || !upperFile.Exists() || !headFile.Exists())
	{
		_isCharacter = false;
	}

	if (!_isCharacter)
	{
		if (!ReadMD3(0, &stream))
		{
			return NULL;
		}
	}
	else
	{
		stream.Close();

		FileStreamPtr lowerStream = lowerFile.Open(FileMode_Open, FileAccess_Read);
		FileStreamPtr upperStream = upperFile.Open(FileMode_Open, FileAccess_Read);
		FileStreamPtr headStream = headFile.Open(FileMode_Open, FileAccess_Read);

		if (!ReadMD3(0, lowerStream) ||
			!ReadMD3(1, upperStream) ||
			!ReadMD3(2, headStream))
		{
			return NULL;
		}

		ReadSkin(0, "lower");
		ReadSkin(1, "upper");
		ReadSkin(2, "head");

		_isAnimated = ReadAnimation("animation.cfg");
	}

	Model* model = CreateModel();
	return model;
}

ShaderMaterial* QuakeMD3ModelReader::CreateShader(const String& name)
{
	RenderSystem* renderSystem = RenderSystem::Current();

	Texture* texture = NULL;
	String fileName = Path::GetFileName(Path::ConvertSeparators(name, false));
	fileName = FileSystem::Instance()->GetFullPath(Path::Combine(_path, fileName));
	if (!File::Exists(fileName))
	{
		fileName = Path::Combine(Path::GetDirectoryName(fileName),
			Path::ChangeExtension(fileName, "jpg"));
	}

	Resource* resource = NULL;
	SEtry
	{
		resource = ResourceHelper::LoadFromFile(fileName, SE_ID_DATA_IMAGE);
	}
	SEcatch (Exception)
	{
	}

	if (resource == NULL)
	{
		return NULL;
	}

	Image* image = (Image*)resource->GetData();
	if (!renderSystem->CreateTexture(&texture) ||
		!texture->Create(image, TextureUsage_Static))
	{
		SE_DELETE(texture);
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"),
			_T("Failed to create the texture: ") + fileName);
		return NULL;
	}

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);

	//pass->RasterizerState.FillMode = FillMode_WireFrame;
	//pass->RasterizerState.CullMode = CullMode_None;
	pass->LightState.Lighting = true;

	SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
	samplerState->SetTexture(texture);

	return shader;
}

md3Tag_t* GetTagFromName(MD3Model* md3File, int frame, const String& name)
{
	for (int i = 0; i < md3File->pinmodel.numTags; i++)
	{
		md3Tag_t* tag = &md3File->tag[frame * md3File->pinmodel.numTags + i];

		if (String::Compare(tag->name, name, true) == 0)
		{
			return tag;
		}
	}

	return NULL;
}

void TagToTransform(md3Tag_t* tag, Vector3& position, Quaternion& orientation)
{
	position = Vector3(tag->origin[0], tag->origin[1], tag->origin[2]);
	Matrix3 matrix;
	matrix.SetColumn(0, tag->axis[0]);
	matrix.SetColumn(1, tag->axis[1]);
	matrix.SetColumn(2, tag->axis[2]);
	orientation = Quaternion::CreateFromRotationMatrix(matrix);
}

Model* QuakeMD3ModelReader::CreateModel()
{
	int i, j, k, f;
	Model* model = new Model();

	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	Bone* rootBone = skeleton->AddBone();
	skeleton->SetRootBone(rootBone);

	AnimationSet* animationSet = NULL;

	if (_isCharacter)
	{
		Bone* bones[3];
		bones[0] = skeleton->AddBone();
		bones[1] = skeleton->AddBone();
		bones[2] = skeleton->AddBone();

		bones[0]->SetName("lower");
		bones[1]->SetName("upper");
		bones[2]->SetName("head");

		rootBone->AddChild(bones[0]);
		bones[0]->AddChild(bones[1]);
		bones[1]->AddChild(bones[2]);

		for (j = 0; j < 3; j++)
		{
			for (i = 0; i < _md3Files[j]->md3Surfaces.Count(); i++)
			{
				Mesh* mesh = CreateMesh(_md3Files[j], &_md3Files[j]->md3Surfaces[i]);
				if (mesh != NULL)
				{
					model->AddMesh(mesh);
					mesh->SetParentBone(bones[j]);
				}
			}
		}

		if (_md3Animation->sequences.Count() > 0)
		{
			animationSet = new AnimationSet();

			for (i = 0; i < _md3Animation->sequences.Count(); ++i)
			{
				MD3Sequence* md3Sequence = &_md3Animation->sequences[i];

				int meshIndex = 0;
				AnimationSequence* sequence = new AnimationSequence();
				sequence->SetName(md3Sequence->name);
				TimeValue sequenceLength = TimeValue((real64)md3Sequence->numFrames / md3Sequence->framesPerSecond);

				for (j = 0; j < 3; j++)
				{
					MD3Model* md3File = _md3Files[j];

					if (md3File->pinmodel.numFrames <= 1)
					{
						meshIndex += md3File->md3Surfaces.Count();
						continue;
					}

					for (k = 0; k < md3File->md3Surfaces.Count(); k++)
					{
						MD3Surface* md3Surface = &md3File->md3Surfaces[k];

						MorphAnimationTrack* morphTrack = new MorphAnimationTrack();
						morphTrack->SetEndTime(sequenceLength);
						morphTrack->SetVertexData(model->GetMeshByIndex(meshIndex)->GetMeshPart(0)->GetVertexData());
						for (f = 0; f < md3Sequence->numFrames; f++)
						{
							if (md3Sequence->firstFrame + f >= md3Surface->vertexBuffers.Count())
								break;

							MorphKeyFrame* keyFrame = (MorphKeyFrame*)morphTrack->AddKeyFrame(
								TimeValue((real64)f / md3Sequence->framesPerSecond));
							keyFrame->SetVertexBuffer(md3Surface->vertexBuffers[md3Sequence->firstFrame + f]);
						}

						sequence->AddAnimationTrack(morphTrack);
						meshIndex++;
					}
				}

				Vector3 position;
				Quaternion orientation;

				BoneAnimationTrack* boneTrack1 = new BoneAnimationTrack();
				boneTrack1->SetEndTime(sequenceLength);
				boneTrack1->SetBone(bones[1]);

				for (f = 0; f < md3Sequence->numFrames; f++)
				{
					if (md3Sequence->firstFrame + f >= _md3Files[0]->pinmodel.numFrames)
						break;

					md3Tag_t* tag_torso = GetTagFromName(_md3Files[0], md3Sequence->firstFrame + f, "tag_torso");
					TagToTransform(tag_torso, position, orientation);

					TransformKeyFrame* keyFrame = (TransformKeyFrame*)boneTrack1->AddKeyFrame(
						TimeValue((real64)f / md3Sequence->framesPerSecond));
					keyFrame->SetTranslation(position);
					keyFrame->SetRotation(orientation);
				}
				sequence->AddAnimationTrack(boneTrack1);

				BoneAnimationTrack* boneTrack2 = new BoneAnimationTrack();
				boneTrack2->SetEndTime(sequenceLength);
				boneTrack2->SetBone(bones[2]);
				for (f = 0; f < md3Sequence->numFrames; f++)
				{
					if (md3Sequence->firstFrame + f >= _md3Files[1]->pinmodel.numFrames)
						break;

					md3Tag_t* tag_head = GetTagFromName(_md3Files[1], md3Sequence->firstFrame + f, "tag_head");
					TagToTransform(tag_head, position, orientation);

					TransformKeyFrame* keyFrame = (TransformKeyFrame*)boneTrack2->AddKeyFrame(
						TimeValue((real64)f / md3Sequence->framesPerSecond));
					keyFrame->SetTranslation(position);
					keyFrame->SetRotation(orientation);
				}
				sequence->AddAnimationTrack(boneTrack2);

				sequence->SetEndTime(sequenceLength);
				animationSet->AddAnimationSequence(sequence);
			}
		}
	}
	else
	{
		MD3Model* md3File = _md3Files[0];
		for (i = 0; i < md3File->md3Surfaces.Count(); i++)
		{
			Mesh* mesh = CreateMesh(md3File, &md3File->md3Surfaces[i]);
			if (mesh != NULL)
			{
				model->AddMesh(mesh);
				mesh->SetParentBone(rootBone);
			}
		}

		if (md3File->pinmodel.numFrames > 1)
		{
			animationSet = new AnimationSet();

			AnimationSequence* sequence = new AnimationSequence();
			TimeValue sequenceLength;

			for (i = 0; i < md3File->md3Surfaces.Count(); i++)
			{
				MD3Surface* md3Surface = &md3File->md3Surfaces[i];
				TimeValue trackLength = TimeValue((real64)md3Surface->surf->numFrames / 15.0);

				MorphAnimationTrack* morphTrack = new MorphAnimationTrack();
				morphTrack->SetEndTime(trackLength);
				morphTrack->SetVertexData(model->GetMeshByIndex(i)->GetMeshPart(0)->GetVertexData());

				for (j = 0; j < md3Surface->surf->numFrames; j++)
				{
					MorphKeyFrame* keyFrame = (MorphKeyFrame*)morphTrack->AddKeyFrame(
						TimeValue((real64)j / 15.0));
					keyFrame->SetVertexBuffer(md3Surface->vertexBuffers[j]);
				}

				sequence->AddAnimationTrack(morphTrack);

				if (trackLength > sequenceLength)
				{
					sequenceLength = trackLength;
				}
			}

			sequence->SetEndTime(sequenceLength);
			animationSet->AddAnimationSequence(sequence);
		}
	}

	if (animationSet != NULL)
	{
		model->SetAnimationSet(animationSet);
	}

	//model->SetTransform(Matrix4::CreateFromQuaternion(
	//	Quaternion::CreateFromAxisAngle(Vector3::UnitY, -Math::Pi/2)));

	return model;
}

Mesh* QuakeMD3ModelReader::CreateMesh(MD3Model* md3File, MD3Surface* md3Surface)
{
	int i;
	Mesh* mesh = new Mesh();
	mesh->SetName(md3Surface->surf->name);

	Array<uint32> indices(md3Surface->surf->numTriangles*3);
	Array<Vector2> textureCoords(md3Surface->surf->numVerts);

	for (i = 0; i < md3Surface->surf->numTriangles; i++)
	{
		indices[3*i] = (uint32)md3Surface->tri[i].indexes[0];
		indices[3*i+1] = (uint32)md3Surface->tri[i].indexes[2];
		indices[3*i+2] = (uint32)md3Surface->tri[i].indexes[1];
	}

	for (i = 0; i < md3Surface->surf->numVerts; i++)
	{
		textureCoords[i] = Vector2(
			(real)md3Surface->st[i].st[0], (real)md3Surface->st[i].st[1]);
	}

	MeshPart* meshPart = CreateMeshPart(md3File, md3Surface, indices, textureCoords);
	if (meshPart != NULL)
	{
		mesh->AddMeshPart(meshPart);

		if (!_isCharacter)
		{
			if (md3Surface->shaders.Count() > 0)
			{
				meshPart->SetShader(md3Surface->shaders[0]);
			}
		}
		else
		{
			if (md3File->md3Skins.ContainsKey(md3Surface->surf->name))
			{
				meshPart->SetShader(md3File->md3Skins[md3Surface->surf->name]);
			}
		}
	}

	return mesh;
}

MeshPart* QuakeMD3ModelReader::CreateMeshPart(MD3Model* md3File, MD3Surface* md3Surface, Array<uint32>& indices, Array<Vector2>& textureCoords)
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

	int32 vertexCount = md3Surface->surf->numTriangles*3;
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

	for (f = 0; f < md3Surface->surf->numFrames; f++)
	{
		Array<Vector3> vertices(md3Surface->surf->numVerts);
		Array<Vector3> normals(md3Surface->surf->numVerts);

		for (i = 0; i < md3Surface->surf->numVerts; i++)
		{
			int frameIndex = f * md3Surface->surf->numVerts + i;
			md3XyzNormal_t* xyzNormal = &md3Surface->xyz[frameIndex];
			// swap y and z coordinates
			vertices[i] = Vector3(
				(real)xyzNormal->xyz[0] * MD3_XYZ_SCALE,
				(real)xyzNormal->xyz[1] * MD3_XYZ_SCALE,
				(real)xyzNormal->xyz[2] * MD3_XYZ_SCALE);

			// decode the normal
			short normal = xyzNormal->normal;
			int lat = (normal >> 8) & 0xff;
			lat = (int)(1.0 * lat * Math::Pi / 128.0);
			int lon = (normal & 0xff);
			lon = (int)(1.0 * lon * Math::Pi / 128.0);
			normals[i] = Vector3(Math::Cos(lat) * Math::Sin(lon), Math::Sin(lat) * Math::Sin(lon), Math::Cos(lon));
		}

		Array<Vector3> _vertices(md3Surface->surf->numTriangles*3);
		Array<Vector3> _normals(md3Surface->surf->numTriangles*3);
		Array<Vector2> _textureCoords(md3Surface->surf->numTriangles*3);

		for (i = 0; i < md3Surface->surf->numTriangles; i++)
		{
			for (j = 0; j < 3; j++)
			{
				_vertices[3*i+j] = vertices[indices[3*i+j]];
				_normals[3*i+j] = normals[indices[3*i+j]];
				_textureCoords[3*i+j] = textureCoords[indices[3*i+j]];
			}
		}

		if (md3Surface->surf->numFrames > 1)
		{
			if (!renderSystem->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
				HardwareBufferUsage_Dynamic, &vertexBuffer))
			{
				md3Surface->vertexBuffers.Add(NULL);
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

		if (md3Surface->surf->numFrames > 1)
		{
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

			md3Surface->vertexBuffers.Add(vertexBuffer);
		}
	}

	return meshPart;
}

bool QuakeMD3ModelReader::ReadMD3(int index, Stream* stream)
{
	int i, j;

	BinaryStream reader(stream);

	_md3Files[index] = new MD3Model;
	MD3Model* md3File = _md3Files[index];

	// Read the header
	reader.Read((SEbyte*)&md3File->pinmodel, sizeof(md3Header_t));

	md3Header_t* pinmodel = &md3File->pinmodel;

	// Validity checks
	if (pinmodel->ident != MD3_IDENT)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"),
			_T("Invalid file."));
		return false;
	}

	if (pinmodel->version != MD3_VERSION)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"),
			_T("Wrong version number."));
		return false;
	}

	if (pinmodel->numFrames <= 0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"), _T("No frames."));
		return false;
	}

	// Load the frames
	stream->Seek(pinmodel->ofsFrames, SeekOrigin_Begin);
	md3File->frame = new md3Frame_t[pinmodel->numFrames];

	for (i = 0; i < pinmodel->numFrames; i++)
	{
		for (j = 0; j < 3; j++)
		{
			md3File->frame[i].bounds[0][j] = reader.ReadReal32();
			md3File->frame[i].bounds[1][j] = reader.ReadReal32();
			md3File->frame[i].localOrigin[j] = reader.ReadReal32();
		}
		md3File->frame[i].radius = reader.ReadReal32();
		reader.Read((SEbyte*)md3File->frame[i].name, 16);
	}

	// Load the tags
	if (pinmodel->numTags * pinmodel->numFrames > 0)
	{
		stream->Seek(pinmodel->ofsTags, SeekOrigin_Begin);
		md3File->tag = new md3Tag_t[pinmodel->numTags * pinmodel->numFrames];

		for (i = 0; i < pinmodel->numTags * pinmodel->numFrames; i++)
		{
			reader.Read((SEbyte*)md3File->tag[i].name, MAX_QPATH);
			md3File->tag[i].origin[0] = reader.ReadReal32();
			md3File->tag[i].origin[1] = reader.ReadReal32();
			md3File->tag[i].origin[2] = reader.ReadReal32();
			for (j = 0; j < 3; j++)
			{
				md3File->tag[i].axis[j][0] = reader.ReadReal32();
				md3File->tag[i].axis[j][1] = reader.ReadReal32();
				md3File->tag[i].axis[j][2] = reader.ReadReal32();
			}
		}
	}

	md3File->md3Surfaces.Resize(pinmodel->numSurfaces);

	// Load the surfaces
	stream->Seek(pinmodel->ofsSurfaces, SeekOrigin_Begin);
	md3File->surf = new md3Surface_t[pinmodel->numSurfaces];

	int ofsSurface = pinmodel->ofsSurfaces;
	for (i = 0; i < pinmodel->numSurfaces; i++)
	{
		md3Surface_t* surf = &md3File->surf[i];

		surf->ident = reader.ReadInt32();
		reader.Read((SEbyte*)surf->name, MAX_QPATH);
		surf->flags = reader.ReadInt32();
		surf->numFrames = reader.ReadInt32();
		surf->numShaders = reader.ReadInt32();
		surf->numVerts = reader.ReadInt32();
		surf->numTriangles = reader.ReadInt32();
		surf->ofsTriangles = reader.ReadInt32();
		surf->ofsShaders = reader.ReadInt32();
		surf->ofsSt = reader.ReadInt32();
		surf->ofsXyzNormals = reader.ReadInt32();
		surf->ofsEnd = reader.ReadInt32();

		if (surf->numVerts > SHADER_MAX_VERTEXES)
		{
			Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"),
				String::Format(_T("The model has more than %i verts on a surface (%i)"),
				SHADER_MAX_VERTEXES, surf->numVerts));
			return false;
		}

		if (surf->numTriangles*3 > SHADER_MAX_INDEXES)
		{
			Logger::Current()->Log(LogLevel::Error, _T("QuakeMD3ModelReader.LoadModel"),
				String::Format(_T("The model has more than %i triangles on a surface (%i)"),
				SHADER_MAX_INDEXES / 3, surf->numTriangles));
			return false;
		}

		MD3Surface* md3Surface = &md3File->md3Surfaces[i];
		md3Surface->surf = surf;

		if (surf->numShaders > 0)
		{
			stream->Seek(ofsSurface + surf->ofsShaders, SeekOrigin_Begin);
			md3Surface->shader = new md3Shader_t[surf->numShaders];

			for (j = 0; j < surf->numShaders; j++)
			{
				reader.Read((SEbyte*)md3Surface->shader[j].name, MAX_QPATH);
				md3Surface->shader[j].shaderIndex = reader.ReadInt32();

				if (md3Surface->shader[j].name != "")
				{
					md3Surface->shaders.Add(CreateShader(md3Surface->shader[j].name));
				}
			}
		}

		// Load the triangles
		stream->Seek(ofsSurface + surf->ofsTriangles, SeekOrigin_Begin);
		md3Surface->tri = new md3Triangle_t[surf->numTriangles];

		for (j = 0; j < surf->numTriangles; j++)
		{
			md3Surface->tri[j].indexes[0] = reader.ReadInt32();
			md3Surface->tri[j].indexes[1] = reader.ReadInt32();
			md3Surface->tri[j].indexes[2] = reader.ReadInt32();
		}

		// Load the ST
		stream->Seek(ofsSurface + surf->ofsSt, SeekOrigin_Begin);
		md3Surface->st = new md3St_t[surf->numVerts];

		for (j = 0; j < surf->numVerts; j++)
		{
			md3Surface->st[j].st[0] = reader.ReadReal32();
			md3Surface->st[j].st[1] = reader.ReadReal32();
		}

		// Load the XyzNormals
		stream->Seek(ofsSurface + surf->ofsXyzNormals, SeekOrigin_Begin);
		md3Surface->xyz = new md3XyzNormal_t[surf->numVerts * surf->numFrames];

		for (j = 0; j < surf->numVerts * surf->numFrames; j++)
		{
			md3Surface->xyz[j].xyz[0] = reader.ReadInt16();
			md3Surface->xyz[j].xyz[1] = reader.ReadInt16();
			md3Surface->xyz[j].xyz[2] = reader.ReadInt16();
			md3Surface->xyz[j].normal = reader.ReadInt16();
		}

		ofsSurface += surf->ofsEnd;
		stream->Seek(ofsSurface, SeekOrigin_Begin);
	}

	return true;
}

bool QuakeMD3ModelReader::ReadSkin(int index, const String& fileName)
{
	String skinName = String::Concat(fileName, String::Concat("_", String::Concat(_skinName, ".skin")));
	File skinFile(Path::Combine(_path, skinName));
	FileStreamPtr skinStream = skinFile.Open(FileMode_Open, FileAccess_Read);
	if (skinStream == NULL)
	{
		return false;
	}

	TextStream reader(skinStream);
	while (reader.Peek() != -1)
	{
		String line = reader.ReadLine();

		int indexSep = line.IndexOf(',');
		if (indexSep != -1)
		{
			String name = line.Substring(0, indexSep).Trim();
			String fileName = line.Substring(indexSep + 1).Trim();
			if (!fileName.IsEmpty())
			{
				_md3Files[index]->md3Skins.Add(name, CreateShader(Path::Combine(_path, fileName)));
			}
		}
	}

	return true;
}

bool QuakeMD3ModelReader::ReadAnimation(const String& fileName)
{
	File animFile(Path::Combine(_path, fileName));
	FileStreamPtr animStream = animFile.Open(FileMode_Open, FileAccess_Read);
	if (animStream == NULL)
	{
		return false;
	}

	_md3Animation = new MD3Animation;

	TextStream reader(animStream);
	while (reader.Peek() != -1)
	{
		String line = reader.ReadLine().Trim();
		if (line.Length() == 0 || !Char::IsDigit(line.Chars(0)))
			continue;

		SEchar* data = (SEchar*)line.Data();
		int length = line.Length();
		for (int i = 0; i < length; ++i)
		{
			if (data[i] =='\t' || data[i] =='/')
				data[i] = ' ';
		}

		Array<String> values = line.Split(' ');
		if (values.Count() < 4)
			continue;

		MD3Sequence md3Sequence;
		md3Sequence.firstFrame = values[0].ToInt32();
		md3Sequence.numFrames = values[1].ToInt32();
		md3Sequence.loopingFrames = values[2].ToInt32();
		md3Sequence.framesPerSecond = values[3].ToInt32();

		if (values.Count() >= 5)
		{
			md3Sequence.name = values[4];
		}

		_md3Animation->sequences.Add(md3Sequence);
	}

	return true;
}

}
