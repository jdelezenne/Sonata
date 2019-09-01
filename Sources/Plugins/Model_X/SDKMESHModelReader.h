/*=============================================================================
SDKMESHModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SDKMESHMODELREADER_H_
#define _SE_SDKMESHMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_X
{

#define MAX_PATH 260

#define SDKMESH_FILE_VERSION 101
#define MAX_VERTEX_ELEMENTS 32
#define MAX_VERTEX_STREAMS 16
#define MAX_FRAME_NAME 100
#define MAX_MESH_NAME 100
#define MAX_SUBSET_NAME 100
#define MAX_MATERIAL_NAME 100
#define MAX_TEXTURE_NAME MAX_PATH
#define MAX_MATERIAL_PATH MAX_PATH
#define INVALID_FRAME ((UINT)-1)
#define INVALID_MESH ((UINT)-1)
#define INVALID_MATERIAL ((UINT)-1)
#define INVALID_SUBSET ((UINT)-1)
#define INVALID_ANIMATION_DATA ((UINT)-1)
#define ERROR_RESOURCE_VALUE 1

typedef SEbyte BYTE;
typedef uint16 WORD;
typedef uint32 UINT;
typedef uint64 UINT64;
typedef real32 FLOAT;

struct D3DXVECTOR3
{
    real32 x;
    real32 y;
    real32 z;
};

struct D3DXVECTOR4
{
    real32 x;
    real32 y;
    real32 z;
    real32 w;
};

struct D3DXMATRIX
{
    union
	{
        struct
		{
            real32        _11, _12, _13, _14;
            real32        _21, _22, _23, _24;
            real32        _31, _32, _33, _34;
            real32        _41, _42, _43, _44;

        };
        real32 m[4][4];
    };
};

struct D3DVERTEXELEMENT9
{
    WORD    Stream;     // Stream index
    WORD    Offset;     // Offset in the stream in bytes
    BYTE    Type;       // Data type
    BYTE    Method;     // Processing method
    BYTE    Usage;      // Semantics
    BYTE    UsageIndex; // Semantic index
};

enum D3DDECLTYPE
{
    D3DDECLTYPE_FLOAT1    =  0,  // 1D float expanded to (value, 0., 0., 1.)
    D3DDECLTYPE_FLOAT2    =  1,  // 2D float expanded to (value, value, 0., 1.)
    D3DDECLTYPE_FLOAT3    =  2,  // 3D float expanded to (value, value, value, 1.)
    D3DDECLTYPE_FLOAT4    =  3,  // 4D float
    D3DDECLTYPE_D3DCOLOR  =  4,  // 4D packed unsigned bytes mapped to 0. to 1. range
                                 // Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
    D3DDECLTYPE_UBYTE4    =  5,  // 4D unsigned byte
    D3DDECLTYPE_SHORT2    =  6,  // 2D signed short expanded to (value, value, 0., 1.)
    D3DDECLTYPE_SHORT4    =  7,  // 4D signed short

// The following types are valid only with vertex shaders >= 2.0


    D3DDECLTYPE_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
    D3DDECLTYPE_SHORT2N   =  9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
    D3DDECLTYPE_SHORT4N   = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
    D3DDECLTYPE_USHORT2N  = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
    D3DDECLTYPE_USHORT4N  = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
    D3DDECLTYPE_UDEC3     = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
    D3DDECLTYPE_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
    D3DDECLTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
    D3DDECLTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values
    D3DDECLTYPE_UNUSED    = 17,  // When the type field in a decl is unused.
};

enum D3DDECLUSAGE
{
    D3DDECLUSAGE_POSITION = 0,
    D3DDECLUSAGE_BLENDWEIGHT,   // 1
    D3DDECLUSAGE_BLENDINDICES,  // 2
    D3DDECLUSAGE_NORMAL,        // 3
    D3DDECLUSAGE_PSIZE,         // 4
    D3DDECLUSAGE_TEXCOORD,      // 5
    D3DDECLUSAGE_TANGENT,       // 6
    D3DDECLUSAGE_BINORMAL,      // 7
    D3DDECLUSAGE_TESSFACTOR,    // 8
    D3DDECLUSAGE_POSITIONT,     // 9
    D3DDECLUSAGE_COLOR,         // 10
    D3DDECLUSAGE_FOG,           // 11
    D3DDECLUSAGE_DEPTH,         // 12
    D3DDECLUSAGE_SAMPLE,        // 13
};

enum SDKMESH_PRIMITIVE_TYPE
{
    PT_TRIANGLE_LIST = 0,
    PT_TRIANGLE_STRIP,
    PT_LINE_LIST,
    PT_LINE_STRIP,
    PT_POINT_LIST,
    PT_TRIANGLE_LIST_ADJ,
    PT_TRIANGLE_STRIP_ADJ,
    PT_LINE_LIST_ADJ,
    PT_LINE_STRIP_ADJ,
};

enum SDKMESH_INDEX_TYPE
{
    IT_16BIT = 0,
    IT_32BIT,
};

enum FRAME_TRANSFORM_TYPE
{
    FTT_RELATIVE = 0,
    FTT_ABSOLUTE,		//This is not currently used but is here to support absolute transformations in the future
};

struct SDKMESH_HEADER
{
    //Basic Info and sizes
    UINT   Version;
    BYTE   IsBigEndian;
    UINT64 HeaderSize;
    UINT64 NonBufferDataSize;
    UINT64 BufferDataSize;

    //Stats
    UINT NumVertexBuffers;
    UINT NumIndexBuffers;
    UINT NumMeshes;
    UINT NumTotalSubsets;
    UINT NumFrames;
    UINT NumMaterials;

    //Offsets to Data
    UINT64 VertexStreamHeadersOffset;
    UINT64 IndexStreamHeadersOffset;
    UINT64 MeshDataOffset;
    UINT64 SubsetDataOffset;
    UINT64 FrameDataOffset;
    UINT64 MaterialDataOffset;
};

struct SDKMESH_VERTEX_BUFFER_HEADER
{
    UINT64 NumVertices;
    UINT64 SizeBytes;
    UINT64 StrideBytes;
    D3DVERTEXELEMENT9 Decl[MAX_VERTEX_ELEMENTS];
    UINT64	DataOffset;
};

struct SDKMESH_INDEX_BUFFER_HEADER
{
    UINT64 NumIndices;
    UINT64 SizeBytes;
    UINT IndexType;
    UINT64	DataOffset;
};

struct SDKMESH_MESH
{
    char Name[MAX_MESH_NAME];
    BYTE NumVertexBuffers;
    UINT VertexBuffers[MAX_VERTEX_STREAMS];
    UINT IndexBuffer;
    UINT NumSubsets;
    UINT NumFrameInfluences; //aka bones

    D3DXVECTOR3 BoundingBoxCenter;
    D3DXVECTOR3 BoundingBoxExtents;

    union
    {
        UINT64 SubsetOffset;	//Offset to list of subsets (This also forces the union to 64bits)
        UINT*  pSubsets;	    //Pointer to list of subsets
    };
    union
    {
        UINT64 FrameInfluenceOffset;  //Offset to list of frame influences (This also forces the union to 64bits)
        UINT*  pFrameInfluences;      //Pointer to list of frame influences
    };
};

struct SDKMESH_SUBSET
{
    char		Name[MAX_SUBSET_NAME];
    UINT		MaterialID;
    UINT		PrimitiveType;
    UINT64		IndexStart;
    UINT64		IndexCount;
    UINT64		VertexStart;
    UINT64		VertexCount;
};

struct SDKMESH_FRAME
{
    char Name[MAX_FRAME_NAME];
    UINT Mesh;
    UINT ParentFrame;
    UINT ChildFrame;
    UINT SiblingFrame;
    D3DXMATRIX Matrix;
    UINT AnimationDataIndex;		//Used to index which set of keyframes transforms this frame
};

struct SDKMESH_MATERIAL
{
    char		Name[MAX_MATERIAL_NAME];

    // Use MaterialInstancePath
    char		MaterialInstancePath[MAX_MATERIAL_PATH];

    // Or fall back to d3d8-type materials
    char		DiffuseTexture[MAX_TEXTURE_NAME];
    char		NormalTexture[MAX_TEXTURE_NAME];
    char		SpecularTexture[MAX_TEXTURE_NAME];

    D3DXVECTOR4	Diffuse;
    D3DXVECTOR4 Ambient;
    D3DXVECTOR4 Specular;
    D3DXVECTOR4 Emissive;
    FLOAT		Power;

	UINT64		Force64_1;
	UINT64		Force64_2;
	UINT64		Force64_3;
	UINT64		Force64_4;
	UINT64		Force64_5;
	UINT64		Force64_6;
};

struct SDKANIMATION_FILE_HEADER
{
    UINT Version;
    BYTE IsBigEndian;
    UINT FrameTransformType;
    UINT NumFrames;
    UINT NumAnimationKeys;
    UINT AnimationFPS;
    UINT64 AnimationDataSize;
    UINT64 AnimationDataOffset;
};

struct SDKANIMATION_DATA
{
    D3DXVECTOR3 Translation;
    D3DXVECTOR4 Orientation;
    D3DXVECTOR3 Scaling;
};

struct SDKANIMATION_FRAME_DATA
{
    char FrameName[MAX_FRAME_NAME];
    union
    {
        UINT64			   DataOffset;
        SDKANIMATION_DATA* pAnimationData;
    };
};

struct SDKMESH
{
	//These are the pointers to the two chunks of data loaded in from the mesh file
	BYTE*                           m_pStaticMeshData;
	BYTE*                           m_pAnimationData;

	//General mesh info
	SDKMESH_HEADER*                 m_pMeshHeader;
	SDKMESH_VERTEX_BUFFER_HEADER*   m_pVertexBufferArray;
	SDKMESH_INDEX_BUFFER_HEADER*    m_pIndexBufferArray;
	SDKMESH_MESH*                   m_pMeshArray;
	SDKMESH_SUBSET*                 m_pSubsetArray;
	SDKMESH_FRAME*                  m_pFrameArray;
	SDKMESH_MATERIAL*               m_pMaterialArray;

	// Adjacency information (not part of the m_pStaticMeshData, so it must be created and destroyed separately )
	SDKMESH_INDEX_BUFFER_HEADER*    m_pAdjacencyIndexBufferArray;

	//Animation (TODO: Add ability to load/track multiple animation sets)
	SDKANIMATION_FILE_HEADER*       m_pAnimationHeader;
	SDKANIMATION_FRAME_DATA*        m_pAnimationFrameData;
};

struct SDKMESHVertexStream
{
	BaseArray<VertexElement> _vertexElements;
	HardwareBuffer* _vertexBuffer;
	UINT _strideBytes;
};

/**
	Microsoft DirectX SDKMesh Model Reader.
*/
class SDKMESHModelReader : public ModelReader
{
protected:
	String _fileName;
	String _path;
	SDKMESH _sdkmesh;
	Model* _model;
	BaseArray<SDKMESHVertexStream> _vertexStreams;
	BaseArray<HardwareBuffer*> _indexBuffers;
	BaseArray<ShaderMaterial*> _materials;

public:
	SDKMESHModelReader();
	virtual ~SDKMESHModelReader();

