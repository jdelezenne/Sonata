/*=============================================================================
M3DSModelReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "3DSModelReader.h"

#define SE_LOGNOTICE(message) Logger::Current()->Log(LogLevel::Notice, _T("3DSModelReader"), ##message##);
#define SE_LOGWARNING(message) Logger::Current()->Log(LogLevel::Warning, _T("3DSModelReader"), ##message##);
#define SE_LOGERROR(message) Logger::Current()->Log(LogLevel::Error, _T("3DSModelReader"), ##message##);

namespace SE_3DS
{

Vector3 Swap3DS(const Vector3& v)
{
	return Vector3(v.X, v.Y, v.Z);
}

M3DSModelReader::M3DSModelReader() :
	ModelReader()
{
}

M3DSModelReader::~M3DSModelReader()
{
}

Model* M3DSModelReader::LoadModel(Stream& stream, ModelReaderOptions* options)
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		SE_LOGERROR(_T("No active renderer."));
		return NULL;
	}

	_stream = &stream;
	_fileName = ((FileStream*)_stream)->GetFileName();

	if (!ReadModel())
	{
		return NULL;
	}

	BuildMaterials(_materials);

	return BuildModel();
}

bool M3DSModelReader::ReadModel()
{
	Mesh3DSChunk parent;
	ReadChunk(parent);
	if (parent.id != M3DS_MAIN)
		return false;

	_scene.edit = NULL;
	_scene.keyf = NULL;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_MAIN_VERSION:
			*_stream >> _scene.version;
			break;
		case M3DS_EDITOR:
			_scene.edit = new Mesh3DSEditor();
			ReadEditorChunk(chunk, *_scene.edit);
			break;
		case M3DS_KFDATA:
			_scene.keyf = new Mesh3DSKeyframer();
			ReadKeyframerChunk(chunk, *_scene.keyf);
			break;
		default:
			SkipChunk(chunk);
		}
	}

	return true;
}

bool M3DSModelReader::ReadChunk(Mesh3DSChunk& chunk)
{
	*_stream >> chunk.id;
	*_stream >> chunk.size;

	return chunk.size >= 6 && !_stream->IsEOF();
}

bool M3DSModelReader::ReadNextChunk(const Mesh3DSChunk& parent, Mesh3DSChunk& chunk)
{
	chunk.offset = _stream->GetPosition();

	if (chunk.offset >= parent.offset + parent.size)
		return false;

	*_stream >> chunk.id;
	*_stream >> chunk.size;

	return chunk.size >= 6 && !_stream->IsEOF();
}

void M3DSModelReader::SkipChunk(const Mesh3DSChunk& chunk)
{
#ifdef _DEBUG
#ifdef _WIN32
	String str = String::Format("SkipChunk(): 0x%00X\n", chunk.id);
	SE_LOGNOTICE(str);
#else
	printf("SkipChunk(): 0x%00X\n", chunk.id);
#endif
#endif
	_stream->Seek(chunk.size-6, SeekOrigin_Current);
}

String M3DSModelReader::ReadString()
{
	String str;
	uint8 buffer[64];
	for (int i = 0; i < 64; i++)
	{
		*_stream >> buffer[i];
		if (buffer[i] == '\0')
			return String((char*)buffer);
	}

	return "";
}

real32 M3DSModelReader::ReadPercentageChunk()
{
	real32 percent;
	int16 temp;

	Mesh3DSChunk chunk;
	ReadChunk(chunk);
	switch (chunk.id)
	{
	case M3DS_INT_PERCENTAGE:
		*_stream >> temp;
		percent = (real32)temp / 100.0f;
		break;
	case M3DS_FLOAT_PERCENTAGE:
		*_stream >> percent;
		break;
	}

	return percent;
}

Color32 M3DSModelReader::ReadColorFloat()
{
	real32 r, g, b;
	*_stream >> r;
	*_stream >> g;
	*_stream >> b;
	return Color32(r, g, b, 1.0f);
}

Color32 M3DSModelReader::ReadColorInt()
{
	uint8 r, g, b;
	*_stream >> r;
	*_stream >> g;
	*_stream >> b;
	return Color32(r/255.0f, g/255.0f, b/255.0f, 1.0f);
}

Color32 M3DSModelReader::ReadColorChunk()
{
	Color32 color;
    bool bHaveGamma = false;

	Mesh3DSChunk chunk;
	ReadChunk(chunk);
	switch (chunk.id)
	{
    case M3DS_COLOR_F:
        color = ReadColorFloat();
        break;
    case M3DS_COLOR_24:
        if (bHaveGamma)
            ReadColorInt();
        else
            color = ReadColorInt();
        break;
    case M3DS_LIN_COLOR_24:
        color = ReadColorInt();
        bHaveGamma = true;
        break;
    case M3DS_LIN_COLOR_F:
        color = ReadColorFloat();
        bHaveGamma = true;
        break;
    }

    return color;
}

Vector2 M3DSModelReader::ReadVector2()
{
	Vector2 v;
	*_stream >> v.X;
	*_stream >> v.Y;

	return v;
}

Vector3 M3DSModelReader::ReadVector3()
{
	Vector3 v;
	*_stream >> v.X;
	*_stream >> v.Y;
	*_stream >> v.Z;

	return v;
}

void M3DSModelReader::ReadEditorChunk(const Mesh3DSChunk& parent, Mesh3DSEditor& edit)
{
	Mesh3DSMaterial* material;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_MESH_VERSION:
			*_stream >> edit.version;
			break;
		case M3DS_MASTER_SCALE:
			*_stream >> edit.master_scale;
			break;
		case M3DS_AMBIENT_LIGHT:
			edit.ambient_light = ReadColorChunk();
			break;
		case M3DS_MAT_ENTRY:
			material = new Mesh3DSMaterial();
			ReadMaterialChunk(chunk, *material);
			edit.materials.Add(material);
			break;
		case M3DS_NAMED_OBJECT:
			ReadObjectChunk(chunk, edit);
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadMaterialChunk(const Mesh3DSChunk& parent, Mesh3DSMaterial& material)
{
	material.use_falloff = false;
	material.use_blur = false;
	material.self_illum = 0.0f;
	material.two_sided = false;
	material.map_decal = false;
	material.additive = false;
	material.use_wire = false;
	material.face_map = false;
	material.use_falloff_in = false;
	material.soften = false;
	material.use_wire_abs = false;
	material.wire_size = 0.0f;
	material.texture1_map = NULL;
	material.texture2_map = NULL;
	material.opacity_map = NULL;
	material.bump_map = NULL;
	material.specular_map = NULL;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_MAT_NAME:
			material.name = ReadString();
			break;
		case M3DS_MAT_AMBIENT:
			material.ambient = ReadColorChunk();
			break;
		case M3DS_MAT_DIFFUSE:
			material.diffuse = ReadColorChunk();
			break;
		case M3DS_MAT_SPECULAR:
			material.specular = ReadColorChunk();
			break;
		case M3DS_MAT_SHININESS:
			material.shininess = ReadPercentageChunk();
			break;
		case M3DS_MAT_SHIN2PCT:
			material.shin_strength = ReadPercentageChunk();
			break;
		case M3DS_MAT_TRANSPARENCY:
			material.transparency = ReadPercentageChunk();
			break;
		case M3DS_MAT_XPFALL:
			material.falloff = ReadPercentageChunk();
			break;
		case M3DS_MAT_USE_XPFALL:
			material.use_falloff = true;
			break;
		case M3DS_MAT_REFBLUR:
			material.blur = ReadPercentageChunk();
			break;
		case M3DS_MAT_SHADING:
			*_stream >> material.shading;
			break;
		case M3DS_MAT_USE_REFBLUR:
			material.use_blur = true;
			break;
		case M3DS_MAT_USE_SELF_ILLUM:
			material.self_illum = true;
			break;
		case M3DS_MAT_TWO_SIDE:
			material.two_sided = true;
			break;
		case M3DS_MAT_DECAL:
			material.map_decal = true;
			break;
		case M3DS_MAT_ADDITIVE:
			material.additive = true;
			break;
		case M3DS_MAT_SELF_ILLUM:
			material.self_illum = ReadPercentageChunk();
			break;
		case M3DS_MAT_WIRE:
			material.use_wire = true;
			break;
		case M3DS_MAT_FACEMAP:
			material.face_map = true;
			break;
		case M3DS_MAT_FALL_OFF:
			material.use_falloff_in = true;
			break;
		case M3DS_MAT_PHONGSOFT:
			material.soften = true;
			break;
		case M3DS_MAT_WIREABS:
			material.use_wire_abs = true;
			break;
		case M3DS_MAT_WIRE_SIZE:
			*_stream >> material.wire_size;
			break;
		case M3DS_MAT_TEXMAP:
			material.texture1_map = new Mesh3DSTexture();
			ReadTextureChunk(chunk, *material.texture1_map);
			break;
		case M3DS_MAT_TEX2MAP:
			material.texture2_map = new Mesh3DSTexture();
			ReadTextureChunk(chunk, *material.texture2_map);
			break;
		case M3DS_MAT_OPACMAP:
			material.opacity_map = new Mesh3DSTexture();
			ReadTextureChunk(chunk, *material.opacity_map);
			break;
		case M3DS_MAT_BUMPMAP:
			material.bump_map = new Mesh3DSTexture();
			ReadTextureChunk(chunk, *material.bump_map);
			break;
		case M3DS_MAT_SPECMAP:
			material.specular_map = new Mesh3DSTexture();
			ReadTextureChunk(chunk, *material.specular_map);
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadTextureChunk(const Mesh3DSChunk& parent, Mesh3DSTexture& texture)
{
	texture.flags = 0;
	texture.percent = 0;
	texture.blur = 0.0f;
	texture.scale[0] = 1.0f;
	texture.scale[1] = 1.0f;
	texture.offset[0] = 0.0f;
	texture.offset[1] = 0.0f;
	texture.rotation = 0.0f;
	int16 temp;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_INT_PERCENTAGE:
			*_stream >> temp;
			texture.percent = (real32)temp / 100.0f;
			break;
		case M3DS_MAT_MAPNAME:
			texture.name = ReadString();
			break;
		case M3DS_MAT_MAP_TILING:
			*_stream >> texture.flags;
			break;
		case M3DS_MAT_MAP_TEXBLUR:
			*_stream >> texture.blur;
			break;
		case M3DS_MAT_MAP_USCALE:
			*_stream >> texture.scale[0];
			break;
		case M3DS_MAT_MAP_VSCALE:
			*_stream >> texture.scale[1];
			break;
		case M3DS_MAT_MAP_UOFFSET:
			*_stream >> texture.offset[0];
			break;
		case M3DS_MAT_MAP_VOFFSET:
			*_stream >> texture.offset[1];
			break;
		case M3DS_MAT_MAP_ANG:
			*_stream >> texture.rotation;
			break;
		case M3DS_MAT_MAP_COL1:
			texture.tint_1 = ReadColorInt();
			break;
		case M3DS_MAT_MAP_COL2:
			texture.tint_2 = ReadColorInt();
			break;
		case M3DS_MAT_MAP_RCOL:
			texture.tint_r = ReadColorInt();
			break;
		case M3DS_MAT_MAP_GCOL:
			texture.tint_g = ReadColorInt();
			break;
		case M3DS_MAT_MAP_BCOL:
			texture.tint_b = ReadColorInt();
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadObjectChunk(const Mesh3DSChunk& parent, Mesh3DSEditor& edit)
{
	Mesh3DSMesh* mesh;
	Mesh3DSCamera* camera;
	Mesh3DSLight* light;

	String name = ReadString();

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	ReadNextChunk(parent, chunk);
	switch (chunk.id)
	{
	case M3DS_N_TRI_OBJECT:
		mesh = new Mesh3DSMesh();
		mesh->name = name;
		ReadMeshChunk(chunk, *mesh);
		edit.meshes.Add(mesh);
		break;
	case M3DS_N_CAMERA:
		camera = new Mesh3DSCamera();
		camera->name = name;
		ReadCameraChunk(chunk, *camera);
		edit.cameras.Add(camera);
		break;
	case M3DS_N_DIRECT_LIGHT:
		light = new Mesh3DSLight();
		light->name = name;
		ReadLightChunk(chunk, *light);
		edit.lights.Add(light);
		break;
	default:
		SkipChunk(chunk);
	}
}

void M3DSModelReader::ReadMeshChunk(const Mesh3DSChunk& parent, Mesh3DSMesh& mesh)
{
	int i, j;

	mesh.scale_rot = Matrix3::Identity;
	mesh.position = Vector3::Zero;
	mesh.vertex_count = 0;
	mesh.vertices = NULL;
	mesh.flag_count = 0;
	mesh.flags = NULL;
	mesh.texel_count = 0;
	mesh.texels = NULL;
	mesh.face_count = 0;
	mesh.faces = NULL;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_MESH_MATRIX:
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					*_stream >> mesh.scale_rot.M[j][i];
				}
			}

			mesh.position = ReadVector3();
			break;
		case M3DS_MESH_COLOR:
			*_stream >> mesh.color;
			break;
		case M3DS_POINT_ARRAY:
			*_stream >> mesh.vertex_count;
			if (mesh.vertex_count > 0)
			{
				mesh.vertices = new Vector3[mesh.vertex_count];
				for (i = 0; i < mesh.vertex_count; ++i)
				{
					mesh.vertices[i] = ReadVector3();
				}
			}
			break;
		case M3DS_POINT_FLAG_ARRAY:
			*_stream >> mesh.flag_count;
			if (mesh.flag_count > 0)
			{
				mesh.flags = new uint16[mesh.flag_count];
				for (i = 0; i < mesh.flag_count; ++i)
				{
					*_stream >> mesh.flags[i];
				}
			}
			break;
		case M3DS_FACE_ARRAY:
			ReadMeshFaceChunk(chunk, mesh);
			break;
		//case M3DS_MESH_TEXTURE_INFO:
		//	break;
		case M3DS_TEX_VERTS:
			*_stream >> mesh.texel_count;
			if (mesh.texel_count > 0)
			{
				mesh.texels = new Vector2[mesh.texel_count];
				for (i = 0; i < mesh.texel_count; ++i)
				{
					mesh.texels[i] = ReadVector2();
				}
			}
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadMeshFaceChunk(const Mesh3DSChunk& parent, Mesh3DSMesh& mesh)
{
	*_stream >> mesh.face_count;
	if (mesh.face_count > 0)
	{
		mesh.faces = new Mesh3DSMeshFace[mesh.face_count];
		for (int i = 0; i < mesh.face_count; ++i)
		{
			*_stream >> mesh.faces[i].indices[0];
			*_stream >> mesh.faces[i].indices[1];
			*_stream >> mesh.faces[i].indices[2];
			*_stream >> mesh.faces[i].flag;
		}
	}

	String name;
	int16 faces;
	int16 index;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_MSH_MAT_GROUP:
			name = ReadString();
			*_stream >> faces;

			for (int i = 0; i < faces; i++)
			{
				*_stream >> index;
				if (index >= 0 && index < mesh.face_count)
					mesh.faces[index].material_name = name;
			}
			break;
		case M3DS_SMOOTH_GROUP:
			for (int i = 0; i < mesh.face_count; i++)
			{
				*_stream >> mesh.faces[i].smoothing;
			}
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadCameraChunk(const Mesh3DSChunk& parent, Mesh3DSCamera& camera)
{
	camera.see_cone = false;
	camera.near_range = 0.0f;
	camera.far_range = 0.0f;

	camera.position = ReadVector3();
	camera.target = ReadVector3();
	*_stream >> camera.roll;

	real32 fov;
	*_stream >> fov;
	camera.fov = 2400.0f / fov;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_CAM_SEE_CONE:
			camera.see_cone = true;
			break;
		case M3DS_CAM_RANGES:
			*_stream >> camera.near_range;
			*_stream >> camera.far_range;
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadLightChunk(const Mesh3DSChunk& parent, Mesh3DSLight& light)
{
	light.off = false;
	light.outer_range = 0.0f;
	light.inner_range = 0.0f;
	light.multiplier = 0.0f;
	light.attenuation = 0.0f;
	light.spot_light = false;
	light.roll = 0.0f;
	light.shadowed = false;
	light.shadow_bias = 0.0f;
	light.shadow_filter = 0.0f;
	light.shadow_size = 0;
	light.see_cone = false;
	light.rectangular_spot = false;
	light.spot_aspect = 0.0f;
	light.use_projector = false;
	light.spot_overshoot = false;
	light.ray_bias = 0.0f;
	light.ray_shadows = false;

	light.position = ReadVector3();

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_COLOR_F:
			light.color = ReadColorFloat();
			break;
		case M3DS_DL_OFF:
			light.off = true;
			break;
		case M3DS_DL_OUTER_RANGE:
			*_stream >> light.outer_range;
			break;
		case M3DS_DL_INNER_RANGE:
			*_stream >> light.inner_range;
			break;
		case M3DS_DL_MULTIPLIER:
			*_stream >> light.multiplier;
			break;
		//case M3DS_DL_EXCLUDE:
		//	break;
		case M3DS_DL_ATTENUATE:
			//*_stream >> light.attenuation;
			break;
		case M3DS_DL_SPOTLIGHT:
			ReadSpotlightChunk(chunk, light);
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadSpotlightChunk(const Mesh3DSChunk& parent, Mesh3DSLight& light)
{
	light.spot_light = true;
	light.spot = ReadVector3();
	*_stream >> light.hot_spot;
	*_stream >> light.fall_off;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_DL_SPOT_ROLL:
			*_stream >> light.roll;
			break;
		case M3DS_DL_SHADOWED:
			light.shadowed = true;
			break;
		case M3DS_DL_LOCAL_SHADOW2:
			*_stream >> light.shadow_bias;
			*_stream >> light.shadow_filter;
			*_stream >> light.shadow_size;
			break;
		case M3DS_DL_SEE_CONE:
			light.see_cone = true;
			break;
		case M3DS_DL_SPOT_RECTANGULAR:
			light.rectangular_spot = true;
			break;
		case M3DS_DL_SPOT_ASPECT:
			*_stream >> light.spot_aspect;
			break;
		case M3DS_DL_SPOT_PROJECTOR:
			light.use_projector = true;
			light.projector = ReadString();
			break;
		case M3DS_DL_SPOT_OVERSHOOT:
			light.spot_overshoot = true;
			break;
		case M3DS_DL_RAY_BIAS:
			*_stream >> light.ray_bias;
			break;
		case M3DS_DL_RAYSHAD:
			light.ray_shadows = true;
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadKeyframerChunk(const Mesh3DSChunk& parent, Mesh3DSKeyframer& keyf)
{
	Mesh3DSNode* node;

	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_KFHDR:
			*_stream >> keyf.keyf_revision;
			keyf.name = ReadString();
			*_stream >> keyf.frames;
			break;
		case M3DS_KFSEG:
			*_stream >> keyf.segment_from;
			*_stream >> keyf.segment_to;
			break;
		case M3DS_KFCURTIME:
			*_stream >> keyf.current_frame;
			break;
		//case M3DS_VIEWPORT_LAYOUT:
		//case M3DS_DEFAULT_VIEW:
		//	break;
		case M3DS_AMBIENT_NODE_TAG:
			node = new Mesh3DSNode();
			ReadNodeChunk(chunk, *node);
			keyf.nodes.Add(node);
			break;
		case M3DS_OBJECT_NODE_TAG:
			node = new Mesh3DSNode();
			ReadNodeChunk(chunk, *node);
			if (node->name == String("$$$DUMMY"))
				delete node;
			else
				keyf.nodes.Add(node);
			break;
		case M3DS_CAMERA_NODE_TAG:
			break;
		case M3DS_TARGET_NODE_TAG:
			break;
		case M3DS_LIGHT_NODE_TAG:
			break;
		case M3DS_SPOTLIGHT_NODE_TAG:
			break;
		case M3DS_L_TARGET_NODE_TAG:
			break;
		default:
			SkipChunk(chunk);
		}
	}
}

void M3DSModelReader::ReadNodeChunk(const Mesh3DSChunk& parent, Mesh3DSNode& node)
{
	Mesh3DSChunk chunk;
	chunk.offset = _stream->GetPosition();
	while (ReadNextChunk(parent, chunk))
	{
		switch (chunk.id)
		{
		case M3DS_NODE_ID:
			*_stream >> node.node_id;
			break;
		case M3DS_NODE_HDR:
			node.name = ReadString();
			*_stream >> node.flags1;
			*_stream >> node.flags2;
			*_stream >> node.parent_id;
			break;
		case M3DS_PIVOT:
			node.data.pivot = ReadVector3();
			break;
		case M3DS_INSTANCE_NAME:
			node.data.instance = ReadString();
			break;
		case M3DS_BOUNDBOX:
			node.data.bbox_min = ReadVector3();
			node.data.bbox_max = ReadVector3();
			break;
		/*case M3DS_POS_TRACK_TAG:
			node.data.pos = ReadVector3();
			break;
		case M3DS_ROT_TRACK_TAG:
			*_stream >> node.data.rot;
			break;
		case M3DS_SCL_TRACK_TAG:
			*_stream >> node.data.roll;
			break;
		case M3DS_FOV_TRACK_TAG:
			*_stream >> node.data.fov;
			break;
		case M3DS_ROLL_TRACK_TAG:
			*_stream >> node.data.roll;
			break;
		case M3DS_COL_TRACK_TAG:
			node.data.col = ReadColorChunk();
			break;
		case M3DS_MORPH_TRACK_TAG:
			break;
		case M3DS_HOT_TRACK_TAG:
			*_stream >> node.data.hotspot;
			break;
		case M3DS_FALL_TRACK_TAG:
			*_stream >> node.data.falloff;
			break;
		case M3DS_HIDE_TRACK_TAG:
			node.data.hide = true;
			break;*/
		default:
			SkipChunk(chunk);
		}
	}
}

