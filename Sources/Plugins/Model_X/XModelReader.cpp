/*=============================================================================
XModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XModelReader.h"

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("XModelReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("XModelReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("XModelReader"), ##message##);

namespace SE_X
{

XModelReader::XModelReader() :
	ModelReader()
{
}

XModelReader::~XModelReader()
{
	Destroy();
}

Model* XModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		SE_LOGERROR(_T("No active renderer."));
		return NULL;
	}

	_stream = &stream;
	_fileName = ((FileStream*)_stream)->GetFileName();
	_path = Path::GetDirectoryName(_fileName);

	if (!ReadModel())
	{
		return NULL;
	}

	return CreateModel();
}

void XModelReader::XMatrix4x4ToTransform(real32* xmatrix, Vector3& position, Quaternion& orientation, Vector3& scale)
{
	Matrix4 matrix = Matrix4::Transpose(Matrix4(xmatrix));
	position = Vector3(matrix.M03, matrix.M13, matrix.M23);

	//TODO: unscale the matrix
	Matrix3 rotationMatrix = Matrix3(
		matrix.M00, matrix.M01, matrix.M02,
		matrix.M10, matrix.M11, matrix.M12,
		matrix.M20, matrix.M21, matrix.M22);

	orientation = Quaternion::CreateFromRotationMatrix(rotationMatrix);
	scale = Vector3::One;
}

bool XModelReader::ReadModel()
{
	*_stream >> _xfile.headerASCII.Magic;
	*_stream >> _xfile.headerASCII.Version;
	*_stream >> _xfile.headerASCII.Format;
	*_stream >> _xfile.headerASCII.FloatSize;

	if (_xfile.headerASCII.Magic != XOFFILE_FORMAT_MAGIC)
		return false;

	if (_xfile.headerASCII.Version != XOFFILE_FORMAT_VERSION2 &&
		_xfile.headerASCII.Version != XOFFILE_FORMAT_VERSION3)
		return false;

	if (_xfile.headerASCII.Format != XOFFILE_FORMAT_BINARY)
		return false;

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_TEMPLATE)
		{
			ReadTemplate();
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("Header"))
			{
				ReadHeader(&_xfile.header);
			}
			else if (name == _T("Material"))
			{
				XMaterial* material = new XMaterial();
				ReadMaterial(material);
				_xfile.materials.Add(material);
			}
			else if (name == _T("Frame"))
			{
				XFrame* frame = new XFrame();
				ReadFrame(frame);
				_xfile.frames.Add(frame);
			}
			else if (name == _T("Mesh"))
			{
				XFrame* frame = new XFrame();
				XMesh* mesh = new XMesh();
				ReadMesh(mesh);
				frame->meshes.Add(mesh);
				_xfile.frames.Add(frame);
			}
			else if (name == _T("AnimTicksPerSecond"))
			{
				ReadBeginBlock();
				ReadIntegerList(&_xfile.animTicksPerSecond.AnimTicksPerSecond, 1);
				ReadEndBlock();
			}
			else if (name == _T("AnimationSet"))
			{
				XAnimationSet* animationSet = new XAnimationSet();
				ReadAnimationSet(animationSet);
				_xfile.animationSets.Add(animationSet);
			}
			else
			{
				SkipBlock(name);
			}
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}

	return true;
}

uint16 XModelReader::ReadToken()
{
	uint16 token;
	*_stream >> token;
	return token;
}

void XModelReader::ReadBeginBlock()
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_OBRACE);
}

void XModelReader::ReadEndBlock()
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_CBRACE);
}

void XModelReader::SkipBlock()
{
	uint16 token = ReadToken();
	while (!_stream->IsEOF())
	{
		if (token == TOKEN_OBRACE)
			SkipBlock();
		if (token == TOKEN_CBRACE)
			return;
		token = ReadToken();
	}
}

void XModelReader::SkipBlock(const String& name)
{
	SE_LOGNOTICE(String::Concat("SkipBlock: ", name));
	SkipBlock();
}

String XModelReader::GetString()
{
	uint32 length;
	*_stream >> length;
	char* buffer = new char[length+1];
	_stream->Read((SEbyte*)buffer, length);
	buffer[length] = '\0';
	return buffer;
}

String XModelReader::ReadName()
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_NAME);
	return GetString();
}

void XModelReader::ReadType()
{
	uint16 token = ReadToken();
	if (token == TOKEN_NAME)
	{
		GetString();
	}
	else if (token == TOKEN_INTEGER)
	{
		uint32 value;
		*_stream >> value;
	}
}

String XModelReader::ReadBlockName()
{
	uint16 token = ReadToken();
	if (token == TOKEN_NAME)
	{
		String name = GetString();
		ReadBeginBlock();
		return name;
	}
	else
	{
		SE_ASSERT(token == TOKEN_OBRACE);
		return "";
	}
}

String XModelReader::ReadString()
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_STRING);
	String str = GetString();
	token = ReadToken();
	SE_ASSERT(token == TOKEN_SEMICOLON);
	return str;
}

void XModelReader::ReadGUID()
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_GUID);

	uint32 data1;
	uint16 data2;
	uint16 data3;
	uint8 data4[8];
	*_stream >> data1;
	*_stream >> data2;
	*_stream >> data3;
	_stream->Read(data4, 8 * sizeof(uint8));
}

void XModelReader::ReadIntegerList(uint32** list)
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_INTEGER_LIST);

	uint32 size;
	*_stream >> size;

	*list = new uint32[size];
	_stream->Read((SEbyte*)*list, size * sizeof(uint32));
}

void XModelReader::ReadIntegerList(uint32* list, int count)
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_INTEGER_LIST);

	uint32 size;
	*_stream >> size;

	SE_ASSERT(count == size);
	_stream->Read((SEbyte*)list, size * sizeof(uint32));
}

void XModelReader::ReadFloatList(real32** list)
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_FLOAT_LIST);

	uint32 size;
	*_stream >> size;

	*list = new real32[size];
	_stream->Read((SEbyte*)*list, size * sizeof(real32));
}

void XModelReader::ReadFloatList(real32* list, int count)
{
	uint16 token = ReadToken();
	SE_ASSERT(token == TOKEN_FLOAT_LIST);

	uint32 size;
	*_stream >> size;

	SE_ASSERT(count == size);
	_stream->Read((SEbyte*)list, size * sizeof(real32));
}

void XModelReader::ReadHeader(XHeader* header)
{
	ReadBeginBlock();
	ReadIntegerList(header->header, 3);
	ReadEndBlock();
}

void XModelReader::ReadTemplate()
{
	String name = ReadBlockName();
	ReadGUID();
	uint16 token = ReadToken();
	while (token != TOKEN_CBRACE)
	{
		if (token == TOKEN_ARRAY)
		{
			ReadType();
			ReadName();
			ReadToken(); //TOKEN_OBRACKET
			ReadType();
			ReadToken(); //TOKEN_CBRACKET
			ReadToken(); //TOKEN_SEMICOLON
		}
		else if (token == TOKEN_LPSTR)
		{
			ReadName();
			ReadToken(); //TOKEN_SEMICOLON
		}
		else if (token == TOKEN_DOT)
		{
			ReadToken(); //TOKEN_DOT
			ReadToken(); //TOKEN_DOT
		}
		else if (token == TOKEN_OBRACKET)
		{
			token = ReadToken();
			if (token == TOKEN_NAME)
			{
				GetString();
				ReadGUID();
			}
			else if (token == TOKEN_DOT)
			{
				ReadToken(); //TOKEN_DOT
				ReadToken(); //TOKEN_DOT
			}
			ReadToken(); //TOKEN_CBRACKET
		}
		else if (token == TOKEN_NAME)
		{
			GetString();
			ReadName();
			ReadToken(); //TOKEN_SEMICOLON
		}
		else
		{
			ReadName();
			ReadToken(); //TOKEN_SEMICOLON
		}
		token = ReadToken();
	}
}

void XModelReader::ReadMaterial(XMaterial* material)
{
	material->name = ReadBlockName();

	real32* list = NULL;
	ReadFloatList(&list);
	if (list != NULL)
	{
		XColorRGBA faceColor = { list[0], list[1], list[2], list[3] };
		material->faceColor = faceColor;
		material->power = list[4];

		XColorRGB specularColor = { list[5], list[6], list[7] };
		material->specularColor = specularColor;

		XColorRGB emissiveColor = { list[8], list[9], list[10] };
		material->emissiveColor = emissiveColor;

		delete[] list;
	}

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("TextureFilename"))
			{
				ReadBeginBlock();
				material->textureFilename.filename = ReadString();
				ReadEndBlock();
			}
			else if (name == _T("EffectInstance"))
			{
				material->effectInstance = new XEffectInstance();
				ReadEffectInstance(material->effectInstance);
			}
			else
			{
				SkipBlock(name);
			}
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}
}

void XModelReader::ReadFrame(XFrame* frame)
{
	frame->name = ReadBlockName();

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("FrameTransformMatrix"))
			{
				ReadBeginBlock();
				ReadFloatList(frame->transformMatrix.frameMatrix.matrix, 16);
				ReadEndBlock();
			}
			else if (name == _T("Mesh"))
			{
				XMesh* mesh = new XMesh();
				ReadMesh(mesh);
				frame->meshes.Add(mesh);
			}
			else if (name == _T("Frame"))
			{
				XFrame* child = new XFrame();
				ReadFrame(child);
				frame->frames.Add(child);
			}
			else
			{
				SkipBlock(name);
			}
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}
}

void XModelReader::ReadMesh(XMesh* mesh)
{
	mesh->name = ReadBlockName();

	ReadIntegerList(&mesh->nVertices, 1);

	mesh->vertices = new XVector[mesh->nVertices];
	ReadFloatList((real32*)mesh->vertices, mesh->nVertices*3);

	uint32* faces = NULL;
	ReadIntegerList(&faces);
	if (faces != NULL)
	{
		mesh->nFaces = faces[0];
		mesh->faces = new XMeshFace[mesh->nFaces];
		uint f = 0;
		uint k = 1;
		while (f < mesh->nFaces)
		{
			uint32 nFaceVertexIndices = faces[k++];
			mesh->faces[f].nFaceVertexIndices = nFaceVertexIndices;
			mesh->faces[f].faceVertexIndices = new uint32[nFaceVertexIndices];
			uint j = 0;
			while (j < nFaceVertexIndices)
			{
#if 0
				mesh->faces[f].faceVertexIndices[nFaceVertexIndices-1-j++] = faces[k++];
#else
				mesh->faces[f].faceVertexIndices[j++] = faces[k++];
#endif
			}
			f++;
		}
		delete[] faces;
	}

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("MeshNormals"))
			{
				ReadMeshNormals(&mesh->normals);
			}
			else if (name == _T("MeshVertexColors"))
			{
				ReadMeshVertexColors(&mesh->vertexColors);
			}
			else if (name == _T("MeshTextureCoords"))
			{
				ReadMeshTextureCoords(&mesh->textureCoords);
			}
			else if (name == _T("MeshMaterialList"))
			{
				ReadMeshMaterialList(&mesh->materialList);
			}
			else if (name == _T("VertexDuplicationIndices"))
			{
				ReadVertexDuplicationIndices(&mesh->vertexDuplicationIndices);
			}
			else if (name == _T("XSkinMeshHeader"))
			{
				ReadSkinMeshHeader(&mesh->skinMeshHeader);
			}
			else if (name == _T("SkinWeights"))
			{
				XSkinWeights* skinWeights = new XSkinWeights();
				ReadSkinWeights(skinWeights);
				mesh->skinWeights.Add(skinWeights);
			}
			else
			{
				SkipBlock(name);
			}
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}
}

void XModelReader::ReadMeshNormals(XMeshNormals* meshNormals)
{
	ReadBeginBlock();

	ReadIntegerList(&meshNormals->nNormals, 1);

	meshNormals->normals = new XVector[meshNormals->nNormals];
	ReadFloatList((real32*)meshNormals->normals, meshNormals->nNormals*3);

	uint32* faceNormals = NULL;
	ReadIntegerList(&faceNormals);
	if (faceNormals != NULL)
	{
		delete[] faceNormals;
	}
	meshNormals->nFaceNormals = 0;//faceNormals[0];
	/*uint f = 1;
	uint count = faceNormals.Count();
	while (f < count)
	{
		XMeshFace meshFace;
		meshFace.nFaceVertexIndices = faceNormals[f++];
		uint j = 0;
		while (j < meshFace.nFaceVertexIndices)
		{
			meshFace.faceVertexIndices.Add(faceNormals[f++]);
			j++;
		}
		meshNormals->faceNormals.Add(meshFace);
	}*/

	ReadEndBlock();
}

