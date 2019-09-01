/*=============================================================================
FFPTechnique.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FFPTechnique.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/Shader/ShaderSystem.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(FFPTechnique);
SE_IMPLEMENT_REFLECTION(FFPTechnique);

FFPTechnique::FFPTechnique() :
	EffectTechnique(),
	_activePass(NULL)
{
}

FFPTechnique::~FFPTechnique()
{
	PassList::Iterator it = _passes.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

int FFPTechnique::GetPassCount() const
{
	return _passes.Count();
}

EffectPass* FFPTechnique::GetPassByIndex(int index) const
{
	return _passes[index];
}

EffectPass* FFPTechnique::GetPassByName(const String& name) const
{
	PassList::Iterator it = _passes.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
		{
			return it.Current();
		}
	}

	return NULL;
}

int FFPTechnique::GetAnnotationCount() const
{
	return 0;
}

EffectAnnotation* FFPTechnique::GetAnnotationByIndex(int index) const
{
	return NULL;
}

EffectAnnotation* FFPTechnique::GetAnnotationByName(const String& name) const
{
	return NULL;
}

bool FFPTechnique::AddPass(FFPPass* pass)
{
	if (pass == NULL)
	{
		throw ArgumentNullException("pass");
		return false;
	}

	_passes.Add(pass);
	return true;
}

bool FFPTechnique::RemovePass(FFPPass* pass)
{
	if (pass == NULL)
	{
		throw ArgumentNullException("pass");
		return false;
	}

	_passes.Remove(pass);
	return true;
}

void FFPTechnique::RemoveAllPasses()
{
	_passes.Clear();
}

void FFPTechnique::BeginPass(EffectPass* pass)
{
	if (_activePass != NULL)
	{
		return;
	}

	RenderSystem* renderSystem = RenderSystem::Current();
	ShaderSystem* shaderSystem = ShaderSystem::Current();

	FFPPass* activePass = (FFPPass*)pass;

	renderSystem->SetFillMode(activePass->RasterizerState.FillMode);
	renderSystem->SetShadeMode(activePass->ShadeMode);
	renderSystem->SetCullMode(activePass->RasterizerState.CullMode);

	renderSystem->SetAlphaState(activePass->AlphaState);
	renderSystem->SetColorWriteEnable(activePass->AlphaState.RenderTargetWriteMask[0]);
	renderSystem->SetDepthState(activePass->DepthState);
	renderSystem->SetStencilState(activePass->StencilState);
	renderSystem->SetScissorState(activePass->ScissorState);
	renderSystem->SetDithering(activePass->Dithering);

	if (shaderSystem != NULL)
	{
		if (activePass->GetVertexProgram() != NULL)
		{
			shaderSystem->SetShaderProgram(activePass->GetVertexProgram());
		}
		if (activePass->GetPixelProgram() != NULL)
		{
			shaderSystem->SetShaderProgram(activePass->GetPixelProgram());
		}
		if (activePass->GetGeometryProgram() != NULL)
		{
			shaderSystem->SetShaderProgram(activePass->GetGeometryProgram());
		}
	}

	int stage;
	int stageCount = activePass->GetSamplerStateCount();
	for (stage = 0; stage < stageCount; stage++)
	{
		renderSystem->SetSamplerState(stage, *activePass->GetSamplerStateByIndex(stage));
	}

	int remaining = SE_MAX_TEXTURE_STAGES - stageCount;
	for (stage = stageCount; stage < remaining; stage++)
	{
		renderSystem->DisableSamplerState(stage);
	}

	renderSystem->SetLightState(activePass->LightState);

	renderSystem->SetMaterialState(activePass->MaterialState);

	stageCount = activePass->GetTextureStateCount();
	for (stage = 0; stage < stageCount; stage++)
	{
		renderSystem->SetTextureState(stage, *activePass->GetTextureStateByIndex(stage));
	}

	_activePass = (FFPPass*)pass;
}

void FFPTechnique::BeginPassFromIndex(int index)
{
	BeginPass(_passes[index]);
}

EffectPass* FFPTechnique::GetActivePass()
{
	return _activePass;
}

void FFPTechnique::EndPass()
{
	if (_activePass == NULL)
	{
		return;
	}

	RenderSystem* renderSystem = RenderSystem::Current();
	ShaderSystem* shaderSystem = ShaderSystem::Current();

	renderSystem->SetFillMode(FillMode_Solid);
	renderSystem->SetShadeMode(ShadeMode_Smooth);
	renderSystem->SetCullMode(CullMode_Back);

	renderSystem->SetAlphaState(AlphaState());
	renderSystem->SetColorWriteEnable(ColorFlag_All);
	renderSystem->SetDepthState(DepthState());
	renderSystem->SetStencilState(StencilState());
	renderSystem->SetScissorState(ScissorState());
	renderSystem->SetDithering(false);

	if (shaderSystem != NULL)
	{
		if (_activePass->GetVertexProgram() != NULL)
		{
			shaderSystem->DisableShaderProgram(_activePass->GetVertexProgram());
		}
		if (_activePass->GetPixelProgram() != NULL)
		{
			shaderSystem->DisableShaderProgram(_activePass->GetPixelProgram());
		}
		if (_activePass->GetGeometryProgram() != NULL)
		{
			shaderSystem->DisableShaderProgram(_activePass->GetGeometryProgram());
		}
	}

	for (int stage = 0; stage < SE_MAX_TEXTURE_STAGES; stage++)
	{
		renderSystem->DisableSamplerState(stage);
	}

	renderSystem->SetLightState(LightState());

	renderSystem->SetMaterialState(MaterialState());

	for (stage = 0; stage < SE_MAX_TEXTURE_STAGES; stage++)
	{
		renderSystem->SetTextureState(stage, TextureState());
	}

	_activePass = NULL;
}

}
