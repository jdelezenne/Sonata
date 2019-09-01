/*=============================================================================
QuakeBSPSceneReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuakeBSPSceneReader.h"

namespace SE_Quake
{

typedef real32 vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef	int	fixed4_t;
typedef	int	fixed8_t;
typedef	int	fixed16_t;

typedef	uint32 GLuint;

#include "q3/q_shared.h"
#include "q3/qfiles.h"
#include "q3/surfaceflags.h"

static const int LightMapSize = 128;
static const int LightMapTotalSize = LightMapSize * LightMapSize * 3;
static const int r_mapOverBrightBits = 2;
static const int r_overBrightBits = 1;
static const int r_curvefactor = 16;

struct BSPEntity
{
	Dictionary<String, String> Vars;
};

struct BSPWorld
{
	int numShaders;
	dshader_t* shaders;

	int numPlanes;
	dplane_t* planes;

	int numNodes;
	int numDecisionNodes;
	dnode_t* nodes;

	int numLeafs;
	dleaf_t* leafs;

	int numSurfaces;
	dsurface_t* surfaces;

	int numVerts;
	drawVert_t* verts;

	int numIndexes;
	int* indexes;

	int numLeafBrushes;
	int* leafbrushes;

	int numLeafSurfaces;
	int* leafsurfaces;

	int numFogs;
	dfog_t* fogs;

	int numBrushes;
	dbrush_t* brushes;

	int numBrushSides;
	dbrushside_t* brushSides;

	int lightMapBytes;
	uint8* lightMapData;

	vec3_t lightGridOrigin;
	vec3_t lightGridSize;
	vec3_t lightGridInverseSize;
	int lightGridBounds[3];
	uint8* lightGridData;

	int numClusters;
	int clusterBytes;
	const uint8* vis;
	uint8* novis;

	char* entityString;
};

struct ShaderHash
{
	ShaderHash()
	{
	}

	ShaderHash(int textureIndex, int lightMapIndex) :
		TextureIndex(textureIndex), LightMapIndex(lightMapIndex)
	{
	}

	int TextureIndex;
	int LightMapIndex;

	bool operator<(const ShaderHash& value) const
	{
		if (TextureIndex == value.TextureIndex)
			return (LightMapIndex < value.LightMapIndex);
		else
			return (TextureIndex < value.TextureIndex);
	}
};

class BSPFile
{
private:
	Stream* _stream;

public:
	BSPWorld _world;
	Array<Texture*> _textures;
	Array<Texture*> _lightMaps;
	Dictionary<ShaderHash, ShaderMaterial*> _shaders;
	BaseArray<BSPEntity> _entities;

public:
	BSPFile();
	~BSPFile();

	bool LoadBSP(Stream* stream);

	bool ReadLump(lump_t* lump, int size, void** out, int32* num = NULL);
	void LoadShaders(lump_t* lump);
	void LoadLightmaps(lump_t* lump);
	void LoadPlanes(lump_t* lump);
	void LoadFogs(lump_t* lump, lump_t* brushesLump, lump_t* sidesLump);
	void LoadSurfaces(lump_t* lump, lump_t* vertsLump, lump_t* indexLump);
	void LoadLeafSurfaces(lump_t* lump);
	void LoadLeafBrushes(lump_t* lump);
	void LoadNodesAndLeafs(lump_t* nodeLump, lump_t* leafLump);
	void LoadSubmodels(lump_t* lump);
	void LoadVisibility(lump_t* lump);
	void LoadEntities(lump_t* lump);
	void LoadLightGrid(lump_t* lump);

	bool CreateShaders();
	bool CreateLightMaps();
	bool CreateEntities();
	ShaderMaterial* GetShader(int textureIndex, int lightMapIndex);

	BSPScene* CreateScene();
	bool CreateBuffers(BSPScene* scene);
	bool CreateSurfaces(BSPScene* scene);
	BSPSurface* CreatePatchSurface(dsurface_t* surface, VertexLayout* vertexLayout);
	BSPSurface* CreateTriangleSurface(dsurface_t* surface, VertexData* vertexData, HardwareBuffer* indexBuffer);
	BSPSurface* CreatePlanarSurface(dsurface_t* surface, VertexData* vertexData);
	bool CreateNodes(BSPScene* scene);
	void SpawnEntities(BSPScene* scene);
};

QuakeBSPSceneReader::QuakeBSPSceneReader()
{
	_bspFile = NULL;
}

QuakeBSPSceneReader::~QuakeBSPSceneReader()
{
	Destroy();
}

void QuakeBSPSceneReader::Destroy()
{
	SE_DELETE(_bspFile);
}

Scene* QuakeBSPSceneReader::LoadScene(Stream& stream, SceneReaderOptions* options)
{
	RenderSystem* renderSystem = RenderSystem::Current();
	if (renderSystem == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeBSPSceneReader.LoadScene"),
			_T("No active render system."));
		return NULL;
	}

	Destroy();

	_fileName = ((FileStream*)&stream)->GetFileName();
	_path = Path::GetDirectoryName(_fileName);

	_stream = &stream;
	if (!ReadBSP())
	{
		return NULL;
	}

	Scene* scene = CreateScene();
	return scene;
}

struct VertexBSP
{
	Vector3 Position;
	Vector3 Normal;
	uint32 Color;
	Vector2 TextureCoordinate;
	Vector2 LightmapCoordinate;
};

Scene* QuakeBSPSceneReader::CreateScene()
{
	return _bspFile->CreateScene();
}

bool QuakeBSPSceneReader::ReadBSP()
{
	BinaryStream reader(_stream);

	_bspFile = new BSPFile();
	_bspFile->LoadBSP(_stream);

	return true;
}

BSPFile::BSPFile()
{
	_world.shaders = NULL;
	_world.planes = NULL;
	_world.nodes = NULL;
	_world.leafs = NULL;
	_world.surfaces = NULL;
	_world.verts = NULL;
	_world.indexes = NULL;
	_world.leafsurfaces = NULL;
	_world.leafbrushes = NULL;
	_world.fogs = NULL;
	_world.lightMapData = NULL;
	_world.lightGridData = NULL;
	_world.vis = NULL;
	_world.novis = NULL;
	_world.entityString = NULL;

	_stream = NULL;
}

BSPFile::~BSPFile()
{
	SE_DELETE_ARRAY(_world.shaders);
	SE_DELETE_ARRAY(_world.planes);
	SE_DELETE_ARRAY(_world.nodes);
	SE_DELETE_ARRAY(_world.leafs);
	SE_DELETE_ARRAY(_world.surfaces);
	SE_DELETE_ARRAY(_world.verts);
	SE_DELETE_ARRAY(_world.indexes);
	SE_DELETE_ARRAY(_world.leafsurfaces);
	SE_DELETE_ARRAY(_world.leafbrushes);
	SE_DELETE_ARRAY(_world.fogs);
	SE_DELETE_ARRAY(_world.lightMapData);
	SE_DELETE_ARRAY(_world.lightGridData);
	SE_DELETE_ARRAY(_world.vis);
	SE_DELETE_ARRAY(_world.novis);
	SE_DELETE_ARRAY(_world.entityString);
}

bool BSPFile::LoadBSP(Stream* stream)
{
	_stream = stream;

	// Read the header
	dheader_t header;
	stream->Read((SEbyte*)&header, sizeof(dheader_t));

	// Validity checks
	if (header.ident != BSP_IDENT)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeBSPSceneReader.LoadModel"),
			_T("Invalid file."));
		return false;
	}

	if (header.version != BSP_VERSION)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuakeBSPSceneReader.LoadModel"),
			_T("Wrong version number."));
		return false;
	}

	LoadShaders(&header.lumps[LUMP_SHADERS]);
	LoadLightmaps(&header.lumps[LUMP_LIGHTMAPS]);
	LoadPlanes(&header.lumps[LUMP_PLANES]);
	LoadFogs(&header.lumps[LUMP_FOGS], &header.lumps[LUMP_BRUSHES], &header.lumps[LUMP_BRUSHSIDES]);
	LoadSurfaces(&header.lumps[LUMP_SURFACES], &header.lumps[LUMP_DRAWVERTS], &header.lumps[LUMP_DRAWINDEXES]);
	LoadLeafSurfaces(&header.lumps[LUMP_LEAFSURFACES]);
	LoadLeafBrushes(&header.lumps[LUMP_LEAFBRUSHES]);
	LoadNodesAndLeafs(&header.lumps[LUMP_NODES], &header.lumps[LUMP_LEAFS]);
	LoadSubmodels(&header.lumps[LUMP_MODELS]);
	LoadVisibility(&header.lumps[LUMP_VISIBILITY]);
	LoadEntities(&header.lumps[LUMP_ENTITIES]);
	LoadLightGrid(&header.lumps[LUMP_LIGHTGRID]);

	CreateShaders();
	CreateLightMaps();
	CreateEntities();

	return true;
}

bool BSPFile::ReadLump(lump_t* lump, int size, void** out, int32* num)
{
	if (lump->filelen % size)
	{
		return false;
	}

	if (lump->filelen == 0)
	{
		*out = NULL;
	}
	else
	{
		*out = new SEbyte[lump->filelen];
		if (_stream->Seek(lump->fileofs, SeekOrigin_Begin) == 0)
			return false;

		_stream->Read((SEbyte*)*out, lump->filelen);
	}

	if (num != NULL)
	{
		*num = lump->filelen / size;
	}
	return true;
}

void BSPFile::LoadShaders(lump_t* lump)
{
	ReadLump(lump, sizeof(dshader_t), (void**)&_world.shaders, &_world.numShaders);
}

void BSPFile::LoadLightmaps(lump_t* lump)
{
	ReadLump(lump, sizeof(SEbyte), (void**)&_world.lightMapData, &_world.lightMapBytes);
}

void BSPFile::LoadPlanes(lump_t* lump)
{
	ReadLump(lump, sizeof(dplane_t), (void**)&_world.planes, &_world.numPlanes);
}

void BSPFile::LoadFogs(lump_t* lump, lump_t* brushesLump, lump_t* sidesLump)
{
	ReadLump(lump, sizeof(dfog_t), (void**)&_world.fogs, &_world.numFogs);
	ReadLump(brushesLump, sizeof(dbrush_t), (void**)&_world.brushes, &_world.numBrushes);
	ReadLump(sidesLump, sizeof(dbrushside_t), (void**)&_world.brushSides, &_world.numBrushSides);
}

void BSPFile::LoadSurfaces(lump_t* lump, lump_t* vertsLump, lump_t* indexLump)
{
	ReadLump(lump, sizeof(dsurface_t), (void**)&_world.surfaces, &_world.numSurfaces);

	ReadLump(vertsLump, sizeof(drawVert_t), (void**)&_world.verts, &_world.numVerts);
	ReadLump(indexLump, sizeof(int), (void**)&_world.indexes, &_world.numIndexes);
}

void BSPFile::LoadLeafSurfaces(lump_t* lump)
{
	ReadLump(lump, sizeof(int), (void**)&_world.leafsurfaces, &_world.numLeafSurfaces);
}

void BSPFile::LoadLeafBrushes(lump_t* lump)
{
	ReadLump(lump, sizeof(int), (void**)&_world.leafbrushes, &_world.numLeafBrushes);
}

void BSPFile::LoadNodesAndLeafs(lump_t* nodeLump, lump_t* leafLump)
{
	ReadLump(nodeLump, sizeof(dnode_t), (void**)&_world.nodes, &_world.numDecisionNodes);
	ReadLump(leafLump, sizeof(dleaf_t), (void**)&_world.leafs, &_world.numLeafs);

	_world.numNodes = _world.numDecisionNodes + _world.numLeafs;
}

void BSPFile::LoadSubmodels(lump_t* lump)
{
}

void BSPFile::LoadVisibility(lump_t* lump)
{
	ReadLump(lump, sizeof(uint8), (void**)&_world.novis);

	_world.numClusters = ((int*)_world.novis)[0];
	_world.clusterBytes = ((int*)_world.novis)[1];
	_world.vis = _world.novis + 8;
}

void BSPFile::LoadEntities(lump_t* lump)
{
	ReadLump(lump, sizeof(char), (void**)&_world.entityString);
}

void BSPFile::LoadLightGrid(lump_t* lump)
{
	ReadLump(lump, sizeof(dplane_t), (void**)&_world.lightGridData);
}

bool BSPFile::CreateShaders()
{
	int i;
	Texture* texture;

	RenderSystem* renderSystem = RenderSystem::Current();

	for (i = 0 ; i < _world.numShaders; i++)
	{
		texture = NULL;
		String shaderName = _world.shaders[i].shader;
		if (String::Compare(shaderName, _T("noshader")) == 0)
		{
			_textures.Add(NULL);
			continue;
		}

		Texture* texture = NULL;
		String baseName = Path::ConvertSeparators(shaderName, false);

		String fileName = FileSystem::Instance()->GetFullPath(String::Concat(baseName, ".jpg"));
		if (!File::Exists(fileName))
		{
			fileName = FileSystem::Instance()->GetFullPath(String::Concat(baseName, ".tga"));
		}

		Resource* resource = ResourceManager::Instance()->Get(fileName);
		if (resource == NULL)
		{
			resource = ResourceHelper::LoadFromFile(fileName, SE_ID_DATA_IMAGE);
		}

		if (resource == NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("QuakeBSPSceneReader.LoadModel"),
				_T("Failed to load the image: ") + fileName);
			_textures.Add(NULL);
			continue;
		}

		Image* image = (Image*)resource->GetData();
		if (!renderSystem->CreateTexture(&texture) ||
			!texture->Create(image, TextureUsage_Static))
		{
			SE_DELETE(texture);
			Logger::Current()->Log(LogLevel::Error, _T("QuakeBSPSceneReader.LoadModel"),
				_T("Failed to create the texture: ") + fileName);
		}
		_textures.Add(texture);
	}

	return true;
}

bool BSPFile::CreateLightMaps()
{
	int i, j;
	int numLightmaps;
	SEbyte* lightMap;
	int intensity;
	Image lightMapImage;
	Texture* lightMapTexture;

	RenderSystem* renderSystem = RenderSystem::Current();

	numLightmaps = _world.lightMapBytes / LightMapTotalSize;
	intensity = r_mapOverBrightBits - r_overBrightBits;

	if (intensity > 0)
	{
		uint32 r, g, b, max;
		for (i = 0; i < numLightmaps; ++i)
		{
			lightMap = _world.lightMapData + i * LightMapTotalSize;

			for (j = 0; j < LightMapSize * LightMapSize; ++j)
			{
				r = lightMap[j*3] << intensity;
				g = lightMap[j*3+1] << intensity;
				b = lightMap[j*3+2] << intensity;

				max = Math::Max(r, Math::Max(g, b));

				if (max > 255)
				{
					max = (255 << 8) / max;
					r = (r * max) >> 8;
					g = (g * max) >> 8;
					b = (b * max) >> 8;
				}
				lightMap[j*3] = r;
				lightMap[j*3+1] = g;
				lightMap[j*3+2] = b;
			}
		}
	}

	for (i = 0 ; i < numLightmaps; ++i)
	{
		lightMap = _world.lightMapData + i * LightMapTotalSize;

		lightMapImage.Create(PixelFormat_R8G8B8A8, LightMapSize, LightMapSize);
		SEbyte* imageData = lightMapImage.GetData();

		uint32* pixel = (uint32*)imageData;
		int p = i * LightMapTotalSize;
		for (int j = 0; j < LightMapSize * LightMapSize; ++j)
		{
			*pixel = Color8(lightMap[j*3], lightMap[j*3+1], lightMap[j*3+2], 255).ToARGB();
			pixel++;
			p += 3;
		}

		if (!renderSystem->CreateTexture(&lightMapTexture) ||
			!lightMapTexture->Create(&lightMapImage, TextureUsage_Static))
		{
			SE_DELETE(lightMapTexture);
			_lightMaps.Add(NULL);
			continue;
		}

		_lightMaps.Add(lightMapTexture);
	}

	return true;
}

bool BSPFile::CreateEntities()
{
	if (_world.entityString == NULL)
		return false;

	int entitySize = String(_world.entityString).Length();
	MemoryStreamPtr entityStream = new MemoryStream((SEbyte*)_world.entityString, entitySize);
	TextStream stream(entityStream);
	stream.SetNewLine(_T("\n"));

	Dictionary<String, String> vars;
	bool inEntity = false;

	while (stream.Peek() != -1)
	{
		String line = stream.ReadLine();
		line.Trim();
		if (line.IsEmpty())
			continue;

		if (line[0] == '{')
		{
			if (inEntity)
			{
				// Error
				return false;
			}
			inEntity = true;
			continue;
		}
		if (line[0] == '}')
		{
			if (!inEntity)
			{
				// Error
				return false;
			}
			inEntity = false;

			if (vars.Count() == 0)
			{
				// No token in this entity
			}
			else
			{
				BSPEntity entity;
				entity.Vars = vars;
				_entities.Add(entity);
			}

			vars.Clear();
			continue;
		}

		int lastIndex = 0;
		int tokenCount = 0;
		String keyToken, valueToken;
		bool inToken = false;
		int length = line.Length();
		for (int i = 0; i  < length; ++i)
		{
			SEchar c = line[i];

			if (c == '\"')
			{
				if (inToken)
				{
					String token = line.Substring(lastIndex + 1, i - lastIndex - 1);
					if (tokenCount == 0)
					{
						keyToken = token;
					}
					else if (tokenCount == 1)
					{
						valueToken = token;
						vars.Add(keyToken, valueToken);
						break; // Ignore the other tokens, only 2 allowed
					}
					tokenCount++;
					inToken = false;
				}
				else
				{
					lastIndex = i;
					inToken = true;
				}
			}
		}
	}

	return true;
}

ShaderMaterial* BSPFile::GetShader(int textureIndex, int lightMapIndex)
{
	int i;

	ShaderHash hash(textureIndex, lightMapIndex);
	for (i = 0 ; i < _world.numShaders; i++)
	{
		if (_shaders.Contains(hash))
		{
			return _shaders[hash];
		}
	}
	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);

	pass->RasterizerState.CullMode = CullMode_None;
	pass->AlphaState.BlendEnable[0] = false;
	pass->LightState.Lighting = false;

	SamplerState* sampler0 = pass->GetSamplerStateByIndex(0);
	sampler0->SetTexture(_textures[textureIndex]);

	TextureState* texture0 = pass->GetTextureStateByIndex(0);
	texture0->TextureCoordinateIndex = 0;
	texture0->ColorOperation = TextureOperation_Modulate;
	texture0->ColorArgument1 = TextureArgument_TextureColor;
	texture0->ColorArgument2 = TextureArgument_Diffuse;
	texture0->AlphaOperation = TextureOperation_Disable;

	if (lightMapIndex >= 0)
	{
		SamplerState* sampler1 = new SamplerState();
		pass->AddSamplerState(sampler1);
		sampler1->SetTexture(_lightMaps[lightMapIndex]);

		TextureState* texture1 = new TextureState();
		pass->AddTextureState(texture1);
		texture1->TextureCoordinateIndex = 1;
		texture1->ColorOperation = TextureOperation_Modulate4X;
		texture1->ColorArgument1 = TextureArgument_TextureColor;
		texture1->ColorArgument2 = TextureArgument_Current;
		texture1->AlphaOperation = TextureOperation_Disable;
	}

	_shaders.Add(hash, shader);
	return shader;
}

BSPScene* BSPFile::CreateScene()
{
	BSPScene* scene = new BSPScene();

	if (!CreateBuffers(scene))
	{
		SE_DELETE(scene);
		return NULL;
	}

	if (!CreateSurfaces(scene))
	{
		SE_DELETE(scene);
		return NULL;
	}

	if (!CreateNodes(scene))
	{
		SE_DELETE(scene);
		return NULL;
	}

	SpawnEntities(scene);

	return scene;
}

bool BSPFile::CreateBuffers(BSPScene* scene)
{
	int i;
	RenderSystem* renderSystem = RenderSystem::Current();

	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* vertexBuffer = NULL;
	HardwareBuffer* indexBuffer = NULL;

	if (!renderSystem->CreateVertexLayout(&vertexLayout))
	{
		return false;
	}

	// Vertex description of a BSP vertices.
	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate, 1));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	// Create a vertex buffer and an index buffer for the whole scene.
	int vertexCount = _world.numVerts;
	int vertexStride = vertexLayout->GetSize();
	if (!renderSystem->CreateVertexBuffer(vertexCount * vertexStride, HardwareBufferUsage_Static, &vertexBuffer))
	{
		return NULL;
	}

	int indexCount = _world.numIndexes;
	if (!renderSystem->CreateIndexBuffer(indexCount * sizeof(int32), IndexBufferFormat_Int32, HardwareBufferUsage_Static, &indexBuffer))
	{
		return NULL;
	}

	VertexBSP* lockedVertices;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedVertices);
	for (i = 0; i < vertexCount; ++i)
	{
		lockedVertices[i].Position = Vector3(
			_world.verts[i].xyz[0], _world.verts[i].xyz[2], _world.verts[i].xyz[1]);
		lockedVertices[i].Normal = Vector3(
			_world.verts[i].normal[0], _world.verts[i].normal[2], _world.verts[i].normal[1]);
		lockedVertices[i].Color = Color8(_world.verts[i].color[0], _world.verts[i].color[1],
			_world.verts[i].color[2], _world.verts[i].color[3]).ToARGB();
		lockedVertices[i].TextureCoordinate = Vector2(_world.verts[i].st[0], _world.verts[i].st[1]);
		lockedVertices[i].LightmapCoordinate = Vector2(_world.verts[i].lightmap[0], _world.verts[i].lightmap[1]);
	}
	vertexBuffer->Unmap();

	uint32* lockedIndices;
	indexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedIndices);
	for (i = 0; i < indexCount; ++i)
	{
		lockedIndices[i] = _world.indexes[i];
	}
	indexBuffer->Unmap();

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexStride));
	vertexData->VertexCount = vertexCount;
	scene->_vertexData = vertexData;

	IndexData* indexData = new IndexData();
	indexData->IndexBuffer = indexBuffer;
	indexData->IndexCount = indexCount;
	scene->_indexData = indexData;

	scene->_leafFaces.Resize(_world.numLeafSurfaces);
	Memory::Copy(&scene->_leafFaces[0], _world.leafsurfaces, sizeof(int) * _world.numLeafSurfaces);
	return true;
}

bool BSPFile::CreateSurfaces(BSPScene* scene)
{
	int i;
	Array<BSPSurface*> surfaces;
	surfaces.SetCapacity(_world.numSurfaces);

	VertexData* vertexData = scene->GetVertexData();
	IndexData* indexData = scene->GetIndexData();

	for (i = 0; i < _world.numSurfaces; i++)
	{
		BSPSurface* surface = NULL;
		switch (_world.surfaces[i].surfaceType)
		{
		case MST_PATCH:
			surface = CreatePatchSurface(&_world.surfaces[i], vertexData->VertexLayout);
			break;
		case MST_TRIANGLE_SOUP:
			surface = CreateTriangleSurface(&_world.surfaces[i], vertexData, indexData->IndexBuffer);
			break;
		case MST_PLANAR:
			surface = CreatePlanarSurface(&_world.surfaces[i], vertexData);
			break;
		case MST_FLARE:
			break;
		default:
			Logger::Current()->Log(LogLevel::Error, _T("QuakeBSPSceneReader.LoadScene"),
				_T("Invalid surface type."));
		}

		if (surface != NULL)
		{
			surface->SetShader(GetShader(_world.surfaces[i].shaderNum, _world.surfaces[i].lightmapNum));
			surfaces.Add(surface);
		}
	}

	scene->_surfaces = surfaces;

	return true;
}

/*
	Flatness Test.

	Take three non-colinear vertices v0, v1, v2 to get the polygon plane equation
	ntx = d with n = (v1 - v2) * (v0 - v2) and d = vt
	For each remaining vertex vi check whether ntvi = d.
	If this fails for at least one vertex, the polygon is not flat.
*/
int Math_FlatnessTest(real max, const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	real length, distance;
	Vector3 d, t, w;

	d = p2 - p0;

	length = d.Length();
	if (length == 0)
		return 0;

	d /= length;
	t = p1 - p0;
	w = d * Vector3::Dot(t, d);
	t = t - w;

	distance = t.Length();
	if (Math::Abs(distance) < max)
		return 0;

	Vector3 v1, v2, v3;
	v1 = (p1 + p0) * 0.5;
	v2 = (p2 + p1) * 0.5;
	v3 = (v1 + v2) * 0.5;

	return 1 + Math::Max(Math_FlatnessTest(max, p0, v1, v3), Math_FlatnessTest(max, v3, v2, p2));
}