void XModelReader::ReadMeshVertexColors(XMeshVertexColors* meshVertexColors)
{
	ReadBeginBlock();

	uint32* intlist = NULL;
	ReadIntegerList(&intlist);
	if (intlist != NULL)
	{
		meshVertexColors->nVertexColors = intlist[0];
		if (meshVertexColors->nVertexColors > 0)
		{
			meshVertexColors->vertexColors = new XIndexedColor[meshVertexColors->nVertexColors];
			meshVertexColors->vertexColors[0].index = intlist[1];

			ReadFloatList((real32*)&meshVertexColors->vertexColors[0].indexColor, 4);
		}

		delete[] intlist;
	}

	for (uint i = 1; i < meshVertexColors->nVertexColors; ++i)
	{
		ReadIntegerList((uint32*)&meshVertexColors->vertexColors[i].index, 1);
		ReadFloatList((real32*)&meshVertexColors->vertexColors[i].indexColor, 4);
	}

	ReadEndBlock();
}

void XModelReader::ReadMeshTextureCoords(XMeshTextureCoords* meshTextureCoords)
{
	ReadBeginBlock();

	ReadIntegerList(&meshTextureCoords->nTextureCoords, 1);

	meshTextureCoords->textureCoords = new XCoords2d[meshTextureCoords->nTextureCoords];
	ReadFloatList((real32*)meshTextureCoords->textureCoords, meshTextureCoords->nTextureCoords*2);

	ReadEndBlock();
}

