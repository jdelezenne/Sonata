/*=============================================================================
TransformKeyFrame.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TRANSFORMKEYFRAME_H_
#define _SE_TRANSFORMKEYFRAME_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/KeyFrame.h"

namespace SonataEngine
{

/** Types of transform key frame. */
enum TransformKeyFrameType
{
	/// Position
	TransformKeyFrameType_Translation = (1<<0),

	/// Rotation
	TransformKeyFrameType_Rotation = (1<<1),

	/// Scale
	TransformKeyFrameType_Scale = (1<<2),

	/// All
	TransformKeyFrameType_All = (TransformKeyFrameType_Translation | TransformKeyFrameType_Rotation | TransformKeyFrameType_Scale),
};

struct KeyVector3
{
	real64 Time;
	Vector3 Value;

	KeyVector3() :
		Time(0.0)
	{
	}

	KeyVector3(real64 time, const Vector3& value) :
		Time(time), Value(value)
	{
	}
};

struct KeyQuaternion
{
	real64 Time;
	Quaternion Value;

	KeyQuaternion() :
		Time(0.0)
	{
	}

	KeyQuaternion(real64 time, const Quaternion& value) :
		Time(time), Value(value)
	{
	}
};

/**
	@brief Transform key frame.

	A transform key frame is a specialized key frame which contains a transform.
*/
class SE_GRAPHICS_EXPORT TransformKeyFrame : public KeyFrame
{
protected:
	TransformKeyFrameType _transformType;
	Vector3 _translation;
	Quaternion _rotation;
	Vector3 _scale;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Default constructor. */
	TransformKeyFrame();

	/** Constructor with a time value.
		@param timeValue The initial time value of the key frame.
	*/
	TransformKeyFrame(const TimeValue& timeValue);

	/** Destructor. */
	virtual ~TransformKeyFrame();
	//@}

	/** @name Properties. */
	//@{
	/** Gets the type of transform in the key frame. */
	TransformKeyFrameType GetTransformType() const { return _transformType; }

	/** Sets the type of transform in the key frame. */
	void SetTransformType(TransformKeyFrameType value) { _transformType = value; }

	/** Gets the translation value of the key frame. */
	const Vector3& GetTranslation() const { return _translation; }

	/** Sets the time value of the key frame. */
	void SetTranslation(const Vector3& value) { _translation = value; }

	/** Gets the rotation value of the key frame. */
	const Quaternion& GetRotation() const { return _rotation; }

	/** Sets the rotation value of the key frame. */
	void SetRotation(const Quaternion& value) { _rotation = value; }

	/** Gets the scale value of the key frame. */
	const Vector3& GetScale() const { return _scale; }

	/** Sets the scale value of the key frame. */
	void SetScale(const Vector3& value) { _scale = value; }
	//@}
};

}

#endif 
