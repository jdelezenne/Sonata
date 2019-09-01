/*=============================================================================
OBJModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "OBJModelReader.h"

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("OBJModelReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("OBJModelReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("OBJModelReader"), ##message##);

namespace SE_OBJ
{

OBJModelReader::OBJModelReader() :
	ModelReader()
{
}

OBJModelReader::~OBJModelReader()
{
}

Model* OBJModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		SE_LOGERROR(_T("No active renderer."));
		return NULL;
	}

	ReadOBJ(stream);

	if (_elements.Count() == 0)
		return NULL;

	Dictionary<String, ShaderMaterial*> shaders;

	BaseArray<OBJMaterial>::Iterator itMat = _materials.GetIterator();
	while (itMat.Next())
	{
		const OBJMaterial& material = itMat.Current();

		ShaderMaterial* shader = new Shader();
		ShaderTechnique* technique = new ShaderTechnique();
		ShaderPass* pass = new ShaderPass();
		pass->MaterialState.AmbientColor = Color32(material.Ka[0], material.Ka[1], material.Ka[2]);
		pass->MaterialState.DiffuseColor = Color32(material.Kd[0], material.Kd[1], material.Kd[2]);
		pass->MaterialState.SpecularColor = Color32(material.Ks[0], material.Ks[1], material.Ks[2]);
		if (!material.map_Kd.IsEmpty())
		{
			//String path = FileSystem::Instance()->GetFullPath(material.map_Kd);
			String source = ((FileStream*)&stream)->GetFileName();
			String path = Path::Combine(Path::GetDirectoryName(source), material.map_Kd);

			Resource* resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_IMAGE);
			if (resource != NULL)
			{
				Texture* texture;
				if (renderer->CreateTexture((Image*)resource->GetData(), TextureUsage_Static, &texture))
				{
					SamplerState* samplerState = new SamplerState();
					samplerState->SetTexture(texture);
					pass->AddSamplerState(samplerState);
					pass->AddTextureState(new TextureState());
				}
			}
		}
		technique->AddPass(pass);
		shader->SetName(material.name);
		shader->AddTechnique(technique);
		shaders.Add(material.name, shader);
	}

	_model = new Model();

	BaseArray<OBJElement>::Iterator it = _elements.GetIterator();
	while (it.Next())
	{
		const OBJElement& element = it.Current();

		bool hasNormals = (element.normals.Count() != 0);
		bool hasTexCoords = (element.texcoords.Count() != 0);

		if (element.vertices.Count() == 0)
		{
			SE_LOGERROR(_T("No vertices found."));
			return NULL;
		}

		if (hasNormals)
		{
			if (element.vertices.Count() < element.normals.Count())
			{
				SE_LOGWARNING(_T("The number of vertices is lower than the number of normals."));
			}
			else if (element.vertices.Count() != element.normals.Count())
			{
				SE_LOGERROR(_T("The number of vertices is different than the number of normals."));
				return NULL;
			}
		}

		if (hasTexCoords)
		{
			if (element.vertices.Count() < element.texcoords.Count())
			{
				SE_LOGWARNING(_T("The number of vertices is lower than the number of texture coordinates."));
			}
			else if (element.vertices.Count() != element.texcoords.Count())
			{
				SE_LOGERROR(_T("The number of vertices is different than the number of texture coordinates."));
				return NULL;
			}
		}

		VertexLayout* vertexLayout;
		if (!renderer->CreateVertexLayout(&vertexLayout))
		{
			return NULL;
		}

		uint16 offset = 0;
		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
		offset += VertexElement::GetTypeSize(VertexFormat_Float3);
		if (hasNormals)
		{
			vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
			offset += VertexElement::GetTypeSize(VertexFormat_Float3);
		}
		if (hasTexCoords)
		{
			vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
			offset += VertexElement::GetTypeSize(VertexFormat_Float2);
		}

		if (!renderer->UpdateVertexLayout(vertexLayout))
		{
			delete vertexLayout;
			return NULL;
		}

		uint32 vertexCount = element.faces.Count() * 3;//vertices.Count();
		HardwareBuffer* vertexBuffer;
		if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
			HardwareBufferUsage_Static, &vertexBuffer))
		{
			delete vertexLayout;
			return NULL;
		}

		SEbyte* vertices;
		vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vertices);
		int faceCount = element.faces.Count();
		for (int f = 0; f < faceCount; ++f)
		{
			for (int v = 0; v < 3; ++v)
			{
				Memory::Copy(vertices, (void*)&element.vertices[element.faces[f].vertex[v].position-1], sizeof(Vector3));
				vertices += sizeof(Vector3);

				if (hasNormals)
				{
					int32 normal = element.faces[f].vertex[v].normal;
					if (normal > 0)
					{
						Memory::Copy(vertices, (void*)&element.normals[normal-1], sizeof(Vector3));
						vertices += sizeof(Vector3);
					}
				}

				if (hasTexCoords)
				{
					int32 texcoord = element.faces[f].vertex[v].texcoord;
					if (texcoord > 0)
					{
						Memory::Copy(vertices, (void*)&element.texcoords[texcoord-1], sizeof(Vector2));
						vertices += sizeof(Vector2);
					}
				}
			}
		}
		vertexBuffer->Unmap();

		MeshPart* meshPart = new MeshPart();
		meshPart->SetName(element.name);

		VertexData* vertexData = new VertexData();
		vertexData->VertexLayout = vertexLayout;
		vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
		vertexData->VertexCount = vertexCount;
		meshPart->SetVertexData(vertexData);

		meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, vertexCount);
		meshPart->SetShader(shaders[element.material]);

		Mesh* mesh = new Mesh();
		mesh->AddMeshPart(meshPart);
		_model->AddMesh(mesh);
	}

	return _model;
}

void OBJModelReader::ReadOBJ(Stream& stream)
{
	OBJElement element;
	OBJFace face;
	String mtlFileName;

	TextStream reader(&stream);

	String line;
	int num = 0;
	while (!stream.IsEOF())
	{
		line = reader.ReadLine();
		num++;

		if (line[0] == _T('#') || line[0] == _T('$'))
		{
			SE_LOGNOTICE(line);
			continue;
		}

		if (line.Length() == 0)
			continue;

		Char separator = _T(' ');
		if (line.IndexOf(_T(' ')) == -1)
			separator = _T('\t');

		Array<String> tokens = line.Split(separator);
		if (tokens.Count() == 0)
		{
			SE_LOGWARNING(_T("Invalid line #") + String::ToString(num));
			continue;
		}

		// v x y z (w)
		if (tokens[0] == (String)_T("v"))
		{
			if (tokens.Count() < 4)
			{
				SE_LOGWARNING(_T("Invalid vertex at line #") + String::ToString(num));
				continue;
			}

			element.vertices.Add(Vector3(tokens[1].ToReal32(), tokens[2].ToReal32(), tokens[3].ToReal32()));
		}

		// vn i j k
		else if (tokens[0] == (String)_T("vn"))
		{
			if (tokens.Count() != 4)
			{
				SE_LOGWARNING(_T("Invalid normal at line #") + String::ToString(num));
				continue;
			}

			element.normals.Add(Vector3(tokens[1].ToReal32(), tokens[2].ToReal32(), tokens[3].ToReal32()));
		}

		// vt u v (w)
		else if ((tokens.Count() == 3 || tokens.Count() == 4) && tokens[0] == (String)_T("vt"))
		{
			if (tokens.Count() < 3)
			{
				SE_LOGWARNING(_T("Invalid texcoord at line #") + String::ToString(num));
				continue;
			}

			element.texcoords.Add(Vector2(tokens[1].ToReal32(), tokens[2].ToReal32()));
		}

		// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
		else if (tokens[0] == (String)_T("f"))
		{
			if (tokens.Count() != 4) // only triangles
			{
				SE_LOGWARNING(_T("Invalid face at line #") + String::ToString(num));
				continue;
			}

			bool invalid = false;
			for (int i = 0; i < 3; i++)
			{
				Array<String> values = tokens[i+1].Split(_T('/'));
				if ((values.Count() < 1) || (values.Count() > 3))
				{
					invalid = true;
					break;
				}

				face.vertex[i].normal = -1;
				face.vertex[i].texcoord = -1;

				face.vertex[i].position = values[0].ToInt32();
				if (values.Count() == 2) // v/vn || v//vn
					face.vertex[i].normal = values[1].ToInt32();
				else if (values.Count() == 3) // v/vt/vn
				{
					face.vertex[i].texcoord = values[1].ToInt32();
					face.vertex[i].normal = values[2].ToInt32();
				}
			}
			if (invalid)
			{
				SE_LOGWARNING(_T("Invalid face at line #") + String::ToString(num));
				continue;
			}

			element.faces.Add(face);
		}

		// mtllib (material library)
		else if (tokens[0] == (String)_T("mtllib"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid material library at line #") + String::ToString(num));
				continue;
			}

			if (stream.GetStreamType() == StreamType_File)
			{
				String mtlname = tokens[1];

				mtlFileName = tokens[1];
			}
		}

		else
		{
			if (element.faces.Count() != 0)
			{
				_elements.Add(element);

				element.name = String::Empty;
				element.faces.Clear();
				element.material = String::Empty;
			}

			// g (group name)
			if (tokens[0] == (String)_T("g"))
			{
				if (tokens.Count() >= 2)
					element.name = line.Right(line.Length() - 2);
			}

			// o (object name)
			else if (tokens[0] == (String)_T("o"))
			{
				if (tokens.Count() >= 2)
					element.name = line.Right(line.Length() - 2);
			}

			// usemtl (material name)
			else if (tokens[0] == (String)_T("usemtl"))
			{
				if (tokens.Count() < 2)
				{
					SE_LOGWARNING(_T("Invalid material name at line #") + String::ToString(num));
					continue;
				}

				element.material = line.Right(line.Length() - ((String)_T("usemtl")).Length() - 1);
			}
		}
	}

	if (element.faces.Count() != 0)
	{
		_elements.Add(element);
	}

	if (!mtlFileName.IsEmpty())
	{
		String currentDir = Environment::GetCurrentDirectory();
		String source = ((FileStream*)&stream)->GetFileName();
		Environment::SetCurrentDirectory(Path::GetDirectoryName(source));

		File* file = new File(mtlFileName);
		FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);

		ReadMTL(*stream);

		Environment::SetCurrentDirectory(currentDir);
	}
}

void OBJModelReader::ReadMTL(Stream& stream)
{
	TextStream reader(&stream);

	OBJMaterial material;

	String line;
	int num = 0;
	while (!stream.IsEOF())
	{
		line = reader.ReadLine();
		num++;

		if (line[0] == _T('#') || line[0] == _T('$'))
		{
			SE_LOGNOTICE(line);
			continue;
		}

		if (line.Length() == 0)
			continue;

		Array<String> tokens = line.Split(_T(' '));
		if (tokens.Count() == 0)
		{
			SE_LOGWARNING(_T("Invalid line #") + String::ToString(num));
			continue;
		}

		if (tokens[0] == (String)_T("newmtl"))
		{
			if (tokens.Count() < 2)
			{
				SE_LOGWARNING(_T("Invalid material at line #") + String::ToString(num));
				continue;
			}

			if (!material.name.IsEmpty())
				_materials.Add(material);

			material = OBJMaterial();
			material.name = line.Right(line.Length() - ((String)_T("newmtl")).Length() - 1);
		}

		else if (tokens[0] == (String)_T("Ni"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid intensity value at line #") + String::ToString(num));
				continue;
			}

			material.Ni = tokens[1].ToInt32();
		}

		else if (tokens[0] == (String)_T("Ns"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid shininess value at line #") + String::ToString(num));
				continue;
			}

			material.Ns = tokens[1].ToInt32();
		}

		else if (tokens[0] == (String)_T("illum"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid illumination value at line #") + String::ToString(num));
				continue;
			}

			material.illum = tokens[1].ToInt32();
		}

		else if (tokens[0] == (String)_T("d") || tokens[0] == (String)_T("Tr"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid transparency value at line #") + String::ToString(num));
				continue;
			}

			material.Tr = tokens[1].ToReal32();
		}

		else if (tokens[0] == (String)_T("Ka"))
		{
			if (tokens.Count() != 4)
			{
				SE_LOGWARNING(_T("Invalid ambient color at line #") + String::ToString(num));
				continue;
			}

			material.Ka[0] = tokens[1].ToReal32();
			material.Ka[1] = tokens[2].ToReal32();
			material.Ka[2] = tokens[3].ToReal32();
		}

		else if (tokens[0] == (String)_T("Kd"))
		{
			if (tokens.Count() != 4)
			{
				SE_LOGWARNING(_T("Invalid diffuse color at line #") + String::ToString(num));
				continue;
			}

			material.Kd[0] = tokens[1].ToReal32();
			material.Kd[1] = tokens[2].ToReal32();
			material.Kd[2] = tokens[3].ToReal32();
		}

		else if (tokens[0] == (String)_T("Ks"))
		{
			if (tokens.Count() != 4)
			{
				SE_LOGWARNING(_T("Invalid specular color at line #") + String::ToString(num));
				continue;
			}

			material.Ks[0] = tokens[1].ToReal32();
			material.Ks[1] = tokens[2].ToReal32();
			material.Ks[2] = tokens[3].ToReal32();
		}

		else if (tokens[0] == (String)_T("map_Ka"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid ambient texture at line #") + String::ToString(num));
				continue;
			}

			material.map_Ka = tokens[1];
		}

		else if (tokens[0] == (String)_T("map_Kd"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid diffuse texture at line #") + String::ToString(num));
				continue;
			}

			material.map_Kd = tokens[1];
		}

		else if (tokens[0] == (String)_T("map_Ks"))
		{
			if (tokens.Count() != 2)
			{
				SE_LOGWARNING(_T("Invalid specular texture at line #") + String::ToString(num));
				continue;
			}

			material.map_Ks = tokens[1];
		}
	}

	if (!material.name.IsEmpty())
		_materials.Add(material);
}

}