void Patch_GetFlatness(real32 maxFlatness, Vector3* controlsPoints, int numCPX, int numCPY, int& flatX, int& flatY)
{
	for (int y = 0; y < numCPY-1; y+=2)
	{
		for (int x = 0; x < numCPX-1; x+=2)
		{
			int index = y * numCPX + x;

			flatX = Math::Max(flatX, Math_FlatnessTest(maxFlatness,
				controlsPoints[index],
				controlsPoints[index+1],
				controlsPoints[index+2]));
			flatX = Math::Max(flatX, Math_FlatnessTest(maxFlatness,
				controlsPoints[index+numCPX],
				controlsPoints[index+numCPX+1],
				controlsPoints[index+numCPX+2]));
			flatX = Math::Max(flatX, Math_FlatnessTest(maxFlatness,
				controlsPoints[index+2*numCPX],
				controlsPoints[index+2*numCPX+1],
				controlsPoints[index+2*numCPX+2]));

			flatY = Math::Max(flatY, Math_FlatnessTest(maxFlatness,
				controlsPoints[index],
				controlsPoints[index+numCPX],
				controlsPoints[index+2*numCPX]));
			flatY = Math::Max(flatY, Math_FlatnessTest(maxFlatness,
				controlsPoints[index+1],
				controlsPoints[index+numCPX+1],
				controlsPoints[index+2*numCPX+1]));
			flatY = Math::Max(flatY, Math_FlatnessTest(maxFlatness,
				controlsPoints[index+2],
				controlsPoints[index+numCPX+2],
				controlsPoints[index+2*numCPX+2]));
		}
	}
}

