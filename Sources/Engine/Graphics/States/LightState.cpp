/*=============================================================================
LightState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LightState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(LightSource);

LightSource::LightSource() :
	LightType(LightType_Point),
	IsEnabled(true),
	AmbientColor(Color32::Black),
	DiffuseColor(Color32::White),
	SpecularColor(Color32::White),
	Position(Vector3::Zero),
	Direction(-Vector3::UnitZ),
	Range(SE_MAX_R32),
	ConstantAttenuation(1.0f),
	LinearAttenuation(0.0f),
	QuadraticAttenuation(0.0f),
	InnerAngle(30.0f),
	OuterAngle(40.0f),
	FalloffExponent(180.0f)
{
}


SE_IMPLEMENT_STRUCT(LightState);

LightState::LightState() :
	Lighting(false),
	NormalizeNormals(true),
	Lights(0)
{
}

}