Model* M3DSModelReader::BuildModel()
{
	if (_fileName.IsEmpty())
		return NULL;

	Model* model = new Model();
	Skeleton* skeleton = new Skeleton();
	model->SetSkeleton(skeleton);

	Bone* rootBone = new Bone(skeleton);
	skeleton->AddBone(rootBone);
	skeleton->SetRootBone(rootBone);

	if (!BuildFrame(model, rootBone))
	{
		return NULL;
	}

	int lightCount = _scene.edit->lights.Count();
	for (int lightIdx = 0; lightIdx < lightCount; lightIdx++)
	{
		Mesh3DSLight* srcLight = _scene.edit->lights[lightIdx];

		Light* light = NULL;
		if (srcLight->spot_light)
		{
			continue;
			light = new SpotLight();

			((SpotLight*)light)->SetPosition(Swap3DS(srcLight->position));
			Vector3 direction = Swap3DS(srcLight->spot - srcLight->position);
			((SpotLight*)light)->SetDirection(Vector3::Normalize(direction));
			((SpotLight*)light)->SetFalloffExponent(srcLight->fall_off);
		}
		else
		{
			light = new PointLight();

			((PointLight*)light)->SetPosition(Swap3DS(srcLight->position));
		}

		light->SetName(srcLight->name);
		light->SetEnabled(!srcLight->off);
		light->SetDiffuseColor(srcLight->color);
		//light->SetSpecularColor(srcLight->color);
		//light->SetConstantAttenuation(srcLight->attenuation);
		//light->SetLinearAttenuation(0.3f);

		//(*scene)->Lights.Add(light);
	}

	/*SceneNode* node = (*scene)->GetRoot();
	int count = node->GetChildCount();
	for (int i = 0; i < count; i++)
	{
		MeshNode* meshNode = dynamic_cast<MeshNode*>(node->GetChild(i));
		Mesh* mesh = meshNode->GetMesh();
		ShaderPass* pass = mesh->GetMaterial()->GetPassByIndex(0);
		for (int l=0; l<(*scene)->Lights.Count(); l++)
			pass->LightState.Lights.Add((*scene)->Lights[l]);
	}*/

	return model;
}

