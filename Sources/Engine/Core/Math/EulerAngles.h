/*=============================================================================
EulerAngles.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EULERANGLES_H_
#define _SE_EULERANGLES_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"

namespace SonataEngine
{

/** Euler angles Rotation order.
	@todo Unused
*/
enum EulerAnglesOrder
{
	EulerAnglesOrder_XYZ,
	EulerAnglesOrder_XZY,
	EulerAnglesOrder_YZX,
	EulerAnglesOrder_YXZ,
	EulerAnglesOrder_ZXY,
	EulerAnglesOrder_ZYX,
	EulerAnglesOrder_XYX,
	EulerAnglesOrder_YZY,
	EulerAnglesOrder_ZXZ
};


/**
	@brief Euler Angles.

	@remarks
		This representation should only ne used for input from and output to other
		angle representations because of the "Glimbal Map" problem and better
		interpolation.
*/
class SE_CORE_EXPORT EulerAngles
{
public:
	union
	{
		struct
		{
			real X;
			real Y;
			real Z;
		};
		struct
		{
			real Pitch;
			real Yaw;
			real Roll;
		};
		real Data[3];
	};

	EulerAnglesOrder Order;

public:
	static const EulerAngles Zero;

	/** @name Constructors. */
	//@{
	EulerAngles();
	EulerAngles(const Vector3& angles, EulerAnglesOrder order = EulerAnglesOrder_XYZ);
	EulerAngles(real x, real y, real z, EulerAnglesOrder order = EulerAnglesOrder_XYZ);
	EulerAngles(real angles, EulerAnglesOrder order = EulerAnglesOrder_XYZ);
	EulerAngles(const EulerAngles& value);
	//@}

	/** @name Access Operators. */
	//@{
	/** Retrieves a component by its index. */
	real operator[](int index) const;
	real& operator[](int index);
	//@}

	/** @name Unary Operators. */
	//@{
	EulerAngles operator+() const;
	EulerAngles operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another angle. */
	EulerAngles& operator+=(const EulerAngles& value);

	/** Substracts this instance by another angle. */
	EulerAngles& operator-=(const EulerAngles& value);

	/** Multiplies this instance by a scalar. */
	EulerAngles& operator*=(real value);
	
	/** Divides this instance by a scalar. */
	EulerAngles& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two angles. */
	EulerAngles operator+(const EulerAngles& value) const;
	
	/** Substracts two angles. */
	EulerAngles operator-(const EulerAngles& value) const;
	
	/** Multiplies an angle by a scalar. */
	EulerAngles operator*(real value) const;
	
	/** Divides an angle by a scalar. */
	EulerAngles operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const EulerAngles& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const EulerAngles& value) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Clamp.
	*/
	static EulerAngles Clamp(const EulerAngles& value, const EulerAngles& min, const EulerAngles& max);
	//@}

	/** @name Convertions. */
	//@{
	/** @todo */
	void FromAxisAngle(const Vector3& axis, real angle);

	void ToVector(Vector3& value) const;
	void ToAxes(Vector3& valX, Vector3& valY, Vector3& valZ) const;
	void ToMatrix3(Matrix3& value) const;
	void ToQuaternion(Quaternion& value) const;
	//@}

    virtual bool Equals(const EulerAngles& value) const;
    virtual bool Equals(const EulerAngles& value, real epsilon) const;
};

#include "EulerAngles.inl"

}

#endif 
