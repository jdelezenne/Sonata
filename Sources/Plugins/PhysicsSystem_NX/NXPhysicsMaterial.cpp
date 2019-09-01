/*=============================================================================
NXPhysicsMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NXPhysicsMaterial.h"

namespace SE_NX
{

NXPhysicsMaterial::NXPhysicsMaterial(NXPhysicsScene* scene) :
	Physics::IMaterial()
{
	_NxMaterialIndex = 0;
	_Scene = scene;

	NxMaterialDesc materialDesc;
	materialDesc.restitution = 0.0f;
	materialDesc.staticFriction	= 0.0f;
	materialDesc.dynamicFriction = 0.0f;
	_NxMaterialIndex = _Scene->GetNxScene()->createMaterial(materialDesc)->getMaterialIndex();
}

NXPhysicsMaterial::~NXPhysicsMaterial()
{
	_Scene->GetNxScene()->releaseMaterial(*_Scene->GetNxScene()->getMaterialFromIndex(_NxMaterialIndex));
}

void NXPhysicsMaterial::Apply()
{
	NxMaterial* material = _Scene->GetNxScene()->getMaterialFromIndex(_NxMaterialIndex);

	material->setDynamicFriction(_DynamicFriction);
	material->setStaticFriction(_StaticFriction);
	material->setRestitution(_Restitution);

	if (!_FrictionEnabled)
		material->setFlags(NX_MF_DISABLE_FRICTION);
}

}
