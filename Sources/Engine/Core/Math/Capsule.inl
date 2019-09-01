/*=============================================================================
Capsule.inl
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

SE_INLINE Capsule::Capsule()
{
}

SE_INLINE Capsule::Capsule(const Segment& segment, real radius) :
	_Segment(segment),
	_Radius(radius)
{
}

SE_INLINE Capsule::Capsule(const Vector3& p0, const Vector3& p1, real radius) :
	_Segment(Segment(p0, p1)),
	_Radius(radius)
{
}

SE_INLINE Capsule::Capsule(const Vector3& center, const Vector3& axis, real height, real radius)
{
	real halfHeight = height / (real)2.0;
	this->_Segment = Segment(center - axis * halfHeight, center + axis * halfHeight);
	this->_Radius = radius;
}

SE_INLINE Capsule::Capsule(const Capsule& value)
{
	this->_Segment = value.GetSegment();
	this->_Radius = value.GetRadius();
}

SE_INLINE bool Capsule::operator==(const Capsule& value) const
{
	return ((_Segment == value.GetSegment()) && (_Radius == value.GetRadius()));
}

SE_INLINE bool Capsule::operator!=(const Capsule& value) const
{
	return !(*this == value);
}

SE_INLINE Vector3 Capsule::GetCenter() const
{
	return _Segment.ComputePoint((real)0.5);
}

SE_INLINE real Capsule::GetHeight() const
{
	return _Segment.GetLength();
}

SE_INLINE const Vector3& Capsule::GetPoint0() const
{
	return _Segment.p0;
}

SE_INLINE void Capsule::SetPoint0(const Vector3& value)
{
	_Segment.p0 = value;
}

SE_INLINE const Vector3& Capsule::GetPoint1() const
{
	return _Segment.p1;
}

SE_INLINE void Capsule::SetPoint1(const Vector3& value)
{
	_Segment.p1 = value;
}

SE_INLINE const Segment& Capsule::GetSegment() const
{
	return _Segment;
}

SE_INLINE void Capsule::SetSegment(const Segment& value)
{
	_Segment = value;
}

SE_INLINE real Capsule::GetRadius() const
{
	return _Radius;
}

SE_INLINE void Capsule::SetRadius(real value)
{
	_Radius = value;
}

SE_INLINE real Capsule::GetArea() const
{
	// sphere + cylinder
	return (Math::Pow(_Radius, (real)2.0) * (real)4.0 * Math::Pi +
		(real)2.0 * Math::Pi * _Radius * GetHeight());
}

SE_INLINE real Capsule::GetVolume() const
{
	// sphere + cylinder
	return (Math::Pow(_Radius, (real)3.0) * (real)(4.0/3.0) * Math::Pi +
		Math::Pow(_Radius, (real)2.0) * GetHeight());
}

SE_INLINE bool Capsule::ContainsPoint(const Vector3& value) const
{
	return (_Segment.GetSquareDistance(value) <= _Radius * _Radius);
}

SE_INLINE void Capsule::Transform(const Matrix4& transform)
{
	*this = Capsule::Transform(*this, transform);
}

SE_INLINE Capsule Capsule::Transform(const Capsule& capsule, const Matrix4& transform)
{
	Capsule result;

	// Transform the center
	result.SetSegment(Segment::Transform(capsule.GetSegment(), transform));
	result.SetRadius(capsule.GetRadius());

	return result;
}
