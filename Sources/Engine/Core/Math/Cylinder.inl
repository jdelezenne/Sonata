/*=============================================================================
Cylinder.inl
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

SE_INLINE Cylinder::Cylinder()
{
}

SE_INLINE Cylinder::Cylinder(const Segment& segment, real radius)
{
	this->_Segment = segment;
	this->_Radius = radius;
}

SE_INLINE Cylinder::Cylinder(const Vector3& p0, const Vector3& p1, real radius)
{
	this->_Segment = Segment(p0, p1);
	this->_Radius = radius;
}

SE_INLINE Cylinder::Cylinder(const Vector3& center, const Vector3& axis, real height, real radius)
{
	real halfHeight = height / (real)2.0;
	this->_Segment = Segment(center - axis * halfHeight, center + axis * halfHeight);
	this->_Radius = radius;
}

SE_INLINE Cylinder::Cylinder(const Cylinder& value)
{
	this->_Segment = value.GetSegment();
	this->_Radius = value.GetRadius();
}

SE_INLINE bool Cylinder::operator==(const Cylinder& value) const
{
	return ((_Segment == value.GetSegment()) && (_Radius == value.GetRadius()));
}

SE_INLINE bool Cylinder::operator!=(const Cylinder& value) const
{
	return !(*this == value);
}

SE_INLINE Vector3 Cylinder::GetCenter() const
{
	return _Segment.ComputePoint((real)0.5);
}

SE_INLINE real Cylinder::GetHeight() const
{
	return _Segment.GetLength();
}

SE_INLINE const Vector3& Cylinder::GetPoint0() const
{
	return _Segment.p0;
}

SE_INLINE void Cylinder::SetPoint0(const Vector3& value)
{
	_Segment.p0 = value;
}

SE_INLINE const Vector3& Cylinder::GetPoint1() const
{
	return _Segment.p1;
}

SE_INLINE void Cylinder::SetPoint1(const Vector3& value)
{
	_Segment.p1 = value;
}

SE_INLINE const Segment& Cylinder::GetSegment() const
{
	return _Segment;
}

SE_INLINE void Cylinder::SetSegment(const Segment& value)
{
	_Segment = value;
}

SE_INLINE real Cylinder::GetRadius() const
{
	return _Radius;
}

SE_INLINE void Cylinder::SetRadius(real value)
{
	_Radius = value;
}

SE_INLINE real Cylinder::GetArea() const
{
	return (Math::Pi * _Radius * _Radius * GetHeight());
}

SE_INLINE real Cylinder::GetVolume() const
{
	// sides + caps
	return ((real)2.0 * Math::Pi * _Radius * GetHeight() +
		(real)2.0 * Math::Pi * _Radius * _Radius);
}

SE_INLINE bool Cylinder::ContainsPoint(const Vector3& value) const
{
	return (_Segment.GetSquareDistance(value) <= _Radius * _Radius);
}

SE_INLINE void Cylinder::Transform(const Matrix4& transform)
{
	*this = Cylinder::Transform(*this, transform);
}

SE_INLINE Cylinder Cylinder::Transform(const Cylinder& cylinder, const Matrix4& transform)
{
	Cylinder result;

	// Transform the center
	result.SetSegment(Segment::Transform(cylinder.GetSegment(), transform));
	result.SetRadius(cylinder.GetRadius());

	return result;
}
