/*=============================================================================
MorphKeyFrame.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MORPHKEYFRAME_H_
#define _SE_MORPHKEYFRAME_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/KeyFrame.h"
#include "Graphics/System/HardwareBuffer.h"

namespace SonataEngine
{

/**
	@brief Morph key frame.

	A morph key frame is a specialized key frame which contains a vertex buffer.
*/
class SE_GRAPHICS_EXPORT MorphKeyFrame : public KeyFrame
{
protected:
	HardwareBufferPtr _vertexBuffer;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Default constructor. */
	MorphKeyFrame();

	/** Constructor with a time value.
		@param timeValue The initial time value of the key frame.
	*/
	MorphKeyFrame(const TimeValue& timeValue);

	/** Destructor. */
	virtual ~MorphKeyFrame();
	//@}

	/** @name Properties. */
	//@{
	/** Gets the vertex buffer of the key frame. */
	HardwareBuffer* GetVertexBuffer() const { return _vertexBuffer; }

	/** Sets the vertex buffer of the key frame. */
	void SetVertexBuffer(HardwareBuffer* value) { _vertexBuffer = value; }
	//@}
};

}

#endif 