template <class T>
T Math_EvaluateQuadricBezier(real t, T p0, T p1, T p2)
{
	real t2 = t * t;
	real t4 = t * (real)2.0;
	return (p0 * ((real)1.0 - t4 + t2)) + (p1 * (t4 - (real)2.0 * t2)) + (p2 * t2);
}

Vector2 Math_EvaluateQuadricBezier(real t, const Vector2& p0, const Vector2& p1, const Vector2& p2)
{
	real t2 = t * t;
	real t4 = t * (real)2.0;
	return (p0 * ((real)1.0 - t4 + t2)) + (p1 * (t4 - (real)2.0 * t2)) + (p2 * t2);
}

Vector3 Math_EvaluateQuadricBezier(real t, const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	real t2 = t * t;
	real t4 = t * (real)2.0;
	return (p0 * ((real)1.0 - t4 + t2)) + (p1 * (t4 - (real)2.0 * t2)) + (p2 * t2);
}

Color32 Math_EvaluateQuadricBezier(real t, const Color32& p0, const Color32& p1, const Color32& p2)
{
	real t2 = t * t;
	real t4 = t * (real)2.0;
	return (p0 * ((real)1.0 - t4 + t2)) + (p1 * (t4 - (real)2.0 * t2)) + (p2 * t2);
}

