/*=============================================================================
EffectMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTMATERIAL_H_
#define _SE_EFFECTMATERIAL_H_

#include "Graphics/Common.h"
#include "Graphics/Materials/ShaderMaterial.h"
#include "Graphics/Shader/EffectShader.h"

namespace SonataEngine
{

/**
	@brief Effect Material.
*/
class SE_GRAPHICS_EXPORT EffectMaterial : public ShaderMaterial
{
protected:
	EffectShader* _effectShader;
	String _techniqueName;
	EffectTechnique* _activeTechnique;

public:
	EffectMaterial();
	virtual ~EffectMaterial();

	/** @name Properties. */
	//@{
	EffectShader* GetEffectShader() const { return _effectShader; }
	void SetEffectShader(EffectShader* value);

	String GetTechnique() const { return _techniqueName; }
	void SetTechnique(EffectTechnique* value);
	void SetTechnique(const String& value);
	//@}

	virtual void Initialize();
	virtual void SetupMaterial(SceneState* sceneState);
	virtual void SetupGeometry(MeshPart* meshPart);
	virtual void SetupPass(SceneState* sceneState, MeshPart* meshPart);

	virtual void BeginMaterial() const;
	virtual void EndMaterial() const;

	virtual int GetPassCount() const;
	virtual void BeginPass(int index) const;
	virtual void EndPass() const;
};

SEPointer(EffectMaterial);

}

#endif
