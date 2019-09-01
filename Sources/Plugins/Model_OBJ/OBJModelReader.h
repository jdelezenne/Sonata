/*=============================================================================
OBJModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_OBJMODELREADER_H_
#define _SE_OBJMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_OBJ
{

struct OBJMaterial
{
	OBJMaterial() :
		Ni(0),
		Ns(0),
		illum(0),
		Tr(1.0f)
	{
		Memory::Set(Ka, 0.2f, 3 * sizeof(real32));
		Memory::Set(Kd, 0.8f, 3 * sizeof(real32));
		Memory::Set(Ks, 1.0f, 3 * sizeof(real32));
	}

	String name;
	int32 Ni;
	int32 Ns;
	int32 illum;
	real32 Tr;
	real32 Ka[3];
	real32 Kd[3];
	real32 Ks[3];
	String map_Ka;
	String map_Kd;
	String map_Ks;
};

struct OBJVertex
{
	int32 position;
	int32 normal;
	int32 texcoord;
};

struct OBJFace
{
	OBJVertex vertex[3];
};

struct OBJElement
{
	OBJElement()
	{
	}

	String name;
	BaseArray<Vector3> vertices;
	BaseArray<Vector3> normals;
	BaseArray<Vector2> texcoords;
	BaseArray<OBJFace> faces;
	String material;
};

class OBJModelReader : public ModelReader
{
public:
	OBJModelReader();
	virtual ~OBJModelReader();

	virtual Model* LoadModel(Stream& source, ModelReaderOptions* options = NULL);

protected:
	void ReadOBJ(Stream& stream);
	void ReadMTL(Stream& stream);

protected:
	Model* _model;
	BaseArray<OBJElement> _elements;
	BaseArray<OBJMaterial> _materials;
};

}

#endif 