void XModelReader::ReadMeshMaterialList(XMeshMaterialList* meshMaterialList)
{
	ReadBeginBlock();

	uint32* materials = NULL;
	ReadIntegerList(&materials);
	if (materials != NULL)
	{
		meshMaterialList->nMaterials = materials[0];
		meshMaterialList->nFaceIndexes = materials[1];
		meshMaterialList->faceIndexes = new uint32[meshMaterialList->nFaceIndexes];
		Memory::Copy(meshMaterialList->faceIndexes, materials + 2, sizeof(uint32) * meshMaterialList->nFaceIndexes);
	}

	for (uint i = 0; i < meshMaterialList->nMaterials; i++)
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_OBRACE)
		{
			String name = GetString();
			BaseArray<XMaterial*>::Iterator it = _xfile.materials.GetIterator();
			while (it.Next())
			{
				if (name == it.Current()->name)
				{
					meshMaterialList->materials.Add(it.Current());
					break;
				}
			}
			ReadEndBlock();
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("Material"))
			{
				XMaterial* material = new XMaterial();
				ReadMaterial(material);
				meshMaterialList->materials.Add(material);
			}
		}
		else
		{
			SkipBlock();
		}
	}

	ReadEndBlock();
}

void XModelReader::ReadVertexDuplicationIndices(XVertexDuplicationIndices* vertexDuplicationIndices)
{
	ReadBeginBlock();

	uint32* intlist = NULL;
	ReadIntegerList(&intlist);
	if (intlist != NULL)
	{
		vertexDuplicationIndices->nIndices = intlist[0];
		vertexDuplicationIndices->nIndices = intlist[1];
		if (vertexDuplicationIndices->nIndices > 0)
		{
			vertexDuplicationIndices->indices = new uint32[vertexDuplicationIndices->nIndices];
			Memory::Copy(vertexDuplicationIndices->indices, intlist + 2,
				sizeof(uint32) * vertexDuplicationIndices->nIndices);
		}

		delete[] intlist;
	}

	ReadEndBlock();
}

void XModelReader::ReadSkinMeshHeader(XSkinMeshHeader* skinMeshHeader)
{
	ReadBeginBlock();

	ReadIntegerList((uint32*)skinMeshHeader, 3);

	ReadEndBlock();
}

