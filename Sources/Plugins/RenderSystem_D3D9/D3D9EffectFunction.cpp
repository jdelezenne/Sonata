/*=============================================================================
D3D9EffectFunction.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9EffectFunction.h"

namespace SE_D3D9
{

D3D9EffectFunction::D3D9EffectFunction(D3D9EffectShader* effect, D3DXHANDLE handle) :
	D3D9EffectVariable(effect, handle)
{
	Initialize();
}

D3D9EffectFunction::~D3D9EffectFunction()
{
}

void D3D9EffectFunction::Initialize()
{
	ID3DXEffect* effect = _effect->GetD3DXEffect();
	if (effect == NULL)
		return;

	D3DXFUNCTION_DESC desc;
	effect->GetFunctionDesc(_handle, &desc);
	_name = desc.Name;
}

}
