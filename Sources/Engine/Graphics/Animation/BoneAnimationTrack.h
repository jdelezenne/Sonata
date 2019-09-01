/*=============================================================================
BoneAnimationTrack.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BONEANIMATIONTRACK_H_
#define _SE_BONEANIMATIONTRACK_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/AnimationTrack.h"
#include "Graphics/Animation/TransformKeyFrame.h"
#include "Graphics/Model/Bone.h"
#include "Graphics/System/RenderData.h"

namespace SonataEngine
{

/**
	@brief Bone animation track.

	A bone track represents the animation of a bone transformation.
*/
class SE_GRAPHICS_EXPORT BoneAnimationTrack : public AnimationTrack
{
protected:
	Bone* _bone;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	BoneAnimationTrack();

	/** Destructor. */
	virtual ~BoneAnimationTrack();
	//@}

	/** Properties. */
	//@{
	Bone* GetBone() const { return _bone; }
	void SetBone(Bone* value) { _bone = value; }
	//@}

	virtual TransformKeyFrame* GetBoneKeyFrame(int index) const;

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