	virtual Model* LoadModel(Stream& source, ModelReaderOptions* options = NULL);

protected:
	void D3DXMATRIXToTransform(D3DXMATRIX* d3dxmatrix, Vector3& position, Quaternion& orientation, Vector3& scale);
	PrimitiveType GetPrimitiveType(SDKMESH_PRIMITIVE_TYPE primType);
	IndexBufferFormat GetIBFormat(UINT indexType);
	VertexFormat GetVertexFormatType(D3DDECLTYPE value);
	VertexSemantic GetVertexSemantic(D3DDECLUSAGE value);
	SDKMESH_FRAME* FindFrame(const String& name);

	bool ReadModel();
	bool ReadAnimation();

	void Destroy();

	Model* CreateModel();
	void CreateFrames(SDKMESH_FRAME* pFrames, UINT numFrames);
	void CreateVertexBuffer(SDKMESH_VERTEX_BUFFER_HEADER* pHeader, void* pVertices);
	void CreateIndexBuffer(SDKMESH_INDEX_BUFFER_HEADER* pHeader, void* pIndices);
	void CreateMaterials(SDKMESH_MATERIAL* pMaterials, UINT numMaterials);
	void CreateMeshes(SDKMESH_MESH* pMeshes, UINT numMeshes);
	void CreateHierarchy();
	void CreateAnimation();
};

}

#endif 
