/*=============================================================================
FFPPass.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FFPPass.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(FFPPass);
SE_IMPLEMENT_REFLECTION(FFPPass);

FFPPass::FFPPass() :
	EffectPass(),
	ShadeMode(ShadeMode_Smooth),
	Dithering(false),
	_vertexProgram(NULL),
	_pixelProgram(NULL),
	_geometryProgram(NULL)
{
}

FFPPass::~FFPPass()
{
	{
		ShaderStateList::Iterator it = _shaderStates.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
	{
		SamplerStateList::Iterator it = _samplerStates.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
	{
		TextureStateList::Iterator it = _textureStates.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
}

int FFPPass::GetAnnotationCount() const
{
	return 0;
}

EffectAnnotation* FFPPass::GetAnnotationByIndex(int index) const
{
	return NULL;
}

EffectAnnotation* FFPPass::GetAnnotationByName(const String& name) const
{
	return NULL;
}

int FFPPass::GetStateCount() const
{
	return _shaderStates.Count();
}

bool FFPPass::AddState(ShaderState* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_shaderStates.Add(value);
	return true;
}

bool FFPPass::RemoveState(ShaderState* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_shaderStates.Remove(value);
	return true;
}

void FFPPass::RemoveAllStates()
{
	_shaderStates.Clear();
}

ShaderState* FFPPass::GetStateByIndex(int index) const
{
	return _shaderStates[index];
}

ShaderState* FFPPass::GetStateByName(const String& name) const
{
	ShaderStateList::Iterator it = _shaderStates.GetIterator();
	while (it.Next())
	{
		if (String::Compare(it.Current()->GetState(), name, true) == 0)
		{
			return it.Current();
		}
	}

	return NULL;
}

int FFPPass::GetSamplerStateCount() const
{
	return _samplerStates.Count();
}

bool FFPPass::AddSamplerState(SamplerState* value)
{
	if (value == NULL)
	{
		throw ArgumentNullException("value");
		return false;
	}

	_samplerStates.Add(value);
	return true;
}

bool FFPPass::RemoveSamplerState(SamplerState* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_samplerStates.Remove(value);
	return true;
}

void FFPPass::RemoveAllSamplerStates()
{
	_samplerStates.Clear();
}

SamplerState* FFPPass::GetSamplerStateByIndex(int index) const
{
	return _samplerStates[index];
}

SamplerState* FFPPass::GetSamplerStateByName(const String& name) const
{
	SamplerStateList::Iterator it = _samplerStates.GetIterator();
	while (it.Next())
	{
		if (String::Compare(it.Current()->Name, name) == 0)
		{
			return it.Current();
		}
	}

	return NULL;
}

int FFPPass::GetTextureStateCount() const
{
	return _textureStates.Count();
}

bool FFPPass::AddTextureState(TextureState* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_textureStates.Add(value);
	return true;
}

bool FFPPass::RemoveTextureState(TextureState* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_textureStates.Remove(value);
	return true;
}

void FFPPass::RemoveAllTextureStates()
{
	_textureStates.Clear();
}

TextureState* FFPPass::GetTextureStateByIndex(int index) const
{
	return _textureStates[index];
}

TextureState* FFPPass::GetTextureStateByName(const String& name) const
{
	TextureStateList::Iterator it = _textureStates.GetIterator();
	while (it.Next())
	{
		if (String::Compare(it.Current()->Name, name) == 0)
		{
			return it.Current();
		}
	}

	return NULL;
}

}
