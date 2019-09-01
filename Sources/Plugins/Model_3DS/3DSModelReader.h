/*=============================================================================
3DSModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_3DSMODELREADER_H_
#define _SE_3DSMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_3DS
{

enum Mesh3DSChunkType
{
	M3DS_NULL_CHUNK             = 0x0000,
	M3DS_MAIN                   = 0x4D4D,
	M3DS_SMAIN                  = 0x2D2D,    
	M3DS_LMAIN                  = 0x2D3D,    
	M3DS_MLIBMAIN               = 0x3DAA,
	M3DS_MATMAIN                = 0x3DFF,    
	M3DS_CMAIN                  = 0xC23D,
	M3DS_MAIN_VERSION           = 0x0002,
	M3DS_MAIN_KEYFVERSION       = 0x0005,

	M3DS_COLOR_F                = 0x0010,
	M3DS_COLOR_24               = 0x0011,
	M3DS_LIN_COLOR_24           = 0x0012,
	M3DS_LIN_COLOR_F            = 0x0013,
	M3DS_INT_PERCENTAGE         = 0x0030,
	M3DS_FLOAT_PERCENTAGE       = 0x0031,

	M3DS_EDITOR                 = 0x3D3D,
	M3DS_MESH_VERSION           = 0x3D3E,
	M3DS_MASTER_SCALE           = 0x0100,
	M3DS_LO_SHADOW_BIAS         = 0x1400,
	M3DS_HI_SHADOW_BIAS         = 0x1410,
	M3DS_SHADOW_MAP_SIZE        = 0x1420,
	M3DS_SHADOW_SAMPLES         = 0x1430,
	M3DS_SHADOW_RANGE           = 0x1440,
	M3DS_SHADOW_FILTER          = 0x1450,
	M3DS_RAY_BIAS               = 0x1460,
	M3DS_O_CONSTS               = 0x1500,
	M3DS_AMBIENT_LIGHT          = 0x2100,
	M3DS_BIT_MAP                = 0x1100,
	M3DS_SOLID_BGND             = 0x1200,
	M3DS_V_GRADIENT             = 0x1300,
	M3DS_USE_BIT_MAP            = 0x1101,
	M3DS_USE_SOLID_BGND         = 0x1201,
	M3DS_USE_V_GRADIENT         = 0x1301,
	M3DS_FOG                    = 0x2200,
	M3DS_FOG_BGND               = 0x2210,
	M3DS_LAYER_FOG              = 0x2302,
	M3DS_DISTANCE_CUE           = 0x2300,
	M3DS_DCUE_BGND              = 0x2310,
	M3DS_USE_FOG                = 0x2201,
	M3DS_USE_LAYER_FOG          = 0x2303,
	M3DS_USE_DISTANCE_CUE       = 0x2301,

	M3DS_MAT_ENTRY              = 0xAFFF,
	M3DS_MAT_NAME               = 0xA000,
	M3DS_MAT_AMBIENT            = 0xA010,
	M3DS_MAT_DIFFUSE            = 0xA020,
	M3DS_MAT_SPECULAR           = 0xA030,
	M3DS_MAT_SHININESS          = 0xA040,
	M3DS_MAT_SHIN2PCT           = 0xA041,
	M3DS_MAT_TRANSPARENCY       = 0xA050,
	M3DS_MAT_XPFALL             = 0xA052,
	M3DS_MAT_USE_XPFALL         = 0xA240,
	M3DS_MAT_REFBLUR            = 0xA053,
	M3DS_MAT_SHADING            = 0xA100,
	M3DS_MAT_USE_REFBLUR        = 0xA250,
	M3DS_MAT_USE_SELF_ILLUM     = 0xA080,
	M3DS_MAT_TWO_SIDE           = 0xA081,
	M3DS_MAT_DECAL              = 0xA082,
	M3DS_MAT_ADDITIVE           = 0xA083,
	M3DS_MAT_SELF_ILLUM         = 0xA084,
	M3DS_MAT_WIRE               = 0xA085,
	M3DS_MAT_FACEMAP            = 0xA088,
	M3DS_MAT_FALL_OFF           = 0xA08A,
	M3DS_MAT_PHONGSOFT          = 0xA08C,
	M3DS_MAT_WIREABS            = 0xA08E,
	M3DS_MAT_WIRE_SIZE          = 0xA087,
	M3DS_MAT_TEXMAP             = 0xA200,
	M3DS_MAT_SXP_TEXT_DATA      = 0xA320,
	M3DS_MAT_TEXMASK            = 0xA33E,
	M3DS_MAT_SXP_TEXTMASK_DATA  = 0xA32A,
	M3DS_MAT_TEX2MAP            = 0xA33A,
	M3DS_MAT_SXP_TEXT2_DATA     = 0xA321,
	M3DS_MAT_TEX2MASK           = 0xA340,
	M3DS_MAT_SXP_TEXT2MASK_DATA = 0xA32C,
	M3DS_MAT_OPACMAP            = 0xA210,
	M3DS_MAT_SXP_OPAC_DATA      = 0xA322,
	M3DS_MAT_OPACMASK           = 0xA342,
	M3DS_MAT_SXP_OPACMASK_DATA  = 0xA32E,
	M3DS_MAT_BUMPMAP            = 0xA230,
	M3DS_MAT_SXP_BUMP_DATA      = 0xA324,
	M3DS_MAT_BUMPMASK           = 0xA344,
	M3DS_MAT_SXP_BUMPMASK_DATA  = 0xA330,
	M3DS_MAT_SPECMAP            = 0xA204,
	M3DS_MAT_SXP_SPEC_DATA      = 0xA325,
	M3DS_MAT_SPECMASK           = 0xA348,
	M3DS_MAT_SXP_SPECMASK_DATA  = 0xA332,
	M3DS_MAT_SHINMAP            = 0xA33C,
	M3DS_MAT_SXP_SHIN_DATA      = 0xA326,
	M3DS_MAT_SHINMASK           = 0xA346,
	M3DS_MAT_SXP_SHINMASK_DATA  = 0xA334,
	M3DS_MAT_SELFIMAP           = 0xA33D,
	M3DS_MAT_SXP_SELFI_DATA     = 0xA328,
	M3DS_MAT_SELFIMASK          = 0xA34A,
	M3DS_MAT_SXP_SELFIMASK_DATA = 0xA336,
	M3DS_MAT_REFLMAP            = 0xA220,
	M3DS_MAT_REFLMASK           = 0xA34C,
	M3DS_MAT_SXP_REFLMASK_DATA  = 0xA338,
	M3DS_MAT_ACUBIC             = 0xA310,
	M3DS_MAT_MAPNAME            = 0xA300,
	M3DS_MAT_MAP_TILING         = 0xA351,
	M3DS_MAT_MAP_TEXBLUR        = 0xA353,
	M3DS_MAT_MAP_USCALE         = 0xA354,
	M3DS_MAT_MAP_VSCALE         = 0xA356,
	M3DS_MAT_MAP_UOFFSET        = 0xA358,
	M3DS_MAT_MAP_VOFFSET        = 0xA35A,
	M3DS_MAT_MAP_ANG            = 0xA35C,
	M3DS_MAT_MAP_COL1           = 0xA360,
	M3DS_MAT_MAP_COL2           = 0xA362,
	M3DS_MAT_MAP_RCOL           = 0xA364,
	M3DS_MAT_MAP_GCOL           = 0xA366,
	M3DS_MAT_MAP_BCOL           = 0xA368,

	M3DS_NAMED_OBJECT           = 0x4000,
	M3DS_N_DIRECT_LIGHT         = 0x4600,
	M3DS_DL_OFF                 = 0x4620,
	M3DS_DL_OUTER_RANGE         = 0x465A,
	M3DS_DL_INNER_RANGE         = 0x4659,
	M3DS_DL_MULTIPLIER          = 0x465B,
	M3DS_DL_EXCLUDE             = 0x4654,
	M3DS_DL_ATTENUATE           = 0x4625,
	M3DS_DL_SPOTLIGHT           = 0x4610,
	M3DS_DL_SPOT_ROLL           = 0x4656,
	M3DS_DL_SHADOWED            = 0x4630,
	M3DS_DL_LOCAL_SHADOW2       = 0x4641,
	M3DS_DL_SEE_CONE            = 0x4650,
	M3DS_DL_SPOT_RECTANGULAR    = 0x4651,
	M3DS_DL_SPOT_ASPECT         = 0x4657,
	M3DS_DL_SPOT_PROJECTOR      = 0x4653,
	M3DS_DL_SPOT_OVERSHOOT      = 0x4652,
	M3DS_DL_RAY_BIAS            = 0x4658,
	M3DS_DL_RAYSHAD             = 0x4627,
	M3DS_N_CAMERA               = 0x4700,
	M3DS_CAM_SEE_CONE           = 0x4710,
	M3DS_CAM_RANGES             = 0x4720,
	M3DS_OBJ_HIDDEN             = 0x4010,
	M3DS_OBJ_VIS_LOFTER         = 0x4011,
	M3DS_OBJ_DOESNT_CAST        = 0x4012,
	M3DS_OBJ_DONT_RECVSHADOW    = 0x4017,
	M3DS_OBJ_MATTE              = 0x4013,
	M3DS_OBJ_FAST               = 0x4014,
	M3DS_OBJ_PROCEDURAL         = 0x4015,
	M3DS_OBJ_FROZEN             = 0x4016,
	M3DS_N_TRI_OBJECT           = 0x4100,
	M3DS_POINT_ARRAY            = 0x4110,
	M3DS_POINT_FLAG_ARRAY       = 0x4111,
	M3DS_FACE_ARRAY             = 0x4120,
	M3DS_MSH_MAT_GROUP          = 0x4130,
	M3DS_SMOOTH_GROUP           = 0x4150,
	M3DS_MSH_BOXMAP             = 0x4190,
	M3DS_TEX_VERTS              = 0x4140,
	M3DS_MESH_MATRIX            = 0x4160,
	M3DS_MESH_COLOR             = 0x4165,
	M3DS_MESH_TEXTURE_INFO      = 0x4170,

	M3DS_KFDATA                 = 0xB000,
	M3DS_KFHDR                  = 0xB00A,
	M3DS_KFSEG                  = 0xB008,
	M3DS_KFCURTIME              = 0xB009,
	M3DS_AMBIENT_NODE_TAG       = 0xB001,
	M3DS_OBJECT_NODE_TAG        = 0xB002,
	M3DS_CAMERA_NODE_TAG        = 0xB003,
	M3DS_TARGET_NODE_TAG        = 0xB004,
	M3DS_LIGHT_NODE_TAG         = 0xB005,
	M3DS_L_TARGET_NODE_TAG      = 0xB006,
	M3DS_SPOTLIGHT_NODE_TAG     = 0xB007,
	M3DS_NODE_ID                = 0xB030,
	M3DS_NODE_HDR               = 0xB010,
	M3DS_PIVOT                  = 0xB013,
	M3DS_INSTANCE_NAME          = 0xB011,
	M3DS_MORPH_SMOOTH           = 0xB015,
	M3DS_BOUNDBOX               = 0xB014,
	M3DS_POS_TRACK_TAG          = 0xB020,
	M3DS_ROT_TRACK_TAG          = 0xB021,
	M3DS_SCL_TRACK_TAG          = 0xB022,
	M3DS_FOV_TRACK_TAG          = 0xB023,
	M3DS_ROLL_TRACK_TAG         = 0xB024,
	M3DS_COL_TRACK_TAG          = 0xB025,
	M3DS_MORPH_TRACK_TAG        = 0xB026,
	M3DS_HOT_TRACK_TAG          = 0xB027,
	M3DS_FALL_TRACK_TAG         = 0xB028,
	M3DS_HIDE_TRACK_TAG         = 0xB029,

	M3DS_POLY_2D                = 0x5000,
	M3DS_SHAPE_OK               = 0x5010,
	M3DS_SHAPE_NOT_OK           = 0x5011,
	M3DS_SHAPE_HOOK             = 0x5020,
	M3DS_PATH_3D                = 0x6000,
	M3DS_PATH_MATRIX            = 0x6005,
	M3DS_SHAPE_2D               = 0x6010,
	M3DS_M_SCALE                = 0x6020,
	M3DS_M_TWIST                = 0x6030,
	M3DS_M_TEETER               = 0x6040,
	M3DS_M_FIT                  = 0x6050,
	M3DS_M_BEVEL                = 0x6060,
	M3DS_XZ_CURVE               = 0x6070,
	M3DS_YZ_CURVE               = 0x6080,
	M3DS_INTERPCT               = 0x6090,
	M3DS_DEFORM_LIMIT           = 0x60A0,

	M3DS_USE_CONTOUR            = 0x6100,
	M3DS_USE_TWEEN              = 0x6110,
	M3DS_USE_SCALE              = 0x6120,
	M3DS_USE_TWIST              = 0x6130,
	M3DS_USE_TEETER             = 0x6140,
	M3DS_USE_FIT                = 0x6150,
	M3DS_USE_BEVEL              = 0x6160,

	M3DS_DEFAULT_VIEW           = 0x3000,
	M3DS_VIEW_TOP               = 0x3010,
	M3DS_VIEW_BOTTOM            = 0x3020,
	M3DS_VIEW_LEFT              = 0x3030,
	M3DS_VIEW_RIGHT             = 0x3040,
	M3DS_VIEW_FRONT             = 0x3050,
	M3DS_VIEW_BACK              = 0x3060,
	M3DS_VIEW_USER              = 0x3070,
	M3DS_VIEW_CAMERA            = 0x3080,
	M3DS_VIEW_WINDOW            = 0x3090,

	M3DS_VIEWPORT_LAYOUT_OLD    = 0x7000,
	M3DS_VIEWPORT_DATA_OLD      = 0x7010,
	M3DS_VIEWPORT_LAYOUT        = 0x7001,
	M3DS_VIEWPORT_DATA          = 0x7011,
	M3DS_VIEWPORT_DATA_3        = 0x7012,
	M3DS_VIEWPORT_SIZE          = 0x7020,
	M3DS_NETWORK_VIEW           = 0x7030
};

struct Mesh3DSChunk
{
	Mesh3DSChunk() :
		id(0),
		size(0),
		offset(0)
	{
	}

	uint16 id;
	uint32 size;
	uint32 offset;
}; 

struct Mesh3DSScene;
	struct Mesh3DSEditor;
		struct Mesh3DSMaterial;
			struct Mesh3DSTexture;
		struct Mesh3DSMesh;
			struct Mesh3DSMeshFace;
		struct Mesh3DSCamera;
		struct Mesh3DSLight;
	struct Mesh3DSKeyframer;
		struct Mesh3DSNode;

struct Mesh3DSScene
{
	uint32 version;
	Mesh3DSEditor* edit;
	Mesh3DSKeyframer* keyf;
}; 

struct Mesh3DSEditor
{
	uint32 version;
	real32 master_scale;
	Color32 ambient_light;
	Array<Mesh3DSMaterial*> materials;
	Array<Mesh3DSMesh*> meshes;
	Array<Mesh3DSCamera*> cameras;
	Array<Mesh3DSLight*> lights;
}; 

struct Mesh3DSMaterial
{
	String name;
	Color32 ambient;
	Color32 diffuse;
	Color32 specular;
	real32 shininess;
	real32 shin_strength;
	bool use_blur;
	real32 blur;
	real32 transparency;
	real32 falloff;
	bool additive;
	bool use_falloff;
	bool use_falloff_in;
	bool has_self_illum;
	real32 self_illum;
	int16 shading;
	bool soften;
	bool face_map;
	bool two_sided;
	bool map_decal;
	bool use_wire;
	bool use_wire_abs;
	real32 wire_size;
	Mesh3DSTexture* texture1_map;
	Mesh3DSTexture* texture2_map;
	Mesh3DSTexture* opacity_map;
	Mesh3DSTexture* bump_map;
	Mesh3DSTexture* specular_map;
}; 

struct Mesh3DSTexture
{
	String name;
	uint16 flags;
	real32 percent;
	real32 blur;
	real32 scale[2];
	real32 offset[2];
	real32 rotation;
	Color32 tint_1;
	Color32 tint_2;
	Color32 tint_r;
	Color32 tint_g;
	Color32 tint_b;
};

struct Mesh3DSMeshFace
{
	uint16 indices[3];
	uint16 flag;
	String material_name;
	uint32 smoothing;
};

struct Mesh3DSMesh
{
	String name;
	uint8 color;
	Matrix3 scale_rot;
	Vector3 position;
	uint16 vertex_count;
	Vector3 *vertices;
	uint16 flag_count;
	uint16* flags;
	uint16 texel_count;
	Vector2* texels;
	uint16 face_count;
	Mesh3DSMeshFace* faces;
};

struct Mesh3DSCamera
{
	String name;
	Vector3 position;
	Vector3 target;
	real32 roll;
	real32 fov;
	bool see_cone;
	real32 near_range;
	real32 far_range;
}; 

struct Mesh3DSLight
{
	String name;
	bool spot_light;
	bool see_cone;
	Color32 color;
	Vector3 position;
	Vector3 spot;
	real32 roll;
	bool off;
	real32 outer_range;
	real32 inner_range;
	real32 multiplier;
	real32 attenuation;
	bool rectangular_spot;
	bool shadowed;
	real32 shadow_bias;
	real32 shadow_filter;
	int16 shadow_size;
	real32 spot_aspect;
	bool use_projector;
	String projector;
	bool spot_overshoot;
	bool ray_shadows;
	real32 ray_bias;
	real32 hot_spot;
	real32 fall_off;
}; 

struct Mesh3DSKeyframer
{
	int16 keyf_revision;
	String name;
	uint32 frames;
	uint32 segment_from;
	uint32 segment_to;
	uint32 current_frame;
	Array<Mesh3DSNode*> nodes;
};

struct Mesh3DSBaseData
{
	Vector3 pivot;
	String instance;
	Vector3 bbox_min;
	Vector3 bbox_max;
	Vector3 pos;
	Quaternion rot;
	Vector3 scl;
	real32 fov;
	real32 roll;
	Color32 col;
	real32 hotspot;
	real32 falloff;
	bool hide;
};

struct Mesh3DSNode
{
	uint16 node_id;
	String name;
	uint16 flags1;
	uint16 flags2;
	uint16 parent_id;
	Mesh3DSBaseData data;
};

/**
	3DS Mesh Importer.
	References:
		- The Unofficial 3DStudio 3DS File Format by Jeff Lewis
		- lib3ds
*/
class M3DSModelReader : public ModelReader
{
public:
	M3DSModelReader();
	virtual ~M3DSModelReader();

