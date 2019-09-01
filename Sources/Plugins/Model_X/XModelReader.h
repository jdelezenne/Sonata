/*=============================================================================
XModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMODELREADER_H_
#define _SE_XMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_X
{

#define XOFFILE_FORMAT_MAGIC \
  ((long)'x' + ((long)'o' << 8) + ((long)'f' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_VERSION2 \
  ((long)'0' + ((long)'3' << 8) + ((long)'0' << 16) + ((long)'2' << 24))

#define XOFFILE_FORMAT_VERSION3 \
  ((long)'0' + ((long)'3' << 8) + ((long)'0' << 16) + ((long)'3' << 24))

#define XOFFILE_FORMAT_BINARY \
  ((long)'b' + ((long)'i' << 8) + ((long)'n' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_TEXT   \
  ((long)'t' + ((long)'x' << 8) + ((long)'t' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_COMPRESSED \
  ((long)'c' + ((long)'m' << 8) + ((long)'p' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_FLOAT_BITS_32 \
  ((long)'0' + ((long)'0' << 8) + ((long)'3' << 16) + ((long)'2' << 24))

#define XOFFILE_FORMAT_FLOAT_BITS_64 \
  ((long)'0' + ((long)'0' << 8) + ((long)'6' << 16) + ((long)'4' << 24))

#define TOKEN_NAME         1
#define TOKEN_STRING       2
#define TOKEN_INTEGER      3
#define TOKEN_GUID         5
#define TOKEN_INTEGER_LIST 6
#define TOKEN_FLOAT_LIST   7

#define TOKEN_OBRACE      10
#define TOKEN_CBRACE      11
#define TOKEN_OPAREN      12
#define TOKEN_CPAREN      13
#define TOKEN_OBRACKET    14
#define TOKEN_CBRACKET    15
#define TOKEN_OANGLE      16
#define TOKEN_CANGLE      17
#define TOKEN_DOT         18
#define TOKEN_COMMA       19
#define TOKEN_SEMICOLON   20
#define TOKEN_TEMPLATE    31
#define TOKEN_WORD        40
#define TOKEN_DWORD       41
#define TOKEN_FLOAT       42
#define TOKEN_DOUBLE      43
#define TOKEN_CHAR        44
#define TOKEN_UCHAR       45
#define TOKEN_SWORD       46
#define TOKEN_SDWORD      47
#define TOKEN_VOID        48
#define TOKEN_LPSTR       49
#define TOKEN_UNICODE     50
#define TOKEN_CSTRING     51
#define TOKEN_ARRAY       52

struct XASCIIHeader
{
	uint32 Magic;
	uint32 Version;
	uint32 Format;
	uint32 Compression;
	uint32 FloatSize;
};

struct XHeader;
struct XFrame;
	struct XFrameTransformMatrix;
	struct XMesh;
		struct XMeshFace;
		struct XMeshNormals;
		struct XMeshVertexColors;
			struct XIndexedColor;
		struct XMeshTextureCoords;
		struct XMeshMaterialList;
			struct XMaterial;
				struct XTextureFilename;
				struct XEffectInstance;
				struct XEffectParam;
		struct XVertexDuplicationIndices;
struct XAnimTicksPerSecond;
struct XAnimationSet;
	struct XAnimation;
		struct XAnimationKey;
			struct XTimedFloatKeys;
				struct XFloatKeys;

struct XHeader
{
	uint32 header[3];
};

struct XVector 
{ 
	real32 x; 
	real32 y; 
	real32 z; 
};

struct XMatrix4x4
{
	real32 matrix[16];
};

struct XCoords2d
{
	real32 u;
	real32 v;
};

struct XColorRGB
{
    real32 red;
    real32 green;
    real32 blue;
};

struct XColorRGBA
{
    real32 red;
    real32 green;
    real32 blue;
	real32 alpha;
};

struct XBoolean
{
	uint32 truefalse;
};

struct XBoolean2d
{
	XBoolean u;
	XBoolean v;
};

struct XFrameTransformMatrix
{
	XFrameTransformMatrix()
	{
		Memory::Copy(frameMatrix.matrix, (void*)&Matrix4::Identity, sizeof(Matrix4));
	}

	XMatrix4x4 frameMatrix;
};

struct XMeshFace
{
	XMeshFace() : nFaceVertexIndices(0), faceVertexIndices(NULL) {}

	uint32 nFaceVertexIndices;
	uint32* faceVertexIndices;
};

struct MeshFaceWraps
{
	MeshFaceWraps() : nFaceWrapValues(0), faceWrapValues(NULL) {}

	uint32 nFaceWrapValues;
	XBoolean2d* faceWrapValues;
};

struct XMeshNormals
{
	XMeshNormals() : nNormals(0), normals(NULL), nFaceNormals(0), faceNormals(NULL) {}

	uint32 nNormals;
	XVector* normals;
	uint32 nFaceNormals;
	XMeshFace* faceNormals;
};

struct XIndexedColor
{
	XIndexedColor() : index(0) {}

	uint32 index;
	XColorRGBA indexColor;
};

struct XMeshVertexColors
{
	XMeshVertexColors() : nVertexColors(0), vertexColors(NULL) {}

	uint32 nVertexColors;
	XIndexedColor* vertexColors;
};

struct XMeshTextureCoords
{
	XMeshTextureCoords() : nTextureCoords(0), textureCoords(NULL) {}

	uint32 nTextureCoords;
	XCoords2d* textureCoords;
};

struct XEffectParamString
{
	String paramName;
	String value;
};

struct XEffectParamDWord
{
	XEffectParamDWord() : value(0) {}

	String paramName;
	uint32 value;
};

struct XEffectParamFloats
{
	XEffectParamFloats() : nFloats(0), floats(NULL) {}

	String paramName;
	uint32 nFloats;
	real32* floats;
};

struct XEffectInstance
{
	String effectFilename;
	BaseArray<XEffectParamString> stringParams;
	BaseArray<XEffectParamDWord> dwordParams;
	BaseArray<XEffectParamFloats> floatsParams;
};

struct XTextureFilename
{
	String filename;
};

struct XMaterialWrap
{
	XBoolean u;
	XBoolean v;
};

struct XMaterial
{
	XMaterial() : power(0), effectInstance(NULL) {}

	String name;
	XColorRGBA faceColor;
	real32 power;
	XColorRGB specularColor;
	XColorRGB emissiveColor;
	XTextureFilename textureFilename;
	XEffectInstance* effectInstance;
};

struct XMeshMaterialList
{
	XMeshMaterialList() : nMaterials(0), nFaceIndexes(0), faceIndexes(NULL) {}

	uint32 nMaterials;
	uint32 nFaceIndexes;
	uint32* faceIndexes;
	BaseArray<XMaterial*> materials;
};

struct XVertexDuplicationIndices
{
	XVertexDuplicationIndices() : nIndices(0), nOriginalVertices(0), indices(NULL) {}

	uint32 nIndices;
	uint32 nOriginalVertices;
	uint32* indices;
};

struct XFaceAdjacency
{
	XFaceAdjacency() : nIndices(0), indices(NULL) {}

	uint32 nIndices;
	uint32* indices;
};

struct XSkinMeshHeader
{
	XSkinMeshHeader() : nMaxSkinWeightsPerVertex(0), nMaxSkinWeightsPerFace(0), nBones(0) {}

	int32 nMaxSkinWeightsPerVertex; 
	int32 nMaxSkinWeightsPerFace; 
	int32 nBones; 
};

struct XSkinWeights 
{ 
    String transformNodeName; 
    uint32 nWeights; 
    uint32* vertexIndices; 
    real32* weights; 
    XMatrix4x4 matrixOffset; 
};

struct XMesh
{
	XMesh() : nVertices(0), vertices(NULL), nFaces(0), faces(NULL) {}

	String name;
	uint32 nVertices;
	XVector* vertices;
	uint32 nFaces;
	XMeshFace* faces;
	XMeshNormals normals;
	XMeshVertexColors vertexColors;
	XMeshTextureCoords textureCoords;
	XMeshMaterialList materialList;
	XVertexDuplicationIndices vertexDuplicationIndices;
	XSkinMeshHeader skinMeshHeader;
	BaseArray<XSkinWeights*> skinWeights;
};
 
struct XFrame
{
	XFrame() { frames.SetCapacity(50); }

	String name;
	XFrameTransformMatrix transformMatrix;
	BaseArray<XFrame*> frames;
	BaseArray<XMesh*> meshes;
};

struct XAnimTicksPerSecond
{
	XAnimTicksPerSecond() : AnimTicksPerSecond(0) {}

	uint32 AnimTicksPerSecond;
};

struct XFloatKeys
{
	XFloatKeys() : nValues(0), values(NULL) {}

    uint32 nValues;
    real32* values;
};

struct XTimedFloatKeys
{
	XTimedFloatKeys() : time(0) {}

	uint32 time; 
	XFloatKeys tfkeys;
};

struct XAnimationKey
{
	XAnimationKey() : keyType(0), nKeys(0) {}

	uint32 keyType;
	uint32 nKeys;
	XTimedFloatKeys* keys;
};

struct XAnimationOptions
{
	XAnimationOptions() : openclosed(0), positionquality(0) {}

	uint32 openclosed;
	uint32 positionquality;
};

struct XAnimation
{
	String name;
	BaseArray<XAnimationKey*> animationKeys;
	String frame;
};

struct XAnimationSet
{
	String name;
	BaseArray<XAnimation*> animations;
};

struct XModel
{
	XASCIIHeader headerASCII;
	XHeader header;
	BaseArray<XFrame*> frames;
	BaseArray<XMaterial*> materials;
	XAnimTicksPerSecond animTicksPerSecond;
	BaseArray<XAnimationSet*> animationSets;
};

/**
	DirectX File Model Reader.
	References:
		- X File Format Reference (Microsoft DirectX 9 SDK)
		- Microsoft DirectX File Format Specification, Version 1.13
*/
class XModelReader : public ModelReader
{
protected:
	Stream* _stream;
	String _fileName;
	String _path;
	XModel _xfile;
	Array<ShaderMaterial*> _materials;

public:
	XModelReader();
	virtual ~XModelReader();

