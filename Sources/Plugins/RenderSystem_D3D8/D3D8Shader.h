/*=============================================================================
D3D8Shader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8SHADER_H_
#define _SE_D3D8SHADER_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

class D3D8Shader;

/** Direct3D8 shader effect variable. */
class D3D8ShaderVariable : public ShaderObject
{
public:
	D3D8ShaderVariable(const D3D8ShaderMaterial* shader, LPCSTR handle);
	virtual ~D3D8ShaderVariable();

	LPCSTR GetHandle() const { return _handle; }

protected:
	const D3D8ShaderMaterial* _shader;
	LPCSTR _handle;
};

/** Direct3D8 shader effect pass. */
class D3D8ShaderPass : public D3D8ShaderVariable, public IShaderPass
{
public:
	D3D8ShaderPass(const D3D8ShaderMaterial* shader, LPCSTR handle);
	virtual ~D3D8ShaderPass();

	virtual String GetName() const { return ShaderObject::GetName(); }
	virtual void SetName(const String& value) { ShaderObject::SetName(value); }
	virtual int GetAnnotationCount() const { return ShaderObject::GetAnnotationCount(); }
	virtual int GetAnnotationIndex(ShaderAnnotation* annotation) const { return ShaderObject::GetAnnotationIndex(annotation); }
	virtual ShaderAnnotation* GetAnnotationByIndex(int index) const { return ShaderObject::GetAnnotationByIndex(index); }
	virtual ShaderAnnotation* GetAnnotationByName(const String& name) const { return ShaderObject::GetAnnotationByName(name); }
	virtual bool SetAnnotation(int index, ShaderAnnotation* annotation) { return ShaderObject::SetAnnotation(index, annotation); }

	virtual int GetSamplerStateCount() const;
	virtual SamplerState* GetSamplerStateByIndex(int index) const;
	virtual SamplerState* GetSamplerStateByName(const String& name) const;
	virtual bool SetSamplerState(int index, SamplerState* sampler);
	//virtual bool SetSamplerState(const String& name, SamplerState* sampler);

	virtual int GetTextureStateCount() const;
	virtual TextureState* GetTextureStateByIndex(int index) const;
	virtual TextureState* GetTextureStateByName(const String& name) const;
	virtual bool SetTextureState(int index, TextureState* texture);
	//virtual bool SetTextureState(const String& name, TextureState* texture);

	virtual ShaderProgram* GetVertexProgram() const;
	virtual void SetVertexProgram(ShaderProgram* value);

	virtual ShaderProgram* GetPixelProgram() const;
	virtual void SetPixelProgram(ShaderProgram* value);

	virtual ShaderProgram* GetGeometryProgram() const;
	virtual void SetGeometryProgram(ShaderProgram* value);

protected:
};

/** Direct3D8 shader effect technique. */
class D3D8ShaderTechnique : public D3D8ShaderVariable, public IShaderTechnique
{
public:
	typedef Dictionary<String, IShaderPass*> ShaderPassHash;

	D3D8ShaderTechnique(const D3D8ShaderMaterial* shader, LPCSTR handle);
	virtual ~D3D8ShaderTechnique();

	virtual String GetName() const { return ShaderObject::GetName(); }
	virtual void SetName(const String& value) { ShaderObject::SetName(value); }
	virtual int GetAnnotationCount() const { return ShaderObject::GetAnnotationCount(); }
	virtual int GetAnnotationIndex(ShaderAnnotation* annotation) const { return ShaderObject::GetAnnotationIndex(annotation); }
	virtual ShaderAnnotation* GetAnnotationByIndex(int index) const { return ShaderObject::GetAnnotationByIndex(index); }
	virtual ShaderAnnotation* GetAnnotationByName(const String& name) const { return ShaderObject::GetAnnotationByName(name); }
	virtual bool SetAnnotation(int index, ShaderAnnotation* annotation) { return ShaderObject::SetAnnotation(index, annotation); }

	virtual int GetPassCount() const;
	virtual int GetPassIndex(IShaderPass* pass) const;
	virtual IShaderPass* GetPassByIndex(int index) const;
	virtual IShaderPass* GetPassByName(const String& name) const;

protected:
	mutable ShaderPassHash _Passes;
};

/** Direct3D8 shader effect. */
class D3D8Shader : public RefObject, public IShader
{
public:
	typedef Dictionary<String, IShaderTechnique*> ShaderTechniqueHash;

	D3D8Shader(ID3DXEffect* pD3DEffect);
	virtual ~D3D8Shader();

	virtual String GetName() const { return _name; }

	virtual void SetName(const String& value) { _name = value; }

	virtual IShaderTechnique* GetCurrentTechnique() const;

	virtual void SetCurrentTechnique(IShaderTechnique* value);

	virtual int GetTechniqueCount() const;
	virtual int GetTechniqueIndex(IShaderTechnique* technique) const;
	virtual IShaderTechnique* GetTechniqueByIndex(int index) const;
	virtual IShaderTechnique* GetTechniqueByName(const String& name) const;

	ID3DXEffect* GetD3DXEffect() const { return _D3DXEffect; }

protected:
	ID3DXEffect* _D3DXEffect;

	String _name;
	mutable ShaderTechniqueHash _Techniques;
};

}

#endif