template <class T>
void Patch_Evaluate(T* controlPoints, int numCPX, int numCPY, int tessX, int tessY, T* dest)
{
	int numPatchesX = numCPX / 2;
	int numPatchesY = numCPY / 2;
	int numTessX, numTessY;
	real32 stepX = 1.0f / (real32)tessX;
	real32 stepY = 1.0f / (real32)tessY;
	int pitch = numPatchesX * tessX + 1;

	int index0, index1, index2;
	int u, v, x, y;
	real32 s, t;
	T patchPoints[3][3];
	T* temp;
	T v1, v2, v3;

	for (v = 0; v < numPatchesY; ++v)
	{
		// last patch has one more row 
		if (v < numPatchesY - 1)
			numTessY = tessY;
		else
			numTessY = tessY + 1;

		for (u = 0; u < numPatchesX; ++u)
		{
			// last patch has one more column
			if (u < numPatchesX - 1)
				numTessX = tessX;
			else
				numTessX = tessX + 1;

			index0 = (v * numCPX + u) << 1;
			index1 = index0 + numCPX;
			index2 = index1 + numCPX;

			// current 3x3 patch control points
			patchPoints[0][0] = controlPoints[index0];
			patchPoints[0][1] = controlPoints[index1];
			patchPoints[0][2] = controlPoints[index2];
			patchPoints[1][0] = controlPoints[index0+1];
			patchPoints[1][1] = controlPoints[index1+1];
			patchPoints[1][2] = controlPoints[index2+1];
			patchPoints[2][0] = controlPoints[index0+2];
			patchPoints[2][1] = controlPoints[index1+2];
			patchPoints[2][2] = controlPoints[index2+2];

			t = 0.0f;
			temp = dest + v * tessY * pitch + u * tessX;

			for (y = 0; y < numTessY; ++y, t += stepY)
			{
				v1 = Math_EvaluateQuadricBezier(t, patchPoints[0][0], patchPoints[0][1], patchPoints[0][2]);
				v2 = Math_EvaluateQuadricBezier(t, patchPoints[1][0], patchPoints[1][1], patchPoints[1][2]);
				v3 = Math_EvaluateQuadricBezier(t, patchPoints[2][0], patchPoints[2][1], patchPoints[2][2]);

				s = 0.0f;
				for (x = 0; x < numTessX; ++x)
				{
					temp[x] = Math_EvaluateQuadricBezier(s, v1, v2, v3);
					s += stepX;
				}

				temp += pitch;
			}
		}
	}
}

