/*=============================================================================
DefaultMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DefaultMaterial.h"
#include "Graphics/SceneManager.h"
#include "Graphics/System/RenderSystem.h"

namespace SonataEngine
{

DefaultMaterial::DefaultMaterial() :
	ShaderMaterial(),
	_effectShader(NULL),
	_defaultLight(true)
{
	Initialize();
}

DefaultMaterial::~DefaultMaterial()
{
}

void DefaultMaterial::Initialize()
{
	if (_effectShader != NULL)
	{
		return;
	}

	_effectShader = new FFPEffectShader();
	FFPTechnique* technique = new FFPTechnique();
	FFPPass* pass = new FFPPass();

	SamplerState* samplerState = new SamplerState();
	pass->AddSamplerState(samplerState);

	TextureState* textureState = new TextureState();
	pass->AddTextureState(textureState);

	technique->AddPass(pass);
	_effectShader->AddTechnique(technique);

	_activeTechnique = technique;
}

void DefaultMaterial::SetupMaterial(SceneState* sceneState)
{
	RenderSystem* renderSystem = RenderSystem::Current();
	renderSystem->SetProjectionTransform(sceneState->Projection);
	renderSystem->SetViewTransform(sceneState->View);

	renderSystem->SetAmbientColor(sceneState->Scene->GetAmbientColor());
	renderSystem->SetScissorState(sceneState->Scene->GetScissorState());
	renderSystem->SetFogState(sceneState->Scene->GetFogState());
}

void DefaultMaterial::SetupGeometry(MeshPart* meshPart)
{
}

void DefaultMaterial::SetupPass(SceneState* sceneState, MeshPart* meshPart)
{
	RenderSystem* renderSystem = RenderSystem::Current();
	renderSystem->SetWorldTransform(sceneState->World);

	FFPPass* pass = (FFPPass*)_activeTechnique->GetPassByIndex(0);
	bool needCameraLight = (pass->LightState.Lighting && sceneState->Camera != NULL);

	if (_defaultLight && needCameraLight)
	{
		if (pass->LightState.Lights.Count() == 0)
		{
			pass->LightState.Lights.Add(LightSource());
		}
		LightSource cameraLight;
		cameraLight.LightType = LightType_Directional;
		cameraLight.Direction = -Vector3::Normalize(sceneState->Camera->GetWorldPosition());
		cameraLight.ConstantAttenuation = 0.2f;
		pass->LightState.Lights.SetItem(0, cameraLight);
	}
}

void DefaultMaterial::BeginMaterial() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_effectShader->BeginTechnique(_activeTechnique);
	}
}

void DefaultMaterial::EndMaterial() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_effectShader->EndTechnique();
	}
}

int DefaultMaterial::GetPassCount() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		return _activeTechnique->GetPassCount();
	}
	else
	{
		return 0;
	}
}

void DefaultMaterial::BeginPass(int index) const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_activeTechnique->BeginPassFromIndex(index);
	}
}

void DefaultMaterial::EndPass() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_activeTechnique->EndPass();
	}
}

}
