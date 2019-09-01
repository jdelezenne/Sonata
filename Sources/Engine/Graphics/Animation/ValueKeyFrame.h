/*=============================================================================
ValueKeyFrame.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VALUEKEYFRAME_H_
#define _SE_VALUEKEYFRAME_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/KeyFrame.h"

namespace SonataEngine
{

/**
	@brief Morph key frame.

	A morph key frame is a specialized key frame which contains a vertex buffer.
*/
class SE_GRAPHICS_EXPORT ValueKeyFrame : public KeyFrame
{
protected:

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Default constructor. */
	ValueKeyFrame();

	/** Constructor with a time value.
		@param timeValue The initial time value of the key frame.
	*/
	ValueKeyFrame(const TimeValue& timeValue);

	/** Destructor. */
	virtual ~ValueKeyFrame();
	//@}

	/** @name Properties. */
	//@{
	//@}
};

}

#endif 