BSPSurface* BSPFile::CreatePatchSurface(dsurface_t* surface, VertexLayout* vertexLayout)
{
	int i;

	RenderSystem* renderSystem = RenderSystem::Current();
	HardwareBuffer* vertexBuffer = NULL;
	HardwareBuffer* indexBuffer = NULL;

	int numCPX = surface->patchWidth;
	int numCPY = surface->patchHeight;
	int numPatchesX = numCPX / 2;
	int numPatchesY = numCPY / 2;
	int flatX;
	int flatY;

	Vector3* positions = new Vector3[surface->numVerts];

	drawVert_t* verts = _world.verts + surface->firstVert;
	for (i = 0; i < surface->numVerts; ++i)
	{
		positions[i] = Vector3(verts[i].xyz[0], verts[i].xyz[2], verts[i].xyz[1]);
	}

	Patch_GetFlatness(r_curvefactor, positions, numCPX, numCPY, flatX, flatY);

	int tessX = (1<<flatX);
	int tessY = (1<<flatY);

	int numVertsX = numPatchesX * tessX + 1;
	int numVertsY = numPatchesY * tessY + 1;

	int vertexCount = numVertsX * numVertsY;
	int vertexStride = vertexLayout->GetSize();
	if (!renderSystem->CreateVertexBuffer(vertexCount * vertexStride, HardwareBufferUsage_Static, &vertexBuffer))
	{
		return NULL;
	}

	int indexCount = 6 * (numVertsX-1) * (numVertsY-1);
	if (!renderSystem->CreateIndexBuffer(indexCount * sizeof(int32), IndexBufferFormat_Int32, HardwareBufferUsage_Static, &indexBuffer))
	{
		return NULL;
	}

	Vector3* normals = new Vector3[surface->numVerts];
	Color32* colors = new Color32[surface->numVerts];
	Vector2* textureCoords = new Vector2[surface->numVerts];
	Vector2* lightmapCoords = new Vector2[surface->numVerts];

	for (i = 0; i < surface->numVerts; ++i)
	{
		normals[i] = Vector3(verts[i].normal[0], verts[i].normal[2], verts[i].normal[1]);
		colors[i] = Color32::FromUInt8(verts[i].color[0], verts[i].color[1],
			verts[i].color[2], verts[i].color[3]);
		textureCoords[i] = Vector2(verts[i].st[0], verts[i].st[1]);
		lightmapCoords[i] = Vector2(verts[i].lightmap[0], verts[i].lightmap[1]);
	}

	Vector3* destPositions = new Vector3[vertexCount];
	Vector3* destNormals = new Vector3[vertexCount];
	Color32* destColors = new Color32[vertexCount];
	Vector2* destTextureCoords = new Vector2[vertexCount];
	Vector2* destLightmapCoords = new Vector2[vertexCount];

	Patch_Evaluate(positions, numCPX, numCPY, tessX, tessY, destPositions);
	Patch_Evaluate(normals, numCPX, numCPY, tessX, tessY, destNormals);
	Patch_Evaluate((Color32*)colors, numCPX, numCPY, tessX, tessY, (Color32*)destColors);
	Patch_Evaluate(textureCoords, numCPX, numCPY, tessX, tessY, destTextureCoords);
	Patch_Evaluate(lightmapCoords, numCPX, numCPY, tessX, tessY, destLightmapCoords);

	VertexBSP* lockedVertices;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedVertices);
	for (i = 0; i < vertexCount; ++i)
	{
		lockedVertices[i].Position = destPositions[i];
		lockedVertices[i].Normal = destNormals[i];
		lockedVertices[i].Color = destColors[i].ToARGB();
		lockedVertices[i].TextureCoordinate = destTextureCoords[i];
		lockedVertices[i].LightmapCoordinate = destLightmapCoords[i];
	}
	vertexBuffer->Unmap();

	uint32* lockedIndices;
	indexBuffer->Map(HardwareBufferMode_Normal, (void**)&lockedIndices);
	int x, y;
	int index = 0;
	int vertex = numVertsX;
	for (y = 0; y < numVertsY-1; ++y)
	{
		for (x = 0; x < numVertsX-1; ++x)
		{
			lockedIndices[index++] = vertex;
			lockedIndices[index++] = vertex + 1;
			lockedIndices[index++] = vertex - numVertsX;

			lockedIndices[index++] = vertex - numVertsX;
			lockedIndices[index++] = vertex + 1;
			lockedIndices[index++] = vertex + 1 - numVertsX;

			vertex++;
		}
		vertex++;
	}
	indexBuffer->Unmap();

	delete[] destPositions;
	delete[] destNormals;
	delete[] destColors;
	delete[] destTextureCoords;
	delete[] destLightmapCoords;

	delete[] positions;
	delete[] normals;
	delete[] colors;
	delete[] textureCoords;
	delete[] lightmapCoords;

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexStride));
	vertexData->VertexCount = vertexCount;

	IndexData* indexData = new IndexData();
	indexData->IndexBuffer = indexBuffer;
	indexData->IndexCount = indexCount;

	RenderData* renderData = new RenderData();
	renderData->VertexData = vertexData;
	renderData->IndexData = indexData;
	renderData->IsIndexed = true;
	renderData->PrimitiveCount = indexCount / 3;
	renderData->Type = PrimitiveType_TriangleList;

	BSPSurface* bspSurface = new BSPSurface();
	bspSurface->_renderData = renderData;
	return bspSurface;
}

