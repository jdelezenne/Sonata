/*=============================================================================
LightState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LIGHTSTATE_H_
#define _SE_LIGHTSTATE_H_

#include "Graphics/Common.h"
#include "Graphics/Lighting/Light.h"

namespace SonataEngine
{

struct LightSource
{
	LightSource();

	LightType LightType;
	bool IsEnabled;
	Color32 AmbientColor;
	Color32 DiffuseColor;
	Color32 SpecularColor;
	Vector3 Position;
	Vector3 Direction;
	real32 Range;
	real32 ConstantAttenuation;
	real32 LinearAttenuation;
	real32 QuadraticAttenuation;
	real32 InnerAngle;
	real32 OuterAngle;
	real32 FalloffExponent;
};

SE_DECLARE_STRUCT(LightSource);
SE_BEGIN_STRUCT(LightSource);
	SE_StructField(IsEnabled, bool);
	SE_StructField(AmbientColor, Color32);
	SE_StructField(DiffuseColor, Color32);
	SE_StructField(SpecularColor, Color32);
	SE_StructField(Position, Vector3);
	SE_StructField(Direction, Vector3);
	SE_StructField(Range, real32);
	SE_StructField(ConstantAttenuation, real32);
	SE_StructField(LinearAttenuation, real32);
	SE_StructField(QuadraticAttenuation, real32);
	SE_StructField(InnerAngle, real32);
	SE_StructField(OuterAngle, real32);
	SE_StructField(FalloffExponent, real32);
SE_END_STRUCT(LightSource);


/**
	Light state.
*/
class SE_GRAPHICS_EXPORT LightState
{
public:
	typedef BaseArray<LightSource> LightList;

	/// Enables or disables lighting.
	bool Lighting;

	/// Enables or disables automatic normalization of vertex normals.
	bool NormalizeNormals;

	/// Light sources.
	LightList Lights;

public:
	/** @name Constructor. */
	//@{
	LightState();
	//@}
};

SE_DECLARE_STRUCT(LightState);
SE_BEGIN_STRUCT(LightState);
	SE_StructField(Lighting, bool);
	SE_StructField(NormalizeNormals, bool);
SE_END_STRUCT(LightState);

}

#endif
