/*=============================================================================
D3D8Shader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8Shader.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SE_D3D8
{

D3D8ShaderVariable::D3D8ShaderVariable(const D3D8ShaderMaterial* shader, LPCSTR handle) :
	_shader(shader),
	_handle(handle)
{
}

D3D8ShaderVariable::~D3D8ShaderVariable()
{
}


D3D8ShaderPass::D3D8ShaderPass(const D3D8ShaderMaterial* shader, LPCSTR handle) :
	D3D8ShaderVariable(shader, handle)
{
}

D3D8ShaderPass::~D3D8ShaderPass()
{
}

int D3D8ShaderPass::GetSamplerStateCount() const
{
	return 0;
}

SamplerState* D3D8ShaderPass::GetSamplerStateByIndex(int index) const
{
	return NULL;
}

SamplerState* D3D8ShaderPass::GetSamplerStateByName(const String& name) const
{
	return NULL;
}

bool D3D8ShaderPass::SetSamplerState(int index, SamplerState* sampler)
{
	return false;
}

int D3D8ShaderPass::GetTextureStateCount() const
{
	return 0;
}

TextureState* D3D8ShaderPass::GetTextureStateByIndex(int index) const
{
	return NULL;
}

TextureState* D3D8ShaderPass::GetTextureStateByName(const String& name) const
{
	return NULL;
}

bool D3D8ShaderPass::SetTextureState(int index, TextureState* texture)
{
	return false;
}

ShaderProgram* D3D8ShaderPass::GetVertexProgram() const
{
	return NULL;
}

void D3D8ShaderPass::SetVertexProgram(ShaderProgram* value)
{
}

ShaderProgram* D3D8ShaderPass::GetPixelProgram() const
{
	return NULL;
}

void D3D8ShaderPass::SetPixelProgram(ShaderProgram* value)
{
}

ShaderProgram* D3D8ShaderPass::GetGeometryProgram() const
{
	return NULL;
}

void D3D8ShaderPass::SetGeometryProgram(ShaderProgram* value)
{
}


D3D8ShaderTechnique::D3D8ShaderTechnique(const D3D8ShaderMaterial* shader, LPCSTR handle) :
	D3D8ShaderVariable(shader, handle)
{
}

D3D8ShaderTechnique::~D3D8ShaderTechnique()
{
}

int D3D8ShaderTechnique::GetPassCount() const
{
	D3DXTECHNIQUE_DESC desc;
	_shader->GetD3DXEffect()->GetTechniqueDesc(_handle, &desc);
	return desc.Passes;
}

int D3D8ShaderTechnique::GetPassIndex(IShaderPass* pass) const
{
	if (pass == NULL)
	{
		SEthrow(ArgumentNullException("pass"));
		return -1;
	}
	else
	{
		if (_Passes.Contains(pass->GetName()))
		{
			D3DXPASS_DESC d3dxDesc;
			HRESULT hr = _shader->GetD3DXEffect()->GetPassDesc(
				_handle, pass->GetName().Data(), &d3dxDesc);

			if (FAILED(hr))
				return -1;
			else
				return String(d3dxDesc.Index).ToInt32();
		}

		return -1;
	}
}

IShaderPass* D3D8ShaderTechnique::GetPassByIndex(int index) const
{
	return NULL;
}

IShaderPass* D3D8ShaderTechnique::GetPassByName(const String& name) const
{
	if (_Passes.Contains(name))
	{
		return _Passes[name];
	}

	D3DXPASS_DESC d3dxDesc;
	HRESULT hr = _shader->GetD3DXEffect()->GetPassDesc(_handle, name.Data(), &d3dxDesc);
	if (FAILED(hr))
		return NULL;

	D3D8ShaderPass* pass = new D3D8ShaderPass(_shader, d3dxDesc.Name);
	_Passes[name] = pass;
	return pass;
}


D3D8Shader::D3D8Shader(ID3DXEffect* pD3DEffect) :
	RefObject(),
	_D3DXEffect(pD3DEffect)
{
}

D3D8Shader::~D3D8Shader()
{
	ShaderTechniqueHash::DictionaryIterator it = _Techniques.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

IShaderTechnique* D3D8Shader::GetCurrentTechnique() const
{
	CHAR hTechnique[512];
	HRESULT hr = _D3DXEffect->GetTechnique((LPCSTR*)&hTechnique);
	if (FAILED(hr))
		return NULL;

	D3DXTECHNIQUE_DESC desc;
	_D3DXEffect->GetTechniqueDesc(hTechnique, &desc);
	
	if (_Techniques.Contains(desc.Name))
	{
		return _Techniques[desc.Name];
	}

	D3D8ShaderTechnique* technique = new D3D8ShaderTechnique(this, hTechnique);
	_Techniques[desc.Name] = technique;
	return technique;
}

void D3D8Shader::SetCurrentTechnique(IShaderTechnique* value)
{
	if (_Techniques.Contains(value->GetName()))
	{
		D3D8ShaderVariable* variable = (D3D8ShaderVariable*)_Techniques[value->GetName()];
		_D3DXEffect->SetTechnique(variable->GetHandle());
	}
}

int D3D8Shader::GetTechniqueCount() const
{
	D3DXEFFECT_DESC desc;
	_D3DXEffect->GetDesc(&desc);
	return desc.Techniques;
}

int D3D8Shader::GetTechniqueIndex(IShaderTechnique* technique) const
{
	if (technique == NULL)
	{
		SEthrow(ArgumentNullException("technique"));
		return -1;
	}
	else
	{
		if (_Techniques.Contains(technique->GetName()))
		{
			D3DXTECHNIQUE_DESC d3dxDesc;
			HRESULT hr = _D3DXEffect->GetTechniqueDesc(
				technique->GetName().Data(), &d3dxDesc);

			if (FAILED(hr))
				return -1;
			else
				return String(d3dxDesc.Index).ToInt32();
		}

		return -1;
	}
}

IShaderTechnique* D3D8Shader::GetTechniqueByIndex(int index) const
{
	return NULL;
}

IShaderTechnique* D3D8Shader::GetTechniqueByName(const String& name) const
{
	if (_Techniques.Contains(name))
	{
		return _Techniques[name];
	}

	D3DXTECHNIQUE_DESC d3dxDesc;
	HRESULT hr = _D3DXEffect->GetTechniqueDesc(name.Data(), &d3dxDesc);
	if (FAILED(hr))
		return NULL;

	D3D8ShaderTechnique* technique = new D3D8ShaderTechnique(this, d3dxDesc.Name);
	_Techniques[name] = technique;
	return technique;
}

}
