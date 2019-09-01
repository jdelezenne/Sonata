/*=============================================================================
Transform3.inl
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

SE_INLINE Transform3::Transform3() :
	_needUpdate(false),
	_translation(Vector3::Zero),
	_scale(Vector3::One),
	_rotation(Quaternion::Identity),
	_matrix(Matrix4::Identity),
	_inverseMatrix(Matrix4::Identity)
{
}

SE_INLINE Transform3::Transform3(const Vector3& translation, const Vector3& scale, const Quaternion& rotation) :
	_needUpdate(true),
	_translation(translation),
	_scale(scale),
	_rotation(rotation)
{
	UpdateMatrix();
}

SE_INLINE Transform3::Transform3(const Transform3& value) :
	_needUpdate(true),
	_translation(value.GetTranslation()),
	_scale(value.GetScale()),
	_rotation(value.GetRotation())
{
	UpdateMatrix();
}

SE_INLINE bool Transform3::operator==(const Transform3& value) const
{
	return ((_translation == value.GetTranslation()) &&
		(_scale == value.GetScale()) &&
		(_rotation == value.GetRotation()));
}

SE_INLINE bool Transform3::operator!=(const Transform3& value) const
{
	return !(*this == value);
}
