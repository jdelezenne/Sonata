/*=============================================================================
SasEffectMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SASEFFECTMATERIAL_H_
#define _SE_SASEFFECTMATERIAL_H_

#include "Graphics/Common.h"
#include "Graphics/Materials/EffectMaterial.h"

namespace SonataEngine
{

/**
	@brief Sas Effect Material.
	DirectX Standard Annotations and Semantics.
*/
class SE_GRAPHICS_EXPORT SasEffectMaterial : public EffectMaterial
{
protected:
	enum SemanticID
	{
		SemanticID_unknown,
		SemanticID_position,
		SemanticID_direction,
		SemanticID_diffuse,
		SemanticID_specular,
		SemanticID_ambient,
		SemanticID_emissive,
		SemanticID_specularpower,
		SemanticID_refraction,
		SemanticID_opacity,
		SemanticID_environment,
		SemanticID_environmentnormal,
		SemanticID_normal,
		SemanticID_height,
		SemanticID_attenuation,
		SemanticID_rendercolortarget,
		SemanticID_renderdepthstenciltarget,
		SemanticID_viewportpixelsize,
		SemanticID_cameraposition,
		SemanticID_time,
		SemanticID_elapsedtime,
		SemanticID_animationtime,
		SemanticID_animationtick,
		SemanticID_mouseposition,
		SemanticID_leftmousedown,
		SemanticID_resetpulse,
		SemanticID_world,
		SemanticID_view,
		SemanticID_projection,
		SemanticID_worldtranspose,
		SemanticID_viewtranspose,
		SemanticID_projectiontranspose,
		SemanticID_worldview,
		SemanticID_worldviewprojection,
		SemanticID_worldinverse,
		SemanticID_viewinverse,
		SemanticID_projectioninverse,
		SemanticID_worldinversetranspose,
		SemanticID_viewinversetranspose,
		SemanticID_projectioninversetranspose,
		SemanticID_worldviewinverse,
		SemanticID_worldviewtranspose,
		SemanticID_worldviewinversetranspose,
		SemanticID_worldviewprojectioninverse,
		SemanticID_worldviewprojectiontranspose,
		SemanticID_worldviewprojectioninversetranspose,
		SemanticID_viewprojection,
		SemanticID_viewprojectiontranspose,
		SemanticID_viewprojectioninverse,
		SemanticID_viewprojectioninversetranspose,
		SemanticID_max
	};

	struct ParameterInfo
	{
		SemanticID semanticID;
		EffectParameter* handle;
	};

	BaseArray<ParameterInfo> _parameterInfos;

public:
	SasEffectMaterial();
	virtual ~SasEffectMaterial();

	virtual void Initialize();
	virtual void SetupMaterial(SceneState* sceneState);
	virtual void SetupGeometry(MeshPart* meshPart);
	virtual void SetupPass(SceneState* sceneState, MeshPart* meshPart);

protected:
	/** Gets an affect parameter from an SasBindAddress value. */
	EffectParameter* GetParameterBySasBind(const String& bind);

	SemanticID GetSemanticID(const String& name);

	void LoadParameters();
	void LoadScript();
};

SEPointer(SasEffectMaterial);

}

#endif