void XModelReader::ReadSkinWeights(XSkinWeights* skinWeights)
{
	ReadBeginBlock();

	skinWeights->transformNodeName = ReadString();

	uint32* intlist = NULL;
	ReadIntegerList(&intlist);
	if (intlist != NULL)
	{
		skinWeights->nWeights = intlist[0];

		skinWeights->vertexIndices = new uint32[skinWeights->nWeights];
		Memory::Copy(skinWeights->vertexIndices, intlist + 1, sizeof(uint32) * skinWeights->nWeights);

		delete[] intlist;
	}

	real32* floatlist = NULL;
	ReadFloatList(&floatlist);
	if (floatlist != NULL)
	{
		skinWeights->weights = new real32[skinWeights->nWeights];
		Memory::Copy(skinWeights->weights, floatlist, sizeof(real32) * skinWeights->nWeights);
		Memory::Copy(skinWeights->matrixOffset.matrix, floatlist + skinWeights->nWeights, sizeof(real32) * 16);

		delete[] floatlist;
	}

	ReadEndBlock();
}

void XModelReader::ReadAnimationSet(XAnimationSet* animationSet)
{
	animationSet->name = ReadBlockName();

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("Animation"))
			{
				XAnimation* animation = new XAnimation();
				ReadAnimation(animation);
				animationSet->animations.Add(animation);
			}
			else
			{
				SkipBlock(name);
			}
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}
}

void XModelReader::ReadAnimation(XAnimation* animation)
{
	animation->name = ReadBlockName();

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("AnimationKey"))
			{
				XAnimationKey* animationKey = new XAnimationKey();
				ReadAnimationKey(animationKey);
				animation->animationKeys.Add(animationKey);
			}
		}
		else if (token == TOKEN_OBRACE)
		{
			animation->frame = ReadName();
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}

	ReadEndBlock();
}

void XModelReader::ReadAnimationKey(XAnimationKey* animationKey)
{
	ReadBeginBlock();

	uint32* intlist = NULL;
	ReadIntegerList(&intlist);
	if (intlist != NULL)
	{
		animationKey->keyType = intlist[0];
		animationKey->nKeys = intlist[1];

		animationKey->keys = new XTimedFloatKeys[animationKey->nKeys];
		if (animationKey->nKeys > 0)
		{
			animationKey->keys[0].time = intlist[2];
			animationKey->keys[0].tfkeys.nValues = intlist[3];
			animationKey->keys[0].tfkeys.values = new real32[animationKey->keys[0].tfkeys.nValues];
			ReadFloatList(animationKey->keys[0].tfkeys.values, animationKey->keys[0].tfkeys.nValues);
		}

		delete[] intlist;
	}

	for (uint i = 1; i < animationKey->nKeys; ++i)
	{
		intlist = NULL;
		ReadIntegerList(&intlist);
		animationKey->keys[i].time = intlist[0];
		animationKey->keys[i].tfkeys.nValues = intlist[1];
		delete[] intlist;

		animationKey->keys[i].tfkeys.values = new real32[animationKey->keys[i].tfkeys.nValues];
		ReadFloatList(animationKey->keys[i].tfkeys.values, animationKey->keys[i].tfkeys.nValues);
	}

	ReadEndBlock();
}

void XModelReader::ReadEffectInstance(XEffectInstance* effectInstance)
{
	ReadBeginBlock();

	while (!_stream->IsEOF())
	{
		uint16 token = ReadToken();
		if (token == TOKEN_CBRACE)
		{
			break;
		}
		else if (token == TOKEN_NAME)
		{
			String name = GetString();
			if (name == _T("EffectParamString"))
			{
				ReadBeginBlock();
				XEffectParamString effectParam = XEffectParamString();
				effectParam.paramName = ReadString();
				effectParam.value = ReadString();
				effectInstance->stringParams.Add(effectParam);
				ReadEndBlock();
			}
			else if (name == _T("EffectParamDWord"))
			{
				ReadBeginBlock();
				XEffectParamDWord effectParam = XEffectParamDWord();
				effectParam.paramName = ReadString();
				ReadIntegerList(&effectParam.value, 1);
				effectInstance->dwordParams.Add(effectParam);
				ReadEndBlock();
			}
			else if (name == _T("EffectParamFloats"))
			{
				ReadBeginBlock();
				XEffectParamFloats effectParam = XEffectParamFloats();
				effectParam.paramName = ReadString();
				ReadIntegerList(&effectParam.nFloats, 1);
				ReadFloatList(&effectParam.floats);
				effectInstance->floatsParams.Add(effectParam);
				ReadEndBlock();
			}
		}
		else if (token == TOKEN_STRING)
		{
			effectInstance->effectFilename = GetString();
			ReadToken(); //TOKEN_SEMICOLON
		}
		else
		{
			SE_LOGNOTICE(String::Concat("Unknown token: ", String::ToString(token)));
		}
	}
}

void XModelReader::Destroy()
{
	int i;

	for (i = 0; i < _xfile.frames.Count(); ++i)
	{
		DestroyFrame(_xfile.frames[i]);
		SE_DELETE(_xfile.frames[i]);
	}
	_xfile.frames.Clear();

	for (i = 0; i < _xfile.materials.Count(); ++i)
	{
		SE_DELETE(_xfile.materials[i]->effectInstance);
	}
	_xfile.materials.Clear();

	for (i = 0; i < _xfile.animationSets.Count(); ++i)
	{
		DestroyAnimationSet(_xfile.animationSets[i]);
		SE_DELETE(_xfile.animationSets[i]);
	}
	_xfile.animationSets.Clear();
}

