/*=============================================================================
Transform3.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Transform3.h"

namespace SonataEngine
{

const Transform3 Transform3::Identity = Transform3();

void Transform3::SetTranslation(const Vector3& value)
{
	_translation = value;
	_needUpdate = true;
}

void Transform3::SetRotation(const Quaternion& value)
{
	_rotation = value;
	_needUpdate = true;
}

void Transform3::SetScale(const Vector3& value)
{
	_translation = value;
	_needUpdate = true;
}

void Transform3::SetScaleUniform(real value)
{
	_scale.X = _scale.Y = _scale.Z = value;
	_needUpdate = true;
}

const Matrix4& Transform3::GetMatrix()
{
	if (_needUpdate)
		UpdateMatrix();

	return _matrix;
}

const Matrix4& Transform3::GetInverseMatrix()
{
	if (_needUpdate)
		UpdateMatrix();

	return _inverseMatrix;
}

void Transform3::Translate(const Vector3& value)
{
	_translation = _translation + _rotation.Rotate(value);
	_needUpdate = true;
}

void Transform3::Rotate(const Quaternion& value)
{
	_rotation = value * _rotation;
	_needUpdate = true;
}

void Transform3::Scale(const Vector3& value)
{
	_scale.Scale(value);
	_needUpdate = true;
}

void Transform3::ScaleUniform(real value)
{
	_scale *= value;
	_needUpdate = true;
}

void Transform3::Inverse()
{
	SetTranslation(-_translation);
	SetRotation(Quaternion::Invert(_rotation));
}

Transform3 Transform3::Inverse(const Transform3& value)
{
	Transform3 result = value;
	result.Inverse();
	return result;
}

void Transform3::Combine(const Transform3& value)
{
	*this = Transform3::Combine(*this, value);
}

Transform3 Transform3::Combine(const Transform3& left, const Transform3& right)
{
	Transform3 result;

	result.SetRotation(left.GetRotation() * right.GetRotation());
	result.SetTranslation(
		left.GetScale() * left.GetRotation().Rotate(right.GetTranslation()) + left.GetTranslation());
	result.SetScale(Vector3::Scaling(left.GetScale(), right.GetScale()));

	return result;
}

void Transform3::UpdateMatrix()
{
	//_matrix = Matrix4::Identity;
	//_matrix.Scale(_scale);
	//_matrix.Translate(_translation);
	//_matrix.RotateQuaternion(_rotation);

	Matrix3 rotation = Matrix3::CreateFromQuaternion(_rotation);
	_matrix.M00 = rotation.M00 * _scale.X;
	_matrix.M01 = rotation.M10 * _scale.Y;
	_matrix.M02 = rotation.M20 * _scale.Z;
	_matrix.M03 = _translation.X;

	_matrix.M10 = rotation.M01 * _scale.X;
	_matrix.M11 = rotation.M11 * _scale.Y;
	_matrix.M12 = rotation.M21 * _scale.Z;
	_matrix.M13 = _translation.Y;

	_matrix.M20 = rotation.M02 * _scale.X;
	_matrix.M21 = rotation.M12 * _scale.Y;
	_matrix.M22 = rotation.M22 * _scale.Z;
	_matrix.M23 = _translation.Z;

	_matrix.M30 = 0.0f;
	_matrix.M31 = 0.0f;
	_matrix.M32 = 0.0f;
	_matrix.M33 = 1.0f;

	_inverseMatrix = Matrix4::Invert(_matrix);

	_needUpdate = false;
}

}
