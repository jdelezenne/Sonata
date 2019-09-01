/*=============================================================================
OBB.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "OBB.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

const OBB OBB::Empty = OBB(Vector3::Zero, Vector3::Zero, Matrix3::Identity);

void OBB::GetCorners(Vector3* corners) const
{
	if (corners == NULL)
	{
		SEthrow(ArgumentNullException("corners"));
		return;
	}

	// From Opcode
	Vector3 axis0 = Rotation.GetRow(0);
	Vector3 axis1 = Rotation.GetRow(1);
	Vector3 axis2 = Rotation.GetRow(2);

	axis0 *= Extents.X;
	axis1 *= Extents.Y;
	axis2 *= Extents.Z;

	corners[0] = Center - axis0 - axis1 - axis2;
	corners[1] = Center + axis0 - axis1 - axis2;
	corners[2] = Center + axis0 + axis1 - axis2;
	corners[3] = Center - axis0 + axis1 - axis2;
	corners[4] = Center - axis0 - axis1 + axis2;
	corners[5] = Center + axis0 - axis1 + axis2;
	corners[6] = Center + axis0 + axis1 + axis2;
	corners[7] = Center - axis0 + axis1 + axis2;
}

void OBB::Transform(const Matrix4& transform)
{
	Center = Vector3::Transform(Center, transform);
	Rotation = Rotation * Matrix3::CreateFromMatrix4(transform);
}

}