void XModelReader::DestroyFrame(XFrame* frame)
{
	int i;

	for (i = 0; i < frame->frames.Count(); ++i)
	{
		DestroyFrame(frame->frames[i]);
		SE_DELETE(frame->frames[i]);
	}

	for (i = 0; i < frame->meshes.Count(); ++i)
	{
		DestroyMesh(frame->meshes[i]);
		SE_DELETE(frame->meshes[i]);
	}
}

void XModelReader::DestroyMesh(XMesh* mesh)
{
	uint i;

	SE_DELETE_ARRAY(mesh->vertices);

	for (i = 0; i < mesh->nFaces; ++i)
	{
		SE_DELETE_ARRAY(mesh->faces[i].faceVertexIndices);
	}
	SE_DELETE_ARRAY(mesh->faces);

	for (i = 0; i < mesh->normals.nFaceNormals; ++i)
	{
		SE_DELETE_ARRAY(mesh->normals.faceNormals[i].faceVertexIndices);
	}
	SE_DELETE_ARRAY(mesh->normals.normals);
	SE_DELETE_ARRAY(mesh->normals.faceNormals);

	SE_DELETE_ARRAY(mesh->vertexColors.vertexColors);

	SE_DELETE_ARRAY(mesh->textureCoords.textureCoords);
	SE_DELETE_ARRAY(mesh->materialList.faceIndexes);

	SE_DELETE_ARRAY(mesh->vertexDuplicationIndices.indices);
}

void XModelReader::DestroyAnimationSet(XAnimationSet* animationSet)
{
	int i, j;
	for (i = 0; i < animationSet->animations.Count(); ++i)
	{
		for (j = 0; j < animationSet->animations[j]->animationKeys.Count(); ++j)
		{
			DestroyAnimationKey(animationSet->animations[i]->animationKeys[j]);
			SE_DELETE(animationSet->animations[i]->animationKeys[j]);
		}
	}
}

void XModelReader::DestroyAnimationKey(XAnimationKey* animationKey)
{
	uint i;
	for (i = 0; i < animationKey->nKeys; ++i)
	{
		SE_DELETE_ARRAY(animationKey->keys[i].tfkeys.values);
	}
	SE_DELETE_ARRAY(animationKey->keys);
}

Model* XModelReader::CreateModel()
{
	if (_xfile.frames.Count() == 0)
	{
		return NULL;
	}

	int i;
	Model* model = new Model();
	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	CreateMaterials(_materials, _xfile.materials);

	Bone* rootBone = skeleton->AddBone();
	skeleton->SetRootBone(rootBone);

	if (_xfile.frames.Count() == 1)
	{
		CreateFrame(model, rootBone, _xfile.frames[0]);
	}
	else
	{
		for (i = 0; i < _xfile.frames.Count(); ++i)
		{
			Bone* bone = skeleton->AddBone();
			rootBone->AddChild(bone);
			CreateFrame(model, bone, _xfile.frames[i]);
		}
	}

	for (i = 0; i < _xfile.frames.Count(); ++i)
	{
		CreateFrameSkin(skeleton, _xfile.frames[i]);
	}
	rootBone->Update();

	if (_xfile.animationSets.Count() > 0)
	{
		AnimationSet* animationSet = new AnimationSet();
		for (i = 0; i < _xfile.animationSets.Count(); ++i)
		{
			CreateAnimationSet(animationSet, skeleton, _xfile.animationSets[i]);
		}
		skeleton->SetAnimationSet(animationSet);
	}

	return model;
}

Texture* XModelReader::CreateTexture(const String& name)
{
	String path = Path::Combine(_path, name);
	Resource* resource = ResourceManager::Instance()->Get(path);
	if (resource == NULL)
	{
		resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_IMAGE);
	}
	if (resource != NULL)
	{
		Texture* texture;
		if (RenderSystem::Current()->CreateTexture(&texture) &&
			texture->Create((Image*)resource->GetData(), TextureUsage_Static))
		{
			return texture;
		}
	}

	return NULL;
}

