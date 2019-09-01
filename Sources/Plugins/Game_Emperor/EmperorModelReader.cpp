/*=============================================================================
EmperorModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "EmperorModelReader.h"

namespace SE_Emperor
{

ShaderMaterial* CreateShader(Texture* texture)
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();
	ShaderPass* pass = new ShaderPass();

	//pass->RasterizerState.FillMode = FillMode_WireFrame;
	pass->LightState.Lighting = true;

	pass->AddSamplerState(new SamplerState());
	TextureState* textureState = new TextureState();
	textureState->SetTexture(texture);
	pass->AddTextureState(textureState);

	technique->AddPass(pass);
	shader->AddTechnique(technique);
	
	return shader;
}

EmperorModelReader::EmperorModelReader() :
	ModelReader()
{
}

EmperorModelReader::~EmperorModelReader()
{
}

Model* EmperorModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	int i;

	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("EmperorModelReader.LoadModel"),
			_T("No active renderer."));
		return NULL;
	}

	int32 version;
	int32 length;

	stream >> version;
	stream >> length;

	// FXDataHeader
	stream.Seek(length, SeekOrigin_Current);

	int32 textures_length;
	SEchar* textures;

	stream >> textures_length;
	textures = new SEchar[textures_length];
	stream.Read((SEbyte*)textures, textures_length);

	SEchar* textures_temp = textures;
	Array<String> texture_names;
	for (i=0; i<textures_length; ++i)
	{
		if (textures[i] == '\0')
		{
			texture_names.Add(textures_temp);
			textures_temp = textures + i + 2;
			i++;
		}
	}

	// Objects
	BaseArray<EmperorObject> roots;
	while (!stream.IsEOF())
	{
		EmperorObject root;
		if (!ReadObject(stream, root))
			break;

		roots.Add(root);
	}

	for (i=0; i<texture_names.Count(); ++i)
	{
		Texture* texture = NULL;
		Resource* resource = ResourceHelper::LoadFromFile(texture_names[i], SE_ID_DATA_IMAGE);
		if (resource != NULL)
		{
			Image* image = (Image*)resource->GetData();
			if (!renderer->CreateTexture(image, &texture))
			{
				Logger::Current()->Log(LogLevel::Error, _T("EmperorModelReader.LoadModel"),
					_T("Failed to create the texture: ") + texture_names[i]);
			}
		}

		ShaderMaterial* shader = CreateShader(texture);
		_shaders.Add(shader);
	}

	_Model = new Model();

	for (i=0; i<roots.Count(); ++i)
	{
		MeshFrame* frame = new MeshFrame();

		ConvertObject(frame, roots[i]);

		_Model->AddMeshFrame(frame);
	}

	_Model->UpdateBoundingBox();

	return _Model;
}

bool EmperorModelReader::ReadObject(Stream& stream, EmperorObject& obj)
{
	int i;

	stream >> obj.vertex_count;
	if (obj.vertex_count == -1)
		return false;

	stream >> obj.object_flags;
	stream >> obj.face_count;
	stream >> obj.child_count;

	stream.Read((SEbyte*)obj.matrix, sizeof(real64) * 16);

	stream >> obj.name_length;
	obj.name = NULL;
	if (obj.name_length > 0)
	{
		obj.name = new SEchar[obj.name_length+1];
		stream.Read((SEbyte*)obj.name, obj.name_length);
		obj.name[obj.name_length] = '\0';
	}

	obj.children.SetSize(obj.child_count);
	for (i=0; i<obj.child_count; ++i)
	{
		ReadObject(stream, obj.children[i]);
	}

	// Mesh
	obj.vertices.SetSize(obj.vertex_count);
	obj.normals.SetSize(obj.vertex_count);

	for (i=0; i<obj.vertex_count; ++i)
	{
		stream >> obj.vertices[i].X;
		stream >> obj.vertices[i].Y;
		stream >> obj.vertices[i].Z;

		stream >> obj.normals[i].X;
		stream >> obj.normals[i].Y;
		stream >> obj.normals[i].Z;
	}

	// Faces
	obj.indices.SetSize(obj.face_count*3);
	obj.face_coordinates.SetSize(obj.face_count*3);

	for (i=0; i<obj.face_count; ++i)
	{
		stream >> obj.indices[3*i];
		stream >> obj.indices[3*i+1];
		stream >> obj.indices[3*i+2];

		stream >> obj.material_index;
		stream >> obj.unknown; //0x00001000

		stream >> obj.face_coordinates[3*i].X;
		stream >> obj.face_coordinates[3*i].Y;
		stream >> obj.face_coordinates[3*i+1].X;
		stream >> obj.face_coordinates[3*i+1].Y;
		stream >> obj.face_coordinates[3*i+2].X;
		stream >> obj.face_coordinates[3*i+2].Y;
	}

	// Animation
	if (obj.object_flags & 4)
	{
		int32 unknown1;
		int32 unknown2;
		int32 unknown3;

		stream >> unknown1;
		stream >> unknown2;
		stream >> unknown3;

		stream.Seek(sizeof(int32)*unknown3, SeekOrigin_Current);

		if (unknown2 < 0)
		{
			int32 unknown21;
			int32 unknown22;

			stream >> unknown21;
			stream >> unknown22;

			stream.Seek(sizeof(int16)*unknown22*4, SeekOrigin_Current);
			stream.Seek(sizeof(int32)*unknown1, SeekOrigin_Current);
		}
	}

	if (obj.object_flags & 8)
	{
		int32 frame_count;
		int32 frame_flag;

		stream >> frame_count;
		stream >> frame_flag;

		real32* matrices;

		frame_count++;

		if (frame_flag == -1)
		{
			matrices = new real32[frame_count*16];
			stream.Read((SEbyte*)matrices, frame_count*sizeof(real32)*12);
		}
		else if (frame_flag == -2)
		{
			matrices = new real32[frame_count*sizeof(real32)*12];
			stream.Read((SEbyte*)matrices, frame_count*sizeof(real32)*12);
		}
		else if (frame_flag == -3)
		{
			int16* frame_indices;
			int32 matrix_count;

			stream >> matrix_count;

			frame_indices = new int16[frame_count];
			stream.Read((SEbyte*)frame_indices, frame_count*sizeof(int16));

			matrices = new real32[matrix_count*12];
			stream.Read((SEbyte*)matrices, matrix_count*sizeof(real32)*12);
		}
	}

	return true;
}

void EmperorModelReader::ConvertObject(MeshFrame* parent, const EmperorObject& obj)
{
	int i;
	MeshFrame* frame = new MeshFrame();

	if (obj.name != NULL)
		frame->SetName(obj.name);

/*
	Matrix4 transform;
	for (i=0; i<16; ++i)
		transform.v[i] = (real)obj.matrix[i];

	transform.Transpose();

	Quaternion q = Quaternion::FromRotationMatrix(transform.ToMatrix3());
	frame->SetTransform(Transform(transform.GetTranslation(), Vector3::One, q));
*/

	if (obj.vertex_count > 0 && obj.face_count > 0)
	{
		Geometry* geometry = new Geometry();
		geometry->SetVertices(obj.vertices);
		geometry->SetNormals(obj.normals);
		geometry->SetIndices(obj.indices);

		Array<Vector2> texcoords(obj.vertex_count);
		for (i=0; i<obj.face_count*3; ++i)
		{
			texcoords[obj.indices[i]] = obj.face_coordinates[i];
		}
		geometry->SetTexCoords(texcoords);

		PrimitiveStream* primitive;
		if (!geometry->CreatePrimitive(&primitive))
		{
			Logger::Current()->Log(LogLevel::Error, _T("EmperorModelReader.LoadModel"),
				_T("Failed to create the primitive for the object: ") + String(obj.name));
		}
		else
		{
			Mesh* mesh = new Mesh();
			mesh->SetPrimitiveStream(primitive);
			geometry->UpdateBoundingBox();
			mesh->SetBoundingBox(geometry->GetBoundingBox());

			if (obj.material_index == 0xffffffff)
			{
				if (_shaders.Count() == 1)
					mesh->SetShader(_shaders[0]);
			}
			else if (obj.material_index < _shaders.Count())
			{
				mesh->SetShader(_shaders[obj.material_index]);
			}

			frame->AddMeshLOD(0.0, mesh);
			frame->UpdateBoundingBox();
		}
	}

	for (i=0; i<obj.child_count; ++i)
	{
		ConvertObject(frame, obj.children[i]);
	}

	parent->AddChild(frame);
}

}
