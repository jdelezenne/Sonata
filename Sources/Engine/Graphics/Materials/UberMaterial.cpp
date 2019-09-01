/*=============================================================================
UberMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "UberMaterial.h"

namespace SonataEngine
{

UberMaterial::UberMaterial() :
	EffectMaterial()
{
}

UberMaterial::~UberMaterial()
{
}

void UberMaterial::Initialize()
{
	EffectMaterial::Initialize();

	if (_effectShader == NULL)
	{
		return;
	}
}

void UberMaterial::SetupMaterial(SceneState* sceneState)
{
	EffectMaterial::SetupMaterial(sceneState);
}

void UberMaterial::SetupGeometry(MeshPart* meshPart)
{
	EffectMaterial::SetupGeometry(meshPart);
}

void UberMaterial::SetupPass(SceneState* sceneState, MeshPart* meshPart)
{
	EffectMaterial::SetupPass(sceneState, meshPart);
}

}
