/*=============================================================================
IntersectionSphereSphere.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "IntersectionSphereSphere.h"

namespace SonataEngine
{

real Math_Sq(real value)
{
	return (value * value);
}

bool IntersectionSphereSphere::Intersect(const Sphere& sphere0, const Sphere& sphere1)
{
	Vector3 diff = sphere1.Center - sphere0.Center;
	real sumRadius = sphere0.Radius + sphere1.Radius;
	return (diff.LengthSquared() <= sumRadius*sumRadius);
}

bool IntersectionSphereSphere::Contact(const Sphere& sphere0, const Sphere& sphere1, SimpleArray<IntersectionContact>* contacts)
{
	Vector3 diff = sphere0.Center - sphere1.Center;
	real diffSqLen = diff.LengthSquared();
	real sumRadius = sphere0.Radius + sphere1.Radius;
	if (diffSqLen > sumRadius*sumRadius)
	{
		return false;
	}

	if (contacts != NULL)
	{
		real diffLen = Math::Sqrt(diffSqLen);
		real depth = sumRadius - diffLen;
		diff /= diffLen;
		Vector3 point = sphere1.Center + diff * (sphere1.Radius - depth * 0.5);

		IntersectionContact contact;
		contact._Position = point;
		contact._Normal = diff;
		contact._Depth = depth;
		contacts->Add(contact);
	}

	return true;
}

bool IntersectionPlaneSphere::Intersect(const Plane& plane, const Sphere& sphere)
{
	real distance = plane.GetDistance(sphere.Center);
	return (Math::Abs(distance) <= sphere.Radius);
}

bool IntersectionPlaneSphere::Contact(const Plane& plane, const Sphere& sphere, SimpleArray<IntersectionContact>* contacts)
{
	real distance = plane.GetDistance(sphere.Center);
	if (Math::Abs(distance) <= sphere.Radius)
	{
		if (contacts != NULL)
		{
			IntersectionContact contact;
			contact._Normal = plane.Normal;
			contact._Position = sphere.Center - sphere.Radius * plane.Normal;
			contact._Depth = sphere.Radius - distance;
			contacts->Add(contact);
		}
		return true;
	}
	else
	{
		return false;
	}
}

real Box_GetSquareDistance(const Box& box, Vector3& closest, const Vector3& point)
{
	Vector3 center = box.Center;
	Vector3 extents = box.Extents;
	Matrix3 rotation = box.Rotation;

	closest = Matrix3::Transpose(rotation) * (point - center);

	// project test point onto box
	real sqDistance = 0.0;
	real delta;

	if (closest.x < 0.0)
	{
		sqDistance += Math_Sq(closest.x);
		closest.x = 0.0;
	}
	else if (closest.x > extents.x)
	{
		delta = closest.x - extents.x;
		sqDistance += Math_Sq(delta);
		closest.x = extents.x;
	}

	if (closest.y < 0.0)
	{
		sqDistance += Math_Sq(closest.y);
		closest.y = 0.0;
	}
	else if (closest.y > extents.y)
	{
		delta = closest.y - extents.y;
		sqDistance += Math_Sq(delta);
		closest.y = extents.y;
	}

	if (closest.z < 0.0)
	{
		sqDistance += Math_Sq(closest.z);
		closest.z = 0.0;
	}
	else if (closest.z > extents.z)
	{
		delta = closest.z - extents.z;
		sqDistance += Math_Sq(delta);
		closest.z = extents.z;
	}

	closest = center + rotation * closest;
	return sqDistance;
}

real Box_GetDistance(const Box& box, Vector3& closest, const Vector3& point)
{
	return Math::Sqrt(Box_GetSquareDistance(box, closest, point));
}

bool IntersectionBoxBox::Intersect(const Box& box0, const Box& box1)
{
	//-----------------
	// From Wild Magic
	//-----------------

	// Cutoff for cosine of angles between box axes.  This is used to catch
	// the cases when at least one pair of axes are parallel.  If this happens,
	// there is no need to test for separation along the Cross(A[i],B[j])
	// directions.
	const real fCutoff = (real)1.0 - Math::Epsilon;
	bool bExistsParallelPair = false;
	int i;

	// convenience variables
	const Vector3* akA = (const Vector3*)(const real*)box0.Rotation;
	const Vector3* akB = (const Vector3*)(const real*)box1.Rotation;
	const real* afEA = box0.Extents;
	const real* afEB = box1.Extents;

	// compute difference of box centers, D = C1-C0
	Vector3 kD = box1.Center - box0.Center;

	real aafC[3][3];	 // matrix C = A^T B, c_{ij} = Dot(A_i,B_j)
	real aafAbsC[3][3];  // |c_{ij}|
	real afAD[3];		// Dot(A_i,D)
	real fR0, fR1, fR;   // interval radii and distance between centers
	real fR01;		   // = R0 + R1

	// axis C0+t*A0
	for (i = 0; i < 3; i++)
	{
		aafC[0][i] = akA[0].Dot(akB[i]);
		aafAbsC[0][i] = Math::Abs(aafC[0][i]);
		if (aafAbsC[0][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}
	afAD[0] = akA[0].Dot(kD);
	fR = Math::Abs(afAD[0]);
	fR1 = afEB[0]*aafAbsC[0][0]+afEB[1]*aafAbsC[0][1]+afEB[2]*aafAbsC[0][2];
	fR01 = afEA[0] + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A1
	for (i = 0; i < 3; i++)
	{
		aafC[1][i] = akA[1].Dot(akB[i]);
		aafAbsC[1][i] = Math::Abs(aafC[1][i]);
		if (aafAbsC[1][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}
	afAD[1] = akA[1].Dot(kD);
	fR = Math::Abs(afAD[1]);
	fR1 = afEB[0]*aafAbsC[1][0]+afEB[1]*aafAbsC[1][1]+afEB[2]*aafAbsC[1][2];
	fR01 = afEA[1] + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A2
	for (i = 0; i < 3; i++)
	{
		aafC[2][i] = akA[2].Dot(akB[i]);
		aafAbsC[2][i] = Math::Abs(aafC[2][i]);
		if (aafAbsC[2][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}
	afAD[2] = akA[2].Dot(kD);
	fR = Math::Abs(afAD[2]);
	fR1 = afEB[0]*aafAbsC[2][0]+afEB[1]*aafAbsC[2][1]+afEB[2]*aafAbsC[2][2];
	fR01 = afEA[2] + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*B0
	fR = Math::Abs(akB[0].Dot(kD));
	fR0 = afEA[0]*aafAbsC[0][0]+afEA[1]*aafAbsC[1][0]+afEA[2]*aafAbsC[2][0];
	fR01 = fR0 + afEB[0];
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*B1
	fR = Math::Abs(akB[1].Dot(kD));
	fR0 = afEA[0]*aafAbsC[0][1]+afEA[1]*aafAbsC[1][1]+afEA[2]*aafAbsC[2][1];
	fR01 = fR0 + afEB[1];
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*B2
	fR = Math::Abs(akB[2].Dot(kD));
	fR0 = afEA[0]*aafAbsC[0][2]+afEA[1]*aafAbsC[1][2]+afEA[2]*aafAbsC[2][2];
	fR01 = fR0 + afEB[2];
	if (fR > fR01)
	{
		return false;
	}

	// At least one pair of box axes was parallel, so the separation is
	// effectively in 2D where checking the "edge" normals is sufficient for
	// the separation of the boxes.
	if (bExistsParallelPair)
	{
		return true;
	}

	// axis C0+t*A0xB0
	fR = Math::Abs(afAD[2]*aafC[1][0]-afAD[1]*aafC[2][0]);
	fR0 = afEA[1]*aafAbsC[2][0] + afEA[2]*aafAbsC[1][0];
	fR1 = afEB[1]*aafAbsC[0][2] + afEB[2]*aafAbsC[0][1];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A0xB1
	fR = Math::Abs(afAD[2]*aafC[1][1]-afAD[1]*aafC[2][1]);
	fR0 = afEA[1]*aafAbsC[2][1] + afEA[2]*aafAbsC[1][1];
	fR1 = afEB[0]*aafAbsC[0][2] + afEB[2]*aafAbsC[0][0];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A0xB2
	fR = Math::Abs(afAD[2]*aafC[1][2]-afAD[1]*aafC[2][2]);
	fR0 = afEA[1]*aafAbsC[2][2] + afEA[2]*aafAbsC[1][2];
	fR1 = afEB[0]*aafAbsC[0][1] + afEB[1]*aafAbsC[0][0];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A1xB0
	fR = Math::Abs(afAD[0]*aafC[2][0]-afAD[2]*aafC[0][0]);
	fR0 = afEA[0]*aafAbsC[2][0] + afEA[2]*aafAbsC[0][0];
	fR1 = afEB[1]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][1];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A1xB1
	fR = Math::Abs(afAD[0]*aafC[2][1]-afAD[2]*aafC[0][1]);
	fR0 = afEA[0]*aafAbsC[2][1] + afEA[2]*aafAbsC[0][1];
	fR1 = afEB[0]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][0];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A1xB2
	fR = Math::Abs(afAD[0]*aafC[2][2]-afAD[2]*aafC[0][2]);
	fR0 = afEA[0]*aafAbsC[2][2] + afEA[2]*aafAbsC[0][2];
	fR1 = afEB[0]*aafAbsC[1][1] + afEB[1]*aafAbsC[1][0];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A2xB0
	fR = Math::Abs(afAD[1]*aafC[0][0]-afAD[0]*aafC[1][0]);
	fR0 = afEA[0]*aafAbsC[1][0] + afEA[1]*aafAbsC[0][0];
	fR1 = afEB[1]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][1];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A2xB1
	fR = Math::Abs(afAD[1]*aafC[0][1]-afAD[0]*aafC[1][1]);
	fR0 = afEA[0]*aafAbsC[1][1] + afEA[1]*aafAbsC[0][1];
	fR1 = afEB[0]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][0];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	// axis C0+t*A2xB2
	fR = Math::Abs(afAD[1]*aafC[0][2]-afAD[0]*aafC[1][2]);
	fR0 = afEA[0]*aafAbsC[1][2] + afEA[1]*aafAbsC[0][2];
	fR1 = afEB[0]*aafAbsC[2][1] + afEB[1]*aafAbsC[2][0];
	fR01 = fR0 + fR1;
	if (fR > fR01)
	{
		return false;
	}

	return true;
}

bool IntersectionBoxBox::Contact(const Box& box0, const Box& box1, SimpleArray<IntersectionContact>* contacts)
{
	Sphere sphere0(box0.Center, Math::Max(box0.Extents.x, Math::Max(box0.Extents.y, box0.Extents.z)));
	Sphere sphere1(box1.Center, Math::Max(box1.Extents.x, Math::Max(box1.Extents.y, box1.Extents.z)));
	return IntersectionSphereSphere::Contact(sphere0, sphere1, contacts);
}

bool IntersectionBoxPlane::Intersect(const Box& box, const Plane& plane)
{
	Vector3 temp(
		box.Extents.x*(plane.Normal.Dot(box.Rotation.GetColumn(0))),
		box.Extents.y*(plane.Normal.Dot(box.Rotation.GetColumn(1))),
		box.Extents.z*(plane.Normal.Dot(box.Rotation.GetColumn(2))));

	real radius = Math::Abs(temp.x) + Math::Abs(temp.y) + Math::Abs(temp.z);

	real distance = plane.GetDistance(box.Center);
	return (Math::Abs(distance) <= radius);
}

bool IntersectionBoxPlane::Contact(const Box& box, const Plane& plane, SimpleArray<IntersectionContact>* contacts)
{
	Sphere sphere(box.Center, Math::Max(box.Extents.x, Math::Max(box.Extents.y, box.Extents.z)));
	return IntersectionPlaneSphere::Contact(plane, sphere, contacts);

	Vector3 temp(
		box.Extents.x*(plane.Normal.Dot(box.Rotation.GetRow(0))),
		box.Extents.y*(plane.Normal.Dot(box.Rotation.GetRow(1))),
		box.Extents.z*(plane.Normal.Dot(box.Rotation.GetRow(2))));

	real radius = Math::Abs(temp.x) + Math::Abs(temp.y) + Math::Abs(temp.z);

	real distance = plane.GetDistance(box.Center);
	if (Math::Abs(distance) > radius)
		return false;

	if (contacts != NULL)
	{
		int i;
		Vector3 corners[8];
		box.ComputeCorners(corners);
		for (i=0; i<8; i++)
		{
			distance = -plane.GetDistance(corners[i]);
			if (distance > -Math::Epsilon)
			{ 
				IntersectionContact contact;
				contact._Normal = plane.Normal;
				contact._Position = corners[i];
				contact._Depth = distance;
				contacts->Add(contact);
			}
		}

		// Sort by depth, deepest first
		int count = contacts->Count()-1;
		while (count > 0)
		{
			IntersectionContact temp;
			for (i=0; i<count; i++)
			{
				if ((*contacts)[i+1]._Depth > (*contacts)[i]._Depth)
				{
					temp = (*contacts)[i];
					(*contacts)[i] = (*contacts)[i+1];
					(*contacts)[i+1] = temp;
				}
			}
			count--;
		}
	}

	return true;
}

bool IntersectionBoxSphere::Intersect(const Box& box, const Sphere& sphere)
{
	Vector3 boxPoint;
	real distance = Box_GetDistance(box, boxPoint, sphere.Center);
	real depth = sphere.Radius - distance;

	return (depth > -Math::Epsilon);
}

bool IntersectionBoxSphere::Contact(const Box& box, const Sphere& sphere, SimpleArray<IntersectionContact>* contacts)
{
	Sphere sphere0(box.Center, Math::Max(box.Extents.x, Math::Max(box.Extents.y, box.Extents.z)));
	return IntersectionSphereSphere::Contact(sphere0, sphere, contacts);

	Vector3 boxPoint;
	real distance = Box_GetDistance(box, boxPoint, sphere.Center);
	real depth = sphere.Radius - distance;

	if (depth <= -Math::Epsilon)
		return false;

	if (contacts != NULL)
	{
		Vector3 dir;
		if (distance < -Math::Epsilon)
		{
			dir = (boxPoint - sphere.Center - boxPoint);
		}
		else if (distance > Math::Epsilon)
		{
			dir = (sphere.Center - boxPoint);
		}
		else
		{
			dir = (sphere.Center - box.Center);
		}

		dir.Normalize();
		IntersectionContact contact;
		contact._Position = boxPoint;
		contact._Normal = dir;
		contact._Depth = depth;
		contacts->Add(contact);
	}

	return true;
}

}
