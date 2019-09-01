/*=============================================================================
DefaultMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DEFAULTMATERIAL_H_
#define _SE_DEFAULTMATERIAL_H_

#include "Graphics/Common.h"
#include "Graphics/Materials/ShaderMaterial.h"
#include "Graphics/Shader/FFPEffectShader.h"

namespace SonataEngine
{

/**
	@brief Default Material.
*/
class SE_GRAPHICS_EXPORT DefaultMaterial : public ShaderMaterial
{
protected:
	FFPEffectShader* _effectShader;
	FFPTechnique* _activeTechnique;

	bool _defaultLight;
	DirectionalLight* _cameraLight;

public:
	DefaultMaterial();
	virtual ~DefaultMaterial();

	/** @name Properties. */
	//@{
	FFPTechnique* GetTechnique() const { return _activeTechnique; }

	bool HasDefaultLight() const { return _defaultLight; }
	void SetDefaultLight(bool value) { _defaultLight = value; }
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

SEPointer(DefaultMaterial);

}

#endif
