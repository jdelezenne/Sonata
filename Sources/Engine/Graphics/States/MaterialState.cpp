/*=============================================================================
MaterialState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MaterialState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(MaterialState);

MaterialState::MaterialState() :
	AmbientColor(Color32::White),
	DiffuseColor(Color32::White),
	SpecularColor(Color32::Black),
	EmissiveColor(Color32::Black),
	Shininess(1.0f),
	VertexColor(false)
{
}

MaterialState::MaterialState(const Color32& ambientColor, const Color32& diffuseColor, const Color32& specularColor, const Color32& emissiveColor, real32 shininess) :
	AmbientColor(ambientColor),
	DiffuseColor(diffuseColor),
	SpecularColor(specularColor),
	EmissiveColor(emissiveColor),
	Shininess(shininess),
	VertexColor(false)
{
}

}
