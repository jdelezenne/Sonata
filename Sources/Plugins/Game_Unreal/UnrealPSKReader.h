/*=============================================================================
UnrealPSKReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UNREALPSKREADER_H_
#define _SE_UNREALPSKREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_Unreal
{

// Unsigned base types.
typedef uint8		BYTE;		// 8-bit  unsigned.
typedef uint16		_WORD;		// 16-bit unsigned.
typedef uint32		DWORD;		// 32-bit unsigned.
typedef uint64		QWORD;		// 64-bit unsigned.

// Signed base types.
typedef	int8		SBYTE;		// 8-bit  signed.
typedef int16		SWORD;		// 16-bit signed.
typedef int32  		INT;		// 32-bit signed.
typedef int64		SQWORD;		// 64-bit signed.

// Character types.
typedef int8		ANSICHAR;	// An ANSI character.
typedef uint16      UNICHAR;	// A unicode character.
typedef uint8		ANSICHARU;	// An ANSI character.
typedef uint16      UNICHARU;	// A unicode character.

// Other base types.
typedef int32		UBOOL;		// Boolean 0 (false) or 1 (true).
typedef real32		FLOAT;		// 32-bit IEEE floating point.
typedef real64		DOUBLE;		// 64-bit IEEE double.
typedef uint32      SIZE_T;     // Corresponds to C SIZE_T.

// Bitfield type.
typedef uint32       BITFIELD;	// For bitfields.

class FVector 
{
public:
	// Variables.
	FLOAT X,Y,Z;

	// Constructors.
	FVector()
	{}
	FVector( FLOAT InX, FLOAT InY, FLOAT InZ )
	:	X(InX), Y(InY), Z(InZ)
	{}
};

class FQuat
{
public:
	// Variables.
	FLOAT X,Y,Z,W;

	// Constructors.
	FQuat()
	{}
	FQuat( FLOAT InX, FLOAT InY, FLOAT InZ, FLOAT InW )
	:	X(InX), Y(InY), Z(InZ), W(InW)
	{}
};

struct VChunkHeader
{
	ANSICHAR ChunkID[20];
	INT Typeflag;
	INT DataSize;
	INT DataCount;
};

struct VPoint
{
	FVector Point;
};

struct VVertex
{
	_WORD PointIndex;
	FLOAT U,V;
	BYTE MatIndex;
	BYTE Reserved;
};

struct VTriangle
{
	_WORD WedgeIndex[3];
	BYTE MatIndex;
	BYTE AuxMatIndex;
	DWORD SmoothingGroups;
};

struct VMaterial
{
	ANSICHAR MaterialName[64];
	INT TextureIndex;
	DWORD PolyFlags;
	INT AuxMaterial;
	DWORD AuxFlags;
	INT LodBias;
	INT LodStyle;
};

struct VJointPos
{
	FQuat Orientation;
	FVector Position;

	FLOAT Length;
	FLOAT XSize;
	FLOAT YSize;
	FLOAT ZSize;
};

struct VBone
{
	ANSICHAR Name[64];
	DWORD Flags;
	INT NumChildren;
	INT ParentIndex;
	VJointPos BonePos;
};

struct VRawBoneInfluence
{
	FLOAT Weight;
	INT PointIndex;
	INT BoneIndex;
};

struct AnimInfoBinary
{
	ANSICHAR Name[64];
	ANSICHAR Group[64];

	INT TotalBones;

	INT RootInclude;
	INT KeyCompressionStyle;
	INT KeyQuotum;
	FLOAT KeyReduction;
	FLOAT TrackTime;
	FLOAT AnimRate;
	INT StartBone;
	INT FirstRawFrame;
	INT NumRawFrames;
};

class FNamedBoneBinary
{
public:
	// Variables.
	ANSICHAR Name[64];
	DWORD Flags;
	INT NumChildren;
	INT ParentIndex;
	VJointPos BonePos;
};

struct VQuatAnimKey
{
	FVector Position;
	FQuat Orientation;
	FLOAT Time;
};

struct PSKFile
{
	BaseArray<VPoint> points;
	BaseArray<VVertex> vertices;
	BaseArray<VTriangle> triangles;
	BaseArray<VMaterial> materials;
	BaseArray<VBone> bones;
	BaseArray<VRawBoneInfluence> rawBoneInfluence;
};

struct PSAFile
{
	BaseArray<FNamedBoneBinary> namedBones;
	BaseArray<AnimInfoBinary> animInfos;
	BaseArray<VQuatAnimKey> quatAnimKeys;
};

class UnrealPSKReader : public ModelReader
{
protected:
	BinaryStream* _stream;
	String _fileName;
	String _path;
	PSKFile _pskFile;
	PSAFile _psaFile;

public:
	UnrealPSKReader();
	virtual ~UnrealPSKReader();

	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL);

protected:
	bool ReadPSK();
	bool ReadPSA();
	void Destroy();
	ShaderMaterial* CreateMaterial();
	Model* CreateModel();
	Mesh* CreateMesh();
	AnimationSet* CreateAnimation(Skeleton* skeleton);
};

}

#endif 