void M3DSModelReader::BuildMaterials(Array<ShaderMaterial*>& materials)
{
	RenderSystem* renderer = RenderSystem::Current();

	int materialCount = _scene.edit->materials.Count();
	for (int mi = 0; mi < materialCount; mi++)
	{
		Mesh3DSMaterial* srcMaterial = _scene.edit->materials[mi];

		ShaderMaterial* material = new Shader();
		material->SetName(srcMaterial->name);

		ShaderTechnique* technique = new ShaderTechnique();
		ShaderPass* pass = new ShaderPass();
		pass->MaterialState.AmbientColor = srcMaterial->ambient;
		pass->MaterialState.DiffuseColor = srcMaterial->diffuse;
		//pass->MaterialState.SpecularColor = srcMaterial->specular;
		//pass->MaterialState.Shininess = srcMaterial->shininess * 100.0f;
		pass->LightState.Lighting = true;

		if (srcMaterial->transparency > 0.0f)
		{
			pass->MaterialState.DiffuseColor.a = srcMaterial->transparency;
			pass->AlphaState.BlendEnable[0] = true;
			pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
			pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
		}

		if (srcMaterial->texture1_map != NULL)
		{
			String path = Path::Combine(path, srcMaterial->texture1_map->name);
			Resource* resource = ResourceHelper::LoadFromFile(
				path, SE_ID_DATA_IMAGE);
			if (resource != NULL)
			{
				Texture* texture;
				if (renderer->CreateTexture((Image*)resource->GetData(), TextureUsage_Static, &texture))
				{
					SamplerState* samplerState = new SamplerState();
					samplerState->SetTexture(texture);
					pass->AddSamplerState(samplerState);

					TextureState* textureState = new TextureState();
					textureState->Offset = Vector2(
						srcMaterial->texture1_map->offset[0],
						srcMaterial->texture1_map->offset[1]);
					textureState->Tile = Vector2(
						srcMaterial->texture1_map->scale[0],
						srcMaterial->texture1_map->scale[1]);
					pass->AddTextureState(textureState);

					if (srcMaterial->opacity_map != NULL)
					{
						pass->AlphaState.TestEnable = true;
						pass->AlphaState.Function = ComparisonFunction_Greater;
					}
				}
			}
		}

		technique->AddPass(pass);
		material->AddTechnique(technique);
		materials.Add(material);
	}
}