BSPSurface* BSPFile::CreateTriangleSurface(dsurface_t* surface, VertexData* vertexData, HardwareBuffer* indexBuffer)
{
	RenderData* renderData = new RenderData();
	renderData->VertexData = vertexData;

	if (surface->numIndexes > 0)
	{
		IndexData* indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = surface->numIndexes;
		renderData->IndexData = indexData;
		renderData->StartVertex = surface->firstVert;
		renderData->StartIndex = surface->firstIndex;
		renderData->IsIndexed = true;
		renderData->Type = PrimitiveType_TriangleList;
		renderData->PrimitiveCount = surface->numIndexes / 3;
	}
	else
	{
		renderData->StartVertex = surface->firstVert;
		renderData->Type = PrimitiveType_TriangleFan;
		renderData->PrimitiveCount = surface->numVerts - 2;
	}

	BSPSurface* bspSurface = new BSPSurface();
	bspSurface->_renderData = renderData;
	return bspSurface;
}

BSPSurface* BSPFile::CreatePlanarSurface(dsurface_t* surface, VertexData* vertexData)
{
	RenderData* renderData = new RenderData();
	renderData->VertexData = vertexData;

	renderData->StartVertex = surface->firstVert;
	renderData->PrimitiveCount = surface->numVerts - 2;
	renderData->Type = PrimitiveType_TriangleFan;

	BSPSurface* bspSurface = new BSPSurface();
	bspSurface->_renderData = renderData;
	return bspSurface;
}

