/*=============================================================================
MorphAnimationTrack.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MORPHANIMATIONTRACK_H_
#define _SE_MORPHANIMATIONTRACK_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/AnimationTrack.h"
#include "Graphics/Animation/MorphKeyFrame.h"
#include "Graphics/System/RenderData.h"

namespace SonataEngine
{

/**
	@brief Morph animation track.

	A morph track represents the animation of vertices.
*/
class SE_GRAPHICS_EXPORT MorphAnimationTrack : public AnimationTrack
{
protected:
	VertexData* _vertexData;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	MorphAnimationTrack();

	/** Destructor. */
	virtual ~MorphAnimationTrack();
	//@}

	/** Properties. */
	//@{
	VertexData* GetVertexData() const { return _vertexData; }
	void SetVertexData(VertexData* value) { _vertexData = value; }
	//@}

	virtual MorphKeyFrame* GetMorphKeyFrame(int index) const;

	virtual void Update(const TimeValue& timeValue);

protected:
	virtual KeyFrame* _CreateKeyFrame(const TimeValue& timeValue);
};

}

#endif 
