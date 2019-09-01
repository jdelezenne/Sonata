/*=============================================================================
EmperorModelReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EMPERORMODELREADER_H_
#define _SE_EMPERORMODELREADER_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_Emperor
{

struct EmperorObject
{
	int32 vertex_count;
	int32 face_count;
	int32 object_flags;
	int32 child_count;
	BaseArray<EmperorObject> children;
	real64 matrix[16];
	int32 name_length;
	SEchar* name;

	Array<Vector3> vertices;
	Array<Vector3> normals;

	Array<int32> indices;
	int32 material_index;
	int32 unknown;
	Array<Vector2> face_coordinates;
};

class EmperorModelReader : public ModelReader
{
public:
	EmperorModelReader();
	virtual ~EmperorModelReader();

	virtual Model* LoadModel(Stream& stream, ModelReaderOptions* options = NULL);

protected:
	void ReadXAF(Stream& stream);
	void ReadXBF(Stream& stream);

	bool ReadObject(Stream& stream, EmperorObject& obj);
	void ConvertObject(MeshFrame* parent, const EmperorObject& obj);

protected:
	Model* _Model;
	Array<ShaderMaterial*> _shaders;
};

}

#endif 