bool BSPFile::CreateNodes(BSPScene* scene)
{
	int i, j;
	scene->_nodeCount = _world.numNodes;
	scene->_leafStart = _world.numDecisionNodes;
	scene->_leafCount = _world.numLeafs;

	scene->_nodes.Resize(_world.numNodes);

	// Nodes
	for (i = 0; i < _world.numDecisionNodes; ++i)
	{
		dnode_t* node = &_world.nodes[i];

		BSPNode* bspNode = &scene->_nodes[i];

		bspNode->_isLeaf = false;

		dplane_t* plane = &_world.planes[node->planeNum];
		bspNode->_plane = Plane(Vector3(plane->normal[0], plane->normal[2], plane->normal[1]), -plane->dist);

		bspNode->_bounds = AABB(
			Vector3(node->mins[0], node->mins[2], node->mins[1]),
			Vector3(node->maxs[0], node->maxs[2], node->maxs[1]));

		// A negative index represents a leaf
		if (node->children[0] < 0)
		{
			// Negate the index
			bspNode->_frontNode = &scene->_nodes[_world.numDecisionNodes + (~(node->children[0]))];
		}
		else
		{
			bspNode->_frontNode = &scene->_nodes[node->children[0]];
		}

		// A negative index represents a leaf
		if (node->children[1] < 0)
		{
			// Negate the index
			bspNode->_backNode = &scene->_nodes[_world.numDecisionNodes + (~(node->children[1]))];
		}
		else
		{
			bspNode->_backNode = &scene->_nodes[node->children[1]];
		}
	}

	// Brushes
	scene->_brushes.Resize(_world.numBrushes);
	for (i = 0; i < _world.numBrushes; ++i)
	{
		dbrush_t* brush = &_world.brushes[i];

		BSPBrush* bspBrush = &scene->_brushes[i];

		bspBrush->planes.SetCapacity(brush->numSides);
		for (j = 0; j < brush->numSides; ++j)
		{
			dbrushside_t* brushSide = &_world.brushSides[brush->firstSide + j];
			dplane_t* plane = &_world.planes[brushSide->planeNum];

			bspBrush->planes.Add(Plane(
				Vector3(plane->normal[0], plane->normal[2], plane->normal[1]), plane->dist));
		}

	}

	// Leaves
	for (i = 0; i < _world.numLeafs; ++i)
	{
		dleaf_t* leaf = &_world.leafs[i];

		BSPNode* bspNode = &scene->_nodes[_world.numDecisionNodes + i];

		bspNode->_isLeaf = true;

		bspNode->_bounds = AABB(
			Vector3(leaf->mins[0], leaf->mins[2], leaf->mins[1]),
			Vector3(leaf->maxs[0], leaf->maxs[2], leaf->maxs[1]));

		bspNode->_faceCount = leaf->numLeafSurfaces;
		bspNode->_faceStart = leaf->firstLeafSurface;
		bspNode->_clusterIndex = leaf->cluster;

		for (j = 0; j < leaf->numLeafBrushes; ++j)
		{
			int leafbrush = _world.leafbrushes[leaf->firstLeafBrush + j];
			dbrush_t* brush = &_world.brushes[leafbrush];
			dshader_t* brushshader = &_world.shaders[brush->shaderNum];

			if ((brushshader->contentFlags & CONTENTS_SOLID) != 0)
			{
				BSPBrush* bspBrush = &scene->_brushes[leafbrush];
				bspNode->_solidBrushes.Add(bspBrush);
			}
		}
	}

	// Clusters
	scene->_clusterCount = _world.numClusters;
	scene->_clusterStride = _world.clusterBytes;
	scene->_clusterData = new uint8[_world.numClusters * _world.clusterBytes];
	Memory::Copy(scene->_clusterData, (void*)_world.vis, _world.numClusters * _world.clusterBytes);

	return true;
}

