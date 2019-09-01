/*=============================================================================
OBJModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "OBJModelReader.h"
#include "Mesh.h"
#include "TriangleShape.h"
#include "PhongShader.h"
//#include "Texture.h"

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("OBJModelReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("OBJModelReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("OBJModelReader"), ##message##);

namespace Raytracer
{

OBJModelReader::OBJModelReader()
{
}

OBJModelReader::~OBJModelReader()
{
}

void OBJModelReader::LoadModel(RTScene* scene, const String& source)
{
	File* file = new File(source);
	FileStreamPtr stream = file->Open(FileMode_Open);
	if (stream == NULL)
		return;

	ReadOBJ(stream);

	if (_Elements.Count() == 0)
		return;

	Dictionary<String, RTShader*> shaders;

	SimpleArray<OBJMaterial>::Iterator itMat = _Materials.GetIterator();
	while (itMat.Next())
	{
		const OBJMaterial& material = itMat.Current();

		PhongShader* shader = new PhongShader();
		shader->SetAmbient(Colour32(material.Ka[0], material.Ka[1], material.Ka[2]));
		shader->SetDiffuse(Colour32(material.Kd[0], material.Kd[1], material.Kd[2]));
		shader->SetSpecular(Colour32(material.Ks[0], material.Ks[1], material.Ks[2]));

		/*if (!material.map_Kd.IsEmpty())
		{
			String path = Path::Combine(Path::GetDirectoryName(source), material.map_Kd);

			Resource* resource = ResourceHelper::LoadFromFile(
				path, SE_ID_DATA_IMAGE);
			if (resource != NULL)
			{
				Texture* texture;
				if (renderer->CreateTexture((Image*)resource->GetData(), &texture))
				{
					pass->AddSamplerState(new SamplerState());
					TextureState* textureState = new TextureState();
					textureState->SetTexture(texture);
					pass->AddTextureState(textureState);
				}
			}
		}*/
		shaders.Add(material.name, shader);
	}

	SimpleArray<OBJElement>::Iterator it = _Elements.GetIterator();
	while (it.Next())
	{
		const OBJElement& element = it.Current();

		bool hasNormals = (element.normals.Count() != 0);
		bool hasTexCoords = (element.texcoords.Count() != 0);

		if (element.vertices.Count() == 0)
		{
			SE_LOGERROR(_T("No vertices found."));
			continue;
		}

		/*if (hasNormals)
		{
			if (element.vertices.Count() < element.normals.Count())
			{
				SE_LOGWARNING(_T("The number of vertices is lower than the number of normals."));
			}
			else if (element.vertices.Count() != element.normals.Count())
			{
				SE_LOGERROR(_T("The number of vertices is different than the number of normals."));
				continue;
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
				continue;
			}
		}*/

#if 0
		RTMesh* mesh = new RTMesh();

		mesh->SetVertices(element.vertices);

		RTMesh::IndexArray indices;
		int faceCount = element.faces.Count();
		indices.SetSize(faceCount*3);
		for (int f = 0; f < faceCount; ++f)
		{
			for (int v = 0; v < 3; ++v)
			{
				indices[f*3+v] = element.faces[f].vertex[v].position-1;
			}
		}
		mesh->SetIndices(indices);

		if (hasNormals)
		{
			mesh->SetNormals(element.normals);
		}
		else
		{
			mesh->GenerateNormals();
		}

		if (hasTexCoords)
		{
			mesh->SetTexCoords(element.texcoords);
		}

		mesh->SetShader(shaders[element.material]);
		scene->Objects().Add(mesh);
#else
		int faceCount = element.faces.Count();
		for (int f = 0; f < faceCount; ++f)
		{
			RTTriangleShape* triangle = new RTTriangleShape();
			triangle->SetVertices(
				element.vertices[element.faces[f].vertex[0].position-1],
				element.vertices[element.faces[f].vertex[1].position-1],
				element.vertices[element.faces[f].vertex[2].position-1]);
			triangle->SetShader(shaders[element.material]);
			scene->Objects().Add(triangle);
		}
#endif
	}
}

void OBJModelReader::ReadOBJ(Stream* stream)
{
	OBJElement element;
	OBJFace face;
	String mtlFileName;

	TextStream reader(stream);

	String line;
	int num = 0;
	while (!stream->IsEOF())
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

			String mtlname = tokens[1];
			mtlFileName = tokens[1];
		}

		else
		{
			if (element.faces.Count() != 0)
			{
				_Elements.Add(element);

				element.name = String::Empty;
				element.faces.Clear();
				//element.material = String::Empty;
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
		_Elements.Add(element);
	}

	if (!mtlFileName.IsEmpty())
	{
		String currentDir = Environment::GetCurrentDirectory();
		String source = ((FileStream*)&stream)->GetFileName();
		Environment::SetCurrentDirectory(Path::GetDirectoryName(source));

		File* file = new File(mtlFileName);
		FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);

		ReadMTL(stream);

		Environment::SetCurrentDirectory(currentDir);
	}
}

void OBJModelReader::ReadMTL(Stream* stream)
{
	TextStream reader(stream);

	OBJMaterial material;

	String line;
	int num = 0;
	while (!stream->IsEOF())
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
				_Materials.Add(material);

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
				SE_LOGWARNING(_T("Invalid ambient colour at line #") + String::ToString(num));
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
				SE_LOGWARNING(_T("Invalid diffuse colour at line #") + String::ToString(num));
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
				SE_LOGWARNING(_T("Invalid specular colour at line #") + String::ToString(num));
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
		_Materials.Add(material);
}

}
