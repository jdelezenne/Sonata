/*=============================================================================
ShaderMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ShaderMaterial.h"

namespace SonataEngine
{

ShaderMaterial::ShaderMaterial() :
	NamedObject()
{
}

ShaderMaterial::~ShaderMaterial()
{
}

bool ShaderMaterial::SupportsStencilPass() const
{
	return false;
}

bool ShaderMaterial::SupportsZPass() const
{
	return false;
}

}