void XModelReader::CreateMaterials(Array<ShaderMaterial*>& materials, const BaseArray<XMaterial*>& xmaterials)
{
	int i;
	int count;

	BaseArray<XMaterial*>::Iterator it = xmaterials.GetIterator();
	while (it.Next())
	{
		XMaterial* xmaterial = it.Current();

		ShaderMaterial* shader = NULL;
		if (xmaterial->effectInstance != NULL)
		{
			String effectPath = Path::Combine(_path, xmaterial->effectInstance->effectFilename);
			EffectShader* effectShader = ShaderSystem::Current()->CreateEffectShader(effectPath);
			if (effectShader != NULL)
			{
				EffectParameter* effectParameter;

				effectParameter = effectShader->GetParameterBySemantic("SasGlobal");
				if (effectParameter != NULL)
				{
					shader = new SasEffectMaterial();
				}
				else
				{
					shader = new EffectMaterial();
				}
				((EffectMaterial*)shader)->SetEffectShader(effectShader);

				count = xmaterial->effectInstance->stringParams.Count();
				for (i = 0; i < count; ++i)
				{
					XEffectParamString* xeffectparam = &xmaterial->effectInstance->stringParams[i];
					effectParameter = effectShader->GetParameterByName(xeffectparam->paramName);
					if (effectParameter != NULL &&
						effectParameter->GetParameterDesc().ParameterType == ShaderParameterType_Texture)
					{
						effectParameter->SetValue(CreateTexture(xeffectparam->value));
					}
				}

				count = xmaterial->effectInstance->floatsParams.Count();
				for (i = 0; i < count; ++i)
				{
					XEffectParamFloats* xeffectparam = &xmaterial->effectInstance->floatsParams[i];
					effectParameter = effectShader->GetParameterByName(xeffectparam->paramName);
					if (effectParameter != NULL &&
						effectParameter->GetParameterDesc().ParameterType == ShaderParameterType_Float)
					{
						effectParameter->SetValue(xeffectparam->floats);
					}
				}

				count = xmaterial->effectInstance->dwordParams.Count();
				for (i = 0; i < count; ++i)
				{
					XEffectParamDWord* xeffectparam = &xmaterial->effectInstance->dwordParams[i];
					effectParameter = effectShader->GetParameterByName(xeffectparam->paramName);
					if (effectParameter != NULL &&
						effectParameter->GetParameterDesc().ParameterType == ShaderParameterType_Integer)
					{
						effectParameter->SetValue(&xeffectparam->value);
					}
				}
			}
		}

		if (shader == NULL)
		{
			shader = new DefaultMaterial();
			FFPPass* pass = (FFPPass*)((DefaultMaterial*)shader)->GetTechnique()->GetPassByIndex(0);

			pass->SetName(xmaterial->name);
			pass->RasterizerState.CullMode = CullMode_None;
			pass->MaterialState.DiffuseColor = Color32(xmaterial->faceColor.red,
				xmaterial->faceColor.green, xmaterial->faceColor.blue, xmaterial->faceColor.alpha);
			pass->MaterialState.SpecularColor = Color32(xmaterial->specularColor.red,
				xmaterial->specularColor.green, xmaterial->specularColor.blue);
			pass->MaterialState.EmissiveColor = Color32(xmaterial->emissiveColor.red,
				xmaterial->emissiveColor.green, xmaterial->emissiveColor.blue);
			pass->MaterialState.Shininess = xmaterial->power;
			pass->LightState.Lighting = true;
			if (xmaterial->faceColor.alpha != 1.0f)
			{
				pass->AlphaState.BlendEnable[0] = true;
				pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
				pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
			}

			if (!xmaterial->textureFilename.filename.IsEmpty())
			{
				SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
				samplerState->SetTexture(CreateTexture(xmaterial->textureFilename.filename));
			}
		}

		materials.Add(shader);
	}
}

void XModelReader::CreateFrame(Model* model, Bone* bone, XFrame* xframe)
{
	bone->SetName(xframe->name);

	Vector3 localPosition;
	Quaternion localOrientation;
	Vector3 localScale;
	XMatrix4x4ToTransform(xframe->transformMatrix.frameMatrix.matrix, localPosition, localOrientation, localScale);
	bone->SetReferencePosition(localPosition);
	bone->SetReferenceOrientation(localOrientation);
	bone->SetReferenceScale(localScale);
	bone->SetTransformToReference();

	int i;
	for (i = 0; i < xframe->meshes.Count(); ++i)
	{
		Mesh* mesh = new Mesh();
		//mesh->SetParentBone(bone);
		model->AddMesh(mesh);
		CreateMesh(mesh, xframe->meshes[i]);
	}

	for (i = 0; i < xframe->frames.Count(); ++i)
	{
		Bone* child = bone->GetSkeleton()->AddBone();
		bone->AddChild(child);
		CreateFrame(model, child, xframe->frames[i]);
	}
}