bool M3DSModelReader::BuildFrame(Model* model, Bone* bone)
{
	if (_scene.edit == NULL || _scene.keyf == NULL)
	{
		return false;
	}

	String path;
	String fileName;
	int index = _fileName.LastIndexOf('\\');
	if (index != -1)
	{
		path = _fileName.Substring(0, index);
		fileName = _fileName.Substring(index + 1);
	}

	bone->SetName(fileName);

	Array<SceneObject*> nodes;

	int cameraCount = _scene.edit->cameras.Count();
	for (int cameraIdx = 0; cameraIdx < cameraCount; cameraIdx++)
	{
		Mesh3DSCamera* srcCamera = _scene.edit->cameras[cameraIdx];

		Camera* camera = new Camera();
		camera->SetName(srcCamera->name);
		camera->SetLocalPosition(srcCamera->position);
		camera->LookAt(srcCamera->target);
		camera->SetFOV(srcCamera->fov);
		camera->SetNearPlane(srcCamera->near_range);
		camera->SetFarPlane(srcCamera->far_range);

		nodes.Add(camera);
	}

	int materialCount = _scene.edit->materials.Count();

	int meshCount = _scene.edit->meshes.Count();
	for (int meshIdx = 0; meshIdx < meshCount; meshIdx++)
	{
		Mesh3DSMesh* srcMesh = _scene.edit->meshes[meshIdx];

		if (srcMesh->vertex_count == 0)
			continue;

		// Builds an array containing the material indices of each face
		int* faceMaterials = new int[srcMesh->face_count];
		memset(faceMaterials, -1, srcMesh->face_count * sizeof(int));

		// Builds an array containing the material uses
		int* materialRefs = new int[materialCount];
		memset(materialRefs, 0, materialCount * sizeof(int));

		for (int fi = 0; fi < srcMesh->face_count; fi++)
		{
			for (int mi = 0; mi < materialCount; mi++)
			{
				if (srcMesh->faces[fi].material_name == _materials[mi]->GetName())
				{
					faceMaterials[fi] = mi;
					materialRefs[mi]++;
					break;
				}
			}
		}

		int size = 0;
		size += 2*sizeof(Vector3); //position + normal
		if (srcMesh->texel_count > 0)
			size += sizeof(Vector2); //texcoord0

		for (int mi = 0; mi < materialCount; mi++)
		{
			if (materialRefs[mi] == 0)
				continue;

			static int nameid = 0;
			String resid;
			if (!srcMesh->name.IsEmpty())
			{
				resid = _fileName + srcMesh->name;
			}
			else
			{
				char buffer[64];
				itoa(nameid++, buffer, 10);
				resid = _fileName + String(buffer);
			}

			Mesh* mesh = NULL;
			if (mesh == NULL)
			{
				mesh = new Mesh();
				mesh->SetName(srcMesh->name);

				MeshPart* meshPart = new MeshPart();

				// Compute face normals
				Vector3* faceNormals = new Vector3[srcMesh->face_count];
				for (int fi = 0; fi < srcMesh->face_count; fi++)
				{
					Vector3 v0 = srcMesh->vertices[srcMesh->faces[fi].indices[0]];
					Vector3 v1 = srcMesh->vertices[srcMesh->faces[fi].indices[1]];
					Vector3 v2 = srcMesh->vertices[srcMesh->faces[fi].indices[2]];
					Vector3 edge1 = v1 - v0;
					Vector3 edge2 = v2 - v0;
					faceNormals[fi] = edge1.Cross(edge2);
				}

				// Compute vertex normals
				Vector3* vertexNormals = new Vector3[srcMesh->vertex_count];
				for (int vi = 0; vi < srcMesh->vertex_count; vi++)
				{
					int faces = 0;
					Vector3 normals = Vector3::Zero;
					for (int fi = 0; fi < srcMesh->face_count; fi++)
					{
						if (vi == srcMesh->faces[fi].indices[0] ||
							vi == srcMesh->faces[fi].indices[1] ||
							vi == srcMesh->faces[fi].indices[2])
						{
							normals += faceNormals[fi];
							faces++;
						}
					}

					vertexNormals[vi] = Vector3::Normalize(normals / (real32)faces);
				}

				SEbyte* vertices = new SEbyte[materialRefs[mi]*3*size];
				int dataOffset = 0;
				for (int fi = 0; fi < srcMesh->face_count; fi++)
				{
					if (faceMaterials[fi] != mi)
						continue;

					for (int vi = 0; vi < 3; vi++)
					{
						Vector3 position = srcMesh->vertices[srcMesh->faces[fi].indices[vi]];
						Vector3 normal = vertexNormals[srcMesh->faces[fi].indices[vi]];

						position = Swap3DS(position);
						normal = Swap3DS(normal);

						Memory::Copy(vertices+dataOffset, &position, sizeof(Vector3));
						dataOffset += sizeof(Vector3);

						Memory::Copy(vertices+dataOffset, &normal, sizeof(Vector3));
						dataOffset += sizeof(Vector3);

						if (srcMesh->texel_count > 0)
						{
							Vector2 texel = srcMesh->texels[srcMesh->faces[fi].indices[vi]];
							texel.Y = 1.0 - texel.Y;
							Memory::Copy(vertices+dataOffset, &texel, sizeof(Vector2));
							dataOffset += sizeof(Vector2);
						}
					}
				}

				delete[] faceNormals;
				delete[] vertexNormals;

				RenderSystem* renderer = RenderSystem::Current();
				VertexLayout* vertexLayout;
				renderer->CreateVertexLayout(&vertexLayout);
				uint16 offset = 0;

				if (srcMesh->vertex_count > 0)
				{
					vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
					offset += VertexElement::GetTypeSize(VertexFormat_Float3);

					vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
					offset += VertexElement::GetTypeSize(VertexFormat_Float3);
				}

				if (srcMesh->texel_count > 0)
				{
					vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
					offset += VertexElement::GetTypeSize(VertexFormat_Float2);
				}

				vertexData->VertexLayout = vertexLayout;

				uint32 vertexCount = materialRefs[mi]*3;
				HardwareBuffer* vertexBuffer;
				if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(), HardwareBufferUsage_Static, &vertexBuffer))
				{
					SE_DELETE(vertexLayout);
					return false;
				}

				SEbyte* vbData;
				vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);
				Memory::Copy(vbData, vertices, vertexCount * vertexLayout->GetSize());
				vertexBuffer->Unmap();

				VertexData* vertexData = new VertexData();
				vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
				vertexData->VertexCount = vertexCount;
				meshPart->SetVertexData(vertexData);
				meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, vertexCount);

				/*HardwareBuffer* indexBuffer;
				Core::Instance()->GetRenderer()->CreateIndexBuffer(faceCount, 1, &indexBuffer);
				SEbyte* idxData;
				vertexBuffer->Lock(HBL_WriteOnly, (void**)&idxData);
				Memory::Copy(vbData, indexData, faceCount * sizeof(uint32));
				vertexBuffer->Unlock((void**)&idxData);
				mesh->SetIndexCount(faceCount*3);
				mesh->SetIndexData(indexBuffer);*/

				mesh->SetShader(_materials[mi]);
				mesh->AddMeshPart(meshPart);
			}

			/*MeshNode* meshNode = new MeshNode();
			meshNode->SetMesh(mesh);*/

			//meshNode->SetLocalPosition(Vector3(srcMesh->position.x, srcMesh->position.z, -srcMesh->position.y));
#if 0
			meshNode->SetLocalPosition(-srcMesh->position);
			Quaternion q;
			q.FromRotationMatrix(Matrix3::Transpose(kRot));
			meshNode->SetLocalOrientation(q);
			meshNode->SetLocalScale(kScaleVec);
#endif

			//nodes.Add(meshNode);
			//(*node)->AddChild(meshNode);
		}

		delete[] faceMaterials;
		delete[] materialRefs;
	}

	/*int nodeCount = _scene.keyf->nodes.Count();
	for (int nodeIdx = 0; nodeIdx < nodeCount; nodeIdx++)
	{
		Mesh3DSNode* srcNode = _scene.keyf->nodes[nodeIdx];

		for (int meshIdx = 0; meshIdx < meshCount; meshIdx++)
		{
			if (srcNode->name == _scene.edit->meshes[meshIdx]->name)
			{
				SceneObject* object = nodes[meshIdx];

				Vector3 pivot = Vector3(srcNode->pivot.x, srcNode->pivot.z, -srcNode->pivot.y);
				object->SetLocalPosition(object->GetLocalPosition() - pivot);

				(*node)->AddChild(object);
				break;
			}
		}
	}*/

	return true;
}

}
