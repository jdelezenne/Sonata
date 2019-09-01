	/*=============================================================================
Transform3.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TRANSFORM3_H_
#define _SE_TRANSFORM3_H_

#include "Core/Common.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

/**
	@brief Transform3.

	Represents a 3D transformation.
	A transform is composed of a translation, a scale and an orientation.
*/
class SE_CORE_EXPORT Transform3
{
public:
	static const Transform3 Identity;

protected:
	bool _needUpdate;
	Vector3 _translation;
	Vector3 _scale;
	Quaternion _rotation;
	Matrix4 _matrix;
	Matrix4 _inverseMatrix;

public:
	/** @name Constructors. */
	//@{
	/** Constructor. */
	Transform3();

	/** Initializes a transform with initial values. */
	Transform3(const Vector3& translation, const Vector3& scale, const Quaternion& rotation);

	/** Copy constructor. */
	Transform3(const Transform3& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Transform3& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Transform3& value) const;
	//@}

	/** @name Properties. */
	//@{
	/**
		Gets the translation.
		@return Translation.
	*/
	const Vector3& GetTranslation() const { return _translation; }

	/**
		Sets the translation.
		@param value Translation.
	*/
	void SetTranslation(const Vector3& value);

	/**
		Gets the rotation.
		@return Rotation.
	*/
	const Quaternion& GetRotation() const { return _rotation; }

	/**
		Sets the rotation.
		@param value Rotation.
	*/
	void SetRotation(const Quaternion& value);

	/**
		Gets the scale.
		@return Scale.
	*/
	const Vector3& GetScale() const { return _scale; }

	/**
		Sets the scale.
		@param value Scale.
	*/
	void SetScale(const Vector3& value);

	/**
		Sets the scale with a uniform value.
		@param value Scale.
	*/
	void SetScaleUniform(real value);

	/**
		Gets the transformation matrix.
		@return Transform3ation matrix.
	*/
	const Matrix4& GetMatrix();

	/**
		Gets the inverse transformation matrix.
		@return Inverse transformation matrix.
	*/
	const Matrix4& GetInverseMatrix();
	//@}

	/** @name Operations. */
	//@{
	/**
		Translates.
		@param value Translation vector.
	*/
	void Translate(const Vector3& value);

	/**
		Rotates.
		@param value Rotation quaternion.
	*/
	void Rotate(const Quaternion& value);

	/**
		Scales.
		@param value Scale vector.
	*/
	void Scale(const Vector3& value);

	/**
		Scales uniformly.
		@param value Scalar.
	*/
	void ScaleUniform(real value);

	/**
		Computes the inverse of this transformation.
	*/
	void Inverse();

	/**
		Computes the inverse of a transformation.
		@param value Transform3ation to inverse.
		@return Inverse of the transformation.
	*/
	static Transform3 Inverse(const Transform3& value);

	/**
		Combines this transformation with another transformation.
	*/
	void Combine(const Transform3& value);

	/**
		Combines this transformation with another transformation.
		@param valA Left transformation.
		@param valB Right transformation.
		@return The combinaison of the two transformations.
	*/
	static Transform3 Combine(const Transform3& valA, const Transform3& valB);
	//@}

protected:
	virtual void UpdateMatrix();
};

#include "Transform3.inl"

}

#endif