void XModelReader::CreateMesh(Mesh* mesh, XMesh* xmesh)
{
	uint i, j, k;
	RenderSystem* renderer = RenderSystem::Current();

	if (xmesh->nVertices == 0)
	{
		return;
	}

	Array<ShaderMaterial*> materials;
	CreateMaterials(materials, xmesh->materialList.materials);

	mesh->SetName(xmesh->name);

	VertexLayout* vertexLayout;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	uint16 offset = 0;

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	if (xmesh->skinMeshHeader.nBones > 0)
	{
		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_UByte4, VertexSemantic_BlendIndices));
		offset += VertexElement::GetTypeSize(VertexFormat_UByte4);

		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float4, VertexSemantic_BlendWeight));
		offset += VertexElement::GetTypeSize(VertexFormat_Float4);
	}

	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	if (xmesh->textureCoords.nTextureCoords > 0)
	{
		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
		offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	}

	if (!renderer->UpdateVertexLayout(vertexLayout))
	{
		SE_DELETE(vertexLayout);
		return;
	}

	// Convert or compute normals
	BaseArray<Vector3> normals(xmesh->normals.nNormals);
	if (xmesh->normals.nNormals > 0)
	{
		for (i = 0; i < xmesh->normals.nNormals; i++)
		{
			XVector xnormal = xmesh->normals.normals[i];
			normals[i] = Vector3(xnormal.x, xnormal.y, xnormal.z);
		}
	}
	else
	{
		//TODO: compute normals
	}

	// Setup skinning data
	std::vector<bool> blendBool(xmesh->nVertices * 4);
	BaseArray<uint8> blendIndices(xmesh->nVertices * 4);
	BaseArray<real32> blendWeights(xmesh->nVertices * 4);
	int vertexIndex;
	real32 weight;

	//Memory::Set((void*)&blendBool[0], 4 * xmesh->nVertices, 0);
	//Memory::Set((void*)&blendIndices[0], 4 * xmesh->nVertices, 0);
	//Memory::Set((void*)&blendWeights[0], 4 * xmesh->nVertices, 0);
	for (i = 0; i < xmesh->nVertices; ++i)
	{
		for (k = 0; k < 4; ++k)
		{
			blendBool[4*i+k] = false;
			blendIndices[4*i+k] = 0;
			blendWeights[4*i+k] = 0.0f;
		}
	}

	Skin* skin = NULL;
	if (xmesh->skinMeshHeader.nBones > 0)
	{
		int count = xmesh->skinWeights.Count();

		skin = new Skin();
		skin->SkinVertices.Resize(count);
		for (i = 0; i < (uint)count; ++i)
		{
			skin->SkinVertices[i].BoneIndex = GetFrameIndex(xmesh->skinWeights[i]->transformNodeName);
			skin->SkinVertices[i].BoneInfluences.Resize(xmesh->skinWeights[i]->nWeights);
			for (j = 0; j < xmesh->skinWeights[i]->nWeights; ++j)
			{
				vertexIndex = xmesh->skinWeights[i]->vertexIndices[j];
				weight = xmesh->skinWeights[i]->weights[j];

				skin->SkinVertices[i].BoneInfluences[j].VertexIndex = vertexIndex;
				skin->SkinVertices[i].BoneInfluences[j].Weight = weight;

				for (k = 0; k < 4; ++k)
				{
					if (!blendBool[4*vertexIndex+k])
					{
						blendIndices[4*vertexIndex+k] = i;
						blendWeights[4*vertexIndex+k] = weight;
						blendBool[4*vertexIndex+k] = true;
						break;
					}
				}
			}
		}
	}

	uint32 vertexCount = xmesh->nVertices;
	HardwareBuffer* vertexBuffer;
	if (!RenderSystem::Current()->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(), HardwareBufferUsage_Static, &vertexBuffer))
	{
		SE_DELETE(vertexLayout);
		return;
	}

	SEbyte* vertices;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vertices);

	for (i = 0; i < xmesh->nVertices; ++i)
	{
		Memory::Copy(vertices, &xmesh->vertices[i], sizeof(Vector3));
		vertices += sizeof(Vector3);

		if (xmesh->skinMeshHeader.nBones > 0)
		{
			Memory::Copy(vertices, &blendIndices[4*i], sizeof(uint8) * 4);
			vertices += sizeof(uint8) * 4;

			Memory::Copy(vertices, &blendWeights[4*i], sizeof(real32) * 4);
			vertices += sizeof(real32) * 4;
		}

		Memory::Copy(vertices, normals[i].Data, sizeof(Vector3));
		vertices += sizeof(Vector3);

		if (xmesh->textureCoords.nTextureCoords > 0)
		{
			Memory::Copy(vertices, &xmesh->textureCoords.textureCoords[i], sizeof(Vector2));
			vertices += sizeof(Vector2);
		}
	}

	vertexBuffer->Unmap();

	uint32 indexCount = 0;
	HardwareBuffer* indexBuffer;
	if (xmesh->nFaces > 0)
	{
		indexCount = xmesh->nFaces * 3;
		if (!RenderSystem::Current()->CreateIndexBuffer(indexCount * sizeof(uint32), IndexBufferFormat_Int32, HardwareBufferUsage_Static, &indexBuffer))
		{
			SE_DELETE(vertexLayout);
			SE_DELETE(vertexBuffer);
			return;
		}

		uint32* indices;
		indexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&indices);

		for (i = 0; i < xmesh->nFaces; i++)
		{
			//TODO: check for triangles faces
			for (uint j = 0; j < 3; j++)
			{
				indices[3*i+j] = xmesh->faces[i].faceVertexIndices[2-j];
			}
		}

		indexBuffer->Unmap();
	}

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	mesh->SetVertexData(vertexData);

	IndexData* indexData = NULL;
	if (indexCount > 0)
	{
		indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;
		mesh->SetIndexData(indexData);
	}

	if (xmesh->materialList.nMaterials == 1 || xmesh->materialList.nFaceIndexes == 0)
	{
		MeshPart* meshPart = new MeshPart();
		mesh->AddMeshPart(meshPart);

		meshPart->SetVertexData(vertexData);
		if (indexCount > 0)
		{
			meshPart->SetIndexData(indexData);
			meshPart->SetIndexed(true);
			meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, indexCount);
		}
		else
		{
			meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, vertexCount);
		}

		meshPart->SetShader(materials[0]);
	}
	else
	{
		// Split the mesh in multiple mesh parts if there are multiple materials
		uint32 faceCount = 1;
		uint32 lastCount = 0;
		uint32 lastIndex = xmesh->materialList.faceIndexes[0];
		for (i = 1; i < xmesh->materialList.nFaceIndexes; ++i)
		{
			if ((i == xmesh->materialList.nFaceIndexes - 1) || (lastIndex != xmesh->materialList.faceIndexes[i]))
			{
				MeshPart* meshPart = new MeshPart();
				mesh->AddMeshPart(meshPart);

				meshPart->SetVertexData(vertexData);
				if (indexCount > 0)
				{
					meshPart->SetIndexData(indexData);
					meshPart->SetIndexed(true);
				}

				meshPart->SetPrimitiveType(PrimitiveType_TriangleList);
				meshPart->SetPrimitiveCount(faceCount);
				meshPart->SetStartIndex(lastCount * 3);

				meshPart->SetShader(materials[lastIndex]);

				lastIndex = xmesh->materialList.faceIndexes[i];
				lastCount += faceCount;
				faceCount = 1;
			}
			faceCount++;
		}
	}
	
	BoundingBox boundingBox = BoundingBox::CreateFromPoints((Vector3*)xmesh->vertices, xmesh->nVertices);
	mesh->SetBoundingBox(boundingBox);
	mesh->SetBoundingSphere(BoundingSphere::CreateFromBox(boundingBox));

	if (skin != NULL)
	{
		mesh->SetSkin(skin);
	}
}