String GetEntityVar_String(BSPEntity* entity, const String& key)
{
	if (!entity->Vars.ContainsKey(key))
		return String::Empty;
	else
		return entity->Vars[key];
}

real32 GetEntityVar_Scalar(BSPEntity* entity, const String& key)
{
	return GetEntityVar_String(entity, key).ToReal32();
}

Vector3 GetEntityVar_Vector3(BSPEntity* entity, const String& key)
{
	String var = GetEntityVar_String(entity, key);
	Array<String> params = var.Split(_T(' '));
	if (params.Count() != 3)
		return Vector3::Zero;
	else
		return Vector3(params[0].ToReal32(), params[2].ToReal32(), params[1].ToReal32());
}

Color32 GetEntityVar_Color32(BSPEntity* entity, const String& key)
{
	String var = GetEntityVar_String(entity, key);
	Array<String> params = var.Split(_T(' '));
	if (params.Count() != 3)
		return Color32::Black;
	else
		return Color32(params[0].ToReal32(), params[2].ToReal32(), params[1].ToReal32());
}

Model* GetModel(const String& name)
{
	Model* model;
	Resource* resource;

	String path = FileSystem::Instance()->GetFullPath(name);
	resource = ResourceManager::Instance()->Get(path);
	if (resource == NULL)
	{
		resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_MODEL);
	}
	if (resource == NULL)
		return NULL;

	model = (Model*)resource->GetData();
	model->SetName(Path::GetFileNameWithoutExtension(name));

	return model;
}

void BSPFile::SpawnEntities(BSPScene* scene)
{
	int i;
	int entityCount = _entities.Count();

	RenderSystem* renderSystem = RenderSystem::Current();

	for (i = 0; i < entityCount; ++i)
	{
		BSPEntity* entity = &_entities[i];

		String classname = GetEntityVar_String(entity, _T("classname"));
		if (classname.IsEmpty())
			continue;

		if (String::Compare(classname, _T("worldspawn")) == 0)
		{
			scene->SetName(GetEntityVar_String(entity, _T("message")));
		}

		else if (String::Compare(classname, _T("misc_model")) == 0)
		{
			Model* model = GetModel(Path::ConvertSeparators(GetEntityVar_String(entity, _T("model")), false));
			if (model != NULL)
			{
				ModelNode* modelNode = new ModelNode();
				modelNode->SetModel(model);
				modelNode->SetLocalPosition(GetEntityVar_Vector3(entity, _T("origin")));
				scene->AddObject(modelNode);
			}
		}

		else if (String::Compare(classname, _T("target_location")) == 0)
		{
			SceneObject* sceneObject = new SceneNode();
			sceneObject->SetName(GetEntityVar_String(entity, _T("message")));
			sceneObject->SetLocalPosition(GetEntityVar_Vector3(entity, _T("origin")));
			scene->AddObject(sceneObject);
		}

		else if (String::Compare(classname, _T("target_position")) == 0)
		{
			SceneObject* sceneObject = new SceneNode();
			sceneObject->SetName(GetEntityVar_String(entity, _T("targetname")));
			sceneObject->SetLocalPosition(GetEntityVar_Vector3(entity, _T("origin")));
			scene->AddObject(sceneObject);
		}

		else if (String::Compare(classname, _T("light")) == 0)
		{
			PointLight* light = new PointLight();
			light->SetLocalPosition(GetEntityVar_Vector3(entity, _T("origin")));
			light->SetDiffuseColor(GetEntityVar_Color32(entity, _T("_color")));
			light->SetRange(GetEntityVar_Scalar(entity, _T("range")));
			scene->AddObject(light);
		}

		else if (String::Compare(classname, _T("info_player_deathmatch")) == 0)
		{
			Camera* camera = new Camera();
			camera->SetPerspective(45.0f,
				renderSystem->GetViewport().GetWidth() / renderSystem->GetViewport().GetHeight(),
				1.0f, 10000.0f);
			camera->SetLocalPosition(GetEntityVar_Vector3(entity, _T("origin")));
			camera->SetLocalOrientation(Quaternion::CreateFromAxisAngle(Vector3::UnitY,
				Math::ToRadians(GetEntityVar_Scalar(entity, _T("angle")))));
			scene->AddObject(camera);
		}
	}
}

}
