/*=============================================================================
OBJModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_OBJMODELREADER_H_
#define _RAYTRACER_OBJMODELREADER_H_

#include "Common.h"
#include "Scene.h"

namespace Raytracer
{

struct OBJMaterial
{
	OBJMaterial() :
		Ni(0),
		Ns(0),
		illum(0),
		Tr(1.0f)
	{
		memset(Ka, 0.2f, 3 * sizeof(real32));
		memset(Kd, 0.8f, 3 * sizeof(real32));
		memset(Ks, 1.0f, 3 * sizeof(real32));
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
	Array<Vector3> vertices;
	Array<Vector3> normals;
	Array<Vector2> texcoords;
	SimpleArray<OBJFace> faces;
	String material;
};

class OBJModelReader
{
public:
	OBJModelReader();
	virtual ~OBJModelReader();

	virtual void LoadModel(RTScene* scene, const String& source);

protected:
	void ReadOBJ(Stream* stream);
	void ReadMTL(Stream* stream);

protected:
	SimpleArray<OBJElement> _Elements;
	SimpleArray<OBJMaterial> _Materials;
};

}

#endif 
