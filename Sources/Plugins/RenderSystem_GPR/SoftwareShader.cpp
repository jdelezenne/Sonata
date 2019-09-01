/*=============================================================================
SoftwareShader.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "SoftwareShader.h"
#include "SoftwareRenderer.h"

OGPRVertexFuncEnv* SoftwareShader::_VertexEnv = NULL;
OGPRFragmentFuncEnv* SoftwareShader::_FragmentEnv = NULL;

SoftwareShader::SoftwareShader() :
	Shader()
{
	VertexSM = new bool[16];
	FragmentSM = new bool[16];
}

SoftwareShader::~SoftwareShader()
{
	SAFE_DELETE_ARRAY(VertexSM);
	SAFE_DELETE_ARRAY(FragmentSM);
}

void SoftwareShader::Initialize(Renderer* renderer, Mesh* mesh)
{
	States = ((SoftwareRenderer*) renderer)->States;

	memset(VertexSM, 0, 16 * sizeof(bool));
	memset(FragmentSM, 0, 16 * sizeof(bool));
}

void SoftwareShader::inStream(uint32 stream, void* s)
{
	if (VertexSM[stream])
		SoftwareShader::GetVertexEnvironment()->ReadStream(stream, s);
}

Colour32 tex1D(uint32 tex, real32 s)
{
	Colour32 colour;
	if (FAILED(SoftwareShader::GetFragmentEnvironment()->SampleBuffer(
		DBT_Unused + tex, 1, (real32*)&s, (real32*)&colour)))
	{
		return Colour32::Black;
	}

	return colour / 255.0f;
}

Colour32 tex2D(uint32 tex, const Vector2& s)
{
	Colour32 colour;
	if (FAILED(SoftwareShader::GetFragmentEnvironment()->SampleBuffer(
		DBT_Unused + tex, 2, (real32*)&s, (real32*)&colour)))
	{
		return Colour32::Black;
	}

	return colour / 255.0f;
}

Colour32 tex3D(uint32 tex, const Vector3& s)
{
	Colour32 colour;
	if (FAILED(SoftwareShader::GetFragmentEnvironment()->SampleBuffer(
		DBT_Unused + tex, 3, (real32*)&s, (real32*)&colour)))
	{
		return Colour32::Black;
	}

	return colour / 255.0f;
}

Colour32 texCUBE(uint32 tex, const Vector3& s)
{
	Colour32 colour;
	if (FAILED(SoftwareShader::GetFragmentEnvironment()->SampleBuffer(
		DBT_Unused + tex, 3, (real32*)&s, (real32*)&colour)))
	{
		return Colour32::Black;
	}

	return colour / 255.0f;
}

Colour32 texRECT(uint32 tex, const Vector2& s)
{
	Colour32 colour;
	if (FAILED(SoftwareShader::GetFragmentEnvironment()->SampleBuffer(
		DBT_Unused + tex, 2, (real32*)&s, (real32*)&colour)))
	{
		return Colour32::Black;
	}

	return colour / 255.0f;
}
