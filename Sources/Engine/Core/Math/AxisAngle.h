/*=============================================================================
AxisAngle.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AXISANGLE_H_
#define _SE_AXISANGLE_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/EulerAngles.h"

namespace SonataEngine
{

/** Rotation by an angle along an axis. */
class SE_CORE_EXPORT AxisAngle
{
public:
	/** @name Constructors. */
	//@{
	AxisAngle();
	AxisAngle(real x, real y, real z, real angle);
	AxisAngle(const Vector3& axis, real angle);
	AxisAngle(const AxisAngle& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const AxisAngle& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const AxisAngle& value) const;
	//@}

	/** @name Convertions. */
	//@{
	void ToEulerAngles(EulerAngles& value);
	void ToMatrix3(Matrix3& value);
	void ToQuaternion(Quaternion& value);
	//@}

    virtual bool Equals(const AxisAngle& value) const;
    virtual bool Equals(const AxisAngle& value, real epsilon) const;

public:
	Vector3 Axis;
	real Angle;
};

#include "AxisAngle.inl"

}

#endif 