	virtual Model* LoadModel(Stream& source, ModelReaderOptions* options = NULL);

protected:
	bool ReadModel();

	bool ReadChunk(Mesh3DSChunk& chunk);
	bool ReadNextChunk(const Mesh3DSChunk& parent, Mesh3DSChunk& chunk);
	void SkipChunk(const Mesh3DSChunk& chunk);
	String ReadString();
	real32 ReadPercentageChunk();
	Color32 ReadColorFloat();
	Color32 ReadColorInt();
	Color32 ReadColorChunk();
	Vector2 ReadVector2();
	Vector3 ReadVector3();

	void ReadEditorChunk(const Mesh3DSChunk& parent, Mesh3DSEditor& edit);
	void ReadMaterialChunk(const Mesh3DSChunk& parent, Mesh3DSMaterial& material);
	void ReadTextureChunk(const Mesh3DSChunk& parent, Mesh3DSTexture& texture);
	void ReadObjectChunk(const Mesh3DSChunk& parent, Mesh3DSEditor& edit);
	void ReadMeshChunk(const Mesh3DSChunk& parent, Mesh3DSMesh& mesh);
	void ReadMeshFaceChunk(const Mesh3DSChunk& parent, Mesh3DSMesh& mesh);
	void ReadCameraChunk(const Mesh3DSChunk& parent, Mesh3DSCamera& camera);
	void ReadLightChunk(const Mesh3DSChunk& parent, Mesh3DSLight& light);
	void ReadSpotlightChunk(const Mesh3DSChunk& parent, Mesh3DSLight& light);
	void ReadKeyframerChunk(const Mesh3DSChunk& parent, Mesh3DSKeyframer& keyf);
	void ReadNodeChunk(const Mesh3DSChunk& parent, Mesh3DSNode& node);

	Model* BuildModel();
	void BuildMaterials(Array<ShaderMaterial*>& materials);
	bool BuildFrame(Model* model, Bone* bone);

protected:
	String _fileName;
	Stream* _stream;
	Mesh3DSScene _scene;
	Array<ShaderMaterial*> _materials;
};

}

#endif 
