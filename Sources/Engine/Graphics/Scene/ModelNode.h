/*=============================================================================
ModelNode.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MODELNODE_H_
#define _SE_MODELNODE_H_

#include "Core/Core.h"
#include "Graphics/Scene/SceneObject.h"
#include "Graphics/Model/Model.h"

namespace SonataEngine
{

/**
	@brief Model node.

	Models can be attached to this node to be rendered.
*/
class SE_GRAPHICS_EXPORT ModelNode : public SceneObject
{
	SE_DECLARE_CLASS(ModelNode, SceneObject);
	SE_BEGIN_REFLECTION(ModelNode);
		SE_Field(_isShadowCaster, bool, Public);
		SE_Field(_isShadowReceiver, bool, Public);
	SE_END_REFLECTION(ModelNode);

protected:
	Array<String> _lightGroups;
	ModelPtr _model;
	bool _isShadowCaster;
	bool _isShadowReceiver;

public:
	ModelNode();
	virtual ~ModelNode();

	const Array<String>& GetLightGroups() const { return _lightGroups; }
	void AddLightGroup(const String& value);
	void RemoveLightGroup(const String& value);

	Model* GetModel() const { return _model; }
	void SetModel(Model* value);

	Matrix4 GetModelTransform();

	bool IsShadowCaster() const { return _isShadowCaster; }
	void SetShadowCaster(bool value) { _isShadowCaster = value; }

	bool IsShadowReceiver() const { return _isShadowReceiver; }
	void SetShadowReceiver(bool value) { _isShadowReceiver = value; }

	virtual void Update(const TimeValue& timeValue);
	virtual void Render();

	virtual void UpdateBounds();
};

}

#endif
