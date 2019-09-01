/*=============================================================================
Frustum.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FRUSTUM_H_
#define _SE_FRUSTUM_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Plane.h"

namespace SonataEngine
{

class BoundingBox;
class BoundingSphere;
class Frustum;
class OBB;

/** Frustum clipping planes. */
enum FrustumPlane
{
	FrustumPlane_Near = 0,
	FrustumPlane_Far = 1,
	FrustumPlane_Left = 2,
	FrustumPlane_Right = 3,
	FrustumPlane_Top = 4,
	FrustumPlane_Bottom = 5,
	NumFrustumPlanes = 6
};

/**
	@brief Frustum.

	The view frustum is the volume of space that is visible by the camera.
	It has a pyramid shape with the tip snipped off.
	the view frustum is bounded by six clip planes.
	The sides of the pyramid are the top, left, bottom, right clip planes.
	The top and bottom of the pyramid respectively are the near and far clip planes.
	The normals of the planes are pointing inside the frustum.

	@remarks A flag could be added to mask some of the planes.
*/
class SE_CORE_EXPORT Frustum
{
protected:
	Matrix4 _matrix;
	Plane _planes[NumFrustumPlanes];
	Vector3 _corners[8];

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Frustum();

	/**
		Creates a new instance of Frustum from a matrix.
		@param value Combined matrix that usually takes view * projection matrix.
	*/
	Frustum(const Matrix4& value);

	/** Copy constructor. */
	Frustum(const Frustum& value);
	//@}

	/** @name Properties. */
	//@{
	/** Gets a frustum plane. */
	Plane GetFrustumPlane(FrustumPlane value) const;

	/** Gets the near plane of the Frustum. */
	Plane GetNear() const;

	/** Gets the far plane of the Frustum. */
	Plane GetFar() const;

	/** Gets the left plane of the Frustum. */
	Plane GetLeft() const;

	/** Gets the right plane of the Frustum. */
	Plane GetRight() const;

	/** Gets the top plane of the Frustum. */
	Plane GetTop() const;

	/** Gets the bottom plane of the Frustum. */
	Plane GetBottom() const;

	/** Gets the Matrix that was used to instantiate this Frustum. */
	Matrix4 GetMatrix() const;

	/** Sets the Matrix to instantiate this Frustum. */
	void SetMatrix(const Matrix4& value);

	/** Gets the array of 8 cornerS. */
	Vector3* GetCorners() const;
	//@}

	/** @name Containment. */
	//@{
	/** Checks whether the current Frustum contains a point. */
	bool Contains(const Vector3& point) const;

	/** Checks whether the current Frustum contains a BoundingSphere. */
	bool Contains(const BoundingSphere& sphere) const;

	/** Checks whether the current Frustum contains a BoundingBox. */
	bool Contains(const BoundingBox& box) const;

	/** Checks whether the current Frustum contains an OBB. */
	bool Contains(const OBB& obb) const;
	//@}

	/** @name Intersection. */
	//@{
	/** Checks whether the current Frustum intersects a BoundingSphere. */
	int Intersects(const BoundingSphere& sphere) const;

	/** Checks whether the current Frustum intersects a BoundingBox. */
	int Intersects(const BoundingBox& box) const;

	/** Checks whether the current Frustum intersects an OBB. */
	int Intersects(const OBB& obb) const;

	/** Checks whether the current Frustum intersects a Frustum. */
	int Intersects(const Frustum& frustum) const;

	/** Checks whether the current Frustum intersects a Ray. */
	bool Intersects(const Ray3& ray, real& result) const;

	/** Checks whether the current Frustum intersects a Plane. */
	PlaneIntersectionType Intersects(const Plane& plane) const;
	//@}

private:
	Vector3 ComputeIntersection(const Plane& plane, const Ray3& ray) const;
	Ray3 ComputeIntersectionLine(const Plane& p1, const Plane& p2) const;
	void SetFrustumMatrix(const Matrix4& value);
	void UpdateCorners();
};

#include "Frustum.inl"

}

#endif 
