/*=============================================================================
ShaderParameterDesc.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ShaderParameterDesc.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(ShaderParameterDesc);
SE_IMPLEMENT_REFLECTION(ShaderParameterDesc);

ShaderParameterDesc::ShaderParameterDesc() :
	Object(),
	Bytes(0),
	StructMembers(0),
	Elements(0),
	Columns(0),
	Rows(0),
	ParameterType(ShaderParameterType_Void),
	ParameterClass(ShaderParameterClass_Object)
{
}

ShaderParameterDesc::~ShaderParameterDesc()
{
}

}
