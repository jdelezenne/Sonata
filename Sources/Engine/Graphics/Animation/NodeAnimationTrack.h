/*=============================================================================
NodeAnimationTrack.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NODEANIMATIONTRACK_H_
#define _SE_NODEANIMATIONTRACK_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/AnimationTrack.h"
#include "Graphics/Animation/TransformKeyFrame.h"
#include "Graphics/Scene/SceneObject.h"
#include "Graphics/System/RenderData.h"

namespace SonataEngine
{

/**
	@brief Node animation track.

	A node track represents the animation of a node transformation.
*/
class SE_GRAPHICS_EXPORT NodeAnimationTrack : public AnimationTrack
{
protected:
	SceneObject* _node;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	NodeAnimationTrack();

	/** Destructor. */
	virtual ~NodeAnimationTrack();
	//@}

	/** Properties. */
	//@{
	SceneObject* GetNode() const { return _node; }
	void SetNode(SceneObject* value) { _node = value; }
	//@}

	virtual TransformKeyFrame* GetNodeKeyFrame(int index) const;

	/** Fills an array with translational key data used for key frame animation. */
	void GetTranslationKeys(BaseArray<KeyVector3>& translationKeys);

	/** Fills an array with rotational key data used for key frame animation. */
	void GetRotationKeys(BaseArray<KeyQuaternion>& rotationKeys);
 
	/** Fills an array with scale key data used for key frame animation. */
	void GetScaleKeys(BaseArray<KeyVector3>& scaleKeys);

	virtual void Update(const TimeValue& timeValue);

protected:
	virtual KeyFrame* _CreateKeyFrame(const TimeValue& timeValue);
};

}

#endif 
