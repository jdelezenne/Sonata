/*=============================================================================
FFPPass.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FFPPASS_H_
#define _SE_FFPPASS_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectPass.h"
#include "Graphics/Shader/ShaderState.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/States/States.h"

namespace SonataEngine
{

/**
	@brief Fixed Function Pipeline shader pass.
*/
class SE_GRAPHICS_EXPORT FFPPass : public EffectPass
{
	SE_DECLARE_CLASS(FFPPass, NamedObject);
	SE_BEGIN_REFLECTION(FFPPass);
		SE_Field(RasterizerState, RasterizerState, Public);
		SE_Field(ShadeMode, ShadeMode, Public);
		SE_Field(DepthState, DepthState, Public);
		SE_Field(StencilState, StencilState, Public);
		SE_Field(ScissorState, ScissorState, Public);
		SE_Field(Dithering, MaterialState, Public);
		SE_Field(AlphaState, AlphaState, Public);
		SE_Field(MaterialState, MaterialState, Public);
		SE_Field(LightState, LightState, Public);
	SE_END_REFLECTION(FFPPass);

public:
	typedef Array<ShaderState*> ShaderStateList;
	typedef Array<SamplerState*> SamplerStateList;
	typedef Array<TextureState*> TextureStateList;

protected:
	ShaderStateList _shaderStates;
	SamplerStateList _samplerStates;
	TextureStateList _textureStates;
	ShaderProgram* _vertexProgram;
	ShaderProgram* _pixelProgram;
	ShaderProgram* _geometryProgram;

public:
	/// Rasterizer State
	RasterizerState RasterizerState;

	/// Shading mode.
	ShadeMode ShadeMode;

	/// Depth State
	DepthState DepthState;

	/// Stencil State
	StencilState StencilState;

	/// Scissor State
	ScissorState ScissorState;

	/// Alpha State
	AlphaState AlphaState;

	/// Dithering.
	bool Dithering;

	/// Material State
	MaterialState MaterialState;

	/// Light State
	LightState LightState;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	FFPPass();

	/** Destructor. */
	virtual ~FFPPass();
	//@}

	virtual ShaderProgram* GetVertexProgram() const { return _vertexProgram; }
	virtual ShaderProgram* GetPixelProgram() const { return _pixelProgram; }
	virtual ShaderProgram* GetGeometryProgram() const { return _geometryProgram; }

	virtual int GetAnnotationCount() const;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const;

	/** @name Shader Programs. */
	//@{
	void SetVertexProgram(ShaderProgram* value) { _vertexProgram = value; }
	void SetPixelProgram(ShaderProgram* value) { _pixelProgram = value; }
	void SetGeometryProgram(ShaderProgram* value) { _geometryProgram = value; }
	//@}

	/** @name Shader States. */
	//@{
	int GetStateCount() const;
	bool AddState(ShaderState* value);
	bool RemoveState(ShaderState* value);
	void RemoveAllStates();
	ShaderState* GetStateByIndex(int index) const;
	ShaderState* GetStateByName(const String& name) const;
	//@}

	/** @name Sampler States. */
	//@{
	int GetSamplerStateCount() const;
	bool AddSamplerState(SamplerState* value);
	bool RemoveSamplerState(SamplerState* value);
	void RemoveAllSamplerStates();
	SamplerState* GetSamplerStateByIndex(int index) const;
	SamplerState* GetSamplerStateByName(const String& name) const;
	//@}

	/** @name Texture States. */
	//@{
	int GetTextureStateCount() const;
	bool AddTextureState(TextureState* value);
	bool RemoveTextureState(TextureState* value);
	void RemoveAllTextureStates();
	TextureState* GetTextureStateByIndex(int index) const;
	TextureState* GetTextureStateByName(const String& name) const;
	//@}
};

SEPointer(FFPPass);

}

#endif