	virtual Model* LoadModel(Stream& source, ModelReaderOptions* options = NULL);

protected:
	static void XMatrix4x4ToTransform(real32* xmatrix, Vector3& position, Quaternion& orientation, Vector3& scale);

	bool ReadModel();

	uint16 ReadToken();
	void ReadBeginBlock();
	void ReadEndBlock();
	void SkipBlock();
	void SkipBlock(const String& name);
	String GetString();
	String ReadName();
	String ReadBlockName();
	String ReadString();
	void ReadType();
	void ReadGUID();
	void ReadIntegerList(uint32** list);
	void ReadIntegerList(uint32* list, int count);
	void ReadFloatList(real32** list);
	void ReadFloatList(real32* list, int count);

	void ReadHeader(XHeader* header);
	void ReadTemplate();
	void ReadMaterial(XMaterial* material);
	void ReadFrame(XFrame* frame);
	void ReadMesh(XMesh* mesh);
	void ReadMeshNormals(XMeshNormals* meshNormals);
	void ReadMeshVertexColors(XMeshVertexColors* meshVertexColors);
	void ReadMeshTextureCoords(XMeshTextureCoords* meshTextureCoords);
	void ReadMeshMaterialList(XMeshMaterialList* meshMaterialList);
	void ReadVertexDuplicationIndices(XVertexDuplicationIndices* vertexDuplicationIndices);
	void ReadSkinMeshHeader(XSkinMeshHeader* skinMeshHeader);
	void ReadSkinWeights(XSkinWeights* skinWeights);
	void ReadAnimationSet(XAnimationSet* animationSet);
	void ReadAnimation(XAnimation* animation);
	void ReadAnimationKey(XAnimationKey* animationKey);
	void ReadEffectInstance(XEffectInstance* effectInstance);

	void Destroy();
	void DestroyFrame(XFrame* frame);
	void DestroyMesh(XMesh* mesh);
	void DestroyAnimationSet(XAnimationSet* animationSet);
	void DestroyAnimationKey(XAnimationKey* animationKey);

	uint32 GetFrameIndex(const String& name);
	uint32 GetFrameIndex(const String& name, const BaseArray<XFrame*>& xframes, uint32& index);

	Model* CreateModel();
	Texture* CreateTexture(const String& name);
	void CreateMaterials(Array<ShaderMaterial*>& materials, const BaseArray<XMaterial*>& xmaterials);
	void CreateFrame(Model* model, Bone* bone, XFrame* xframe);
	void CreateMesh(Mesh* mesh, XMesh* xmesh);
	void CreateFrameSkin(Skeleton* skeleton, XFrame* xframe);
	void CreateSkin(Skeleton* skeleton, XMesh* xmesh);
	void CreateAnimationSet(AnimationSet* animationSet, Skeleton* skeleton, XAnimationSet* xanimationset);
};

}

#endif 