uint32 XModelReader::GetFrameIndex(const String& name)
{
	uint32 index = 0;
	if (_xfile.frames.Count() > 1)
	{
		index++;
	}
	return GetFrameIndex(name, _xfile.frames, index);
}

uint32 XModelReader::GetFrameIndex(const String& name, const BaseArray<XFrame*>& xframes, uint32& index)
{
	uint32 result;
	for (int i = 0; i < xframes.Count(); ++i)
	{
		if (xframes[i]->name == name)
		{
			return index;
		}
		index++;
		result = GetFrameIndex(name, xframes[i]->frames, index);
		if (result != -1)
		{
			return result;
		}
	}
	return -1;
}

void XModelReader::CreateFrameSkin(Skeleton* skeleton, XFrame* xframe)
{
	int i;
	for (i = 0; i < xframe->meshes.Count(); ++i)
	{
		CreateSkin(skeleton, xframe->meshes[i]);
	}

	for (i = 0; i < xframe->frames.Count(); ++i)
	{
		CreateFrameSkin(skeleton, xframe->frames[i]);
	}
}

void XModelReader::CreateSkin(Skeleton* skeleton, XMesh* xmesh)
{
	int i;
	for (i = 0; i < xmesh->skinWeights.Count(); ++i)
	{
		Bone* bone = skeleton->GetBoneByName(xmesh->skinWeights[i]->transformNodeName);
		if (bone != NULL)
		{
			Matrix4 matrix = Matrix4::Invert(Matrix4(xmesh->skinWeights[i]->matrixOffset.matrix));
			bone->SetPoseTransform(Matrix4::Transpose(Matrix4((real32*)&matrix.Data)));
		}
	}
}

void XModelReader::CreateAnimationSet(AnimationSet* animationSet, Skeleton* skeleton, XAnimationSet* xanimationset)
{
	int i, j, k;

	AnimationSequence* sequence = new AnimationSequence();
	sequence->SetName(xanimationset->name);
	TimeValue endTime;

	int trackCount = xanimationset->animations.Count();
	for (i = 0; i < trackCount; ++i)
	{
		XAnimation* xanimation = xanimationset->animations[i];

		BoneAnimationTrack* track = new BoneAnimationTrack();
		track->SetBone(skeleton->GetBoneByName(xanimation->frame));
		TimeValue trackTime;

		int keyCount = xanimation->animationKeys.Count();
		for (j = 0; j < keyCount; ++j)
		{
			XAnimationKey* xanimationkey = xanimation->animationKeys[j];

			TimeValue keyTime;
			Vector3 translation;
			Quaternion rotation;
			Vector3 scale;

			for (k = 0; k < (int)xanimationkey->nKeys; ++k)
			{
				XTimedFloatKeys* xtimedfloatkeys = &xanimationkey->keys[k];

				keyTime = TimeValue((real64)xtimedfloatkeys->time /
					(real64)_xfile.animTicksPerSecond.AnimTicksPerSecond);

				TransformKeyFrame* keyFrame = (TransformKeyFrame*)track->GetKeyFrameAtTime(keyTime);
				if (keyFrame == NULL)
				{
					keyFrame = (TransformKeyFrame*)track->AddKeyFrame(keyTime);
					keyFrame->SetTransformType((TransformKeyFrameType)0);
				}

				if (xanimationkey->keyType == 2 && xtimedfloatkeys->tfkeys.nValues == 3)
				{
					keyFrame->SetTransformType((TransformKeyFrameType)
						(keyFrame->GetTransformType() | TransformKeyFrameType_Translation));

					translation = Vector3(xtimedfloatkeys->tfkeys.values);
					keyFrame->SetTranslation(translation);
				}
				else if (xanimationkey->keyType == 0 && xtimedfloatkeys->tfkeys.nValues == 4)
				{
					keyFrame->SetTransformType((TransformKeyFrameType)
						(keyFrame->GetTransformType() | TransformKeyFrameType_Rotation));

					rotation = Quaternion::Invert(Quaternion(
						xtimedfloatkeys->tfkeys.values[1], xtimedfloatkeys->tfkeys.values[2],
						xtimedfloatkeys->tfkeys.values[3], xtimedfloatkeys->tfkeys.values[0]));
					keyFrame->SetRotation(rotation);
				}
				else if (xanimationkey->keyType == 1 && xtimedfloatkeys->tfkeys.nValues == 3)
				{
					keyFrame->SetTransformType((TransformKeyFrameType)
						(keyFrame->GetTransformType() | TransformKeyFrameType_Scale));

					scale = Vector3(xtimedfloatkeys->tfkeys.values);
					keyFrame->SetScale(scale);
				}
				else if (xanimationkey->keyType == 4 && xtimedfloatkeys->tfkeys.nValues == 16)
				{
					keyFrame->SetTransformType(TransformKeyFrameType_All);

					XMatrix4x4ToTransform(xtimedfloatkeys->tfkeys.values, translation, rotation, scale);
					keyFrame->SetTranslation(translation);
					keyFrame->SetRotation(rotation);
					keyFrame->SetScale(scale);
				}
			}

			if (keyTime > trackTime)
			{
				trackTime = keyTime;
			}
		}

		track->SetEndTime(trackTime);
		sequence->AddAnimationTrack(track);

		if (trackTime > endTime)
		{
			endTime = trackTime;
		}
	}

	sequence->SetEndTime(endTime);
	animationSet->AddAnimationSequence(sequence);
}

}
